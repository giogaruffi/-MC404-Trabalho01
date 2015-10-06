#include <stdio.h>
#include <string.h>
#include <stdlib.h>

 typedef struct endereco{
	int end, pos; /*0 esquerda 1 direita*/
 } endereco;

 typedef struct rotulo{
	char nome[100];
	endereco endereco;
	struct rotulo *prox;
 } rotulo;
 
 typedef struct palavras{
	int num_linha, pal_esq,  pal_dir, pal_int;
	char inst_esq[2], inst_dir[2];
	struct palavras *prox;  
 } palavra;
 
 endereco muda_Endereco(endereco *end_);
 void preencher_mapa(palavra *inicio, endereco *end_, char instrucao[2], char *token);
 
 /* Funcoes de Listas Ligadas para o Mapa de Memoria */
 void InicializaMapaMemoria(palavra *inicio);
 void InserePalavraMapa(palavra *inicio, int valor, int linha);
 void LiberaMapaPalavras(palavra *inicio);
 void ImprimePalavras(palavra *inicio);

 /* Funcoes de Listas Ligadas para o Mapa de Rotulos */
 void InicializaMapaRotulos(rotulo *inicio);
 void InsereRotuloFim(rotulo *inicio, char *token, endereco end);
 void LiberaRotulos(rotulo *inicio); 
 void ImprimeRotulos(rotulo *inicio);
 endereco ProcuraRotulo(rotulo *inicio, char *token);

 int main(int argc, char *argv[]){
	FILE *arq_entrada; /* associado ao arquivo de entrada */
	FILE *arq_saida; /* associado ao arquivo de saida */
	char delim[] = " \n";/*delimitadores dos tokens*/
	char str[1001];/*string referente a cada linha*/
	char *token;/*string referente a cada token*/
	int tam;
	rotulo mapa_rot;	
	palavra mapa_mem;
	endereco end = {0,0};
	
	arq_entrada = fopen(argv[1],"r");/*abre o arquivo para leitura*/

	arq_saida = fopen(argv[2],"w");/*abre o arquivo para escrita*/

	InicializaMapaRotulos(&mapa_rot);
	InicializaMapaMemoria(&mapa_mem);

	/* PRIMEIRA LEITURA DO ARQUIVO - MAPA DE ROTULOS */
	while(!feof(arq_entrada)){	
		/* Carrega o primeiro token da linha de entrada */
		token = strtok(fgets(str,1000,arq_entrada),delim);
		while(token != NULL){		
		  tam = strlen(token);
		  /* Teste se eh diretiva (primeiro caracter eh '.' */		
		  if(token[0] == '.'){ 
		  }
		  /* Teste se eh roluto (ultimo caracter eh ':' */
		  else if(token[tam-1] == ':'){
		    InsereRotuloFim(&mapa_rot,token,end);
		  }
		  /* Teste se eh comentario (primeiro caracter eh '#' */
		  else if(token[0] == '#'){
		    /* Percorre ate o fim da linha */		
		    token = strtok(NULL,"\n");
		  }else{
		  	preencher_mapa(&mapa_mem,&end,"AA","0x10");
		  	end = muda_Endereco(&end);
		  	preencher_mapa(&mapa_mem,&end,"BB","0x01");
		  	end = muda_Endereco(&end);
		  }
		  token = strtok(NULL, delim);
		}
   	}
   	
   	ImprimeRotulos(&mapa_rot);  	
   	ImprimePalavras(&mapa_mem);
 	
 	LiberaMapaPalavras(&mapa_mem);
   	LiberaRotulos(&mapa_rot);

	fclose(arq_entrada);/*fecha o arquivo de entrada*/

	fclose(arq_saida);/*fecha o arquivo de saida*/
    

 return 0;
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
	palavra *novo,*temp, *aux;
	int flag = 0;

    temp = inicio;
    while(temp->prox!=NULL && temp->num_linha != end_->end){
		temp = temp->prox;
    }
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
    }else{
    		novo = (palavra*)malloc(sizeof(palavra));
	  		novo->num_linha = end_->end;
	  		temp->pal_int = -1;
	  	if(end_->pos == 1){
	  		novo->pal_dir = (int)strtol(token, NULL, 0);
			strcpy(novo->inst_dir,instrucao);
	  		strcpy(novo->inst_esq,"00");
	  	}else{
	  		novo->pal_esq = (int)strtol(token, NULL, 0);
			strcpy(novo->inst_esq,instrucao);
	  		strcpy(novo->inst_dir,"00");
	  	}
	  	novo->prox=NULL;
	  	temp->prox = novo;   
    	
 		/* Insere na posicao do mapa em ordem */
  		temp = inicio;
    	if(temp->prox == NULL)
    		temp->prox = novo;
    	else{	
      		while(temp->prox!=NULL && !flag){
    			if(temp->prox->num_linha >= novo->num_linha)
	  				flag = 1;
				else
	  				temp = temp->prox;
      		}	    
      		if(!flag)
				temp->prox = novo;
      		else{
      			if(temp->prox->num_linha == novo->num_linha){
      				novo->prox = temp->prox->prox;
      				temp->prox = novo;
      			}else{
					aux = temp->prox;
					temp->prox = novo;
					novo->prox = aux;
      			}
      		}
      	}
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
 
 void InserePalavraMapa(palavra *inicio, int valor, int linha) {
	palavra *novo,*temp,*aux;
	int tam, flag = 0;

	novo = (palavra*)malloc(sizeof(palavra));
    novo->num_linha = linha;
  	novo->pal_int = valor;
  	novo->pal_esq = -1;
  	novo->pal_dir = -1;
    novo->prox = NULL;

	/* Insere na posicao do mapa em ordem */
    temp = inicio;
    if(temp->prox == NULL)
    	temp->prox = novo;
    else{	
      	while(temp->prox!=NULL && !flag){
    		if(temp->prox->num_linha >= novo->num_linha)
	  			flag = 1;
			else
	  			temp = temp->prox;
      	}	    
      	if(!flag)
			temp->prox = novo;
      	else{
      		if(temp->prox->num_linha == novo->num_linha){
      			novo->prox = temp->prox->prox;
      			temp->prox = novo;
      		}else{
			aux = temp->prox;
			temp->prox = novo;
			novo->prox = aux;
      		}
      	}
    }
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
	  printf("lin %d inst_esq %c%c pal_esq %d inst_dir %c%c pal_dir %d\n", temp->num_linha, temp->inst_esq[0], temp->inst_esq[1], temp->pal_esq, temp->inst_dir[0], temp->inst_dir[1], temp->pal_dir);
	}
 }


 /* IMPLEMENTAÇÃO DAS FUNÇÕES DE LISTA LIGADA */ 
 void InicializaMapaRotulos(rotulo *inicio) {
	inicio->endereco.end = -1;
	inicio->endereco.pos = -1;
	inicio->prox = NULL;
 }

 void InsereRotuloFim(rotulo *inicio, char *token, endereco end) {
	rotulo *novo,*temp;

	novo = (rotulo*)malloc(sizeof(rotulo));
    novo->endereco = end;
    strcpy(novo->nome,token);
    novo->prox=NULL;

    temp = inicio;
    while(temp->prox!=NULL){
	    temp = temp->prox;
	}
	   temp->prox = novo;

 }

 void LiberaRotulos(rotulo *inicio) {
	rotulo *temp;

	for(temp = inicio->prox; temp != NULL; temp = temp-> prox){
		free(temp);
	}
}

 void ImprimeRotulos(rotulo *inicio){
 	rotulo *temp;
 	
	for(temp = inicio->prox; temp != NULL; temp = temp->prox){
		printf("%s", temp->nome);
		printf("no endereco %d e posicao %d", temp->endereco.end, temp->endereco.pos);
		printf("\n");
	}
}

 endereco ProcuraRotulo(rotulo *inicio, char *token){
 	rotulo *temp;
 
	for(temp = inicio->prox; temp != NULL; temp = temp->prox){
        if (!strcmp(temp->nome, token)){
	  		return temp->endereco;
        }
        temp = temp->prox;
    }
	return;
}
