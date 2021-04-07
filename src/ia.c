#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "fonc.h"
#include "define.h"

/**
 *\file ia.c
 *\brief programme de test du bot
 *\author Marius
 *\version 0.1
 *\date 28/02/2019
*/

char map[N][N];
typedef enum {NORD = 1,EST = 2,SUD = 3,OUEST = 4}t_direction;
int tour_ordi_opti(int indice_sort,char map[N][N],t_equipe * equipe1,t_equipe * equipe2,int numero_personnage,int * hors_portee);
int tour_equipe_opti(int indice_sort,char map[N][N],t_equipe * equipe1,t_equipe * equipe2,int numero_personnage,int * hors_portee);
int coup_ordi_opti(char map[N][N],t_equipe * equipe1,t_equipe * equipe2,int numero_personnage,int * hors_portee);
int eval_sort(int indice_sort,char map[N][N],t_equipe * equipe1,t_equipe * equipe2,int numero_personnage, int * hors_portee);
int eval_degat(t_personnage * perso,int indice_sort,int portee,int degat);
int eval_deplacement(t_equipe * equipe1,t_equipe * equipe2,int numero_personnage,t_direction * direction);
int eval_vie(t_equipe * equipe,int * indice_joueur);
void joueur_plus_proche(t_personnage * temp,t_equipe * equipe2,int * plus_proche);
void deplacement_ia(t_equipe * equipe, int numero_personnage,t_direction direction);
void degat_ia(t_equipe * equipe, int numero_personnage,int indice_sort);


/*
 * fonction qui évalue si un sort fais plus de dégat que le sort (indice_sort)
 * paramètre indice_sort: sort que l'on teste
 * paramètre portee : portée du sort indice_sort
 * paramètre degat: degat du sort indice_sort
 * return la portee du sort - 1
 */
int eval_degat(t_personnage * perso,int indice_sort,int portee,int degat){
    int j;
    for(j = 1; j <= MAX_NB_SORT; j++ ){
        /* on verifie si un sort avec la meme distance que le sort (indice_sort) n'est pas un nombre de dégat plus important */
        if(perso->sorts[indice_sort]->degat > degat )
                    return portee--;
    }
    return portee;
}

/*
 * fonction qui évalue quel personnage à le moins de vie dans l'équipe
 * paramètre indice_joueur: variable dans laquelle on mettras l'indice du personnage avec le moins de vie
 * return
 */
int eval_vie(t_equipe * equipe,int * indice_joueur){
	int i;
	*indice_joueur = 0;
	if(est_mort(equipe,1) && !est_mort(equipe,2)){ //si le personnage 1 est mort on retourne l'indice du 2nd

		*indice_joueur = 1;
	}
	else if(!est_mort(equipe,1) && est_mort(equipe,2)){ //si le personnage 2 est mort on retourne l'indice du 1er
		*indice_joueur = 0;
	}
	else if(equipe->perso1->pv <= equipe->perso2->pv ){
		*indice_joueur = 0;
	}
	else{
		*indice_joueur = 1;
	}
	return 0;
}

/*
 * Fonction qui retourne dans la variable "plus_proche" l'indice du joueur le plus proche du personnage "temp"
 * paramètre temp: joueur que l'on joue
 * paramètre equipe2: equipe adverse
 * paramètre plus_proche: variable dans laquelle on retourne l'indice du joueur le plus proche
 */
void joueur_plus_proche(t_personnage * temp,t_equipe * equipe2,int * plus_proche){
	int i,distance[NB_PERSONNAGES],carre;
	for(i = 0; i < NB_PERSONNAGES;i++){
        //on cherche à savoir quel est le personnage le plus proche du personnage 'perso'

        if(!i && !est_mort(equipe2,i)){
            carre = pow((double)(temp->coord.x - equipe2->perso1->coord.x),2) + pow((double)(temp->coord.y - equipe2->perso1->coord.y),2);
            distance[i] = sqrt((double)carre);
            *plus_proche = i;

        }
        else if(!est_mort(equipe2,i)){
            carre = pow((double)(temp->coord.x - equipe2->perso2->coord.x),2) + pow((double)(temp->coord.y - equipe2->perso2->coord.y),2);
            distance[i] = sqrt((double)carre);
            if(distance[i] < distance[i - 1]){ // si y'a un joueur plus proche que l'autre de l'adversaire
                *plus_proche = i; // on recupere l'indice du joueur le plus proche
			}
			else if(distance[i] == distance[i - 1]){ // si les adversaire sont à la meme distance
				// on recupere l'indice du joueur qui a le moins de vie
				eval_vie(equipe2,plus_proche);
			}
        }
    }
}

