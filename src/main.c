#include <stdio.h>
#include <stdlib.h>
#include "fonc.h"
#include "define.h"

/**
 *\file main.c
 *\brief programme principale du jeu
 *\author Dylan
 *\author Marius
 *\version 0.1
 *\date 28/02/2019
*/

char map[N][N];/*!< tableau globale qui sert de carte de  jeu */


/**
*\fn int main()
*\brief programme principale
*\return void
* l'ajout de classe peut etre effectué à partir du fichier classe.txt
* de meme pour les sorts à partir du fichier sorts.txt
* la création des personnages, de la liste des sorts, de la liste des classe sont effectués ici
* le choix des classe est effectué dans le programme principale
* le jeu s'arrete quand l'une des deux équipes n'a plus de personnages
* une destruction des sorts, des classes, et des personnages est effectué à la fin du programme
*/
int main(){

	int i,val,j,test=0,nb_tour = 1,mort1 = 0,mort2 = 0;
	int classe[NB_PERSONNAGES+1];
  int indice_equipe = 1,nump = 1;
	int type_partie;
	int nb_joueur;

	t_equipe * equipe1, * equipe2;

	/*Creation de toutes les classes et tous les sorts */
	t_personnage * persos[CLASSES+1];
	t_sort * sorts[SORTS+1];
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

	do {
		printf("Voulez vous jouez en local[1] ou distant[2] ? \n");
		scanf("%d", &type_partie );
	}while((type_partie !=1 && type_partie !=2));
	do{
		printf("Combien de joueurs 2 ou 4 ? \n");
		scanf("%d", &nb_joueur );
	}while((nb_joueur !=2 && nb_joueur != 4));

	//jouer une partie distante
	if(type_partie==2){
		serveur(nb_joueur, persos ,equipe1 ,equipe2);
		tab[1]->perso1=NULL;
		tab[1]->perso2=NULL;
		tab[2]->perso1=NULL;
		tab[2]->perso2=NULL;
	}

	//jouer une partie locale
	else{
			for(j = 1 ; j <= NB_EQUIPES; j++){

	      do{
	           printf("L'equipe %d choisi ses classes : \n",j);

	          for(i = 1; i <= CLASSES; i++){

	              printf("[%d] : %s\n",i,persos[i]->nom);

	          }

	          for (i = 1; i <= NB_PERSONNAGES; i++){
	              printf("Choix personnage %d: ",i);
	              scanf("%d",&val);
	              classe[i] = val;
	          }
	          test = 0;

	          for(i = 1;i <= NB_PERSONNAGES && test != 1; i++){
	              if((classe[i] < 1) || (classe[i] > CLASSES))
	                  test = 1;
	              else
	                  test = 0;
	          }

	          if(test)
	              printf("Vous devez taper un nombre compris entre 1 ou %i\n",CLASSES);

	      }while(test);

      tab[j]->perso1 = copie_perso(persos[classe[1]]);
      tab[j]->perso2 = copie_perso(persos[classe[2]]);
      tab[j]->numEquipe = j;
      tab[j]->nbPersoVivant = NB_PERSONNAGES;

  	}

	}
	/*Fin de la création des équipes et personnages par équipe*/

	initialisation(map,equipe1,equipe2);
  printf("===================================================\n\tDEMARRAGE DE LA  PARTIE\n===================================================\n");
	maj(map,equipe1,equipe2);
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
					tour(map,equipe2,equipe1,nump);
					indice_equipe--;
				}
				else if(nump == 2 && !est_mort(tab[indice_equipe],nump))
					tour(map,equipe2,equipe1,nump);



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
	for(i = 1; i <= SORTS; i++){
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
