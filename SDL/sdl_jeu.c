#include "fonc.h"
#include "sdl_fonc.h"

/**
 *\file sdl_jeu.c
 *\brief programme d'initialisation du jeu version SDL
 *\author Vivien-Junior
 *\version 0.1
 *\date 08/04/2019
*/

/*******************************
********FONCTIONS DE JEU********
********************************/

/**
 *\fn void SDL_choix_perso(t_joueur *joueur1, t_joueur *joueur2)
 *\brief crée les personnages choisis
 *\param joueur1 joueur numéro 1
 *\param joueur2 joueur numéro 2
 *\return void
 * La fonction permet à l'utilisateur de choisir un personnage et de les initialisé selon leurs classes
*/
void SDL_choix_perso(t_joueur *joueur1, t_joueur *joueur2){

	int i, j, classe;
	int num_j = 1,choix;
	int x = 0, y = 0;

	joueur1->perso1 = NULL;
	joueur1->perso2 = NULL;
	joueur2->perso1 = NULL;
	joueur2->perso2 = NULL;

	SDL_RenderClear(renderer);
	/*Afficher le panel des personnage*/
	SDL_affichage_choix_perso(1);
	SDL_afficher_perso_choisi(*joueur1, *joueur2);
	SDL_RenderPresent(renderer);
	/*Attente du choix du premier perso du joueur 1*/
	do{
		SDL_Event e;
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_QUIT){
  				SDL_Quit();
  				return;
			}
			else if(e.type == SDL_MOUSEBUTTONDOWN){
				x = e.button.x;
				y = e.button.y;
			}
		}
	} while(!acces_choix_perso(SDL_choix_p, x, y, &classe));
	/*Création du premier perso du joueur 1 en fonction de la classe choisie*/ 
	joueur1->perso1 = copie_perso(persos[classe]);

	SDL_affichage_choix_perso(1);
	SDL_afficher_perso_choisi(*joueur1, *joueur2);
	SDL_RenderPresent(renderer);
	x = 0;
	y = 0;
	/*Attente du choix du second perso du joueur 1*/
	do{
		SDL_Event e;
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_QUIT){
  				SDL_Quit();
  				return;
			}
			else if(e.type == SDL_MOUSEBUTTONDOWN){
					x = e.button.x;
					y = e.button.y;
			}
		}
	} while(!acces_choix_perso(SDL_choix_p, x, y, &classe));
	/*Création du second perso du joueur 1 en fonction de la classe choisie*/ 
	joueur1->perso2 = copie_perso(persos[classe]);

	joueur1->numJoueur = 1;
	joueur1->nbPersoVivant = NB_PERSONNAGES;

	SDL_RenderClear(renderer);
	SDL_affichage_choix_perso(2);
	SDL_afficher_perso_choisi(*joueur1, *joueur2);
	SDL_RenderPresent(renderer);

	x = 0;
	y = 0;
	/*Attente du choix du premier perso du joueur 2*/
	do{
		SDL_Event e;
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_QUIT){
                SDL_DestroyRenderer(renderer);
  				SDL_DestroyWindow(window);
  				SDL_Quit();
  				return;
			}
			else if(e.type == SDL_MOUSEBUTTONDOWN){
				x = e.button.x;
				y = e.button.y;
			}
		}
	} while(!acces_choix_perso(SDL_choix_p, x, y, &classe));
	/*Création du premier perso du joueur 2 en fonction de la classe choisie*/ 
	joueur2->perso1 = copie_perso(persos[classe]);

	SDL_RenderClear(renderer);
	SDL_affichage_choix_perso(2);
	SDL_afficher_perso_choisi(*joueur1, *joueur2);
	SDL_RenderPresent(renderer);

	x = 0;
	y = 0;
	/*Attente du choix du second perso du joueur 2*/
	do{
		SDL_Event e;
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_QUIT){
  				SDL_Quit();
  				return;
			}
			else if(e.type == SDL_MOUSEBUTTONDOWN){
				x = e.button.x;
				y = e.button.y;
			}
		}
	} while(!acces_choix_perso(SDL_choix_p, x, y, &classe));
	/*Création du second perso du joueur 2 en fonction de la classe choisie*/
	joueur2->perso2 = copie_perso(persos[classe]);
	joueur2->numJoueur = 2;
	joueur2->nbPersoVivant = NB_PERSONNAGES;

	SDL_RenderClear(renderer);
	SDL_affichage_choix_perso(2);
	SDL_afficher_perso_choisi(*joueur1, *joueur2);
	SDL_RenderPresent(renderer);
	SDL_Delay(500);

}

