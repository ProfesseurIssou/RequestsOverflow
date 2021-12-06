#include "MainMenu.h"


/*INIT*/
//Constructeur
MainMenu::MainMenu(sf::RenderWindow *prmPtrGameWindow){
	//Variables
	//Programme
	ptrGameWindow = prmPtrGameWindow;																							//On enregistre le pointeur de la fenetre de jeux

	//Text
	textDifficultyLevel.setString("Difficulté");																				//Definition du texte du menu
	textDifficultyLevel.setCharacterSize(30);																					//Definition de la taille du texte
	textDifficultyLevel.setFillColor(sf::Color::White);																			//Definition de la couleur du texte
	textDifficultyLevel.setOrigin(textDifficultyLevel.getLocalBounds().width/2.0f,(textDifficultyLevel.getLocalBounds().height/2.0f));//Definition de l'origine du texte en sont centre
	textDifficultyLevel.setFont(fontTetris);																					//Definition de la police de caractere

	textEasyDifficultyLevel.setString("Facile");																				//Definition du texte du menu
	textEasyDifficultyLevel.setCharacterSize(30);																				//Definition de la taille du texte
	textEasyDifficultyLevel.setFillColor(sf::Color::White);																		//Definition de la couleur du texte
	textEasyDifficultyLevel.setOrigin(textEasyDifficultyLevel.getLocalBounds().width/2.0f,(textEasyDifficultyLevel.getLocalBounds().height/2.0f));//Definition de l'origine du texte en sont centre
	textEasyDifficultyLevel.setFont(fontTetris);																				//Definition de la police de caractere
	
	textMediumDifficultyLevel.setString("Moyenne");																				//Definition du texte du menu
	textMediumDifficultyLevel.setCharacterSize(30);																				//Definition de la taille du texte
	textMediumDifficultyLevel.setFillColor(sf::Color::White);																	//Definition de la couleur du texte
	textMediumDifficultyLevel.setOrigin(textMediumDifficultyLevel.getLocalBounds().width/2.0f,(textMediumDifficultyLevel.getLocalBounds().height/2.0f));//Definition de l'origine du texte en sont centre
	textMediumDifficultyLevel.setFont(fontTetris);																				//Definition de la police de caractere
	
	textMasterDifficultyLevel.setString("Master");																				//Definition du texte du menu
	textMasterDifficultyLevel.setCharacterSize(30);																				//Definition de la taille du texte
	textMasterDifficultyLevel.setFillColor(sf::Color::White);																	//Definition de la couleur du texte
	textMasterDifficultyLevel.setOrigin(textMasterDifficultyLevel.getLocalBounds().width/2.0f,(textMasterDifficultyLevel.getLocalBounds().height/2.0f));//Definition de l'origine du texte en sont centre
	textMasterDifficultyLevel.setFont(fontTetris);																				//Definition de la police de caractere

	//Font
	fontTetris.loadFromFile("Font/Tetris.ttf");

}
//Destructeur
MainMenu::~MainMenu(){
	//Variables
	//Programme
}
/*####*/

/*UPDATE*/
//Mise a jour du menu principale
int MainMenu::Update(sf::Vector2f prmMousePos,bool prmMouseLeftClick){
	//Variables
	int nextScene;																												//Prochaine scene (-1 = ça ne bouge pas)
	uint gameWindowSizeW;																										//Largeur de la fenetre 
	uint gameWindowSizeH;																										//Hauteur de la fenetre
	
	//Programme
	nextScene = SCENE_MAINMENU; 																								//La scene reste la même
	gameWindowSizeW = ptrGameWindow->getSize().x;																				//Recuperation de la largeur de la fenetre
	gameWindowSizeH = ptrGameWindow->getSize().y;																				//Recuperation de la hauteur de la fenetre

	//Texte
	textDifficultyLevel.setOrigin(textDifficultyLevel.getLocalBounds().width/2.0f,(textDifficultyLevel.getLocalBounds().height/2.0f));//Definition de l'origine du texte en sont centre
	textDifficultyLevel.setPosition((gameWindowSizeW/2),(gameWindowSizeH/2)*0.6);												//On met le texte au centre haut de l'ecran

	textEasyDifficultyLevel.setOrigin(textEasyDifficultyLevel.getLocalBounds().width/2.0f,(textEasyDifficultyLevel.getLocalBounds().height/2.0f));//Definition de l'origine du texte en sont centre
	textEasyDifficultyLevel.setPosition((gameWindowSizeW/2),(gameWindowSizeH/2)*0.8);											//On met le texte au centre haut de l'ecran
	if(textEasyDifficultyLevel.getGlobalBounds().contains(prmMousePos.x,prmMousePos.y)){										//Si le curseur est sur le texte
		textEasyDifficultyLevel.setFillColor(sf::Color::Green);																		//On met le texte en vert
		if(prmMouseLeftClick){																										//Si on clique dessus
			nextScene = SCENE_GAME_EASY;																								//La prochaine scene est le jeu en facile
		}
	}else{																														//Sinon
		textEasyDifficultyLevel.setFillColor(sf::Color::White);																		//On met le texte en blanc
	}

	textMediumDifficultyLevel.setOrigin(textMediumDifficultyLevel.getLocalBounds().width/2.0f,(textMediumDifficultyLevel.getLocalBounds().height/2.0f));//Definition de l'origine du texte en sont centre
	textMediumDifficultyLevel.setPosition((gameWindowSizeW/2),(gameWindowSizeH/2));												//On met le texte au centre haut de l'ecran
	if(textMediumDifficultyLevel.getGlobalBounds().contains(prmMousePos.x,prmMousePos.y)){										//Si le curseur est sur le texte
		textMediumDifficultyLevel.setFillColor(sf::Color::Blue);																	//On met le texte en bleu
		if(prmMouseLeftClick){																										//Si on clique dessus
			nextScene = SCENE_GAME_MEDIUM;																								//La prochaine scene est le jeu en facile
		}
	}else{																														//Sinon
		textMediumDifficultyLevel.setFillColor(sf::Color::White);																	//On met le texte en blanc
	}

	textMasterDifficultyLevel.setOrigin(textMasterDifficultyLevel.getLocalBounds().width/2.0f,(textMasterDifficultyLevel.getLocalBounds().height/2.0f));//Definition de l'origine du texte en sont centre
	textMasterDifficultyLevel.setPosition((gameWindowSizeW/2),(gameWindowSizeH/2)*1.2);											//On met le texte au centre haut de l'ecran
	if(textMasterDifficultyLevel.getGlobalBounds().contains(prmMousePos.x,prmMousePos.y)){										//Si le curseur est sur le texte
		textMasterDifficultyLevel.setFillColor(sf::Color::Red);																		//On met le texte en red
		if(prmMouseLeftClick){																										//Si on clique dessus
			nextScene = SCENE_GAME_MASTER;																								//La prochaine scene est le jeu en facile
		}
	} else{																														//Sinon
		textMasterDifficultyLevel.setFillColor(sf::Color::White);																	//On met le texte en blanc
	}

	//Render
	ptrGameWindow->draw(textDifficultyLevel);																					//Affichage du texte
	ptrGameWindow->draw(textEasyDifficultyLevel);																				//Affichage du texte
	ptrGameWindow->draw(textMediumDifficultyLevel);																				//Affichage du texte
	ptrGameWindow->draw(textMasterDifficultyLevel);																				//Affichage du texte

	return nextScene;																											//On retourne la prochaine scene
}
/*######*/