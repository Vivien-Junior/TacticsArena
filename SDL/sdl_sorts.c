#include "fonc.h"
#include "sdl_fonc.h"

/**
 *\file sdl_sorts.c
 *\brief Fonction des sorts des personnages version SDL 
 *\author Vivien-Junior
 *\version 0.1
 *\date 08/04/2019
*/

/*******************************
**************SORTS*************
********************************/

/*******************************
*		Sorts du Guerrier	   *
********************************/

/**
 *\fn SDL_saut(t_joueur * j1,t_joueur * j2, int numero_perso)
 *\brief fonction saut
 *\param j1 joueur actif 
 *\param j2 joueur passif
 *\param numero_perso numéro du personnage qui souhaite effectuer le sort
 *\return void
 * La fonction permet au personnage actif de sauter des cases voire des obstacles
*/
void SDL_saut(t_joueur * j1,t_joueur * j2, int numero_perso){

	/*matrice de portée*/
	char point[N][N];
	int i, j, g, coordi = 0, coordj = 0;
	char choix;

	t_personnage * temp;
	if(numero_perso == 1 )
	    temp = j1->perso1;
	else
	    temp = j1->perso2;
  	int dist=temp->sorts[0]->portee;

	/*recopie matrice dans la matrice point qui affiche la portee*/
	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++)
			point[i][j] = map[i][j];
	}
	i = temp->coord.y;
	j = temp->coord.x - dist ;
	/*affiche la porter vers le haut*/
	for( ; i >= 0  && ( dist >= 0 ) ; i--, dist--){
		j = temp->coord.x - dist ;
		for( ;(j <= temp->coord.x + dist) ; j++){
			if(j>=0){
				if(j<N){
					if(point[i][j] == '.')
						point[i][j] = 'A';
				}
			}
		}
	}
	dist=temp->sorts[0]->portee;
	g = temp->coord.y;
	j = temp->coord.x - dist ;
	/*affiche la porter vers le bas*/
	for( ; g < N  && ( dist >= 0 ) ; g++, dist--){
		j = temp->coord.x - dist ;
		for( ;(j <= temp->coord.x + dist) ; j++){
			if(j>=0){
				if(j<N){
					if(point[g][j] == '.'){
						/*ne pas repasser une seconde fois sur la ligne du joueur*/
						if(g != temp->coord.y)
								point[g][j] = 'A';
					}
				}
			}
		}
	}

	/*affichage de la portée*/
	SDL_RenderClear(renderer);

	if(j1->numJoueur == 1)
		SDL_afficher_map(point, * j1, * j2);
	else
		SDL_afficher_map(point, * j2, * j1);
	SDL_RenderPresent(renderer);

	int x = 0, y = 0;
	/*Attente du choix de l'utilisateur*/
	do{
		SDL_Event e;
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_MOUSEBUTTONDOWN){
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
}

/**
 *\fn void SDL_soin (t_joueur * j1, t_joueur * j2,int numero_perso)
 *\brief fonction soin
 *\param j1 joueur actif 
 *\param j2 joueur passif
 *\param numero_perso numéro du personnage qui souhaite effectuer le sort
 *\return void
 * La fonction permet de soigner le personnage actif et d'augmenter ses points de vie
*/
void SDL_soin (t_joueur * j1, t_joueur * j2,int numero_perso){

  	t_personnage * temp;

    if(numero_perso == 1 )
        temp = j1->perso1;
    else
        temp = j1->perso2;

  	if(temp->pv+temp->sorts[1]->degat <= temp->pv_max)
   		temp->pv += temp->sorts[1]->degat;
  	else
    	temp->pv = temp->pv_max;

	effet_clignotement( * j1, * j2, temp);
	SDL_afficher_degat(* j1, * j2, temp, temp->sorts[1]->degat, 'p');

	if(j1->numJoueur == 1)
		SDL_afficher_map(map, * j1, * j2);
	else
		SDL_afficher_map(map, * j2, * j1);
	SDL_RenderPresent(renderer);
}

/**
 *\fn void SDL_petit_coup (t_joueur * j1, t_joueur * j2,int numero_perso)
 *\brief fonction petit_coup
 *\param j1 joueur actif 
 *\param j2 joueur passif
 *\param numero_perso numéro du personnage qui souhaite effectuer le sort
 *\return void
 * La fonction permet d'affliger des dégâts à l'adversaire
*/
void SDL_petit_coup (t_joueur * j1, t_joueur * j2,int numero_perso){

	/*matrice de portée*/
	char point[N][N];
	char player;

	t_personnage * temp;

    if( j1->numJoueur == 1){
      if(numero_perso == 1){
      	temp = j1->perso1;
      	player ='1';
      }
      else{
      	temp = j1->perso2;
      	player = '3';
      }
    }
    else{
      if(numero_perso == 1){
      	temp = j1->perso1;
      	player ='2';
      }
      else{
      	temp = j1->perso2;
      	player = '4';
      }
    }

	int i, j, g, dist = temp->sorts[2]->portee, coordi = 0, coordj = 0;
	char choix;

	/* recopie matrice dans la matrice point qui affiche la portee */
	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++)
			point[i][j] = map[i][j];
	}

	i = temp->coord.y;
	j = temp->coord.x - dist ;
	/*affiche la portée vers le haut*/
	for( ; i >= 0  && ( dist >= 0 ) ; i--, dist--){
		j = temp->coord.x - dist ;
		for( ;(j <= temp->coord.x + dist) ; j++){
			if(j>=0){
				if(j<N){
					if(point[i][j] == '.')
						point[i][j] = 'A';
					else if(point[i][j] != player){
						switch(point[i][j]){
							case '1':
								point[i][j] = 'a';
							break;
							case '2':
								point[i][j] = 'b';
							break;
							case '3':
								point[i][j] = 'c';
							break;
							case '4':
								point[i][j] = 'd';
							break;
						}
					}
				}
			}
		}
	}
	dist=temp->sorts[2]->portee;
	g = temp->coord.y;
	j = temp->coord.x - dist ;
	/*affiche la portée vers le bas*/
	for( ; g < N  && ( dist >= 0 ) ; g++, dist--){
		j = temp->coord.x - dist ;
		for( ;(j <= temp->coord.x + dist) ; j++){
			if(j>=0){
				if(j<N){
					if(point[g][j] == '.'){
						/*ne pas repasser une seconde fois sur la ligne du joueur*/
						if(g != temp->coord.y)
								point[g][j] = 'A';
					}
					else if(point[g][j] != player){
						switch(point[g][j]){
							case '1':
								point[g][j] = 'a';
							break;
							case '2':
								point[g][j] = 'b';
							break;
							case '3':
								point[g][j] = 'c';
							break;
							case '4':
								point[g][j] = 'd';
							break;
						}
					}
				}
			}
		}
	}
	/*affichage de la carte avec le choix des cases atteignables par le personnage*/
	SDL_RenderClear(renderer);
	if(j1->numJoueur == 1)
		SDL_afficher_map(point, * j1, * j2);
	else
		SDL_afficher_map(point, * j2, * j1);
	SDL_RenderPresent(renderer);

	int x = 0, y = 0;
	/*Attente du choix de l'utilisateur*/
	do{
		SDL_Event e;
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_MOUSEBUTTONDOWN){
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

	int deg;
  	/*réduction des points de vies après le coup*/
	if(j1->numJoueur==1){
		switch (map[coordi][coordj]){
			case '2' : 
				deg = temp->sorts[2]->degat;

				if(strcmp(j2->perso1->statut, "Armure") == 0){
					deg /= 2;
					strcpy(j2->perso1->statut, "Inconnu");
				}
				j2->perso1->pv -= deg;
				effet_clignotement(* j1, * j2, j2->perso1);
				SDL_afficher_degat(* j1, * j2, j2->perso1, deg, 'n');
				printf("%s touché.\nPoint de vie : %i\n", j2->perso1->nom, j2->perso1->pv);
			break;
			case '4' :
				deg = temp->sorts[2]->degat;

				if(strcmp(j2->perso2->statut, "Armure") == 0){
					deg /= 2;
					strcpy(j2->perso2->statut, "Inconnu");
				}
		
				j2->perso2->pv -= deg;
				effet_clignotement(* j1, * j2, j2->perso2);
				SDL_afficher_degat(* j1, * j2, j2->perso2, deg, 'n');
				printf("%s touché.\nPoint de vie : %i\n", j2->perso2->nom, j2->perso2->pv);
			 break;
		}
	}

	if(j1->numJoueur==2){
		switch (map[coordi][coordj]){
			case '1' :
				deg = temp->sorts[2]->degat;

				if(strcmp(j2->perso1->statut, "Armure") == 0){
					deg /= 2;
					strcpy(j2->perso1->statut, "Inconnu");
				}
				j2->perso1->pv -= deg;
				effet_clignotement(* j1, * j2, j2->perso1);
				SDL_afficher_degat(* j1, * j2, j2->perso1, deg, 'n');
				printf("%s touché.\nPoint de vie : %i\n", j2->perso1->nom, j2->perso1->pv);
			break;
			case '3' :
				deg = temp->sorts[2]->degat;

				if(strcmp(j2->perso2->statut, "Armure") == 0){
					deg /= 2;
					strcpy(j2->perso2->statut, "Inconnu");
				}
				j2->perso2->pv -= deg;
				effet_clignotement(* j1, * j2, j2->perso2);
				SDL_afficher_degat(* j1, * j2, j2->perso2, deg, 'n');
				printf("%s touché.\nPoint de vie : %i\n", j2->perso2->nom, j2->perso2->pv);
		 	break;
		}
	}
	if(j1->numJoueur == 1)
		SDL_afficher_map(map, * j1, * j2);
	else
		SDL_afficher_map(map, * j2, * j1);
	SDL_RenderPresent(renderer);
}

