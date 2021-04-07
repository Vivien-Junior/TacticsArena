#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "fonc.h"
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <unistd.h>

/**
 *\file sorts.c
 *\brief programme d'initialisation du jeu
 *\author Dylan
 *\author Vivien-Junior
 *\author Alexandre
 *\version 0.1
 *\date 28/02/2019
*/

//tous les sorts
char buffer[BUFFER_LEN];
int server_socket;
t_equipe * tab_joueur[N];

/*************************************************************
*				   Sorts du guerrier	 					 *
*************************************************************/

/**
*\fn void saut (char map[N][N], t_personnage * perso1,t_equipe * equipe2, t_equipe * equipe1,int nump,int degat,int portee, int numj)
*\brief fonction saut
*\param map carte de jeu
*\param perso1 personnage actif pendant le tour jeu
*\param equipe1 equipe active durant le tour de jeu
*\param equipe2 equipe passif durant le tour de jeu
*\param nump  numéro du personnage %numj actuellement
*\param degat degat du sort
*\param portee portée du sort
*\param numj  numéro du equipe qui joue actuellement
*\param reseau sert à savoir si on est en reseau ou non
*\return int
*/

int saut (char map[N][N], t_personnage * perso1,t_equipe * equipe2, t_equipe * equipe1,int nump,int degat,int portee, int numj, int reseau){

 char point[N][N];/*matrice affichant les possibilités de jeu*/
 int i, j, g,dist = portee,car=0;
 char choix;
 t_coordonnees coor;

 coor = range(map,perso1,equipe2,equipe1,nump,degat,portee,numj, reseau);

 if(((abs(perso1->coord.x-coor.x)+abs(perso1->coord.y-coor.y))>portee|| (coor.x<0 || coor.x>=N) || (coor.y<0 || coor.y>=N)) || (map[coor.y][coor.x]!='.')){
    if(!reseau)
      printf("\nAction impossible, restitution des points d'action\n");
   return 0;
 }

 else {
    perso1->coord.x = coor.x;

    perso1->coord.y = coor.y;
 }
 if(!reseau){
   if(equipe1->numEquipe == 1){
     maj(map,equipe1,equipe2);
     affichage_map(map);
   }
   else {
     maj(map,equipe2,equipe1);
     affichage_map(map);
   }
 }
 return 1;
}


/**
*\fn void soin (char map[N][N], t_personnage * perso1,t_equipe * equipe2, t_equipe * equipe1,int nump,int degat,int portee, int numj)
*\brief Fonction qui regarde la vie au personnage perso1
*\param map carte de jeu
*\param perso1 personnage que l'on souhaite soigner
*\param equipe1 equipe active durant le tour de jeu
*\param equipe2 equipe passif durant le tour de jeu
*\param nump  numéro du personnage %numj actuellement
*\param degat degat du sort
*\param portee portée du sort
*\param numj  numéro du equipe qui joue actuellement
*\param reseau sert à savoir si on est en reseau ou non
*\return int
*/
int soin (char map[N][N], t_personnage * perso1,t_equipe * equipe2, t_equipe * equipe1,int nump,int degat,int portee, int numj, int reseau){
  int i;

  if(!reseau){
    if(equipe1->perso1->pv+degat <= equipe1->perso1->pv_max)
      equipe1->perso1->pv += degat;
    else
      equipe1->perso1->pv = equipe1->perso1->pv_max;

    if(equipe1->numEquipe == 1){
      maj(map,equipe1,equipe2);
      affichage_map(map);
    }
    else {
      maj(map,equipe2,equipe1);
      affichage_map(map);
    }

    printf("%s soigné.\n Il a maintenant : %i pv\n\n", equipe1->perso1->nom, equipe1->perso1->pv);
  }

  else{
    for(i=0;numj!=tab_joueur[i]->client_socket;i++);
    if(tab_joueur[i]->perso1->pv+degat <= tab_joueur[i]->perso1->pv_max)
      tab_joueur[i]->perso1->pv += degat;
    else
      tab_joueur[i]->perso1->pv = tab_joueur[i]->perso1->pv_max;
    envoie_map(map, tab_joueur, numj, 2);
    char test='0';
    //On verifie que la portee du sort pour savoir si il a besoin d'une cible */
    if(portee == 0)
      test='1';
    strcpy(buffer, &test);
    send(tab_joueur[i]->client_socket, buffer, BUFFER_LEN, 0);
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer,"%s soigné.\n Il a maintenant : %i pv\n", perso1->nom, perso1->pv);
    send(tab_joueur[i]->client_socket, buffer, strlen(buffer), 0);
  }

  return 1;
}


 /**
 *\fn void petit_coup (char map[N][N], t_personnage * perso1,t_equipe * equipe2, t_equipe * equipe1,int nump,int degat,int portee, int numj)
 *\brief Fonction qui effectue un sort d'un rayon "portee" autour du personnage
 *\param map carte de jeu
 *\param perso1 personnage qui attaque
 *\param equipe1 equipe active durant le tour de jeu
 *\param equipe2 equipe passif durant le tour de jeu
 *\param nump  numéro du personnage %numj actuellement
 *\param degat degat du sort
 *\param portee portée du sort
 *\param numj  numéro du equipe qui joue actuellement
 *\param reseau sert à savoir si on est en reseau ou non
 *\return int
 */
int petit_coup (char map[N][N], t_personnage * perso1,t_equipe * equipe2, t_equipe * equipe1,int nump,int degat,int portee, int numj, int reseau){

  if(portee_degat(map,perso1,equipe2,equipe1,nump,degat,portee,numj, reseau))
    return 1;

  return 0;

}



/**
*\fn void grosCoup (char map[N][N], t_personnage * perso1,t_equipe * equipe2, t_equipe * equipe1,int nump,int degat,int portee, int numj)
*\brief Fonction qui effectue un coup une case autour du personnage sur une distance "portee"
*\param map carte de jeu
*\param perso1 personnage qui attaque
*\param equipe1 equipe active durant le tour de jeu
*\param equipe2 equipe passif durant le tour de jeu
*\param nump  numéro du personnage %numj actuellement
*\param degat degat du sort
*\param portee portée du sort
*\param numj  numéro du equipe qui joue actuellement
*\param reseau sert à savoir si on est en reseau ou non
*\return int
*/
int grosCoup (char map[N][N], t_personnage * perso1,t_equipe * equipe2, t_equipe * equipe1,int nump,int degat,int portee, int numj, int reseau){

  if(portee_degat(map,perso1,equipe2,equipe1,nump,degat,portee,numj, reseau))
    return 1;

  return 0;

}



/*************************************************************
*				   Sorts de l'archer					 	*
*************************************************************/

 /**
 *\fn void diago (char map[N][N], t_personnage * perso1,t_equipe * equipe2, t_equipe * equipe1,int nump,int degat,int portee, int numj)
 *\brief Fonction qui effectue une coup en diagonale dans les 4 directions par rapport à la position actuel du perso1 sur une distance "portee"
 *\param map carte de jeu
 *\param perso1 personnage qui attaque
 *\param equipe1 equipe active durant le tour de jeu
 *\param equipe2 equipe passif durant le tour de jeu
 *\param nump  numéro du personnage %numj actuellement
 *\param degat degat du sort
 *\param portee portée du sort
 *\param numj  numéro du equipe qui joue actuellement
 *\param reseau sert à savoir si on est en reseau ou non
 *\return int
 */

