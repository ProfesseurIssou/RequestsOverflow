#include "GameBlock.h"

/*INIT*/
//Constructeur
GameBlock::GameBlock(sf::RenderWindow *prmPtrGameWindow,uint prmBoardPosX,uint prmBoardPosY,std::vector<std::vector<GameBlock>> *prmPtrBoard,ResourcesManager *prmPtrResourcesManager,std::vector<Data> *prmPtrDataList,uint *prmPtrNbAvailableSwitch,uint *prmPtrNbAvailableRouter,std::vector<StructPathFinding> *prmptrThreadOutputPath){
	//Variables
	//Programme
	ptrGameWindow = prmPtrGameWindow;																				//Pointeur de la fenetre de jeux
	boardPosX = prmBoardPosX;																						//Position X en fonction du plateau
	boardPosY = prmBoardPosY;																						//Position Y en fonction du plateau
	ptrBoard = prmPtrBoard;																							//Pointeur du plateau du jeux
	ptrResoucesManager = prmPtrResourcesManager;																	//Pointeur du gestionnaire de resources
	ptrDataList = prmPtrDataList;																					//Pointeur de la liste des données

	//Sprite
	spriteBlock.setPosition(prmBoardPosX*BLOCK_SIZE_X,prmBoardPosY*BLOCK_SIZE_Y);									//Definition de la position du block
	spriteDevice.setPosition(prmBoardPosX*BLOCK_SIZE_X,prmBoardPosY*BLOCK_SIZE_Y);									//Definition de la position de l'appareil
	spriteWire.setPosition(prmBoardPosX*BLOCK_SIZE_X,prmBoardPosY*BLOCK_SIZE_Y);									//Definition de la position du cable

	//States
	blockHover = false;																								//Par defaut non
	blockSelected = false;																							//Par defaut non
	blockSelectedCache = false;																						//Par defaut non

	keyRHold = false;																								//Par defaut non
	mouseLeftHold = false;																							//Par defaut non

	deviceType = DEVICE_NONE;																						//Par defaut rien
	wayUP = WIRE_NONE;																								//Par defaut non
	wayUPRIGHT = WIRE_NONE;																							//Par defaut non
	wayRIGHT = WIRE_NONE;																							//Par defaut non
	wayRIGHTDOWN = WIRE_NONE;																						//Par defaut non
	wayDOWN = WIRE_NONE;																							//Par defaut non
	wayDOWNLEFT = WIRE_NONE;																						//Par defaut non
	wayLEFT = WIRE_NONE;																							//Par defaut non
	wayLEFTUP = WIRE_NONE;																							//Par defaut non

	//Variables
	nbConnectedCable = 0;																							//Par defaut 0 cable connecté
	nbStoredData = 0;																								//Par defaut 0
	ptrNbAvailableSwitch = prmPtrNbAvailableSwitch;																	//Definition du pointeur
	ptrNbAvailableRouter = prmPtrNbAvailableRouter;																	//Definition du pointeur

	/*THREAD*/
	ptrThreadOutputPath = prmptrThreadOutputPath;																	//Pointeur vers la sortie des path
	threadListIndex = -1;																							//Index du block actuel dans la sortie du thread (-1 par defaut (car pas encore ajouté))
	/*######*/
}
//Destructeur
GameBlock::~GameBlock(){
	//Variables
	//Programme
}
/*####*/