/**
 *\fn void SDL_grosCoup (t_joueur * j1, t_joueur * j2,int numero_perso)
 *\brief fonction gros_coup
 *\param j1 joueur actif 
 *\param j2 joueur passif
 *\param numero_perso numéro du personnage qui souhaite effectuer le sort
 *\return void
 * La fonction permet d'affliger des dégâts à l'adversaire
*/
void SDL_grosCoup (t_joueur * j1, t_joueur * j2,int numero_perso){

	/*matrice de portée*/
	char point[N][N];
	char player;

	t_personnage * temp;

  	if( j1->numJoueur == 1){
      if(numero_perso == 1){
      	temp = j1->perso1;
      	player ='1';
      }
      else{
      	temp = j1->perso2;
      	player = '3';
      }
    }
    else{
      if(numero_perso == 1){
      	temp = j1->perso1;
      	player ='2';
      }
      else{
      	temp = j1->perso2;
      	player = '4';
      }
    }
	int dist = temp->sorts[3]->portee, coordi = 0, coordj = 0;
	int i, j;
	char choix;
    /*copie plan jeu dans la matrice point*/
	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++){
			point[i][j] = map[i][j];
		}
	}
	i = temp->coord.y + 1;
	j = temp->coord.x ;
	int nb = 0;

	for(; i < N && nb < dist && point[i][j] != 'o'; i++, nb++){
		if(point[i][j] == '.')
			point[i][j] = 'A';
		else if(point[i][j] != player){
			switch(point[i][j]){
				case '1':
					point[i][j] = 'a';
				break;
				case '2':
					point[i][j] = 'b';
				break;
				case '3':
					point[i][j] = 'c';
				break;
				case '4':
					point[i][j] = 'd';
				break;
			}
		}
	}
	i = temp->coord.y - 1;
	j = temp->coord.x;
	nb = 0;

	for(; i >= 0  && nb < dist && point[i][j] != 'o'; i--, nb++){
		if(point[i][j] == '.')
			point[i][j] = 'A';
		else if(point[i][j] != player){
			switch(point[i][j]){
				case '1':
					point[i][j] = 'a';
				break;
				case '2':
					point[i][j] = 'b';
				break;
				case '3':
					point[i][j] = 'c';
				break;
				case '4':
					point[i][j] = 'd';
				break;
			}
		}
	}
	i = temp->coord.y;
	j = temp->coord.x + 1;
	nb = 0;

	for(; j < N && nb < dist && point[i][j] != 'o'; j++, nb++){
		if(point[i][j] == '.')
			point[i][j] = 'A';
		else if(point[i][j] != player){
			switch(point[i][j]){
				case '1':
					point[i][j] = 'a';
				break;
				case '2':
					point[i][j] = 'b';
				break;
				case '3':
					point[i][j] = 'c';
				break;
				case '4':
					point[i][j] = 'd';
				break;
			}
		}
	}
	i = temp->coord.y;
	j = temp->coord.x - 1;
	nb = 0;

	for(; j >= 0 && nb < dist && point[i][j] != 'o'; j--, nb++){
		if(point[i][j] == '.')
			point[i][j] = 'A';
		else if(point[i][j] != player){
			switch(point[i][j]){
				case '1':
					point[i][j] = 'a';
				break;
				case '2':
					point[i][j] = 'b';
				break;
				case '3':
					point[i][j] = 'c';
				break;
				case '4':
					point[i][j] = 'd';
				break;
			}
		}
	}
	/* affichage de la carte avec le choix des cases atteignables par le personnage perso1*/
	SDL_RenderClear(renderer);
	if(j1->numJoueur == 1)
		SDL_afficher_map(point, * j1, * j2);
	else
		SDL_afficher_map(point, * j2, * j1);
	SDL_RenderPresent(renderer);

	int x = 0, y = 0;
	/*Attente du choix de l'utilisateur*/
	do{
		SDL_Event e;
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_MOUSEBUTTONDOWN){
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

	int deg;
  	/*réduction des points de vies après le coup*/
	if(j1->numJoueur==1){
		switch (map[coordi][coordj]){
			case '2' : 
				deg = temp->sorts[3]->degat;

				if(strcmp(j2->perso1->statut, "Armure") == 0){
					deg /= 2;
					strcpy(j2->perso1->statut, "Inconnu");
				}
				j2->perso1->pv -= deg;
				effet_clignotement(* j1, * j2, j2->perso1);
				SDL_afficher_degat(* j1, * j2, j2->perso1, deg, 'n');
				printf("%s touché.\nPoint de vie : %i\n", j2->perso1->nom, j2->perso1->pv);
			break;
			case '4' :
				deg = temp->sorts[3]->degat;

				if(strcmp(j2->perso2->statut, "Armure") == 0){
					deg /= 2;
					strcpy(j2->perso2->statut, "Inconnu");
				}
				j2->perso2->pv -= deg;
				effet_clignotement(* j1, * j2, j2->perso2);
				SDL_afficher_degat(* j1, * j2, j2->perso2, deg, 'n');
				printf("%s touché.\nPoint de vie : %i\n", j2->perso2->nom, j2->perso2->pv);
			break;
		}
	}
	if(j1->numJoueur==2){
		switch (map[coordi][coordj]){
			case '1' :
				deg = temp->sorts[3]->degat;

				if(strcmp(j2->perso1->statut, "Armure") == 0){
					deg /= 2;
					strcpy(j2->perso1->statut, "Inconnu");
				}
				j2->perso1->pv -= deg;
				effet_clignotement(* j1, * j2, j2->perso1);
				SDL_afficher_degat(* j1, * j2, j2->perso1, deg, 'n');
				printf("%s touché.\nPoint de vie : %i\n", j2->perso1->nom, j2->perso1->pv);
			break;
			case '3' :
				deg = temp->sorts[3]->degat;

				if(strcmp(j2->perso2->statut, "Armure") == 0){
					deg /= 2;
					strcpy(j2->perso2->statut, "Inconnu");
				}	
				j2->perso2->pv -= deg;
				effet_clignotement(* j1, * j2, j2->perso2);
				SDL_afficher_degat(* j1, * j2, j2->perso2, deg, 'n');
				printf("%s touché.\nPoint de vie : %i\n", j2->perso2->nom, j2->perso2->pv);
		 	break;
		}
	}
	if(j1->numJoueur == 1)
		SDL_afficher_map(map, * j1, * j2);
	else
		SDL_afficher_map(map, * j2, * j1);
	SDL_RenderPresent(renderer);
}

/*******************************
*		Sorts de l'Archer	   *
********************************/

/**
 *\fn void SDL_diago (t_joueur * j1, t_joueur * j2,int numero_perso)
 *\brief fonction diago
 *\param j1 joueur actif 
 *\param j2 joueur passif
 *\param numero_perso numéro du personnage qui souhaite effectuer le sort
 *\return void
 * La fonction permet d'affliger des dégâts à l'adversaire
*/
void SDL_diago (t_joueur * j1, t_joueur * j2,int numero_perso){

	/*matrice de portée*/
	char point[N][N];
	int i, j;
	char choix;
	char player;

	t_personnage * temp;

    if( j1->numJoueur == 1){
      if(numero_perso == 1){
      	temp = j1->perso1;
      	player ='1';
      }
      else{
      	temp = j1->perso2;
      	player = '3';
      }
    }
    else{
      if(numero_perso == 1){
      	temp = j1->perso1;
      	player ='2';
      }
      else{
      	temp = j1->perso2;
      	player = '4';
      }
    }
  	int dist = temp->sorts[0]->portee, coordi = 0, coordj = 0;
	/*copie plan jeu dans la matrice point*/
	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++)
			point[i][j] = map[i][j];
	}
	i = temp->coord.y + 1;
	j = temp->coord.x + 1;
	int nb = 0;

	for(; (i < N && j < N) && nb < dist && point[i][j] != 'o'; i++, j++, nb++){
		if(point[i][j] == '.')
			point[i][j] = 'A';
		else if(point[i][j] != player){
			switch(point[i][j]){
				case '1':
					point[i][j] = 'a';
				break;
				case '2':
					point[i][j] = 'b';
				break;
				case '3':
					point[i][j] = 'c';
				break;
				case '4':
					point[i][j] = 'd';
				break;
			}
		}
	}
	i = temp->coord.y - 1;
	j = temp->coord.x - 1;
	nb = 0;

	for(; (i >= 0  && j >= 0) && nb < dist && point[i][j] != 'o'; i--, j--, nb++){
		if(point[i][j] == '.')
			point[i][j] = 'A';
		else if(point[i][j] != player){
			switch(point[i][j]){
				case '1':
					point[i][j] = 'a';
				break;
				case '2':
					point[i][j] = 'b';
				break;
				case '3':
					point[i][j] = 'c';
				break;
				case '4':
					point[i][j] = 'd';
				break;
			}
		}
	}
	i = temp->coord.y - 1;
	j = temp->coord.x + 1;
	nb = 0;

	for(; (i >= 0  && j < N) && nb < dist && point[i][j] != 'o'; i--, j++, nb++){
		if(point[i][j] == '.')
			point[i][j] = 'A';
		else if(point[i][j] != player){
			switch(point[i][j]){
				case '1':
					point[i][j] = 'a';
				break;
				case '2':
					point[i][j] = 'b';
				break;
				case '3':
					point[i][j] = 'c';
				break;
				case '4':
					point[i][j] = 'd';
				break;
			}
		}
	}
	i = temp->coord.y + 1;
	j = temp->coord.x - 1;
	nb = 0;

	for(; (i < N  && j >= 0) && nb < dist && point[i][j] != 'o'; i++, j--, nb++){
		if(point[i][j] == '.')
			point[i][j] = 'A';
		else if(point[i][j] != player){
			switch(point[i][j]){
				case '1':
					point[i][j] = 'a';
				break;
				case '2':
					point[i][j] = 'b';
				break;
				case '3':
					point[i][j] = 'c';
				break;
				case '4':
					point[i][j] = 'd';
				break;
			}
		}
	}
	/* affichage de la carte avec le choix des cases atteignables par le personnage perso1*/
	SDL_RenderClear(renderer);
	if(j1->numJoueur == 1)
		SDL_afficher_map(point, * j1, * j2);
	else
		SDL_afficher_map(point, * j2, * j1);
	SDL_RenderPresent(renderer);

	int x = 0, y = 0;
	/*Attente du choix de l'utilisateur*/
	do{
		SDL_Event e;
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_MOUSEBUTTONDOWN){
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

  	int deg;
  	/*réduction des points de vies après le coup*/
	if(j1->numJoueur==1){
		switch (map[coordi][coordj]){
			case '2' : 
				deg = temp->sorts[0]->degat;

				if(strcmp(j2->perso1->statut, "Armure") == 0){
					deg /= 2;
					strcpy(j2->perso1->statut, "Inconnu");
				}
				j2->perso1->pv -= deg;
				effet_clignotement(* j1, * j2, j2->perso1);
				SDL_afficher_degat(* j1, * j2, j2->perso1, deg, 'n');
				printf("%s touché.\nPoint de vie : %i\n", j2->perso1->nom, j2->perso1->pv);
			break;
			case '4' :
				deg = temp->sorts[0]->degat;

				if(strcmp(j2->perso2->statut, "Armure") == 0){
					deg /= 2;
					strcpy(j2->perso2->statut, "Inconnu");
				}
				j2->perso2->pv -= deg;
				effet_clignotement(* j1, * j2, j2->perso2);
				SDL_afficher_degat(* j1, * j2, j2->perso2, deg, 'n');
				printf("%s touché.\nPoint de vie : %i\n", j2->perso2->nom, j2->perso2->pv);
			 break;
		}
	}

	if(j1->numJoueur==2){
		switch (map[coordi][coordj]){
			case '1' :
				deg = temp->sorts[0]->degat;

				if(strcmp(j2->perso1->statut, "Armure") == 0){
					deg /= 2;
					strcpy(j2->perso1->statut, "Inconnu");
				}

				j2->perso1->pv -= deg;
				effet_clignotement(* j1, * j2, j2->perso1);
				SDL_afficher_degat(* j1, * j2, j2->perso1, deg, 'n');
				printf("%s touché.\nPoint de vie : %i\n", j2->perso1->nom, j2->perso1->pv);
			break;
			case '3' :
				deg = temp->sorts[0]->degat;

				if(strcmp(j2->perso2->statut, "Armure") == 0){
					deg /= 2;
					strcpy(j2->perso2->statut, "Inconnu");
				}
				j2->perso2->pv -= deg;
				effet_clignotement(* j1, * j2, j2->perso2);
				SDL_afficher_degat(* j1, * j2, j2->perso2, deg, 'n');
				printf("%s touché.\nPoint de vie : %i\n", j2->perso2->nom, j2->perso2->pv);
		 	break;
		}
	}
	if(j1->numJoueur == 1)
		SDL_afficher_map(map, * j1, * j2);
	else
		SDL_afficher_map(map, * j2, * j1);
	SDL_RenderPresent(renderer);
}