int diago (char map[N][N], t_personnage * perso1,t_equipe * equipe2, t_equipe * equipe1,int nump,int degat,int portee, int numj, int reseau){

	char point[N][N];/*matrice affichant les possibilités de jeu*/
	int i, j;
	char choix;

  t_coordonnees coor;

  /*copie plan jeu dans la matrice point*/
	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++){
			point[i][j] = map[i][j];
		}
	}

	i = perso1->coord.y + 1;
	j = perso1->coord.x + 1;


	int nb = 0;
	int car = 0;
	//printf("%i\n", perso1.s1.portee);

	for(; (i < N && j < N) && nb < portee && point[i][j] == '.'; i++, j++, nb++, car++)
		point[i][j] = 'A' + car;

	i = perso1->coord.y - 1;
	j = perso1->coord.x - 1;
	nb = 0;

	for(; (i >= 0  && j >= 0) && nb < portee && point[i][j] == '.'; i--, j--, nb++, car++)
		point[i][j] = 'A' + car;

	i = perso1->coord.y - 1;
	j = perso1->coord.x + 1;
	nb = 0;

	for(; (i >= 0  && j < N) && nb < portee && point[i][j] == '.'; i--, j++, nb++, car++)
		point[i][j] = 'A' + car;

		i = perso1->coord.y + 1;
		j = perso1->coord.x - 1;
		nb = 0;

	for(; (i < N  && j >= 0) && nb < portee && point[i][j] == '.'; i++, j--, nb++, car++)
		point[i][j] = 'A' + car;

  int x = 0, y = 0;
  if(!reseau){
  	affichage_map(point);

  	do{
  			printf("Où souhaitez vous taper : ");
  			scanf(" %c", &choix);
  	}while(!existe(point, choix, &x, &y));
  }

  else{
    for(i=0;numj!=tab_joueur[i]->client_socket;i++);
    envoie_map(point, tab_joueur, numj, 2);
    char test='0';
    //On verifie que la portee du sort pour savoir si il a besoin d'une cible */
    if(portee == 0)
      test='1';
    strcpy(buffer, &test);
    send(tab_joueur[i]->client_socket, buffer, BUFFER_LEN, 0);
    memset(buffer, 0, sizeof(buffer));
    do{
  			sprintf(buffer,"Où souhaitez vous taper : \n");
        send(tab_joueur[i]->client_socket, buffer, strlen(buffer), 0);
        /* reception de la cible choisit par le joueur */
        memset(buffer, 0, sizeof(buffer));
        recv(tab_joueur[i]->client_socket,buffer,BUFFER_LEN, 0);
        strncpy(&choix,buffer+4,1);
  	}while(!existe(point, choix, &x, &y));
  }

  coor.x = x;
  coor.y = y;

  if(damage(map,perso1,equipe2,equipe1,nump,degat,portee,numj,coor, reseau))
    return 1;

  return 0;

}

  /**
  *\fn void ligne (char map[N][N], t_personnage * perso1,t_equipe * equipe2, t_equipe * equipe1,int nump,int degat,int portee, int numj)
  *\brief Fonction qui effectue une coup en ligne dans les 4 directions par rapport à la position actuel du perso1 sur une distance "portee"
  *\param map carte de jeu
  *\param perso1 personnage qui attaque
  *\param equipe1 equipe active durant le tour de jeu
  *\param equipe2 equipe passif durant le tour de jeu
  *\param nump  numéro du personnage %numj actuellement
  *\param degat degat du sort
  *\param portee portée du sort
  *\param numj  numéro du equipe qui joue actuellement
  *\param reseau sert à savoir si on est en reseau ou non
  *\return int
  */


int ligne (char map[N][N], t_personnage * perso1,t_equipe * equipe2, t_equipe * equipe1,int nump,int degat,int portee, int numj, int reseau){

	char point[N][N];/*matrice affichant les possibilités de jeu*/
	int i, j;
	char choix;

  t_coordonnees coor;

  /*copie plan jeu dans la matrice point*/

	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++){
			point[i][j] = map[i][j];
		}
	}

	i = perso1->coord.y + 1;
	j = perso1->coord.x ;


	int nb = 0;
	int car = 0;

	for(; i < N && nb < portee && point[i][j] == '.'; i++, nb++, car++)
		point[i][j] = 'A' + car;

	i = perso1->coord.y - 1;
	j = perso1->coord.x;
	nb = 0;

	for(; i >= 0  && nb < portee && point[i][j] == '.'; i--, nb++, car++)
		point[i][j] = 'A' + car;

	i = perso1->coord.y;
	j = perso1->coord.x + 1;
	nb = 0;

	for(; j < N && nb < portee && point[i][j] == '.'; j++, nb++, car++)
		point[i][j] = 'A' + car;

		i = perso1->coord.y;
		j = perso1->coord.x - 1;
		nb = 0;

	for(; j >= 0 && nb < portee && point[i][j] == '.'; j--, nb++, car++)
		point[i][j] = 'A' + car;

	int x = 0, y = 0;

  if(!reseau){
    affichage_map(point);
  	do{
  			printf("Où souhaitez vous taper : ");
  			scanf(" %c", &choix);
  	}while(!existe(point, choix, &x, &y));
  }
  else{
    for(i=0;numj!=tab_joueur[i]->client_socket;i++);
    envoie_map(point, tab_joueur, numj, 2);
    char test='0';
    //On verifie que la portee du sort pour savoir si il a besoin d'une cible */
    if(portee == 0)
      test='1';
    strcpy(buffer, &test);
    send(tab_joueur[i]->client_socket, buffer, BUFFER_LEN, 0);
    memset(buffer, 0, sizeof(buffer));
    do{
  			sprintf(buffer,"Où souhaitez vous taper : \n");
        send(tab_joueur[i]->client_socket, buffer, strlen(buffer), 0);
        /* reception de la cible choisit par le joueur */
        memset(buffer, 0, sizeof(buffer));
        recv(tab_joueur[i]->client_socket,buffer,BUFFER_LEN, 0);
        strncpy(&choix,buffer+4,1);
  	}while(!existe(point, choix, &x, &y));
  }
  coor.x = x;
  coor.y = y;

  if(damage(map,perso1,equipe2,equipe1,nump,degat,portee,numj,coor, reseau))
    return 1;

  return 0;

}

/**
*\fn void double_tape (char map[N][N], t_personnage * perso1,t_equipe * equipe2, t_equipe * equipe1,int nump,int degat,int portee, int numj)
*\brief Fonction qui effectue un sort d'un rayon "portee" autour du personnage
*\param map carte de jeu
*\param perso1 personnage qui attaque
*\param equipe1 equipe active durant le tour de jeu
*\param equipe2 equipe passif durant le tour de jeu
*\param nump  numéro du personnage %numj actuellement
*\param degat degat du sort
*\param portee portée du sort
*\param numj  numéro du equipe qui joue actuellement
*\param reseau sert à savoir si on est en reseau ou non
*\return int
*/

int double_tape (char map[N][N], t_personnage * perso1,t_equipe * equipe2, t_equipe * equipe1,int nump,int degat,int portee, int numj, int reseau){

	if(portee_degat(map,perso1,equipe2,equipe1,nump,degat,portee,numj, reseau))
    return 1;

  return 0;

}

/**
*\fn void coup_zone(char map[N][N], t_personnage * perso1,t_equipe * equipe2, t_equipe * equipe1,int nump,int degat,int portee, int numj)
*\brief Fonction qui effectue un sort d'un rayon "portee" autour du personnage sur une "portée" autour de la case ciblée
*\param map carte de jeu
*\param perso1 personnage qui attaque
*\param equipe1 equipe active durant le tour de jeu
*\param equipe2 equipe passif durant le tour de jeu
*\param nump  numéro du personnage %numj actuellement
*\param degat degat du sort
*\param portee portée du sort
*\param numj  numéro du equipe qui joue actuellement
*\param reseau sert à savoir si on est en reseau ou non
*\return int
*/

