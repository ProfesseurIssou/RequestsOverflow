#pragma once

#include <SFML/Graphics.hpp>

#include "config.h"

typedef unsigned int uint;                                                                                          //Simplification

class GameOver{
	public:
		/*INIT*/
		GameOver(sf::RenderWindow *prmPtrGameWindow);																		//Constructeur
		~GameOver();																										//Destructeur
		/*####*/

		/*UPDATE*/
		int Update(sf::Vector2f prmMousePos,bool prmMouseLeftClick,uint prmScore);											//Mise a jour du gameover
		/*######*/

	private:
		/*SFML*/
		sf::RenderWindow *ptrGameWindow;																					//Pointeur de la fenetre de jeux

		//Text
		sf::Text textScore;																									//Texte du score
		sf::Text textBack;																									//Texte du bouton de retour au menu

		//Font
		sf::Font fontTetris;																								//Police de caractere Tetris

		/*####*/
};

