#include <stdio.h>
#include <string.h>
#include<stdlib.h>
#include "definitions.h"

void readline(char *line){
  fgets(line, MAX_LINE, stdin);
  int l = strlen(line) - 1;
  if(line[l] == '\n'){
    line[l] = '\0';
  }
}

void converter_carta(Card* carta){
    
    if(carta->nome_carta[0]=='1'){
      carta->valor = 10;
      strcpy(carta->naipe, &carta->nome_carta[2]);
    }else{
      if(carta->nome_carta[0]== 'K'|| carta->nome_carta[0] == 'Q' || carta->nome_carta[0] == 'J' || carta->nome_carta[0] =='A'){
        switch(carta->nome_carta[0]){
           case 'K': carta->valor= K; break;
           case 'Q': carta->valor = Q; break;
           case 'J': carta->valor = J; break;
           default: carta->valor = A; break;
        }
      }else{
         carta->valor = carta->nome_carta[0] - '0';
      }
      strcpy(carta->naipe, &carta->nome_carta[1]);
    }
}

//funcao que ordena as cartas
void ordenar(Card* carta, int tam){
    Card aux;
    int i, j;
    for(i=0; i<tam-1; i++){
        for(j=i+1; j<tam; j++){
            if(carta[i].valor>carta[j].valor){
              aux = carta[i];
              carta[i] = carta[j];
              carta[j] = aux;
            }
        }
    }
}

Card* reajuste(Card* mao, int* tam){
  Card aux[100]={0};
  int i, j=0;
  for(i=0; i<(*tam); i++){
    if(mao[i].valor!=0){
      aux[j] = mao[i];
      j++;
    }
  }
  (*tam)= j;
  
  for(i=0; i<100; i++){
    mao[i] = aux[i];
  }
  return mao;
}

char* descartar(Card* myhand, int tamanho_da_mao, Jogo*vetor_oponente, int n_jogos){
  FILE* arquivox = fopen("Descartar", "a");
	char* carta_lixo = calloc(10, sizeof(char));
  Card* vetor_descarte=calloc(sizeof(Card),1);
  int distancia=0;
  int tam_vetor=0;

  for(int i=0; i<tamanho_da_mao; i++){
    int aux = 0;
    if(myhand[i].valor!=0){

      for(int j=0; j<tamanho_da_mao;j++){  
        if(strcmp(myhand[i].naipe, myhand[j].naipe)==0 && i!=j){
          int distancia= abs(myhand[i].valor - myhand[j].valor);
          if(distancia <=2){
            if(distancia==0 && myhand[j].valor!=2){
              fprintf(arquivox, "carta a ser descartada = %s\n", myhand[j].nome_carta);
              strcpy(carta_lixo, myhand[j].nome_carta);
              myhand[j].valor=0;
              return carta_lixo;
            }else{
              aux++;
            }
          }
        }
      }
        if(aux==0 && myhand[i].valor!=2){
          vetor_descarte = realloc(vetor_descarte, sizeof(Card)*(tam_vetor+1));
          strcpy(vetor_descarte[tam_vetor].nome_carta, myhand[i].nome_carta);
          converter_carta(&vetor_descarte[tam_vetor]);
          tam_vetor++;  
        }
      }

    }
  int aux_vetor=0;
  if(tam_vetor>0){
    for(int h=0; h<tam_vetor; h++){
      for(int i=0; i<n_jogos; i++){
        for(int j=0; j<vetor_oponente[i].tam;j++){
          if(strcmp(vetor_descarte[h].naipe, vetor_oponente[i].vetor_cartas[j].naipe)==0 && vetor_oponente[i].vetor_cartas[j].valor!=2){
            distancia = abs(vetor_descarte[h].valor - vetor_oponente[i].vetor_cartas[j].valor);
            if(distancia==1){
              vetor_descarte[h].valor=0;
              aux_vetor++;
            }
          }
        }
      }
    }
  }
    
  if(tam_vetor>aux_vetor){
    ordenar(vetor_descarte, tam_vetor);
    strcpy(carta_lixo, vetor_descarte[tam_vetor-1].nome_carta);
  } else{
    strcpy(carta_lixo, myhand[tamanho_da_mao-1].nome_carta);
  }
  //zerar o valor da carta que vai ser descarta
  for(int x=0;x<tamanho_da_mao; x++){
    if(strcmp(carta_lixo, myhand[x].nome_carta)==0 && myhand[x].valor!=0){
      fprintf(arquivox, "carta a ser descartada aqui= %s ", carta_lixo);
      myhand[x].valor=0;
      break;
    }
  }
  fprintf(arquivox,"\n");
  fclose(arquivox);
  return carta_lixo;
}