int coup_zone (char map[N][N], t_personnage * perso1,t_equipe * equipe2, t_equipe * equipe1,int nump,int degat,int portee, int numj, int reseau){

	char point[N][N];/*matrice affichant les possibilités de jeu*/
	int i, j, g, dist = portee,car=0;
	char choix;
  t_coordonnees coor;

  int nb_client;
  int cpt, cpt2;
  int l=1;

	/* recopie matrice dans la matrice point qui affiche la portee */

	coor = range(map,perso1,equipe2,equipe1,nump,degat,portee,numj, reseau);

  if(((abs(perso1->coord.x-coor.x)+abs(perso1->coord.y-coor.y))>portee)){
    if(!reseau)
      printf("\nAction impossible, restitution des points d'action\n");
    return 0;
  }

  else {
      //printf("x = %i y = %i\n", x, y);
      //vérifie si il y a des personnages dans la croix de largeur 'l' si oui leurs infliges les dégats

      if(reseau){

        for(cpt=0;numj!=tab_joueur[cpt]->client_socket;cpt++);

        /*Pour 2 joueurs on trouve son adversaire */
        if(tab_joueur[cpt]->numEquipe==1)
          for(cpt2=0;tab_joueur[cpt2]->numEquipe == tab_joueur[cpt]->numEquipe;cpt2++);
        if(tab_joueur[cpt]->numEquipe==2)
          for(cpt2=0;tab_joueur[cpt2]->numEquipe == tab_joueur[cpt]->numEquipe;cpt2++);

        if(tab_joueur[cpt]->perso2!=NULL){
          nb_client=2;
        }
        else
          nb_client=4;
      }

      //ligne horizontale
      for(i=coor.y,j=coor.x-l;j!=coor.x+(l+1);j++){
        fprintf(stderr,"case touché : %c",map[i][j]);
        if(numj==1 || (reseau && tab_joueur[cpt]->numEquipe==1)){
          switch (map[i][j]){

            case '2' :
              if (!(strcmp("armure",equipe2->perso1->effets[0].nom)))
                degat = (degat/2);
              if(!reseau){
                equipe2->perso1->pv -= degat;
                printf("%s touché.\nPoint de vie : %i\n", equipe2->perso1->nom, equipe2->perso1->pv);
              }
              else{
                tab_joueur[cpt2]->perso1->pv -= degat;
                sprintf(buffer,"%s touché.\nPoint de vie : %i\n", tab_joueur[cpt2]->perso1->nom, tab_joueur[cpt2]->perso1->pv);
                send_all_tour(tab_joueur, cpt, nb_client, 1);
              }
            break;

            case '4' :
                if (!(strcmp("armure",equipe2->perso2->effets[0].nom)))
                  degat = (degat/2);
                if(!reseau){
                  equipe2->perso2->pv -= degat;
                  printf("%s touché.\nPoint de vie : %i\n", equipe2->perso2->nom, equipe2->perso2->pv);
                }
                else{
                  tab_joueur[cpt2]->perso2->pv -= degat;
                  sprintf(buffer,"%s touché.\nPoint de vie : %i\n", tab_joueur[cpt2]->perso2->nom, tab_joueur[cpt2]->perso2->pv);
                  send_all_tour(tab_joueur, cpt, nb_client, 1);
                }
            break;
          }
        }

        if(numj==2 || (reseau && tab_joueur[cpt]->numEquipe==2)){
          switch (map[i][j]){

            case '1' :
              if (!(strcmp("armure",equipe2->perso1->effets[0].nom)))
                degat = (degat/2);
              if(!reseau){
                equipe2->perso1->pv -= degat;
                printf("%s touché.\nPoint de vie : %i\n", equipe2->perso1->nom, equipe2->perso1->pv);
              }
              else{
                tab_joueur[cpt2]->perso1->pv -= degat;
                sprintf(buffer,"%s touché.\nPoint de vie : %i\n", tab_joueur[cpt2]->perso1->nom, tab_joueur[cpt2]->perso1->pv);
                send_all_tour(tab_joueur, cpt, nb_client, 1);
              }
            break;

            case '3' :
              if (!(strcmp("armure",equipe2->perso2->effets[0].nom)))
                degat = (degat/2);
              if(!reseau){
                equipe2->perso2->pv -= degat;
                printf("%s touché.\nPoint de vie : %i\n", equipe2->perso2->nom, equipe2->perso2->pv);
              }
              else{
                tab_joueur[cpt2]->perso2->pv -= degat;
                sprintf(buffer,"%s touché.\nPoint de vie : %i\n", tab_joueur[cpt2]->perso2->nom, tab_joueur[cpt2]->perso2->pv);
                send_all_tour(tab_joueur, cpt, nb_client, 1);
              }
            break;
          }
        }
      }

            //ligne verticale
      for(i=coor.y-l,j=coor.x;i!=coor.y+(l+1);i++){
        fprintf(stderr,"case touché : %c",map[i][j]);
        if(i!=coor.y){
          if(numj==1){
            switch (map[i][j]){

              case '2' :
                if (!(strcmp("armure",equipe2->perso1->effets[0].nom)))
                  degat = (degat/2);
                if(!reseau){
                  equipe2->perso1->pv -= degat;
                  printf("%s touché.\nPoint de vie : %i\n", equipe2->perso1->nom, equipe2->perso1->pv);
                }
                else{
                  tab_joueur[cpt2]->perso1->pv -= degat;
                  sprintf(buffer,"%s touché.\nPoint de vie : %i\n", tab_joueur[cpt2]->perso1->nom, tab_joueur[cpt2]->perso1->pv);
                  send_all_tour(tab_joueur, cpt, nb_client, 1);
                }
              break;

              case '4' :
                  if (!(strcmp("armure",equipe2->perso2->effets[0].nom)))
                    degat = (degat/2);
                  if(!reseau){
                    equipe2->perso2->pv -= degat;
                    printf("%s touché.\nPoint de vie : %i\n", equipe2->perso2->nom, equipe2->perso2->pv);
                  }
                  else{
                    tab_joueur[cpt2]->perso2->pv -= degat;
                    sprintf(buffer,"%s touché.\nPoint de vie : %i\n", tab_joueur[cpt2]->perso2->nom, tab_joueur[cpt2]->perso2->pv);
                    send_all_tour(tab_joueur, cpt, nb_client, 1);
                  }
              break;
            }
          }

          if(numj==2){
            switch (map[i][j]){

              case '1' :
                if (!(strcmp("armure",equipe2->perso1->effets[0].nom)))
                  degat = (degat/2);
                if(!reseau){
                  equipe2->perso1->pv -= degat;
                  printf("%s touché.\nPoint de vie : %i\n", equipe2->perso1->nom, equipe2->perso1->pv);
                }
                else{
                  tab_joueur[cpt2]->perso1->pv -= degat;
                  sprintf(buffer,"%s touché.\nPoint de vie : %i\n", tab_joueur[cpt2]->perso1->nom, tab_joueur[cpt2]->perso1->pv);
                  send_all_tour(tab_joueur, cpt, nb_client, 1);
                }
              break;

              case '3' :
                if(!(strcmp("armure",equipe2->perso2->effets[0].nom)))
                  degat = (degat/2);
                if(!reseau){
                  equipe2->perso2->pv -= degat;
                  printf("%s touché.\nPoint de vie : %i\n", equipe2->perso2->nom, equipe2->perso2->pv);
                }
                else{
                  tab_joueur[cpt2]->perso2->pv -= degat;
                  sprintf(buffer,"%s touché.\nPoint de vie : %i\n", tab_joueur[cpt2]->perso2->nom, tab_joueur[cpt2]->perso2->pv);
                  send_all_tour(tab_joueur, cpt, nb_client, 1);
                }
                break;
            }
          }
        }
      }//fin for
  }//fin else

  if(!reseau){
    if(equipe1->numEquipe == 1){
      maj(map,equipe1,equipe2);
      affichage_map(map);
    }
    else {
      maj(map,equipe2,equipe1);
      affichage_map(map);
    }
  }
  else{
    if(tab_joueur[cpt]->numEquipe == 1){
      maj( map, tab_joueur[0], tab_joueur[1]);
      envoie_map(map, tab_joueur, numj, 2);
    }
    else{
      maj( map, tab_joueur[1], tab_joueur[0]);
      envoie_map(map, tab_joueur, numj, 2);
    }
  }

  return 1;
}

/*************************************************************
*				   Sorts du Tank	 					 	 *
*************************************************************/

/**
*\fn void armure(char map[N][N], t_personnage * perso1,t_equipe * equipe2, t_equipe * equipe1,int nump,int degat,int portee, int numj)
*\brief Fonction effectue l'effet armure sur le personnage "perso1"
*\param map carte de jeu
*\param perso1 personnage qui attaque
*\param equipe1 equipe active durant le tour de jeu
*\param equipe2 equipe passif durant le tour de jeu
*\param nump  numéro du personnage %numj actuellement
*\param degat degat du sort
*\param portee portée du sort
*\param numj  numéro du equipe qui joue actuellement
*\param reseau sert à savoir si on est en reseau ou non
*\return int
*/

int armure (char map[N][N], t_personnage * perso1,t_equipe * equipe2, t_equipe * equipe1,int nump,int degat,int portee, int numj, int reseau){
  int i;
  int nb_client;

  creer_effet(perso1,1,perso1->coord.x,perso1->coord.y);

  if(!reseau){
    if(equipe1->numEquipe == 1){
      maj(map,equipe1,equipe2);
      affichage_map(map);
    }
    else {
      maj(map,equipe2,equipe1);
      affichage_map(map);
    }

    printf("Vous prendrez la moitié des dégats jusqu'a votre prochain tour\n\n");
  }

  else{
    for(i=0;numj!=tab_joueur[i]->client_socket;i++);

    if(tab_joueur[i]->perso2!=NULL){
      nb_client=2;
    }
    else
      nb_client=4;

    if(tab_joueur[i]->numEquipe == 1){
      maj( map, tab_joueur[0], tab_joueur[1]);
      envoie_map(map, tab_joueur, numj, 2);
    }
    else{
      maj( map, tab_joueur[1], tab_joueur[0]);
      envoie_map(map, tab_joueur, numj, 2);
    }
    char test='0';
    //On verifie que la portee du sort pour savoir si il a besoin d'une cible */
    if(portee == 0)
      test='1';
    strcpy(buffer, &test);
    send(tab_joueur[i]->client_socket, buffer, BUFFER_LEN, 0);
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer,"Vous prendrez la moitié des dégats jusqu'a votre prochain tour\n\n");
    send_all_tour(tab_joueur, i, nb_client, 1);
  }

  return 1;
}

/**
*\fn void attire(char map[N][N], t_personnage * perso1,t_equipe * equipe2, t_equipe * equipe1,int nump,int degat,int portee, int numj)
*\brief sort qui attire le personnage ciblée de N cases jusqu'au personnage qui effectue l'action
*\param map carte de jeu
*\param perso1 personnage qui attaque
*\param equipe1 equipe active durant le tour de jeu
*\param equipe2 equipe passif durant le tour de jeu
*\param nump  numéro du personnage %numj actuellement
*\param degat degat du sort
*\param portee portée du sort
*\param numj  numéro du equipe qui joue actuellement
*\param reseau sert à savoir si on est en reseau ou non
*\return int
*/

