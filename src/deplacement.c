#include <stdio.h>
#include <stdlib.h>
#include<math.h>
#include "fonc.h"

/**
 *\file deplacement.c
 *\brief programme d'initialisation du jeu
 *\author Dylan
 *\version 0.1
 *\date 28/02/2019
*/


/**
*\fn int deplacement(t_equipe * equipe1,t_equipe * equipe2,char map[N][N],int  pm ,int nump)
*\brief  Fonction permettant de deplacer son personnage et changer ses coordonnées
*\param equipe1 equipe que l'on souhaite déplacer
*\param equipe2 equipe adversaire utilisée pour obtenir leurs coordonnees
*\param map[N][N] matrice qui sert au tour du jeu
*\param pm nombre de déplacement encore disponible pour le personnage que l'on souhaite déplacer
*\param nump numéro du personnage que l'on souhaite déplacer dans l'equipe
*\return int : qui correspondant au nombre de déplacement qui restera au personnage (nump de l'equipe 1) au prochain tour
*
* La fonction vérifie si le personnage n'essaye pas d'aller hors de la map
* en cas de déplacement effectué ou d'un échec un message est affiché correspondant à l'action effectué
* suite au déplacement on effectue une mise à jour de la map pour que le déplacement effectué soit pris en compte lors de l'affichage de la map
*/
int deplacement(t_equipe * equipe1,t_equipe * equipe2,char map[N][N],int  pm ,int nump){
    t_personnage * temp;
    int i = 0,compteur,j,test=0;


    if(nump == 1 ){
        temp = equipe1->perso1;
    }
    else{
        temp = equipe1->perso2;
    }

    char c;

    do{
        printf("Choisissez l'initial entre haut, bas, gauche, ou droite pour vous déplacer ou R pour rester sur place\n");

	  while ((c = getchar()) != EOF && c != '\n');

        scanf(" %c",&c);
    }
    while(c != 'h' && c != 'b' && c != 'g' && c != 'd' && c != 'R');

  		switch (c) {
  			case 'h':

  				if(temp->coord.y-1<0 || (map[temp->coord.y-1][temp->coord.x]!='.')){
    				printf(" ---- Déplacement impossible ---- \n" );
  					break;
  				}
  				else{
  					(temp->coord.y)--;
    				printf(" ---- Déplacement en Haut ---- \n" );
  					pm--;
  					break;
  				}

  			case 'b':

  				if(temp->coord.y+1>=N || (map[temp->coord.y+1][temp->coord.x]!='.')){
    				printf(" ---- Déplacement impossible ---- \n" );
  					break;
  				}

  				else{
  					(temp->coord.y)++;
    				printf(" ---- Déplacement en Bas ---- \n" );
  					pm--;
  					break;
  				}

  			case 'g':

  				if(temp->coord.x-1<0 || (map[temp->coord.y][temp->coord.x-1]!='.')){
    				printf(" ---- Déplacement impossible ---- \n" );
  					break;
  				}

  				else{
  					(temp->coord.x)--;
    				printf(" ---- Déplacement à Gauche ---- \n" );
  					pm--;
  					break;
  				}

  			case 'd':

  				if(temp->coord.x+1>=N || (map[temp->coord.y][temp->coord.x+1]!='.')){
    				printf(" ---- Déplacement impossible ---- \n" );
  					break;
  				}

  				else{
  					(temp->coord.x)++;
    				printf(" ---- Déplacement à Droite ---- \n" );
  					pm--;
  					break;
  				}

  			case 'R':

  				return pm;
  		}

  if(equipe1->numEquipe == 1){
    maj(map,equipe1,equipe2);
    affichage_map(map);
  }
  else {
    maj(map,equipe2,equipe1);
    affichage_map(map);
  }
	return pm;
}
