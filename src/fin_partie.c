#include <stdio.h>
#include <stdlib.h>
#include "fonc.h"

/**
 *\file fin_partie.c
 *\brief Fonctions de test sur la fin de la partie
 *\author Marius
 *\author Vivien-Junior
 *\version 0.1
 *\date 28/02/2019
*/



 /**
  *\fn int est_mort(t_equipe * equipe, int nump)
  *\brief fonction qui teste si le personnage (nump) de l'equipe (equipe) est mort
  *\param equipe struct equipe
  *\param nump  personnage n du equipe dont on veut tester la mort
  *\return int : retourne 1 si le personnage n°nump de l'equipe est encore en vie
  */
int est_mort(t_equipe * equipe, int nump){
	int bool = 0;
	/* le personnage est deja mort */
	if(equipe->perso1->pv == -1 && nump == 1){
		return 1;
	}
	if(equipe->perso2->pv == -1 && nump == 2){
		return 1;
	}
	/*sinon s'il vient de mourir */
	switch(nump){
		case 1:
				if(equipe->perso1->pv <= 0 ){
					/* on check qu'il soit mort puis on lui mets un nombre de points de vie suffisament petit
					 * pour pas qu'il soit reconsideré comme mort de nouveau dans les autres fonctions
					 faisant appel est_mort();
					 */
					equipe->nbPersoVivant--;
					equipe->perso1->pv =-1;
          printf(" Le %s du equipe %i est mort ",equipe->perso1->nom,equipe->numEquipe);
					bool = 1;
				}
				break;
		case 2:
				if(equipe->perso2->pv <= 0){
					equipe->nbPersoVivant--;
					equipe->perso2->pv =-1;
          printf(" Le %s du equipe %i est mort ",equipe->perso2->nom,equipe->numEquipe);
					bool = 1;
				}
				break;
	}
	return bool;
}

 /**
  *\fn int partie_finie(t_equipe * equipe)
  *\brief fonction booleenne qui teste l'equipe n'a plus de joueur vivant
  *\param equipe struct equipe
  *\return int : retourne vrai s'il reste des personnages dans l'equipe
  */
int partie_finie(t_equipe * equipe){
	return (equipe->nbPersoVivant);
}
