/********************************************************************
  Bot-exemplo
  
  Após receber as informações iniciais do jogo, a cada rodada, esse
  bot irá apenas puxar uma carta e, em seguida, descartá-la.
  Cabe a você agora aprimorar sua estratégia!!!
 ********************************************************************/

#include <stdio.h>
#include <string.h>
#include<stdlib.h>
#include "definitions.h"
#include "funcs.h"

/**
 * Lê uma linha da entrada padrão e remove o '\n' se necessário.
 * @param line string a ser alterada com o conteúdo da entrada-padrão.
 */

int main(){
  char line[MAX_LINE];  // dados temporários
  char myId[ID_SIZE];    // identificador do bot em questão
  char* pedaco;
  FILE* log = fopen("Simulacao", "w");

  Card myhand[100]={0};
  Card* lixo= calloc(sizeof(Card), 1);//vetor de cartas;
  int tam_lixo=0;
  int tam_myhand = 11;
  int num_jogos = 0;
  int num_jogos_inicial = 0;
  int game = 0;
  int l, a;

  Jogo* vetor_jogos = calloc(sizeof(Jogo), 1);
  vetor_jogos[0].vetor_cartas = calloc(sizeof(Card), 1);
  int mao_oponente=11;
  Jogo* vetor_jogos_oponente;
  int num_jogos_oponente = 0;

  setbuf(stdin, NULL);   // stdin, stdout e stderr não terão buffers
  setbuf(stdout, NULL);  // assim, nada é "guardado temporariamente"
  setbuf(stderr, NULL);
  setbuf(log, NULL);

                         // DADOS DO INÍCIO DA PARTIDA
  readline(line);        // lê os identificadores dos jogadores
  readline(line);        // lê o identificador do bot (seu id)
  strncpy(myId, line, ID_SIZE); // guarda para uso futuro
  fprintf(log, "Meu id: %s\n", myId);

  readline(line);        // lê as cartas que o bot tem na mão
  fprintf(log, "Minha mão: %s\n", line);
  pedaco = strtok(line, " ");
  
  for(int i=0; i<tam_myhand; i++){
    pedaco = strtok(NULL, " ");
    strcpy(myhand[i].nome_carta, pedaco);
    converter_carta(&myhand[i]);
  }
  
  ordenar(myhand, tam_myhand);

  readline(line); // lê a carta inicial da pilha de descarte.
  strcpy(lixo[tam_lixo].nome_carta,line);
  converter_carta(&lixo[tam_lixo]);
  tam_lixo++;
  fprintf(log, "Lixo_inicio: %s\n", lixo[tam_lixo-1].nome_carta);

  
  while(1) {             // DADOS DURANTE A PARTIDA
    do {                  // lê e descarta os dados até chegar sua vez
      readline(line);     // sai do laço quando for enviado seu id
     
      fprintf(log, "COMANDO: %s\n", line);

    // VETOR LIXO 
      if(strncmp("DISCARD",line,7)==0){
        pedaco = line + 8;
        lixo = realloc(lixo, sizeof(Card)*(tam_lixo+1));
        strcpy(lixo[tam_lixo].nome_carta,pedaco);
        converter_carta(&lixo[tam_lixo]);
        tam_lixo++;
      } 

      if(strncmp("GET_DISCARD",line,11)==0){
        mao_oponente = mao_oponente + tam_lixo;
        tam_lixo=0;
        lixo = realloc(lixo, sizeof(Card)*(tam_lixo+1));
      }


      if(strncmp("MELD_NEW",line,8)==0){
        vetor_jogos_oponente = realloc(vetor_jogos_oponente, sizeof(Jogo)*(num_jogos_oponente+1));
        vetor_jogos_oponente[num_jogos_oponente].vetor_cartas=calloc(sizeof(Card),1);
        pedaco = strtok(line+10, " ");
        int n=0;
        while(strcmp(pedaco,"]")!=0){
          strcpy(vetor_jogos_oponente[num_jogos_oponente].vetor_cartas[n].nome_carta, pedaco);
          fprintf(log, "carta do oponente %s\n",pedaco);
          converter_carta(&vetor_jogos_oponente[num_jogos_oponente].vetor_cartas[n]);
          n++;
          vetor_jogos_oponente[num_jogos_oponente].vetor_cartas=realloc(vetor_jogos_oponente[num_jogos_oponente].vetor_cartas, sizeof(Card)*(n+1));
          pedaco = strtok(NULL, " ");
          
        } 
        vetor_jogos_oponente[num_jogos_oponente].tam = n;
        ordenar(vetor_jogos_oponente[num_jogos_oponente].vetor_cartas, vetor_jogos_oponente[num_jogos_oponente].tam);
        mao_oponente = mao_oponente - n;
        fprintf(log,"tamanho da mao: %i\n", mao_oponente);
        num_jogos_oponente++;

      }


    } while (strcmp(line, myId)); // sai do laço quando for a sua vez!

    
    if(puxa_lixo(lixo, tam_lixo, myhand, &tam_myhand, &vetor_jogos, &num_jogos, mao_oponente)){
      for(int x=0; x<tam_lixo-1; x++){
        myhand[tam_myhand] = lixo[x];
        tam_myhand++;
      }
      ordenar(myhand, tam_myhand);
      tam_lixo=0;
      lixo = realloc(lixo, sizeof(Card)*(tam_lixo+1));
    }else{
      printf("GET_STOCK\n");       
      readline(line);       
      strcpy(myhand[tam_myhand].nome_carta, line); 
      converter_carta(&myhand[tam_myhand]);  
      tam_myhand++;  
      ordenar(myhand, tam_myhand); 
    }
        
                 
    fprintf(log, "tam_hand antes de puxar: %d\n", tam_myhand);
    fprintf(log, "botman puxou a carta %s\n", line);
    fprintf(log, "tam_myhand depois de puxar = %d\n", tam_myhand);
    //tenta criar um jogo com a carta do GET_STOCK

    //FUNCAO PRA TENTAR AUMENTAR JOGO
    if(num_jogos>=1){
      aumenta_jogo(myhand, &tam_myhand, &vetor_jogos, num_jogos);
      fprintf(log, "Tentei aumentar um jogo\n");
    }

    num_jogos_inicial = num_jogos;
    //O QUE EU TENTEI FAZER PRA BAIXAR JOGO FORA DA FUNCAO
    if(cria_jogo(&vetor_jogos, myhand, &tam_myhand, &num_jogos)){
      for(a=num_jogos_inicial+1; a<num_jogos+1; a++){
      baixa_jogo(&vetor_jogos, &a);
      }
    }
    
    
    char* carta_descarte =  descartar(myhand,tam_myhand, vetor_jogos_oponente,num_jogos_oponente); //escolhe uma carta pra descartar
		fprintf(log, "DISCARD %s\n", carta_descarte);
    printf("DISCARD %s\n", carta_descarte); //descarta
    
    for(int k=0; k<tam_myhand; k++){
      if(myhand[k].valor==0){
      fprintf(log, " %s ", myhand[k].nome_carta);
      }
      
    } 
    reajuste(myhand, &tam_myhand); 
    fprintf(log, "botman descartou: %s\n",carta_descarte);
    fprintf(log, "tam_myhand depois de descartar = %d\n", tam_myhand);

    lixo = realloc(lixo, sizeof(Card)*(tam_lixo+1));
    strcpy(lixo[tam_lixo].nome_carta,carta_descarte);
    converter_carta(&lixo[tam_lixo]);
    tam_lixo++;

  }
  return 0;
}