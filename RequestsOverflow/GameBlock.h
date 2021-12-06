#pragma once

#include <SFML/System.hpp>																							//Thread
#include <SFML/Graphics.hpp>

#include <functional>																								//Bind

#include "config.h"
#include "ResourcesManager.h"				                                                                        //Gestion des resources
#include "PathFinder.h"																								//FindPathToServer FindPathToComputer
#include "Data.h"																									//Data

/*CONFIG*/
#define BLOCK_SIZE_X 100						//En pixel
#define BLOCK_SIZE_Y 100						//En pixel
/*######*/

/*SPRITE SCALE*/
#define SCALE_BACKGROUND 1
#define SCALE_DEVICE_DB 2
#define SCALE_DEVICE_COMPUTER 2
#define SCALE_DEVICE_ROUTER 1
#define SCALE_DEVICE_SWITCH 1
#define SCALE_WIRE 10
/*###########*/

/*WIRE TYPE*/
#define WIRE_NONE 0
#define WIRE_ADSL 1
/*#########*/

/*LIMIT*/
#define DATA_LIMIT_1 10																								//Avant de passer au 2
#define DATA_LIMIT_2 100																							//Avant de passer au 3
#define DATA_LIMIT_3 200																							//Avant de passer au 4
#define DATA_LIMIT_4 400																							//Avant Gameover
/*#####*/


/*COMPUTER CONFIG*/
#define BLOCK_SPAWN_COEF 0.01																						//Coef d'apparition des donn�es
/*###############*/

/*DATABASE CONFIG*/
#define DB_PROCESS_DATA_MS 1000																						//Temp en milliseconde entre chaque donn�es supprim� de la base
/*###############*/

/*DESTRUCTION MODE*/
#define DESTRUCTION_NONE 0
#define DESTRUCTION_WIRE 1
#define DESTRUCTION_DEVICE 2
/*################*/

typedef unsigned int uint;                                                                                          //Simplification

class GameBlock{
	public:
		/*INIT*/
		GameBlock(sf::RenderWindow *prmPtrGameWindow,uint prmBoardPosX,uint prmBoardPosY,std::vector<std::vector<GameBlock>> *prmPtrBoard,ResourcesManager *prmPtrResourcesManager,std::vector<Data> *prmPtrDataList,uint *prmPtrNbAvailableSwitch,uint *prmPtrNbAvailableRouter);//Constructeur
		~GameBlock();																									//Destructeur
		/*####*/

		/*UPDATE*/
		bool Update(uint prmDifficulty,sf::Vector2f prmMousePos,bool prmMouseLeftClick,uint prmDestructionMode,uint prmSelectedBuildWire,uint prmSelectedBuildDevice,bool prmKeyR);//Mise a jour du block
		/*######*/

		/*CHECK*/
		bool CursorHover(sf::Vector2f prmMousePos);																		//Si le curseur est dessus
		bool CursorSelected(bool prmMouseLeftClick);																	//Si le bloc est cliqu�
		bool CableConnexionAvailable();																					//Si on peut connecter encore un cable
		bool DeviceConnexionAvailable(uint prmDeviceType);																//Si on peut connecter un appareil
		/*#####*/

		/*SETTER*/
		void SetWayUP(bool prmCableType);																				//Definir le cablage vers le haut
		void SetWayUPRIGHT(bool prmCableType);																			//Definir le cablage vers le haut-droite
		void SetWayRIGHT(bool prmCableType);																			//Definir le cablage vers le droite
		void SetWayRIGHTDOWN(bool prmCableType);																		//Definir le cablage vers le droite-bas
		void SetWayDOWN(bool prmCableType);																				//Definir le cablage vers le bas
		void SetWayDOWNLEFT(bool prmCableType);																			//Definir le cablage vers le bas-gauche
		void SetWayLEFT(bool prmCableType);																				//Definir le cablage vers le gauche
		void SetWayLEFTUP(bool prmCableType);																			//Definir le cablage vers le gauche-haut
		void SetSelected(bool prmSelected);																				//Definir la selection du block
		void SetDevice(uint prmDeviceType);																				//Definit le type d'appareil
		void AddOneData();																								//Ajouter une donn�es dans le bloc
		void GetOneData();																								//Recup�rer une donn�es dans le bloc
		void StopThread();																								//On arrete le thread du pathfinding
		/*######*/