void baixa_jogo(Jogo** jogos, int* num_jogos){
  char texto[100] = "MELD_NEW [ ";
  int i;
  for(i=0; i<((*jogos)[*num_jogos-1]).tam; i++){
        strcat(texto, ((*jogos)[*num_jogos-1]).vetor_cartas[i].nome_carta);
        strcat(texto, " ");
      }
      strcat(texto, "]\0");
      printf("%s", texto);
      printf("\n");
}

//funcao que cria jogos com as cartas da mao
int cria_jogo(Jogo** jogos, Card* myhand, int* tam_myhand, int* num_jogos){
  
  int i, j, m, n, k, l, q, z, a, b, mele =0, rep=0, tam_jogo=0, game=0;
  char texto[100] = "MELD_NEW [ ";
  Card aux[100]={0};
  FILE* arquivo = fopen("Cria Jogo", "a");
	setbuf(arquivo, NULL);
  
  //pega as cartas de myhand uma por uma pra testar se forma jogo
  for(i=0; i<(*tam_myhand); i++){
		((*jogos)[*num_jogos]).vetor_cartas[tam_jogo]=myhand[i];
    fprintf(arquivo, "primeira carta = %s\n", myhand[i].nome_carta); 
    //pega a carta na frente da carta escolhida e testa o naipe e a diferenca de valor
    for(j=i+1; j<(*tam_myhand); j++){
      if(strcmp(myhand[i].naipe, myhand[j].naipe)==0){
        //se encontrar uma sequencia adiciona a carta no vetor jogos
        if((myhand[j].valor-((*jogos)[*num_jogos]).vetor_cartas[tam_jogo].valor)==1){        
          tam_jogo++;
          ((*jogos)[*num_jogos]).vetor_cartas = realloc(((*jogos)[*num_jogos]).vetor_cartas, sizeof(Card)*(tam_jogo+1));
          ((*jogos)[*num_jogos]).vetor_cartas[tam_jogo] = myhand[j];
        }
        //testa se da pra colocar um mele
        else if((myhand[j].valor-((*jogos)[*num_jogos]).vetor_cartas[tam_jogo].valor)==2 && mele==0){ 
          for(q=0; q<(*tam_myhand); q++){
            if(myhand[q].valor==2 && strcmp(myhand[q].nome_carta, ((*jogos)[*num_jogos]).vetor_cartas[0].nome_carta)!=0){ 
              //adiciona o mele
              tam_jogo++;
              ((*jogos)[*num_jogos]).vetor_cartas = realloc(((*jogos)[*num_jogos]).vetor_cartas, sizeof(Card)*(tam_jogo+1));
              ((*jogos)[*num_jogos]).vetor_cartas[tam_jogo] = myhand[q];
              mele = 1;
              //adiciona a carta da frente
              tam_jogo++;
              ((*jogos)[*num_jogos]).vetor_cartas = realloc(((*jogos)[*num_jogos]).vetor_cartas, sizeof(Card)*(tam_jogo+1));
              ((*jogos)[*num_jogos]).vetor_cartas[tam_jogo] = myhand[j];
              break;
            }
          }
        }        
      }
    }
    //se o jogo tiver menos que 3 cartas, zera o vetor jogos
    if(tam_jogo<=1){
      ((*jogos)[*num_jogos]).vetor_cartas=realloc(((*jogos)[*num_jogos]).vetor_cartas, sizeof(Card) *1);
    }

    //se o jogos tiver mais do que 2 cartas     
    else{
      //abre espaco para mais um jogo em jogos, e confirma que conseguiu formar um jogo
      game = 1;
      ((*jogos)[*num_jogos]).tam = tam_jogo+1;
      //PREENCHE O INDICE DO JOGO NO VETOR DE JOGOS
      ((*jogos)[*num_jogos]).indice = *num_jogos;
      (*num_jogos)++;
			//jogos é Jogo**, para chegar no vetor (Jogo*), tem que derreferenciar um nível
      *jogos = realloc(*jogos, sizeof(Jogo) * (*num_jogos+1));
			((*jogos)[*num_jogos]).vetor_cartas = calloc(sizeof(Card), 1);
			
      //CHECA SE DA PRA COLOCAR UM AS NO INICIO
			if(((*jogos)[*num_jogos-1]).vetor_cartas[0].valor==2){
        
        for(z=0; z<(*tam_myhand); z++){
          if(myhand[z].valor == 14 && strcmp(myhand[z].naipe, ((*jogos)[*num_jogos-1]).vetor_cartas[0].naipe)==0 && strcmp(((*jogos)[*num_jogos-1]).vetor_cartas[0].nome_carta, ((*jogos)[*num_jogos-1]).vetor_cartas[tam_jogo].nome_carta)==1){
            //ATUALIZA O TAMANHO DOS JOGOS
            (((*jogos)[*num_jogos]).tam)++;
            tam_jogo++;
            //DA REALLOC EM JOGOS
            ((*jogos)[*num_jogos-1]).vetor_cartas = realloc(((*jogos)[*num_jogos-1]).vetor_cartas, sizeof(Card)*(tam_jogo+1));
            aux[0] = myhand[z];
            for(a=0; a<((*jogos)[*num_jogos]).tam; a++){
              aux[a+1] = ((*jogos)[*num_jogos-1]).vetor_cartas[a];
            }
            for(b=0; b<((*jogos)[*num_jogos]).tam; b++){
              ((*jogos)[*num_jogos-1]).vetor_cartas[b] = aux[b];
            }
            //CRIA VETOR AUX QUE RECEBE O AS E TODAS AS CARTAS DO JOGO
            //DA REALLOC EM JOGO E AUMENTA O VALOR DE TAM JOGO E ATUALIZA O TAM DO STRUCT
            //JOGO RECEBE AS CARTAS DE AUX

          }
        }
      }
      mele = 0;
    
      //apaga as cartas que vao ser baixadas do vetor mao
      for(m=0; m<(*tam_myhand); m++){
        for(n=0; n<((*jogos)[*num_jogos-1]).tam; n++){
          if(strcmp(myhand[m].nome_carta, ((*jogos)[*num_jogos-1]).vetor_cartas[n].nome_carta)==0){
            
            for(k=m+1; k<(*tam_myhand); k++){
              //testa se a carta que vai ser deletada é repetida, se for so deleta a ultima das repetidas
              if(strcmp(myhand[m].nome_carta, myhand[k].nome_carta)==0){
                rep = 1;
              }
            }
            if(rep==0){
              myhand[m].nome_carta[0] = '\0';
              myhand[m].valor = 0;
              myhand[m].naipe[0] = '\0';
            }
            rep = 0; 
          }
        }
      }
      
      //ajusta o tamanho de myhand, tirando as cartas que foram deletadas
      myhand = reajuste(myhand, tam_myhand);
      i=-1;
      
    }
    tam_jogo = 0;
  }
  fclose(arquivo);
  return game;
}

