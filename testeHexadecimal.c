#include <stdio.h>
#include <string.h>
#include <stdlib.h>

 typedef struct endereco{
	int end, pos; /*0 esquerda 1 direita*/
 } endereco;

 typedef struct palavras{
	int num_linha, pal_esq,  pal_dir, pal_int;
	char inst_esq[3], inst_dir[3];
	struct palavras *prox;  
 } palavra;
 
 void hexadecimal(palavra *inicio, FILE *saida);
 void diretivas(char *str, endereco *end, palavra *mapa);
 /* Funcoes de Listas Ligadas para o Mapa de Memoria */
 void InicializaMapaMemoria(palavra *inicio);
 void InserePalavraMapa(palavra *inicio, int valor, int linha);
 void LiberaMapaPalavras(palavra *inicio);
 void preencher_mapa(palavra *inicio, endereco *end_, char instrucao[2], char *token);
 void OrdenaMapa(palavra *inicio);
 void ImprimePalavras(palavra *inicio);
 endereco muda_Endereco(endereco *end_);
 
 int main(int argc, char *argv[]){
	FILE *arq_entrada; /* associado ao arquivo de entrada */
	FILE *arq_saida; /* associado ao arquivo de saida */
	palavra mapa_mem; /* mapa de memoria */
 	endereco end = {0,0}; /* ponteiro de endereco */
	
	arq_entrada = fopen(argv[1],"r");/*abre o arquivo para leitura*/

	arq_saida = fopen(argv[2],"w");/*abre o arquivo para escrita*/
	
	/* Inicializa mapa dos rotulos */
	InicializaMapaMemoria(&mapa_mem);
	
	/* Construir Mapa de Memoria hipotetico */
	InserePalavraMapa(&mapa_mem, 1, 0);
	end.end++;
 	preencher_mapa(&mapa_mem, &end, "00", "0x0");
 	end.pos = 1;
 	preencher_mapa(&mapa_mem, &end, "01", "10");
 	end.end++;
 	preencher_mapa(&mapa_mem, &end, "00", "10");
 	end.end = 100;
 	end.pos = 0;
 	preencher_mapa(&mapa_mem, &end, "10", "10");
 	
 	diretivas(".org",&end,&mapa_mem);
 	preencher_mapa(&mapa_mem, &end, "11", "12");
	
 	diretivas(".align",&end,&mapa_mem);
 	preencher_mapa(&mapa_mem, &end, "12", "2");
	
	OrdenaMapa(&mapa_mem);
        
	ImprimePalavras(&mapa_mem);
	
	/* Escrever arquivo de saida */
    hexadecimal(&mapa_mem,arq_saida);

   	LiberaMapaPalavras(&mapa_mem);
   	
	fclose(arq_entrada);/*fecha o arquivo de entrada*/

	fclose(arq_saida);/*fecha o arquivo de saida*/

 	return 0;
 }
 
 void hexadecimal(palavra *mapa, FILE *saida){
 	char linha[5], pal_int[11], pal_esq[4], pal_dir[4];
 	palavra *temp;

	for(temp = mapa->prox; temp != NULL; temp = temp->prox){
		sprintf(&linha[0], "%04X", temp->num_linha);
		fprintf(saida,"%s ",linha);
		if(temp->pal_int != -1){
			sprintf(&pal_int[0], "%010X", temp->pal_int);
			fprintf(saida,"%c%c", pal_int[0], pal_int[1]);
			fprintf(saida," %c%c%c ", pal_int[2], pal_int[3], pal_int[4]);
			fprintf(saida,"%c%c", pal_int[5], pal_int[6]);
			fprintf(saida," %c%c%c\n", pal_int[7], pal_int[8], pal_int[9]);
		}
		else{
			fprintf(saida,"%s ", temp->inst_esq);
			sprintf(&pal_esq[0], "%03X", temp->pal_esq);
			fprintf(saida,"%s ",pal_esq);
			fprintf(saida,"%s ", temp->inst_dir);
			sprintf(&pal_dir[0], "%03X", temp->pal_dir);
			fprintf(saida,"%s\n",pal_dir);
 		}
 	}
 }
 
 void diretivas(char *str, endereco *end, palavra *mapa){
 	char aux[101];
 	int aux2;
         
         
    if (!strcmp(str, ".org")){
    	strcpy(aux,"0x10");
		(*end).end = (int)strtol(aux, NULL, 0);
		(*end).pos = 0;
    }
    else if (!strcmp(str, ".align")){
    	strcpy(aux,"0x01");
   		aux2 = (int)strtol(aux, NULL, 0);
   		(*end).end = (((*end).end/aux2) + 1) * aux2;
	}
 }
 
  /* IMPLEMENTAÇÃO DAS FUNÇÕES DE LISTA LIGADA MAPA MEMORIA */ 
 void InicializaMapaMemoria(palavra *inicio) {
	
	inicio->pal_esq = -1;
	inicio->pal_dir = -1;
	inicio->pal_int = -1;
	inicio->num_linha = -1;
	strcpy(inicio->inst_esq,"00");
	strcpy(inicio->inst_dir,"00");
    
	inicio->prox = NULL;
 } 

 endereco muda_Endereco(endereco *end_){

    endereco aux;
    aux.end = end_->end;

    
    if(end_->pos == 0){
        end_->pos = 1;
    }

    else if(end_->pos == 1){
      end_->end++;
      end_->pos = 0;
    }

	return (*end_);
 }
 
   
 void preencher_mapa(palavra *inicio, endereco *end_, char instrucao[2], char *token){
	palavra *novo,*temp;

	/* Percorre a lista ate o fim ou ate encontrar a linha a ser preenchida */
    temp = inicio;
    while(temp->prox!=NULL && temp->num_linha != end_->end){
		temp = temp->prox;
    }
    /* Caso a linha ja exista, deve-se sobrescreve-la */
    if(temp->num_linha == end_->end){
		if(end_->pos == 1){
			temp->pal_dir = (int)strtol(token, NULL, 0);
			temp->pal_int = -1;
			strcpy(temp->inst_dir,instrucao);
    	}else{
			temp->pal_esq = (int)strtol(token, NULL, 0);
			temp->pal_int = -1;
			strcpy(temp->inst_esq,instrucao);
    	}
    /* Caso contrario, cria-se uma nova */
    }else{
    		novo = (palavra*)malloc(sizeof(palavra));
	  		novo->num_linha = end_->end;
	  		novo->pal_int = -1;
	  	if(end_->pos == 1){
	  		novo->pal_dir = (int)strtol(token, NULL, 0);
			novo->pal_esq = 0;
	  		strcpy(novo->inst_esq,"00");
			strcpy(novo->inst_dir,instrucao);
	  	}else{
	  		novo->pal_esq = (int)strtol(token, NULL, 0);
			novo->pal_dir = 0;
	  		strcpy(novo->inst_dir,"00");
			strcpy(novo->inst_esq,instrucao);
	  	}
	  	novo->prox = NULL;
	  	temp->prox = novo;   
    }
 }
 
 void OrdenaMapa(palavra *inicio){
 	palavra *temp, *temp2, *aux, *aux2, *pos;
 	int num, teste;
 	 
 	for(temp = inicio->prox, temp2 = inicio->prox, pos = inicio->prox; temp->prox != NULL; temp = temp->prox){
        num = temp->num_linha;
        pos = temp;
        for(aux = temp; aux->prox != NULL; aux = aux->prox){
            teste = (*(aux->prox)).num_linha;
            if(teste < num){
                num = teste;
                pos = aux;
            }
        }
        if(num != temp->num_linha){
            aux2 = pos->prox;
            if(aux2!= NULL)
                pos->prox = aux2->prox;
            else
                pos->prox = NULL;
            if(pos != temp){
                aux2->prox = temp->prox;
                temp->prox = pos->prox;
                pos->prox = temp;
            }
            else
                aux2->prox = temp;
 
            if(temp == inicio->prox)
                inicio->prox = aux2;
            else
                temp2->prox = aux2;
            temp = aux2;
        }
        temp2 = temp;
    }
 }
 
 void InserePalavraMapa(palavra *inicio, int valor, int linha) {
	palavra *novo,*temp,*aux;
	int tam, flag = 0;

	novo = (palavra*)malloc(sizeof(palavra));
    novo->num_linha = linha;
  	novo->pal_int = valor;
  	novo->pal_esq = -1;
  	novo->pal_dir = -1;
	strcpy(novo->inst_esq,"\0");
	strcpy(novo->inst_dir,"\0");
    novo->prox = NULL;
    
    /* Percorre lista ate o fim e adiciona o noh */
    temp = inicio;
    while(temp->prox != NULL)
    	temp = temp->prox;
    temp->prox = novo;
 }

 void LiberaMapaPalavras(palavra *inicio) {

   palavra *temp;

	for(temp = inicio->prox; temp != NULL; temp = temp-> prox){
		free(temp);
	}
 }

 void ImprimePalavras(palavra *inicio){
 	palavra *temp;
 	
	for(temp = inicio->prox; temp != NULL; temp = temp->prox){
		if(temp->pal_int == -1)
			printf("lin %d inst_esq %c%c pal_esq %d inst_dir %c%c pal_dir %d\n", temp->num_linha, temp->inst_esq[0], temp->inst_esq[1], temp->pal_esq, temp->inst_dir[0], temp->inst_dir[1], temp->pal_dir);
		else
			printf("lin %d pal %d\n", temp->num_linha, temp->pal_int);
	}
 }
