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
	int tam;
	rotulo mapa_rot;	
	endereco end = {0,0};
	
	arq_entrada = fopen(argv[1],"r");/*abre o arquivo para leitura*/

	arq_saida = fopen(argv[2],"w");/*abre o arquivo para escrita*/

	InicializaMapaRotulos(&mapa_rot);

	/* PRIMEIRA LEITURA DO ARQUIVO - MAPA DE ROTULOS */
	while(!feof(arq_entrada)){	
		/* Carrega o primeiro token da linha de entrada */
		token = strtok(fgets(str,1000,arq_entrada),delim);
		while(token != NULL){		
		  tam = strlen(token);
		  /* Teste se eh diretiva (primeiro caracter eh '.' */		
		  if(token[0] == '.'){ 
		    printf("diretiva %s\n", token);	
		  }
		  /* Teste se eh roluto (ultimo caracter eh ':' */
		  else if(token[tam-1] == ':'){
		    printf("rotulo %s\n", token);
		    InsereRotuloFim(&mapa_rot,token,end);
		  }
		  /* Teste se eh comentario (primeiro caracter eh '#' */
		  else if(token[0] == '#'){
		    printf("comentario %s\n", token);
		    /* Percorre ate o fim da linha */		
		    token = strtok(NULL,"\n");
		  }
		  token = strtok(NULL, delim);
		}
   	}
   	
   	ImprimeRotulos(&mapa_rot);
   	
   	LiberaRotulos(&mapa_rot);

	fclose(arq_entrada);/*fecha o arquivo de entrada*/

	fclose(arq_saida);/*fecha o arquivo de saida*/
    

 return 0;
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
