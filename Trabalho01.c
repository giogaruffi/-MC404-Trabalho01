#include <stdio.h>
#include <string.h>
#include <stdlib.h>


 typedef struct endereco{
	int end, pos; /*0 esquerda 1 direita*/
 } endereco;

 typedef struct palavras{
	int num_linha, inst_esq, pal_esq, inst_dir, pal_dir;  
 } palavra;

 typedef struct rotulo{
	char nome[100];
	endereco endereco;
	struct rotulo *prox;
 } rotulo;

 /* Funcoes de reconhecimento de elementos de linguagem de montagem*/
 void diretivas(char *str, endereco *end, palavra *mapa);
 void rotulos(char *str, endereco *end, rotulo *rot);
 void instrucoes(char *str, endereco *end, palavra *mapa, rotulo *rot);

  /* Funcoes de Listas Ligadas adaptadas para o montador */
 void InicializaMapaRotulos(rotulo *inicio);
 void InsereRotuloFim(rotulo *inicio, char *token, endereco end);
 void ImprimeRotulos(rotulo *inicio);
 void LiberaRotulos(rotulo *inicio);

 int main(int argc, char *argv[]){
	FILE *arq_entrada; /* associado ao arquivo de entrada */
	FILE *arq_saida; /* associado ao arquivo de saida */
	char delim[] = " \n";/*delimitadores dos tokens*/
	char str[1001];/*string referente a cada linha*/
	char *token;/*string referente a cada token*/
	int tam; /* auxiliar */
	palavra *mapa; /* mapa de memoria */
	rotulo mapa_rot; /* mapa de rotulos */
 	endereco end = {0,0}; /* ponteiro de endereco */
	
	arq_entrada = fopen(argv[1],"r");/*abre o arquivo para leitura*/

	arq_saida = fopen(argv[2],"w");/*abre o arquivo para escrita*/
	
	/* Inicializa mapa dos rotulos */
	InicializaMapaRotulos(&mapa_rot);

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
			  instrucoes(token, end, mapa, mapa_rot);	
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
				instrucoes(token, end, mapa, rot);	
			}
		token = strtok(NULL, delim);
		}
   	}

	/* Percorrer mapa de memoria e preencher vazios com zeros */
	preencher_zeros(mapa);

	/* Escrever arquivo de saida */
	hexadecimal(mapa);

	ImprimeRotulos(&mapa_rot);
   	
   	LiberaRotulos(&mapa_rot);

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
