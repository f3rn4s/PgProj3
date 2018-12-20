/*-------------------------------------------------------------
 * Jogo do backgamon em modo gráfico
 * 
 * 
 * Build:
 * 	 gcc -o gamon -Wall -std=c99 gamon.c model.c components.c dice.c utils.c -lpg
 * 
 * ISEL, novembro 2018
 *------------------------------------------------------------*/


#include <stdio.h>

#include <pg/graphics.h>
#include <pg/events.h>
#include <stdbool.h>

#include "components.h"
#include "model.h"
#include "dice.h"
#include "utils.h"


/*  Cores */
#define BACKGROUND_COLOR graph_rgb(130,130,120)
#define BAR_COLOR graph_rgb(255, 110, 0)
#define BACK_COLOR graph_rgb(255, 190, 130)
#define DARK_TRIANGLE graph_rgb(0, 125, 15)
#define LIGHT_TRIANGLE graph_rgb(140, 255, 140)

/* Dimensões */

// dimensões da moldura do tabuleiro
#define BAR_WIDTH 11
#define BAR_HEIGHT 10

// dimensões da zona de captura
#define HALF_BOARD_HEIGHT 300
#define HALF_BOARD_WIDTH 240

// dimensões totais do tabuleiro
#define TOTAL_HEIGHT (BAR_HEIGHT*2 + HALF_BOARD_HEIGHT)
#define TOTAL_WIDTH (2*HALF_BOARD_WIDTH + 4*BAR_WIDTH)

// canto superior esquerdo do tabuleiro
#define BX 50
#define BY 70

// sobre as "casas" do jogo
#define TRIANGLE_SPACE_X 8
#define TRIANGLE_SPACE_Y 60
#define TRIANGLE_BASE  ((HALF_BOARD_WIDTH -7*TRIANGLE_SPACE_X)/6)
#define TRIANGLE_HEIGHT ((HALF_BOARD_HEIGHT - TRIANGLE_SPACE_Y)/2)

// referências para determinar a posição de cada "casa"
#define FIRST_QUAD_X (BX + BAR_WIDTH + TRIANGLE_SPACE_X)
#define FIRST_QUAD_Y (BY + BAR_HEIGHT + HALF_BOARD_HEIGHT -2)

#define SECOND_QUAD_X (BX + 3*BAR_WIDTH + HALF_BOARD_WIDTH + TRIANGLE_SPACE_X)
#define SECOND_QUAD_Y FIRST_QUAD_Y 

#define THIRD_QUAD_X (BX + TOTAL_WIDTH - BAR_WIDTH - TRIANGLE_SPACE_X - TRIANGLE_BASE)
#define THIRD_QUAD_Y (BY + BAR_HEIGHT +1)

#define FOURTH_QUAD_X (BX + BAR_WIDTH + HALF_BOARD_WIDTH - TRIANGLE_SPACE_X- TRIANGLE_BASE)
#define FOURTH_QUAD_Y THIRD_QUAD_Y

// localização das peças capturadas
#define CAPTURED_WHITES_X (BX + 2*BAR_WIDTH + HALF_BOARD_WIDTH)
#define CAPTURED_WHITES_Y (BY + HALF_BOARD_HEIGHT + BAR_HEIGHT - TRIANGLE_HEIGHT)

#define CAPTURED_REDS_X (BX + 2*BAR_WIDTH + HALF_BOARD_WIDTH)
#define CAPTURED_REDS_Y  (BY + BAR_HEIGHT + TRIANGLE_HEIGHT)

// dimensão das peças
#define PIECE_RADIUS (TRIANGLE_BASE/2 -6)


// View com o nome do jogador
#define E_VIEWER_X (BX - 30)
#define E_VIEWER_Y 15
#define E_VIEWER_MAX_CHARS 8
#define E_VIEWER_MARGIN 10

// View com as mensagens
#define M_VIEWER_X (BX+150)
#define M_VIEWER_Y 425
#define M_VIEWER_MAX_CHARS 49
#define M_VIEWER_MARGIN 10