/*UPDATE*/
//Mise a jour du menu principale
bool GameBlock::Update(uint prmDifficulty,sf::Vector2f prmMousePos,bool prmMouseLeftClick,uint prmDestructionMode,uint prmSelectedBuildWire,uint prmSelectedBuildDevice,bool prmKeyR){
	//Variables
	uint i;																											//Compteur de boucle
	std::vector<std::vector<int>> newPath;																			//Path pour les données
	//Programme

	//Check
	blockHover = CursorHover(prmMousePos);																			//Check si le curseur survole le block
	if(!mouseLeftHold){																								//Si le clique n'est pas maintenue
		blockSelected = CursorSelected(prmMouseLeftClick);																//Check si le block est cliqué
	}

	//Process Data
	if(clockDBProcess.getElapsedTime().asMilliseconds()>DB_PROCESS_DATA_MS && deviceType==DEVICE_DB && nbStoredData>0){//Si on depasse le temps de traitement de la db ET que la case est une DB ET qu'il y a au moins 1 données
		GetOneData();																									//On supprime une donnée
		clockDBProcess.restart();																						//On redemmare le temps de traitement
	}
	//Fond du block
	if(blockSelected || blockHover){																				//Si le curseur est sur le bloque
		spriteBlock.setTexture(ptrResoucesManager->gameBlock.textureBlockSelected);										//On definie la texture du fond comme selectionné
	}else{																											//Sinon
		spriteBlock.setTexture(ptrResoucesManager->gameBlock.textureBlockNotSelected);									//On definie la texture du fond comme non selectionné
	}
	spriteBlock.setScale(SCALE_BACKGROUND,SCALE_BACKGROUND);														//On definie le SCALE pour le fond
	ptrGameWindow->draw(spriteBlock);																				//On applique la texture du fond

	//Rotation
	if(blockHover && prmKeyR && !keyRHold){																			//Si block survolé ET R pressé (rotation) ET R n'est pas maintenue
		Rotate();																										//Rotation des cables
	}

	//Mise en place des cables
	if(blockHover && prmMouseLeftClick && prmDestructionMode == DESTRUCTION_WIRE){									//Si le bloc est survolé ET qu'on clique dessus ET on est en mode destruction
		SetWayUP(WIRE_NONE);																							//Pas de cable
		SetWayUPRIGHT(WIRE_NONE);																						//Pas de cable
		SetWayRIGHT(WIRE_NONE);																							//Pas de cable
		SetWayRIGHTDOWN(WIRE_NONE);																						//Pas de cable
		SetWayDOWN(WIRE_NONE);																							//Pas de cable
		SetWayDOWNLEFT(WIRE_NONE);																						//Pas de cable
		SetWayLEFT(WIRE_NONE);																							//Pas de cable
		SetWayLEFTUP(WIRE_NONE);																						//Pas de cable
	}
	if(blockSelected && !blockHover && prmSelectedBuildWire){														//Si le block est selectionné ET que le bloque n'est pas survolé ET qu'un cable est selectionné
		//UP
		if(boardPosY!=0 && (*ptrBoard)[boardPosY-1][boardPosX].CursorHover(prmMousePos)){								//Si on est pas tout en haut ET que le curseur et sur la case du dessus
			if(CableConnexionAvailable() && (*ptrBoard)[boardPosY-1][boardPosX].CableConnexionAvailable()){					//Si on peut connecté la case actuel et la case du dessus
				SetWayUP(WIRE_ADSL);																							//Le block actuel vers le haut
				(*ptrBoard)[boardPosY-1][boardPosX].SetWayDOWN(prmSelectedBuildWire);											//Le block au dessus vers le bas
			}
			blockSelected = false;																							//Le block n'est plus selectionné
			(*ptrBoard)[boardPosY-1][boardPosX].SetSelected(true);															//Le block au dessus est selectionné
		}
		//UPRIGHT
		if(boardPosY!=0 && boardPosX!=(*ptrBoard)[0].size()-1 && (*ptrBoard)[boardPosY-1][boardPosX+1].CursorHover(prmMousePos)){//Si on est pas tout en haut ET qu'on n'est pas tout a droite ET que le curseur et sur la case du dessus a droite
			if(CableConnexionAvailable() && (*ptrBoard)[boardPosY-1][boardPosX+1].CableConnexionAvailable()){				//Si on peut connecté la case actuel et la case du dessus a droite
				SetWayUPRIGHT(WIRE_ADSL);																						//Le block actuel vers le haut droite
				(*ptrBoard)[boardPosY-1][boardPosX+1].SetWayDOWNLEFT(prmSelectedBuildWire);										//Le block en haut drroite vers le bas gauche
			}
			blockSelected = false;																							//Le block n'est plus selectionné
			(*ptrBoard)[boardPosY-1][boardPosX+1].SetSelected(prmSelectedBuildWire);										//Le block au dessus droite est selectionné
		}
		//RIGHT
		if(boardPosX!=(*ptrBoard)[0].size()-1 && (*ptrBoard)[boardPosY][boardPosX+1].CursorHover(prmMousePos)){			//Si on est pas tout a droite ET que le curseur et sur la case de droite
			if(CableConnexionAvailable() && (*ptrBoard)[boardPosY][boardPosX+1].CableConnexionAvailable()){					//Si on peut connecté la case actuel et la case du droite
				SetWayRIGHT(WIRE_ADSL);																							//Le block actuel vers la droite
				(*ptrBoard)[boardPosY][boardPosX+1].SetWayLEFT(prmSelectedBuildWire);											//Le block au dessus vers la gauche
			}
			blockSelected = false;																							//Le block n'est plus selectionné
			(*ptrBoard)[boardPosY][boardPosX+1].SetSelected(prmSelectedBuildWire);											//Le block de droite est selectionné
		}
		//RIGHTDOWN
		if(boardPosY!=(*ptrBoard).size()-1 && boardPosX!=(*ptrBoard)[0].size()-1 && (*ptrBoard)[boardPosY+1][boardPosX+1].CursorHover(prmMousePos)){//Si on est pas tout en bas ET qu'on n'est pas tout a droite ET que le curseur et sur la case du dessus en bas droite
			if(CableConnexionAvailable() && (*ptrBoard)[boardPosY+1][boardPosX+1].CableConnexionAvailable()){				//Si on peut connecté la case actuel et la case droite bas
				SetWayRIGHTDOWN(WIRE_ADSL);																						//Le block actuel vers la droite bas
				(*ptrBoard)[boardPosY+1][boardPosX+1].SetWayLEFTUP(prmSelectedBuildWire);										//Le block en bas a droite vers la gauche haut
			}
			blockSelected = false;																							//Le block n'est plus selectionné
			(*ptrBoard)[boardPosY+1][boardPosX+1].SetSelected(prmSelectedBuildWire);										//Le block en bas a droite est selectionné
		}
		//DOWN
		if(boardPosY!=(*ptrBoard).size()-1 && (*ptrBoard)[boardPosY+1][boardPosX].CursorHover(prmMousePos)){			//Si on est pas tout en bas ET que le curseur et sur la case du dessous
			if(CableConnexionAvailable() && (*ptrBoard)[boardPosY+1][boardPosX].CableConnexionAvailable()){					//Si on peut connecté la case actuel et la case du bas
				SetWayDOWN(WIRE_ADSL);																							//Le block actuel vers le bas
				(*ptrBoard)[boardPosY+1][boardPosX].SetWayUP(prmSelectedBuildWire);												//Le block au dessous vers le haut
			}
			blockSelected = false;																							//Le block n'est plus selectionné
			(*ptrBoard)[boardPosY+1][boardPosX].SetSelected(prmSelectedBuildWire);											//Le block au dessou est selectionné
		}
		//DOWNLEFT
		if(boardPosY!=(*ptrBoard).size()-1 && boardPosX!=0 && (*ptrBoard)[boardPosY+1][boardPosX-1].CursorHover(prmMousePos)){//Si on est pas tout en bas ET qu'on n'est pas tout a gauche ET que le curseur et sur la case du dessous
			if(CableConnexionAvailable() && (*ptrBoard)[boardPosY+1][boardPosX-1].CableConnexionAvailable()){				//Si on peut connecté la case actuel et la case du bas gauche
				SetWayDOWNLEFT(WIRE_ADSL);																						//Le block actuel vers la bas gauche
				(*ptrBoard)[boardPosY+1][boardPosX-1].SetWayUPRIGHT(prmSelectedBuildWire);										//Le block bas gauche vers haut droite
			}
			blockSelected = false;																							//Le block n'est plus selectionné
			(*ptrBoard)[boardPosY+1][boardPosX-1].SetSelected(prmSelectedBuildWire);										//Le block au dessou gauche est selectionné
		}
		//LEFT
		if(boardPosX!=0 && (*ptrBoard)[boardPosY][boardPosX-1].CursorHover(prmMousePos)){								//Si on est pas tout a gauche ET que le curseur et sur la case de droite
			if(CableConnexionAvailable() && (*ptrBoard)[boardPosY][boardPosX-1].CableConnexionAvailable()){					//Si on peut connecté la case actuel et la case gauche
				SetWayLEFT(WIRE_ADSL);																							//Le block actuel vers la gauche
				(*ptrBoard)[boardPosY][boardPosX-1].SetWayRIGHT(prmSelectedBuildWire);											//Le block gauche vers la droite
			}
			blockSelected = false;																							//Le block n'est plus selectionné
			(*ptrBoard)[boardPosY][boardPosX-1].SetSelected(prmSelectedBuildWire);											//Le block a geuche est selectionné
		}
		//LEFTUP
		if(boardPosY!=0 && boardPosX!=0 && (*ptrBoard)[boardPosY-1][boardPosX-1].CursorHover(prmMousePos)){				//Si on est pas tout en haut ET si on est pas tout a droite ET que le curseur et sur la case de gauche haut
			if(CableConnexionAvailable() && (*ptrBoard)[boardPosY-1][boardPosX-1].CableConnexionAvailable()){				//Si on peut connecté la case actuel et la case gauche haut
				SetWayLEFTUP(WIRE_ADSL);																						//Le block actuel vers la gauche haut
				(*ptrBoard)[boardPosY-1][boardPosX-1].SetWayRIGHTDOWN(prmSelectedBuildWire);									//Le block au haut gauche vers la bas droite
			}
			blockSelected = false;																							//Le block n'est plus selectionné
			(*ptrBoard)[boardPosY-1][boardPosX-1].SetSelected(prmSelectedBuildWire);										//Le block au haut a gauche est selectionné
		}
	}
	
	//Mise en place des appareils
	if(blockHover && prmMouseLeftClick && prmDestructionMode == DESTRUCTION_DEVICE){								//Si le bloc est survolé ET qu'on clique dessus ET on est en mode destruction d'appareil
		if(deviceType!=DEVICE_COMPUTER && deviceType!=DEVICE_DB){														//Si l'appareil n'est pas un serveur ni un ordinateur
			if(deviceType==DEVICE_SWITCH)(*ptrNbAvailableSwitch)++;															//Si l'appareil est un switch => plus un switch disponible
			if(deviceType==DEVICE_ROUTER)(*ptrNbAvailableRouter)++;															//Si l'appareil est un routeur => plus un routeur disponible
			deviceType = DEVICE_NONE;																						//On retire l'appareil
		}

		SetWayUP(WIRE_NONE);																							//Pas de cable
		SetWayUPRIGHT(WIRE_NONE);																						//Pas de cable
		SetWayRIGHT(WIRE_NONE);																							//Pas de cable
		SetWayRIGHTDOWN(WIRE_NONE);																						//Pas de cable
		SetWayDOWN(WIRE_NONE);																							//Pas de cable
		SetWayDOWNLEFT(WIRE_NONE);																						//Pas de cable
		SetWayLEFT(WIRE_NONE);																							//Pas de cable
		SetWayLEFTUP(WIRE_NONE);																						//Pas de cable
	}
	if(blockHover && prmMouseLeftClick && prmSelectedBuildDevice && DeviceConnexionAvailable(prmSelectedBuildDevice)){//Si le block est survolé ET un clique ET qu'un appareil est selectionné ET qu'il n'y a encore aucun appareil
		if(prmSelectedBuildDevice == DEVICE_SWITCH && (*ptrNbAvailableSwitch) != 0){									//Si c'est un switch ET si il reste des switch 
			SetDevice(prmSelectedBuildDevice);																				//On definie l'appareil
			(*ptrNbAvailableSwitch)--;																						//On retire un switch
		};
		if(prmSelectedBuildDevice == DEVICE_ROUTER && (*ptrNbAvailableRouter) != 0){									//Si c'est un routeur ET si il reste des switch 
			SetDevice(prmSelectedBuildDevice);																				//On definie l'appareil
			(*ptrNbAvailableRouter)--;																						//On retire un routeur
		};
	}
	
	//Generation des données
	if(RandomFloat(0,1) < prmDifficulty*BLOCK_SPAWN_COEF && deviceType==DEVICE_COMPUTER)AddOneData();				//Si on doit ajouter une données => On ajoute une données
	if(deviceType == DEVICE_COMPUTER && nbStoredData){																//Si la case est un ordiinateur ET qu'il y a des données stocké
		while((*ptrThreadOutputPath)[threadListIndex].pathLock){};														//Tant que le path est bloquer => on attend
		newPath = (*ptrThreadOutputPath)[threadListIndex].outputPath;													//Recuperation du path actuellement calculé
		if(!newPath.empty()){																							//Si on a un PATH
			(*ptrDataList).push_back(Data(ptrGameWindow,ptrBoard,ptrResoucesManager,boardPosX,boardPosY,DATA_TARGET_DB,newPath));//Creation d'une donnée avec le path
			GetOneData();																									//On retire une données du stockage
		}
	}

	spriteWire.setScale(SCALE_WIRE,SCALE_WIRE);																	//On definie le SCALE pour les cables
	//Wire Render
	if(wayUP){
		spriteWire.setTexture(ptrResoucesManager->gameBlock.textureWayADSL_UP);										//On definie la texture
		ptrGameWindow->draw(spriteWire);																				//On applique la texture du cable
	}
	if(wayUPRIGHT){																									//Si cable vers le haut droite
		spriteWire.setTexture(ptrResoucesManager->gameBlock.textureWayADSL_RIGHT_UP);									//On definie la texture
		ptrGameWindow->draw(spriteWire);																				//On applique la texture du cable
	}
	if(wayRIGHT){																									//Si cable vers le droite
		spriteWire.setTexture(ptrResoucesManager->gameBlock.textureWayADSL_RIGHT);										//On definie la texture
		ptrGameWindow->draw(spriteWire);																				//On applique la texture du cable
	}
	if(wayRIGHTDOWN){																								//Si cable vers le droite bas
		spriteWire.setTexture(ptrResoucesManager->gameBlock.textureWayADSL_RIGHT_DOWN);								//On definie la texture
		ptrGameWindow->draw(spriteWire);																				//On applique la texture du cable
	}
	if(wayDOWN){																									//Si cable vers le bas
		spriteWire.setTexture(ptrResoucesManager->gameBlock.textureWayADSL_DOWN);										//On definie la texture
		ptrGameWindow->draw(spriteWire);																				//On applique la texture du cable
	}
	if(wayDOWNLEFT){																								//Si cable vers le bas gauche
		spriteWire.setTexture(ptrResoucesManager->gameBlock.textureWayADSL_LEFT_DOWN);									//On definie la texture
		ptrGameWindow->draw(spriteWire);																				//On applique la texture du cable
	}
	if(wayLEFT){																									//Si cable vers le gauche
		spriteWire.setTexture(ptrResoucesManager->gameBlock.textureWayADSL_LEFT);										//On definie la texture
		ptrGameWindow->draw(spriteWire);																				//On applique la texture du cable
	}
	if(wayLEFTUP){																									//Si cable vers le gauche haut
		spriteWire.setTexture(ptrResoucesManager->gameBlock.textureWayADSL_LEFT_UP);									//On definie la texture
		ptrGameWindow->draw(spriteWire);																				//On applique la texture du cable
	}

	//Device
	switch(deviceType){																								//Pour chaque type d'appareil
		case DEVICE_COMPUTER:																							//Si c'est un ordinateur
			spriteDevice.setScale(SCALE_DEVICE_COMPUTER,SCALE_DEVICE_COMPUTER);												//On definie le SCALE pour l'ordinateur
			if(nbStoredData<=DATA_LIMIT_1)spriteDevice.setTexture(ptrResoucesManager->gameBlock.textureBlockComputer_1);	//Si on est en dessous de la limite 1 => On definie la texture computer 1 (vide)
			if(nbStoredData>DATA_LIMIT_1 && nbStoredData<=DATA_LIMIT_2)spriteDevice.setTexture(ptrResoucesManager->gameBlock.textureBlockComputer_2);//Si on est au dessus de la limite 1 ET en dessous de la limite 2 => On definie la texture computer 2
			if(nbStoredData>DATA_LIMIT_2 && nbStoredData<=DATA_LIMIT_3)spriteDevice.setTexture(ptrResoucesManager->gameBlock.textureBlockComputer_3);//Si on est au dessus de la limite 2 ET en dessous de la limite 3 => On definie la texture computer 3
			if(nbStoredData>DATA_LIMIT_3 && nbStoredData<=DATA_LIMIT_4)spriteDevice.setTexture(ptrResoucesManager->gameBlock.textureBlockComputer_4);//Si on est au dessus de la limite 3 ET en dessous de la limite 4 => On definie la texture computer 4
			ptrGameWindow->draw(spriteDevice);																				//On affiche le block
			break;
		case DEVICE_ROUTER:																								//Si c'est un routeur
			spriteDevice.setScale(SCALE_DEVICE_ROUTER,SCALE_DEVICE_ROUTER);													//On definie le SCALE pour le routeur
			spriteDevice.setTexture(ptrResoucesManager->gameBlock.textureBlockROUTER);										//On definie la texture
			ptrGameWindow->draw(spriteDevice);																				//On affiche le block
			break;
		case DEVICE_SWITCH:																								//Si c'est un switch
			spriteDevice.setScale(SCALE_DEVICE_SWITCH,SCALE_DEVICE_SWITCH);													//On definie le SCALE pour le switch
			spriteDevice.setTexture(ptrResoucesManager->gameBlock.textureBlockSWITCH);										//On definie la texture
			ptrGameWindow->draw(spriteDevice);																				//On affiche le block
			break;
		case DEVICE_DB:																									//Si c'est une db
			spriteDevice.setScale(SCALE_DEVICE_DB,SCALE_DEVICE_DB);															//On definie le SCALE pour la db
			if(nbStoredData<=DATA_LIMIT_1)spriteDevice.setTexture(ptrResoucesManager->gameBlock.textureBlockDB_LEVEL_1);	//Si on est en dessous de la limite 1 => On definie la texture DB 1 (vide)
			if(nbStoredData>DATA_LIMIT_1 && nbStoredData<=DATA_LIMIT_2)spriteDevice.setTexture(ptrResoucesManager->gameBlock.textureBlockDB_LEVEL_2);//Si on est au dessus de la limite 1 ET en dessous de la limite 2 => On definie la texture DB 2
			if(nbStoredData>DATA_LIMIT_2 && nbStoredData<=DATA_LIMIT_3)spriteDevice.setTexture(ptrResoucesManager->gameBlock.textureBlockDB_LEVEL_3);//Si on est au dessus de la limite 2 ET en dessous de la limite 3 => On definie la texture DB 3
			if(nbStoredData>DATA_LIMIT_3 && nbStoredData<=DATA_LIMIT_4)spriteDevice.setTexture(ptrResoucesManager->gameBlock.textureBlockDB_LEVEL_4);//Si on est au dessus de la limite 3 ET en dessous de la limite 4 => On definie la texture DB 4
			//GAMEOVER##########################################
			ptrGameWindow->draw(spriteDevice);																				//On affiche le block
			break;
	}

	//Return
	spriteBlock.setScale(1,1);																						//On definie le SCALE par defaut
	spriteDevice.setScale(1,1);																						//On definie le SCALE par defaut
	spriteWire.setScale(1,1);																						//On definie le SCALE par defaut
	blockSelectedCache = blockSelected;																				//On definie l'ancien etat
	keyRHold = prmKeyR;																								//On definie l'ancien etat
	mouseLeftHold = prmMouseLeftClick;																				//On definue l'ancien etat
	return (nbStoredData>DATA_LIMIT_4);																				//Fin
}
/*######*/

