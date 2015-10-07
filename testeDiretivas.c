#include <stdio.h>
#include <string.h>
#include <stdlib.h>

 typedef struct endereco{
	int end, pos; /*0 esquerda 1 direita*/
 } endereco;

 typedef struct rotulo{
	char nome[101];
	endereco endereco;
	struct rotulo *prox;
 } rotulo;
 
 typedef struct palavras{
	int num_linha, pal_esq,  pal_dir, pal_int;
	char inst_esq[3], inst_dir[3];
	struct palavras *prox;  
 } palavra;
 
 typedef struct nome{
 	char carac[101];
 	int valor;
 	struct nome *prox;
 } nome;
 
 /* Funcoes de reconhecimento de elementos de linguagem de montagem*/
 void diretivas(char *str, endereco *end, palavra *mapa, nome *lista);

  /* Funcoes de Listas Ligadas adaptadas para o montador */
 void InicializaMapaRotulos(rotulo *inicio);
 void InsereRotuloFim(rotulo *inicio, char *token, endereco end);
 void ImprimeRotulos(rotulo *inicio);
 void LiberaRotulos(rotulo *inicio);
 endereco ProcuraRotulo(rotulo *inicio, char *token);
 /* Funcoes de Listas Ligadas para o Mapa de Memoria */
 void InicializaMapaMemoria(palavra *inicio);
 void InserePalavraMapa(palavra *inicio, int valor, int linha);
 void LiberaMapaPalavras(palavra *inicio);
 void preencher_mapa(palavra *inicio, endereco *end_, char instrucao[2], char *token);
 void OrdenaMapa(palavra *inicio);
 void ImprimePalavras(palavra *inicio);
 endereco muda_Endereco(endereco *end_);
 /* Funcoes de Listas Ligadas para os nomes setados */
 void InicializaNomes(nome *inicio);
 void InsereNomeFim(nome *inicio, char *token, int valor);
 void ImprimeNomes(nome *inicio);
 void LiberaNomes(nome *inicio);
 int ProcuraNome(nome *inicio, char *token);
 
 
  int main(int argc, char *argv[]){
	FILE *arq_entrada; /* associado ao arquivo de entrada */
	FILE *arq_saida; /* associado ao arquivo de saida */
	char delim[] = " \n";/*delimitadores dos tokens*/
	char str[1001];/*string referente a cada linha*/
	char *token;/*string referente a cada token*/
	int tam; /* auxiliar */
	palavra mapa_mem; /* mapa de memoria */
	rotulo mapa_rot; /* mapa de rotulos */
	nome lista_nom; /* lista de nomes */
 	endereco end = {0,0}; /* ponteiro de endereco */
	
	arq_entrada = fopen(argv[1],"r");/*abre o arquivo para leitura*/

	arq_saida = fopen(argv[2],"w");/*abre o arquivo para escrita*/
	
	/* Inicializa mapa dos rotulos e memoria */
	InicializaMapaRotulos(&mapa_rot);
	InicializaMapaMemoria(&mapa_mem);
	InicializaNomes(&lista_nom);

	/* PRIMEIRA LEITURA DO ARQUIVO - MAPA DE ROTULOS */
	while(!feof(arq_entrada)){	
		/* Carrega o primeiro token da linha de entrada */
		token = strtok(fgets(str,1000,arq_entrada),delim);
		while(token != NULL){		
			/* Calcula tamanho do token */
			tam = strlen(token);						
			/* Teste se eh diretiva (primeiro caracter eh '.') */		
			if(token[0] == '.'){
				diretivas(token, &end, &mapa_mem, &lista_nom);
			}
			/* Teste se eh roluto (ultimo caracter eh ':') */
			else if(token[tam-1] == ':'){
				InsereRotuloFim(&mapa_rot,token,end);
			}
			/* Teste se eh comentario (primeiro caracter eh '#') */
			else if(token[0] == '#'){
				/* Percorre ate o fim da linha */		
				token = strtok(NULL,"\n");		
			}
			/* TESTAR SE EH NECESSARIO INTERPRETAR INSTRUCOES NA PRIMEIRA LEITURA 
			else{
				instrucoes(token, end, mapa, mapa_rot);	
			}*/
			token = strtok(NULL, delim);
		}
   	}
 
	OrdenaMapa(&mapa_mem);
        
   	
   	printf("Mapa rotulos\n");
	ImprimeRotulos(&mapa_rot);  	
   	printf("Mapa Memoria\n");
   	ImprimePalavras(&mapa_mem);
   	printf("Lista Nomes\n");
   	ImprimeNomes(&lista_nom);
 	
 	LiberaMapaPalavras(&mapa_mem);
   	LiberaRotulos(&mapa_rot);
	LiberaNomes(&lista_nom);

	fclose(arq_entrada);/*fecha o arquivo de entrada*/

	fclose(arq_saida);/*fecha o arquivo de saida*/
    

	return 0;
 }
 
 
 void diretivas(char *str, endereco *end, palavra *mapa, nome *lista){
 	char delim[] = " ,\n";/*delimitadores dos tokens*/
 	char *token, aux3[100];
 	int aux, aux2, i;
        
    if (!strcmp(str, ".org")){
      token = strtok(NULL, delim);
      (*end).end = (int)strtol(token, NULL, 0);
      (*end).pos = 0;
    }
         
    else if (!strcmp(str, ".align")){
  		token = strtok(NULL, delim);
   		aux = (int)strtol(token, NULL, 0);
   		(*end).end = (((*end).end/aux) + 1) * aux;
	}

    else if (!strcmp(str, ".wfill")){
  		token = strtok(NULL, delim); /* Carrega Num de linhas que devem serem preenchidas */
		aux = (int)strtol(token, NULL, 0); 
		token = strtok(NULL,delim); /* Carrega o Valor q deve ser colocado nas linhas */
		if(lista->prox != NULL){
			aux2 = ProcuraNome(lista,token); /* Testa se nao eh um nome */
			if(aux2 == 1024){
				aux2 = (int)strtol(token, NULL, 0);
			}
		}
		/*Adiciona na lista do mapa de memoria N elementos de valor D */
		for(i = 0; i < aux; i++){
			InserePalavraMapa(mapa, aux2, (*end).end);
			(*end).end++;
		}
	}
		      	
    else if (!strcmp(str, ".word")){
		token = strtok(NULL, delim);
		if(lista->prox != NULL){
			aux = ProcuraNome(lista,token); /* Testa se nao eh um nome */
			if(aux == 1024){
				aux = (int)strtol(token, NULL, 0);
			}
		}
		InserePalavraMapa(mapa, aux, (*end).end);
		(*end).end++;
	}
    else if (!strcmp(str, ".set")){  
		token = strtok(NULL, delim); /* Carrega os caracteres do nome a ser setado */
		strcpy(aux3,token);
		token = strtok(NULL, delim); /* Carrega valor a ser associado ao nome */
		aux = (int)strtol(token, NULL, 0);
		/* Insere nomes na lista */
		InsereNomeFim(lista,aux3,aux);
	}
	
	printf("end: %d e pos: %d\n", (*end).end,(*end).pos);

 }

 /* IMPLEMENTAÇÃO DAS FUNÇÕES DE LISTA LIGADA MAPA DE ROTULOS */ 
 void InicializaMapaRotulos(rotulo *inicio) {
	inicio->endereco.end = -1;
	inicio->endereco.pos = -1;
	inicio->prox = NULL;
 }

 void InsereRotuloFim(rotulo *inicio, char *token, endereco end) {
	rotulo *novo,*temp;
	int tam;

	novo = (rotulo*)malloc(sizeof(rotulo));
    novo->endereco = end;
    strcpy(novo->nome,token);
    tam = strlen(token);
    novo->nome[tam-1] = '\0';
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
		printf("%s na linha %d e posicao %d\n", temp->nome, temp->endereco.end, temp->endereco.pos);
	}
 }

 endereco ProcuraRotulo(rotulo *inicio, char *token){
 	rotulo *temp;
	endereco aux;
 
	for(temp = inicio->prox; temp != NULL; temp = temp->prox){
	  if (!strcmp(temp->nome, token)){
	  		return temp->endereco;
	  }
      	}
	aux.end = 1024;
	
	return aux;
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
 
 /* IMPLEMENTAÇÃO DAS FUNÇÕES DE LISTA LIGADA NOMES */ 
 void InicializaNomes(nome *inicio){
 	inicio->valor = -1;
	strcpy(inicio->carac,"00");
	inicio->prox = NULL;
 }
 
 void InsereNomeFim(nome *inicio, char *token, int valor){
 	nome *novo,*temp;
	int tam;

	novo = (nome*)malloc(sizeof(nome));
    novo->valor = valor;
    strcpy(novo->carac,token);
    novo->prox=NULL;

    temp = inicio;
    while(temp->prox!=NULL){
	    temp = temp->prox;
	}
	   temp->prox = novo;
 }
 
  void LiberaNomes(nome *inicio){	
	nome *temp;

	for(temp = inicio->prox; temp != NULL; temp = temp-> prox){
		free(temp);
	}
 }
 
 void ImprimeNomes(nome *inicio){
 	nome *temp;
 	
	for(temp = inicio->prox; temp != NULL; temp = temp->prox){
		printf("%s valor %d\n", temp->carac, temp->valor);
	}
 }
  
 int ProcuraNome(nome *inicio, char *token){
 	nome *temp;
 
	for(temp = inicio->prox; temp != NULL; temp = temp->prox){
	  if (!strcmp(temp->carac, token)){
	  		return temp->valor;
	  }
	}

	return 1024;
 }
