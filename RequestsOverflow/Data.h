#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

#include "ResourcesManager.h"				                                                                        //Gestion des resources
//#include "GameBlock.h"

typedef unsigned int uint;                                                                                          //Simplification

/*SIGNATURE*/
class GameBlock;
/*#########*/

/*DEFINE*/
#define DATA_TARGET_DB 0																							//La donnée doit aller jusqu'a une DB
#define DATA_TARGET_COMPUTER 1																						//La donnée doit aller jusqu'a un ordinateur
/*######*/

/*CONFIG*/
#define DATA_NB_STEPS 10																							//Nombre de pas entre chaque case de deplacement de la données
#define SCALE_DATA 1
/*######*/



class Data{
	public:
		/*INIT*/
		Data(sf::RenderWindow *prmPtrGameWindow,std::vector<std::vector<GameBlock>> *prmPtrBoard,ResourcesManager *prmPtrResourcesManager,uint prmStartPosX,uint prmStartPosY,uint prmTargetDevice,std::vector<std::vector<int>> prmPath);//Constructeur
		~Data();																										//Destructeur
		/*####*/

		/*UPDATE*/
		bool Update();																									//Mise a jour de la data (retourne 1 si elle a fini (destruction), retourne 0 si elle n'a pas fini)
		/*######*/

	private:
		/*RESOURCES*/
		ResourcesManager *ptrResoucesManager;																			//Gestionnaire des resources
		
		/*SFML*/
		sf::Sprite dataSprite;																							//Sprite de la donnée
		sf::RenderWindow *ptrGameWindow;																				//Pointeur de la fenetre de jeux

		/*GAME*/
		std::vector<std::vector<GameBlock>> *ptrBoard;																	//Pointeur vers le plateau

		/*VARIABLES*/
		std::vector<std::vector<int>> path;																				//Liste des cases (PATH)
		uint currentPos;																								//Etape actuel dans la liste du PATH
		uint currentStep;																								//Etape actuel dans le deplacement entre deux case
};