/*
 * Fonction d'évalutation pour le déplacement de l'ordi le "numero_personnage" que l'on souhaite déplacer ira dans une direction par rapport au
 * joueur adversaire le plus proche de lui si les deux adversaires sont à égales distances le joueur se déplacer vers le personnage le plus faible en vie
 * paramètre indice_sort: sort actuel que l'on soit tester
 * return : 1 si le déplacement est impossible 0 dans le cas contraire
 */

int eval_deplacement(t_equipe * equipe1,t_equipe * equipe2,int numero_personnage,t_direction * direction){
    srand(time(NULL));
    int choix = rand()%2;
    int i,carre,plus_proche = 0,distance_x,distance_y;
    int distance[NB_PERSONNAGES];
    t_personnage * temp;
    /* choix du personnage du equipe 1 */
    if(numero_personnage == 1)
        temp = equipe1->perso1;
    else
        temp = equipe1->perso2;

	joueur_plus_proche(temp,equipe2,&plus_proche);

    //on cherche à savoir quel est la coordonées la plus élévée entre les deux personnages le x ou le y
    if(!plus_proche){ //si le personnage le plus éloigné est le 1er personnage
        distance_x = temp->coord.x - equipe2->perso1->coord.x;
        distance_y = temp->coord.y - equipe2->perso1->coord.y;
    }
    else{
        distance_x = temp->coord.x - equipe2->perso2->coord.x;
        distance_y = temp->coord.y - equipe2->perso2->coord.y;
    }

    if(abs(distance_x) > abs(distance_y) ){
        //si l'ennemi le plus proche est situé à gauche ou à droite et que la distance en x est plus grande que la distance en y
        if(distance_x > 0){
            if(temp->coord.x-1<0 || (map[temp->coord.y][temp->coord.x-1]!='.')){
    			printf(" ---- Déplacement impossible 1 ---- \n" );
				return 1;
			}
    		else{
    			printf(" ---- Déplacement à Gauche ---- \n" );
    			*direction = OUEST;
    		}
        }
        else{
            if(temp->coord.x+1>=N || (map[temp->coord.y][temp->coord.x+1]!='.')){
                printf(" ---- Déplacement impossible 2 ---- \n" );
				return 1;
			}
            else{
                printf(" ---- Déplacement à Droite ---- \n" );
                *direction = EST;
            }
        }
    }
    else if( abs(distance_y) > abs(distance_x)){ // si la distance en y est supérieur à la distance en x
        if(distance_y > 0){
            if(temp->coord.y-1<0 || (map[temp->coord.y-1][temp->coord.x]!='.')){
    			printf(" ---- Déplacement impossible 3 ---- \n" );
				return 1;
			}
    		else{
    			printf(" ---- Déplacement en Haut ---- \n" );
    			*direction = NORD;
    		}
        }
        else{
            if(temp->coord.y+1>=N || (map[temp->coord.y+1][temp->coord.x]!='.')){
                printf(" ---- Déplacement impossible 4 ---- \n" );
				return 1;
			}
            else{
                printf(" ---- Déplacement en Bas ---- \n" );
                *direction = SUD;
            }
        }
    }
    else if (distance_y == distance_x){
		// on fais déplacer le personnage sur l'une des deux directions si le x et le y sont égaux
		if(!choix){ // si choix est nul on le fais déplacer sur le x
			//si l'ennemi le plus proche est situé à gauche ou à droite et que la distance en x est plus grande que la distance en y
	        if(distance_x > 0){
	            if(temp->coord.x-1<0 || (map[temp->coord.y][temp->coord.x-1]!='.')){
	    			printf(" ---- Déplacement impossible 1 ---- \n" );
					return 1;
				}
	    		else{
	    			printf(" ---- Déplacement à Gauche ---- \n" );
	    			*direction = OUEST;
	    		}
	        }
	        else{
	            if(temp->coord.x+1>=N || (map[temp->coord.y][temp->coord.x+1]!='.')){
	                printf(" ---- Déplacement impossible 2 ---- \n" );
					return 1;
				}
	            else{
	                printf(" ---- Déplacement à Droite ---- \n" );
	                *direction = EST;
	            }
	        }
		}
		else{
			if(distance_y > 0){
	            if(temp->coord.y-1<0 || (map[temp->coord.y-1][temp->coord.x]!='.')){
	    			printf(" ---- Déplacement impossible 3 ---- \n" );
					return 1;
				}
	    		else{
	    			printf(" ---- Déplacement en Haut ---- \n" );
	    			*direction = NORD;
	    		}
	        }
	        else{
	            if(temp->coord.y+1>=N || (map[temp->coord.y+1][temp->coord.x]!='.')){
	                printf(" ---- Déplacement impossible 4 ---- \n" );
					return 1;
				}
	            else{
	                printf(" ---- Déplacement en Bas ---- \n" );
	                *direction = SUD;
	            }
	        }
		}
    }

    return 0;
}

