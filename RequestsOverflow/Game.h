#pragma once
#include <SFML/System.hpp>																							//Thread
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

#include "config.h"
#include "PathFinder.h"
#include "ResourcesManager.h"				                                                                        //Gestion des resources
#include "GameBlock.h"
#include "Others.h"																									//RandUint
#include "Data.h"																									//Data


/*CONFIG*/
#define BOARD_SIZE_X 20
#define BOARD_SIZE_Y 20
#define TOOLS_PICTURE_SIZE_X 32
#define TOOLS_PICTURE_SIZE_Y 32
/*######*/

/*WIRE TYPE*/
#define WIRE_NONE 0
#define WIRE_ADSL 1
#define WIRE_FIBRE 2
/*#########*/

/*DESTRUCTION MODE*/
#define DESTRUCTION_NONE 0
#define DESTRUCTION_WIRE 1
#define DESTRUCTION_DEVICE 2
/*################*/

typedef unsigned int uint;                                                                                          //Simplification

class Game{
	public:
		/*INIT*/
		Game(sf::RenderWindow *prmPtrGameWindow,ResourcesManager *prmPtrResoucesManager,sf::View *prmPtrGameView);			//Constructeur
		~Game();																											//Destructeur
		/*####*/

		/*UPDATE*/
		int Update(uint prmDifficulty,sf::Vector2f prmMousePos,bool prmMouseLeftClick,bool prmMouseRightClick,bool prmKeyR);//Mise a jour du plateau de jeux
		/*######*/

		/*GETTER*/
		uint GetScore();
		/*######*/
	private:
		/*RESOURCES*/
		ResourcesManager *ptrResoucesManager;																				//Gestionnaire des resources
		/*#########*/
	
		/*SFML*/
		sf::RenderWindow *ptrGameWindow;																					//Pointeur de la fenetre de jeux
		sf::View *ptrGameView;																								//Pointeur de la vue du joueur
		sf::Clock gameClock;																								//Horloge du jeux

		//Text
		sf::Text textNbAvailableSwitch;																						//Nombre de switch disponible
		sf::Text textNbAvailableRouter;																						//Nombre de routeur disponible
		sf::Text textMinutesInGame;																							//Minutes in game

		//Font
		sf::Font fontTetris;																								//Police de caractere Tetris

		//Sprite Tools
		sf::Sprite spriteWireEraserTool;																					//Sprite du destructeur de cable
		sf::Sprite spriteADSLWireTool;																						//Sprite du cable ADSL
		sf::Sprite spriteDeviceEraserTool;																					//Sprite du destructeur d'appareil
		sf::Sprite spriteRouterTool;																						//Sprite du routeur
		sf::Sprite spriteSwitchTool;																						//Sprite du switch
		/*####*/

		/*GAME*/
		std::vector<std::vector<GameBlock>> board;																			//Plateau de jeux
		uint destructionMode;																								//Si on est en mode destruction (1 = cable, 2 = appareil)
		uint selectedBuildWire;																								//Cable selectionné pour la construction
		uint selectedBuildDevice;																							//Appareil selectionné pour la construction
		std::vector<Data> dataList;																							//Liste des données actuel sur l'ecran
		/*####*/

		/*STATES*/
		bool mouseLeftClickHold;																							//Si le clique de sourie est maintenue
		bool mouseLeftClickUsedByOverMenu;																					//Si le clique a été utilisé par un menu superposé
		bool mouseRightClickHold;																							//Si le clique de sourie est maintenue
		/*######*/

		/*VARIABLES*/
		uint gameMinutes;																									//Score actuel (pour le niveau de difficulté), 1 second = 1 minute in game
		uint currentDifficulty;																								//Difficulté actuel
		uint nbAvailableRouter;																								//Nombre de routeur disponible
		uint nbAvailableSwitch;																								//Nombre de switch disponible
		/*#########*/

		/*OTHER*/
		bool AddRandomNewDevice(uint prmNbNewServer,uint prmNbNewComputer);													//Ajout de nouveau appareils aleatoirement sur la map
		void AddAvailableDevice(uint prmNbSwitch,uint prmNbRouter);															//Ajout de nouveau appareils dans le stock
		/*#####*/

		/*THREAD*/
		sf::Thread *ptrThreadPathFinding;																					//Thread de la recherche du chemin
		bool threadRunning;																									//Si le thread doit tourner
		std::vector<StructPathFinding> threadOutputPath;																	//Liste des path de sortie
		/*######*/
};

