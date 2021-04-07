#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fonc.h"
#include "define.h"
/**
 *\file persosorts.c
 *\brief fonction création des sorts
 *\author Dylan
 *\version 0.1
 *\date 28/02/2019
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

	for(i = MAX_NB_SORT-1; i >= 0; i--){
//fprintf(stderr,"suppression sort %d\n",i);
		suppr_sort(&((*perso)->sorts[i]));
	}
	free((*perso)->nom);
	free(*perso);
	*perso = NULL;

}


/**
*\fn void (*creer_sort(int id_sort))(char [N][N], t_personnage * , t_equipe* ,t_equipe* ,int,int ,int,int )
*\brief Fonction de creation des pointeurs sur fonction pour mettre dans la structure des sorts
*\param id_sort numero du sort que l'on souhaite créer
*\return void
*/
int (*creer_sort(int id_sort))(char [N][N], t_personnage * , t_equipe* ,t_equipe* ,int,int ,int,int, int ){

	int (*fonc )(char [N][N], t_personnage * ,  t_equipe* ,t_equipe* ,int, int,int,int, int);


	switch (id_sort) {
		case 1:

			fonc = saut;
			break;
		case 2:
			fonc = soin;
			break;
		case 3:
			fonc = petit_coup;
			break;
		case 4:
			fonc = grosCoup;
			break;
		case 5:
			fonc = diago;
			break;
		case 6:
			fonc = ligne;
			break;
		case 7:
			fonc = double_tape;
			break;
		case 8:
			fonc = coup_zone;
			break;
		case 9:
			fonc = armure;
			break;
		case 10:
			fonc = attire;
			break;
		case 11:
			fonc = chenchen;
			break;
		case 12:
			fonc = bigshaq;
			break;
		case 13:
			fonc = minotaure;
			break;
		case 14:
			fonc = felin;
			break;
		case 15:
			fonc = fuego;
			break;
		case 16:
			fonc = revitalisation;
			break;
		default : fonc = NULL;
	}
	return fonc;
}

/**
*\fn t_personnage * creation_classes(t_personnage * persos[],t_sort * sorts[])
*\brief creation des toutes les classes en remplissant les structures
*\param persos[] tableau vide dans lequel on va remplir toutes les informations sur toutes les classes possibles
*\param sorts[] tableau de sorts contenant les sorts
*\return t_personnage
* la fonction va lire les caractérisques des personnages dans un fichier dont on a prédéfinie le format
* on remplit les sorts de la classe crée selon les sorts du tableau sorts grâce à un indice recupéré dans le fichier classe.txt
*/
void creation_sorts(t_sort * sorts[]){

	FILE * s = NULL;
	s = fopen("../src/sorts.txt","r");

	int verif,id,portee,degat,cd,uptm,point_action;


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


		//	printf("%d %s %d %d %d %d %d",id,nom,portee,degat,cd,uptm,point_action);
			nom = malloc(sizeof(char)*TAILLE_MAX);
		}
		free(nom);
		fclose(s);
	}
	else{
		printf("Pas dans le fichier\n" );

	}

}


/**
*\fn void creation_classes(t_personnage * persos[],t_sort * sorts[])
*\brief creation des toutes les classes en remplissant les structures
*\param persos[] tableau vide dans lequel on va remplir toutes les informations sur toutes les classes possibles
*\param sorts[] tableau de sorts contenant les sorts
*\return t_personnage
* la fonction va lire les caractérisques des personnages dans un fichier dont on a prédéfinie le format
* on remplit les sorts de la classe crée selon les sorts du tableau sorts grâce à un indice recupéré dans le fichier classe.txt
*/
void creation_classes(t_personnage * persos[],t_sort * sorts[]){

	FILE * c = NULL;
	c = fopen("../src/classe.txt","r");



	int verif;
	int id,point_action,pv,pv_max,pa,pm,s1,s2,s3,s4;



	if(c != NULL){
		char * nom2 = malloc(sizeof(char)*TAILLE_MAX);
		while((verif = fscanf(c,"%d %s %d %d %d %d %d %d %d %d",&id,nom2,&pv,&pv_max,&pa,&pm,&s1,&s2,&s3,&s4)) != EOF){

			persos[id]->id = id;
			persos[id]->nom = nom2;
			persos[id]->pv = pv;
			persos[id]->pv_max = pv_max;
			persos[id]->pa_max = pa;
			persos[id]->pm = pm;
			persos[id]->sorts[0] = copie_sort(sorts[s1]);
			persos[id]->sorts[1] = copie_sort(sorts[s2]);
			persos[id]->sorts[2] = copie_sort(sorts[s3]);
			persos[id]->sorts[3] = copie_sort(sorts[s4]);
			creer_effet(persos[id],6,persos[id]->coord.x,persos[id]->coord.y);

			nom2 = malloc(sizeof(char)*TAILLE_MAX);
		}

		free(nom2);
		fclose(c);
	}
	else{
		printf("Pas dans le fichier\n" );
	}
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
*\param perso perso sur lequel on souhaite effectuer une copie
*\return t_personnage : le nouveau personnage crée en copie
*/
t_personnage * copie_perso(t_personnage * perso){

		t_personnage * nv_perso = malloc(sizeof(t_personnage));
		nv_perso->nom = malloc(sizeof(char)* TAILLE_MAX);

		nv_perso->id = perso->id ;
		strcpy(nv_perso->nom,perso->nom);
		nv_perso->pv = perso->pv ;
		nv_perso->pv_max = perso->pv_max;
		nv_perso->pa_max = perso->pa_max ;
		nv_perso->pm = perso->pm ;
		nv_perso->sorts[0] = copie_sort(perso->sorts[0]);
		nv_perso->sorts[1] = copie_sort(perso->sorts[1]);
		nv_perso->sorts[2] = copie_sort(perso->sorts[2]);
		nv_perso->sorts[3] = copie_sort(perso->sorts[3]);
		nv_perso->effets[0] = perso->effets[0];
		nv_perso->effets[1] = perso->effets[1];

		return nv_perso;
}
