#include "GameOver.h"

/*INIT*/
GameOver::GameOver(sf::RenderWindow *prmPtrGameWindow){
	//Variables
	//Programme
	ptrGameWindow = prmPtrGameWindow;																							//On enregistre le pointeur de la fenetre de jeux

	//Text
	textScore.setString("0");																									//Definition du texte du score
	textScore.setCharacterSize(30);																								//Definition de la taille du texte
	textScore.setFillColor(sf::Color::White);																					//Definition de la couleur du texte
	textScore.setOrigin(textScore.getLocalBounds().width/2.0f,(textScore.getLocalBounds().height/2.0f));						//Definition de l'origine du texte en sont centre
	textScore.setFont(fontTetris);																								//Definition de la police de caractere

	textBack.setString("Back");																									//Definition du texte du bouton
	textBack.setCharacterSize(30);																								//Definition de la taille du texte
	textBack.setFillColor(sf::Color::White);																					//Definition de la couleur du texte
	textBack.setOrigin(textBack.getLocalBounds().width/2.0f,(textBack.getLocalBounds().height/2.0f));							//Definition de l'origine du texte en sont centre
	textBack.setFont(fontTetris);																								//Definition de la police de caractere

	//Font
	fontTetris.loadFromFile("Font/Tetris.ttf");
}
GameOver::~GameOver(){}
/*####*/


/*UPDATE*/
//Mise a jour du menu principale
int GameOver::Update(sf::Vector2f prmMousePos,bool prmMouseLeftClick,uint prmScore){
	//Variables
	int nextScene;																												//Prochaine scene (-1 = ça ne bouge pas)
	uint gameWindowSizeW;																										//Largeur de la fenetre 
	uint gameWindowSizeH;																										//Hauteur de la fenetre

	//Programme
	nextScene = SCENE_GAMEOVER; 																								//La scene reste la même
	gameWindowSizeW = ptrGameWindow->getSize().x;																				//Recuperation de la largeur de la fenetre
	gameWindowSizeH = ptrGameWindow->getSize().y;																				//Recuperation de la hauteur de la fenetre

	//Texte
	textScore.setOrigin(textScore.getLocalBounds().width/2.0f,(textScore.getLocalBounds().height/2.0f));						//Definition de l'origine du texte en sont centre
	textScore.setPosition((gameWindowSizeW/2),(gameWindowSizeH/2)*0.6);															//On met le texte au centre haut de l'ecran
	textScore.setString(std::to_string(prmScore));																				//On definie le score
	
	textBack.setOrigin(textBack.getLocalBounds().width/2.0f,(textBack.getLocalBounds().height/2.0f));							//Definition de l'origine du texte en sont centre
	textBack.setPosition((gameWindowSizeW/2),(gameWindowSizeH/2)*0.8);															//On met le texte au centre haut de l'ecran
	if(textBack.getGlobalBounds().contains(prmMousePos.x,prmMousePos.y)){														//Si le curseur est sur le texte
		textBack.setFillColor(sf::Color::Green);																					//On met le texte en vert
		if(prmMouseLeftClick){																										//Si on clique dessus
			nextScene = SCENE_MAINMENU;																									//La prochaine scene est le menu
		}
	} else{																														//Sinon
		textBack.setFillColor(sf::Color::White);																					//On met le texte en blanc
	}


	//Render
	ptrGameWindow->draw(textScore);																								//Affichage du texte
	ptrGameWindow->draw(textBack);																								//Affichage du texte

	return nextScene;																											//On retourne la prochaine scene
}
/*######*/