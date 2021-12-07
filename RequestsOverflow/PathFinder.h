#pragma once
#include <vector>

#include <SFML/Graphics.hpp>

//#include "GameBlock.h"
#include "Others.h"																									//RandUint

typedef unsigned int uint;                                                                                          //Simplification

/*CONTANTE*/
#define FINDER_MODE_TO_SERVER 0
#define FINDER_MODE_TO_COMPUTER 1
/*########*/

/*SIGNATURE*/
class GameBlock;
/*#########*/

//Pour chaque case suivante, on cree une recusion, puis quand on arrive au bout, on retourne si il y a un points recherché au bout, (liste vide [] = rien, position x et y [[x9,y9],[x10,y10],[.....]] = trouvé)

struct StructPathFinding{
	uint posX;
	uint posY;
	uint findMode;
	bool pathLock;
	std::vector<std::vector<int>> outputPath;
};


/*FIND*/
void ThreadPathFinder(std::vector<std::vector<GameBlock>> *prmPtrBoard,std::vector<StructPathFinding>* prmPtrOutput,bool *prmThreadRunning);//Gestion du thread du pathfinding
std::vector<std::vector<int>> FindPathToServer(std::vector<std::vector<GameBlock>> *prmPtrBoard,uint prmStartPosX,uint prmStartPosY,std::vector<std::vector<bool>> prmListCaseUsed={});//Cherche le chemin vers le serveur
std::vector<std::vector<int>> FindPathToComputer(std::vector<std::vector<GameBlock>> *prmPtrBoard,uint prmStartPosX,uint prmStartPosY,std::vector<std::vector<bool>> prmListCaseUsed={});//Cherche le chemin vers l'ordinateur