/**
 *\fn void SDL_initialisation(t_joueur * joueur1,t_joueur * joueur2)
 *\brief initialise la position des deux joueurs
 *\param joueur1 joueur numéro 1
 *\param joueur2 joueur numéro 2
 *\return void
 * la fonction génère aussi des obstacles sur la carte
 * les caractères impairs ('1' et '3') sont utilisés pour le joueur 1, les caractères pairs pour le joueur 2
*/
void SDL_initialisation(t_joueur * joueur1,t_joueur * joueur2){

	srand(time(NULL));
	int y1 = rand()%3+(N-3), x1 = rand()%(N-4)+2, y2, x2, x = 0, y = 0, coordi = 0, coordj = 0;
	int i,j,numero_j=1;
	char choix1 = '0',choix2 = '0';
	int nb_persos = 1;
	/* nb d'obstacles max sur la carte*/
	int nb_obs = 10; 
  	/*remplissage de la map sans les joueurs positionnés*/
	for(i = 0; i < N ; i++){
		for(j = 0;j < N; j++)
				map[i][j] = '.';
	}

	while(numero_j <=2){
		if(numero_j == 1){
			/*Affichage des propositions d'emplacement pour le joueur 1*/
			map[y1][x1]='A';
			map[y1-1][x1-2]='B';
			map[y1 -1][x1 + 2]='C';
			
			SDL_RenderClear(renderer);
			SDL_afficher_map(map, *joueur1, *joueur2);
			SDL_afficher_message("Placer personnage 1", 945, 90, "Police/calendarnote.ttf", 35, 'o', 'N');
			SDL_afficher_message("ÌJoueur1Í",1055, 25, "Police/yorkwhiteletter.ttf", 45, 'r', 'N');
			SDL_RenderPresent(renderer);
			/*Attente du choix d'emplacement perso 1 joueur 1*/
			do{
				SDL_Event e;
				while(SDL_PollEvent(&e)){
					if(e.type == SDL_QUIT){
  						SDL_Quit();
  						return;
					}
					else if(e.type == SDL_MOUSEBUTTONDOWN){
						x = e.button.x;
						y = e.button.y;
					}
				}
			} while(!acces_possible(x, y, &coordi, &coordj));

			map[coordi][coordj] = 'x';
			SDL_RenderClear(renderer);
			SDL_afficher_map(map, *joueur1, *joueur2);
			SDL_RenderPresent(renderer);
			SDL_Delay(100);

			map[coordi][coordj] = '1';
			joueur1->perso1->coord.x = coordj;
			joueur1->perso1->coord.y = coordi;

			SDL_RenderClear(renderer);
			SDL_afficher_map(map, *joueur1, *joueur2);
			SDL_afficher_message("Placer personnage 2", 945, 90, "Police/calendarnote.ttf", 35, 'o', 'N');
			SDL_afficher_message("ÌJoueur1Í",1055, 25, "Police/yorkwhiteletter.ttf", 45, 'r', 'N');
			SDL_RenderPresent(renderer);

			coordj = 0;
			coordi = 0;
			/*Attente du choix d'emplacement perso 2 joueur 1*/
			do{
				SDL_Event e;
				while(SDL_PollEvent(&e)){
					if(e.type == SDL_QUIT){
  						SDL_Quit();
  						return;
					}
					else if(e.type == SDL_MOUSEBUTTONDOWN){
						x = e.button.x;
						y = e.button.y;
					}
				}
			} while(!acces_possible(x, y, &coordi, &coordj));

			map[coordi][coordj] = 'x';
			SDL_RenderClear(renderer);
			SDL_afficher_map(map, *joueur1, *joueur2);
			SDL_RenderPresent(renderer);
			SDL_Delay(100);

            map[coordi][coordj] = '3';
            joueur1->perso2->coord.x = coordj;
			joueur1->perso2->coord.y = coordi;

            if(map[y1][x1]!= '1' && map[y1][x1]!= '3')
            		map[y1][x1]='.';
            if(map[y1-1][x1-2]!='1' && map[y1-1][x1-2]!= '3')
            		map[y1-1][x1-2]='.';
            if(map[y1 -1][x1 + 2]!='1' && map[y1 -1][x1 + 2]!= '3')
            		map[y1 -1][x1 + 2]='.';
    	}
		else{
			/*Affichage des propositions d'emplacement pour le joueur 1*/
			y2 = rand()%3;
			x2 = rand()%(N-4)+2;
			map[y2][x2]='D';
			map[y2+1][x2-2]='E';
			map[y2 +1][x2 + 2]='F';

			SDL_RenderClear(renderer);
			SDL_afficher_map(map, *joueur1, *joueur2);
			SDL_afficher_message("Placer personnage 1", 945, 90, "Police/calendarnote.ttf", 35, 'o', 'N');
			SDL_afficher_message("ÌJoueur2Í",1055, 25, "Police/yorkwhiteletter.ttf", 45, 'r', 'N');
			SDL_RenderPresent(renderer);

			coordj = 0;
			coordi = 0;
			/*Attente du choix d'emplacement perso 1 joueur 2*/
			do{
				SDL_Event e;
				while(SDL_PollEvent(&e)){
					if(e.type == SDL_QUIT){
  						SDL_Quit();
  						return;
					}
					else if(e.type == SDL_MOUSEBUTTONDOWN){
						x = e.button.x;
						y = e.button.y;
					}
				}
			} while(!acces_possible(x, y, &coordi, &coordj));

			map[coordi][coordj] = 'x';
			SDL_RenderClear(renderer);
			SDL_afficher_map(map, *joueur1, *joueur2);
			SDL_RenderPresent(renderer);
			SDL_Delay(100);

			map[coordi][coordj] = '2';
			joueur2->perso1->coord.x = coordj;
			joueur2->perso1->coord.y = coordi;

			SDL_RenderClear(renderer);
			SDL_afficher_map(map, *joueur1, *joueur2);
			SDL_afficher_message("Placer personnage 2", 945, 90, "Police/calendarnote.ttf", 35, 'o', 'N');
			SDL_afficher_message("ÌJoueur2Í",1055, 25, "Police/yorkwhiteletter.ttf", 45, 'r', 'N');
			SDL_RenderPresent(renderer);

			coordj = 0;
			coordi = 0;
			/*Attente du choix d'emplacement perso 2 joueur 2*/
			do{
				SDL_Event e;
				while(SDL_PollEvent(&e)){
					if(e.type == SDL_QUIT){
  						SDL_Quit();
  						return;
					}
					else if(e.type == SDL_MOUSEBUTTONDOWN){
						x = e.button.x;
						y = e.button.y;
					}
				}
			} while(!acces_possible(x, y, &coordi, &coordj));

			map[coordi][coordj] = 'x';
			SDL_RenderClear(renderer);
			SDL_afficher_map(map, *joueur1, *joueur2);
			SDL_RenderPresent(renderer);
			SDL_Delay(100);
			map[coordi][coordj] = '4';
			joueur2->perso2->coord.x = coordj;
			joueur2->perso2->coord.y = coordi;

			if(map[y2][x2]!= '2' && map[y2][x2]!= '4')
            	map[y2][x2]='.';
      		if(map[y2+1][x2-2]!='2' && map[y2+1][x2-2]!= '4')
            	map[y2+1][x2-2]='.';
      		if(map[y2 +1][x2 + 2]!='2' && map[y2 +1][x2 + 2]!= '4')
            	map[y2 +1][x2 + 2]='.';

		}
		choix1 = 0;
    	choix2 = 0;
		numero_j++;
	}
    /*Obstacles générés sur toutes la carte sauf les bords*/
	int obs_x = rand()%(N-2)+1 , obs_y = rand()%(N-2)+1;
	/* génération des obstacles sur la carte */
	i = 1;
	while(i <= nb_obs){
		if(map[obs_y][obs_x] == '.'){
			map[obs_y][obs_x] = 'o';
			i++;
		}
		obs_x = rand()%(N-2)+1 , obs_y = rand()%(N-2)+1;
	}
	SDL_RenderClear(renderer);
	SDL_afficher_map(map, *joueur1, *joueur2);
	SDL_RenderPresent(renderer);
	SDL_Delay(1000);
}

