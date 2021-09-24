//Arquivo com as chamadas das funções
void readline(char *line);
void converter_carta(Card* carta);
void ordenar(Card* carta, int tam);
Card* reajuste(Card* mao, int* tam);
char* descartar(Card* myhand, int tamanho_da_mao, Jogo*vetor_oponente, int n_jogos);
void baixa_jogo(Jogo** jogos, int* num_jogos);
int cria_jogo(Jogo** jogos, Card* myhand, int* tam_myhand, int* num_jogos);
int puxa_lixo(Card*lixo, int tam_lixo, Card* myhand, int* tam_mao, Jogo** vetor_jogos, int* num_jogos, int tam_mao_oponente);
void meld_join(int indice, Card carta);
void aumenta_jogo(Card* myhand, int* tam_myhand, Jogo** jogos, int num_jogos);