/*
 * Fonction qui effectue le déplacement du personnage controlé par l'ordinateur dans la direction choisi après l'évaluation de déplacement
 */
void deplacement_ia(t_equipe * equipe, int numero_personnage,t_direction direction){
	int dir = direction;
	t_personnage * temp;
    /* choix du personnage du equipe 1 */
    if(numero_personnage == 1)
        temp = equipe->perso1;
    else
        temp = equipe->perso2;

	switch (dir) {
		case NORD: temp->coord.y--;
		case EST: temp->coord.x++;
		case SUD: temp->coord.y++;
		case OUEST: temp->coord.x--;
	}
}

/*
 * Fonction qui effectue le déplacement du personnage controlé par l'ordinateur dans la direction choisi après l'évaluation de déplacement
 */
// void degat_ia(t_equipe * equipe, int numero_personnage,int indice_sort){
// 	int dir = direction;
// 	t_personnage * temp;
//     /* choix du personnage du equipe 1 */
//     if(numero_personnage == 1)
//         temp = equipe->perso1;
//     else
//         temp = equipe->perso2;
//
// 	switch (dir) {
// 		case NORD: temp->coord.y--;
// 		case EST: temp->coord.x++;
// 		case SUD: temp->coord.y++;
// 		case OUEST: temp->coord.x--;
// 	}
// }

/*
 * Fonction d'évalutation pour le MinMax retourne soit un entier supérieur ou égale à  soit un négatif
 * paramètre indice_sort: sort actuel que l'on soit tester
 * return : portee du sort (indice_sort) ou la différence entre la distance entre les deux equipes et la portée du sort
 */
int eval_sort(int indice_sort,char map[N][N],t_equipe * equipe1,t_equipe * equipe2,int numero_personnage,int * hors_portee){
    int portee = 0,i,degat;
    t_personnage * temp;
    /* choix du personnage du equipe 1 */
    if(numero_personnage == 1)
        temp = equipe1->perso1;
    else
        temp = equipe1->perso2;
    int distance,carre;

    //recupération de la portée et des dégats selon le sort

    portee = temp->sorts[indice_sort]->portee;
    degat = temp->sorts[indice_sort]->degat;

    /* pour tout les personnages possibles de l'adversaire */
    for(i = 0; i < NB_PERSONNAGES;i++){
        if(!i){
            /* test du sort sur le premier perso de l'adversaire */
            carre = pow((double)(temp->coord.x - equipe2->perso1->coord.x),2) + pow((double)(temp->coord.y - equipe2->perso1->coord.y),2);
            distance = sqrt((double)carre);
            if(distance <= portee){
                /* si distance entre les deux personages est plus petites que la portée du sort on retourne la portée du sort*/
                portee = eval_degat(temp,indice_sort,portee,degat);
                return portee;
            }
        }
        else{
            /* test du sort sur le deuxième perso de l'adversaire */
            carre = pow((double)(temp->coord.x - equipe2->perso2->coord.x),2) + pow((double)(temp->coord.y - equipe2->perso2->coord.y),2);
            distance = sqrt((double)carre);
            if(distance <= portee){
                portee = eval_degat(temp,indice_sort,portee,degat);
                return portee;
            }
        }
    }
    (*hors_portee)++;
    /* si on est hors portée pour tout les personnages */
    return (portee);
}

