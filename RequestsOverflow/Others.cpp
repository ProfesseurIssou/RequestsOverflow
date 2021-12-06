#include "Others.h"


/*CALCUL*/
//Generation d'un uint aleatoire entre deux limites
uint RandomUint(uint prmMin,uint prmMax){
	//Variables
	//Programme
	if(prmMin==prmMax)return prmMin;										//Pour eviter la division par 0
	return prmMin+(rand())/((RAND_MAX/((prmMax+1)-prmMin)));
}
//Generation d'un float aleatoire entre deux limites
float RandomFloat(float prmMin,float prmMax){
	//Variables
	//Programme
	if(prmMin==prmMax)return prmMin;										//Pour eviter la division par 0
	return prmMin+(rand())/((RAND_MAX/(prmMax-prmMin)));
}
/*######*/