/*CHECK*/
//Si le curseur est dessus
bool GameBlock::CursorHover(sf::Vector2f prmMousePos){
	//Variables
	sf::FloatRect hitbox;																							//Hitbox de la case
	float offset;																									//Offset de decallage pour centré la hitbox
	//Programme
	hitbox = spriteBlock.getGlobalBounds();																			//Recuperation de la hitbox actuel
	offset = hitbox.width * 0.3;																					//Decallage
	hitbox.width = hitbox.width-offset;																				//Application du decallage vers la gauche
	hitbox.left = hitbox.left+(offset/2);																			//Application du decallage vers la droite
	hitbox.height = hitbox.height-offset;																			//Application du decallage vers le haut
	hitbox.top = hitbox.top+(offset/2);																				//Application du decallage vers le bas

	//return spriteBlock.getGlobalBounds().contains(prmMousePos.x,prmMousePos.y);										//Retourne si le curseur est sur le sprite
	return hitbox.contains(prmMousePos.x,prmMousePos.y);															//Retourne si le curseur est sur le sprite
}
//Si le bloc est cliqué
bool GameBlock::CursorSelected(bool prmMouseLeftClick){
	//Variables
	bool isSelected;																								//Si le bloque est selectionné
	//Programme
	isSelected = false;																								//Par defaut non

	if(blockHover && prmMouseLeftClick){																			//Si le bloque est survolé et que le clique est fait
		isSelected = true;																								//Bloque selectionné
	}
	return isSelected;
}
//Si on peut connecté encore un cable
bool GameBlock::CableConnexionAvailable(){
	//Variables
	uint connexionAvailable;																						//Si on peut connecté un cable
	//Programme
	connexionAvailable = false;																						//Par defaut non
	switch(deviceType){																								//Pour chaque type d'appareil
		case DEVICE_NONE:																								//Aucun appareil
			if(nbConnectedCable < 2)connexionAvailable = true;																//Si il y a moins de 2 cable connecté => On peut connecté un cable
			break;
		case DEVICE_COMPUTER:																							//Ordinateur
			if(nbConnectedCable == 0)connexionAvailable = true;																//Si il n'y a aucun cable connecté => On peut connecté un cable
			break;
		case DEVICE_DB:																									//DB
			if(nbConnectedCable == 0)connexionAvailable = true;																//Si il n'y a aucun cable connecté => On peut connecté un cable
			break;
		case DEVICE_ROUTER:																								//Router
			if(nbConnectedCable < 3)connexionAvailable = true;																//Si il y a moins de 3 cable connecté => On peut connecté un cable
			break;
		case DEVICE_SWITCH:																								//SWITCH
			if(nbConnectedCable < 5)connexionAvailable = true;																//Si il y a moins de 5 cable connecté => On peut connecté un cable
			break;
	}
	return connexionAvailable;																						//On retourne si on peut connecter un cable
}
//Si on peut placé un appareil
bool GameBlock::DeviceConnexionAvailable(uint prmDeviceType){
	//Variables
	uint connexionAvailable;																						//Si on peut connecter un appareil
	//Programme
	connexionAvailable = false;																						//Par defaut non
	switch(prmDeviceType){																							//Pour chaque type d'appareil d'entré
		case DEVICE_NONE:																								//Aucun appareil
			connexionAvailable = true;																						//On peut retirer l'appareil
			break;
		case DEVICE_COMPUTER:																							//Ordinateur
			if(deviceType == DEVICE_NONE)connexionAvailable = true;															//Si il n'y a pas d'appareil sur la case => On peut connecté un ordinateur
			break;
		case DEVICE_DB:																									//DB
			if(deviceType == DEVICE_NONE)connexionAvailable = true;															//Si il n'y a pas d'appareil sur la case => On peut connecté une db
			break;
		case DEVICE_ROUTER:																								//Router
			if(nbConnectedCable <= 3 && deviceType != DEVICE_COMPUTER && deviceType != DEVICE_DB)connexionAvailable = true;	//Si il y a 3 cable max connecté ET que le block n'est pas un ordinateur ET n'est pas une db => On peut connecté une db
			break;
		case DEVICE_SWITCH:																								//SWITCH
			if(nbConnectedCable <= 5 && deviceType != DEVICE_COMPUTER && deviceType != DEVICE_DB)connexionAvailable = true;	//Si il y a 5 cable max connecté ET que le block n'est pas un ordinateur ET n'est pas une db => On peut connecté une db
			break;
	}
	return connexionAvailable;																						//On retourne si on peut connecter un cable
}
/*#####*/

