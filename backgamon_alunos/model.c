/*-------------------------------------------------------------
 * funções para manter o estado do jogo do "Gamão"
 * Programação trabalho 3

 * Grupo: K
 * 24703    Samir Santo
 * 35164    Fernando Duarte
 * 10065    Luis Silva  
 * ISEL, outubro 2018
 *------------------------------------------------------------*/


#include <stdio.h>
#include <stdbool.h>
#include "model.h"



// variávels globais para representar o estado do tabuleiro de jogo


int reds[NPLACES]; 		// distribuição das peças do jogador RED
int whites[NPLACES]; 	// distribuição das peças do jogador WHITE

int hittedWhites;		// total de peças brancas capturadas
int hittedReds;			// total de peças vermelhas capturadas

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
bool place_is_open(int place_num, int color) {
	
	if(color==0)
	{
		if(pieces_at(place_num, 1)>1)
			return false;
	}
	else
		if(pieces_at(place_num, 0)>1)
			return false;
	return true;//verificar depois
}

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
int piece_put(int place_num, int quant, int color) {
	if(color==WHITE)
	{
		if(pieces_at(place_num, RED)>RED)
			return -1;
		if(pieces_at(place_num, RED)==RED)
		{
			reds[place_num]--;//é o prof a fazer a captura 
			whites[place_num]++;//é sempre 1 peça!
			return 1;
		}
		if(pieces_at(place_num, RED)==WHITE)
		{
			whites[place_num]++;
			return 0;
		}
	}
	
	if(color==RED)
	{
		if(pieces_at(place_num, WHITE)>RED)
			return -1;
		if(pieces_at(place_num, WHITE)==RED)
		{
			whites[place_num]--;
			reds[place_num]++;
			return 1;
		}
		if(pieces_at(place_num, WHITE)==WHITE)
		{
			reds[place_num]++;
			return 0;
		}
	}
	return 0;
}

/**
 * Remove, se possível, uma peça do jogador "color" da casa indicada ("place_num")
 * Parâmetros:
 * 		place_num - lugar (de 0 a 23)
 *  	color - identificação de jogador (WHITE ou RED)
 * Retorna:
 * 		true se a remoção foi possível (havia peças do jogador na casa
 * 		false se a casa estava vazia ou tinha peças do adversário
 **/	
bool piece_remove(int place_num,  int color)
{
	if(pieces_at(place_num, color)>WHITE)
	{
		if(color==WHITE)
			whites[place_num]--;
		if(color==RED)
			reds[place_num]--;
		return true;
	}
	else
		return false;
}

/**
 * Indica o total de peças do jogador indicado ("color") presentes na casa "place_num"
 * Parâmetros:
 * 		place_num - lugar (de 0 a 23)
 *  	color - identificação de jogador (WHITE ou RED)
 * Retorna- o número de peças do jogador na casa(0 se não existirem)
 **/
int pieces_at(int place_num, int color) {
	if(color==WHITE)
		return whites[place_num];
		
	if(color==RED)
		return reds[place_num];
	
	return 0;
}

/**
 * Esta função é usada na libertação de peças
 * Serve para traduzir um valor de dado (1 a 6) na correspondente casa "home" do jogador
 * Parâmetros:
 * 		face - valor do dado jogado (1 a 6)
 *  	color - identificação de jogador (WHITE ou RED)
 * Retorna- a posição da casa "home" do jogador correspondente ao valor da face do dado
 **/
 int place_home_other(int num, int color) {
	if (color == WHITE) //jogador white
		return 11+num;//11 é zero de cima
		
    if (color == RED) //jogador red 
		return 12-num;//12 é zero de baixo
	return 0;
}

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
bool place_any(int mov1, int mov2, int color) {
	if(color==WHITE)
		for(int i = 1;i<NPLACES;i++)
			if(pieces_at(i, WHITE)>=1)
				if(pieces_at(i+mov1, RED)<2 || pieces_at(i+mov2, RED)<2)
					return true;
		
	if(color==RED)
		for(int i = 1;i<NPLACES;i++)//percorrer as casas todas
			if(pieces_at(i, RED)>=1)//se tiver peças reds
				if(pieces_at(i-mov1, WHITE)<2 || pieces_at(i-mov2, WHITE)<2)
					return true;//se uma estiver sem +1whites devolver true
	
	return false;
}

/** Adiciona uma peça às peças capturadas do jogador "color"
 * 
 * Parâmetros:
 * 		color - identificação de jogador (WHITE ou RED)
 * Retorna:
 * 		Nada
 **/
void bar_put(int color) {
	if(color==WHITE)
		hittedWhites++;
	
	if(color==RED)
		hittedReds++;
	
	return;
}

/**
 * Remove uma peça, se possível. às peças capturadas do jogador "color"
 * 
 * Parâmetros:
 * 		color - identificação de jogador (WHITE ou RED)
 * Retorna:
 * 		true se a remoção foi possível
 * 		false se não havia peças capturadas do jogador "color"
 **/
bool bar_remove(int color) {
	if(color==WHITE)
	{
		if(hittedWhites==0)
			return false;
		hittedWhites--;
	}
	if(color==RED)
	{
		if(hittedReds==0)
			return false;
		hittedReds--;
	}
	return true;
}

/**
 * Retorna o total de peças capturadas do jogador indicado
 * 
 * Parâmetros:
 * 		color - identificação de jogador (WHITE ou RED)
 * Retorna:
 * 		total de peças capturadas do jogador (ou 0 se não existirem)
 **/
int bar_captured(int color) {
	if(color==WHITE)
		return hittedWhites;
	if(color==RED)
		return hittedReds;
	return 0;
}

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
bool bar_any(int face1, int face2, int color) {
	bool devolve;
    
    
    switch(color) {
            //para a cor branco vai verificar se existe alguma pca do outro 
            //nas posicoes das faces do dados se sim 
        
    case WHITE: //white    
            if (pieces_at(11+face1, 1)<2 || pieces_at(11+face2, 1)<2)//11enão12
            devolve = true;       
            break;
    case RED: //red    
            if (pieces_at(12-face1, 0)<2 || pieces_at(12-face2, 0)<2)//12enão11
            devolve = true;      
            break;            
    }
	
	return devolve;

}

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
bool game_win(int color) {
	int soma=0;
	if(color==WHITE)//whites
		for(int i = 6;i<12;i++)//percorrer zona white
			soma += pieces_at(i, 0);//somar peças white nessa zona
	if(soma == 15)
		return true;//vitoria white
		
	if(color==RED)//reds
		for(int i = 12;i<18;i++)//percorrer zona red
			soma += pieces_at(i, 1);//somar peças red nessa zona
	if(soma == 15)
		return true;//vitoria red
		
	return false;
}

/**
 * Coloca o jogo no estado inicial.
 * Isto é com as peças de cada jogador colocadas nas casas convencionadas
 * e sem peças capturadas
 **/
void game_init() {
	hittedWhites = 0;// total de peças brancas capturadas 
	hittedReds   = 0; // total de peças vermelhas capturadas

	reds[0]  = 5; // distribuição das peças do jogador RED
	reds[19]  = 3;
	reds[17]  = 5;
	reds[11]  = 2;
	
	whites[6]= 5; // distribuição das peças do jogador WHITE
	whites[4]= 3;
	whites[12]= 2;
	whites[23]= 5;
}
