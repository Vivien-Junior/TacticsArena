#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <unistd.h>
#include "fonc.h"
#include "define.h"
#include <time.h>

char buffer[BUFFER_LEN];
 char map[N][N];
t_equipe * tab_joueur[N];

void fin(int sig){
	printf("fin du serveur");
}

int hostname_to_ip(char * hostname , char* ip)
{
    struct hostent *he;
    struct in_addr **addr_list;
    int i;

    if ( (he = gethostbyname( hostname ) ) == NULL)
    {
        // get the host info
        herror("gethostbyname");
        return 1;
    }

    addr_list = (struct in_addr **) he->h_addr_list;

    for(i = 0; addr_list[i] != NULL; i++)
    {
        //Return the first one;
        strcpy(ip , inet_ntoa(*addr_list[i]) );
        return 0;
    }

    return 1;
}


void view_ip()
{
  char s[256];
  struct hostent *host;
  struct in_addr **adr;
  if (!gethostname(s, 256))
    if ((host = gethostbyname(s)) != NULL)
      for (adr = (struct in_addr **)host->h_addr_list; *adr; adr++)
          printf("IP : %s\n", inet_ntoa(**adr));
}



/**
*\fn send_all_tour(t_equipe ** tab_joueur, int j, int nb_client,int info_donnee)
*\brief fonction qui envoie à tout les clients ou à tous sauf celui dont le tour est en cours
*\param tab_joueur tableau contenant les informations sur les joueurs
*\param j qui sert à savoir qui joue actuellement
*\param nb_client nombre de joueurs connecté
*\param info_donnee sert à savoir à qui faire l'envoie
*\return void
*/
void send_all_tour(t_equipe ** tab_joueur, int j, int nb_client,int info_donnee){
	//Annonce aux autres joueurs que c'est le tour de j
	for(int i=0;i < nb_client;i++){

		// int taille = strlen(buffer)+1;
		//
		// printf("%d\n\n%s", taille, buffer);


		switch(info_donnee){
			//info 1 : on envoie l'info à tout le monde
			case 1:
				send(tab_joueur[i]->client_socket, buffer, BUFFER_LEN, 0); // corriger ca
				break;
			//info 2 : on envoie la map
			/*case 2:
				send(tab_joueur[i]->client_socket, buffer, BUFFER_LEN, 0);
				break;*/
			//info 3 : on envoie l'info à tout le monde sauf au joueur dont le tour est en cours
			case 3:
				if(i==j);
				else{
					send(tab_joueur[i]->client_socket, buffer, BUFFER_LEN, 0);
				}
				break;
			default:
				printf("Erreur info_donnee\n");
				break;
		}

	}

}

/**
*\fn envoie_map(char matriceJeu[N][N], t_equipe ** tab_joueur, int j, int nb_client)
*\brief fonction qui concatene la map dans une chaine afin de l'envoyer aux clients ou à un client
*\param matriceJeu contient la map
*\param tab_joueur tableau contenant les informations sur les joueurs
*\param j qui sert à savoir qui joue actuellement
*\param nb_client nombre de joueurs connecté
*\return void
*/
void envoie_map(char matriceJeu[N][N], t_equipe ** tab_joueur, int j, int nb_client){
		int x, y, i;
		int test=0;
		char chaine[BUFFER_LEN];


		/* Initialise la chaine */
		for(x=0; x < N ; x++){
			for(y=0; y < N ; y++){
				if (x == 0 && y == 0)
					sprintf(buffer," %c |",matriceJeu[0][0]);
				else {
					if(matriceJeu[x][y]=='A')
						test=1;
					sprintf(chaine," %c |",matriceJeu[x][y]);
					strcat(buffer, chaine);
				}
			}
			strcat(buffer, "\n"); /* fin de ligne */
		}
		strcat(buffer, "\n");
		printf("\n%s \n", buffer);

		for(i=0;i < nb_client;i++){
			if(tab_joueur[i]->client_socket == j)
				j=i;
		}

		if(test){
			send(tab_joueur[j]->client_socket, buffer, BUFFER_LEN, 0);
		}
		else
			send_all_tour(tab_joueur, j, nb_client, 1);

}