int attire (char map[N][N], t_personnage * perso1,t_equipe * equipe2, t_equipe * equipe1,int nump,int degat,int portee, int numj, int reseau){

  char point[N][N];/*matrice affichant les possibilités de jeu*/
	int i, j, cpt2;
	char choix;
    /*copie plan jeu dans la matrice point*/

	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++){
			point[i][j] = map[i][j];
		}
	}

	i = perso1->coord.y + 1;
	j = perso1->coord.x ;


	int nb = 0;
	int car = 0;

	for(; i < N && nb < portee && point[i][j] == '.'; i++, nb++, car++)
		point[i][j] = 'A' + car;

	i = perso1->coord.y - 1;
	j = perso1->coord.x;
	nb = 0;

	for(; i >= 0  && nb < portee && point[i][j] == '.'; i--, nb++, car++)
		point[i][j] = 'A' + car;

	i = perso1->coord.y;
	j = perso1->coord.x + 1;
	nb = 0;

	for(; j < N && nb < portee && point[i][j] == '.'; j++, nb++, car++)
		point[i][j] = 'A' + car;

		i = perso1->coord.y;
		j = perso1->coord.x - 1;
		nb = 0;

	for(; j >= 0 && nb < portee && point[i][j] == '.'; j--, nb++, car++)
		point[i][j] = 'A' + car;

  int x = 0, y = 0;

  if(!reseau){
  	affichage_map(point);
  	do{
  			printf("Où souhaitez vous taper : ");
  			scanf(" %c", &choix);
  	}while(!existe(point, choix, &x, &y));
  }

  else{
    for(i=0;numj!=tab_joueur[i]->client_socket;i++);

    /*Pour 2 joueurs on trouve son adversaire */
    if(tab_joueur[i]->numEquipe==1)
      for(cpt2=0;tab_joueur[cpt2]->numEquipe == tab_joueur[i]->numEquipe;cpt2++);
    if(tab_joueur[i]->numEquipe==2)
      for(cpt2=0;tab_joueur[cpt2]->numEquipe == tab_joueur[i]->numEquipe;cpt2++);

    envoie_map(point, tab_joueur, numj, 2);
    char test='0';
    //On verifie que la portee du sort pour savoir si il a besoin d'une cible */
    if(portee == 0)
      test='1';
    strcpy(buffer, &test);
    send(tab_joueur[i]->client_socket, buffer, BUFFER_LEN, 0);
    memset(buffer, 0, sizeof(buffer));
    do{
  			sprintf(buffer,"Où souhaitez vous taper : \n");
        send(tab_joueur[i]->client_socket, buffer, strlen(buffer), 0);
        /* reception de la cible choisit par le joueur */
        memset(buffer, 0, sizeof(buffer));
        recv(tab_joueur[i]->client_socket,buffer,BUFFER_LEN, 0);
        strncpy(&choix,buffer+4,1);
  	}while(!existe(point, choix, &x, &y));
  }

  if(((abs(perso1->coord.x-x)+abs(perso1->coord.y-y))>portee)){
    if(!reseau)
      printf("\nAction impossible, restitution des points d'action\n");
    return 0;
  }

  else {
  		if(numj==1 || (reseau && tab_joueur[i]->numEquipe==1)){
  			switch (map[y][x]){

  				case '2' :

            if(!reseau){
              /* Si l'écart entre les 2 equipes est de plus de 3 cases */
              if(abs(perso1->coord.x - equipe2->perso1->coord.x > 3)){
                if(perso1->coord.x < equipe2->perso1->coord.x )
                    equipe2->perso1->coord.x -= 3;
                else if(perso1->coord.x > equipe2->perso1->coord.x )
                    equipe2->perso1->coord.x += 3;
              }

              /* Sinon l'attire a son corps à corps du nb de cases d'écart*/
              else{
                if(perso1->coord.x < equipe2->perso1->coord.x )
                  equipe2->perso1->coord.x = perso1->coord.x + 1;
                else if(perso1->coord.x > equipe2->perso1->coord.x )
                  equipe2->perso1->coord.x = perso1->coord.x - 1;
              }


        			/* Si l'écart entre les 2 equipes est de plus de 3 cases */
              if(abs(perso1->coord.y - equipe2->perso1->coord.y > 3)){
                if(perso1->coord.y < equipe2->perso1->coord.y )
                    equipe2->perso1->coord.y -= 3;
                else if(perso1->coord.y > equipe2->perso1->coord.y )
                    equipe2->perso1->coord.y += 3;
              }
              /* Sinon l'attire a son corps à corps du nb de cases d'écart*/
              else{
                if(perso1->coord.y < equipe2->perso1->coord.y )
                  equipe2->perso1->coord.y = perso1->coord.y + 1;
                else if(perso1->coord.y > equipe2->perso1->coord.y )
                  equipe2->perso1->coord.y = perso1->coord.y - 1;
              }
            }

            else{
              /* Si l'écart entre les 2 equipes est de plus de 3 cases */
              if(abs(perso1->coord.x - tab_joueur[cpt2]->perso1->coord.x > 3)){
                if(perso1->coord.x < tab_joueur[cpt2]->perso1->coord.x )
                    tab_joueur[cpt2]->perso1->coord.x -= 3;
                else if(perso1->coord.x > tab_joueur[cpt2]->perso1->coord.x )
                    tab_joueur[cpt2]->perso1->coord.x += 3;
              }

              /* Sinon l'attire a son corps à corps du nb de cases d'écart*/
              else{
                if(perso1->coord.x < tab_joueur[cpt2]->perso1->coord.x )
                  tab_joueur[cpt2]->perso1->coord.x = perso1->coord.x + 1;
                else if(perso1->coord.x > tab_joueur[cpt2]->perso1->coord.x )
                  tab_joueur[cpt2]->perso1->coord.x = perso1->coord.x - 1;
              }


              /* Si l'écart entre les 2 equipes est de plus de 3 cases */
              if(abs(perso1->coord.y - tab_joueur[cpt2]->perso1->coord.y > 3)){
                if(perso1->coord.y < tab_joueur[cpt2]->perso1->coord.y )
                    tab_joueur[cpt2]->perso1->coord.y -= 3;
                else if(perso1->coord.y > tab_joueur[cpt2]->perso1->coord.y )
                    tab_joueur[cpt2]->perso1->coord.y += 3;
              }
              /* Sinon l'attire a son corps à corps du nb de cases d'écart*/
              else{
                if(perso1->coord.y < tab_joueur[cpt2]->perso1->coord.y )
                  tab_joueur[cpt2]->perso1->coord.y = perso1->coord.y + 1;
                else if(perso1->coord.y > tab_joueur[cpt2]->perso1->coord.y )
                  tab_joueur[cpt2]->perso1->coord.y = perso1->coord.y - 1;
              }
            }
  					break;

  			 	case '4' :

            if(!reseau){
              /* Si l'écart entre les 2 equipes est de plus de 3 cases */
              if(abs(perso1->coord.x - equipe2->perso2->coord.x > 3)){
                if(perso1->coord.x < equipe2->perso2->coord.x )
                    equipe2->perso2->coord.x -= 3;
                else if(perso1->coord.x > equipe2->perso2->coord.x )
                    equipe2->perso2->coord.x += 3;
              }
              /* Sinon l'attire a son corps à corps du nb de cases d'écart*/
              else{
                if(perso1->coord.x < equipe2->perso2->coord.x )
                  equipe2->perso2->coord.x = perso1->coord.x + 1;
                else if(perso1->coord.x > equipe2->perso2->coord.x )
                  equipe2->perso2->coord.x = perso1->coord.x - 1;
              }


              /* Si l'écart entre les 2 equipes est de plus de 3 cases */
              if(abs(perso1->coord.y - equipe2->perso2->coord.y > 3)){
                if(perso1->coord.y < equipe2->perso2->coord.y )
                    equipe2->perso2->coord.y -= 3;
                else if(perso1->coord.y > equipe2->perso2->coord.y )
                    equipe2->perso2->coord.y += 3;
              }
              /* Sinon l'attire a son corps à corps du nb de cases d'écart*/
              else{
                if(perso1->coord.y < equipe2->perso2->coord.y )
                  equipe2->perso2->coord.y = perso1->coord.y + 1;
                else if(perso1->coord.y > equipe2->perso2->coord.y )
                  equipe2->perso2->coord.y = perso1->coord.y - 1;
              }
            }

            else{
              /* Si l'écart entre les 2 equipes est de plus de 3 cases */
              if(abs(perso1->coord.x - tab_joueur[cpt2]->perso2->coord.x > 3)){
                if(perso1->coord.x < tab_joueur[cpt2]->perso2->coord.x )
                    tab_joueur[cpt2]->perso2->coord.x -= 3;
                else if(perso1->coord.x > tab_joueur[cpt2]->perso2->coord.x )
                    tab_joueur[cpt2]->perso2->coord.x += 3;
              }
              /* Sinon l'attire a son corps à corps du nb de cases d'écart*/
              else{
                if(perso1->coord.x < tab_joueur[cpt2]->perso2->coord.x )
                  tab_joueur[cpt2]->perso2->coord.x = perso1->coord.x + 1;
                else if(perso1->coord.x > tab_joueur[cpt2]->perso2->coord.x )
                  tab_joueur[cpt2]->perso2->coord.x = perso1->coord.x - 1;
              }


              /* Si l'écart entre les 2 equipes est de plus de 3 cases */
              if(abs(perso1->coord.y - tab_joueur[cpt2]->perso2->coord.y > 3)){
                if(perso1->coord.y < tab_joueur[cpt2]->perso2->coord.y )
                    tab_joueur[cpt2]->perso2->coord.y -= 3;
                else if(perso1->coord.y > tab_joueur[cpt2]->perso2->coord.y )
                    tab_joueur[cpt2]->perso2->coord.y += 3;
              }
              /* Sinon l'attire a son corps à corps du nb de cases d'écart*/
              else{
                if(perso1->coord.y < tab_joueur[cpt2]->perso2->coord.y )
                  tab_joueur[cpt2]->perso2->coord.y = perso1->coord.y + 1;
                else if(perso1->coord.y > tab_joueur[cpt2]->perso2->coord.y )
                  tab_joueur[cpt2]->perso2->coord.y = perso1->coord.y - 1;
              }
            }
          break;
  			}//fin switch
  	  }//fin numj==1

  		if(numj==2 || (reseau && tab_joueur[i]->numEquipe==2)){
  			switch (map[y][x]){

          case '1' :
            if(!reseau){
              /* Si l'écart entre les 2 equipes est de plus de 3 cases */
              if(abs(perso1->coord.x - equipe2->perso1->coord.x > 3)){
                if(perso1->coord.x < equipe2->perso1->coord.x )
                    equipe2->perso1->coord.x -= 3;
                else if(perso1->coord.x > equipe2->perso1->coord.x )
                    equipe2->perso1->coord.x += 3;
              }

              /* Sinon l'attire a son corps à corps du nb de cases d'écart*/
              else{
                if(perso1->coord.x < equipe2->perso1->coord.x )
                  equipe2->perso1->coord.x = perso1->coord.x + 1;
                else if(perso1->coord.x > equipe2->perso1->coord.x )
                  equipe2->perso1->coord.x = perso1->coord.x - 1;
              }


              /* Si l'écart entre les 2 equipes est de plus de 3 cases */
              if(abs(perso1->coord.y - equipe2->perso1->coord.y > 3)){
                if(perso1->coord.y < equipe2->perso1->coord.y )
                    equipe2->perso1->coord.y -= 3;
                else if(perso1->coord.y > equipe2->perso1->coord.y )
                    equipe2->perso1->coord.y += 3;
              }
              /* Sinon l'attire a son corps à corps du nb de cases d'écart*/
              else{
                if(perso1->coord.y < equipe2->perso1->coord.y )
                  equipe2->perso1->coord.y = perso1->coord.y + 1;
                else if(perso1->coord.y > equipe2->perso1->coord.y )
                  equipe2->perso1->coord.y = perso1->coord.y - 1;
              }
            }

            else{
              /* Si l'écart entre les 2 equipes est de plus de 3 cases */
              if(abs(perso1->coord.x - tab_joueur[cpt2]->perso1->coord.x > 3)){
                if(perso1->coord.x < tab_joueur[cpt2]->perso1->coord.x )
                    tab_joueur[cpt2]->perso1->coord.x -= 3;
                else if(perso1->coord.x > tab_joueur[cpt2]->perso1->coord.x )
                    tab_joueur[cpt2]->perso1->coord.x += 3;
              }

              /* Sinon l'attire a son corps à corps du nb de cases d'écart*/
              else{
                if(perso1->coord.x < tab_joueur[cpt2]->perso1->coord.x )
                  tab_joueur[cpt2]->perso1->coord.x = perso1->coord.x + 1;
                else if(perso1->coord.x > tab_joueur[cpt2]->perso1->coord.x )
                  tab_joueur[cpt2]->perso1->coord.x = perso1->coord.x - 1;
              }


              /* Si l'écart entre les 2 equipes est de plus de 3 cases */
              if(abs(perso1->coord.y - tab_joueur[cpt2]->perso1->coord.y > 3)){
                if(perso1->coord.y < tab_joueur[cpt2]->perso1->coord.y )
                    tab_joueur[cpt2]->perso1->coord.y -= 3;
                else if(perso1->coord.y > tab_joueur[cpt2]->perso1->coord.y )
                    tab_joueur[cpt2]->perso1->coord.y += 3;
              }
              /* Sinon l'attire a son corps à corps du nb de cases d'écart*/
              else{
                if(perso1->coord.y < tab_joueur[cpt2]->perso1->coord.y )
                  tab_joueur[cpt2]->perso1->coord.y = perso1->coord.y + 1;
                else if(perso1->coord.y > tab_joueur[cpt2]->perso1->coord.y )
                  tab_joueur[cpt2]->perso1->coord.y = perso1->coord.y - 1;
              }
            }

        break;

  			 	case '3' :
            if(!reseau){
              /* Si l'écart entre les 2 equipes est de plus de 3 cases */
                if(abs(perso1->coord.x - equipe2->perso2->coord.x > 3)){
                  if(perso1->coord.x < equipe2->perso2->coord.x )
                      equipe2->perso2->coord.x -= 3;
                  else if(perso1->coord.x > equipe2->perso2->coord.x )
                      equipe2->perso2->coord.x += 3;
                }
                /* Sinon l'attire a son corps à corps du nb de cases d'écart*/
                else{
                  if(perso1->coord.x < equipe2->perso2->coord.x )
                    equipe2->perso2->coord.x = perso1->coord.x + 1;
                  else if(perso1->coord.x > equipe2->perso2->coord.x )
                    equipe2->perso2->coord.x = perso1->coord.x - 1;
                }


                /* Si l'écart entre les 2 equipes est de plus de 3 cases */
                if(abs(perso1->coord.y - equipe2->perso2->coord.y > 3)){
                  if(perso1->coord.y < equipe2->perso2->coord.y )
                      equipe2->perso2->coord.y -= 3;
                  else if(perso1->coord.y > equipe2->perso2->coord.y )
                      equipe2->perso2->coord.y += 3;
                }
                /* Sinon l'attire a son corps à corps du nb de cases d'écart*/
                else{
                  if(perso1->coord.y < equipe2->perso2->coord.y )
                    equipe2->perso2->coord.y = perso1->coord.y + 1;
                  else if(perso1->coord.y > equipe2->perso2->coord.y )
                    equipe2->perso2->coord.y = perso1->coord.y - 1;
                }
            }

            else{
              /* Si l'écart entre les 2 equipes est de plus de 3 cases */
                if(abs(perso1->coord.x - tab_joueur[cpt2]->perso2->coord.x > 3)){
                  if(perso1->coord.x < tab_joueur[cpt2]->perso2->coord.x )
                      tab_joueur[cpt2]->perso2->coord.x -= 3;
                  else if(perso1->coord.x > tab_joueur[cpt2]->perso2->coord.x )
                      tab_joueur[cpt2]->perso2->coord.x += 3;
                }
                /* Sinon l'attire a son corps à corps du nb de cases d'écart*/
                else{
                  if(perso1->coord.x < tab_joueur[cpt2]->perso2->coord.x )
                    tab_joueur[cpt2]->perso2->coord.x = perso1->coord.x + 1;
                  else if(perso1->coord.x > tab_joueur[cpt2]->perso2->coord.x )
                    tab_joueur[cpt2]->perso2->coord.x = perso1->coord.x - 1;
                }


                /* Si l'écart entre les 2 equipes est de plus de 3 cases */
                if(abs(perso1->coord.y - tab_joueur[cpt2]->perso2->coord.y > 3)){
                  if(perso1->coord.y < tab_joueur[cpt2]->perso2->coord.y )
                      tab_joueur[cpt2]->perso2->coord.y -= 3;
                  else if(perso1->coord.y > tab_joueur[cpt2]->perso2->coord.y )
                      tab_joueur[cpt2]->perso2->coord.y += 3;
                }
                /* Sinon l'attire a son corps à corps du nb de cases d'écart*/
                else{
                  if(perso1->coord.y < tab_joueur[cpt2]->perso2->coord.y )
                    tab_joueur[cpt2]->perso2->coord.y = perso1->coord.y + 1;
                  else if(perso1->coord.y > tab_joueur[cpt2]->perso2->coord.y )
                    tab_joueur[cpt2]->perso2->coord.y = perso1->coord.y - 1;
                }
            }
          break;
        }//fin switch
      }//fin numj==1
  }
  if(!reseau){
    if(equipe1->numEquipe == 1){
      maj(map,equipe1,equipe2);
      affichage_map(map);
    }
    else {
      maj(map,equipe2,equipe1);
      affichage_map(map);
    }
  }
  else{
    if(tab_joueur[i]->numEquipe == 1){
      maj( map, tab_joueur[0], tab_joueur[1]);
      envoie_map(map, tab_joueur, numj, 2);
    }
    else{
      maj( map, tab_joueur[1], tab_joueur[0]);
      envoie_map(map, tab_joueur, numj, 2);
    }
  }
  return 1;
}