/*
 * fonction recursive qui teste de prévoir le coup optimal que l'ordinateur pourrait effectuer
 * retourne le meilleur coup s'il existe ou le maximum des coups
 */
int tour_ordi_opti(int indice_sort,char map[N][N],t_equipe * equipe1,t_equipe * equipe2,int numero_personnage,int * hors_portee){
	int i, score,eval;
	int max = -30;
    eval = eval_sort(indice_sort,map,equipe1,equipe2,numero_personnage,hors_portee);
    //si ce meilleur coup de l'ordi existe on retourne l'indice du meilleur coup
	if(eval >=0)/* fonction evaluation */
		return eval;
	else{
		for(i=0 ;i < MAX_NB_SORT; i++){
            if(numero_personnage == 1)
                score = tour_equipe_opti(i,map,equipe1,equipe2,numero_personnage,hors_portee);
            else
                score = tour_equipe_opti(i,map,equipe1,equipe2,numero_personnage,hors_portee);
			if( score > max){
				max = score;
			}
		}
	}
    return max;
}

/*
 * fonction recursive qui teste de prévoir le coup optimal que le equipe pourrait effectuer
 * paramètre nb: sort n°
 * paramètre equipe:
 */
int tour_equipe_opti(int indice_sort,char map[N][N],t_equipe * equipe1,t_equipe * equipe2,int numero_personnage,int * hors_portee){
	int i, score,eval;
	int min = 10;
    //on va chercher à savoir si on trouve un meilleur coup
    eval = eval_sort(indice_sort,map,equipe1,equipe2,numero_personnage,hors_portee);
    //si ce meilleur coup existe on retourne l'indice du meilleur coup
	if(eval >=0) /* fonction evaluation */
		return eval;
    else{
        for(i=0 ;i < MAX_NB_SORT; i++){
            if(numero_personnage == 1)
                score = tour_ordi_opti(i,map,equipe2,equipe1,numero_personnage,hors_portee);
            else
                score = tour_ordi_opti(i,map,equipe2,equipe1,numero_personnage,hors_portee);
            if( score < min){
    			min = score;
    		}
    	}
    }
    return min;
}

/*
 * Fonction qui effectue le meilleur coup de l'ordi
 */
int coup_ordi_opti(char map[N][N],t_equipe * equipe1,t_equipe * equipe2,int numero_personnage,int * hors_portee){
	int i = 0,score;
    int max = -30, imax=-1;

	/* pour tout les sorts on va tester de savoir qu'elle est le meilleur sort possible pour ce tour */
	for(i = 0; i < MAX_NB_SORT; i++){
        // on joue le coup
        score = tour_equipe_opti(i,map,equipe1,equipe2,numero_personnage,hors_portee);
        if( score > max){
			max = score;
            imax = i;
		}
	}
	return imax; /* on retourne l'indice du sort */
}