/*SETTER*/
//Definir le cablage vers le haut
void GameBlock::SetWayUP(bool prmCableType){
	//Variables
	//Programme
	if(!wayUP && prmCableType){																						//Si wayUP vide ET le nouveau cable definie
		wayUP = prmCableType;																							//Definition du nouveau cable
		nbConnectedCable++;																								//+1 cable
	}
	else if(wayUP && !prmCableType){																				//Si wayUP definie ET le nouveau cable vide
		wayUP = prmCableType;																							//Definition du nouveau cable
		nbConnectedCable--;																								//-1 cable
		if((*ptrBoard)[boardPosY-1][boardPosX].GetWayDOWN())(*ptrBoard)[boardPosY-1][boardPosX].SetWayDOWN(WIRE_NONE);	//Si le block du dessus vers le bas => le block du dessus vers le bas retiré
	}
}
//Definir le cablage vers le haut-droite
void GameBlock::SetWayUPRIGHT(bool prmCableType){
	//Variables
	//Programme
	if(!wayUPRIGHT && prmCableType){																				//Si wayUPRIGHT vide ET le nouveau cable definie
		wayUPRIGHT = prmCableType;																						//Definition du nouveau cable
		nbConnectedCable++;																								//+1 cable
	}
	if(wayUPRIGHT && !prmCableType){																				//Si wayUPRIGHT definie ET le nouveau cable vide
		wayUPRIGHT = prmCableType;																						//Definition du nouveau cable
		nbConnectedCable--;																								//-1 cable
		if((*ptrBoard)[boardPosY-1][boardPosX+1].GetWayDOWNLEFT())(*ptrBoard)[boardPosY-1][boardPosX+1].SetWayDOWNLEFT(WIRE_NONE);//Si block haut droite vers bas gauche => retiré
	}

}
//Definir le cablage vers le droite
void GameBlock::SetWayRIGHT(bool prmCableType){
	//Variables
	//Programme
	if(!wayRIGHT && prmCableType){																					//Si wayRIGHT vide ET le nouveau cable definie
		wayRIGHT = prmCableType;																						//Definition du nouveau cable
		nbConnectedCable++;																								//+1 cable
	}
	if(wayRIGHT && !prmCableType){																					//Si wayRIGHT definie ET le nouveau cable vide
		wayRIGHT = prmCableType;																						//Definition du nouveau cable
		nbConnectedCable--;																								//-1 cable
		if((*ptrBoard)[boardPosY][boardPosX+1].GetWayLEFT())(*ptrBoard)[boardPosY][boardPosX+1].SetWayLEFT(WIRE_NONE);	//Si block droite vers gauche => retiré
	}
}
//Definir le cablage vers le droite-bas
void GameBlock::SetWayRIGHTDOWN(bool prmCableType){
	//Variables
	//Programme
	if(!wayRIGHTDOWN && prmCableType){																				//Si wayRIGHTDOWN vide ET le nouveau cable definie
		wayRIGHTDOWN = prmCableType;																					//Definition du nouveau cable
		nbConnectedCable++;																								//+1 cable
	}
	if(wayRIGHTDOWN && !prmCableType){																				//Si wayRIGHTDOWN definie ET le nouveau cable vide
		wayRIGHTDOWN = prmCableType;																					//Definition du nouveau cable
		nbConnectedCable--;																								//-1 cable
		if((*ptrBoard)[boardPosY+1][boardPosX+1].GetWayLEFTUP())(*ptrBoard)[boardPosY+1][boardPosX+1].SetWayLEFTUP(WIRE_NONE);//Si block bas a droite => retiré
	}
}
//Definir le cablage vers le bas
void GameBlock::SetWayDOWN(bool prmCableType){
	//Variables
	//Programme
	if(!wayDOWN && prmCableType){																					//Si wayDOWN vide ET le nouveau cable definie
		wayDOWN = prmCableType;																							//Definition du nouveau cable
		nbConnectedCable++;																								//+1 cable
	}
	if(wayDOWN && !prmCableType){																					//Si wayDOWN definie ET le nouveau cable vide
		wayDOWN = prmCableType;																							//Definition du nouveau cable
		nbConnectedCable--;																								//-1 cable
		if((*ptrBoard)[boardPosY+1][boardPosX].GetWayUP())(*ptrBoard)[boardPosY+1][boardPosX].SetWayUP(WIRE_NONE);		//Si block dessous vers haut => retiré
	}
}
//Definir le cablage vers le bas-gauche
void GameBlock::SetWayDOWNLEFT(bool prmCableType){
	//Variables
	//Programme
	if(!wayDOWNLEFT && prmCableType){																				//Si wayDOWNLEFT vide ET le nouveau cable definie
		wayDOWNLEFT = prmCableType;																						//Definition du nouveau cable
		nbConnectedCable++;																								//+1 cable
	}
	if(wayDOWNLEFT && !prmCableType){																				//Si wayDOWNLEFT definie ET le nouveau cable vide
		wayDOWNLEFT = prmCableType;																						//Definition du nouveau cable
		nbConnectedCable--;																								//-1 cable
		if((*ptrBoard)[boardPosY+1][boardPosX-1].GetWayUPRIGHT())(*ptrBoard)[boardPosY+1][boardPosX-1].SetWayUPRIGHT(WIRE_NONE);//Si block bas gauche vers haut droite => retiré
	}
}
//Definir le cablage vers le gauche
void GameBlock::SetWayLEFT(bool prmCableType){
	//Variables
	//Programme
	if(!wayLEFT && prmCableType){																					//Si wayLEFT vide ET le nouveau cable definie
		wayLEFT = prmCableType;																							//Definition du nouveau cable
		nbConnectedCable++;																								//+1 cable
	}
	if(wayLEFT && !prmCableType){																					//Si wayLEFT definie ET le nouveau cable vide
		wayLEFT = prmCableType;																							//Definition du nouveau cable
		nbConnectedCable--;																								//-1 cable
		if((*ptrBoard)[boardPosY][boardPosX-1].GetWayRIGHT())(*ptrBoard)[boardPosY][boardPosX-1].SetWayRIGHT(WIRE_NONE);//Si block gauche vers droite => retiré
	}
}
//Definir le cablage vers le gauche-haut
void GameBlock::SetWayLEFTUP(bool prmCableType){
	//Variables
	//Programme
	if(!wayLEFTUP && prmCableType){																					//Si wayLEFTUP vide ET le nouveau cable definie
		wayLEFTUP = prmCableType;																						//Definition du nouveau cable
		nbConnectedCable++;																								//+1 cable
	}
	if(wayLEFTUP && !prmCableType){																					//Si wayLEFTUP definie ET le nouveau cable vide
		wayLEFTUP = prmCableType;																						//Definition du nouveau cable
		nbConnectedCable--;																								//-1 cable
		if((*ptrBoard)[boardPosY-1][boardPosX-1].GetWayRIGHTDOWN())(*ptrBoard)[boardPosY-1][boardPosX-1].SetWayRIGHTDOWN(WIRE_NONE);//Si block haut gauche vers bas droite => retiré
	}
}
//Definir la selection du block
void GameBlock::SetSelected(bool prmSelected){
	//Variables
	//Programme
	blockSelected = prmSelected;																					//Definition de la selection du block
}
//Definit le type d'appareil
void GameBlock::SetDevice(uint prmDeviceType){
	//Variables
	//Programme
	switch(deviceType){																											//Pour chaque type d'appareil
		case DEVICE_SWITCH:																											//Si c'est un switch
			(*ptrNbAvailableSwitch)++;																									//On rend le switch
			break;
		case DEVICE_ROUTER:																											//Si c'est un routeur
			(*ptrNbAvailableRouter)++;																									//On rend le routeur
			break;
	}
	deviceType = prmDeviceType;																									//On definie le nouvel appareil

	if(deviceType==DEVICE_COMPUTER){																							//Si l'appareil est un ordinateur
		SetWayUP(WIRE_NONE);																										//Pas de cable
		SetWayUPRIGHT(WIRE_NONE);																									//Pas de cable
		SetWayRIGHT(WIRE_NONE);																										//Pas de cable
		SetWayRIGHTDOWN(WIRE_NONE);																									//Pas de cable
		SetWayDOWN(WIRE_NONE);																										//Pas de cable
		SetWayDOWNLEFT(WIRE_NONE);																									//Pas de cable
		SetWayLEFT(WIRE_NONE);																										//Pas de cable
		SetWayLEFTUP(WIRE_NONE);																									//Pas de cable
		if(threadListIndex==-1){																									//Si le calcul pour ce block n'est pas defini
			threadListIndex = (*ptrThreadOutputPath).size();																			//On definie la position du block dans la recherche
			StructPathFinding temp;																										//Instance de la case dans le pathfinder
			temp.findMode = FINDER_MODE_TO_SERVER;																						//Definition du type de recherche
			temp.posX = boardPosX;																										//Definition de la position de départ
			temp.posY = boardPosY;																										//Definition de la position finale
			temp.pathLock = false;																										//Pas de lock par defaut
			(*ptrThreadOutputPath).push_back(temp);																						//Ajout de l'instance de recherche dans le pathfinder
		}
	}
	if(deviceType==DEVICE_DB){																									//Si l'appareil est un serveur
		SetWayUP(WIRE_NONE);																										//Pas de cable
		SetWayUPRIGHT(WIRE_NONE);																									//Pas de cable
		SetWayRIGHT(WIRE_NONE);																										//Pas de cable
		SetWayRIGHTDOWN(WIRE_NONE);																									//Pas de cable
		SetWayDOWN(WIRE_NONE);																										//Pas de cable
		SetWayDOWNLEFT(WIRE_NONE);																									//Pas de cable
		SetWayLEFT(WIRE_NONE);																										//Pas de cable
		SetWayLEFTUP(WIRE_NONE);																									//Pas de cable
		if(threadListIndex==-1){																									//Si le calcul pour ce block n'est pas defini
			threadListIndex = (*ptrThreadOutputPath).size();																			//On definie la position du block dans la recherche
			StructPathFinding temp;																										//Instance de la case dans le pathfinder
			temp.findMode = FINDER_MODE_TO_COMPUTER;																					//Definition du type de recherche
			temp.posX = boardPosX;																										//Definition de la position de départ
			temp.posY = boardPosY;																										//Definition de la position finale
			temp.pathLock = false;																										//Pas de lock par defaut
			(*ptrThreadOutputPath).push_back(temp);																						//Ajout de l'instance de recherche dans le pathfinder
		}
	}
}
//Ajouter une données dans le bloc
void GameBlock::AddOneData(){
	//Variables
	//Programme
	nbStoredData++;																												//On ajoute une donnée
}
//Ajouter une données dans le bloc
void GameBlock::GetOneData(){
	//Variables
	//Programme
	nbStoredData--;																												//On retire une donnée
}
/*######*/

