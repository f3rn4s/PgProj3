

// tipo para representar uma posição no ecrã


typedef struct Dice {
	Point orig;		// posição do dado (canto superior esquerdo)
	int value;;		// valor do dado (1 a 6)
	int side;		// dimensão do lado do dado
	int dotSize;	// diâmetro dos pontos da face do dado
	bool selected;	// indica se o dado está seleccionado
	bool enabled;	// indica se o dado pode ser usado numa jogada
} Dice;

 
/**
 * Cria e retorna um novo dado numa dado posição na janela gráfica, 
 * com um determinado tamanho e valor inicial, não seleccionado e enabled
 * Se face == 0 escolhe aleatoriamente um valor
 * Parâmetros:
 * 		x, y - coordenadas do cantio superior esquerdo do dado
 * 		side - dimensão do lado
 * 		face- valor inicial
 * Retorna:
 * 		o dado criado
 */
Dice dice_create(int x, int y, int side, int face);
	 
/**
 * Desenha o dado na sua posição e com o seu valor
 * Parâmetros: 
 * 		d - dado a desenhar
 * Retorna:
 * 		nada
 **/		
void dice_draw(Dice d);
	 
/**
 * "Lança" o dado, modifica aleatoriamente o seu valor. Apresenta o
 * resultado do lançamento e retorna o dado com o novo valor
 * Parâmetros:
 * 		d - Dado a lançar
 * Retorna:
 * 		o dado após o lançamento
 */
Dice dice_roll(Dice d);


/**
 * Determina se o ponto de coordenadas "x","y" está contido no dado "d"
 * Parâmetros:
 * 		d - dado a verificar
 * 		x,y - coordenadas do ponto a verificar
 * Retorno:
 * 		true se o ponto está contido no dado, false caso contrário
 **/
bool dice_contains(Dice d, int x, int y);

/**
 * Determina se o dado "d" está seleccionado
 * Parâmetros:
 * 		d - dado a verificar
 * Retorno:
 * 		true se o dado está seleccionado, false caso contrário
 **/	
bool dice_is_selected(Dice d);

 /**
 * Determina se o dado "d" pode ser usado
 * Parâmetros:
 * 		d - dado a verificar
 * Retorno:
 * 		true se o dado pode ser jogado, false caso contrário
 **/	 
bool dice_is_enabled(Dice d);
 

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
Dice dice_select(Dice d, bool selected);

 /**
 * coloca o dado "d" pronto (ou não) a ser usado
 * Parâmetros:
 * 		d - dado a seleccionar
 * 		enabled - a true indica que o dado pode ser usado, a false
 * 				que o dado não pode ser usado
 * Retorno:
 * 		O dado alterado
 **/
Dice dice_enable(Dice d, bool enabled);

 /**
 * retorna o valor do dado "d"  
 * Parâmetros:
 * 		d - dado a observar o valor
 * Retorno:
 * 		O valor do dado
 **/
int dice_value(Dice d);

 /**
 * Altera o valor do dado "d". Appresentado a alteração do valor
 * e retorna o dado com o novo valor
 * Parâmetros:
 * 		d - dado a alterar o valor
 * 		face - novo valor
 * Retorno:
 * 		O dado com o novo valor
 **/
Dice dice_set_value(Dice d, int face); 
	 
	