		/*GETTER*/
		bool GetWayUP();																								//Recuperer le cablage vers le haut
		bool GetWayUPRIGHT();																							//Recuperer le cablage vers le haut-droite
		bool GetWayRIGHT();																								//Recuperer le cablage vers le droite
		bool GetWayRIGHTDOWN();																							//Recuperer le cablage vers le droite-bas
		bool GetWayDOWN();																								//Recuperer le cablage vers le bas
		bool GetWayDOWNLEFT();																							//Recuperer le cablage vers le bas-gauche
		bool GetWayLEFT();																								//Recuperer le cablage vers le gauche
		bool GetWayLEFTUP();																							//Recuperer le cablage vers le gauche-haut
		uint GetDeviceType();																							//Recuperer le type d'appareil
		uint GetNbStoredData();																							//Recuperer le nombre de donn�es actuellement stock�
		/*######*/

	private:
		/*RESOURCES*/
		ResourcesManager *ptrResoucesManager;																			//Gestionnaire des resources
		
		/*SFML*/
		sf::RenderWindow *ptrGameWindow;																				//Pointeur de la fenetre de jeux

		/*CLOCK*/
		sf::Clock clockDBProcess;																						//Horloge de la DB pour le traitement des donn�es
		/*#####*/

		//Sprite
		sf::Sprite spriteBlock;																							//Sprite du block
		sf::Sprite spriteDevice;																						//Sprite de l'appareil
		sf::Sprite spriteWire;																							//Sprite du cable
		/*####*/

		/*STATES*/
		uint boardPosX;																									//Position X de la case en fonction du plateau
		uint boardPosY;																									//Position Y de la case en fonction du plateau
		
		bool blockHover;																								//Si le block est survoll�
		bool blockSelected;																								//Si le block est selectionn�
		bool blockSelectedCache;																						//Si le block est selectionn� (ancienne valeur)

		bool keyRHold;																									//Touche R maintenue
		bool mouseLeftHold;																								//Clique gauche maintenue
		
		uint deviceType;																								//Type d'appareil
		bool wayUP;																										//Liaison vers haut
		bool wayUPRIGHT;																								//Liaison vers haut droite
		bool wayRIGHT;																									//Liaison vers droite
		bool wayRIGHTDOWN;																								//Liaison vers droite bas
		bool wayDOWN;																									//Liaison vers bas
		bool wayDOWNLEFT;																								//Liaison vers bas gauche
		bool wayLEFT;																									//Liaison vers gauche
		bool wayLEFTUP;																									//Liaison vers gauche haut
		uint nbConnectedCable;																							//Nombre de cable connect�

		bool ThreadRunning;																								//Si le thread doit tourner
		bool pathLock;																									//Si le thread (pathfinding) ecrit sur le path
		/*######*/

		/*VARIABLES*/
		uint nbStoredData;																								//Nombre de donn�es stoquer dans la case
		uint *ptrNbAvailableSwitch;																						//Pointeur vers le nombre de switch disponible
		uint *ptrNbAvailableRouter;																						//Pointeur vers le nombre de routeur disponible
		/*#########*/

		/*GAME*/
		std::vector<std::vector<GameBlock>> *ptrBoard;																	//Pointeur vers le plateau
		std::vector<Data> *ptrDataList;																					//Liste des donn�es actuel sur l'ecran
		/*####*/

		/*THREAD*/
		std::vector<std::vector<int>> currentDataPath;																	//Chemin de donn�es actuellement calcul� par le thread
		sf::Thread *ptrThreadPathFinding;																				//Thread de la recherche du chemin vers le serveurs
		/*######*/

		/*OTHER*/
		void Rotate();																									//Rotation des fils
		/*#####*/
};

