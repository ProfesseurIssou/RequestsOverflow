#pragma once

#include <SFML/Graphics.hpp>

#include "config.h"

typedef unsigned int uint;                                                                                          //Simplification

class MainMenu{
	public:
		/*INIT*/
		MainMenu(sf::RenderWindow *prmPtrGameWindow);																		//Constructeur
		~MainMenu();																										//Destructeur
		/*####*/

		/*UPDATE*/
		int Update(sf::Vector2f prmMousePos,bool prmMouseLeftClick);														//Mise a jour du menu principale
		/*######*/

	private:
		/*SFML*/
		sf::RenderWindow *ptrGameWindow;																					//Pointeur de la fenetre de jeux
		
		//Text
		sf::Text textDifficultyLevel;																						//Texte de la selection de la difficulté
		sf::Text textEasyDifficultyLevel;																					//Texte de la selection de la difficulté facile
		sf::Text textMediumDifficultyLevel;																					//Texte de la selection de la difficulté moyenne
		sf::Text textMasterDifficultyLevel;																					//Texte de la selection de la difficulté master
		
		//Font
		sf::Font fontTetris;																								//Police de caractere Tetris
																															
		/*####*/
};

