//Tipos e dados e deinições/variáveis do Botman

#ifndef DEF
#define DEF

/** Tamanho máximo de uma linha da entrada-padrão */
#define MAX_LINE 200

/** Tamanho máximo do identificador do bot */
#define ID_SIZE 50

typedef enum {
    J=11, Q, K, A
} Valor;

typedef struct {
  char nome_carta[6];
  int valor;
  char naipe[6];
}Card;

typedef struct{
	Card* vetor_cartas;
	int tam;
  int indice;
} Jogo;

#endif
