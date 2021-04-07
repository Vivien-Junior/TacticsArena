#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fonc.h"
#include "define.h"
#include "sdl_fonc.h"

/**
 *\file sdl_perso_sorts.c
 *\brief Fonction de création des sorts version SDL
 *\author Vivien-Junior
 *\version 0.1
 *\date 13/04/2019
*/

/**
 *\fn void suppr_sort(t_sort ** sort)
 *\brief Fonction de suppression de sort
 *\param sort sort que l'on souhaite supprimer
 *\return void
 * passage d'un double pointeur en paramètre pour pouvoir mettre le pointeur et non la copie à NULL
*/
void suppr_sort(t_sort ** sort){

		free((*sort)->nom);
		free(*sort);
		*sort = NULL;
}

/**
*\fn void suppr_perso(t_personnage ** perso)
*\brief Fonction de suppression de perso
*\param perso personnage que l'on souhaite supprimer
*\return void
* passage d'un double pointeur en paramètre pour pouvoir mettre le pointeur et non la copie à NULL
*/
void suppr_perso(t_personnage ** perso){
	int i;
	for(i = MAX_NB_SORT-1; i > 0; i--)
		suppr_sort(&((*perso)->sorts[i]));
	free((*perso)->nom);
	free(*perso);
	*perso = NULL;
}

/**
 *\fn void (*creer_sort(int id_sort))(t_joueur* ,t_joueur* ,int)
 *\brief Fonction de creation des pointeurs sur fonction pour mettre dans la structure des sorts
 *\param id_sort numero du sort que l'on souhaite créer
 *\return void
*/
void (*creer_sort(int id_sort))(t_joueur* ,t_joueur* ,int){

	void (*fonc )(t_joueur* ,t_joueur* ,int);
	switch (id_sort) {
		case 1:
			fonc = SDL_saut;
		break;
		case 2:
			fonc = SDL_soin;
		break;
		case 3:
			fonc = SDL_petit_coup;
		break;
		case 4:
			fonc = SDL_grosCoup;
		break;
		case 5:
			fonc = SDL_diago;
		break;
		case 6:
			fonc = SDL_ligne;
		break;
		case 7:
			fonc = SDL_double_tape;
		break;
		case 8:
			fonc = SDL_coup_zone;
		break;
		case 9:
			fonc = SDL_armure;
		break;
		case 10:
			fonc = SDL_attire;
		break;
		case 11:
			fonc = SDL_chenchen;
		break;
		case 12:
			fonc = SDL_bigshaq;
		break;
		case 13:
			fonc = SDL_minotaure;
		break;
		case 14:
			fonc = SDL_felin;
		break;
		case 15:
			fonc = SDL_fuego;
		break;
		case 16:
			fonc = SDL_revitalisation;
		break;
		default : fonc = NULL;
	}
	return fonc;
}

/**
*\fn void creation_sorts(t_sort * sorts[])
*\brief creation des tous les sorts en remplissant les structures
*\param sorts[] tableau vide dans lequel on va remplir toutes les informations sur tous les sorts
*\return void
* la fonction va lire les caractérisques de tout les sorts dans un fichier dont on a prédéfinie le format
*/
void creation_sorts(t_sort * sorts[]){

	FILE * s = NULL;
	s = fopen("sorts.txt","r");
	int verif,id,portee,degat,cd,uptm,point_action;
	/*Copie des valeurs de caractéristiques des sorts à partir d'un fichier*/
	if(s != NULL){
		char * nom = malloc(sizeof(char)* TAILLE_MAX);
		while((verif = fscanf(s,"%d %s %d %d %d %d %d",&id,nom,&portee,&degat,&cd,&uptm,&point_action)) != EOF){
			sorts[id]->id = id;
			sorts[id]->nom = nom;
			sorts[id]->portee = portee;
			sorts[id]->degat = degat;
			sorts[id]->cd = cd;
			sorts[id]->uptm = uptm;
			sorts[id]->point_action = point_action;
			sorts[id]->sort = creer_sort(id);
			nom = malloc(sizeof(char)*TAILLE_MAX);
		}
		free(nom);
		fclose(s);
	}
	else
		printf("Pas dans le fichier\n" );
}