/**
*\fn void chenchen(char map[N][N], t_personnage * perso1,t_equipe * equipe2, t_equipe * equipe1,int nump,int degat,int portee, int numj)
*\brief sort
*\param map carte de jeu
*\param perso1 personnage qui attaque
*\param equipe1 equipe active durant le tour de jeu
*\param equipe2 equipe passif durant le tour de jeu
*\param nump  numéro du personnage %numj actuellement
*\param degat degat du sort
*\param portee portée du sort
*\param numj  numéro du equipe qui joue actuellement
*\param reseau sert à savoir si on est en reseau ou non
*\return int
*/

int chenchen (char map[N][N], t_personnage * perso1,t_equipe * equipe2, t_equipe * equipe1,int nump,int degat,int portee, int numj, int reseau){

  if(portee_degat(map,perso1,equipe2,equipe1,nump,degat,portee,numj, reseau))
    return 1;

  return 0;

}

/**
*\fn void bigshaq(char map[N][N], t_personnage * perso1,t_equipe * equipe2, t_equipe * equipe1,int nump,int degat,int portee, int numj)
*\brief sort
*\param map carte de jeu
*\param perso1 personnage qui attaque
*\param equipe1 equipe active durant le tour de jeu
*\param equipe2 equipe passif durant le tour de jeu
*\param nump  numéro du personnage %numj actuellement
*\param degat degat du sort
*\param portee portée du sort
*\param numj  numéro du equipe qui joue actuellement
*\param reseau sert à savoir si on est en reseau ou non
*\return int
*/