/**
*\fn initialisation_partie(char matriceJeu[N][N], int nb_client, t_equipe ** tab_joueur, int j)
*\brief fonction qui initialise la partie en réseau
*\param matriceJeu contient la map
*\param tab_joueur tableau contenant les informations sur les joueurs
*\param j qui sert à savoir qui joue actuellement
*\param nb_client nombre de joueurs connecté
*\return void
*/
void initialisation_partie(char matriceJeu[N][N], t_equipe ** tab_joueur, int j, int nb_client){
	srand(time(NULL));
	int y1 = rand()%3+(N-3), x1 = rand()%(N-4)+2, y2, x2;
	int i,choix1 = 0,choix2 = 0,x=0;
	int nb_obs = 10; /* nb d'obstacles max sur la carte*/
	int test_pour_2 = 0, test_pour_4 = 0;

	sprintf(buffer,"Initialisation \n");
	send_all_tour(tab_joueur, j, nb_client, 1);

  /* remplissage de la map sans les equipes positionnés */
	for(i = 0; i < N ; i++){
		for(j = 0;j < N; j++){
				matriceJeu[i][j] = '.';
		}
	}


	for(j=0; j < nb_client; j++){
		sprintf(buffer,"\n ---- Choix des cases possibles pour le joueur %i ---- \n", j+1);
		send_all_tour(tab_joueur, j, nb_client, 1);

		if(tab_joueur[j]->numEquipe == 1){
			matriceJeu[y1][x1]='1';
			matriceJeu[y1-1][x1-2]='2';
			matriceJeu[y1 -1][x1 + 2]='3';
			/* Choix possible pour le positionement */
			sprintf(buffer,"\n[1]{x=%i y=%i}\n[2]{x=%i y=%i}\n[3]{x=%i y=%i}\n",x1,y1,x1-2,y1-1,x1+2,y1-1);
			send_all_tour(tab_joueur, j, nb_client, 1);
			memset(buffer, 0, sizeof(buffer));
			envoie_map(matriceJeu, tab_joueur, j, nb_client);

			do{
				test_pour_2 = 0, test_pour_4 = 0;
				sprintf(buffer,"Le joueur %d choisit où se placer ... \n", j+1);
				send_all_tour(tab_joueur, j, nb_client, 3);
				if(nb_client==2)
					x=2;
				else
					x=1;

				sprintf(buffer,"Où voulez vous placer votre %s ?", tab_joueur[j]->perso1->nom);
				send(tab_joueur[j]->client_socket, buffer, BUFFER_LEN, 0);
				/*reception du choix 1*/
				memset(buffer, 0, sizeof(buffer));
				recv(tab_joueur[j]->client_socket,buffer,BUFFER_LEN, 0);
				choix1 = atoi(buffer+4);
				printf("buffer choix 1 : %s\n", buffer );
				printf("choix 1 : %d\n", choix1 );
				if(x==2){
					sprintf(buffer,"Où voulez vous placer votre %s ?", tab_joueur[j]->perso2->nom);
					send(tab_joueur[j]->client_socket, buffer, BUFFER_LEN, 0);
					/*reception du choix 2*/
					memset(buffer, 0, sizeof(buffer));
					recv(tab_joueur[j]->client_socket,buffer,BUFFER_LEN, 0);
					choix2 = atoi(buffer+4);
					printf("buffer choix 2 : %s\n", buffer );
					printf("choix 2 : %d\n", choix2 );
				}

				/** Test pour 4 joueurs donc 1 seul perso */
				if(x==1)
					if( ((choix1 < 1)||(choix1 > 3)) )
						test_pour_4 = 1;

				/** Test pour 2 joueurs donc 2 persos */
				if(x==2)
					if( ((choix1 < 1)||(choix1 > 3)) || (choix1 == choix2) || ((choix2 < 1)||(choix2 > 3)) )
						test_pour_2 = 1;

			}while( (test_pour_2 == 1) || (test_pour_4 == 1) );

			/* choix case du perso 1 de l'équipe 1 */

			switch(choix1){
				case 1:
				 	matriceJeu[y1][x1] = '1';
					matriceJeu[y1-1][x1-2]='.';
					matriceJeu[y1 -1][x1 + 2]='.';
					tab_joueur[j]->perso1->coord.x = x1;
					tab_joueur[j]->perso1->coord.y = y1;
				break;

				case 2:
					matriceJeu[y1][x1] = '.';
					matriceJeu[y1-1][x1-2]='1';
					matriceJeu[y1 -1][x1 + 2]='.';
					tab_joueur[j]->perso1->coord.x = x1-2;
					tab_joueur[j]->perso1->coord.y = y1-1;
				break;

				case 3:
					matriceJeu[y1][x1] = '.';
					matriceJeu[y1-1][x1-2]='.';
					matriceJeu[y1 -1][x1 + 2]='1';
					tab_joueur[j]->perso1->coord.x = x1+2;
					tab_joueur[j]->perso1->coord.y = y1-1;
				break;
			}

			/* choix case du perso 2 de l'équipe 1 */

			if(choix2 != 0){
				switch(choix2){
					case 1:
						matriceJeu[y1][x1] = '3';
						matriceJeu[y1-1][x1-2]='.';
						matriceJeu[y1 -1][x1 + 2]='.';
						matriceJeu[tab_joueur[j]->perso1->coord.y][tab_joueur[j]->perso1->coord.x] = '1';
						tab_joueur[j]->perso2->coord.x = x1;
						tab_joueur[j]->perso2->coord.y = y1;
					break;

					case 2:
						matriceJeu[y1][x1] = '.';
						matriceJeu[y1-1][x1-2]='3';
						matriceJeu[y1 -1][x1 + 2]='.';
						matriceJeu[tab_joueur[j]->perso1->coord.y][tab_joueur[j]->perso1->coord.x] = '1';
						tab_joueur[j]->perso2->coord.x = x1-2;
						tab_joueur[j]->perso2->coord.y = y1-1;
					break;

					case 3:matriceJeu[y1][x1] = '.';
						matriceJeu[y1-1][x1-2]='.';
						matriceJeu[y1 -1][x1 + 2]='3';
						matriceJeu[tab_joueur[j]->perso1->coord.y][tab_joueur[j]->perso1->coord.x] = '1';
						tab_joueur[j]->perso2->coord.x = x1+2;
						tab_joueur[j]->perso2->coord.y = y1-1;
					break;
				}
			}

		}

		else if(tab_joueur[j]->numEquipe == 2){
			/* choix des positions de la deuxième équipe */
			y2 = rand()%3;
			x2 = rand()%(N-4)+2;
			matriceJeu[y2][x2]='4';
			matriceJeu[y2+1][x2-2]='5';
			matriceJeu[y2 +1][x2 + 2]='6';
			sprintf(buffer,"\n[4]{x=%i y=%i}\n[5]{x=%i y=%i}\n[6]{x=%i y=%i}\n",x2 ,y2 ,x2 -2,y2+1,x2+2,y2+1);
			send_all_tour(tab_joueur, j, nb_client, 1);
			envoie_map(matriceJeu, tab_joueur, j, nb_client);

			do{
				test_pour_2 = 0, test_pour_4 = 0;
				sprintf(buffer,"Le joueur %d choisit où se placer ... \n", j+1);
				send_all_tour(tab_joueur, j, nb_client, 3);
				if(nb_client==2)
					x=2;
				else
					x=1;

				sprintf(buffer,"Où voulez vous placer votre %s ?", tab_joueur[j]->perso1->nom);
				send(tab_joueur[j]->client_socket, buffer, BUFFER_LEN, 0);
				/*reception du choix 1*/
				memset(buffer, 0, sizeof(buffer));
				recv(tab_joueur[j]->client_socket,buffer,BUFFER_LEN, 0);
				choix1 = atoi(buffer+4);
				if(x==2){
					sprintf(buffer,"Où voulez vous placer votre %s ?", tab_joueur[j]->perso2->nom);
					send(tab_joueur[j]->client_socket, buffer, BUFFER_LEN, 0);
					/*reception du choix 2*/
					memset(buffer, 0, sizeof(buffer));
					recv(tab_joueur[j]->client_socket,buffer,BUFFER_LEN, 0);
					choix2 = atoi(buffer+4);
				}

				/** Test pour 4 joueurs donc 1 seul perso */
				if(x==1)
					if( ((choix1 < 4)||(choix1 > 6)) )
						test_pour_4 = 1;

				/** Test pour 2 joueurs donc 2 persos */
				if(x==2)
					if( ((choix1 < 4)||(choix1 > 6)) || (choix1 == choix2) || ((choix2 < 4)||(choix2 > 6)) )
						test_pour_2 = 1;
			}while( (test_pour_2 == 1) || (test_pour_4 == 1) );

			/* choix case du perso 1 de l'équipe 2 */

			switch(choix1){
				case 4: matriceJeu[y2][x2]='2';
					matriceJeu[y2+1][x2-2]='.';
					matriceJeu[y2 +1][x2 + 2]='.';
					tab_joueur[j]->perso1->coord.x = x2;
					tab_joueur[j]->perso1->coord.y = y2;
				break;

				case 5: matriceJeu[y2][x2]='.';
					matriceJeu[y2+1][x2-2]='2';
					matriceJeu[y2+1][x2 + 2]='.';
					tab_joueur[j]->perso1->coord.x = x2-2;
					tab_joueur[j]->perso1->coord.y = y2+1;
				break;

				case 6: matriceJeu[y2][x2]='.';
					matriceJeu[y2+1][x2-2]='.';
					matriceJeu[y2 +1][x2 + 2]='2';
					tab_joueur[j]->perso1->coord.x = x2+2;
					tab_joueur[j]->perso1->coord.y = y2+1;
				break;
			}

			/* choix case du perso 2 de l'équipe 2 */

			if(choix2 != 0){
				switch(choix2){
					case 4:
						matriceJeu[y2][x2]='4';
						matriceJeu[y2+1][x2-2]='.';
						matriceJeu[y2 +1][x2 + 2]='.';
	          matriceJeu[tab_joueur[j]->perso1->coord.y][tab_joueur[j]->perso1->coord.x] = '2';
						tab_joueur[j]->perso2->coord.x = x2;
						tab_joueur[j]->perso2->coord.y = y2;
					break;

					case 5: matriceJeu[y2][x2]='.';
						matriceJeu[y2+1][x2-2]='4';
						matriceJeu[y2+1][x2 + 2]='.';
	          matriceJeu[tab_joueur[j]->perso1->coord.y][tab_joueur[j]->perso1->coord.x] = '2';
						tab_joueur[j]->perso2->coord.x = x2-2;
						tab_joueur[j]->perso2->coord.y = y2+1;
					break;

					case 6: matriceJeu[y2][x2]='.';
						matriceJeu[y2+1][x2-2]='.';
						matriceJeu[y2 +1][x2 + 2]='4';
	          matriceJeu[tab_joueur[j]->perso1->coord.y][tab_joueur[j]->perso1->coord.x] = '2';
						tab_joueur[j]->perso2->coord.x = x2+2;
						tab_joueur[j]->perso2->coord.y = y2+1;
					break;
				}
			}

		}
		affichage_map(matriceJeu);
	}//fin for

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

	envoie_map(matriceJeu, tab_joueur, j, nb_client);
	affichage_map(matriceJeu);

}