/**
 *\fn void SDL_ligne (t_joueur * j1, t_joueur * j2,int numero_perso)
 *\brief fonction ligne
 *\param j1 joueur actif 
 *\param j2 joueur passif
 *\param numero_perso numéro du personnage qui souhaite effectuer le sort
 *\return void
 * La fonction permet d'affliger des dégâts à l'adversaire
*/
void SDL_ligne (t_joueur * j1, t_joueur * j2,int numero_perso){

	/*matrice de portée*/
	char point[N][N];
	int i, j;
	char choix;

	char player;

	t_personnage * temp;

    if( j1->numJoueur == 1){
      if(numero_perso == 1){
      	temp = j1->perso1;
      	player ='1';
      }
      else{
      	temp = j1->perso2;
      	player = '3';
      }
    }
    else{
      if(numero_perso == 1){
      	temp = j1->perso1;
      	player ='2';
      }
      else{
      	temp = j1->perso2;
      	player = '4';
      }
    }

  	int dist = temp->sorts[1]->portee, coordi = 0, coordj = 0;
  	/*copie plan jeu dans la matrice point*/
	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++)
			point[i][j] = map[i][j];
	}
	i = temp->coord.y + 1;
	j = temp->coord.x ;

	int nb = 0;

	for(; i < N && nb < dist && point[i][j] != 'o'; i++, nb++){
		if(point[i][j] == '.')
			point[i][j] = 'A';
		else if(point[i][j] != player){
			switch(point[i][j]){
				case '1':
					point[i][j] = 'a';
				break;
				case '2':
					point[i][j] = 'b';
				break;
				case '3':
					point[i][j] = 'c';
				break;
				case '4':
					point[i][j] = 'd';
				break;
			}
		}
	}
	i = temp->coord.y - 1;
	j = temp->coord.x;
	nb = 0;

	for(; i >= 0  && nb < dist && point[i][j] != 'o'; i--, nb++){
		if(point[i][j] == '.')
			point[i][j] = 'A';
		else if(point[i][j] != player){
			switch(point[i][j]){
				case '1':
					point[i][j] = 'a';
				break;
				case '2':
					point[i][j] = 'b';
				break;
				case '3':
					point[i][j] = 'c';
				break;
				case '4':
					point[i][j] = 'd';
				break;
			}
		}
	}
	i = temp->coord.y;
	j = temp->coord.x + 1;
	nb = 0;

	for(; j < N && nb < dist && point[i][j] != 'o'; j++, nb++){
		if(point[i][j] == '.')
			point[i][j] = 'A';
		else if(point[i][j] != player){
			switch(point[i][j]){
				case '1':
					point[i][j] = 'a';
				break;
				case '2':
					point[i][j] = 'b';
				break;
				case '3':
					point[i][j] = 'c';
				break;
				case '4':
					point[i][j] = 'd';
				break;
			}
		}
	}
	i = temp->coord.y;
	j = temp->coord.x - 1;
	nb = 0;

	for(; j >= 0 && nb < dist && point[i][j] != 'o'; j--, nb++){
		if(point[i][j] == '.')
			point[i][j] = 'A';
		else if(point[i][j] != player){
			switch(point[i][j]){
				case '1':
					point[i][j] = 'a';
				break;
				case '2':
					point[i][j] = 'b';
				break;
				case '3':
					point[i][j] = 'c';
				break;
				case '4':
					point[i][j] = 'd';
				break;
			}
		}
	}
	/* affichage de la carte avec le choix des cases atteignables par le personnage perso1*/
	SDL_RenderClear(renderer);
	if(j1->numJoueur == 1)
		SDL_afficher_map(point, * j1, * j2);
	else
		SDL_afficher_map(point, * j2, * j1);
	SDL_RenderPresent(renderer);

	int x = 0, y = 0;
	/*Attente du choix de l'utilisateur*/
	do{
		SDL_Event e;
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_MOUSEBUTTONDOWN){
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

  	int deg;
  	/*réduction des points de vies après le coup*/
	if(j1->numJoueur==1){
		switch (map[coordi][coordj]){
			case '2' : 
				deg = temp->sorts[1]->degat;

				if(strcmp(j2->perso1->statut, "Armure") == 0){
					deg /= 2;
					strcpy(j2->perso1->statut, "Inconnu");
				}
				j2->perso1->pv -= deg;
				effet_clignotement(* j1, * j2, j2->perso1);
				SDL_afficher_degat(* j1, * j2, j2->perso1, deg, 'n');
				printf("%s touché.\nPoint de vie : %i\n", j2->perso1->nom, j2->perso1->pv);
			break;
			case '4' :
				deg = temp->sorts[1]->degat;

				if(strcmp(j2->perso2->statut, "Armure") == 0){
					deg /= 2;
					strcpy(j2->perso2->statut, "Inconnu");
				}
				j2->perso2->pv -= deg;
				effet_clignotement(* j1, * j2, j2->perso2);
				SDL_afficher_degat(* j1, * j2, j2->perso2, deg, 'n');
				printf("%s touché.\nPoint de vie : %i\n", j2->perso2->nom, j2->perso2->pv);
			 break;
		}
	}
	if(j1->numJoueur==2){
		switch (map[coordi][coordj]){
			case '1' :
				deg = temp->sorts[1]->degat;

				if(strcmp(j2->perso1->statut, "Armure") == 0){
					deg /= 2;
					strcpy(j2->perso1->statut, "Inconnu");
				}
				j2->perso1->pv -= deg;
				effet_clignotement(* j1, * j2, j2->perso1);
				SDL_afficher_degat(* j1, * j2, j2->perso1, deg, 'n');
				printf("%s touché.\nPoint de vie : %i\n", j2->perso1->nom, j2->perso1->pv);
			break;
			case '3' :
				deg = temp->sorts[1]->degat;

				if(strcmp(j2->perso2->statut, "Armure") == 0){
					deg /= 2;
					strcpy(j2->perso2->statut, "Inconnu");
				}
				j2->perso2->pv -= deg;
				effet_clignotement(* j1, * j2, j2->perso2);
				SDL_afficher_degat(* j1, * j2, j2->perso2, deg, 'n');
				printf("%s touché.\nPoint de vie : %i\n", j2->perso2->nom, j2->perso2->pv);
		 	break;
		}
	}
	if(j1->numJoueur == 1)
		SDL_afficher_map(map, * j1, * j2);
	else
		SDL_afficher_map(map, * j2, * j1);
	SDL_RenderPresent(renderer);
}