// dados
#define DADO1_X (BX-30)
#define DADO2_X (BX+30)
#define DADOS_Y	420
#define DADOS_SIDE 40

// localização da "casa" e 
// sentido do empilhamento das peças (para cima ou para baixo).
typedef struct PointDir {
	int x, y;  // localização da "casa" (canto superior esquerdo) 
	// o campo "up" indica o sentido do empilhamento das peças
	// a true se o empilhamento for para "cima", a false se for para "baixo"
	bool up; 
} PointDir;

// representa uma casa e as suas peças na janela gráfica
typedef struct PlaceView {
	PointDir ref;	    // localização 
	int placeNum;		// número (0 a 23) da casa
} PlaceView;



// variáveis globais

PlaceView boardPlaces[24]; 	// array com os representantes gráficos das casas do tabuleiro


int player;					// Jogador corrente (WHITE ou RED)
int playNumber;				// Jogadas no turno do jogador corrente (0,1 ou 2)

bool endGame;				// Indica que o jogo terminou!
bool game_started;			// Indica se o jogo está en adamento (a true após a primeira jogada)

// Componentes gráficos

Button loadBut;		// botão para recuperar estado do jogo previamente salvo
Button saveBut;		// botão para salvar o estdo do jogo
MsgView playerName;	// caixa de texto com o nome do jogador corrente (RED ou WHITE)
MsgView msgbox;		// caixa de texto de mensagens
Dice d1, d2; 		// dados

/**
 * Inicia o "turno" de um dado jogador:
 * apresenta o nome do jogador (WHITE ou RED)
 * lança os dados e guarad o jogador na variável global "player"
 **/
void setPlayer(int p) {
	player = p;
	if (p == RED)
		mv_show_clrtext(playerName, "Red", c_red, ALIGN_CENTER);
	else
		mv_show_clrtext(playerName, "White", c_white, ALIGN_CENTER);
	playNumber = 0;
	d1 = dice_roll(d1);
	d2 = dice_roll(d2);
	
}


// Funções auxiliares de desenho

void draw_background() {
	graph_rect(0,0, GRAPH_WIDTH, GRAPH_HEIGHT, BACKGROUND_COLOR,true);
}

void draw_triangle( int x, int y, int bw, int height, RGB clr, bool up) {
	graph_triangle(x, y, x+bw, y, x + bw/2, y + (up ? -height : height), clr, true);
}

void draw_triangle_at(PointDir pd, RGB color) {
	draw_triangle( pd.x, pd.y, TRIANGLE_BASE, TRIANGLE_HEIGHT, color, pd.up);
}


// relativas à zona de captura

/**
 * Refresca a apresentação da zona de captura
 * Parâmetros: 
 * 		não tem
 * Retorna:
 * 		nada
 **/
void bar_draw() {
	// clear bar
	graph_rect(BX+BAR_WIDTH+HALF_BOARD_WIDTH, BY+BAR_WIDTH+1,
	           BAR_WIDTH*2,
	           HALF_BOARD_HEIGHT,
	           BAR_COLOR, true);
	// draw captured pieces
	int cr = bar_captured(RED);
	int cw = bar_captured(WHITE);
	
	//printf("captured reds = %d\n", cr);
	//printf("captured whites = %d\n", cw);
	if (cr > 0) {
		int rcolor = (cr == 1) ? c_red : graph_rgb(128,0,0);
		graph_circle(CAPTURED_REDS_X, CAPTURED_REDS_Y, PIECE_RADIUS+1, rcolor, true);
		graph_circle(CAPTURED_REDS_X, CAPTURED_REDS_Y, PIECE_RADIUS+1, c_black,false);
	}
	if (cw > 0) {
		int wcolor = (cw == 1) ? c_white : c_gray;
		graph_circle(CAPTURED_WHITES_X, CAPTURED_WHITES_Y, PIECE_RADIUS+1, wcolor, true);
		graph_circle(CAPTURED_WHITES_X, CAPTURED_WHITES_Y, PIECE_RADIUS+1, c_black,false);
	}
}

