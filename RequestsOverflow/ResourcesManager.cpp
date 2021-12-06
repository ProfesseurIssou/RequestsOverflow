#include "resourcesManager.h"

/*INIT*/
//Constructeur
ResourcesManager::ResourcesManager(){
	//Variables
	//Programme
	gameBlock.textureBlockNotSelected.loadFromFile("IMG/BLOCK/NotSelected.png");												//Texture fond du block non selectionné
	gameBlock.textureBlockSelected.loadFromFile("IMG/BLOCK/Selected.png");														//Texture fond du block selectionné


	gameBlock.textureWayADSL_UP.loadFromFile("IMG/WIRE/ADSL_UP.png");															//Texture ADSL dans un chemin UP
	gameBlock.textureWayADSL_DOWN.loadFromFile("IMG/WIRE/ADSL_DOWN.png");														//Texture ADSL dans un chemin DOWN
	gameBlock.textureWayADSL_LEFT.loadFromFile("IMG/WIRE/ADSL_LEFT.png");														//Texture ADSL dans un chemin LEFT
	gameBlock.textureWayADSL_RIGHT.loadFromFile("IMG/WIRE/ADSL_RIGHT.png");														//Texture ADSL dans un chemin RIGHT
	gameBlock.textureWayADSL_LEFT_UP.loadFromFile("IMG/WIRE/ADSL_UP_LEFT.png");													//Texture ADSL dans un chemin UP LEFT
	gameBlock.textureWayADSL_LEFT_DOWN.loadFromFile("IMG/WIRE/ADSL_DOWN_LEFT.png");												//Texture ADSL dans un chemin DOWN LEFT
	gameBlock.textureWayADSL_RIGHT_UP.loadFromFile("IMG/WIRE/ADSL_UP_RIGHT.png");												//Texture ADSL dans un chemin UP RIGHT
	gameBlock.textureWayADSL_RIGHT_DOWN.loadFromFile("IMG/WIRE/ADSL_DOWN_RIGHT.png");											//Texture ADSL dans un chemin DOWN RIGHT


	gameTools.textureNotSelectedWireEraser.loadFromFile("IMG/TOOLS/notSelectedWireEraser.png");									//Texture Destructeur de cable non selectionné
	gameTools.textureNotSelectedWireADSL.loadFromFile("IMG/TOOLS/notSelectedWireADSL.png");										//Texture ADSL non selectionné
	gameTools.textureNotSelectedDeviceEraser.loadFromFile("IMG/TOOLS/notSelectedDeviceEraser.png");								//Texture Destructeur d'appareil non selectionné
	gameTools.textureNotSelectedRouter.loadFromFile("IMG/TOOLS/notSelectedROUTER.png");											//Texture Routeur non selectionné
	gameTools.textureNotSelectedSwitch.loadFromFile("IMG/TOOLS/notSelectedSWITCH.png");											//Texture Switch non selectionné
	
	gameTools.textureSelectedWireEraser.loadFromFile("IMG/TOOLS/selectedWireEraser.png");										//Texture Destructeur de cable selectionné
	gameTools.textureSelectedWireADSL.loadFromFile("IMG/TOOLS/selectedWireADSL.png");											//Texture ADSL selectionné
	gameTools.textureSelectedDeviceEraser.loadFromFile("IMG/TOOLS/selectedDeviceEraser.png");									//Texture Destructeur d'appareil selectionné
	gameTools.textureSelectedRouter.loadFromFile("IMG/TOOLS/selectedROUTER.png");												//Texture Routeur selectionné
	gameTools.textureSelectedSwitch.loadFromFile("IMG/TOOLS/selectedSWITCH.png");												//Texture Switch selectionné


	gameBlock.textureBlockComputer_1.loadFromFile("IMG/DEVICES/COMPUTER1.png");													//Ordinateur 1
	gameBlock.textureBlockComputer_2.loadFromFile("IMG/DEVICES/COMPUTER2.png");													//Ordinateur 2
	gameBlock.textureBlockComputer_3.loadFromFile("IMG/DEVICES/COMPUTER3.png");													//Ordinateur 3
	gameBlock.textureBlockComputer_4.loadFromFile("IMG/DEVICES/COMPUTER4.png");													//Ordinateur 4
	gameBlock.textureBlockROUTER.loadFromFile("IMG/DEVICES/ROUTER.png");														//Routeur
	gameBlock.textureBlockSWITCH.loadFromFile("IMG/DEVICES/SWITCH.png");														//Switch
	gameBlock.textureBlockDB_LEVEL_1.loadFromFile("IMG/DEVICES/DB_LEVEL1.png");													//Serveur 1
	gameBlock.textureBlockDB_LEVEL_2.loadFromFile("IMG/DEVICES/DB_LEVEL2.png");													//Serveur 2
	gameBlock.textureBlockDB_LEVEL_3.loadFromFile("IMG/DEVICES/DB_LEVEL3.png");													//Serveur 3
	gameBlock.textureBlockDB_LEVEL_4.loadFromFile("IMG/DEVICES/DB_LEVEL4.png");													//Serveur 4


	gameData.textureData.loadFromFile("IMG/DATA/data.png");																		//Données
}
//Destructeur
ResourcesManager::~ResourcesManager(){
	//Variables
	//Programme
}
/*####*/