/**
 *\fn void creation_classes(t_personnage * persos[],t_sort * sorts[])
 *\brief creation des toutes les classes en remplissant les structures
 *\param persos[] tableau vide dans lequel on va remplir toutes les informations sur toutes les classes possibles
 *\param sorts[] tableau de sorts contenant les sorts
 *\return void
 * la fonction va lire les caractérisques des personnages dans un fichier dont on a prédéfinie le format
 * on remplit les sorts de la classe crée selon les sorts du tableau sorts grâce à un indice recupéré dans le fichier classe.txt
*/
void creation_classes(t_personnage * persos[],t_sort * sorts[]){

	FILE * c = NULL;
	c = fopen("classe.txt","r");
	int verif;
	int id,point_action,pv,pv_max,pa,pm,s1,s2,s3,s4;

	if(c != NULL){
		char * nom2 = malloc(sizeof(char)*TAILLE_MAX);
		while((verif = fscanf(c,"%d %s %d %d %d %d %d %d %d %d",&id,nom2,&pv,&pv_max,&pa,&pm,&s1,&s2,&s3,&s4)) != EOF){
			persos[id]->id = id;
			persos[id]->nom = nom2;
			persos[id]->pv = pv;
			persos[id]->pv_max = pv_max;
			persos[id]->pa = pa;
			persos[id]->pm = pm;
			persos[id]->sorts[0] = sorts[s1];
			persos[id]->sorts[1] = sorts[s2];
			persos[id]->sorts[2] = sorts[s3];
			persos[id]->sorts[3] = sorts[s4];
			nom2 = malloc(sizeof(char)*TAILLE_MAX);
		}
		free(nom2);
		fclose(c);
	}
	else
		printf("Pas dans le fichier\n" );
}

/**
 *\fn t_sort * copie_sort(t_sort * sort)
 *\brief fonction qui effectue une copie d'un sort puis le retour le sort crée
 *\param sort sort sur lequel on souhaite effectuer une copie
 *\return t_sort : le nouveau sort crée en copie
*/
t_sort * copie_sort(t_sort * sort){

	t_sort * nv_sort = malloc(sizeof(t_sort));
	nv_sort->nom = malloc(sizeof(char)* TAILLE_MAX);
	nv_sort->id = sort->id;
	strcpy(nv_sort->nom,sort->nom);
	nv_sort->portee = sort->portee;
	nv_sort->degat = sort->degat;
	nv_sort->cd = sort->cd;
	nv_sort->uptm = sort->uptm;
	nv_sort->point_action = sort->point_action;
	nv_sort->sort = sort->sort;

	return nv_sort;
}

/**
*\fn t_personnage * copie_perso(t_personnage * perso)
*\brief copie de la classe que l'utilisateur souhaite utiliser
*\param perso perso sur lequel on souhaite effectuer la copie
*\return t_personnage : le nouveau personnage crée en copie
*/
t_personnage * copie_perso(t_personnage * perso){

	t_personnage * nv_perso = malloc(sizeof(t_personnage));
	nv_perso->nom = malloc(sizeof(char)* TAILLE_MAX);
	nv_perso->id = perso->id ;
	strcpy(nv_perso->nom,perso->nom);
	nv_perso->pv = perso->pv ;
	nv_perso->pv_max = perso->pv_max;
	nv_perso->pa = perso->pa ;
	nv_perso->pm = perso->pm ;
	nv_perso->sorts[0] = copie_sort(perso->sorts[0]);
	nv_perso->sorts[1] = copie_sort(perso->sorts[1]);
	nv_perso->sorts[2] = copie_sort(perso->sorts[2]);
	nv_perso->sorts[3] = copie_sort(perso->sorts[3]);
	strcpy(nv_perso->statut, "Inconnu");

	return nv_perso;
}