/**
 * Verifica se o ponto de coordenadas passados por parâmetros
 * está contido na zona de captura
 * 
 * Parâmetros:
 * 		x - abcissa do ponto
 * 		y - ordenada do ponto
 * 		p - cor do jogador (WHITE ou RED)
 * Retorna:
 * 		true se o ponto pertencer à área de captura, false caso contrário
 */
bool bar_selected(int x, int y, int p) {
	if (p == WHITE) {
		return (x >= CAPTURED_WHITES_X - PIECE_RADIUS && 
				x < CAPTURED_WHITES_X + PIECE_RADIUS &&
				y >= CAPTURED_WHITES_Y - PIECE_RADIUS &&
				y < CAPTURED_WHITES_Y + PIECE_RADIUS);
	}
	else {
		return (x >= CAPTURED_REDS_X - PIECE_RADIUS && 
				x < CAPTURED_REDS_X + PIECE_RADIUS &&
				y >= CAPTURED_REDS_Y - PIECE_RADIUS &&
				y < CAPTURED_REDS_Y + PIECE_RADIUS);
	}
	
}

// relativas à representação das "casas" do tabuleiro

/**
 * Desenha as peças de um dada côr presentes na "casa" indicada
 * Parâmetros:
 * 		pd - localização da casa 
 * 		quant- número de peças a colocar
 * 		color - côr das peças a colocar
 * Retorna:
 * 		nada
 * */		
void draw_pieces_at(PointDir pd, int quant, int color) {
	int x = pd.x + TRIANGLE_BASE/2; 
	int y = pd.y + (pd.up ? - PIECE_RADIUS: +PIECE_RADIUS);
	for(int i=0; i < min(quant, 5); ++i) {
		graph_circle(x, y, PIECE_RADIUS, color == WHITE ? c_white : c_red, true);
		graph_circle(x, y, PIECE_RADIUS, c_black, false);
		if (pd.up)  y -= PIECE_RADIUS*2; else y += PIECE_RADIUS*2;
	}
	if (quant == 6) {
		graph_circle(x, y, PIECE_RADIUS, color == WHITE ? c_white: c_red, true);
		graph_circle(x, y, PIECE_RADIUS, c_black, false);
	}
	else if (quant > 6) {
		graph_circle(x, y, PIECE_RADIUS, color == WHITE ? c_gray: graph_rgb(128,0,0), true);
		graph_circle(x, y, PIECE_RADIUS, c_black, false);
	}
}

/**
 * Dada uma "casa" do tabuleiro (de 0 a 23) retorna a posição da "casa" na j
 * janela gráfica
 * 
 * Parâmetros:
 * 	place_num - índice da "casa" (0 a 23)
 * Retorna:
 * 	PointDir com a localização da casa
 **/
PointDir place_pointOf(int place_num) {
	int xp, yp;
	bool up;
	
	switch(place_num) {
		case 0: case 1: case 2: case 3: case 4: case 5: 
			xp = FIRST_QUAD_X + place_num*(TRIANGLE_BASE+TRIANGLE_SPACE_X);
			yp = FIRST_QUAD_Y;
			up = true;
			break;
			
		case 6: case 7: case 8: case 9: case 10: case 11: 
			place_num -= 6;
			xp =  SECOND_QUAD_X + place_num*(TRIANGLE_BASE+TRIANGLE_SPACE_X);
			yp =  SECOND_QUAD_Y;
			up = true;
			break;
		case 12: case 13: case 14: case 15: case 16: case 17: 
			place_num -= 12;
			xp =  THIRD_QUAD_X - place_num*(TRIANGLE_BASE+TRIANGLE_SPACE_X);
			yp =  THIRD_QUAD_Y;
			up = false;
			break;
		default:
			place_num -= 18;
			xp =  FOURTH_QUAD_X - place_num*(TRIANGLE_BASE+TRIANGLE_SPACE_X);
			yp =  FOURTH_QUAD_Y;
			up = false;
			break;
	}
	PointDir p = { xp, yp, up };
	return p;
}