/**
 *\fn SDL_double_tape (t_joueur * j1, t_joueur * j2,int numero_perso)
 *\brief fonction double_tape
 *\param j1 joueur actif 
 *\param j2 joueur passif
 *\param numero_perso numéro du personnage qui souhaite effectuer le sort
 *\return void
 * La fonction permet d'affliger des dégâts à l'adversaire
*/
void SDL_double_tape (t_joueur * j1, t_joueur * j2,int numero_perso){

	/*matrice de portée*/
	char point[N][N];
	int i, j, g;
	char choix, player;

	t_personnage * temp;
	if( j1->numJoueur == 1){
      if(numero_perso == 1){
      	temp = j1->perso1;
      	player ='1';
      }
      else{
      	temp = j1->perso2;
      	player = '3';
      }
    }
    else{
      if(numero_perso == 1){
      	temp = j1->perso1;
      	player ='2';
      }
      else{
      	temp = j1->perso2;
      	player = '4';
      }
    }

  	int dist = temp->sorts[2]->portee, coordi = 0, coordj = 0;
	/* recopie matrice dans la matrice point qui affiche la portee */
	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++)
			point[i][j] = map[i][j];
	}
	i = temp->coord.y;
	/*affiche la portée vers le haut*/
	for( ; i >= 0  && ( dist >= 0 ) ; i--, dist--){
		j = temp->coord.x - dist ;
		for( ;(j <= temp->coord.x + dist) ; j++){
			if(j>=0){
				if(j<N){
					if(point[i][j] == '.')
						point[i][j] = 'A';
					else if(point[i][j] != player){
						switch(point[i][j]){
							case '1':
								point[i][j] = 'a';
							break;
							case '2':
								point[i][j] = 'b';
							break;
							case '3':
								point[i][j] = 'c';
							break;
							case '4':
								point[i][j] = 'd';
							break;
						}
					}
				}
			}
		}
	}
	dist = temp->sorts[2]->portee;
	g = temp->coord.y;
	/*affiche la portée vers le bas*/
	for( ; g < N  && ( dist >= 0 ) ; g++, dist--){
		j = temp->coord.x - dist ;
		for( ;(j <= temp->coord.x + dist) ; j++){
			if(j>=0){
				if(j<N){
					if(g != temp->coord.y){
						/*ne pas repasser une seconde fois sur la ligne du joueur*/
						if(point[g][j] == '.')
								point[g][j] = 'A';
						else if(point[g][j] != player){
							switch(point[g][j]){
								case '1':
									point[g][j] = 'a';
								break;
								case '2':
									point[g][j] = 'b';
								break;
								case '3':
									point[g][j] = 'c';
								break;
								case '4':
									point[g][j] = 'd';
								break;
							}
						}
					}
				}
			}
		}
	}
	/* affichage de la carte avec le choix des cases atteignables par le personnage perso1*/
	SDL_RenderClear(renderer);
	if(j1->numJoueur == 1)
		SDL_afficher_map(point, * j1, * j2);
	else
		SDL_afficher_map(point, * j2, * j1);
	SDL_RenderPresent(renderer);

	int x = 0, y = 0;
	/*Attente du choix de l'utilisateur*/
	do{
		SDL_Event e;
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_MOUSEBUTTONDOWN){
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

  	int deg;
  	/*réduction des points de vies après le coup*/
	if(j1->numJoueur==1){
		switch (map[coordi][coordj]){
			case '2' : 
				deg = temp->sorts[2]->degat;

				if(strcmp(j2->perso1->statut, "Armure") == 0){
					deg /= 2;
					strcpy(j2->perso1->statut, "Inconnu");
				}
				j2->perso1->pv -= deg;
				effet_clignotement(* j1, * j2, j2->perso1);
				SDL_afficher_degat(* j1, * j2, j2->perso1, deg, 'n');
				printf("%s touché.\nPoint de vie : %i\n", j2->perso1->nom, j2->perso1->pv);
			break;
			case '4' :
				deg = temp->sorts[2]->degat;

				if(strcmp(j2->perso2->statut, "Armure") == 0){
					deg /= 2;
					strcpy(j2->perso2->statut, "Inconnu");
				}
				j2->perso2->pv -= deg;
				effet_clignotement(* j1, * j2, j2->perso2);
				SDL_afficher_degat(* j1, * j2, j2->perso2, deg, 'n');
				printf("%s touché.\nPoint de vie : %i\n", j2->perso2->nom, j2->perso2->pv);
			 break;
		}
	}
	if(j1->numJoueur==2){
		switch (map[coordi][coordj]){
			case '1' :
				deg = temp->sorts[2]->degat;

				if(strcmp(j2->perso1->statut, "Armure") == 0){
					deg /= 2;
					strcpy(j2->perso1->statut, "Inconnu");
				}
				j2->perso1->pv -= deg;
				effet_clignotement(* j1, * j2, j2->perso1);
				SDL_afficher_degat(* j1, * j2, j2->perso1, deg, 'n');
				printf("%s touché.\nPoint de vie : %i\n", j2->perso1->nom, j2->perso1->pv);
			break;
			case '3' :
				deg = temp->sorts[2]->degat;

				if(strcmp(j2->perso2->statut, "Armure") == 0){
					deg /= 2;
					strcpy(j2->perso2->statut, "Inconnu");
				}
				j2->perso2->pv -= deg;
				effet_clignotement(* j1, * j2, j2->perso2);
				SDL_afficher_degat(* j1, * j2, j2->perso2, deg, 'n');
				printf("%s touché.\nPoint de vie : %i\n", j2->perso2->nom, j2->perso2->pv);
		 	break;
		}
	}
	if(j1->numJoueur == 1)
		SDL_afficher_map(map, * j1, * j2);
	else
		SDL_afficher_map(map, * j2, * j1);
	SDL_RenderPresent(renderer);
}

/**
 *\fn SDL_coup_zone (t_joueur * j1, t_joueur * j2,int numero_perso)
 *\brief fonction coup_zone
 *\param j1 joueur actif 
 *\param j2 joueur passif
 *\param numero_perso numéro du personnage qui souhaite effectuer le sort
 *\return void
 * La fonction permet d'affliger des dégâts aux adversaires autour du point d'impact
*/
void SDL_coup_zone (t_joueur * j1, t_joueur * j2,int numero_perso){

	/*matrice de portée*/
	char point[N][N];
	int i, j, g;
	char choix, player;

	t_personnage * temp;
	if( j1->numJoueur == 1){
      if(numero_perso == 1){
      	temp = j1->perso1;
      	player ='1';
      }
      else{
      	temp = j1->perso2;
      	player = '3';
      }
    }
    else{
      if(numero_perso == 1){
      	temp = j1->perso1;
      	player ='2';
      }
      else{
      	temp = j1->perso2;
      	player = '4';
      }
    }

  	int dist = temp->sorts[3]->portee, coordi = 0, coordj = 0;
	/*recopie matrice dans la matrice point qui affiche la portee*/
	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++)
			point[i][j] = map[i][j];
	}
	i = temp->coord.y;
	j = temp->coord.x - dist ;
	/*affiche la porter vers le haut*/
	for( ; i >= 0  && ( dist >= 0 ) ; i--, dist--){
		j = temp->coord.x - dist ;
		for( ;(j <= temp->coord.x + dist) ; j++){
			if(j>=0){
				if(j<N){
					/*verifie si il y a un obstacle ou un joueur à l'emplacement*/
					if(point[i][j] == '.' )
							point[i][j] = 'A';
					else if(point[i][j] != player){
						switch(point[i][j]){
							case '1':
								point[i][j] = 'a';
							break;
							case '2':
								point[i][j] = 'b';
							break;
							case '3':
								point[i][j] = 'c';
							break;
							case '4':
								point[i][j] = 'd';
							break;
						}
					}
				}
			}
		}
	}
	dist = temp->sorts[3]->portee, coordi = 0, coordj = 0;
	g = temp->coord.y;
	j = temp->coord.x - dist ;
	/*affiche la porter vers le bas*/
	for( ; g < N  && ( dist >= 0 ) ; g++, dist--){
		j = temp->coord.x - dist ;
		for( ;(j <= temp->coord.x + dist) ; j++){
			if(j>=0){
				if(j<N){
					if(g != temp->coord.y){
						/*ne pas repasser une seconde fois sur la ligne du joueur*/
						if(point[g][j] == '.')
								point[g][j] = 'A';
						else if(point[g][j] != player){
							switch(point[g][j]){
								case '1':
									point[g][j] = 'a';
								break;
								case '2':
									point[g][j] = 'b';
								break;
								case '3':
									point[g][j] = 'c';
								break;
								case '4':
									point[g][j] = 'd';
								break;
							}
						}
					}
				}
			}
		}
	}
	/* affichage de la carte avec le choix des cases atteignables par le personnage perso1*/
	SDL_RenderClear(renderer);
	if(j1->numJoueur == 1)
		SDL_afficher_map(point, * j1, * j2);
	else
		SDL_afficher_map(point, * j2, * j1);
	SDL_RenderPresent(renderer);

	int x = 0, y = 0;
	/*Attente du choix de l'utilisateur*/
	do{
		SDL_Event e;
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_MOUSEBUTTONDOWN){
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
  	/*vérifie si il y a des personnages dans la croix de largeur 'l' si oui leurs infliges les dégats*/
	int l = 1;
	int deg;
	/*ligne horizontale*/
	for(i = coordi, j = coordj-l; j <= coordj + l; j++){
		if(j1->numJoueur == 1){
			switch (map[i][j]){
				case '2' :
					deg = temp->sorts[3]->degat;

					if(strcmp(j2->perso1->statut, "Armure") == 0){
						deg /= 2;
						strcpy(j2->perso1->statut, "Inconnu");
					}
					j2->perso1->pv -= deg;
					effet_clignotement(* j1, * j2, j2->perso1);
					SDL_afficher_degat(* j1, * j2, j2->perso1, deg, 'n');
					printf("%s touché.\nPoint de vie : %i\n", j2->perso1->nom, j2->perso1->pv);
				break;
				case '4' :
					deg = temp->sorts[3]->degat;

					if(strcmp(j2->perso2->statut, "Armure") == 0){
						deg /= 2;
						strcpy(j2->perso2->statut, "Inconnu");
					}
					j2->perso2->pv -= deg;
					effet_clignotement(* j1, * j2, j2->perso2);
					SDL_afficher_degat(* j1, * j2, j2->perso2, deg, 'n');
					printf("%s touché.\nPoint de vie : %i\n", j2->perso2->nom, j2->perso2->pv);
				break;
			}
		}
		if(j1->numJoueur == 2){
			switch (map[i][j]){
				case '1' :
					deg = temp->sorts[3]->degat;

					if(strcmp(j2->perso1->statut, "Armure") == 0){
						deg /= 2;
						strcpy(j2->perso1->statut, "Inconnu");
					}
					j2->perso1->pv -= deg;
					effet_clignotement(* j1, * j2, j2->perso1);
					SDL_afficher_degat(* j1, * j2, j2->perso1, deg, 'n');
					printf("%s touché.\nPoint de vie : %i\n", j2->perso1->nom, j2->perso1->pv);
				break;
				case '3' :
					deg = temp->sorts[3]->degat;

					if(strcmp(j2->perso2->statut, "Armure") == 0){
						deg /= 2;
						strcpy(j2->perso2->statut, "Inconnu");
					}
					j2->perso2->pv -= deg;
					effet_clignotement(* j1, * j2, j2->perso2);
					SDL_afficher_degat(* j1, * j2, j2->perso2, deg, 'n');
					printf("%s touché.\nPoint de vie : %i\n", j2->perso2->nom, j2->perso2->pv);
				break;
			}
		}
	}
	/*ligne verticale*/
	for(i = coordi - l, j = coordj; i <= coordi + l; i++){
		if(i!=coordi){
			if(j1->numJoueur == 1){
				switch (map[i][j]){
					case '2' :
						deg = temp->sorts[3]->degat;

						if(strcmp(j2->perso1->statut, "Armure") == 0){
							deg /= 2;
							strcpy(j2->perso1->statut, "Inconnu");
						}
						j2->perso1->pv -= deg;
						effet_clignotement(* j1, * j2, j2->perso1);
						SDL_afficher_degat(* j1, * j2, j2->perso1, deg, 'n');
						printf("%s touché.\nPoint de vie : %i\n", j2->perso1->nom, j2->perso1->pv);
					break;
					case '4' :
						deg = temp->sorts[3]->degat;

						if(strcmp(j2->perso2->statut, "Armure") == 0){
							deg /= 2;
							strcpy(j2->perso2->statut, "Inconnu");
						}
						j2->perso2->pv -= deg;
						effet_clignotement(* j1, * j2, j2->perso2);
						SDL_afficher_degat(* j1, * j2, j2->perso2, deg, 'n');
						printf("%s touché.\nPoint de vie : %i\n", j2->perso2->nom, j2->perso2->pv);
					break;
				}
			}
			if(j1->numJoueur == 2){
				switch (map[i][j]){
					case '1' :
						deg = temp->sorts[3]->degat;

						if(strcmp(j2->perso1->statut, "Armure") == 0){
							deg /= 2;
							strcpy(j2->perso1->statut, "Inconnu");
						}
						j2->perso1->pv -= deg;
						effet_clignotement(* j1, * j2, j2->perso1);
						SDL_afficher_degat(* j1, * j2, j2->perso1, deg, 'n');
						printf("%s touché.\nPoint de vie : %i\n", j2->perso1->nom, j2->perso1->pv);
					break;
					case '3' :
						deg = temp->sorts[3]->degat;

						if(strcmp(j2->perso2->statut, "Armure") == 0){
							deg /= 2;
							strcpy(j2->perso2->statut, "Inconnu");
						}
						j2->perso2->pv -= deg;
						effet_clignotement(* j1, * j2, j2->perso2);
						SDL_afficher_degat(* j1, * j2, j2->perso2, deg, 'n');
						printf("%s touché.\nPoint de vie : %i\n", j2->perso2->nom, j2->perso2->pv);
					break;
				}
			}
		}
	}
	if(j1->numJoueur == 1)
		SDL_afficher_map(map, * j1, * j2);
	else
		SDL_afficher_map(map, * j2, * j1);
	SDL_RenderPresent(renderer);
}

