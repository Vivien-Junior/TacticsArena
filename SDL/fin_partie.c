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
  *\fn int est_mort(t_joueur * joueur, int nump)
  *\brief Fonction qui teste si le personnage (nump) du joueur (joueur) est mort
  *\param joueur le joueur sur qui le test s'effectue
  *\param nump le personnage n du joueur dont on veut tester la mort
  *\return int : retourne 1 si le personnage nump du joueur est encore en vie
*/
int est_mort(t_joueur * joueur, int nump){
	int bool = 0;
	/* le personnage est deja mort */
	if(joueur->perso1->pv == -1 && nump == 1){
		return 1;
	}
	if(joueur->perso2->pv == -1 && nump == 2){
		return 1;
	}
	/*sinon s'il vient de mourir */
	switch(nump){
		case 1:
			if(joueur->perso1->pv >-50 && joueur->perso1->pv <= 0 ){
				/* on check qu'il soit mort puis on lui mets un nombre de points de vie suffisament petit
				 * pour pas qu'il soit reconsideré comme mort de nouveau dans les autres fonctions
				 faisant appel est_mort();
				 */
				joueur->perso1->pv =-1;
                printf(" Le %s du joueur %i est mort ",joueur->perso1->nom,joueur->numJoueur);
				bool = 1;
			}
		break;
		case 2:
			if(joueur->perso2->pv >-50 && joueur->perso2->pv <= 0){
				joueur->perso2->pv =-1;
                printf(" Le %s du joueur %i est mort ",joueur->perso2->nom,joueur->numJoueur);
				bool = 1;
			}
		break;
	}
	return bool;
}
/**
  *\fn partie_finie(t_joueur * joueur)
  *\brief Fonction booleenne qui teste si le joueur n'a plus de personnage vivant
  *\param joueur joueur sur qui on teste la défaite
  *\return int : retourne vrai s'il reste des personnages vivants 
*/
int partie_finie(t_joueur * joueur){
	return (joueur->nbPersoVivant == 0);
}