/**
 *\fn int SDL_deplacement(t_joueur * j1,t_joueur * j2, int  n, int numero_perso)
 *\brief  Fonction permettant de deplacer son personnage et changer ses coordonnées
 *\param j1 joueur 1
 *\param j2 joueur 2
 *\param n nombre de déplacement
 *\param numero_perso numéro du personnage que l'on souhaite déplacer
 *\return int : qui correspondant au nombre de déplacement restant
 * La fonction affiche une portée de déplacement
 * suite au déplacement on effectue une mise à jour de la map pour que le déplacement effectué soit pris en compte lors de l'affichage du plan de jeu
*/
int SDL_deplacement(t_joueur * j1,t_joueur * j2, int  n, int numero_perso){

	int x = 0, y = 0, coordi = 0, coordj = 0;
	t_personnage * temp;

	if(numero_perso == 1)
	    temp = j1->perso1;
	else
	 	temp = j1->perso2;

	char choix;
	/*Matrice de portée*/
	char point[N][N];
	int i, j;
	/*Copie de la matrice de jeu dans la matrice de portée*/
	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++)
			point[i][j] = map[i][j];
	}
	i = temp->coord.y + 1;
	j = temp->coord.x ;
	
	int nb = 0;
	int car = 0;

	/*Affichage de la portée de développement*/
	for(; i < N && nb < n && point[i][j] == '.'; i++, nb++, car++)
		point[i][j] = 'A' + car;

	i = temp->coord.y - 1;
	j = temp->coord.x;
	nb = 0;

	for(; i >= 0  && nb < n && point[i][j] == '.'; i--, nb++, car++)
		point[i][j] = 'A' + car;

	i = temp->coord.y;
	j = temp->coord.x + 1;
	nb = 0;

	for(; j < N && nb < n && point[i][j] == '.'; j++, nb++, car++){
		point[i][j] = 'A' + car;
	}

	i = temp->coord.y;
	j = temp->coord.x - 1;
	nb = 0;

	for(; j >= 0 && nb < n && point[i][j] == '.'; j--, nb++, car++)
		point[i][j] = 'A' + car;

	if(n >= 2){
		if(point[temp->coord.y + 1][temp->coord.x + 1] == '.' && (temp->coord.y + 1) < N && (temp->coord.x + 1) < N)
			point[temp->coord.y + 1][temp->coord.x + 1] = 'A' + car++;
		if(point[temp->coord.y - 1][temp->coord.x - 1] == '.' && (temp->coord.y - 1) >= 0 && (temp->coord.x - 1) >= 0)
			point[temp->coord.y - 1][temp->coord.x - 1] = 'A' + car++;
		if(point[temp->coord.y + 1][temp->coord.x - 1] == '.' && (temp->coord.y + 1) < N && (temp->coord.x - 1) >= 0)
			point[temp->coord.y + 1][temp->coord.x - 1] = 'A' + car++;
		if(point[temp->coord.y - 1][temp->coord.x + 1] == '.' && (temp->coord.y - 1) >= 0 && (temp->coord.x + 1) < N)
			point[temp->coord.y - 1][temp->coord.x + 1] = 'A' + car++;
	}

	if(n >= 3){
		if(point[temp->coord.y + 2][temp->coord.x + 1] == '.' && (temp->coord.y + 2) < N && (temp->coord.x + 1) < N)
			point[temp->coord.y + 2][temp->coord.x + 1] = 'A' + car++;
		if(point[temp->coord.y - 2][temp->coord.x - 1] == '.' && (temp->coord.y - 2) >= 0 && (temp->coord.x - 1) >= 0)
			point[temp->coord.y - 2][temp->coord.x - 1] = 'A' + car++;
		if(point[temp->coord.y + 2][temp->coord.x - 1] == '.' && (temp->coord.y + 2) < N && (temp->coord.x - 1) >= 0)
			point[temp->coord.y + 2][temp->coord.x - 1] = 'A' + car++;
		if(point[temp->coord.y - 2][temp->coord.x + 1] == '.' && (temp->coord.y - 2) >= 0 && (temp->coord.x + 1) < N)
			point[temp->coord.y - 2][temp->coord.x + 1] = 'A' + car++;

		if(point[temp->coord.y + 1][temp->coord.x + 2] == '.' && (temp->coord.y + 1) < N && (temp->coord.x + 2) < N)
			point[temp->coord.y + 1][temp->coord.x + 2] = 'A' + car++;
		if(point[temp->coord.y - 1][temp->coord.x - 2] == '.' && (temp->coord.y - 1) >=  0 && (temp->coord.x - 2) >= 0)
			point[temp->coord.y - 1][temp->coord.x - 2] = 'A' + car++;
		if(point[temp->coord.y + 1][temp->coord.x - 2] == '.' && (temp->coord.y + 1) < N && (temp->coord.x - 2) >= 0)
			point[temp->coord.y + 1][temp->coord.x - 2] = 'A' + car++;
		if(point[temp->coord.y - 1][temp->coord.x + 2] == '.' && (temp->coord.y - 1) >= 0 && (temp->coord.x + 2) < N)
			point[temp->coord.y - 1][temp->coord.x + 2] = 'A' + car++;
	}
	SDL_RenderClear(renderer);
	if(j1->numJoueur == 1)
		SDL_afficher_map(point, * j1, * j2);
	else
		SDL_afficher_map(point, * j2, * j1);
	SDL_RenderPresent(renderer);
	/*Attente du choix de case de déplacement fait par l'utilisateur*/
	do{
		SDL_Event e;
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_QUIT){
                SDL_DestroyRenderer(renderer);
				SDL_DestroyWindow(window);
				SDL_Quit();
			}
			else if(e.type == SDL_MOUSEBUTTONDOWN){
				x = e.button.x;
				y = e.button.y;
			}
		}
	} while(!acces_possible(x, y, &coordi, &coordj));

	point[coordi][coordj] = 'x';
	SDL_RenderClear(renderer);
	if(j1->numJoueur == 1)
		SDL_afficher_map(point, * j1, * j2);
	else
		SDL_afficher_map(point, * j2, * j1);
	SDL_RenderPresent(renderer);
	SDL_Delay(100);

	int nb_dep = abs(temp->coord.x - coordj) + abs(temp->coord.y - coordi);

	temp->coord.x = coordj;
	temp->coord.y = coordi;
	
	if(j1->numJoueur==1){
		maj(map,j1,j2);
		SDL_RenderClear(renderer);
		SDL_afficher_map(map, * j1, * j2);
		SDL_RenderPresent(renderer);
	}
	else{
		maj(map,j2,j1);
		SDL_RenderClear(renderer);
		SDL_afficher_map(map, * j2, * j1);
		SDL_RenderPresent(renderer);
	}
	return n - nb_dep;
}
