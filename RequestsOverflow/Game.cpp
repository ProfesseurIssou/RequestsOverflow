#include "Game.h"

/*INIT*/
//Constructeur
Game::Game(sf::RenderWindow *prmPtrGameWindow,ResourcesManager *prmPtrResoucesManager,sf::View *prmPtrGameView){
	//Variables
	uint x,y;																													//Compteur de boucle
	std::vector<GameBlock> boardLine;																							//Generateur de la ligne

	//Programme
	srand((unsigned)time(NULL));																								//Seed aleatoire

	/*TEXT*/
	fontTetris.loadFromFile("Font/Tetris.ttf");

	textNbAvailableSwitch.setFont(fontTetris);																					//Definition de la police
	textNbAvailableSwitch.setString("0");																						//Texte par defaut
	textNbAvailableSwitch.setCharacterSize(300);																				//Definition de la taille du texte
	textNbAvailableSwitch.setFillColor(sf::Color::White);																		//Definition de la couleur du texte

	textNbAvailableRouter.setFont(fontTetris);																					//Definition de la police
	textNbAvailableRouter.setString("0");																						//Texte par defaut
	textNbAvailableRouter.setCharacterSize(300);																				//Definition de la taille du texte
	textNbAvailableRouter.setFillColor(sf::Color::White);																		//Definition de la couleur du texte

	textMinutesInGame.setFont(fontTetris);																						//Definition de la police
	textMinutesInGame.setString("0:0");																							//Texte par defaut
	textMinutesInGame.setCharacterSize(300);																					//Definition de la taille du texte
	textMinutesInGame.setFillColor(sf::Color::White);																			//Definition de la couleur du texte
	/*####*/

	ptrGameWindow = prmPtrGameWindow;																							//On enregistre le pointeur de la fenetre de jeux
	ptrResoucesManager = prmPtrResoucesManager;																					//On enregistre le pointeur du gestionnaire des resources
	ptrGameView = prmPtrGameView;																								//On enregistre le pointeur de la vie du joueur


	/*THREAD*/
	threadRunning = true;																										//On fait tourner le thread
	ptrThreadPathFinding = new sf::Thread(std::bind(&ThreadPathFinder,&board,&threadOutputPath,&threadRunning));				//Creation du thread pour le pathfinding
	ptrThreadPathFinding->launch();																								//On lance le thread
	/*######*/

	/*DIFFICULTY*/
	gameClock.restart();																										//Redémarrage de l'horloge
	gameMinutes = 0;																											//Score de 0
	currentDifficulty = 0;																										//Difficulté actuel
	/*##########*/

	/*DEVICES*/
	nbAvailableRouter = 0;																										//0 routeur disponible
	nbAvailableSwitch = 0;																										//0 switch disponible
	/*#######*/

	/*MOUSE*/
	mouseLeftClickHold = false;																									//Le clique de sourie n'est pas maintenue
	mouseLeftClickUsedByOverMenu = false;																						//Le clique de sourie n'est pas utilisé par un menu superposant
	mouseRightClickHold = false;																								//Le clique de sourie n'est pas maintenue
	/*#####*/

	/*MODE*/
	destructionMode = DESTRUCTION_NONE;																							//Pas en mode destruction
	selectedBuildWire = WIRE_NONE;																								//Pas de cable selectionné pour la construction
	selectedBuildDevice = DEVICE_NONE;																							//Pas d'appareil selectionné pour la construction
	/*####*/

	for(y=0;y<BOARD_SIZE_Y;y++){																								//Pour chaque ligne du plateau
		boardLine.clear();																											//On efface la ligne de génération
		for(x=0;x<BOARD_SIZE_X;x++){																								//Pour chaque case de chaque ligne
			boardLine.push_back(GameBlock(ptrGameWindow,x,y,&board,ptrResoucesManager,&dataList,&nbAvailableSwitch,&nbAvailableRouter,&threadOutputPath));//On ajoute le block a la liste temporaire
		}
		board.push_back(boardLine);																									//On ajoute la ligne au plateau
	}
}
//Destructeur
Game::~Game(){
	//Variables
	//Programme
	threadRunning = false;																										//On arrete le thread
	(*ptrThreadPathFinding).wait();																								//On attend que le thread s'arrete
	dataList.clear();																											//On suppirme les donneés
}
/*####*/

