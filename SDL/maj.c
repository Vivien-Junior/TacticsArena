#include <stdio.h>
#include <stdlib.h>
#include "fonc.h"
/**
 *\file maj.c
 *\brief Mise à jour
 *\author Vivien-Junior
 *\author Marius
 *\version 0.1
 *\date 28/02/2019
 * fonction de mise à jour de la map
*/

/**
 *\fn void maj(char map[N][N], t_joueur * joueur1, t_joueur * joueur2)
 *\brief Fonction de mise à jour de l'affichage de la map
 *\param map[N][N] matrice qui sert de plan de jeu
 *\param joueur1 utilisé afin d'obtenir les coordonnées des persos du joueur 1
 *\param joueur2 utilisé afin d'obtenir les coordonnées des persos du joueur 2
 *\return void
 * la fonction replace les joueurs à leurs nouvelles coordonnees si un déplacement à été effectué
 * la fonction n'affiche pas les personnages s'ils sont morts (vie < 0)
 * le caractère 'o' correspond aux obstacles sur la map et '.' aux  case classiques du jeu
 */
void maj(char map[N][N], t_joueur * joueur1, t_joueur * joueur2){
	int i, j;
	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++){
			if(map[i][j] != 'o')
				map[i][j] = '.';
        }
	}
    /* joueur 1 */
    if(!est_mort(joueur1,1))
	    map[joueur1->perso1->coord.y][joueur1->perso1->coord.x] = '1';
    if(!est_mort(joueur1,2))
        map[joueur1->perso2->coord.y][joueur1->perso2->coord.x] = '3';
    /* joueur 2*/
    if(!est_mort(joueur2,1))
	   map[joueur2->perso1->coord.y][joueur2->perso1->coord.x] = '2';
    if(!est_mort(joueur2,2))
        map[joueur2->perso2->coord.y][joueur2->perso2->coord.x] = '4';
}