/**
*\fn sort_uti_reseau(t_personnage * perso, t_equipe ** tab_joueur, int j)
*\brief fonction qui concatene les choix possible pour les sorts en une chaine qui sera envoyé au client
*\param perso perso actuellement utilisé
*\param tab_joueur tableau contenant les informations sur les joueurs
*\param j qui sert à savoir qui joue actuellement
*\return void
*/

void sort_uti_reseau(t_personnage * perso, t_equipe ** tab_joueur, int j){
	int i = 1;
	char chaine[BUFFER_LEN];

	memset(buffer, 0, sizeof(buffer));
	sprintf(buffer," ---- Quel sort souhaitez vous effectuer ? ---- \n");

	for (i = 0; i < MAX_NB_SORT ; i++){
		sprintf(chaine,"[%i] %s : portée [%i] / dégât [%i] / utilisation par tour [%i]\npoint d'action [%i]\n",i+1, perso->sorts[i]->nom, perso->sorts[i]->portee, perso->sorts[i]->degat, perso->sorts[i]->upt, perso->sorts[i]->point_action);
		strcat(buffer, chaine);
	}
	sprintf(chaine,"[%i] Ne pas utiliser de sort\nchoix:",i+1);
	strcat(buffer, chaine);

	send(tab_joueur[j]->client_socket, buffer, BUFFER_LEN, 0);

}