int bigshaq (char map[N][N], t_personnage * perso1,t_equipe * equipe2, t_equipe * equipe1,int nump,int degat,int portee, int numj, int reseau){

	char point[N][N];/*matrice affichant les possibilités de jeu*/
	int i, j;
	char choix;
	int degats[4]={8,16,32,128};
  int nb_client;
  int cpt, cpt2;

  t_coordonnees coor;

	coor = range(map,perso1,equipe2,equipe1,nump,degat,portee,numj, reseau);

  if(((abs(perso1->coord.x-coor.x)+abs(perso1->coord.y-coor.y))>portee)){
    if(!reseau)
      printf("\nAction impossible, restitution des points d'action\n");
    return 0;
  }

  else {

    if(reseau){

      for(cpt=0;numj!=tab_joueur[cpt]->client_socket;cpt++);

      /*Pour 2 joueurs on trouve son adversaire */
      if(tab_joueur[cpt]->numEquipe==1)
        for(cpt2=0;tab_joueur[cpt2]->numEquipe == tab_joueur[cpt]->numEquipe;cpt2++);
      if(tab_joueur[cpt]->numEquipe==2)
        for(cpt2=0;tab_joueur[cpt2]->numEquipe == tab_joueur[cpt]->numEquipe;cpt2++);

      if(tab_joueur[cpt]->perso2!=NULL){
        nb_client=2;
      }
      else
        nb_client=4;
    }

    /* variation des dégats suivant les points de vie du tank */
    if(perso1->pv > 50)
      degat = degats[0];
    else if(perso1->pv > 25)
      degat = degats[1];
    else if(perso1->pv > 2)
      degat = degats[3];
    else if(perso1->pv == 1)
      degat = degats[4];

    //réduction des points de vies après le coup
    if(numj==1 || (reseau && tab_joueur[cpt]->numEquipe==1)){
      switch (map[coor.y][coor.x]){

        case '2' :
          if (!(strcmp("armure",equipe2->perso1->effets[0].nom)))
            degat = (degat/2);
          if(!reseau){
            equipe2->perso1->pv -= degat;
            printf("%s touché.\nPoint de vie : %i\n", equipe2->perso1->nom, equipe2->perso1->pv);
          }
          else{
            tab_joueur[cpt2]->perso1->pv -= degat;
            sprintf(buffer,"%s touché.\nPoint de vie : %i\n", tab_joueur[cpt2]->perso1->nom, tab_joueur[cpt2]->perso1->pv);
            send_all_tour(tab_joueur, cpt, nb_client, 1);
          }
        break;

        case '4' :
          if (!(strcmp("armure",equipe2->perso2->effets[0].nom)))
            degat = (degat/2);
          if(!reseau){
            equipe2->perso2->pv -= degat;
            printf("%s touché.\nPoint de vie : %i\n", equipe2->perso2->nom, equipe2->perso2->pv);
          }
          else{
            tab_joueur[cpt2]->perso2->pv -= degat;
            sprintf(buffer,"%s touché.\nPoint de vie : %i\n", tab_joueur[cpt2]->perso2->nom, tab_joueur[cpt2]->perso2->pv);
            send_all_tour(tab_joueur, cpt, nb_client, 1);
          }
        break;
      }
    }

    if(numj==2 || (reseau && tab_joueur[cpt]->numEquipe==2)){
      switch (map[coor.y][coor.x]){

        case '1' :
          if (!(strcmp("armure",equipe2->perso1->effets[0].nom)))
            degat = (degat/2);
          if(!reseau){
            equipe2->perso1->pv -= degat;
            printf("%s touché.\nPoint de vie : %i\n", equipe2->perso1->nom, equipe2->perso1->pv);
          }
          else{
            tab_joueur[cpt2]->perso1->pv -= degat;
            sprintf(buffer,"%s touché.\nPoint de vie : %i\n", tab_joueur[cpt2]->perso1->nom, tab_joueur[cpt2]->perso1->pv);
            send_all_tour(tab_joueur, cpt, nb_client, 1);
          }
        break;

        case '3' :
          if (!(strcmp("armure",equipe2->perso2->effets[0].nom)))
            degat = (degat/2);
          if(!reseau){
            equipe2->perso2->pv -= degat;
            printf("%s touché.\nPoint de vie : %i\n", equipe2->perso2->nom, equipe2->perso2->pv);
          }
          else{
            tab_joueur[cpt2]->perso2->pv -= degat;
            sprintf(buffer,"%s touché.\nPoint de vie : %i\n", tab_joueur[cpt2]->perso2->nom, tab_joueur[cpt2]->perso2->pv);
            send_all_tour(tab_joueur, cpt, nb_client, 1);
          }
        break;
      }
    }
  }

  if(!reseau){
    if(equipe1->numEquipe == 1){
      maj(map,equipe1,equipe2);
      affichage_map(map);
    }
    else {
      maj(map,equipe2,equipe1);
      affichage_map(map);
    }
  }
  return 1;
}


/*************************************************************
*				   Sorts du Druide	 					 	 *
*************************************************************/


/**
*\fn void felin(char map[N][N], t_personnage * perso1,t_equipe * equipe2, t_equipe * equipe1,int nump,int degat,int portee, int numj)
*\brief sort effectue l'effet felin sur le personnage "perso1"
*\param map carte de jeu
*\param perso1 personnage qui attaque
*\param equipe1 equipe active durant le tour de jeu
*\param equipe2 equipe passif durant le tour de jeu
*\param nump  numéro du personnage %numj actuellement
*\param degat degat du sort
*\param portee portée du sort
*\param numj  numéro du equipe qui joue actuellement
*\param reseau sert à savoir si on est en reseau ou non
*\return int
*/

int minotaure (char map[N][N], t_personnage * perso1,t_equipe * equipe2, t_equipe * equipe1,int nump,int degat,int portee, int numj, int reseau){
  int i;
  int nb_client;
  creer_effet(perso1,2,perso1->coord.x,perso1->coord.y);

  if(!reseau){
    if(equipe1->numEquipe == 1){
      maj(map,equipe1,equipe2);
      affichage_map(map);
    }
    else {
      maj(map,equipe2,equipe1);
      affichage_map(map);
    }
    printf("Vous vous transformez en minotaure et aurez désormais 3 points d'actions (une seule transformation à la fois)\n\n");
  }

  else{
    for(i=0;numj!=tab_joueur[i]->client_socket;i++);

    if(tab_joueur[i]->perso2!=NULL){
      nb_client=2;
    }
    else
      nb_client=4;

    if(tab_joueur[i]->numEquipe == 1){
      maj( map, tab_joueur[0], tab_joueur[1]);
      envoie_map(map, tab_joueur, numj, 2);
    }
    else{
      maj( map, tab_joueur[1], tab_joueur[0]);
      envoie_map(map, tab_joueur, numj, 2);
    }
    char test='0';
    //On verifie que la portee du sort pour savoir si il a besoin d'une cible */
    if(portee == 0)
      test='1';
    strcpy(buffer, &test);
    send(tab_joueur[i]->client_socket, buffer, BUFFER_LEN, 0);
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer,"Vous vous transformez en minotaure et aurez désormais 3 points d'actions (une seule transformation à la fois)\n\n");
    send_all_tour(tab_joueur, i, nb_client, 1);
  }
  return 1;

}

/**
*\fn void chouette(char map[N][N], t_personnage * perso1,t_equipe * equipe2, t_equipe * equipe1,int nump,int degat,int portee, int numj)
*\brief sort effectue l'effet chouette sur le personnage "perso1"
*\param map carte de jeu
*\param perso1 personnage qui attaque
*\param equipe1 equipe active durant le tour de jeu
*\param equipe2 equipe passif durant le tour de jeu
*\param nump  numéro du personnage %numj actuellement
*\param degat degat du sort
*\param portee portée du sort
*\param numj  numéro du equipe qui joue actuellement
*\param reseau sert à savoir si on est en reseau ou non
*\return int
*/

