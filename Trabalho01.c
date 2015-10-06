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
 
 /* Funcoes de Listas Ligadas para o Mapa de Rotulos */
 void InicializaMapaRotulos(rotulo *inicio);
 void InsereRotuloFim(rotulo *inicio, char *token, endereco end);
 void ImprimeRotulos(rotulo *inicio);
 void LiberaRotulos(rotulo *inicio); 
 /* Funcoes de Listas Ligadas para o Mapa de Memoria */
 void InicializaMapaMemoria(palavra *inicio);
 void InserePalavraFim(palavra *inicio, char *token, endereco end);
/* void ImprimePalavras(palavra *inicio);*/
 void LiberaMapaPalavras(palavra *inicio);
 void preencher_mapa(palavra *inicio, endereco *end_, char instrucao[2], char *token);
 void ImprimePalavras(palavra *inicio);
 endereco muda_Endereco(endereco *end_);
 
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
	
	/* Inicializa mapa dos rotulos e memoria */
	InicializaMapaRotulos(&mapa_rot);
	InicializaMapaMemoria(&mapa_mem);

	/* PRIMEIRA LEITURA DO ARQUIVO - MAPA DE ROTULOS */
	while(!feof(arq_entrada)){	
		/* Carrega o primeiro token da linha de entrada */
		token = strtok(fgets(str,1000,arq_entrada),delim);
		while(token != NULL){		
			/* Calcula tamanho do token */
			tam = strlen(token);						
			/* Teste se eh diretiva (primeiro caracter eh '.' */		
			if(token[0] == '.'){
			/*  diretivas(token, end, mapa);*/	
			}
			/* Teste se eh roluto (ultimo caracter eh ':' */
			else if(token[tam-1] == ':'){
		    	InsereRotuloFim(&mapa_rot,token,end);
			}
			/* Teste se eh comentario (primeiro caracter eh '#' */
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
			/* Teste se eh diretiva (primeiro caracter eh '.' */		
			if(token[0] == '.'){
				diretivas(token, end, mapa);	
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
	preencher_zeros(mapa_mem);

	/* Escrever arquivo de saida */
	hexadecimal(mapa_mem);

	ImprimeRotulos(&mapa_rot);
   	ImprimePalavras(&mapa_mem); 	
   	
   	LiberaRotulos(&mapa_rot);
   	LiberaMapaPalavras(&mapa_mem);
   	
	fclose(arq_entrada);/*fecha o arquivo de entrada*/

	fclose(arq_saida);/*fecha o arquivo de saida*/
    

 return 0;
}


 void diretivas(char *str, endereco *end, palavra *mapa){
 	char delim = " ,\n";/*delimitadores dos tokens*/
 	char *token;
 	int aux, aux2, i;
      
 	switch (token){
 		case ".org":
 			token = strtok(str, delim);
        		end.endereco = (int)strtol(token, NULL, 0);
        		end.pos = 0;
        	break;
         
        	case ".align":
        		token = strtok(str, delim);
        		aux = (int)strtol(token, NULL, 0);
        		end.endereco = ((end.endereco/aux) + 1) * aux;
        	break;
         
		case ".wfill":
			token = strtok(str, delim); /* Carrega Num de linhas que devem serem preenchidas */
			aux = (int)strtol(token, NULL, 0); 
			token = strtok(NULL,delim); /* Carrega o Valor q deve ser colocado nas linhas */
			aux2 = (int)strtol(token, NULL, 0);
		/* Adiciona na lista do mapa de memoria N elementos de valor D */
		for(i = 0;i < aux; i++){
			adicionar_lista(mapa, aux2, end.endereco);
			end.endereco++;
		}
		break;
        
		case ".word":
	
		break;
         
		case ".set":
		break;
	}
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

    /* Teste se a palavra a ser inserida eh na esquerda */
    if(end_->pos == 0){
    	novo = (palavra*)malloc(sizeof(palavra));
    	novo->num_linha = end_->end;
    	novo->pal_esq = (int)strtol(token, NULL, 0);
    	novo->pal_dir = 0;
    	strcpy(novo->inst_esq,instrucao);		
    	strcpy(novo->inst_dir,"00");
    	novo->prox=NULL;
    	
    	/* Percorre a lista ate o fim e insere o novo noh */
    	temp = inicio;
   		while(temp->prox!=NULL){
			temp = temp->prox;
		}
			temp->prox = novo;
	/* Caso de ser inserida na direita, testar primeiro se o noh realmente existe */
    }else if(end_->pos == 1){	
    	temp = inicio;
    	while(temp->prox!=NULL || temp->num_linha != end_->end){
			temp = temp->prox;
      	}
      	if(temp->num_linha == end_->end){
			temp->num_linha = end_->end;
			temp->pal_dir = (int)strtol(token, NULL, 0);
			strcpy(temp->inst_dir,instrucao);
      	}else{
	  		novo = (palavra*)malloc(sizeof(palavra));
	  		novo->num_linha = end_->end;
	  		novo->pal_dir = (int)strtol(token, NULL, 0);
	  		strcpy(novo->inst_dir,instrucao);
	  		novo->prox=NULL;
	  		temp->prox = novo;
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