//vai retornar a posição do vetor jogos que tem jogo com a primeira carta do lixo
int puxa_lixo(Card*lixo, int tam_lixo, Card* myhand, int* tam_mao, Jogo** vetor_jogos, int* num_jogos, int tam_mao_oponente){
  FILE* papel = fopen("Puxa Lixo", "a");

  int num_aux=0;
  int tam_aux = *tam_mao;
  Jogo* jogos_aux=calloc(sizeof(Jogo),1);
  jogos_aux[0].vetor_cartas = calloc(sizeof(Card), 1);
  Card vetor_aux[100];
  int retorno=0;

  if(*tam_mao<12 && tam_mao_oponente>4){
    
    for(int a = 0; a<tam_aux; a++){
      vetor_aux[a] = myhand[a];
    }
    cria_jogo(&jogos_aux, vetor_aux, &tam_aux, &num_aux);
    reajuste(vetor_aux, &tam_aux);
    vetor_aux[tam_aux] = lixo[tam_lixo-1];
    tam_aux++;
    ordenar(vetor_aux, tam_aux);
    free(jogos_aux);
    
    
    if(cria_jogo(vetor_jogos, vetor_aux, &tam_aux, num_jogos)){ 
      fprintf(papel, "Quero pegar o lixo!\n");
      fprintf(papel, "Carta do lixo = %s\n", lixo[tam_lixo-1].nome_carta);

        char texto[100] = "GET_DISCARD [ ";
        for(int j=0; j<((*vetor_jogos)[*num_jogos-1]).tam; j++){  
          if(strcmp(lixo[tam_lixo-1].nome_carta,((*vetor_jogos)[*num_jogos-1]).vetor_cartas[j].nome_carta)!=0){
            strcat(texto,((*vetor_jogos)[*num_jogos-1]).vetor_cartas[j].nome_carta);
            strcat(texto, " ");
            for(int h=0; h<*tam_mao; h++){
              if(strcmp(myhand[h].nome_carta,(*vetor_jogos)[*num_jogos-1].vetor_cartas[j].nome_carta)==0){
              myhand[h].valor = 0;
              reajuste(myhand, tam_mao);
              break;
              }
            }
          }
        }
        strcat(texto, "]\0");
        printf("%s", texto);
        fprintf(papel, "%s", texto);
        printf("\n");
        reajuste(myhand,tam_mao);
        retorno = 1;
      
    }
  }
      
  fclose(papel);
  return retorno; 
}