int felin (char map[N][N], t_personnage * perso1,t_equipe * equipe2, t_equipe * equipe1,int nump,int degat,int portee, int numj, int reseau){
  int i;
  int nb_client;
  creer_effet(perso1,3,perso1->coord.x,perso1->coord.y);

  if(!reseau){
    if(equipe1->numEquipe == 1){
      maj(map,equipe1,equipe2);
      affichage_map(map);
    }
    else {
      maj(map,equipe2,equipe1);
      affichage_map(map);
    }

    printf("Vous vous transformez en felin et aurez désormais le double de points de mouvement (une seule transformation à la fois)\n\n");
  }
  else{
    for(i=0;numj!=tab_joueur[i]->client_socket;i++);

    if(tab_joueur[i]->perso2!=NULL){
      nb_client=2;
    }
    else
      nb_client=4;

    if(tab_joueur[i]->numEquipe == 1){
      maj( map, tab_joueur[0], tab_joueur[1]);
      envoie_map(map, tab_joueur, numj, 2);
    }
    else{
      maj( map, tab_joueur[1], tab_joueur[0]);
      envoie_map(map, tab_joueur, numj, 2);
    }
    char test='0';
    //On verifie que la portee du sort pour savoir si il a besoin d'une cible */
    if(portee == 0)
      test='1';
    strcpy(buffer, &test);
    send(tab_joueur[i]->client_socket, buffer, BUFFER_LEN, 0);
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer,"Vous vous transformez en felin et aurez désormais le double de points de mouvement (une seule transformation à la fois)\n\n");
    send_all_tour(tab_joueur, i, nb_client, 1);
  }
  return 1;

}

/**
*\fn void fuego(char map[N][N], t_personnage * perso1,t_equipe * equipe2, t_equipe * equipe1,int nump,int degat,int portee, int numj)
*\brief sort inflige des degats a distance
*\param map carte de jeu
*\param perso1 personnage qui attaque
*\param equipe1 equipe active durant le tour de jeu
*\param equipe2 equipe passif durant le tour de jeu
*\param nump  numéro du personnage %numj actuellement
*\param degat degat du sort
*\param portee portée du sort
*\param numj  numéro du equipe qui joue actuellement
*\param reseau sert à savoir si on est en reseau ou non
*\return int
*/

int fuego (char map[N][N], t_personnage * perso1,t_equipe * equipe2, t_equipe * equipe1,int nump,int degat,int portee, int numj, int reseau){

  if(portee_degat(map,perso1,equipe2,equipe1,nump,degat,portee,numj, reseau))
    return 1;

  return 0;

}

/**
*\fn void revitalisation(char map[N][N],t_personnage * equipe1, t_joueur equipe1)
*\brief sort soigne un allié ou lui-meme
*\param map carte de jeu
*\param perso1 personnage qui attaque
*\param equipe1 equipe active durant le tour de jeu
*\param equipe2 equipe passif durant le tour de jeu
*\param nump  numéro du personnage %numj actuellement
*\param degat degat du sort
*\param portee portée du sort
*\param numj  numéro du equipe qui joue actuellement
*\param reseau sert à savoir si on est en reseau ou non
*\return int
*/

int revitalisation (char map[N][N], t_personnage * perso1,t_equipe * equipe2, t_equipe * equipe1,int nump,int degat,int portee, int numj, int reseau){

  char point[N][N];/*matrice affichant les possibilités de jeu*/
	int i, j, g, dist = portee,car=0;
	char choix;
  int nb_client;
  int cpt,cpt2;

  t_coordonnees coor;

  coor = range(map,perso1,equipe2,equipe1,nump,degat,portee,numj, reseau);

  if(((abs(perso1->coord.x-coor.x)+abs(perso1->coord.y-coor.y))>portee)){
    if(!reseau)
      printf("\nAction impossible, restitution des points d'action\n");
    return 0;
  }

  else {

    if(reseau){

      for(cpt=0;numj!=tab_joueur[cpt]->client_socket;cpt++);

      /*Pour 2 joueurs on trouve son adversaire */
      if(tab_joueur[cpt]->numEquipe==1)
        for(cpt2=0;tab_joueur[cpt2]->numEquipe == tab_joueur[cpt]->numEquipe;cpt2++);
      if(tab_joueur[cpt]->numEquipe==2)
        for(cpt2=0;tab_joueur[cpt2]->numEquipe == tab_joueur[cpt]->numEquipe;cpt2++);


      if(tab_joueur[cpt]->perso2!=NULL){
        nb_client=2;
      }
      else
        nb_client=4;
    }
    //réduction des points de vies après le coup
    if(numj==2 || (reseau && tab_joueur[cpt]->numEquipe==2)){
      switch (map[coor.y][coor.x]){

        case '2' :
          if(!reseau){
            if(equipe1->perso1->pv+degat <= equipe1->perso1->pv_max)
              equipe1->perso1->pv += degat;
            else
              equipe1->perso1->pv = equipe1->perso1->pv_max;
            printf("%s soigné.\n Il a maintenant : %i pv\n", equipe1->perso1->nom, equipe1->perso1->pv);
          }
          else{
            if(tab_joueur[cpt]->perso1->pv+degat <= tab_joueur[cpt]->perso1->pv_max)
              tab_joueur[cpt]->perso1->pv += degat;
            else
              tab_joueur[cpt]->perso1->pv = tab_joueur[cpt]->perso1->pv_max;
            sprintf(buffer,"%s soigné.\n Il a maintenant : %i pv\n", tab_joueur[cpt]->perso1->nom, tab_joueur[cpt]->perso1->pv);
            send_all_tour(tab_joueur, cpt, nb_client, 1);
          }
        break;

        case '4' :
          if(!reseau){
            if(equipe1->perso2->pv+degat <= equipe1->perso2->pv_max)
              equipe1->perso2->pv += degat;
            else
              equipe1->perso2->pv = equipe1->perso2->pv_max;
            printf("%s soigné.\n Il a maintenant : %i pv\n", equipe1->perso2->nom, equipe1->perso2->pv);
          }
          else{
            if(tab_joueur[cpt]->perso2->pv+degat <= tab_joueur[cpt]->perso2->pv_max)
              tab_joueur[cpt]->perso2->pv += degat;
            else
              tab_joueur[cpt]->perso2->pv = tab_joueur[cpt]->perso2->pv_max;
            sprintf(buffer,"%s soigné.\n Il a maintenant : %i pv\n", tab_joueur[cpt]->perso2->nom, tab_joueur[cpt]->perso2->pv);
            send_all_tour(tab_joueur, cpt, nb_client, 1);
          }
        break;
      }
    }

    else if(numj==1 || (reseau && tab_joueur[cpt]->numEquipe==1)){
      switch (map[coor.y][coor.x]){

        case '1' :
          if(!reseau){
            if(equipe1->perso1->pv+degat <= equipe1->perso1->pv_max)
              equipe1->perso1->pv += degat;
            else
              equipe1->perso1->pv = equipe1->perso1->pv_max;
            printf("%s soigné.\n Il a maintenant : %i pv\n", equipe1->perso1->nom, equipe1->perso1->pv);
          }
          else{
            if(tab_joueur[cpt]->perso1->pv+degat <= tab_joueur[cpt]->perso1->pv_max)
              tab_joueur[cpt]->perso1->pv += degat;
            else
              tab_joueur[cpt]->perso1->pv = tab_joueur[cpt]->perso1->pv_max;
            sprintf(buffer,"%s soigné.\n Il a maintenant : %i pv\n", tab_joueur[cpt]->perso1->nom, tab_joueur[cpt]->perso1->pv);
            send_all_tour(tab_joueur, cpt, nb_client, 1);
          }
        break;

        case '3' :
          if(!reseau){
            if(equipe1->perso2->pv+degat <= equipe1->perso2->pv_max)
              equipe1->perso2->pv += degat;
            else
              equipe1->perso2->pv = equipe1->perso2->pv_max;
            printf("%s soigné.\n Il a maintenant : %i pv\n", equipe1->perso2->nom, equipe1->perso2->pv);
          }
          else{
            if(tab_joueur[cpt]->perso2->pv+degat <= tab_joueur[cpt]->perso2->pv_max)
              tab_joueur[cpt]->perso2->pv += degat;
            else
              tab_joueur[cpt]->perso2->pv = tab_joueur[cpt]->perso2->pv_max;
            sprintf(buffer,"%s soigné.\n Il a maintenant : %i pv\n", tab_joueur[cpt]->perso2->nom, tab_joueur[cpt]->perso2->pv);
            send_all_tour(tab_joueur, cpt, nb_client, 1);
          }
        break;
      }
    }
  }//fin else

  if(!reseau){
    if(equipe1->numEquipe == 1){
    maj(map,equipe1,equipe2);
    affichage_map(map);
    }
    else {
    maj(map,equipe2,equipe1);
    affichage_map(map);
    }
  }

  else{
    if(tab_joueur[cpt]->numEquipe == 1){
      maj( map, tab_joueur[0], tab_joueur[1]);
      envoie_map(map, tab_joueur, numj, 2);
    }
    else{
      maj( map, tab_joueur[1], tab_joueur[0]);
      envoie_map(map, tab_joueur, numj, 2);
    }
  }

  return 1;
}

/**
*\fn int existe(char mat[N][N], char choix, int * x, int * y)
*\brief fonction qui verifie si la case choix existe bien
*\param map carte de jeu
*\param choix case ciblée
*\param *x coordonnées de la case ciblée
*\param *y coordonnées de la case ciblée
*\return int : booléen si la case existe ou pas
*/

