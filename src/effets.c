#include <stdio.h>
#include <stdlib.h>
#include "fonc.h"

/**
 *\file effets.c
 *\brief programme d'initialisation du jeu
 *\author Dylan
 *\version 0.1
 *\date 28/02/2019
*/

/**
 *\fn void creer_effet(t_personnage * p,int effet_voulu,int x,int y)
 *\brief fonction qui crée un effet
 *\param p personnage dont on veut créer l'effet
 *\param effet_voulu effet que l'on souhaite appliquer sur le personnage p
 *\param x coordonnees du personnages sur lequel l'effet sera effectué
 *\param y coordonnees du personnages sur lequel l'effet sera effectué
 *\return void
 */

void creer_effet(t_personnage * p,int effet_voulu,int x,int y){


  switch(effet_voulu){
    case (1):
        p->effets[0].nom = "armure";
        p->effets[0].duree = 100;
        p->effets[0].cible.x = x;
        p->effets[0].cible.y = y;
      break;
    case (2):
        p->effets[1].nom = "minotaure";
        p->effets[1].duree = 100;
        p->effets[1].cible.x = x;
        p->effets[1].cible.y = y;
      break;
    case (3):
        p->effets[1].nom = "felin";
        p->effets[1].duree = 100;
        p->effets[1].cible.x = x;
        p->effets[1].cible.y = y;
      break;

    case (6):
        p->effets[0].nom = "vide";
        p->effets[0].duree = 100;
        p->effets[0].cible.x = x;
        p->effets[0].cible.y = y;

        p->effets[1].nom = "vide";
        p->effets[1].duree = 100;
        p->effets[1].cible.x = x;
        p->effets[1].cible.y = y;

      break;
  }
}