/*UPDATE*/
//Mise a jour du menu principale
int Game::Update(uint prmDifficulty,sf::Vector2f prmMousePos,bool prmMouseLeftClick,bool prmMouseRightClick,bool prmKeyR){
	//Variables
	uint i,x,y;																													//Compteur de boucle
	int nextScene;																												//Prochaine scene (-1 = ça ne bouge pas)
	uint gameWindowSizeW;																										//Largeur de la fenetre 
	uint gameWindowSizeH;																										//Hauteur de la fenetre
	bool gameover;																												//Si gameover
	uint newDevice;																												//Choix aleatoire du nouvel appareil (1=Switch, 2=Router, 3=rien(en master)
	
	//Programme
	gameover = false;																											//Pas en gameover
	nextScene = prmDifficulty;																									//On reste sur la même scene
	gameWindowSizeW = ptrGameWindow->getSize().x;																				//Taille X de la fenetre
	gameWindowSizeH = ptrGameWindow->getSize().y;																				//Taille Y de la fenetre

	//Clock
	if(gameClock.getElapsedTime().asSeconds()>=1){																				//Si une seconde c'est ecoulé
		gameMinutes++;																												//Plus une minute
		gameClock.restart();																										//Remise a 0 de l'horloge
	}

	/*AutoDifficulte*/
	if(gameMinutes/60 == currentDifficulty){																					//Si on est a la même heure que la difficulté
		if(prmDifficulty == SCENE_GAME_MASTER){																						//Si on est en mode MASTER
			newDevice = RandomUint(1,3);																								//On choisie un appareil aleatoire entre Switch Router et Rien
		}else{																														//Sinon
			newDevice = RandomUint(1,2);																								//On choisie un appareil aleatoire entre Switch Router
		}
		switch(newDevice){																											//Pour chaque type de nouvel appareil
			case 1:																														//Si switch
				AddAvailableDevice(1,0);
				break;
			case 2:																														//Si router
				AddAvailableDevice(0,1);
				break;
		}
		AddRandomNewDevice(1,1);																									//On ajoute un serveur et un ordinateur
		currentDifficulty++;																										//Difficulté suivante
	}

	/*ToolMenu Selection*/
	if(prmMouseRightClick && !mouseRightClickHold){																				//Si on faut un clique droit
		if(destructionMode == DESTRUCTION_DEVICE || selectedBuildDevice != DEVICE_NONE){											//Si on est en destruction d'appareil OU en construction d'appareil
			destructionMode = DESTRUCTION_NONE;																							//Mode destruction désactivé
			selectedBuildWire = WIRE_NONE;																								//Aucun cable séléctionné
			selectedBuildDevice = DEVICE_NONE;																							//Aucun appareil selectionné			
		}else{																														//Sinon (c'est les cables)
			if(destructionMode == DESTRUCTION_NONE && selectedBuildWire == WIRE_NONE){													//Si on n'est pas en destruction de cable ET aucun cable
				destructionMode = DESTRUCTION_WIRE;																							//On se met en destruction de cables
			}else if(destructionMode == DESTRUCTION_WIRE){																				//Sinon Si on est en destruction de cable
				destructionMode = DESTRUCTION_NONE;																							//On desactive la destruction
				selectedBuildWire = WIRE_ADSL;																								//On selectionne le cable ADSL
			}else{																														//Sinon (le cable est selectionné)
				selectedBuildWire = WIRE_NONE;																								//Aucun cable selectionné
			}
		}

	}
	if(spriteWireEraserTool.getGlobalBounds().contains(prmMousePos) && prmMouseLeftClick){										//Si on clique sur l'effaceur de cable
		mouseLeftClickUsedByOverMenu = true;																						//Le clique a été utilisé par le menu
		if(!mouseLeftClickHold){																									//Si le clique n'est pas maintenue
			if(destructionMode == DESTRUCTION_WIRE){																					//Si on est en mode destruction de cable
				destructionMode = DESTRUCTION_NONE;																							//Mode destruction désactivé
				selectedBuildWire = WIRE_NONE;																								//Aucun cable séléctionné
				selectedBuildDevice = DEVICE_NONE;																							//Aucun appareil selectionné
			}else{																														//Sinon
				destructionMode = DESTRUCTION_WIRE;																							//Mode destruction des cables activé
				selectedBuildWire = WIRE_NONE;																								//Aucun cable selectionné
				selectedBuildDevice = DEVICE_NONE;																							//Aucun appareil selectionné
			}
		}
	}
	if(spriteADSLWireTool.getGlobalBounds().contains(prmMousePos) && prmMouseLeftClick){										//Si on clique sur l'outil Cable ADSL
		mouseLeftClickUsedByOverMenu = true;																						//Le clique a été utilisé par le menu
		if(!mouseLeftClickHold){																									//Si le clique n'est pas maintenue
			if(selectedBuildWire){																										//Si on est en mode destruction
				destructionMode = DESTRUCTION_NONE;																							//Mode destruction désactivé
				selectedBuildWire = WIRE_NONE;																								//Aucun cable séléctionné
				selectedBuildDevice = DEVICE_NONE;																							//Aucun appareil selectionné
			}else{																														//Sinon
				destructionMode = DESTRUCTION_NONE;																							//Mode destruction désactivé
				selectedBuildWire = WIRE_ADSL;																								//ADSL cable selectionné
				selectedBuildDevice = DEVICE_NONE;																							//Aucun appareil selectionné
			}
		}
	}
	
	if(spriteDeviceEraserTool.getGlobalBounds().contains(prmMousePos) && prmMouseLeftClick){									//Si on clique sur l'effaceur d'appareil
		mouseLeftClickUsedByOverMenu = true;																						//Le clique a été utilisé par le menu
		if(!mouseLeftClickHold){																									//Si le clique n'est pas maintenue
			if(destructionMode == DESTRUCTION_DEVICE){																					//Si on est en mode destruction d'appareil
				destructionMode = DESTRUCTION_NONE;																							//Mode destruction désactivé
				selectedBuildWire = WIRE_NONE;																								//Aucun cable séléctionné
				selectedBuildDevice = DEVICE_NONE;																							//Aucun appareil selectionné
			} else{																														//Sinon
				destructionMode = DESTRUCTION_DEVICE;																						//Mode destruction des appareil activé
				selectedBuildWire = WIRE_NONE;																								//Aucun cable selectionné
				selectedBuildDevice = DEVICE_NONE;																							//Aucun appareil selectionné
			}
		}
	}
	if(spriteRouterTool.getGlobalBounds().contains(prmMousePos) && prmMouseLeftClick){											//Si on clique sur l'outil Router
		mouseLeftClickUsedByOverMenu = true;																						//Le clique a été utilisé par le menu
		if(!mouseLeftClickHold){																									//Si le clique n'est pas maintenue
			if(selectedBuildDevice == DEVICE_ROUTER){																					//Si on est en mode destruction
				destructionMode = DESTRUCTION_NONE;																							//Mode destruction désactivé
				selectedBuildWire = WIRE_NONE;																								//Aucun cable séléctionné
				selectedBuildDevice = DEVICE_NONE;																							//Aucun appareil selectionné
			}else{																														//Sinon
				destructionMode = DESTRUCTION_NONE;																							//Mode destruction désactivé
				selectedBuildWire = WIRE_NONE;																								//Aucun cable selectionné
				selectedBuildDevice = DEVICE_ROUTER;																						//Routeur selectionné
			}
		}
	}
	if(spriteSwitchTool.getGlobalBounds().contains(prmMousePos) && prmMouseLeftClick){											//Si on clique sur l'outil SWITCH
		mouseLeftClickUsedByOverMenu = true;																						//Le clique a été utilisé par le menu
		if(!mouseLeftClickHold){																									//Si le clique n'est pas maintenue
			if(selectedBuildDevice == DEVICE_SWITCH){																					//Si on est en mode destruction
				destructionMode = DESTRUCTION_NONE;																							//Mode destruction désactivé
				selectedBuildWire = WIRE_NONE;																								//Aucun cable séléctionné
				selectedBuildDevice = DEVICE_NONE;																							//Aucun appareil selectionné
			}else{																														//Sinon
				destructionMode = DESTRUCTION_NONE;																							//Mode destruction désactivé
				selectedBuildWire = WIRE_NONE;																								//Aucun cable selectionné
				selectedBuildDevice = DEVICE_SWITCH;																						//Switch selectionné
			}
		}
	}

	/*Block Update*/
	for(y=0;y<BOARD_SIZE_Y;y++){																								//Pour chaque ligne du plateau
		for(x=0;x<BOARD_SIZE_X;x++){																								//Pour chaque case de chaque ligne
			if(!mouseLeftClickUsedByOverMenu){																							//Si le clique n'est pas encore utilisé
				gameover = board[y][x].Update(prmDifficulty,prmMousePos,prmMouseLeftClick,destructionMode,selectedBuildWire,selectedBuildDevice,prmKeyR);//Mise a jour de la case
			}else{																														//Sinon clique pas pris en compte
				gameover = board[y][x].Update(prmDifficulty,prmMousePos,false,destructionMode,selectedBuildWire,selectedBuildDevice,prmKeyR);//Mise a jour de la case
			}
			if(gameover)return SCENE_GAMEOVER;																						//Si on a perdu => On change de scene
		}
	}

	/*ToolMenu/Text Render*/
	//Destructeur de cable
	spriteWireEraserTool.setScale(((ptrGameView->getSize().x*TOOLS_PICTURE_SIZE_X)/gameWindowSizeW)*0.1,((ptrGameView->getSize().y*TOOLS_PICTURE_SIZE_Y)/gameWindowSizeH)*0.1);//Definition de la taille des icones des outils
	if(destructionMode == DESTRUCTION_WIRE){																					//Si on est en mode destruction
		spriteWireEraserTool.setPosition(ptrGameView->getCenter().x-(((ptrGameView->getSize().x*32)/gameWindowSizeW)*16),ptrGameView->getCenter().y+(((ptrGameView->getSize().y*32)/gameWindowSizeH))*9);//Position de l'outil destruction (centreX - (((tailleX * TailleIMG) / tailleScreen) * OFFSET))
		spriteWireEraserTool.setTexture(ptrResoucesManager->gameTools.textureSelectedWireEraser);									//On met la texture comme selectionné
	}else{																														//Sinon
		spriteWireEraserTool.setPosition(ptrGameView->getCenter().x-(((ptrGameView->getSize().x*32)/gameWindowSizeW)*16),ptrGameView->getCenter().y+(((ptrGameView->getSize().y*32)/gameWindowSizeH))*10);//Position de l'outil destruction (centreX - (((tailleX * TailleIMG) / tailleScreen) * OFFSET))
		spriteWireEraserTool.setTexture(ptrResoucesManager->gameTools.textureNotSelectedWireEraser);								//On met la texture comme non selectionné
	}
	//Cable ADSL
	spriteADSLWireTool.setScale(((ptrGameView->getSize().x*TOOLS_PICTURE_SIZE_X)/gameWindowSizeW)*0.1,((ptrGameView->getSize().y*TOOLS_PICTURE_SIZE_Y)/gameWindowSizeH)*0.1);//Definition de la taille des icones des outils
	if(selectedBuildWire){																										//Si on est en mode destruction
		spriteADSLWireTool.setPosition(ptrGameView->getCenter().x-(((ptrGameView->getSize().x*32)/gameWindowSizeW)*12),ptrGameView->getCenter().y+(((ptrGameView->getSize().y*32)/gameWindowSizeH))*9);//Position de l'outil destruction (centreX - (((tailleX * TailleIMG) / tailleScreen) * OFFSET))
		spriteADSLWireTool.setTexture(ptrResoucesManager->gameTools.textureSelectedWireADSL);										//On met la texture comme selectionné
	}else{																														//Sinon
		spriteADSLWireTool.setPosition(ptrGameView->getCenter().x-(((ptrGameView->getSize().x*32)/gameWindowSizeW)*12),ptrGameView->getCenter().y+(((ptrGameView->getSize().y*32)/gameWindowSizeH))*10);//Position de l'outil destruction (centreX - (((tailleX * TailleIMG) / tailleScreen) * OFFSET))
		spriteADSLWireTool.setTexture(ptrResoucesManager->gameTools.textureNotSelectedWireADSL);									//On met la texture comme non selectionné
	}
	
	//Destructeur d'appareil
	spriteDeviceEraserTool.setScale(((ptrGameView->getSize().x*TOOLS_PICTURE_SIZE_X)/gameWindowSizeW)*0.1,((ptrGameView->getSize().y*TOOLS_PICTURE_SIZE_Y)/gameWindowSizeH)*0.1);//Definition de la taille des icones des outils
	if(destructionMode == DESTRUCTION_DEVICE){																					//Si on est en mode destruction
		spriteDeviceEraserTool.setPosition(ptrGameView->getCenter().x-(((ptrGameView->getSize().x*32)/gameWindowSizeW)*6),ptrGameView->getCenter().y+(((ptrGameView->getSize().y*32)/gameWindowSizeH))*9);//Position de l'outil destruction (centreX - (((tailleX * TailleIMG) / tailleScreen) * OFFSET))
		spriteDeviceEraserTool.setTexture(ptrResoucesManager->gameTools.textureSelectedDeviceEraser);									//On met la texture comme selectionné
	} else{																														//Sinon
		spriteDeviceEraserTool.setPosition(ptrGameView->getCenter().x-(((ptrGameView->getSize().x*32)/gameWindowSizeW)*6),ptrGameView->getCenter().y+(((ptrGameView->getSize().y*32)/gameWindowSizeH))*10);//Position de l'outil destruction (centreX - (((tailleX * TailleIMG) / tailleScreen) * OFFSET))
		spriteDeviceEraserTool.setTexture(ptrResoucesManager->gameTools.textureNotSelectedDeviceEraser);								//On met la texture comme non selectionné
	}

	//Routeur
	spriteRouterTool.setScale(((ptrGameView->getSize().x*TOOLS_PICTURE_SIZE_X)/gameWindowSizeW)*0.1,((ptrGameView->getSize().y*TOOLS_PICTURE_SIZE_Y)/gameWindowSizeH)*0.1);//Definition de la taille des icones des outils
	textNbAvailableRouter.setScale(((ptrGameView->getSize().x)/gameWindowSizeW)*0.1,((ptrGameView->getSize().y)/gameWindowSizeH)*0.1);//Definition de la taille des icones des outils
	textNbAvailableRouter.setString(std::to_string(nbAvailableRouter));															//On definie le texte
	if(selectedBuildDevice == DEVICE_ROUTER){																					//Si on est en mode destruction
		spriteRouterTool.setPosition(ptrGameView->getCenter().x-(((ptrGameView->getSize().x*32)/gameWindowSizeW)*2),ptrGameView->getCenter().y+(((ptrGameView->getSize().y*32)/gameWindowSizeH))*9);//Position de l'outil destruction (centreX - (((tailleX * TailleIMG) / tailleScreen) * OFFSET))
		spriteRouterTool.setTexture(ptrResoucesManager->gameTools.textureSelectedRouter);											//On met la texture comme selectionné
		textNbAvailableRouter.setPosition(ptrGameView->getCenter().x-(((ptrGameView->getSize().x*32)/gameWindowSizeW)*-1),ptrGameView->getCenter().y+(((ptrGameView->getSize().y*32)/gameWindowSizeH))*8);//Position de l'outil destruction (centreX - (((tailleX * TailleIMG) / tailleScreen) * OFFSET))
	}else{																														//Sinon
		spriteRouterTool.setPosition(ptrGameView->getCenter().x-(((ptrGameView->getSize().x*32)/gameWindowSizeW)*2),ptrGameView->getCenter().y+(((ptrGameView->getSize().y*32)/gameWindowSizeH))*10);//Position de l'outil destruction (centreX - (((tailleX * TailleIMG) / tailleScreen) * OFFSET))
		spriteRouterTool.setTexture(ptrResoucesManager->gameTools.textureNotSelectedRouter);										//On met la texture comme non selectionné
		textNbAvailableRouter.setPosition(ptrGameView->getCenter().x-(((ptrGameView->getSize().x*32)/gameWindowSizeW)*-1),ptrGameView->getCenter().y+(((ptrGameView->getSize().y*32)/gameWindowSizeH))*9);//Position de l'outil destruction (centreX - (((tailleX * TailleIMG) / tailleScreen) * OFFSET))
	}

	//Switch
	spriteSwitchTool.setScale(((ptrGameView->getSize().x*TOOLS_PICTURE_SIZE_X)/gameWindowSizeW)*0.1,((ptrGameView->getSize().y *TOOLS_PICTURE_SIZE_Y)/gameWindowSizeH)*0.1);//Definition de la taille des icones des outils
	textNbAvailableSwitch.setScale(((ptrGameView->getSize().x)/gameWindowSizeW)*0.1,((ptrGameView->getSize().y)/gameWindowSizeH)*0.1);//Definition de la taille des icones des outils
	textNbAvailableSwitch.setString(std::to_string(nbAvailableSwitch));															//On definie le texte
	if(selectedBuildDevice == DEVICE_SWITCH){																					//Si on est en mode destruction
		spriteSwitchTool.setPosition(ptrGameView->getCenter().x-(((ptrGameView->getSize().x*32)/gameWindowSizeW)*-2),ptrGameView->getCenter().y+(((ptrGameView->getSize().y*32)/gameWindowSizeH))*9);//Position de l'outil destruction (centreX - (((tailleX * TailleIMG) / tailleScreen) * OFFSET))
		spriteSwitchTool.setTexture(ptrResoucesManager->gameTools.textureSelectedSwitch);											//On met la texture comme selectionné
		textNbAvailableSwitch.setPosition(ptrGameView->getCenter().x-(((ptrGameView->getSize().x*32)/gameWindowSizeW)*-5),ptrGameView->getCenter().y+(((ptrGameView->getSize().y*32)/gameWindowSizeH))*8);//Position de l'outil destruction (centreX - (((tailleX * TailleIMG) / tailleScreen) * OFFSET))
	}else{																														//Sinon
		spriteSwitchTool.setPosition(ptrGameView->getCenter().x-(((ptrGameView->getSize().x*32)/gameWindowSizeW)*-2),ptrGameView->getCenter().y+(((ptrGameView->getSize().y*32)/gameWindowSizeH))*10);//Position de l'outil destruction (centreX - (((tailleX * TailleIMG) / tailleScreen) * OFFSET))
		spriteSwitchTool.setTexture(ptrResoucesManager->gameTools.textureNotSelectedSwitch);										//On met la texture comme non selectionné
		textNbAvailableSwitch.setPosition(ptrGameView->getCenter().x-(((ptrGameView->getSize().x*32)/gameWindowSizeW)*-5),ptrGameView->getCenter().y+(((ptrGameView->getSize().y*32)/gameWindowSizeH))*9);//Position de l'outil destruction (centreX - (((tailleX * TailleIMG) / tailleScreen) * OFFSET))
	}
	/*##############*/

	//Data
	for(i=0;i<dataList.size();i++){																					//Pour chaque données
		if(dataList[i].Update()){																						//Si la données a fini
			dataList.erase(dataList.begin()+i);																				//On supprime la donnée
		}
	}
	
	/*Text*/
	textMinutesInGame.setScale(((ptrGameView->getSize().x)/gameWindowSizeW)*0.1,((ptrGameView->getSize().y)/gameWindowSizeH)*0.1);//Definition de la taille des icones des outils
	textMinutesInGame.setPosition(ptrGameView->getCenter().x-(((ptrGameView->getSize().x*32)/gameWindowSizeW)*-14),ptrGameView->getCenter().y+(((ptrGameView->getSize().y*32)/gameWindowSizeH))*-11);//Position de l'outil destruction (centreX - (((tailleX * TailleIMG) / tailleScreen) * OFFSET))
	textMinutesInGame.setString(std::to_string(gameMinutes/60)+":"+std::to_string(gameMinutes%60));
																																																	 /*####*/

	/*Window Render*/
	ptrGameWindow->draw(spriteWireEraserTool);																					//On affiche l'outil de destruction de cable
	ptrGameWindow->draw(spriteADSLWireTool);																					//On affiche l'outil de ADSL
	ptrGameWindow->draw(spriteDeviceEraserTool);																				//On affiche l'outil de destruction d'appareil
	ptrGameWindow->draw(spriteRouterTool);																						//On affiche l'outil de routeur
	ptrGameWindow->draw(spriteSwitchTool);																						//On affiche l'outil de switch
	ptrGameWindow->draw(textNbAvailableSwitch);																					//On affiche le nombre de switch disponible
	ptrGameWindow->draw(textNbAvailableRouter);																					//On affiche le nombre de router disponible
	ptrGameWindow->draw(textMinutesInGame);																						//On affiche l'heure in game

	/*Return*/
	mouseLeftClickHold = prmMouseLeftClick;																						//Pour savoir si le clique est continue
	mouseLeftClickUsedByOverMenu = false;																						//Le clique n'est plus utilisé
	mouseRightClickHold = prmMouseRightClick;																					//Pour savoir si le clique est continue
	return nextScene;																											//On retourne la prochaine scene
}
/*######*/