/**
*\fn deplacement_reseau(char map[N][N],int  pm ,int nump, int nb_client, t_equipe ** tab_joueur, int j)
*\brief fonction pour les déplacements
*\param map contient la map
*\param pm contient le nombre de point de mouvement restant
*\param nump détermine si c'est le perso1 ou perso2
*\param tab_joueur tableau contenant les informations sur les joueurs
*\param j qui sert à savoir qui joue actuellement
*\param nb_client nombre de joueurs connecté
*\return void
*/

int deplacement_reseau(char map[N][N],int  pm ,int nump, t_equipe ** tab_joueur, int j, int nb_client){
    t_personnage * temp;


    if(nump == 1 ){
        temp = tab_joueur[j]->perso1;
    }
    else{
        temp = tab_joueur[j]->perso2;
    }

    char c;

    do{
        sprintf(buffer,"Choisissez l'initial entre haut, bas, gauche, ou droite pour vous déplacer ou R pour rester sur place\n");
				send(tab_joueur[j]->client_socket, buffer, BUFFER_LEN, 0);
				memset(buffer, 0, sizeof(buffer));
				recv(tab_joueur[j]->client_socket, buffer, BUFFER_LEN,0);
				fprintf(stderr,"bufffer : %s\n",buffer);
				strncpy(&c, buffer+4, 1);
    }
    while(c != 'h' && c != 'b' && c != 'g' && c != 'd' && c != 'R');

  		switch (c) {
  			case 'h':

  				if(temp->coord.y-1<0 || (map[temp->coord.y-1][temp->coord.x]!='.')){
    				sprintf(buffer," ---- Déplacement impossible ---- \n" );
						send(tab_joueur[j]->client_socket, buffer, BUFFER_LEN, 0);
  					break;
  				}
  				else{
  					(temp->coord.y)-=1;
    				sprintf(buffer," ---- Déplacement en Haut ---- \n" );
						send(tab_joueur[j]->client_socket, buffer, BUFFER_LEN, 0);
  					pm--;
  					break;
  				}

  			case 'b':

  				if(temp->coord.y+1>=N || (map[temp->coord.y+1][temp->coord.x]!='.')){
    				sprintf(buffer," ---- Déplacement impossible ---- \n" );
						send(tab_joueur[j]->client_socket, buffer, BUFFER_LEN, 0);
  					break;
  				}

  				else{
  					(temp->coord.y)++;
    				sprintf(buffer," ---- Déplacement en Bas ---- \n" );
						send(tab_joueur[j]->client_socket, buffer, BUFFER_LEN, 0);
  					pm--;
  					break;
  				}

  			case 'g':

  				if(temp->coord.x-1<0 || (map[temp->coord.y][temp->coord.x-1]!='.')){
    				sprintf(buffer," ---- Déplacement impossible ---- \n" );
						send(tab_joueur[j]->client_socket, buffer, BUFFER_LEN, 0);
  					break;
  				}

  				else{
  					(temp->coord.x)--;
    				sprintf(buffer," ---- Déplacement à Gauche ---- \n" );
						send(tab_joueur[j]->client_socket, buffer, BUFFER_LEN, 0);
  					pm--;
  					break;
  				}

  			case 'd':

  				if(temp->coord.x+1>=N || (map[temp->coord.y][temp->coord.x+1]!='.')){
    				sprintf(buffer," ---- Déplacement impossible ---- \n" );
						send(tab_joueur[j]->client_socket, buffer, BUFFER_LEN, 0);
  					break;
  				}

  				else{
  					(temp->coord.x)++;
    				sprintf(buffer," ---- Déplacement à Droite ---- \n" );
						send(tab_joueur[j]->client_socket, buffer, BUFFER_LEN, 0);
  					pm--;
  					break;
  				}

  			case 'R':
  				return pm;
  		}

	return pm;
}

/**
 *\fn void affichage_coord_reseau(t_equipe * equipe)
 *\brief affichage des coordonnees, du nom, de la vie des personnages de l'equipe passée en paramètre
 *\param equipe structure contenant tout les personnages de l'équipe
 *\param tab_joueur tableau contenant les informations sur les joueurs
 *\param j qui sert à savoir qui joue actuellement
 *\param nb_client nombre de joueurs connecté
 *\return void
 */