/*GETTER*/
//Recuperer le cablage vers le haut
bool GameBlock::GetWayUP(){
	//Variables
	//Programme
	return wayUP;
}
//Recuperer le cablage vers le haut-droite
bool GameBlock::GetWayUPRIGHT(){
	//Variable
	//Programme
	return wayUPRIGHT;
}
//Recuperer le cablage vers le droite
bool GameBlock::GetWayRIGHT(){
	//Variables
	//Programme
	return wayRIGHT;
}
//Recuperer le cablage vers le droite-bas
bool GameBlock::GetWayRIGHTDOWN(){
	//Variables
	//Progeamme
	return wayRIGHTDOWN;
}
//Recuperer le cablage vers le bas
bool GameBlock::GetWayDOWN(){
	//Variables
	//Programme
	return wayDOWN;
}
//Recuperer le cablage vers le bas-gauche
bool GameBlock::GetWayDOWNLEFT(){
	//Variables
	//Programme
	return wayDOWNLEFT;
}
//Recuperer le cablage vers le gauche
bool GameBlock::GetWayLEFT(){
	//Variables
	//Programme
	return wayLEFT;
}
//Recuperer le cablage vers le gauche-haut
bool GameBlock::GetWayLEFTUP(){
	//Variables
	//Programme
	return wayLEFTUP;
}
//Recuperer le type d'appareil
uint GameBlock::GetDeviceType(){
	//Variables
	//Programme
	return deviceType;
}
//Recuperer le nombre de données actuellement stocké
uint GameBlock::GetNbStoredData(){
	//Variables
	//Programme
	return nbStoredData;
}
/*######*/