/*******************************
*		  Sorts du Tank 	   *
********************************/

/**
 *\fn SDL_armure (t_joueur * j1, t_joueur * j2, int numero_perso)
 *\brief fonction armure
 *\param j1 joueur actif 
 *\param j2 joueur passif
 *\param numero_perso numéro du personnage qui souhaite effectuer le sort
 *\return void
 * La fonction permet de réduire les dégâts subits
*/
void SDL_armure (t_joueur * j1, t_joueur * j2,int numero_perso){

	t_personnage * temp;
	
	if(numero_perso == 1 )
      temp = j1->perso1;
  	else
      temp = j1->perso2;
  	creer_effet(temp,1,temp->coord.x,temp->coord.y);
  	effet_clignotement(* j1, * j2, temp);
  	strcpy(temp->statut, "");
  	strcpy(temp->statut, "Armure");

  	if(j1->numJoueur == 1)
    	maj(map,j1,j2);
    else
    	maj(map,j2,j1);

  	if(j1->numJoueur == 1)
		SDL_afficher_map(map, * j1, * j2);
	else
		SDL_afficher_map(map, * j2, * j1);
	SDL_RenderPresent(renderer);
}

/**
 *\fn SDL_attire (t_joueur * j1, t_joueur * j2, int numero_perso)
 *\brief fonction attire
 *\param j1 joueur actif 
 *\param j2 joueur passif
 *\param numero_perso numéro du personnage qui souhaite effectuer le sort
 *\return void
 * La fonction permet d'attirer le personnage adverse
*/
void SDL_attire(t_joueur * j1, t_joueur * j2,int numero_perso){

	/*matrice de portée*/
  	char point[N][N];
	int i, j, g;
	char choix, player;

	t_personnage * temp;
	if( j1->numJoueur == 1){
      if(numero_perso == 1){
      	temp = j1->perso1;
      	player ='1';
      }
      else{
      	temp = j1->perso2;
      	player = '3';
      }
    }
    else{
      if(numero_perso == 1){
      	temp = j1->perso1;
      	player ='2';
      }
      else{
      	temp = j1->perso2;
      	player = '4';
      }
    }
  	int dist = temp->sorts[1]->portee, coordi = 0, coordj = 0;
    /*copie plan jeu dans la matrice point*/
	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++)
			point[i][j] = map[i][j];
	}
	i = temp->coord.y + 1;
	j = temp->coord.x ;
	int nb = 0;

	for(; i < N && nb < dist && point[i][j] != 'o'; i++, nb++){
		if(point[i][j] == '.')
			point[i][j] = 'A';
		else if(point[i][j] != player){
			switch(point[i][j]){
				case '1':
					point[i][j] = 'a';
				break;
				case '2':
					point[i][j] = 'b';
				break;
				case '3':
					point[i][j] = 'c';
				break;
				case '4':
					point[i][j] = 'd';
				break;
			}
		}
	}
	i = temp->coord.y - 1;
	j = temp->coord.x;
	nb = 0;

	for(; i >= 0  && nb < dist && point[i][j] != 'o'; i--, nb++){
		if(point[i][j] == '.')
			point[i][j] = 'A';
		else if(point[i][j] != player){
			switch(point[i][j]){
				case '1':
					point[i][j] = 'a';
				break;
				case '2':
					point[i][j] = 'b';
				break;
				case '3':
					point[i][j] = 'c';
				break;
				case '4':
					point[i][j] = 'd';
				break;
			}
		}
	}
	i = temp->coord.y;
	j = temp->coord.x + 1;
	nb = 0;

	for(; j < N && nb < dist && point[i][j] != 'o'; j++, nb++){
		if(point[i][j] == '.')
			point[i][j] = 'A';
		else if(point[i][j] != player){
			switch(point[i][j]){
				case '1':
					point[i][j] = 'a';
				break;
				case '2':
					point[i][j] = 'b';
				break;
				case '3':
					point[i][j] = 'c';
				break;
				case '4':
					point[i][j] = 'd';
				break;
			}
		}
	}
	i = temp->coord.y;
	j = temp->coord.x - 1;
	nb = 0;

	for(; j >= 0 && nb < dist && point[i][j] != 'o'; j--, nb++){
		if(point[i][j] == '.')
			point[i][j] = 'A';
		else if(point[i][j] != player){
			switch(point[i][j]){
				case '1':
					point[i][j] = 'a';
				break;
				case '2':
					point[i][j] = 'b';
				break;
				case '3':
					point[i][j] = 'c';
				break;
				case '4':
					point[i][j] = 'd';
				break;
			}
		}
	}
	/* affichage de la carte avec le choix des cases atteignables par le personnage perso1*/
	SDL_RenderClear(renderer);
	if(j1->numJoueur == 1)
		SDL_afficher_map(point, * j1, * j2);
	else
		SDL_afficher_map(point, * j2, * j1);
	SDL_RenderPresent(renderer);

	int x = 0, y = 0;
	/*Attente du choix de l'utilisateur*/
	do{
		SDL_Event e;
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_MOUSEBUTTONDOWN){
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

	if(j1->numJoueur==1){
		switch (map[coordi][coordj]){
			case '2' :
				effet_clignotement(* j1, * j2, j2->perso1);
		        /* Si l'écart entre les 2 joueurs est de plus de 3 cases */
		        if(abs(temp->coord.x - j2->perso1->coord.x > 3)){
		          if(temp->coord.x < j2->perso1->coord.x )
		              j2->perso1->coord.x -= 3;
		          else if(temp->coord.x > j2->perso1->coord.x )
		              j2->perso1->coord.x += 3;
		        }
		        /* Sinon l'attire a son corps à corps du nb de cases d'écart*/
		        else{
		          if(temp->coord.x < j2->perso1->coord.x )
		            j2->perso1->coord.x = temp->coord.x + 1;
		          else if(temp->coord.x > j2->perso1->coord.x )
		            j2->perso1->coord.x = temp->coord.x - 1;
		        }
		  		/* Si l'écart entre les 2 joueurs est de plus de 3 cases */
		        if(abs(temp->coord.y - j2->perso1->coord.y > 3)){
		          if(temp->coord.y < j2->perso1->coord.y )
		              j2->perso1->coord.y -= 3;
		          else if(temp->coord.y > j2->perso1->coord.y )
		              j2->perso1->coord.y += 3;
		        }
		        /* Sinon l'attire a son corps à corps du nb de cases d'écart*/
		        else{
		          if(temp->coord.y < j2->perso1->coord.y )
		            j2->perso1->coord.y = temp->coord.y + 1;
		          else if(temp->coord.y > j2->perso1->coord.y )
		            j2->perso1->coord.y = temp->coord.y - 1;
		        }
			break;
		 	case '4' :
		 		effet_clignotement(* j1, * j2, j2->perso2);
		        /* Si l'écart entre les 2 joueurs est de plus de 3 cases */
		        if(abs(temp->coord.x - j2->perso2->coord.x > 3)){
		          if(temp->coord.x < j2->perso2->coord.x )
		              j2->perso2->coord.x -= 3;
		          else if(temp->coord.x > j2->perso2->coord.x )
		              j2->perso2->coord.x += 3;
		        }
		        /* Sinon l'attire a son corps à corps du nb de cases d'écart*/
		        else{
		          if(temp->coord.x < j2->perso2->coord.x )
		            j2->perso2->coord.x = temp->coord.x + 1;
		          else if(temp->coord.x > j2->perso2->coord.x )
		            j2->perso2->coord.x = temp->coord.x - 1;
		        }
		        /* Si l'écart entre les 2 joueurs est de plus de 3 cases */
		        if(abs(temp->coord.y - j2->perso2->coord.y > 3)){
		          if(temp->coord.y < j2->perso2->coord.y )
		              j2->perso2->coord.y -= 3;
		          else if(temp->coord.y > j2->perso2->coord.y )
		              j2->perso2->coord.y += 3;
		        }
		        /* Sinon l'attire a son corps à corps du nb de cases d'écart*/
		        else{
		          if(temp->coord.y < j2->perso2->coord.y )
		            j2->perso2->coord.y = temp->coord.y + 1;
		          else if(temp->coord.y > j2->perso2->coord.y )
		            j2->perso2->coord.y = temp->coord.y - 1;
		        }
    		break;
		}
	}
	if(j1->numJoueur==2){
		switch (map[coordi][coordj]){
        	case '1' :
 				effet_clignotement(* j1, * j2, j2->perso1);
		        /* Si l'écart entre les 2 joueurs est de plus de 3 cases */
		        if(abs(temp->coord.x - j2->perso1->coord.x > 3)){
		          if(temp->coord.x < j2->perso1->coord.x )
		              j2->perso1->coord.x -= 3;
		          else if(temp->coord.x > j2->perso1->coord.x )
		              j2->perso1->coord.x += 3;
		        }
		        /* Sinon l'attire a son corps à corps du nb de cases d'écart*/
		        else{
		          if(temp->coord.x < j2->perso1->coord.x )
		            j2->perso1->coord.x = temp->coord.x + 1;
		          else if(temp->coord.x > j2->perso1->coord.x )
		            j2->perso1->coord.x = temp->coord.x - 1;
		        }
		        /* Si l'écart entre les 2 joueurs est de plus de 3 cases */
		        if(abs(temp->coord.y - j2->perso1->coord.y > 3)){
		          if(temp->coord.y < j2->perso1->coord.y )
		              j2->perso1->coord.y -= 3;
		          else if(temp->coord.y > j2->perso1->coord.y )
		              j2->perso1->coord.y += 3;
		        }
		        /* Sinon l'attire a son corps à corps du nb de cases d'écart*/
		        else{
		          if(temp->coord.y < j2->perso1->coord.y )
		            j2->perso1->coord.y = temp->coord.y + 1;
		          else if(temp->coord.y > j2->perso1->coord.y )
		            j2->perso1->coord.y = temp->coord.y - 1;
		        }
      		break;
			case '3' :
				effet_clignotement(* j1, * j2, j2->perso2);
		        /* Si l'écart entre les 2 joueurs est de plus de 3 cases */
		          if(abs(temp->coord.x - j2->perso2->coord.x > 3)){
		            if(temp->coord.x < j2->perso2->coord.x )
		                j2->perso2->coord.x -= 3;
		            else if(temp->coord.x > j2->perso2->coord.x )
		                j2->perso2->coord.x += 3;
		          }
		          /* Sinon l'attire a son corps à corps du nb de cases d'écart*/
		          else{
		            if(temp->coord.x < j2->perso2->coord.x )
		              j2->perso2->coord.x = temp->coord.x + 1;
		            else if(temp->coord.x > j2->perso2->coord.x )
		              j2->perso2->coord.x = temp->coord.x - 1;
		          }
		          /* Si l'écart entre les 2 joueurs est de plus de 3 cases */
		          if(abs(temp->coord.y - j2->perso2->coord.y > 3)){
		            if(temp->coord.y < j2->perso2->coord.y )
		                j2->perso2->coord.y -= 3;
		            else if(temp->coord.y > j2->perso2->coord.y )
		                j2->perso2->coord.y += 3;
		          }
		          /* Sinon l'attire a son corps à corps du nb de cases d'écart*/
		          else{
		            if(temp->coord.y < j2->perso2->coord.y )
		              j2->perso2->coord.y = temp->coord.y + 1;
		            else if(temp->coord.y > j2->perso2->coord.y )
		              j2->perso2->coord.y = temp->coord.y - 1;
		          }
        	break;
    	}
	}
	if(j1->numJoueur==1){
		maj(map,j1,j2);
		SDL_RenderClear(renderer);
		SDL_afficher_map(map, * j1, * j2);
		SDL_RenderPresent(renderer);
	}
	else{
		maj(map,j2,j1);
		SDL_RenderClear(renderer);
		SDL_afficher_map(map, * j2, * j2);
		SDL_RenderPresent(renderer);
	}
}