int existe(char mat[N][N], char choix, int * x, int * y){
	int i, j;

	if(choix == 'o')
		return 0;

	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++){
			if(mat[i][j] == choix){
				*x = j;
				*y = i;
				return 1;
			}
		}
	}
	return 0;
}

t_coordonnees range(char map[N][N], t_personnage * perso1,t_equipe * equipe2, t_equipe * equipe1,int nump,int degat,int portee, int numj, int reseau){

  t_coordonnees coor;

  char point[N][N];/*matrice affichant les possibilités de jeu*/
  int i, j, g, dist = portee,car=0;
  char choix;

  /* recopie matrice dans la matrice point qui affiche la portee */

  for(i = 0; i < N; i++){
    for(j = 0; j < N; j++){
      point[i][j] = map[i][j];
    }
  }


  i = perso1->coord.y;
  j = perso1->coord.x - dist ;
  //affiche la portée vers le haut

  for( ; i >= 0  && ( dist >= 0 ) ; i--, dist--){
    j = perso1->coord.x - dist ;
    for( ;(j <= perso1->coord.x + dist) ; j++){
      if(j>=0){
        if(j<N){
        //verifie si il y a un obstacle ou une equipe à l'emplacement
          if(point[i][j] == '.'){
            point[i][j] = 'A' + car;
            car++;
          }
        }
      }
    }
  }

  dist = portee;
  g = perso1->coord.y;
  j = perso1->coord.x - dist ;
  //affiche la portée vers le bas
  for( ; g < N  && ( dist >= 0 ) ; g++, dist--){
    j = perso1->coord.x - dist ;
    for( ;(j <= perso1->coord.x + dist) ; j++){
      if(j>=0){
        if(j<N){
          if(point[g][j] == '.'){
  //ne pas repasser une seconde fois sur la ligne de  l'equipe
            if(g != perso1->coord.y){
              point[g][j] = 'A' + car;
              car ++;
            }
          }
        }
      }
    }
  }

  int x = 0, y = 0;
  if(!reseau){
    /* affichage de la carte avec le choix des cases atteignables par le personnage perso1*/
    affichage_map(point);

    do{
      printf("Où souhaitez vous taper : ");
      scanf(" %c", &choix);
    }while(!existe(point, choix, &x, &y));

    coor.x = x;
    coor.y = y;
  }

  else{

    /*En réseau numj prend la valeur de client_socket du joueur en cours de tour */

    /* Boucle qui a pour but de trouver a qui le tour est en cours */
    for(i=0;numj!=tab_joueur[i]->client_socket;i++);

    /*envoie la map */
    envoie_map(point, tab_joueur, i, 2);
    memset(buffer, 0, sizeof(buffer));
    char test='0';
    //On verifie que la portee du sort pour savoir si il a besoin d'une cible */
    if(portee == 0)
      test='1';
    strcpy(buffer, &test);
    send(tab_joueur[i]->client_socket, buffer, BUFFER_LEN, 0);
    /*envoie msg choix d'action*/
    do{
      sprintf(buffer,"Où souhaitez vous taper : \n");
      send(tab_joueur[i]->client_socket, buffer, strlen(buffer), 0);
      /* reception de la cible choisit par le joueur */
      memset(buffer, 0, sizeof(buffer));
      recv(tab_joueur[i]->client_socket,buffer,BUFFER_LEN, 0);
      strncpy(&choix,buffer+4,1);
    }while(!existe(point, choix, &x, &y));

    coor.x = x;
    coor.y = y;
  }

  return coor;
}

int damage(char map[N][N], t_personnage * perso1,t_equipe * equipe2, t_equipe * equipe1,int nump,int degat,int portee, int numj,t_coordonnees coor, int reseau){
    int i;
    //réduction des points de vies après le coup
    if(((abs(perso1->coord.x-coor.x)+abs(perso1->coord.y-coor.y))>portee)){
      if(!reseau)
        printf("\nAction impossible, restitution des points d'action\n");
      else{
        for(i=0;numj!=tab_joueur[i]->client_socket;i++);
        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer,"\nAction impossible, restitution des points d'action\n");
        send(tab_joueur[i]->client_socket, buffer, strlen(buffer), 0);
      }
      return 0;
    }

    else {

      if(!reseau){
        if(numj==1){
          switch (map[coor.y][coor.x]){

            case '2' :
              if (!(strcmp("armure",equipe2->perso1->effets[0].nom)))
                degat = (degat/2);

              equipe2->perso1->pv -= degat;
              printf("%s touché.\nPoint de vie : %i\n", equipe2->perso1->nom, equipe2->perso1->pv);
            break;

            case '4' :
              if (!(strcmp("armure",equipe2->perso2->effets[0].nom)))
                degat = (degat/2);

              equipe2->perso2->pv -= degat;
              printf("%s touché.\nPoint de vie : %i\n", equipe2->perso2->nom, equipe2->perso2->pv);
          break;
          }
        }

      if(numj==2){
        switch (map[coor.y][coor.x]){

          case '1' :
            if (!(strcmp("armure",equipe2->perso1->effets[0].nom)))
              degat = (degat/2);

            equipe2->perso1->pv -= degat;
            printf("%s touché.\nPoint de vie : %i\n", equipe2->perso1->nom, equipe2->perso1->pv);
          break;

          case '3' :
            if (!(strcmp("armure",equipe2->perso2->effets[0].nom)))
              degat = (degat/2);

            equipe2->perso2->pv -= degat;
            printf("%s touché.\nPoint de vie : %i\n", equipe2->perso2->nom, equipe2->perso2->pv);
          break;
        }
      }

      if(equipe1->numEquipe == 1){
        maj(map,equipe1,equipe2);
        affichage_map(map);
      }
      else {
        maj(map,equipe2,equipe1);
        affichage_map(map);
      }
    }

    else{
      int nb_client;
      int cpt2;
      for(i=0;numj!=tab_joueur[i]->client_socket;i++);

      /*Pour 2 joueurs on trouve son adversaire */
      if(tab_joueur[i]->numEquipe==1)
        for(cpt2=0;tab_joueur[cpt2]->numEquipe == tab_joueur[i]->numEquipe;cpt2++);
      if(tab_joueur[i]->numEquipe==2)
        for(cpt2=0;tab_joueur[cpt2]->numEquipe == tab_joueur[i]->numEquipe;cpt2++);


      if(tab_joueur[i]->perso2!=NULL){
        nb_client=2;
      }
      else
        nb_client=4;

      if(numj==1 || (reseau && tab_joueur[i]->numEquipe==1)){
        switch (map[coor.y][coor.x]){

          case '2' :
            if (!(strcmp("armure", tab_joueur[1]->perso1->effets[0].nom)))
              degat = (degat/2);
            tab_joueur[1]->perso1->pv -= degat;
            sprintf(buffer,"%s touché.\nPoint de vie : %i\n", tab_joueur[1]->perso1->nom, tab_joueur[1]->perso1->pv);
            send_all_tour(tab_joueur, i, nb_client, 1);
          break;

          case '4' :
            if (!(strcmp("armure", tab_joueur[1]->perso2->effets[0].nom)))
              degat = (degat/2);
            tab_joueur[1]->perso2->pv -= degat;
            sprintf(buffer,"%s touché.\nPoint de vie : %i\n", tab_joueur[1]->perso2->nom, tab_joueur[1]->perso2->pv);
            send_all_tour(tab_joueur, i, nb_client, 1);
        break;
        }
      }

      if(numj==2 || (reseau && tab_joueur[i]->numEquipe==2)){
        switch (map[coor.y][coor.x]){

          case '1' :
            if (!(strcmp("armure", tab_joueur[1]->perso1->effets[0].nom)))
              degat = (degat/2);

            tab_joueur[0]->perso1->pv -= degat;
            sprintf(buffer,"%s touché.\nPoint de vie : %i\n", tab_joueur[0]->perso1->nom, tab_joueur[0]->perso1->pv);
            send_all_tour(tab_joueur, i, nb_client, 1);
          break;

          case '3' :
            if (!(strcmp("armure", tab_joueur[1]->perso2->effets[0].nom)))
              degat = (degat/2);

            tab_joueur[0]->perso2->pv -= degat;
            sprintf(buffer,"%s touché.\nPoint de vie : %i\n", tab_joueur[0]->perso2->nom, tab_joueur[0]->perso2->pv);
            send_all_tour(tab_joueur, i, nb_client, 1);
          break;
        }
      }

      // if(tab_joueur[i]->numEquipe == 1){
      //  maj( map, tab_joueur[0], tab_joueur[1]);
      //  envoie_map(map, tab_joueur, numj, 2);
      // }
      // else {
      //  maj( map, tab_joueur[1], tab_joueur[0]);
      //  envoie_map(map, tab_joueur, numj, 2);
      // }
    }
  }//fin else

  return 1;
}

int portee_degat (char map[N][N], t_personnage * perso1,t_equipe * equipe2, t_equipe * equipe1,int nump,int degat,int portee, int numj, int reseau){

  t_coordonnees coor;

  coor = range(map,perso1,equipe2,equipe1,nump,degat,portee,numj, reseau);
  return (damage(map,perso1,equipe2,equipe1,nump,degat,portee,numj,coor, reseau));


}