/*OTHER*/
//Rotation des fils
void GameBlock::Rotate(){
	//Variables
	uint oldPosUP,oldPosUPRIGHT,oldPosRIGHT,oldPosRIGHTDOWN,oldPosDOWN,oldPosDOWNLEFT,oldPosLEFT,oldPosLEFTUP;										//Ancienne positions des cables
	//Programme
	oldPosUP = wayUP;																																//Haut
	oldPosUPRIGHT = wayUPRIGHT;																														//Haut Droite
	oldPosRIGHT = wayRIGHT;																															//Droite
	oldPosRIGHTDOWN = wayRIGHTDOWN;																													//Droite Bas
	oldPosDOWN = wayDOWN;																															//Bas
	oldPosDOWNLEFT = wayDOWNLEFT;																													//Bas Gauche
	oldPosLEFT = wayLEFT;																															//Gauche
	oldPosLEFTUP = wayLEFTUP;																														//Gauche Haut

	wayUP = oldPosLEFTUP;																															//Rotation
	wayUPRIGHT = oldPosUP;																															//Rotation
	wayRIGHT = oldPosUPRIGHT;																														//Rotation
	wayRIGHTDOWN = oldPosRIGHT;																														//Rotation
	wayDOWN = oldPosRIGHTDOWN;																														//Rotation
	wayDOWNLEFT = oldPosDOWN;																														//Rotation
	wayLEFT = oldPosDOWNLEFT;																														//Rotation
	wayLEFTUP = oldPosLEFT;																															//Rotation

	return;																																			//Retour
}
/*#####*/