/**
 *\fn SDL_chenchen (t_joueur * j1, t_joueur * j2, int numero_perso)
 *\brief fonction chenchen
 *\param j1 joueur actif 
 *\param j2 joueur passif
 *\param numero_perso numéro du personnage qui souhaite effectuer le sort
 *\return void
 * La fonction permet d'affliger des dégats au personnage adverse
*/
void SDL_chenchen (t_joueur * j1, t_joueur * j2, int numero_perso){

	/*matrice de portée*/
	char point[N][N];
	int i, j, g;
	char choix, player;

	t_personnage * temp;
	if( j1->numJoueur == 1){
      if(numero_perso == 1){
      	temp = j1->perso1;
      	player ='1';
      }
      else{
      	temp = j1->perso2;
      	player = '3';
      }
    }
    else{
      if(numero_perso == 1){
      	temp = j1->perso1;
      	player ='2';
      }
      else{
      	temp = j1->perso2;
      	player = '4';
      }
    }
  	int dist = temp->sorts[2]->portee, coordi = 0, coordj = 0;
	/* recopie matrice dans la matrice point qui affiche la portee */
	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++)
			point[i][j] = map[i][j];
	}
	i = temp->coord.y;
	j = temp->coord.x - dist ;
	/*affiche la porter vers le haut*/
	for( ; i >= 0  && ( dist >= 0 ) ; i--, dist--){
		j = temp->coord.x - dist ;
		for( ;(j <= temp->coord.x + dist) ; j++){
			if(j>=0){
				if(j<N){
					if(point[i][j] == '.' )
							point[i][j] = 'A';
					else if(point[i][j] != player){
						switch(point[i][j]){
							case '1':
								point[i][j] = 'a';
							break;
							case '2':
								point[i][j] = 'b';
							break;
							case '3':
								point[i][j] = 'c';
							break;
							case '4':
								point[i][j] = 'd';
							break;
						}
					}
				}
			}
		}
	}
	dist = temp->sorts[2]->portee;
	g = temp->coord.y;
	j = temp->coord.x - dist ;
	/*affiche la porter vers le bas*/
	for( ; g < N  && ( dist >= 0 ) ; g++, dist--){
		j = temp->coord.x - dist ;
		for( ;(j <= temp->coord.x + dist) ; j++){
			if(j>=0){
				if(j<N){
					if(g != temp->coord.y){
						/*ne pas repasser une seconde fois sur la ligne du joueur*/
						if(point[g][j] == '.')
								point[g][j] = 'A';
						else if(point[g][j] != player){
							switch(point[g][j]){
								case '1':
									point[g][j] = 'a';
								break;
								case '2':
									point[g][j] = 'b';
								break;
								case '3':
									point[g][j] = 'c';
								break;
								case '4':
									point[g][j] = 'd';
								break;
							}
						}
					}
				}
			}
		}
	}
	/* affichage de la carte avec le choix des cases atteignables par le personnage perso1*/
	SDL_RenderClear(renderer);
	if(j1->numJoueur == 1)
		SDL_afficher_map(point, * j1, * j2);
	else
		SDL_afficher_map(point, * j2, * j1);
	SDL_RenderPresent(renderer);

	int x = 0, y = 0;
	/*Attente du choix de l'utilisateur*/
	do{
		SDL_Event e;
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_MOUSEBUTTONDOWN){
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

  	int deg;
  	/*réduction des points de vies après le coup*/
	if(j1->numJoueur==1){
		switch (map[coordi][coordj]){
			case '2' : 
				deg = temp->sorts[2]->degat;

				if(strcmp(j2->perso1->statut, "Armure") == 0){
					deg /= 2;
					strcpy(j2->perso1->statut, "Inconnu");
				}
				j2->perso1->pv -= deg;
				effet_clignotement(* j1, * j2, j2->perso1);
				SDL_afficher_degat(* j1, * j2, j2->perso1, deg, 'n');
				printf("%s touché.\nPoint de vie : %i\n", j2->perso1->nom, j2->perso1->pv);
			break;
			case '4' :
				deg = temp->sorts[2]->degat;

				if(strcmp(j2->perso2->statut, "Armure") == 0){
					deg /= 2;
					strcpy(j2->perso2->statut, "Inconnu");
				}
				j2->perso2->pv -= deg;
				effet_clignotement(* j1, * j2, j2->perso2);
				SDL_afficher_degat(* j1, * j2, j2->perso2, deg, 'n');
				printf("%s touché.\nPoint de vie : %i\n", j2->perso2->nom, j2->perso2->pv);
			 break;
		}
	}
	if(j1->numJoueur==2){
		switch (map[coordi][coordj]){
			case '1' :
				deg = temp->sorts[2]->degat;

				if(strcmp(j2->perso1->statut, "Armure") == 0){
					deg /= 2;
					strcpy(j2->perso1->statut, "Inconnu");
				}
				j2->perso1->pv -= deg;
				effet_clignotement(* j1, * j2, j2->perso1);
				SDL_afficher_degat(* j1, * j2, j2->perso1, deg, 'n');
				printf("%s touché.\nPoint de vie : %i\n", j2->perso1->nom, j2->perso1->pv);
			break;
			case '3' :
				deg = temp->sorts[2]->degat;

				if(strcmp(j2->perso2->statut, "Armure") == 0){
					deg /= 2;
					strcpy(j2->perso2->statut, "Inconnu");
				}
				j2->perso2->pv -= deg;
				effet_clignotement(* j1, * j2, j2->perso2);
				SDL_afficher_degat(* j1, * j2, j2->perso2, deg, 'n');
				printf("%s touché.\nPoint de vie : %i\n", j2->perso2->nom, j2->perso2->pv);
		 	break;
		}
	}
	if(j1->numJoueur == 1)
		SDL_afficher_map(map, * j1, * j2);
	else
		SDL_afficher_map(map, * j2, * j1);
	SDL_RenderPresent(renderer);
}

