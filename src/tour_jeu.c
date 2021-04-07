#include <stdio.h>
#include <stdlib.h>
#include "fonc.h"
#include <string.h>

/**
 *\file tour_jeu.c
 *\brief fonction du tour de jeu d'un personnage
 *\author Marius
 *\author Dylan
 *\author Alexandre
 *\version 0.1
 *\date 28/02/2019
*/

/**
 *\fn void tour(char map[N][N],t_equipe * equipe1,t_equipe * equipe2,int nump )
 *\brief fonction de gestion d'un tour de jeu
 *\param map carte de jeu
 *\param equipe1 personnage actif pendant le tour jeu
 *\param equipe2 personnage passif durant le tour de jeu
 *\param numj numéro du equipe qui joue actuellement
 *\param nump numéro du %numj actuellement
 *\return void
 * la fonction demande au personne s'il souhaite effectuer un déplacement/utiliser un ou des sort(s)/passer son tour et ne rien faire
 * une mise à jour de la carte est effectué après avoir effectué une action (déplacement,sort) pour faire disparaitre un joueur qui serait mort après l'utilisation d'un sort
 */
void tour(char map[N][N],t_equipe * equipe1,t_equipe * equipe2,int nump ){

		char c;
		t_personnage * temp;

		if(nump == 1 ){
        temp = equipe1->perso1;
    }
    else{
        temp = equipe1->perso2;
    }

		if (!(strcmp("armure",temp->effets[0].nom))){
				creer_effet(temp,6,temp->coord.x,temp->coord.y);
		}

    /* variable qui compte le nombre de déplacement max possible par personnage*/
    int pm=temp->pm;
    int choix_action = 0,choix_sort =0;
    temp->sorts[0]->upt=temp->sorts[0]->uptm;
    temp->sorts[1]->upt=temp->sorts[1]->uptm;
    temp->sorts[2]->upt=temp->sorts[2]->uptm;
    temp->sorts[3]->upt=temp->sorts[3]->uptm;

		/* variable qui compte le nombre de points d'actions max du personnage 1 */
		temp->pa = temp->pa_max;

		if (!(strcmp("minotaure",temp->effets[1].nom)))
			temp->pa += 1;

		if (!(strcmp("felin",temp->effets[1].nom)))
			pm = pm * 2;

    /* tant que le equipe ne passe pas son tour OU s'il lui reste des points de déplacements et d'actions */
    while(choix_action != 3 /*|| (pm > 0 && point_action > 0)*/){
        do{
            printf(" ---- Quelle action souhaitez vous effectuer ? ---- \n[1]:Se déplacer ?[nombre de déplacement:%i]\n[2]:Utiliser un sort ? [nombre de points d'actions:%i]\n[3]:Passer son tour\nchoix:",pm,temp->pa);

						while ((c = getchar()) != EOF && c != '\n');

            scanf("%i",&choix_action );
        }
        while(choix_action < 1 || choix_action > 3);

        switch(choix_action){
            case 1:

                if(pm > 0 ){
                    pm = deplacement(equipe1,equipe2,map,pm,nump);
                }
                else{
                    printf("\n ---- Vous avez utilisé tous vos points de déplacements ----\n\n");
                }
                break;

								case 2:

		                if(temp->pa > 0){
		                    do{
		                        /*affichage de la liste des sorts utilisable par le personnage actuel*/
		                        sort_uti(temp);
		                        scanf("%i",&choix_sort );
		                    }
		                    while(choix_sort < 1 || choix_sort > MAX_NB_SORT+1);

												if (choix_sort == 5)
														printf(" ---- Vous n'avez utilisé aucun sort,vous ne perdez aucun point d'action ----\n\n");

												else {
														if (temp->sorts[choix_sort-1]->point_action <= temp->pa && temp->sorts[choix_sort-1]->upt > 0 ) {
																if(temp->sorts[choix_sort-1]->sort(map,temp,equipe2,equipe1,nump,temp->sorts[choix_sort-1]->degat,temp->sorts[choix_sort-1]->portee,equipe1->numEquipe,0)){
																	temp->pa -= temp->sorts[choix_sort-1]->point_action ;
																	temp->sorts[choix_sort-1]->upt-=1;
																}
														}
														else if(temp->sorts[0]->upt == 0){
																printf(" ---- Vous ne pouvez plus utiliser ce sort ce tour ci ----\n\n");
														}
														else{
																printf(" ---- Vous n'avez pas assez de points d'actions ----\n\n");
														}
		                    }

		                }

										else{
												printf(" ---- Vous n'avez pas assez de points d'actions ----\n\n");
											break;

										}
										break;

								case 3:
										printf("\n ---- Vous avez passé votre tour ---- \n\n");
											break;
		        }
						if(equipe1->numEquipe == 1){
					    maj(map,equipe1,equipe2);
					  }
					  else {
					    maj(map,equipe2,equipe1);
					  }
		    }
		}