void meld_join(int indice, Card carta){
  char texto[100] = "MELD_JOIN ";
  char aux[10];

  sprintf(aux, "%d", indice);
  strcat(texto, aux);
  strcat(texto, " [ ");
  strcat(texto, carta.nome_carta);
  strcat(texto, " ]");
  //IMPRIME O COMANDO NO TERMINAL
  printf("%s\n", texto);
}

void aumenta_jogo(Card* myhand, int* tam_myhand, Jogo** jogos, int num_jogos){
  int i, j, x, k;
  FILE* ble = fopen("Aumenta Jogo", "a");
  char aux[10];
  Card vetor_aux[100]={0};
  //IMPRIME AS CARTAS ATUAIS DA MAO
  fprintf(ble, "cartas atuais da mao:\n");
  fprintf(ble, "\n");
    
  for(x=0; x<(*tam_myhand); x++){
    fprintf(ble, "%s ", myhand[x].nome_carta);
  }
  fprintf(ble, "\n");
  fprintf(ble, "\n");

  //PEGA UMA CARTA DA MAO
  for(i=0; i<*tam_myhand; i++){
    //PEGA UM JOGO
    for(j=0; j<num_jogos; j++){
      //IMPRIME O JOGO ANALISADO
      fprintf(ble, "j = %d, indice = %d, tamin = %d: ", j, ((*jogos)[j]).indice, ((*jogos)[j]).tam);
      for(k=0; k<((*jogos)[num_jogos-1]).tam; k++){
        fprintf(ble, "%s ", ((*jogos)[j]).vetor_cartas[k].nome_carta);
      }
      fprintf(ble, "\n");
      fprintf(ble, "\n");
      //CHECA SE A ULTIMA CARTA DO JOGO TA CERTA

      //TESTA SE A CARTA DA MAO ENTRA NO INICIO DO JOGO
      if(((*jogos)[j]).vetor_cartas[0].valor-myhand[i].valor==1 && strcmp(myhand[i].naipe, ((*jogos)[j]).vetor_cartas[0].naipe)==0){
        fprintf(ble, "Carta %s, entra no incio do jogo %d\n", myhand[i].nome_carta, ((*jogos)[j]).indice);
        //IMPRIME O COMANDO NO TERMINAL
        meld_join(((*jogos)[j]).indice, myhand[i]);
        (((*jogos)[j]).tam)++;
        //DA REALLOC NO VETOR DE CARTAS
        ((*jogos)[j]).vetor_cartas = realloc(((*jogos)[j]).vetor_cartas, sizeof(Card)*(((*jogos)[j]).tam));
        //ADICIONA CARTA NA PRIMEIRA POSICAO DO VETOR AUXILIAR
        vetor_aux[0] = myhand[i];
        //VETOR AUX COPIA AS CARTAS DE VETOR JOGOS
        for(int a=1; a<((*jogos)[j]).tam; a++){
          vetor_aux[a] = ((*jogos)[j]).vetor_cartas[a-1];
        }
        //VETOR JOGOS COPIA AS CARTAS DE VETOR AUX
        for(int b = 0; b<((*jogos)[j]).tam; b++){
          ((*jogos)[j]).vetor_cartas[b] = vetor_aux[b];
        }
        //ZERA A CARTA NA MAO
        myhand[i].valor = 0;
        reajuste(myhand, tam_myhand);
      }
      //TESTA SE A CARTA ENTRA NO FIM DO JOGO
      else if(myhand[i].valor-((*jogos)[j]).vetor_cartas[((*jogos)[j]).tam-1].valor==1 && strcmp(myhand[i].naipe, ((*jogos)[j]).vetor_cartas[((*jogos)[j]).tam].naipe)==0){
        fprintf(ble, "Carta %s entra no final do jogo %d\n", myhand[i].nome_carta, ((*jogos)[j]).indice);
        //IMPRIME O COMANDO NO TERMINAL
        meld_join(((*jogos)[j]).indice, myhand[i]);
        //AUMENTA A POSICAO DA ULTIMA CARTA
        (((*jogos)[j]).tam)++;
        //DA REALLOC NO VETOR DE CARTAS DE JOGOS
        ((*jogos)[j]).vetor_cartas = realloc(((*jogos)[j]).vetor_cartas, sizeof(Card)*(((*jogos)[j]).tam));
        //ADICIONA A CARTA DA MAO NA ULTIMA POSICAO DE JOGOS
        ((*jogos)[j]).vetor_cartas[((*jogos)[j]).tam-1] = myhand[i];

        fprintf(ble, "carta adicionada: %s\n", ((*jogos)[j]).vetor_cartas[((*jogos)[j]).tam-1].nome_carta);
        //ZERA A CARTA NA MAO
        myhand[i].valor = 0;
        reajuste(myhand, tam_myhand);
      }
    }

  }

  fclose(ble);

}