/**
 *\fn SDL_bigshaq (t_joueur * j1, t_joueur * j2, int numero_perso)
 *\brief fonction bigshaq
 *\param j1 joueur actif 
 *\param j2 joueur passif
 *\param numero_perso numéro du personnage qui souhaite effectuer le sort
 *\return void
 * La fonction permet d'affliger des dégats au personnage adverse
*/
void SDL_bigshaq (t_joueur * j1, t_joueur * j2,int numero_perso){

	/*matrice de portée*/
	char point[N][N];
	int i, j, g;
	char choix, player;

	t_personnage * temp;
	if( j1->numJoueur == 1){
      if(numero_perso == 1){
      	temp = j1->perso1;
      	player ='1';
      }
      else{
      	temp = j1->perso2;
      	player = '3';
      }
    }
    else{
      if(numero_perso == 1){
      	temp = j1->perso1;
      	player ='2';
      }
      else{
      	temp = j1->perso2;
      	player = '4';
      }
    }
  	int dist = temp->sorts[3]->portee, coordi = 0, coordj = 0;
	int degats[4]={8,16,32,128};
	/*copie plan jeu dans la matrice point*/
	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++)
			point[i][j] = map[i][j];
	}
	i = temp->coord.y + 1;
	j = temp->coord.x ;
	int nb = 0;

	for(; i < N && nb < dist && point[i][j] != 'o'; i++, nb++){
		if(point[i][j] == '.')
			point[i][j] = 'A';
		else if(point[i][j] != player){
			switch(point[i][j]){
				case '1':
					point[i][j] = 'a';
				break;
				case '2':
					point[i][j] = 'b';
				break;
				case '3':
					point[i][j] = 'c';
				break;
				case '4':
					point[i][j] = 'd';
				break;
			}
		}
	}
	i = temp->coord.y - 1;
	j = temp->coord.x;
	nb = 0;

	for(; i >= 0  && nb < dist && point[i][j] != 'o'; i--, nb++){
		if(point[i][j] == '.')
			point[i][j] = 'A';
		else if(point[i][j] != player){
			switch(point[i][j]){
				case '1':
					point[i][j] = 'a';
				break;
				case '2':
					point[i][j] = 'b';
				break;
				case '3':
					point[i][j] = 'c';
				break;
				case '4':
					point[i][j] = 'd';
				break;
			}
		}
	}
	i = temp->coord.y;
	j = temp->coord.x + 1;
	nb = 0;

	for(; j < N && nb < dist && point[i][j] != 'o'; j++, nb++){
		if(point[i][j] == '.')
			point[i][j] = 'A';
		else if(point[i][j] != player){
			switch(point[i][j]){
				case '1':
					point[i][j] = 'a';
				break;
				case '2':
					point[i][j] = 'b';
				break;
				case '3':
					point[i][j] = 'c';
				break;
				case '4':
					point[i][j] = 'd';
				break;
			}
		}
	}
	i = temp->coord.y;
	j = temp->coord.x - 1;
	nb = 0;

	for(; j >= 0 && nb < dist && point[i][j] != 'o'; j--, nb++){
		if(point[i][j] == '.')
			point[i][j] = 'A';
		else if(point[i][j] != player){
			switch(point[i][j]){
				case '1':
					point[i][j] = 'a';
				break;
				case '2':
					point[i][j] = 'b';
				break;
				case '3':
					point[i][j] = 'c';
				break;
				case '4':
					point[i][j] = 'd';
				break;
			}
		}
	}
	/* affichage de la carte avec le choix des cases atteignables par le personnage perso1*/
	SDL_RenderClear(renderer);
	if(j1->numJoueur == 1)
		SDL_afficher_map(point, * j1, * j2);
	else
		SDL_afficher_map(point, * j2, * j1);
	SDL_RenderPresent(renderer);

	int x = 0, y = 0;
	/*Attente du choix de l'utilisateur*/
	do{
		SDL_Event e;
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_MOUSEBUTTONDOWN){
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

	int deg;
    /*réduction des points de vies après le coup*/
    if(j1->numJoueur==1){
  		switch (map[coordi][coordj]){
  			case '2' :
          		if(temp->pv > 50){
          			deg = degats[0];

          			if(strcmp(j2->perso1->statut, "Armure") == 0){
						deg /= 2;
						strcpy(j2->perso1->statut, "Inconnu");
					}
            		j2->perso1->pv -= deg;
            	}
          		else if(temp->pv > 25){
            		deg = degats[1];

          			if(strcmp(j2->perso1->statut, "Armure") == 0){
						deg /= 2;
						strcpy(j2->perso1->statut, "Inconnu");
					}
            		j2->perso1->pv -= deg;
          		}
          		else if(temp->pv > 2){
            		deg = degats[2];

          			if(strcmp(j2->perso1->statut, "Armure") == 0){
						deg /= 2;
						strcpy(j2->perso1->statut, "Inconnu");
					}
            		j2->perso1->pv -= deg;
            	}	
          		else if(temp->pv == 1){
            		deg = degats[3];

          			if(strcmp(j2->perso1->statut, "Armure") == 0){
						deg /= 2;
						strcpy(j2->perso1->statut, "Inconnu");
					}
            		j2->perso1->pv -= deg;
          		}
          		effet_clignotement(* j1, * j2, j2->perso1);
				SDL_afficher_degat(* j1, * j2, j2->perso1, deg, 'n');
				printf("%s touché.\nPoint de vie : %i\n", j2->perso1->nom, j2->perso1->pv);
  			break;
  		 	case '4' :
          		if(temp->pv > 50){
            		deg = degats[0];

          			if(strcmp(j2->perso2->statut, "Armure") == 0){
						deg /= 2;
						strcpy(j2->perso2->statut, "Inconnu");
					}
            		j2->perso2->pv -= deg;
          		}
          		else if(temp->pv > 25){
            		deg = degats[1];

          			if(strcmp(j2->perso2->statut, "Armure") == 0){
						deg /= 2;
						strcpy(j2->perso2->statut, "Inconnu");
					}
            		j2->perso2->pv -= deg;
          		}
          		else if(temp->pv > 2){
            		deg = degats[2];

          			if(strcmp(j2->perso2->statut, "Armure") == 0){
						deg /= 2;
						strcpy(j2->perso2->statut, "Inconnu");
					}
            		j2->perso2->pv -= deg;
          		}
          		else if(temp->pv == 1){
            		deg = degats[3];

          			if(strcmp(j2->perso2->statut, "Armure") == 0){
						deg /= 2;
						strcpy(j2->perso2->statut, "Inconnu");
					}
            		j2->perso2->pv -= deg;
          		}
				effet_clignotement(* j1, * j2, j2->perso2);
				SDL_afficher_degat(* j1, * j2, j2->perso2, deg, 'n');
				printf("%s touché.\nPoint de vie : %i\n", j2->perso2->nom, j2->perso2->pv);
          break;
        }
  	}
    if(j1->numJoueur==2){
    	switch (map[coordi][coordj]){
        case '1' :
         	if(temp->pv > 50){
      			deg = degats[0];

      			if(strcmp(j2->perso1->statut, "Armure") == 0){
					deg /= 2;
					strcpy(j2->perso1->statut, "Inconnu");
				}
        		j2->perso1->pv -= deg;
        	}
      		else if(temp->pv > 25){
        		deg = degats[1];

      			if(strcmp(j2->perso1->statut, "Armure") == 0){
					deg /= 2;
					strcpy(j2->perso1->statut, "Inconnu");
				}
        		j2->perso1->pv -= deg;
      		}
      		else if(temp->pv > 2){
        		deg = degats[2];

      			if(strcmp(j2->perso1->statut, "Armure") == 0){
					deg /= 2;
					strcpy(j2->perso1->statut, "Inconnu");
				}
        		j2->perso1->pv -= deg;
        	}	
      		else if(temp->pv == 1){
        		deg = degats[3];

      			if(strcmp(j2->perso1->statut, "Armure") == 0){
					deg /= 2;
					strcpy(j2->perso1->statut, "Inconnu");
				}
        		j2->perso1->pv -= deg;
      		}
      		effet_clignotement(* j1, * j2, j2->perso1);
			SDL_afficher_degat(* j1, * j2, j2->perso1, deg, 'n');
			printf("%s touché.\nPoint de vie : %i\n", j2->perso1->nom, j2->perso1->pv);
    	break;
    	case '3' :
          	if(temp->pv > 50){
        		deg = degats[0];

      			if(strcmp(j2->perso2->statut, "Armure") == 0){
					deg /= 2;
					strcpy(j2->perso2->statut, "Inconnu");
				}
        		j2->perso2->pv -= deg;
      		}
      		else if(temp->pv > 25){
        		deg = degats[1];

      			if(strcmp(j2->perso2->statut, "Armure") == 0){
					deg /= 2;
					strcpy(j2->perso2->statut, "Inconnu");
				}
        		j2->perso2->pv -= deg;
      		}
      		else if(temp->pv > 2){
        		deg = degats[2];

      			if(strcmp(j2->perso2->statut, "Armure") == 0){
					deg /= 2;
					strcpy(j2->perso2->statut, "Inconnu");
				}
        		j2->perso2->pv -= deg;
      		}
      		else if(temp->pv == 1){
        		deg = degats[3];

      			if(strcmp(j2->perso2->statut, "Armure") == 0){
					deg /= 2;
					strcpy(j2->perso2->statut, "Inconnu");
				}
        		j2->perso2->pv -= deg;
      		}
			effet_clignotement(* j1, * j2, j2->perso2);
			SDL_afficher_degat(* j1, * j2, j2->perso2, deg, 'n');
			printf("%s touché.\nPoint de vie : %i\n", j2->perso2->nom, j2->perso2->pv);
        break;
      }
  	}

  	if(j1->numJoueur == 1)
		SDL_afficher_map(map, * j1, * j2);
	else
		SDL_afficher_map(map, * j2, * j1);
	SDL_RenderPresent(renderer);
}

/*******************************
*		  Sorts du Druide 	   *
********************************/

/**
 *\fn SDL_minotaure (t_joueur * j1, t_joueur * j2, int numero_perso)
 *\brief fonction transformation en minotaure
 *\param j1 joueur actif 
 *\param j2 joueur passif
 *\param numero_perso numéro du personnage qui souhaite effectuer le sort
 *\return void
 * La fonction permet de se transformer en minotaure : augmentation des points d'action
*/
void SDL_minotaure (t_joueur * j1, t_joueur * j2,int numero_perso){

	t_personnage * temp;
	if(numero_perso == 1 )
      temp = j1->perso1;
  	else
      temp = j1->perso2;
  	creer_effet(temp,2,temp->coord.x,temp->coord.y);
  	effet_clignotement(* j1, * j2, temp);
  	strcpy(temp->statut, "");
  	strcpy(temp->statut, "Minotaure");

  	if(j1->numJoueur == 1)
    	maj(map,j1,j2);
  	else 
    	maj(map,j2,j1);
	if(j1->numJoueur == 1)
		SDL_afficher_map(map, * j1, * j2);
	else
		SDL_afficher_map(map, * j2, * j1);
	SDL_RenderPresent(renderer);
}

/**
 *\fn SDL_felin (t_joueur * j1, t_joueur * j2, int numero_perso)
 *\brief fonction transformation en félin
 *\param j1 joueur actif 
 *\param j2 joueur passif
 *\param numero_perso numéro du personnage qui souhaite effectuer le sort
 *\return void
 * La fonction permet de se transformer en félin : augmentation des points de mouvement
*/
void SDL_felin (t_joueur * j1, t_joueur * j2,int numero_perso){

	t_personnage * temp;
	if(numero_perso == 1 )
      temp = j1->perso1;
  	else
      temp = j1->perso2;
  	creer_effet(temp,3,temp->coord.x,temp->coord.y);
	effet_clignotement(* j1, * j2, temp);
	strcpy(temp->statut, "");
	strcpy(temp->statut, "Felin");

  	if(j1->numJoueur == 1)
    	maj(map,j1,j2);
  	else
    	maj(map,j2,j1);
  	if(j1->numJoueur == 1)
		SDL_afficher_map(map, * j1, * j2);
	else
		SDL_afficher_map(map, * j2, * j1);

	SDL_RenderPresent(renderer);
}

