#include "PathFinder.h"

#include "GameBlock.h"

/*FIND*/
//Gestion du thread du pathfinding
void ThreadPathFinder(std::vector<std::vector<GameBlock>> *prmPtrBoard,uint prmStartPosX,uint prmStartPosY,uint prmFindMode,std::vector<std::vector<int>> *prmPtrOutputPath,bool *prmThreadRunning,bool *prmPathLock){
	//Variables
	std::vector<std::vector<int>> newPath;																		//Nouveau chemin trouvé
	newPath.clear();																							//On vide la liste
	//Programme
	switch(prmFindMode){																						//Pour chaque mode de recherche
		case FINDER_MODE_TO_SERVER:																					//Si on cherche vers le serveur
			while(*prmThreadRunning){																					//Tant que le scan doit etre fait
				newPath = FindPathToServer(prmPtrBoard,prmStartPosX,prmStartPosY);											//On recherche un nouveau chemin
				(*prmPathLock) = true;																						//On bloque la lecture du path
				(*prmPtrOutputPath) = newPath;																				//On ajoute le nouveau path
				(*prmPathLock) = false;																						//On debloque la lecture du path
			}
			break;
		case FINDER_MODE_TO_COMPUTER:																				//Si on cherche vers l'ordinateur
			while(*prmThreadRunning){																					//Tant que le scan doit etre fait
				newPath = FindPathToComputer(prmPtrBoard,prmStartPosX,prmStartPosY);										//On recherche un nouveau chemin
				(*prmPathLock) = true;																						//On bloque la lecture du path
				(*prmPtrOutputPath) = newPath;																				//On ajoute le nouveau path
				(*prmPathLock) = false;																						//On debloque la lecture du path
			}
			break;
	}
}
//Cherche le chemin vers le serveur ([[]] => Aucun chemin trouvé // [[X,Y],[X2,Y2]] ou [[X,Y]] => Chemin trouvé)
std::vector<std::vector<int>> FindPathToServer(std::vector<std::vector<GameBlock>> *prmPtrBoard,uint prmStartPosX,uint prmStartPosY,std::vector<std::vector<bool>> prmListCaseUsed){
	//Variables
	uint i;																										//Compteur de boucle
	std::vector<std::vector<std::vector<int>>> subPathAvailable;												//Liste de toute les possibilité de PATH
	std::vector<std::vector<int>> newSubPath;																	//Nouveau PATH retourné par une récursion
	uint currentDeviceType;																						//Type d'appareil de la case actuel

	uint bestSubPath;																							//Index du meilleur subPath
	std::vector<int> lastCaseBest;																				//Derniere case du meilleur path (ici le serveur)
	std::vector<int> lastCase;																					//Derniere case du path actuel (ici le serveur)
	//Programme

	/*DB*/
	currentDeviceType = (*prmPtrBoard)[prmStartPosY][prmStartPosX].GetDeviceType();								//Recuperation de l'appareil de la case actuel
	if(currentDeviceType == DEVICE_DB){																			//Si la case actuel est une DB
		return {{(int)prmStartPosX,(int)prmStartPosY}};																//On retourne la position actuel
	}

	/*ptrListCaseUsed*/
	if(prmListCaseUsed.empty()){																				//Si le tableau de bool n'est pas encore créé
		prmListCaseUsed = std::vector<std::vector<bool>>((*prmPtrBoard)[0].size(),std::vector<bool>((*prmPtrBoard).size(),false));//On créé un tableau de la même taille que le plateau avec toute les valeur a False
	}
	prmListCaseUsed[prmStartPosY][prmStartPosX] = true;															//La case actuel est maintenant utilisé
	
	/*Recursion Search*/
	if((*prmPtrBoard)[prmStartPosY][prmStartPosX].GetWayUP()){													//Si la case actuel va vers le haut
		if(!prmListCaseUsed[prmStartPosY-1][prmStartPosX]){															//Si la case du dessus n'est pas encore utilisé
			newSubPath = FindPathToServer(prmPtrBoard,prmStartPosX,prmStartPosY-1,prmListCaseUsed);						//On créé une récurtion
			if(!newSubPath.empty()){																					//Si la recurtion à trouvé un chemin
				newSubPath.insert(newSubPath.begin(),{(int)prmStartPosX,(int)prmStartPosY});								//On ajoute la case actuel au PATH final
				subPathAvailable.push_back(newSubPath);																		//On ajoute le nouveau PATH trouvé
			}
		}
	}
	if((*prmPtrBoard)[prmStartPosY][prmStartPosX].GetWayUPRIGHT()){												//Si la case actuel va vers le haut droite
		if(!prmListCaseUsed[prmStartPosY-1][prmStartPosX+1]){														//Si la case dessus droite n'est pas encore utilisé
			newSubPath = FindPathToServer(prmPtrBoard,prmStartPosX+1,prmStartPosY-1,prmListCaseUsed);					//On créé une récurtion
			if(!newSubPath.empty()){																					//Si la recurtion à trouvé un chemin
				newSubPath.insert(newSubPath.begin(),{(int)prmStartPosX,(int)prmStartPosY});								//On ajoute la case actuel au PATH final
				subPathAvailable.push_back(newSubPath);																		//On ajoute le nouveau PATH trouvé
			}
		}
	}
	if((*prmPtrBoard)[prmStartPosY][prmStartPosX].GetWayRIGHT()){												//Si la case actuel va vers la droite
		if(!prmListCaseUsed[prmStartPosY][prmStartPosX+1]){															//Si la case de droite n'est pas encore utilisé
			newSubPath = FindPathToServer(prmPtrBoard,prmStartPosX+1,prmStartPosY,prmListCaseUsed);						//On créé une récurtion
			if(!newSubPath.empty()){																					//Si la recurtion à trouvé un chemin
				newSubPath.insert(newSubPath.begin(),{(int)prmStartPosX,(int)prmStartPosY});								//On ajoute la case actuel au PATH final
				subPathAvailable.push_back(newSubPath);																		//On ajoute le nouveau PATH trouvé
			}
		}
	}
	if((*prmPtrBoard)[prmStartPosY][prmStartPosX].GetWayRIGHTDOWN()){											//Si la case actuel va vers la droite bas
		if(!prmListCaseUsed[prmStartPosY+1][prmStartPosX+1]){														//Si la case de droite bas n'est pas encore utilisé
			newSubPath = FindPathToServer(prmPtrBoard,prmStartPosX+1,prmStartPosY+1,prmListCaseUsed);					//On créé une récurtion
			if(!newSubPath.empty()){																					//Si la recurtion à trouvé un chemin
				newSubPath.insert(newSubPath.begin(),{(int)prmStartPosX,(int)prmStartPosY});								//On ajoute la case actuel au PATH final
				subPathAvailable.push_back(newSubPath);																		//On ajoute le nouveau PATH trouvé
			}
		}
	}
	if((*prmPtrBoard)[prmStartPosY][prmStartPosX].GetWayDOWN()){												//Si la case actuel va vers le bas
		if(!prmListCaseUsed[prmStartPosY+1][prmStartPosX]){															//Si la case du dessous n'est pas encore utilisé
			newSubPath = FindPathToServer(prmPtrBoard,prmStartPosX,prmStartPosY+1,prmListCaseUsed);						//On créé une récurtion
			if(!newSubPath.empty()){																					//Si la recurtion à trouvé un chemin
				newSubPath.insert(newSubPath.begin(),{(int)prmStartPosX,(int)prmStartPosY});								//On ajoute la case actuel au PATH final
				subPathAvailable.push_back(newSubPath);																		//On ajoute le nouveau PATH trouvé
			}
		}
	}
	if((*prmPtrBoard)[prmStartPosY][prmStartPosX].GetWayDOWNLEFT()){											//Si la case actuel va vers le bas gauche
		if(!prmListCaseUsed[prmStartPosY+1][prmStartPosX-1]){														//Si la case bas gauche n'est pas encore utilisé
			newSubPath = FindPathToServer(prmPtrBoard,prmStartPosX-1,prmStartPosY+1,prmListCaseUsed);					//On créé une récurtion
			if(!newSubPath.empty()){																					//Si la recurtion à trouvé un chemin
				newSubPath.insert(newSubPath.begin(),{(int)prmStartPosX,(int)prmStartPosY});								//On ajoute la case actuel au PATH final
				subPathAvailable.push_back(newSubPath);																		//On ajoute le nouveau PATH trouvé
			}
		}
	}
	if((*prmPtrBoard)[prmStartPosY][prmStartPosX].GetWayLEFT()){												//Si la case actuel va vers la gauche
		if(!prmListCaseUsed[prmStartPosY][prmStartPosX-1]){															//Si la case de gauche n'est pas encore utilisé
			newSubPath = FindPathToServer(prmPtrBoard,prmStartPosX-1,prmStartPosY,prmListCaseUsed);						//On créé une récurtion
			if(!newSubPath.empty()){																					//Si la recurtion à trouvé un chemin
				newSubPath.insert(newSubPath.begin(),{(int)prmStartPosX,(int)prmStartPosY});								//On ajoute la case actuel au PATH final
				subPathAvailable.push_back(newSubPath);																		//On ajoute le nouveau PATH trouvé
			}
		}
	}
	if((*prmPtrBoard)[prmStartPosY][prmStartPosX].GetWayLEFTUP()){												//Si la case actuel va vers la gauche haut
		if(!prmListCaseUsed[prmStartPosY-1][prmStartPosX-1]){														//Si la case gauche haut n'est pas encore utilisé
			newSubPath = FindPathToServer(prmPtrBoard,prmStartPosX-1,prmStartPosY-1,prmListCaseUsed);					//On créé une récurtion
			if(!newSubPath.empty()){																					//Si la recurtion à trouvé un chemin
				newSubPath.insert(newSubPath.begin(),{(int)prmStartPosX,(int)prmStartPosY});								//On ajoute la case actuel au PATH final
				subPathAvailable.push_back(newSubPath);																		//On ajoute le nouveau PATH trouvé
			}
		}
	}

	/*Check*/
	if(subPathAvailable.empty())return {};																	//Si aucun subPATH trouvé => retourne rien

	/*Path Selection*/
	bestSubPath = 0;																							//Par defaut le premier
	switch(currentDeviceType){																					//Pour chaque type d'appareil e la case actuel
		case DEVICE_SWITCH:																							//Si c'est un switch
			return subPathAvailable[RandomUint(0,subPathAvailable.size()-1)];											//On retourne un PATH aleatoire
			break;
		case DEVICE_ROUTER:																							//Si c'est un routeur
			lastCaseBest = subPathAvailable[bestSubPath].back();														//On prend la position de la DB du meilleur PATH actuel
			for(i=0;i<subPathAvailable.size();i++){																		//Pour chaque subPATH
				lastCase = subPathAvailable[i].back();																		//On prend la position de la DB du PATH actuel
				if((*prmPtrBoard)[lastCaseBest[1]][lastCaseBest[0]].GetNbStoredData() > (*prmPtrBoard)[lastCase[1]][lastCase[0]].GetNbStoredData()){//Si le meilleur subPATH mène sur une DB plus remplis que le subPATH actuel
					bestSubPath = i;																							//On defini l'index du meilleur subPATH
					lastCaseBest = subPathAvailable[bestSubPath].back();														//On prend la position de la DB du meilleur PATH actuel
				}
			}
			return subPathAvailable[bestSubPath];																		//On retourne le meilleur subPATH trouvé
			break;
		default:																									//Par defaut
			return subPathAvailable[0];																					//On retourne le premier PATH 
			break;
	}

}
//Cherche le chemin vers l'ordinateur
std::vector<std::vector<int>> FindPathToComputer(std::vector<std::vector<GameBlock>> *prmPtrBoard,uint prmStartPosX,uint prmStartPosY,std::vector<std::vector<bool>> prmListCaseUsed){
	//Variables
	std::vector<std::vector<std::vector<uint>>> subPathAvailable;												//Liste de toute les possibilité de PATH
	//Programme
	return {{}};
}
/*####*/