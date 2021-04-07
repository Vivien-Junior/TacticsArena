#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "fonc.h"


/**
 *\file init.c
 *\brief programme d'initialisation du jeu
 *\author Marius
 *\author Dylan
 *\author Vivien-Junior
 *\version 0.1
 *\date 28/02/2019
*/


/**
*\fn void initialisation(char matriceJeu[N][N],t_equipe * equipe1,t_equipe * equipe2)
*\brief initialise la position des deux equipes
*\param matriceJeu[N][N] matrice qui sert au tour du jeu
*\param equipe numéro de l'equipe qui sera placé sur la carte
*\return void
* la fonction génére aussi des obstacles sur la carte
* les caractères impairs ('1' et '3') sont utilisés pour l'équipe une, les caractères pairs pour l'equipe 2
*/

/*init de la map, et placement des perso*/
void initialisation(char matriceJeu[N][N],t_equipe * equipe1,t_equipe * equipe2){
	srand(time(NULL));
	int y1 = rand()%3+(N-3), x1 = rand()%(N-4)+2, y2, x2;
	int i,j,choix1 = 0,choix2 = 0,numero_j=1;
	int nb_persos = 1;
	int nb_obs = 10; /* nb d'obstacles max sur la carte*/
    /* remplissage de la map sans les equipes positionnés */
	for(i = 0; i < N ; i++){
		for(j = 0;j < N; j++){
				matriceJeu[i][j] = '.';
		}
	}

	while(numero_j <=2){
		printf("\n ---- Choix des cases possibles pour le equipe %i ---- \n",numero_j);
		/* choix des positions de la premiere equipe*/
		if(numero_j == 1){
			matriceJeu[y1][x1]='1';
			matriceJeu[y1-1][x1-2]='2';
			matriceJeu[y1 -1][x1 + 2]='3';
			printf("\n[1]{x=%i y=%i}\n[2]{x=%i y=%i}\n[3]{x=%i y=%i}\n",x1,y1,x1-2,y1-1,x1+2,y1-1);
			affichage_map(matriceJeu);
			do{
				printf("Choix pour le perso 1 du equipe %i: ",numero_j);
				scanf("%i", &choix1);
                printf("Choix pour le perso 2 du equipe %i: ",numero_j);
				scanf("%i", &choix2);

			}while( ((choix1 < 1)||(choix1 >3)) || ((choix2 < 1)||(choix2 >3)) || (choix1 == choix2) );

			switch(choix1){
				case 1: matriceJeu[y1][x1] = '1';
					matriceJeu[y1-1][x1-2]='.';
					matriceJeu[y1 -1][x1 + 2]='.';
					equipe1->perso1->coord.x = x1;
					equipe1->perso1->coord.y = y1;
				break;

				case 2:
                    matriceJeu[y1][x1] = '.';
					matriceJeu[y1-1][x1-2]='1';
					matriceJeu[y1 -1][x1 + 2]='.';
					equipe1->perso1->coord.x = x1-2;
					equipe1->perso1->coord.y = y1-1;
				break;

				case 3:
                    matriceJeu[y1][x1] = '.';
					matriceJeu[y1-1][x1-2]='.';
					matriceJeu[y1 -1][x1 + 2]='1';
					equipe1->perso1->coord.x = x1+2;
					equipe1->perso1->coord.y = y1-1;
				break;
			}

      switch(choix2){ /* choix case du perso 2 du equipe 1 */
				case 1: matriceJeu[y1][x1] = '3';
					matriceJeu[y1-1][x1-2]='.';
					matriceJeu[y1 -1][x1 + 2]='.';
                    matriceJeu[equipe1->perso1->coord.y][equipe1->perso1->coord.x] = '1';
					equipe1->perso2->coord.x = x1;
					equipe1->perso2->coord.y = y1;

				break;

				case 2: matriceJeu[y1][x1] = '.';
					matriceJeu[y1-1][x1-2]='3';
				    matriceJeu[y1 -1][x1 + 2]='.';
                    matriceJeu[equipe1->perso1->coord.y][equipe1->perso1->coord.x] = '1';
					equipe1->perso2->coord.x = x1-2;
					equipe1->perso2->coord.y = y1-1;
				break;

				case 3:matriceJeu[y1][x1] = '.';
					matriceJeu[y1-1][x1-2]='.';
					matriceJeu[y1 -1][x1 + 2]='3';
                    matriceJeu[equipe1->perso1->coord.y][equipe1->perso1->coord.x] = '1';
					equipe1->perso2->coord.x = x1+2;
					equipe1->perso2->coord.y = y1-1;
				break;
			}
		}
		else{
			/* choix des positions du deuxième equipe */
			y2 = rand()%3;
			x2 = rand()%(N-4)+2;
			matriceJeu[y2][x2]='4';
			matriceJeu[y2+1][x2-2]='5';
			matriceJeu[y2 +1][x2 + 2]='6';
			printf("\n[4]{x=%i y=%i}\n[5]{x=%i y=%i}\n[6]{x=%i y=%i}\n",x2 ,y2 ,x2 -2,y2+1,x2+2,y2+1);

			affichage_map(matriceJeu);
				do{
                    printf("Choix pour le perso 1 du equipe %i: ",numero_j);
    				scanf("%i", &choix1);
                    printf("Choix pour le perso 2 du equipe %i: ",numero_j);
    				scanf("%i", &choix2);
                }
                while(((choix1 < 4)||(choix1 >6)) || ((choix2 < 4)||(choix2 >6)) || (choix1 == choix2));

				switch(choix1){
					case 4: matriceJeu[y2][x2]='2';
						matriceJeu[y2+1][x2-2]='.';
						matriceJeu[y2 +1][x2 + 2]='.';
						equipe2->perso1->coord.x = x2;
						equipe2->perso1->coord.y = y2;
					break;

					case 5: matriceJeu[y2][x2]='.';
						matriceJeu[y2+1][x2-2]='2';
						matriceJeu[y2+1][x2 + 2]='.';
						equipe2->perso1->coord.x = x2-2;
						equipe2->perso1->coord.y = y2+1;
					break;

					case 6: matriceJeu[y2][x2]='.';
						matriceJeu[y2+1][x2-2]='.';
						matriceJeu[y2 +1][x2 + 2]='2';
						equipe2->perso1->coord.x = x2+2;
						equipe2->perso1->coord.y = y2+1;
					break;
				}
                switch(choix2){ /* perso 2 equipe 2 */
					case 4: matriceJeu[y2][x2]='4';
						matriceJeu[y2+1][x2-2]='.';
						matriceJeu[y2 +1][x2 + 2]='.';
                        matriceJeu[equipe2->perso1->coord.y][equipe2->perso1->coord.x] = '2';
						equipe2->perso2->coord.x = x2;
						equipe2->perso2->coord.y = y2;
					break;

					case 5: matriceJeu[y2][x2]='.';
						matriceJeu[y2+1][x2-2]='4';
						matriceJeu[y2+1][x2 + 2]='.';
                        matriceJeu[equipe2->perso1->coord.y][equipe2->perso1->coord.x] = '2';
						equipe2->perso2->coord.x = x2-2;
						equipe2->perso2->coord.y = y2+1;
					break;

					case 6: matriceJeu[y2][x2]='.';
						matriceJeu[y2+1][x2-2]='.';
						matriceJeu[y2 +1][x2 + 2]='4';
                        matriceJeu[equipe2->perso1->coord.y][equipe2->perso1->coord.x] = '2';
						equipe2->perso2->coord.x = x2+2;
						equipe2->perso2->coord.y = y2+1;
					break;
				}
		}

		choix1 = 0;
        choix2 = 0;
		numero_j++;
	}
    /* obstacles générés sur toutes la carte sauf les bords */
	int obs_x = rand()%(N-2)+1 , obs_y = rand()%(N-2)+1;

	/* génération des obstacles sur la carte */
	i = 1;
	while(i <= nb_obs){
		if(matriceJeu[obs_y][obs_x] == '.'){
			matriceJeu[obs_y][obs_x] = 'o';
			i++;
		}
		obs_x = rand()%(N-2)+1 , obs_y = rand()%(N-2)+1;
	}
	//affichage_map(matriceJeu);
}