/**
 *\fn SDL_fuego (t_joueur * j1, t_joueur * j2, int numero_perso)
 *\brief fonction fuego
 *\param j1 joueur actif 
 *\param j2 joueur passif
 *\param numero_perso numéro du personnage qui souhaite effectuer le sort
 *\return void
 * La fonction permet d'affliger des dégats au personnage adverse
*/
void SDL_fuego(t_joueur * j1, t_joueur * j2,int numero_perso){

	/*matrice de portée*/
	char point[N][N];
	int i, j, g;
	char choix, player;

	t_personnage * temp;
	if( j1->numJoueur == 1){
      if(numero_perso == 1){
      	temp = j1->perso1;
      	player ='1';
      }
      else{
      	temp = j1->perso2;
      	player = '3';
      }
    }
    else{
      if(numero_perso == 1){
      	temp = j1->perso1;
      	player ='2';
      }
      else{
      	temp = j1->perso2;
      	player = '4';
      }
    }
  	int dist = temp->sorts[2]->portee, coordi = 0, coordj = 0;
	/* recopie matrice dans la matrice point qui affiche la portee */
	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++)
			point[i][j] = map[i][j];
	}
	i = temp->coord.y;
	j = temp->coord.x - dist ;
	/*affiche la portée vers le haut*/
	for( ; i >= 0  && ( dist >= 0 ) ; i--, dist--){
		j = temp->coord.x - dist ;
		for( ;(j <= temp->coord.x + dist) ; j++){
			if(j>=0){
				if(j<N){
					if(point[i][j] == '.' )
							point[i][j] = 'A';
					else if(point[i][j] != player){
						switch(point[i][j]){
							case '1':
								point[i][j] = 'a';
							break;
							case '2':
								point[i][j] = 'b';
							break;
							case '3':
								point[i][j] = 'c';
							break;
							case '4':
								point[i][j] = 'd';
							break;
						}
					}
				}
			}
		}
	}
	dist=temp->sorts[2]->portee;
	g = temp->coord.y;
	j = temp->coord.x - dist ;
	/*affiche la portée vers le bas*/
	for( ; g < N  && ( dist >= 0 ) ; g++, dist--){
		j = temp->coord.x - dist ;
		for( ;(j <= temp->coord.x + dist) ; j++){
			if(j>=0){
				if(j<N){
					if(g != temp->coord.y){
						/*ne pas repasser une seconde fois sur la ligne du joueur*/
						if(point[g][j] == '.')
								point[g][j] = 'A';
						else if(point[g][j] != player){
							switch(point[g][j]){
								case '1':
									point[g][j] = 'a';
								break;
								case '2':
									point[g][j] = 'b';
								break;
								case '3':
									point[g][j] = 'c';
								break;
								case '4':
									point[g][j] = 'd';
								break;
							}
						}
					}
				}
			}
		}
	}
	/* affichage de la carte avec le choix des cases atteignables par le personnage perso1*/
	SDL_RenderClear(renderer);
	if(j1->numJoueur == 1)
		SDL_afficher_map(point, * j1, * j2);
	else
		SDL_afficher_map(point, * j2, * j1);
	SDL_RenderPresent(renderer);

	int x = 0, y = 0;
	/*Attente du choix de l'utilisateur*/
	do{
		SDL_Event e;
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_MOUSEBUTTONDOWN){
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

  	int deg;
  	/*réduction des points de vies après le coup*/
	if(j1->numJoueur==1){
		switch (map[coordi][coordj]){
			case '2' : 
				deg = temp->sorts[2]->degat;

				if(strcmp(j2->perso1->statut, "Armure") == 0){
					deg /= 2;
					strcpy(j2->perso1->statut, "Inconnu");
				}
				j2->perso1->pv -= deg;
				effet_clignotement(* j1, * j2, j2->perso1);
				SDL_afficher_degat(* j1, * j2, j2->perso1, deg, 'n');
				printf("%s touché.\nPoint de vie : %i\n", j2->perso1->nom, j2->perso1->pv);
			break;
			case '4' :
				deg = temp->sorts[2]->degat;

				if(strcmp(j2->perso2->statut, "Armure") == 0){
					deg /= 2;
					strcpy(j2->perso2->statut, "Inconnu");
				}
				j2->perso2->pv -= deg;
				effet_clignotement(* j1, * j2, j2->perso2);
				SDL_afficher_degat(* j1, * j2, j2->perso2, deg, 'n');
				printf("%s touché.\nPoint de vie : %i\n", j2->perso2->nom, j2->perso2->pv);
			 break;
		}
	}
	if(j1->numJoueur==2){
		switch (map[coordi][coordj]){
			case '1' :
				deg = temp->sorts[2]->degat;

				if(strcmp(j2->perso1->statut, "Armure") == 0){
					deg /= 2;
					strcpy(j2->perso1->statut, "Inconnu");
				}
				j2->perso1->pv -= deg;
				effet_clignotement(* j1, * j2, j2->perso1);
				SDL_afficher_degat(* j1, * j2, j2->perso1, deg, 'n');
				printf("%s touché.\nPoint de vie : %i\n", j2->perso1->nom, j2->perso1->pv);
			break;
			case '3' :
				deg = temp->sorts[2]->degat;

				if(strcmp(j2->perso2->statut, "Armure") == 0){
					deg /= 2;
					strcpy(j2->perso2->statut, "Inconnu");
				}
				j2->perso2->pv -= deg;
				effet_clignotement(* j1, * j2, j2->perso2);
				SDL_afficher_degat(* j1, * j2, j2->perso2, deg, 'n');
				printf("%s touché.\nPoint de vie : %i\n", j2->perso2->nom, j2->perso2->pv);
		 	break;
		}
	}
	if(j1->numJoueur == 1)
		SDL_afficher_map(map, * j1, * j2);
	else
		SDL_afficher_map(map, * j2, * j1);
	SDL_RenderPresent(renderer);
}

/**
 *\fn SDL_felin (t_joueur * j1, t_joueur * j2, int numero_perso)
 *\brief fonction revitalisation
 *\param j1 joueur actif 
 *\param j2 joueur passif
 *\param numero_perso numéro du personnage qui souhaite effectuer le sort
 *\return void
 * La fonction permet de soigner un coéquipier
*/
void SDL_revitalisation (t_joueur * j1, t_joueur * j2,int numero_perso){

	/*matrice de portée*/
  	char point[N][N];
	int i, j, g;
	char choix, player;

	t_personnage * temp;
	if( j1->numJoueur == 1){
      if(numero_perso == 1){
      	temp = j1->perso1;
      	player ='1';
      }
      else{
      	temp = j1->perso2;
      	player = '3';
      }
    }
    else{
      if(numero_perso == 1){
      	temp = j1->perso1;
      	player ='2';
      }
      else{
      	temp = j1->perso2;
      	player = '4';
      }
    }
  	int dist = temp->sorts[3]->portee, coordi = 0, coordj = 0;
	/* recopie matrice dans la matrice point qui affiche la portee */
	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++)
			point[i][j] = map[i][j];
	}
	i = temp->coord.y;
	j = temp->coord.x - dist ;
	/*affiche la portée vers le haut*/
	for( ; i >= 0  && ( dist >= 0 ) ; i--, dist--){
		j = temp->coord.x - dist ;
		for( ;(j <= temp->coord.x + dist) ; j++){
			if(j>=0){
				if(j<N){
					if(point[i][j] == '.' )
							point[i][j] = 'A';
					else if(point[i][j] != player){
						switch(point[i][j]){
							case '1':
								point[i][j] = 'a';
							break;
							case '2':
								point[i][j] = 'b';
							break;
							case '3':
								point[i][j] = 'c';
							break;
							case '4':
								point[i][j] = 'd';
							break;
						}
					}
				}
			}
		}
	}
  	dist = temp->sorts[3]->portee;
	g = temp->coord.y;
	j = temp->coord.x - dist ;
	/*affiche la portée vers le bas*/
	for( ; g < N  && ( dist >= 0 ) ; g++, dist--){
		j = temp->coord.x - dist ;
		for( ;(j <= temp->coord.x + dist) ; j++){
			if(j>=0){
				if(j<N){
					if(g != temp->coord.y){
						/*ne pas repasser une seconde fois sur la ligne du j*/
						if(point[g][j] == '.')
								point[g][j] = 'A';
						else if(point[g][j] != player){
							switch(point[g][j]){
								case '1':
									point[g][j] = 'a';
								break;
								case '2':
									point[g][j] = 'b';
								break;
								case '3':
									point[g][j] = 'c';
								break;
								case '4':
									point[g][j] = 'd';
								break;
							}
						}
					}
				}
			}
		}
	}
  	/* affichage de la carte avec le choix des cases atteignables par le personnage perso1*/
	SDL_RenderClear(renderer);
	if(j1->numJoueur == 1)
		SDL_afficher_map(point, * j1, * j2);
	else
		SDL_afficher_map(point, * j2, * j1);
	SDL_RenderPresent(renderer);

	int x = 0, y = 0;
	/*Attente du choix de l'utilisateur*/
	do{
		SDL_Event e;
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_MOUSEBUTTONDOWN){
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
	/*réduction des points de vies après le coup*/
	if(j1->numJoueur==2){
		switch (map[coordi][coordj]){
			case '2' :
				j1->perso1->pv += temp->sorts[3]->degat;
				effet_clignotement(* j1, * j2, j1->perso1);
				SDL_afficher_degat(* j1, * j2, j1->perso1, temp->sorts[3]->degat, 'p');
				printf("%s soigné.\nPoint de vie : %i\n", j1->perso1->nom, j1->perso1->pv);
			break;
		 	case '4' :
				j1->perso2->pv += temp->sorts[3]->degat;
				effet_clignotement(* j1, * j2, j1->perso2);
				SDL_afficher_degat(* j1, * j2, j1->perso2, temp->sorts[3]->degat, 'p');
				printf("%s soigné.\nPoint de vie : %i\n", j1->perso2->nom, j1->perso2->pv);
	  		break;
		}
	}
	if(j1->numJoueur==1){
		switch (map[coordi][coordj]){
			case '1' :
				j1->perso1->pv += temp->sorts[3]->degat;
				effet_clignotement(* j1, * j2, j1->perso1);
				SDL_afficher_degat(* j1, * j2, j1->perso1, temp->sorts[3]->degat, 'p');
				printf("%s soigné.\nPoint de vie : %i\n", j1->perso1->nom, j1->perso1->pv);
			break;
			case '3' :
				j1->perso2->pv += temp->sorts[3]->degat;
				effet_clignotement(* j1, * j2, j1->perso2);
				SDL_afficher_degat(* j1, * j2, j1->perso2, temp->sorts[3]->degat, 'p');
				printf("%s soigné.\nPoint de vie : %i\n", j1->perso2->nom, j1->perso2->pv);
		 	break;
		}
	}
	if(j1->numJoueur == 1)
		SDL_afficher_map(map, * j1, * j2);
	else
		SDL_afficher_map(map, * j2, * j1);
	SDL_RenderPresent(renderer);
}