/**
 * Refresca na janela gráfica a representação de uma dada
 * casa. A chamar sempre que ocorra uma alterção do número e/ou côr
 * das peças na casa. 
 * 
 * Parâmetros:
 * 		pv - estrutura que representa a "casa" na janela gráfica
 * Retorna:
 * 		nada
 */
void place_draw(PlaceView pv) {
	int color, n;
	
	// obter peças (quantidade e côr) presentes na "casa"
	n = pieces_at(pv.placeNum, RED);
	if (n>0) color = RED;
	else {
		n = pieces_at(pv.placeNum, WHITE);
		color = WHITE;	
	}	
		
	// primeiro apagar o lugar...
	PointDir pd = pv.ref;
	if (pd.up) 
		graph_rect(pd.x, pd.y - TRIANGLE_HEIGHT, TRIANGLE_BASE, TRIANGLE_HEIGHT, BACK_COLOR, true);
	else
		graph_rect(pd.x, pd.y , TRIANGLE_BASE, TRIANGLE_HEIGHT, BACK_COLOR, true);
	//printf("old place erased!\n");
	
	// ...agora desenhá-lo de novo
	draw_triangle_at(pv.ref, (pv.placeNum % 2== 0) ? DARK_TRIANGLE: LIGHT_TRIANGLE);
	if (color == WHITE) {
		draw_pieces_at(pv.ref, n, WHITE);
	}
	else {
		draw_pieces_at(pv.ref, n, RED);
	}
		
}

/**
 * Dadas as coordenada de um ponto na janela gráfica, indica
 * ou número da casa a que pertencem ou -1 caso não pertencam a nenhuma casa
 * Parâmetros:
 * 	x, y - coordenadas do ponto
 * Retorna:
 * 		se o ponto pertencer a uma casa retorna oíndica da casa (0 a 23)
 * 		-1 se o ponto não pertence a nehuma casa
 **/
int place_at_coord(int x, int y) {
	for (int i=0; i < 24; ++i) {
		PointDir pd = boardPlaces[i].ref;
		if (pd.up) {
			if (x >= pd.x && x < pd.x + TRIANGLE_BASE &&
			    y <= pd.y && y > pd.y -TRIANGLE_HEIGHT)
			    return i;
		} else if (x >= pd.x && x < pd.x + TRIANGLE_BASE &&
			    y >= pd.y && y < pd.y +TRIANGLE_HEIGHT) {
			    return i;
		}
	}
	return -1;
}

/**
 * Cria o representante de uma "casa" na janela gráfica 
 * dada a sua localização e o índice da casa que representa
 *
 * Parâmetros:
 * 	PointDir pd - localização da casa na janela gráfica
 *  int num - índice da casa
 * Retorna
 * 	A estrutura que representa a casa na janela gráfica
 **/
PlaceView place_view_create(PointDir pd, int num) {
	PlaceView pv;
	
	pv.ref = pd;
	pv.placeNum = num;
	return pv;
}



/**
 * Inicia o array global "boardPlaces" que contém 
 * os representatntes gráficos das 24 casas do tabuleiro
 **/
void place_views_init() {
	for(int i= 0; i < 24; ++i) {
		PointDir pd = place_pointOf(i);
		boardPlaces[i] = place_view_create(pd, i);
	}
}


// relativas a todo o tabuleiro de jogo 

/**
 * Desenha o tabuleiro de jogo na janela gráfica
 */
void draw_board() {
	 
	graph_rect(BX, BY, TOTAL_WIDTH, TOTAL_HEIGHT, BAR_COLOR, true);
	graph_rect(BX, BY, TOTAL_WIDTH, TOTAL_HEIGHT, c_black, false);
	
	graph_rect(BX+BAR_WIDTH, BY+BAR_HEIGHT, HALF_BOARD_WIDTH, HALF_BOARD_HEIGHT, BACK_COLOR, true);
	graph_rect(BX+BAR_WIDTH, BY+BAR_HEIGHT, HALF_BOARD_WIDTH, HALF_BOARD_HEIGHT, c_black, false);
	graph_rect(BX+3*BAR_WIDTH+HALF_BOARD_WIDTH, 
			   BY+BAR_HEIGHT, HALF_BOARD_WIDTH, HALF_BOARD_HEIGHT, BACK_COLOR, true);
	graph_rect(BX+3*BAR_WIDTH+HALF_BOARD_WIDTH, 
			   BY+BAR_HEIGHT, HALF_BOARD_WIDTH, HALF_BOARD_HEIGHT, c_black, false);
			   
	
	for(int i=0; i < 24; ++i)
		place_draw(boardPlaces[i]);	
}