void affichage_coord_reseau(t_equipe * equipe, t_equipe ** tab_joueur, int j, int nb_client){
	char chaine[BUFFER_LEN];

	sprintf(buffer,"equipe %i : \n", equipe->numEquipe);
	sprintf(chaine,"- Coordonnées %s : x = %i et y = %i {Vie actuelle:%i}\n", equipe->perso1->nom, equipe->perso1->coord.x, equipe->perso1->coord.y,equipe->perso1->pv);
	strcat(buffer, chaine);
  sprintf(chaine,"- Coordonnées %s : x = %i et y = %i {Vie actuelle:%i}\n", equipe->perso2->nom, equipe->perso2->coord.x, equipe->perso2->coord.y,equipe->perso2->pv);
	strcat(buffer, chaine);
	sprintf(chaine,"\n");
	strcat(buffer, chaine);

	send_all_tour(tab_joueur, j, nb_client, 1);
}


/**
*\fn tour_reseau(char map[N][N],t_equipe * equipe1,t_equipe * equipe2, int nump ,int nb_client, t_equipe ** tab_joueur, int j)
*\brief fonction de gestion des tours en reseau
*\param map contient la map
*\param equipe structure contenant tout les personnages de l'équipe1
*\param equipe structure contenant tout les personnages de l'équipe2
*\param nump détermine si c'est le perso1 ou perso2
*\param tab_joueur tableau contenant les informations sur les joueurs
*\param j qui sert à savoir qui joue actuellement
*\param nb_client nombre de joueurs connecté
*\return void
*/
void tour_reseau(char map[N][N],t_equipe * equipe1,t_equipe * equipe2, int nump, t_equipe ** tab_joueur, int j, int nb_client){

		t_personnage * temp;

		if(nump == 1 ){
        temp = tab_joueur[j]->perso1;
    }
    else{
        temp = tab_joueur[j]->perso2;
    }

		if (!(strcmp("armure",temp->effets[0].nom))){
				creer_effet(temp,6,temp->coord.x,temp->coord.y);
		}

    /* variable qui compte le nombre de déplacement max possible par personnage*/
    int pm=temp->pm;
    int choix_action = 0,choix_sort =0;
		char c;

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
            sprintf(buffer," ---- Quelle action souhaitez vous effectuer ? ---- \n[1]:Se déplacer ?[nombre de déplacement:%i]\n[2]:Utiliser un sort ? [nombre de points d'actions:%i]\n[3]:Passer son tour\nchoix:",pm,temp->pa);
						send(tab_joueur[j]->client_socket, buffer, BUFFER_LEN, 0);
						memset(buffer, 0, sizeof(buffer));
						recv(tab_joueur[j]->client_socket, buffer, BUFFER_LEN,0);
						fprintf(stderr,"bufer : %s\n",buffer );
						strncpy(&c, buffer+4, 1);
						choix_action = atoi(&c);
        }
        while(choix_action < 1 || choix_action > 3);
				fprintf(stderr,"choix recu par le serv : %d\n",choix_action );

        switch(choix_action){
            case 1:

                if(pm > 0 ){
									pm = deplacement_reseau( map, pm , nump, tab_joueur, j,  nb_client);
                }
                else{
                    sprintf(buffer,"erreur ---- Vous avez utilisé tous vos points de déplacements ----\n\n");
										send(tab_joueur[j]->client_socket, buffer, BUFFER_LEN, 0);
                }
                break;

            case 2:

                if(temp->pa > 0){
                    do{
                        /*affichage de la liste des sorts utilisable par le personnage actuel*/
												sort_uti_reseau(temp, tab_joueur, j);
												memset(buffer, 0, sizeof(buffer));
												recv(tab_joueur[j]->client_socket, buffer, BUFFER_LEN,0);
												choix_sort =  atoi(buffer+4);
                    }
                    while(choix_sort < 1 || choix_sort > MAX_NB_SORT+1);

										if (choix_sort == 5){
												sprintf(buffer," ---- Vous n'avez utilisé aucun sort,vous ne perdez aucun point d'action ----\n\n");
												send(tab_joueur[j]->client_socket, buffer, BUFFER_LEN, 0);
										}

										else{
											choix_sort--;

	                    if (temp->sorts[choix_sort]->point_action <= temp->pa && temp->sorts[choix_sort]->upt > 0 ) {
													if(temp->sorts[choix_sort]->sort(map,temp,equipe2,equipe1,nump,temp->sorts[choix_sort]->degat,temp->sorts[choix_sort]->portee,tab_joueur[j]->client_socket,1));
													temp->pa -= temp->sorts[choix_sort]->point_action ;
													temp->sorts[choix_sort]->upt-=1;
	                    }
	                    else if(temp->sorts[choix_sort]->upt == 0){
	                        sprintf(buffer,"erreur ---- Vous ne pouvez plus utiliser ce sort ce tour ci ----\n\n");
													send(tab_joueur[j]->client_socket, buffer, BUFFER_LEN, 0);
	                    }
	                    else{
	                        sprintf(buffer,"erreur ---- Vous n'avez pas assez de points d'actions ----\n\n");
													send(tab_joueur[j]->client_socket, buffer, BUFFER_LEN, 0);
	                    }
	                 }

                }

								else{
									sprintf(buffer,"erreur ---- Vous n'avez pas assez de points d'actions ----\n\n");
									send(tab_joueur[j]->client_socket, buffer, BUFFER_LEN, 0);
									break;

								}
								break;

						case 3:
							sprintf(buffer,"\n ---- Vous avez passé votre tour ---- \n\n");
							send(tab_joueur[j]->client_socket, buffer, BUFFER_LEN, 0);
							break;
        }//fin switch

				if(tab_joueur[j]->numEquipe == 1){
					maj( map, tab_joueur[0], tab_joueur[1]);
			    envoie_map(map, tab_joueur, j, nb_client);
				}
				else{
				 	maj( map, tab_joueur[1], tab_joueur[0]);
			    envoie_map(map, tab_joueur, j, nb_client);
			  }

    }//fin while

}