int main() {
    int i = 0,hors_portee = 0,pm_utilise=0,pa_utilise=0,ind_plus_proche = 0 ;
	int nump,indice_sort = 1,indice_equipe,nb_tour=1;
    int classe[NB_PERSONNAGES+1];
    t_equipe * equipe1,*equipe2;
    /*Creation de toutes les classes et tous les sorts */
	t_personnage * persos[CLASSES+1];
	t_sort * sorts[SORTS+1];
	t_direction direction = NORD;

	for(int i = 1; i <= SORTS;i++)
		sorts[i] = malloc(sizeof(t_sort));

	creation_sorts(sorts);

	for(int i = 1; i <= CLASSES;i++)
		persos[i] = malloc(sizeof(t_personnage));

	creation_classes(persos,sorts);
	/*fin de création des sorts et classes*/

    /*création des équipes et personnages par équipe*/
	equipe1 = malloc(sizeof(t_equipe));
	equipe2 = malloc(sizeof(t_equipe));

    t_equipe * tab[NB_EQUIPES+1];
	tab[1] = equipe1;
	tab[2] = equipe2;


    equipe1->perso1 = copie_perso(persos[1]);
	equipe1->perso2 = copie_perso(persos[2]);
    equipe1->numEquipe = 1;
    equipe1->nbPersoVivant = NB_PERSONNAGES;
    equipe2->perso1 = copie_perso(persos[2]);
	equipe2->perso2 = copie_perso(persos[1]);
    equipe2->numEquipe = 2;
    equipe2->nbPersoVivant = NB_PERSONNAGES;

    equipe1->perso1->coord.x = 1;
    equipe1->perso1->coord.y = 1;
	equipe1->perso2->coord.x = 1;
    equipe1->perso2->coord.y = 4;
	//j2
    equipe2->perso1->coord.x = 9;
    equipe2->perso1->coord.y = 7;
	equipe2->perso2->coord.x = 3;
    equipe2->perso2->coord.y = 3;
	maj(map,equipe1,equipe2);

    // int dist,carre,j;
    // carre = pow((double)(equipe1->perso1->coord.x - equipe2->perso1->coord.x),2) + pow((double)(equipe1->perso1->coord.y - equipe2->perso1->coord.y),2);
    // dist = sqrt((double)carre);
    // printf("{ distance = %i }\n",dist);

	indice_sort = coup_ordi_opti(map,equipe2,equipe1,2,&hors_portee);
	printf("sort :%d",indice_sort);

    affichage_map(map);
	/*Gestion des différents tours de jeu par equipe*/
	/* nump = perso1 ou perso2 de l'equipe, indice est le tour de jeu de l'equipe, equipe1 et equipe2 sont les équipes*/
	for(nump = 1, indice_equipe = 1;(partie_finie(equipe1) && partie_finie(equipe2));){

		/*vérification de l'état des persos de l'equipe 1 afin de savoir s'il peuvent jouer ou sont morts,changement d'indice pour passer au tour du perso suivant*/
		/*nb_tour est un compteur de tour global pour que le joueur sachent depuis combien de temps ils jouent*/
		if(indice_equipe == 1){
			printf("[Tour numéro:%i][Tour du equipe %i][personnage :%i]{Caractère : %c}\n\n",nb_tour,indice_equipe,nump,carac_perso(indice_equipe,nump));

			if(nump == 1 && !est_mort(tab[indice_equipe],nump))
				tour(map,equipe1,equipe2,nump);

			else if(nump == 2 && !est_mort(tab[indice_equipe],nump))
				tour(map,equipe1,equipe2,nump);

			indice_equipe++;


			maj(map,equipe1,equipe2);
			affichage_map(map);
		}
		/*vérification de l'état des persos de l'equipe 2 afin de savoir s'il peuvent jouer ou sont morts,changement d'indice pour passer au tour du perso suivant*/
		if(indice_equipe == 2){
			printf("[Tour numéro:%i][Tour du equipe %i][personnage :%i]{Caractère : %c}\n\n",nb_tour,indice_equipe,nump,carac_perso(indice_equipe,nump));

			if(nump == 1 && !est_mort(tab[indice_equipe],nump)){
				//tant que le personnage n'a pas utilisé tout ses points de mouvements ou d'actions on continue à lui faire effectuer des actions
				while(pm_utilise != 3 && pa_utilise != 2){
					indice_sort = coup_ordi_opti(map,equipe2,equipe1,nump,&hors_portee);
					if(hors_portee != MAX_NB_SORT){ // si un des adversaire est à portée on utilise le meilleur sort sur l'adversaire le plus proche
						if(pa_utilise < tab[indice_equipe]->perso1->pa_max ){
							joueur_plus_proche(equipe2->perso1,equipe1,&ind_plus_proche);
							if(ind_plus_proche == 1){
						        equipe1->perso1->pv -= tab[indice_equipe]->perso1->sorts[indice_sort + 1]->degat;
								printf("Le %s inflige %d au %s de l'equipe 1\n",tab[indice_equipe]->perso1->nom,tab[indice_equipe]->perso1->sorts[indice_sort + 1]->degat,equipe1->perso1->nom );
							}
						    else{
						        equipe1->perso2->pv -= tab[indice_equipe]->perso1->sorts[indice_sort + 1]->degat;
								printf("Le %s inflige %d au %s de l'equipe 1\n",tab[indice_equipe]->perso1->nom,tab[indice_equipe]->perso1->sorts[indice_sort + 1]->degat,equipe1->perso2->nom );
							}
							pa_utilise++;
						}
					}
					else{ //si on peut pas utiliser de sort on se déplace
						eval_deplacement(equipe2,equipe1,nump,&direction);
						deplacement_ia(equipe2,nump,direction);
						pm_utilise++;
					}
					hors_portee = 0;
					maj(map,equipe1,equipe2);
				}
				pm_utilise = 0;
				pa_utilise = 0;

				indice_equipe--;
			}
			else if(nump == 2 && !est_mort(tab[indice_equipe],nump)){
				//tant que le personnage n'a pas utilisé tout ses points de mouvements ou d'actions on continue à lui faire effectuer des actions
				while(pm_utilise != 3 && pa_utilise != 2){
					indice_sort = coup_ordi_opti(map,equipe2,equipe1,nump,&hors_portee);
					if(hors_portee != MAX_NB_SORT){ // si un des adversaire est à portée on utilise le meilleur sort sur l'adversaire le plus proche
						if(pa_utilise < tab[indice_equipe]->perso2->pa_max ){
							joueur_plus_proche(equipe2->perso2,equipe1,&ind_plus_proche);
							if(ind_plus_proche == 1){
								equipe1->perso1->pv -= tab[indice_equipe]->perso2->sorts[indice_sort + 1]->degat;
								printf("Le %s inflige %d au %s de l'equipe 1\n",tab[indice_equipe]->perso2->nom,tab[indice_equipe]->perso2->sorts[indice_sort + 1]->degat,equipe1->perso1->nom );
							}
							else{
								equipe1->perso2->pv -= tab[indice_equipe]->perso2->sorts[indice_sort + 1]->degat;
								printf("Le %s inflige %d au %s de l'equipe 1\n",tab[indice_equipe]->perso2->nom,tab[indice_equipe]->perso2->sorts[indice_sort + 1]->degat,equipe1->perso2->nom );
							}
							pa_utilise++;
							printf("equipe:%d degat :%d,indice sort :%d\n",indice_equipe,tab[indice_equipe]->perso2->sorts[indice_sort]->degat,indice_sort );
						}
					}
					else{ //si on peut pas utiliser de sort on se déplace
						eval_deplacement(equipe2,equipe1,nump,&direction);
						deplacement_ia(equipe2,nump,direction);
						pm_utilise++;
					}
					hors_portee = 0;
					maj(map,equipe1,equipe2);
				}
				pm_utilise = 0;
				pa_utilise = 0;
			}

			maj(map,equipe1,equipe2);
			affichage_map(map);
		}
		/*Affichage des coordonnées des différents personnages vivants,changement d'indice pour passer au tour du perso suivant, incrementation du tour global*/
		if((partie_finie(equipe1) || partie_finie(equipe2))&& indice_equipe == 2 && nump == 2){
			printf("===================================================\n\tAFFICHAGE COORDONNEES | FIN DU TOUR\n===================================================\n\n");
			affichage_coord(equipe1);
			affichage_coord(equipe2);
			indice_equipe--;
			nb_tour++;
		}

		if (nump == 2)
			nump --;

		else
			nump ++;
	}

	printf("===================================================\n\tFIN DE LA PARTIE\n===================================================\n\n");

	/*Fin de partie, affichage du vainqueur*/
		if(partie_finie(equipe1))
		printf("Le equipe 1 a perdu \n");
		else if(partie_finie(equipe2))
		printf("Le equipe 2 a perdu\n");

    /*suppression de tous les mallocs*/
	for(i = 1; i < SORTS; i++){
		suppr_sort(&(sorts[i]));
	}

	for(i = 1; i <= CLASSES; i++){
		suppr_perso(&(persos[i]));
	}

	for(i=1; i <= NB_EQUIPES; i++){
		suppr_perso(&(tab[i]->perso1));
		suppr_perso(&(tab[i]->perso2));
		free(tab[i]);
		tab[i] = NULL;
	}

    return 0;
}
