#include "Data.h"

#include "GameBlock.h"

/*INIT*/
//Constructeur
Data::Data(sf::RenderWindow *prmPtrGameWindow,std::vector<std::vector<GameBlock>> *prmPtrBoard,ResourcesManager *prmPtrResourcesManager,uint prmStartPosX,uint prmStartPosY,uint prmTargetDevice,std::vector<std::vector<int>> prmPath){
	//Variables
	//Programme
	ptrResoucesManager = prmPtrResourcesManager;															//Recuperation du gestionnaire de resources
	ptrGameWindow = prmPtrGameWindow;																		//Recuperation de la fenetre de jeux
	ptrBoard = prmPtrBoard;																					//Recuperation du plateau de jeux

	currentPos = 1;																							//Premiere etape (vers la deuxieme case)
	currentStep = 0;																						//Etape actuel dans le deplacement entre deux case
	path = prmPath;																							//Recuperation du PATH

	dataSprite.setTexture(ptrResoucesManager->gameData.textureData);										//Definition de la texture du sprite
}
//Destructeur
Data::~Data(){
	//Variables
	//Programme
}
/*####*/

/*UPDATE*/
//Mise a jour de la data (retourne 1 si elle a fini (destruction), retourne 0 si elle n'a pas fini)
bool Data::Update(){
	//Variables
	bool pathFinished;																						//Si la données est arrivé a destination
	float currentBlockPosX;																					//Position du block actuelle sur la fenetre
	float currentBlockPosY;																					//Position du block actuelle sur la fenetre
	float nextBlockPosX;																					//Position du block suivant sur la fenetre
	float nextBlockPosY;																					//Position du block suivant sur la fenetre
	//Programme
	pathFinished = false;																					//Par defaut non

	/*MOVE*/
	if(currentStep < DATA_NB_STEPS){																		//Si on n'est pas encore arrivé a la nouvelle case
		currentBlockPosX = BLOCK_SIZE_X*path[currentPos-1][0];													//Position central du block actuel
		currentBlockPosY = BLOCK_SIZE_Y*path[currentPos-1][1];													//Position central du block actuel
		nextBlockPosX = BLOCK_SIZE_X*path[currentPos][0];														//Position central du block suivant
		nextBlockPosY = BLOCK_SIZE_Y*path[currentPos][1];														//Position central du block suivant

		dataSprite.setPosition(																					//Deplacement de la donnée
			currentBlockPosX+(((nextBlockPosX-currentBlockPosX)/DATA_NB_STEPS)*currentStep),						//Nouvelle position X en fonction du step actuel
			currentBlockPosY+(((nextBlockPosY-currentBlockPosY)/DATA_NB_STEPS)*currentStep)							//Nouvelle position Y en fonction du step actuel
			);
		currentStep++;																							//Etape suivante
	}else{																									//Sinon
		currentPos++;																							//Etape du path suivant
		currentStep = 0;																						//Etape de deplacement a 0
	}

	/*CHECK*/
	if(currentPos == path.size()){																			//Si on arrive a destination
		pathFinished = true;																					//La donnée a fini
		(*ptrBoard)[path[currentPos-1][1]][path[currentPos-1][0]].AddOneData();									//On stock la données dans la case final
	}

	/*RENDER*/
	ptrGameWindow->draw(dataSprite);																		//Affichage du sprite de la data

	return pathFinished;																					//Retourne si la donnée a fini
}
/*######*/