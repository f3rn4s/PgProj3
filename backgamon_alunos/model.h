/*-------------------------------------------------------------
 * Jogo do backgamon
 * Protótipos das funções do modelo e constantes
 * 
 * ISEL, outubro 2018
 *------------------------------------------------------------*/


#define NPIECES 15

#define NPLACES 24

 

/*--------------------------------------------------------
 * o tabuleiro é representado por dois arrays de 24 posições
 * um por cada jogador
 *------------------------------------------------------*/

#include <stdbool.h>

// identificação dos jogadores 
#define WHITE 0
#define RED 1


/* funções associadas a peças e lugares */


/**
 * retorna true se o lugar indicado estiver disponível (open)
 * para o jogador da cor indicada colocar peças
 * Parâmetros:
 * 		place_num - lugar (de 0 a 23)
 *  	color - identificação de jogador (WHITE ou RED)
 * Retorna:
 * 		true se o lugar está disponível
 * 		false caso contrário
 **/
bool place_is_open(int place_num, int color);

/**
 * A função tenta colocar uma quantidade de peças de um dado
 * jogador na posição indicada
 * 
 * Parâmetros:
 * 		place_num - posição do lugar (0 a 23)
 * 		quant - total de peças a colocar (no desenrolar do jogo é sempre 1)
 * 		color - identificação do jogador (WHITE ou RED)
 * Retorna:
 * 		-1 se ou lugar não está disponível
 * 		 1 se o lugar estava disponível e a unica peça do adversário presente foi capturada
 * 		 0 se o lugar estava vazio, não havendo portanto captura
 **/
int piece_put(int place_num, int quant, int color);

/**
 * Remove, se possível, uma peça do jogador "color" da casa indicada ("place_num")
 * Parâmetros:
 * 		place_num - lugar (de 0 a 23)
 *  	color - identificação de jogador (WHITE ou RED)
 * Retorna:
 * 		true se a remoção foi possível (havia peças do jogador na casa
 * 		false se a casa estava vazia ou tinha peças do adversário
 **/		
bool piece_remove(int place_num,  int color);

/**
 * Indica o total de peças do jogador indicado ("color") presentes na casa "place_num"
 * Parâmetros:
 * 		place_num - lugar (de 0 a 23)
 *  	color - identificação de jogador (WHITE ou RED)
 * Retorna- o número de peças do jogador na casa(0 se não existirem)
 **/
int pieces_at(int place_num, int color);

/**
 * Esta função é usada na libertação de peças
 * Serve para traduzir um valor de dado (1 a 6) na correspondente casa "home" do jogador
 * Parâmetros:
 * 		face - valor do dado jogado (1 a 6)
 *  	color - identificação de jogador (WHITE ou RED)
 * Retorna- a posição da casa "home" do jogador correspondente ao valor da face do dado
 **/
int place_home_other(int face, int color);

/**
 * Este função indica se é possível alguma jogada a partir de qualquer casa
 * que envolva os deslocamentos "mov1" e "mov2" para o jogador indicado
 * Parâmetros:
 * 		face1 - valor correspondente à face de um dado. Pode também ser zero
 * 			   o que quer dizer se o valor deve ser ignorado
 *		face2 - valor correspondente à face do outro dado. Pode também ser zero
 * 			   o que quer dizer se o valor deve ser ignorado
 * 		color - identificação de jogador (WHITE ou RED)
 *  Retorna:
 * 		true se houver pelo menos um moviment
 **/
bool place_any(int face1, int face2, int color);


/**
 * Adiciona uma peça às peças capturadas do jogador "color"
 * 
 * Parâmetros:
 * 		color - identificação de jogador (WHITE ou RED)
 * Retorna:
 * 		Nada
 **/
void bar_put(int color);

/**
 * Remove uma peça, se possível. às peças capturadas do jogador "color"
 * 
 * Parâmetros:
 * 		color - identificação de jogador (WHITE ou RED)
 * Retorna:
 * 		true se a remoção foi possível
 * 		false se não havia peças capturadas do jogador "color"
 **/
bool bar_remove(int color);

/**
 * Retorna o total de peças capturadas do jogador indicado
 * 
 * Parâmetros:
 * 		color - identificação de jogador (WHITE ou RED)
 * Retorna:
 * 		total de peças capturadas do jogador (ou 0 se não existirem)
 **/
int bar_captured(int color);

/**
 * Verifica se é possível uma jogada de libertação de peça capturada,
 * dados os valores de dados indicados, para o jogador também indicado
 * Parâmetros:
 * 		face1 - valor correspondente à face de um dado. Pode também ser zero
 * 			   o que quer dizer se o valor deve ser ignorado
 *		face2 - valor correspondente à face do outro dado. Pode também ser zero
 * 			   o que quer dizer se o valor deve ser ignorado
 * 		color - identificação de jogador (WHITE ou RED)
 * Retorna:
 * 		true se a jogada for possível, false caso contrário
 **/
bool bar_any(int face1, int face2, int color);

/**
 * verifica se foi atingida uma situação de vitória para o jogador indicadp
 * isto é, todas as suas 15 peças estão na sua zona "home"
 * 
 * Parâmetros:
 * 		color - identificação de jogador (WHITE ou RED)
 * Retorna:
 * 		true em caso de situação vitoriosa
 * 		false caso contrário
 * */
bool game_win(int color);


/**
 * Coloca o jogo no estado inicial.
 * Isto é com as peças de cada jogador colocadas nas casas convencionadas
 * e sem peças capturadas
 **/
void game_init();