// validação e realização da jogada

/**
 * Valida e efectua (se válida) jogada (de peça livre) do jogador corrente 
 * (presente na variável global "player"), de acordo com o 
 * valor do dado seleccionado e ainda não jogado.
 * Parâmetros;
 * 		start - posição da peça do jogador corrente a mover
 * Retorna:
 * 		true - se a jogada foi válida e realizada
 * 		false - se houve uma tentativa de jogada inválida
 */
bool play(int start) {
	 // a implementar
	 return false;
}

/**
 * Valida e efectua (se válida) jogada do jogador corrente 
 * (presente na variável global "player"), para libertar uma 
 * peça capturada, de acordo com o valor do dado seleccionado 
 * e ainda não jogado.
 * Parâmetros;
 * 		não tem
 * Retorna:
 * 		true se a jogada de libertação de peça foi válida e realizada
 * 		false - se houve uma tentativa de jogada inválida 
 */
bool play_bar() {
	// a implementar
	return false;	
}

// mensagens

void winMessage(int player) {
	char msg[100];
	sprintf(msg, "%s venceu o jogo! Parabens.", player == RED ? "RED" : "WHITE");
	mv_show_text(msgbox, msg, ALIGN_LEFT);
}

void haveCapturedPiecesMessage() {
	mv_show_text(msgbox, "Tem pecas capturadas", ALIGN_LEFT);
}

void giveTurnMessage(int player) {
	char msg[100];
	sprintf(msg, "%s nao tem jogadas possiveis. A vez ao %s",
					player == RED ? "RED" : "WHITE", 	player == RED ? "WHITE" : "RED");
	mv_show_text(msgbox, msg, ALIGN_LEFT);
}

void endOfGameMessage() {
	mv_show_text(msgbox, "Jogo terminado!", ALIGN_LEFT);
}


/**
 * Executa o processamento do evento rato 
 * de acordo com a regras do jogo do gamâo
 */
void process_mouse(MouseEvent me) {
	// a implementar
}



/**
 * Criação e apresentação dos componentes gráficos usados
 */
void create_components() {
	playerName = mv_create(E_VIEWER_X, E_VIEWER_Y, E_VIEWER_MAX_CHARS, 
						   E_VIEWER_MARGIN, E_VIEWER_MARGIN, 
						   LARGE_FONT, 
						   c_orange, c_dgray);
						   
	msgbox = mv_create(M_VIEWER_X, M_VIEWER_Y, M_VIEWER_MAX_CHARS, 
						   M_VIEWER_MARGIN, M_VIEWER_MARGIN, 
						   SMALL_FONT, 
						   c_orange, c_dgray);
					   

	d1 = dice_create(DADO1_X, DADOS_Y, DADOS_SIDE, 0);
	d2 = dice_create(DADO2_X, DADOS_Y, DADOS_SIDE, 0);
	
	dice_draw(d1);
	dice_draw(d2);
	
	mv_show_text(msgbox, "Iniciar jogo!", ALIGN_LEFT);
}


int main() {
	rand_start();			// semente para o gerador aleatório
	graph_init();			
	
	game_init();			// iniciação do estado do modelo de jogo
	place_views_init();		// iniciação da representação visual das "casas" do jogo
	
	draw_background();		// colocação da cor de background
	draw_board();			// apresentação do estado inicial do tabuleiro jogo
	create_components();	// criação e afixação de componentes visuais adicionais		
	
	graph_regist_mouse_handler(process_mouse); // registo do handler de eventos do rato
	
	// início aleatório do jogador - branco ou vermelho
	setPlayer(irand(1,10) <= 5 ? WHITE : RED);
	
	graph_start();
	return 0;
}
