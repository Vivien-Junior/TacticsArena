#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fonc.h"

/**
 *\file affichages.c
 *\brief affichages
 *\author Vivien-Junior
 *\author Marius
 *\version 0.1
 *\date 28/02/2019
 * fonctions d'affichages de la carte, des sorts, des caracteristiques des sorts..
*/

/**
 *\fn void affichage_sort(t_personnage * perso)
 *\brief affichage des la liste des sorts utilisables par le personnage passé en paramètre
 *\param perso personnage dont on va afficher sa liste de sorts
 *\return void
 */
void affichage_sort(t_personnage * perso){
/*affiche les sorts selon le personnage choisi par l'utilisateur*/
	int i;
	printf("Personnage : %s\n\n", perso->nom);
	printf("Listes des sorts: \n");

	/*affichage des quatres sorts*/
	for (i = 0; i < MAX_NB_SORT ; i++)
	printf("- %s : portée [%i] / dégât [%i] / utilisation par tour [%i]\npoint d'action [%i]\n", perso->sorts[i]->nom, perso->sorts[i]->portee, perso->sorts[i]->degat, perso->sorts[i]->upt, perso->sorts[i]->point_action);
}

/**
 *\fn void sort_uti(t_personnage * perso)
 *\brief affiche les caracteristiques des differents sorts selon le personnage choisi par l'utilisateur
 *\param perso personnage dont on va afficher les caracteristiques des ses sorts
 *\return void
 */
void sort_uti(t_personnage * perso){
	int i = 1;
	printf(" ---- Quel sort souhaitez vous effectuer ? ---- \n");

	for (i = 0; i < MAX_NB_SORT ; i++)
	printf("[%i] %s : portée [%i] / dégât [%i] / utilisation par tour [%i]\npoint d'action [%i]\n",i+1, perso->sorts[i]->nom, perso->sorts[i]->portee, perso->sorts[i]->degat, perso->sorts[i]->upt, perso->sorts[i]->point_action);
	printf("[%i] Ne pas utiliser de sort\nchoix:",i+1);
}

/**
 *\fn void affichage_map(char map[N][N])
 *\brief affiche le plan de jeu
 *\param matriceJeu[N][N] matrice qui sert au tour du jeu
 *\return void
 * les personnages des joueurs sont représentés par des chiffres sur l'affichage de la carte
 */
void affichage_map(char map[N][N]){

	int i, j;

	printf(" ---- [Plan de jeu] ---- \n\n");

	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++)
			printf(" %c |", map[i][j]);
		printf("\n");
	}
	printf("\n");
}

/**
 *\fn void affichage_coord(t_equipe * equipe)
 *\brief affichage des coordonnees, du nom, de la vie des personnages de l'equipe passée en paramètre
 *\param equipe structure contenant tout les personnages de l'équipe
 *\return void
 */
void affichage_coord(t_equipe * equipe){
	printf("equipe %i : \n", equipe->numEquipe);
	printf("- Coordonnées %s : x = %i et y = %i {Vie actuelle:%i}\n", equipe->perso1->nom, equipe->perso1->coord.x, equipe->perso1->coord.y,equipe->perso1->pv);
  printf("- Coordonnées %s : x = %i et y = %i {Vie actuelle:%i}\n", equipe->perso2->nom, equipe->perso2->coord.x, equipe->perso2->coord.y,equipe->perso2->pv);
	printf("\n");
}

/**
 *\fn void maj(char map[N][N], t_equipe * equipe1, t_equipe * equipe2)
 *\brief Fonction de mise à jour de l'affichage de la map
 *\param matriceJeu[N][N] matrice qui sert au tour du jeu
 *\param  equipe1 utilisé afin d'obtenir les coordonnées de tout les persos
 *\param equipe2 utilisé afin d'obtenir les coordonnées de tout les persos
 *\return void
 * la fonction replace juste les joueurs à leurs nouvelles coordonnees si un déplacement à été effectué
 * la fonction n'affiche pas les personnages s'ils sont morts (vie < 0)
 * le caractère 'o' correspond au obstacle sur la map et '.' sont tout les position atteignables par les personnages
 */
void maj(char map[N][N], t_equipe * equipe1, t_equipe * equipe2){
	int i, j;
	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++){
			if(map[i][j] != 'o')
				map[i][j] = '.';
        }
	}

    /* equipe 1 */
    if(!est_mort(equipe1,1)){
	    //itoa(equipe1->numEquipe, &map[equipe1->perso1->coord.y][equipe1->perso1->coord.x], 10);
			map[equipe1->perso1->coord.y][equipe1->perso1->coord.x] = equipe1->numEquipe + '0';
    }
    if(!est_mort(equipe1,2)){
      //itoa(equipe1->numEquipe+2, &map[equipe1->perso2->coord.y][equipe1->perso2->coord.x], 10);
			map[equipe1->perso2->coord.y][equipe1->perso2->coord.x] = equipe1->numEquipe + '2';
    }
    /* equipe 2*/
    if(!est_mort(equipe2,1)){
	    //itoa(equipe2->numEquipe, &map[equipe2->perso1->coord.y][equipe2->perso1->coord.x], 10);
			map[equipe2->perso1->coord.y][equipe2->perso1->coord.x] = equipe2->numEquipe + '0';
    }
    if(!est_mort(equipe2,2)){
      //itoa(equipe2->numEquipe+2, &map[equipe2->perso2->coord.y][equipe2->perso2->coord.x], 10);
			map[equipe2->perso2->coord.y][equipe2->perso2->coord.x] = equipe2->numEquipe + '2';
    }
}


/**
 *\fn char carac_perso(int numj,int nump)
 *\brief Fonction qui retourne le caractère correspondant au personnage d'indice: numero_personnage du equipe : indice_equipes
 *\param numj numéro du l'equipe dont on veut savoir le caractère
 *\param nump numéro du personnage du equipe (numj)
 *\return char : le caractère du personnage correspondant
 */
char carac_perso(int numj,int nump){
	char c;
	if(numj == 1){
		if(nump == 1){
			c = '1';
		}
		else{
			c = '3';
		}
	}
	else{
		if(nump == 1){
			c = '2';
		}
		else{
			c = '4';
		}
	}

	return c;
}