/**
*\fn serveur (int nb_joueur,t_personnage * persos [CLASSES+1], t_equipe * equipe1, t_equipe * equipe2)
*\brief fonction principale du serveur
*\param nb_joueur indique le nombre de joueur dans la partie
*\param persos[CLASSES+1] permet de recevoir les choix possibles pour les classes
*\param equipe structure contenant tout les personnages de l'équipe1
*\param equipe structure contenant tout les personnages de l'équipe2
*\return int
*/

int serveur (int nb_joueur, t_personnage * persos[CLASSES+1], t_equipe * equipe1, t_equipe * equipe2){
	int ma_socket;
	//int client_socket[N];
	struct sockaddr_in mon_address, client_address;
	unsigned int mon_address_longueur	;
	bzero(&mon_address,sizeof(mon_address));
	mon_address.sin_port = htons(30000);
	mon_address.sin_family = AF_INET;
	mon_address.sin_addr.s_addr = htonl(INADDR_ANY);

	char *hostname = "localhost";
    char ip[100];

    hostname_to_ip(hostname , ip);
	fprintf(stderr, "%s resolved to %s" , hostname , ip);
	view_ip();

	/* creation de socket */
	if ((ma_socket = socket(AF_INET,SOCK_STREAM,0))== -1) {
		printf("Impossible de créer la socket\n");
		exit(0);
	}

	/* Permet de réouvrir le serveur sur la même socket même si le serveur n'a pas été quitté correctement */
	char enable = 1;
    setsockopt(ma_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));

	/* bind serveur - socket */
	bind(ma_socket,(struct sockaddr *)&mon_address,sizeof(mon_address));
	/* ecoute sur la socket */
	listen(ma_socket,5);
	/* accept la connexion */
	mon_address_longueur = sizeof(client_address);


	//Boucle qui permet de garder le serveur ouvert prêt à relancer une nouvelle fin_partie
	//Tant que l'on à pas fermé le serveur avec un ctrl+c (    )ou sur la croix (sdl_quit)
	//while()

  /* on attend que les clients se connecte */
	int nb_client=0;
	//t_equipe * tab_joueur[nb_joueur];

	for(int i = 0; i < nb_joueur;i++)
		tab_joueur[i] = malloc(sizeof(t_equipe));


	while (nb_client < nb_joueur) {
		printf("En attente de connection\n");
		tab_joueur[nb_client]->client_socket = accept(ma_socket,(struct sockaddr *)&client_address,&mon_address_longueur);
		nb_client++;
	}
	printf("tous les joueurs sont ready !\n");

	int i=0, j;

	memset(buffer, 0, sizeof(buffer));

	//demande aux joueurs son équipe
	int membre_team1=0, membre_team2=0;
	for(j=0;j < nb_client;j++){

		sprintf(buffer,"Choissisez votre équipe :\n");
		send(tab_joueur[j]->client_socket, buffer, BUFFER_LEN, 0);

		sprintf(buffer,"Le joueur %d choisit son équipe :\n",j+1);
		send_all_tour(tab_joueur, j, nb_client, 3);

		memset(buffer, 0, sizeof(buffer));
		recv(tab_joueur[j]->client_socket, buffer, BUFFER_LEN,0);
		//verif si les equipes sont complete
		if(atoi(buffer+4) == 1 && membre_team1>=nb_client/2){
			sprintf(buffer,"Il n'y a plus de place dans l'équipe 1 :( vous allez rejoindre l'autre équipe\n");
			send(tab_joueur[j]->client_socket, buffer, BUFFER_LEN, 0);
			equipe2 = tab_joueur[j];
			tab_joueur[j]->numEquipe = 2;
		}
		else if(atoi(buffer+4) == 2 && membre_team2>=nb_client/2){
			sprintf(buffer,"Il n'y a plus de place dans l'équipe 2 :( vous allez rejoindre l'autre équipe\n");
			send(tab_joueur[j]->client_socket, buffer, BUFFER_LEN, 0);
			 equipe1 = tab_joueur[j];
			tab_joueur[j]->numEquipe = 1;
		}
		else{
			if(atoi(buffer+4)==1){
				equipe1 = tab_joueur[j];
				tab_joueur[j]->numEquipe = atoi(buffer+4);
			}
			else{
				equipe2 = tab_joueur[j];
				tab_joueur[j]->numEquipe = atoi(buffer+4);
			}
		}

		if(atoi(buffer+4)==1)
			membre_team1++;
		else if(atoi(buffer+4)==2)
			membre_team2++;

		sprintf(buffer,"Le joueur %d a choisit l'équipe : %d\n",j+1 ,tab_joueur[j]->numEquipe);
		send_all_tour(tab_joueur, j, nb_client, 3);
		fprintf(stderr,"Le joueur %d a choisit l'équipe : %d\n",j+1 ,tab_joueur[j]->numEquipe);
	}//fin for

	//demande aux joueurs son ou ses persos
	for(int j=0;j < nb_client;j++){
		int classe,classe2;
		char chaine[BUFFER_LEN];
		//si il y a quatre joueurs
		// if(nb_client==4){
		// 	sprintf(buffer,"Choissisez votre classe :\n");
		// 	send(tab_joueur[j]->client_socket, buffer, BUFFER_LEN, 0);
		//
		// 	sprintf(chaine,"\n");
		// 	for(i = 1; i <= CLASSES; i++){
		// 		sprintf(buffer,"[%d] : %s\n",i,persos[i]->nom);
		// 		strcat(chaine,	buffer);
		// 	}
		// 	sprintf(buffer,"%s\n",chaine);
		// 	send(tab_joueur[j]->client_socket, buffer, BUFFER_LEN, 0);
		//
		// 	sprintf(buffer,"Le joueur %d choisit sa classe :\n",j+1);
		// 	send_all_tour(tab_joueur, j, nb_client, 3);
		//
		// 	memset(buffer, 0, sizeof(buffer));
		// 	recv(tab_joueur[j]->client_socket, buffer, BUFFER_LEN,0);
		// 	classe = atoi(buffer+4);
		// 	if(strncmp("MSG", buffer, 3)==0){
		// 		tab_joueur[j]->perso1 = copie_perso(persos[classe]);
		// 		sprintf(buffer,"Le joueur %d a choisit sa classe : %s\n",j+1 ,tab_joueur[j]->perso1->nom);
		// 		send_all_tour(tab_joueur, j, nb_client, 3);
		// 		fprintf(stderr,"Le joueur %d a choisit la classe : %s\n",j+1 ,tab_joueur[j]->perso1->nom);
		// 	}
		// }

		//si il y a deux joueurs
		//else{
			sprintf(buffer,"Choissisez vos classes :\n");
			send(tab_joueur[j]->client_socket, buffer, BUFFER_LEN, 0);

			//affiche les choix possible et les envoies au client
			sprintf(chaine,"\n");
			for(i = 1; i <= CLASSES; i++){
				sprintf(buffer,"[%d] : %s\n",i,persos[i]->nom);
				//strcpy(chaine,strcat(chaine,	buffer));
				strcat(chaine,	buffer);
			}
			sprintf(buffer,"%s",chaine);

			send(tab_joueur[j]->client_socket, buffer, BUFFER_LEN, 0);

			sprintf(buffer,"Le joueur %d choisit ses classes :\n",j+1);
			send_all_tour(tab_joueur, j, nb_client, 3);

			memset(buffer, 0, sizeof(buffer));
			//choix 1er classe
			recv(tab_joueur[j]->client_socket, buffer, BUFFER_LEN,0);
			classe = atoi(buffer+4);
			memset(buffer, 0, sizeof(buffer));
			//choix 2e classe
			recv(tab_joueur[j]->client_socket, buffer, BUFFER_LEN,0);
			classe2 = atoi(buffer+4);
			if(strncmp("MSG", buffer, 3)==0){
				tab_joueur[j]->perso1 = copie_perso(persos[classe]);
				tab_joueur[j]->perso2 = copie_perso(persos[classe2]);
				sprintf(buffer,"Le joueur %d a choisit ses classes : %s , %s\n",j+1 ,tab_joueur[j]->perso1->nom, tab_joueur[j]->perso2->nom);
				send_all_tour(tab_joueur, j, nb_client, 3);
			}
		//}
	}

	//initialisation partie
	initialisation_partie(map,tab_joueur,j,nb_client);
 	affichage_map(map);



	//boucle de partie
	int fin_partie=0, j_connect = nb_client,x=0;
	int liste_deco[nb_client];
	int nump=1; /* numero perso*/
	int numj=1;  /*numero joueur*/
	int nb_tour=1;
  while(!fin_partie && partie_finie(equipe1) && partie_finie(equipe2)){

		//boucle d'un tour
		for( j=0; j < nb_client; j++){
			//on verifie que le joueur est connecté
			int verif=0;
			for( i=0; liste_deco[i] < nb_client; i++)
				if(j==liste_deco[i])
					verif=1;

			if(verif==0){
				memset(buffer, 0, sizeof(buffer));

				fprintf(stderr,"Tour de j%d \n", j);

				/* Si il reste des joueurs connectés */
				if(j_connect>0){
					//Annonce au joueur j que c'est son tour
					sprintf(buffer,"Cest votre tour\n");
					send(tab_joueur[j]->client_socket, buffer, BUFFER_LEN, 0);

					//printf("socket du client : %d = %d\n", j, tab_joueur[j]->client_socket);
					//printf("socket du client : %d = %d\n", j+1, tab_joueur[j+1]->client_socket);

					//Annonce aux autres joueurs que c'est le tour de j
					// sprintf(buffer,"\n Cest le tour du joueur %d\n", j+1);
					// send_all_tour(tab_joueur, j, nb_client, 3);

					//On attend les actions du joueur

					memset(buffer, 0, sizeof(buffer));
					recv(tab_joueur[j]->client_socket, buffer, BUFFER_LEN,0);
					/* Le joueur continue à jouer */
					if(strncmp("MSG", buffer, 3)==0){
						/* On cherche a savoir si c'est le perso 1 ou 2 qui joue */
						if(nump == 1){
							sprintf(buffer,"[Tour numéro : %d][Tour de l'équipe %d][personnage : %s]{Caractère : %c}\n\n", nb_tour, tab_joueur[j]->numEquipe, tab_joueur[j]->perso1->nom, carac_perso(tab_joueur[j]->numEquipe,nump));
							send_all_tour(tab_joueur, j, nb_client, 1);
							envoie_map(map, tab_joueur, j, nb_client);

							if(!est_mort( tab_joueur[j], tab_joueur[j]->perso1->id))
								tour_reseau(map, equipe1, equipe2, nump, tab_joueur, j, nb_client);
						}

						if(nump == 2){
							sprintf(buffer,"[Tour numéro : %d][Tour de l'équipe %d][personnage : %s]{Caractère : %c}\n\n", nb_tour, tab_joueur[j]->numEquipe, tab_joueur[j]->perso2->nom, carac_perso(tab_joueur[j]->numEquipe,nump));
							send_all_tour(tab_joueur, j, nb_client, 1);
							envoie_map(map, tab_joueur, j, nb_client);

							if(!est_mort( tab_joueur[j], tab_joueur[j]->perso2->id))
								tour_reseau(map, equipe2, equipe1, nump, tab_joueur, j, nb_client);
						}

						if(tab_joueur[j]->numEquipe == 1){
							maj( map, tab_joueur[0], tab_joueur[1]);
							envoie_map(map, tab_joueur, j, nb_client);
						}
						else {
							maj( map, tab_joueur[1], tab_joueur[0]);
							envoie_map(map, tab_joueur, j, nb_client);
						}
						affichage_map(map);

					}

					/* Le joueur va quitter la partie */
					else if(strncmp("QUITTER", buffer, 7) == 0) {
						printf(" déconnexion demandée : '%s'\n",buffer);
						//nombre de joueurs encore connecté
						j_connect--;
						//Annonce aux autres joueurs que j est déco
						sprintf(buffer,"Le joueur %d vient de se déconnecter\n", j+1);
						send_all_tour(tab_joueur, j, nb_client, 3);
						//on met le joueur deco dans une liste de personne deco
						liste_deco[x]=j;
						//fprintf(stderr,"joueur co : %d\n", j_connect);
						shutdown(tab_joueur[j]->client_socket,SHUT_RDWR);//  desactive les futurs envois et receptions
						close(tab_joueur[j]->client_socket);
					}

					// permet de vérifier si la connection avec un client déconnecter est bien fermée
					else {
						printf("[serveur] message inconnu : '%s'\n", buffer);
					}

					/*Affichage des coordonnées des différents personnages vivants,changement d'indice pour passer au tour du perso suivant, incrementation du tour global*/

					if((partie_finie(equipe1) || partie_finie(equipe2))&& tab_joueur[j]->numEquipe == 2 && nump == 2){
						sprintf(buffer,"===================================================\n\tAFFICHAGE COORDONNEES | FIN DU TOUR\n===================================================\n\n");
						send_all_tour(tab_joueur, j, nb_client, 1);
						affichage_coord_reseau(equipe1, tab_joueur, j, nb_client);
						affichage_coord_reseau(equipe2, tab_joueur, j, nb_client);
						nb_tour++;
					}
					/* Si c'est le tour de l'équipe 1 alors on passe à l'équipe2 */
					if(numj == 1)
						numj ++;

					/* Si c'est le tour du perso1 de l'équipe 2 alors on passe au perso2 de l'équipe1 */
					else if(nump == 1 && numj == 2){
						nump ++;
						numj--;
					}

					/* Si c'est le tour du perso2 de l'équipe 2 alors on passe au perso1 de l'équipe1 */
					else if(nump == 2 && numj == 2){
						numj--;
						nump--;
					}

					//fermeture de la partie si tout les joueurs sont déconnecté
					if(j_connect == 0){
							fin_partie=1;
					}
				}//fin if j_connect
			}//fin if verif
		}//fin for
	}//fin while ( fin de partie)

	/*
	printf("===================================================\n\tFIN DE LA PARTIE\n===================================================\n\n");
	*/
	/*Fin de partie, affichage du vainqueur*/
	/*
	if(partie_finie(equipe1))
		printf("Le equipe 1 a perdu \n");
	else if(partie_finie(equipe2))
		printf("Le equipe 2 a perdu\n");
	*/



	shutdown(ma_socket,2);
	close(ma_socket);
	return 0;
}





/*
	l66 corriger
	le joueur peut preshot sa selection de perso
	rendre lenvoie de msg plus general ( un fonc avec des switch ou truc comme ca)
	(pour les fonctions init ect faire un flags pour reseau ou local et mettre les if au lieu de doubler la fonction
	OU
	couper la fonction en pleins de morceau general au local / reseau et appliquer avec le flag)
	 cree des fonctions pour chaque etapes

	faire une boucle de jeu pour ne pas fermer le serveur mais juste la fin_partie
	gerer reprise deco par ia
	gerer la gestion du ctrl+c (Pour serv et client)
	implementer le jeu
		- pas oublier de gere la fin de fin_partie
		- envoie des données


	pb avec attire
	pb avec coup_zone
	pb avec revita
	truc bizarre quand tout le monde quitte
*/


// modif de la matrice only coter serveur donc pas de triche possible
