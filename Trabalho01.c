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
	int num_linha, pal_esq,  pal_dir;
	char inst_esq[2], inst_dir[2];
	struct palavras *prox;  
 } palavra;
 
 typedef struct nome{
 	char carac[100];
 	int valor;
 	struct nome *prox;
 } nome;
  
 /* Funcoes de reconhecimento de elementos de linguagem de montagem*/
 void diretivas(char *str, endereco *end, palavra *mapa, nome *lista);
 endereco muda_Endereco(endereco *end_);
 void preencher_mapa(palavra *inicio, endereco *end_, char instrucao[2], char *token);
 void OrdenaMapa(palavra *inicio, palavra *novo);
 void instrucoes(char str[], endereco *end, palavra *mapa, rotulo *rot);
 
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
 
 /* Funcoes de Listas Ligadas para os nomes setados */
 void InicializaNomes(nome *inicio);
 void InsereNomeFim(nome *inicio, char *token, int valor);
 void LiberaNomes(nome *inicio);
 void ImprimeNomes(nome *inicio);
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
	
	/* Inicializa mapa dos rotulos */
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
			/* TESTAR SE EH NECESSARIO INTERPRETAR INSTRUCOES NA PRIMEIRA LEITURA */
			else{
				instrucoes(token, end, &mapa_mem, &mapa_rot);	
			}
			token = strtok(NULL, delim);
		}
   	}
	
	/* Volta ao inicio do arquivo e reseta o contador de posicao */
	fseek(arq_entrada, 0, SEEK_SET);
	end.endereco = 0;
	end.pos = 0;

	/* SEGUNDA LEITURA DO ARQUIVO - MAPA DE MEMORIA */
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
			/* Teste se eh comentario (primeiro caracter eh '#' */
			else if(token[0] == '#'){
				/* Percorre ate o fim da linha */		
				token = strtok(NULL,"\n");		
			}
			/* Se nao eh nenhum desses, eh uma instrucao */
			else{
				instrucoes(token, end, &mapa_mem, &mapa_rot);	
			}
		token = strtok(NULL, delim);
		}
   	}

	/* Percorrer mapa de memoria e preencher vazios com zeros */
	preencher_zeros(&mapa_mem);

	/* Escrever arquivo de saida */
	hexadecimal(&mapa_mem);

   	LiberaRotulos(&mapa_rot);
   	LiberaMapaPalavras(&mapa_mem);
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
	  	novo->prox = NULL;
	  	temp->prox = novo;   
  
  		OrdenaMapa(inicio,novo);
    }
 }
 
 void OrdenaMapa(palavra *inicio, palavra *novo){
 	palavra *temp, *aux;
 	int flag = 0;
 
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

 //int verifica(char *str){
   //  char c = str[0];

 //if(strcmp(c ,".") == 0){
 //eh rotulo, ver como esta as funcoes de rotulo
 //}

 //else{
   //return strtol(str,NULL, 16);
 //}
//}

void instrucoes(char str[], endereco *end, palavra *mapa, rotulo *rot){

    char *token;
    char *aux;

    if(strcmp(str,  "LDMQ") == 0){
            end = muda_Endereco(end);
            token = strtok(str,"()");
            preencher_mapa(end,mapa,"0A","000");
    }
    else if(strcmp(str,  "LDMQM") == 0){
            end = muda_Endereco(end);
            token = strtok(str,"()");
            preencher_mapa(end,mapa,"09",token);
    }
    else if(strcmp(str,  "STR") == 0){
            end = muda_Endereco(end);
            token = strtok(str,"()");
            preencher_mapa(end,mapa,"21",token);
    }
    else if(strcmp(str,  "LOAD") == 0){
            end = muda_Endereco(end);
            token = strtok(str,"()");
            preencher_mapa(end,mapa,"01",token);
    }
    else if(strcmp(str,  "LDN") == 0){
            end = muda_Endereco(end);
            token = strtok(str,"()");
            preencher_mapa(end,mapa,"02",token);
    }
    else if(strcmp(str,  "LDABS") == 0){
            end = muda_Endereco(end);
            token = strtok(str,"()");
            preencher_mapa(end,mapa,"03",token);
    }
    else if(strcmp(str,  "JMP") == 0){
//verificar esquerda e direita
            end = muda_Endereco(end);
            token = strtok(str,"(,");
            aux = strtok(NULL,")");
            if(strcmp(aux,"0:19") == 0){
               preencher_mapa(end,mapa,"0D", token);
            }
            else if(strcmp(aux,"20:39") == 0){
               preencher_mapa(end,mapa,"0E", token);
            }

    }
    else if(strcmp(str,  "JGEZ") == 0){
    //verificar direita e esquerda
            end = muda_Endereco(end);
            token = strtok(str,"()");
            aux = strtok(NULL,")");
            if(strcmp(aux,"0:19") == 0){
               preencher_mapa(end,mapa,"0F", token);
            }
            else if(strcmp(aux,"20:39") == 0){
               preencher_mapa(end,mapa,"10", token);
            }
    }
    else if(strcmp(str,  "ADD") == 0){
            end = muda_Endereco(end);
            token = strtok(str,"()");
            preencher_mapa(end,mapa,"05",token);
    }
    else if(strcmp(str,  "ADDABS") == 0){
            end = muda_Endereco(end);
            token = strtok(str,"()");
            preencher_mapa(end,mapa,"07",token);
    }
    else if(strcmp(str,  "SUB") == 0){
            end = muda_Endereco(end);
            token = strtok(str,"()");
            preencher_mapa(end,mapa,"06",token);
    }
    else if(strcmp(str,  "SUBABS") == 0){
            end = muda_Endereco(end);
            token = strtok(str,"()");
            preencher_mapa(end,mapa,"08",token);
    }
    else if(strcmp(str,  "MUL") == 0){
            end = muda_Endereco(end);
            token = strtok(str,"()");
            preencher_mapa(end,mapa,"0B",token);
    }
    else if(strcmp(str,  "DIV") == 0){
            end = muda_Endereco(end);
            token = strtok(str,"()");
            preencher_mapa(end,mapa,"0C",token);
    }
    else if(strcmp(str,  "LSH") == 0){
            end = muda_Endereco(end);
            token = strtok(str,"()");
            preencher_mapa(end,mapa,"14","000");
    }
    else if(strcmp(str,  "RSH") == 0){
            end = muda_Endereco(end);
            token = strtok(str,"()");
            preencher_mapa(end,mapa,"15","000");
    }
    else if(strcmp(str,  "STM") == 0){
        //se operando for instrcao a direita ou a esquerda.
            end = muda_Endereco(end);
            token = strtok(str,"()");
            aux = strtok(NULL,")");
            if(strcmp(aux,"8:19") == 0){
               preencher_mapa(end,mapa,"12", token);
            }
            else if(strcmp(aux,"28:39") == 0){
               preencher_mapa(end,mapa,"13", token);
            }
    }

}
 /*------------------------------- NAO ALTERAR POR ENQUANTO ------------------------------*/

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
		if(temp->pal_int == -1)
			printf("lin %d inst_esq %c%c pal_esq %d inst_dir %c%c pal_dir %d\n", temp->num_linha, temp->inst_esq[0], temp->inst_esq[1], temp->pal_esq, temp->inst_dir[0], temp->inst_dir[1], temp->pal_dir);
		else
			printf("lin %d pal %d\n", temp->num_linha, temp->pal_int);
	}
 }

 /* IMPLEMENTAÇÃO DAS FUNÇÕES DE LISTA LIGADA DE ROTULOS */ 
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
