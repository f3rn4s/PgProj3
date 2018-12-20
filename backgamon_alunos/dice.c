#include <pg/graphics.h>

#include "components.h"
#include "utils.h"
#include "dice.h"

#define CELL_SIZE (12)
#define CELL_COLOR (c_white)
#define CELLS_PER_LINE (3)
#define CELLS_PER_COL (3)
#define CELL_CIRCLE_COLOR (black)
#define CELL_CROSS_COLOR (c_red)
#define CELL_MARGIN (4)

#define LINE_SIZE (2)

#define BOARD_COLOR (c_black)

#define CELL_STATE_NULL (-1)
#define CELL_STATE_CIRCLE ('O')
#define CELL_STATE_CROSS ('X')
#define CELL_STATE_EMPTY ('?')


/**
 * Cria e retorna um novo dado numa dado posição na janela gráfica, 
 * com um determinado tamanho e valor inicial, não seleccionado e enabled.
 *  * Se face == 0 escolhe aleatoriamente um valor
 * Parâmetros:
 * 		x, y - coordenadas do cantio superior esquerdo do dado
 * 		side - dimensão do lado
 * 		face- valor inicial
 * Retorna:
 * 		o dado criado
 */
Dice dice_create(int x, int y, int side, int face) {

	Dice d;
	d.orig.x = x;
	d.orig.y = y;
	d.side = side;
	if(face == 0)
		d.value= random_number();
	else
		d.value = face;
	
	return d;
}

  

/**
 * Desenha o dado na sua posição e com o seu valor
 * Parâmetros: 
 * 		d - dado a desenhar
 * Retorna:
 * 		nada
 **/
void dice_draw(Dice d) { //Falta colocar valor nos dados
	 {		
		for(int i = 0; i < CELLS_PER_LINE; ++i)
		{
			for(int j = 0; j < CELLS_PER_LINE; ++j)
			{				
				graph_rect(d.orig.x,d.orig.y,d.side,d.side, CELL_COLOR, true);
			}
		}
	}
}

/**
 * Determina se o ponto de coordenadas "x","y" está contido no dado "d"
 * Parâmetros:
 * 		d - dado a verificar
 * 		x,y - coordenadas do ponto a verificar
 * Retorno:
 * 		true se o ponto está contido no dado, false caso contrário
 **/
bool dice_contains(Dice d, int x, int y) {
	// a implementar
	return false;
}

/**
 * "Lança" o dado, modifica aleatoriamente o seu valor. Apresenta o
 * resultado do lançamento e retorna o dado com o novo valor
 * Parâmetros:
 * 		d - Dado a lançar
 * Retorna:
 * 		o dado após o lançamento
 */
Dice dice_roll(Dice d) {
	// a implementar
	return d;
}

/**
 * Determina se o dado "d" está seleccionado
 * Parâmetros:
 * 		d - dado a verificar
 * Retorno:
 * 		true se o dado está seleccionado, false caso contrário
 **/	
bool dice_is_selected(Dice d) {
	return d.selected;
}

 /**
 * selecciona (ou desselecciona)  o dado "d". Apresenta o resultado da selecção
 * e retorna o dado com o novo estado
 * Parâmetros:
 * 		d - dado a seleccionar
 * 		selected - a true indica que o dado é para seleccionar, a false
 * 				é para desseleccionar
 * Retorno:
 * 		O dado alterado
 **/
Dice dice_select(Dice d, bool selected) {
	// a implementar
	return d;
}

 /**
 * retorna o valor do dado "d"  
 * Parâmetros:
 * 		d - dado a observar o valor
 * Retorno:
 * 		O valor do dado
 **/
int dice_value(Dice d) {
	return d.value;
}

 /**
 * coloca o dado "d" pronto (ou não) a ser usado
 * Parâmetros:
 * 		d - dado a seleccionar
 * 		enabled - a true indica que o dado pode ser usado, a false
 * 				que o dado não pode ser usado
 * Retorno:
 * 		O dado alterado
 **/
Dice dice_enable(Dice d, bool enabled) {
	d.enabled = enabled;
	return d;
}

 /**
 * Determina se o dado "d" pode ser usado
 * Parâmetros:
 * 		d - dado a verificar
 * Retorno:
 * 		true se o dado pode ser jogado, false caso contrário
 **/
bool dice_is_enabled(Dice d) {
	return d.enabled;
}

 /**
 * Altera o valor do dado "d". Appresentado a alteração do valor
 * e retorna o dado com o novo valor
 * Parâmetros:
 * 		d - dado a alterar o valor
 * 		face - novo valor
 * Retorno:
 * 		O dado com o novo valor
 **/
Dice dice_set_value(Dice d, int face) {
	// a impelmentar
	return d;
}
	
int random() //Funcao random universal para ser usado para qualquer situacao //https://www.tutorialspoint.com/c_standard_library/c_function_srand.html
{
	int random;
	time_t t;

	srand((unsigned) time(&t));

	for(int i = 0; i < 5; i++) 
        random = (int)(rand() % 2225);

    return random;
}
int random_number() // função para retirar valores random com valores 1,2 e 3
{
	int a = 0;
	a = random();
	if (a > 750 && a < 1500)
	{
		return a = 2;
	}
	else if(a >= 1500 )
	{
		return a = 3;
	}
	else
	{
		return a = 1;
	}
}	
	
