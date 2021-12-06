#pragma once

#include <SFML/Graphics.hpp>

typedef unsigned int uint;                                                                                          //Simplification

/*STRUCT*/
struct StructGameBlockResources{																					//Resources des block
	sf::Texture textureBlockNotSelected;																				//Texture fond du block non selectionné
	sf::Texture textureBlockSelected;																					//Texture fond du block selectionné
	
	sf::Texture textureWayADSL_UP;																						//Texture ADSL dans un chemin UP
	sf::Texture textureWayADSL_DOWN;																					//Texture ADSL dans un chemin DOWN
	sf::Texture textureWayADSL_LEFT;																					//Texture ADSL dans un chemin LEFT
	sf::Texture textureWayADSL_RIGHT;																					//Texture ADSL dans un chemin RIGHT
	sf::Texture textureWayADSL_LEFT_UP;																					//Texture ADSL dans un chemin UP LEFT
	sf::Texture textureWayADSL_LEFT_DOWN;																				//Texture ADSL dans un chemin DOWN LEFT
	sf::Texture textureWayADSL_RIGHT_UP;																				//Texture ADSL dans un chemin UP RIGHT
	sf::Texture textureWayADSL_RIGHT_DOWN;																				//Texture ADSL dans un chemin DOWN RIGHT

	sf::Texture textureBlockComputer_1;																					//Ordinateur 1
	sf::Texture textureBlockComputer_2;																					//Ordinateur 2
	sf::Texture textureBlockComputer_3;																					//Ordinateur 3
	sf::Texture textureBlockComputer_4;																					//Ordinateur 4
	sf::Texture textureBlockROUTER;																						//Routeur
	sf::Texture textureBlockSWITCH;																						//Switch
	sf::Texture textureBlockDB_LEVEL_1;																					//Serveur 1
	sf::Texture textureBlockDB_LEVEL_2;																					//Serveur 2
	sf::Texture textureBlockDB_LEVEL_3;																					//Serveur 3
	sf::Texture textureBlockDB_LEVEL_4;																					//Serveur 4
};
struct StructGameToolsResources{																					//Resources des outils
	sf::Texture textureNotSelectedWireEraser;																			//Texture Wire Eraser non selectionné
	sf::Texture textureNotSelectedWireADSL;																				//Texture Wire adsl non selectionné
	sf::Texture textureNotSelectedDeviceEraser;																			//Texture Device Eraser non selectionné
	sf::Texture textureNotSelectedRouter;																				//Texture Router non selectionné
	sf::Texture textureNotSelectedSwitch;																				//Texture Switch non selectionné
	
	sf::Texture textureSelectedWireEraser;																				//Texture Wire Eraser selectionné
	sf::Texture textureSelectedWireADSL;																				//Texture Wire adsl selectionné
	sf::Texture textureSelectedDeviceEraser;																			//Texture Device Eraser selectionné
	sf::Texture textureSelectedRouter;																					//Texture Router selectionné
	sf::Texture textureSelectedSwitch;																					//Texture Switch selectionné
};
struct StructGameDataResources{																						//Resources des données
	sf::Texture textureData;																							//Texture d'une donnée
};
/*######*/

class ResourcesManager{
	public:
		/*INIT*/
		ResourcesManager();																								//Constructeur
		~ResourcesManager();																							//Destructeur
		/*####*/

		/*STRUCT*/
		StructGameBlockResources gameBlock;																				//Resources des block
		StructGameToolsResources gameTools;																				//Resources des outils
		StructGameDataResources gameData;																				//Resources des données
		/*######*/

	private:
};