/*GETTER*/
uint Game::GetScore(){
	return gameMinutes;
}
/*######*/

/*OTHER*/
//Ajout de nouveau appareils aleatoirement sur la map
bool Game::AddRandomNewDevice(uint prmNbNewServer,uint prmNbNewComputer){
	//Variables
	uint i;																														//Comtpeur de boucle
	uint x,y;																													//Position de la case
	//Programme

	/*SERVER*/
	for(i=0;i<prmNbNewServer;i++){																								//Pour chaque serveur a placer
		x = RandomUint(0,BOARD_SIZE_X-1);																							//Position X aleatoire
		y = RandomUint(0,BOARD_SIZE_Y-1);																							//Position Y aleatoire
		while(!board[y][x].DeviceConnexionAvailable(DEVICE_DB)){																	//Tant que la case n'est pas libre
			x = RandomUint(0,BOARD_SIZE_X-1);																							//Position X aleatoire
			y = RandomUint(0,BOARD_SIZE_Y-1);																							//Position Y aleatoire
		}
		board[y][x].SetDevice(DEVICE_DB);																							//On place le serveur
	}
	/*COMPUTER*/
	for(i=0;i<prmNbNewComputer;i++){																							//Pour chaque ordinateur a placer
		x = RandomUint(0,BOARD_SIZE_X-1);																							//Position X aleatoire
		y = RandomUint(0,BOARD_SIZE_Y-1);																							//Position Y aleatoire
		while(!board[y][x].DeviceConnexionAvailable(DEVICE_COMPUTER)){																//Tant que la case n'est pas libre
			x = RandomUint(0,BOARD_SIZE_X-1);																							//Position X aleatoire
			y = RandomUint(0,BOARD_SIZE_Y-1);																							//Position Y aleatoire
		}
		board[y][x].SetDevice(DEVICE_COMPUTER);																						//On place l'ordinateur
	}

	return true;																												//Generation terminé
}
//Ajout de nouveau appareils dans le stock
void Game::AddAvailableDevice(uint prmNbSwitch,uint prmNbRouter){
	//Variables
	//Programme
	nbAvailableSwitch += prmNbSwitch;																							//Ajout des switch
	nbAvailableRouter += prmNbRouter;																							//Ajout des routeurs
}
/*#####*/