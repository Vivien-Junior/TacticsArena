#include "fonc.h"
#include "sdl_fonc.h"

/*******************************
*******OUTILS POUR LA SDL*******
********************************/

/**
 *\file sdl_outils.c
 *\brief Outils pour la SDL
 *\author Vivien-Junior
 *\version 0.1
 *\date 08/04/2019
*/

/**
 *\fn void SDL_ecrire_texte(char * chaine, char * police_ec, int taille, char color, char style)
 *\brief crée les personnages choisis
 *\param chaine le texte à écrire
 *\param police la police d'écriture
 *\param taille la taille de la police
 *\param color la couleur que l'on souhaite donner au texte
 *\param style le style d'écriture du texte (Gras, Italic, Souligné ou Normal)
 *\return void
 * La fonction permet d'écrire du texte selon une police, une taille, une couleur et un style
*/
void SDL_ecrire_texte(char * chaine, char * police_ec, int taille, char color, char style){

	SDL_Surface * tex_texte;

  	if((police = TTF_OpenFont(police_ec, taille)) == NULL)
  		fprintf(stderr, "erreur chargement font\n");
  	/*Charger police selon le style : N pour normal, G pour Gras, I pour Italic et S pour souligner*/
  	switch(style){
  		case 'N':
  			TTF_SetFontStyle(police, TTF_STYLE_NORMAL);
  		break;
  		case 'G':
  			TTF_SetFontStyle(police, TTF_STYLE_BOLD);
  		break;

  		case 'I':
  			TTF_SetFontStyle(police, TTF_STYLE_ITALIC);
  		break;

  		case 'S':
  			TTF_SetFontStyle(police, TTF_STYLE_UNDERLINE);
  		break;
  	}
  	SDL_Color couleur;
  	/*Choix de la couleur du texte : b pour blanc, n pour noir, o pour orange, r pour rouge, j pour jaune et R pour rouge foncé*/
  	switch(color){
  		case 'b':
  			couleur = blanc;
  		break;
  		case 'n':
  			couleur = noir;
  		break;
  		case 'o':
  			couleur = orange;
  		break;
  		case 'r':
  			couleur = rouge;
  		break;
  		case 'j':
  			couleur = jaune;
  		break;
  		case 'R':
  			couleur = rouge2;
  		break;
  	}
  	/*Charger texte*/
  	tex_texte = TTF_RenderUTF8_Blended(police, chaine, couleur);
  	
  	if(!tex_texte)
  		fprintf(stderr, "Erreur à la création du texte : %s\n", SDL_GetError());
  	texte = SDL_CreateTextureFromSurface(renderer, tex_texte);
    SDL_FreeSurface(tex_texte);
  	
  	if(!texte)
  		fprintf(stderr, "Erreur à la création du rendu du texte : %s\n", SDL_GetError());
  	TTF_CloseFont(police);
}

/**
 *\fn void effet_clignotement(t_joueur j1, t_joueur j2, t_personnage * perso)
 *\brief effet de clignotement
 *\param j1 joueur 1
 *\param j2 joueur 2
 *\param perso personnage à faire clignoter
 *\return void
 * La fonction permet de faire clignoter un personnage
*/
void effet_clignotement(t_joueur j1, t_joueur j2, t_personnage * perso){

	char c = map[perso->coord.y][perso->coord.x];
	int i;

	for(i = 0; i < 4; i++){
		map[perso->coord.y][perso->coord.x] = '.';
		SDL_RenderClear(renderer);
		if(j1.numJoueur == 1)
			SDL_afficher_map(map, j1, j2);
		else
			SDL_afficher_map(map, j2, j1);
		SDL_RenderPresent(renderer);
		SDL_Delay(30);
		map[perso->coord.y][perso->coord.x] = c;
		SDL_RenderClear(renderer);
		if(j1.numJoueur == 1)
			SDL_afficher_map(map, j1, j2);
		else
			SDL_afficher_map(map, j2, j1);
		SDL_RenderPresent(renderer);
		SDL_Delay(30);
	}
}

/**
 *\fn t_case recherche_case(int i, int j)
 *\brief recherche de case
 *\param i coodonnée de la ligne
 *\param j coodonnée de la colonne
 *\return t_case la case correspondante
 * La fonction permet de rechercher les cordonnées d'une case sur la fenêtre en fonction des cordonnées de la même case dans la matrice
*/
t_case recherche_case(int i, int j){

	int x;
	for(x = 0; x < N * N; x++){
		if(SDL_map[x].i == i && SDL_map[x].j == j)
			return SDL_map[x];
	}
}

/**
 *\fn int acces_choix_perso(t_choix SDL_choix[], int x, int y, int * classe)
 *\brief Acces menu
 *\param SDL_choix[] tableau d'options du menu
 *\param x abscisse du clic
 *\param y ordonnée du clic
 *\param classe option choisie
 *\return int 
 * La fonction permet de vérifier que la case cliquée est bien accessible
*/
int acces_choix_perso(t_choix SDL_choix[], int x, int y, int * classe){

	for(int i = 0; SDL_choix[i].num_choix; i++){
		if(x > SDL_choix[i].b.x && y > SDL_choix[i].b.y && x < SDL_choix[i].c.x && y < SDL_choix[i].a.y){
			* classe = SDL_choix[i].num_choix;
			return 1;
		}
	}
	return 0;
}

/**
 *\fn float eq_droite(t_coord p1, t_coord p2, int x)
 *\brief Equation de droite
 *\param p1 coordonnée du premier point
 *\param p2 coordonnée du second point
 *\param x abscisse clic
 *\return float ordonné calculé
 * La fonction permet de calculer l'ordonnée d'un x donné selon l'équation de la droite passée dont les coordonées sont passés en paramètre
*/
float eq_droite(t_coord p1, t_coord p2, int x){

	float m = ((float)p2.y - (float)p1.y)/((float)p2.x - (float)p1.x);
	float p = (float)p1.y - (m * (float)p1.x);
	float y = (m * x) + p;
	return y;
}

/**
 *\fn int dans_case(t_coord a, t_coord b, t_coord c, t_coord d, int x, int y)
 *\brief Clic dans la case
 *\param p1 coordonnée du premier point
 *\param p2 coordonnée du second point
 *\param x abscisse clic
 *\return float ordonné calculé
 * La fonction permet de vérifier que le clic est bien dans un périmètre
*/
int dans_case(t_coord a, t_coord b, t_coord c, t_coord d, int x, int y){

	if((float)y >= eq_droite(a,b,x) && (float)y >= eq_droite(b,c,x) && (float)y <= eq_droite(a,d,x) && (float)y <= eq_droite(d,c,x))
		return 1;
	return 0;
}

/**
 *\fn int acces_possible(int x, int y, int * ymap, int * xmap)
 *\brief Clic dans la case
 *\param x abscisse du clic
 *\param y ordonnée du clic
 *\param xmap coordonnée colonne matrice de jeu
 *\param ymap coordonnée ligne matrice de jeu
 *\return int
 * La fonction permet de vérifier que la case est accessible
*/
int acces_possible(int x, int y, int * ymap, int * xmap){

	int i;
	for(i = 0; i < N*N; i++){
		/*Si la case est un obstacle elle est inaccessible*/
		if(SDL_map[i].type == OBSTACLE){
			if(dans_case(SDL_map[i].a, SDL_map[i].b, SDL_map[i].c, SDL_map[i].d, x, y))
				return 0;
		}
		/*Si la case est pas colorée, donc pas une portée, elle est inaccessible*/
		else if(SDL_map[i].type == CASE_GRISE){
			if(dans_case(SDL_map[i].a, SDL_map[i].b, SDL_map[i].c, SDL_map[i].d, x, y))
				return 0;
		}
		/*Si la case est colorée, donc est une portée, elle est accessible*/
		else if(SDL_map[i].type == CASE_BLEUE){
			if(dans_case(SDL_map[i].a, SDL_map[i].b, SDL_map[i].c, SDL_map[i].d, x, y)){
				* ymap = SDL_map[i].i;
				* xmap = SDL_map[i].j;
				return 1;
			}
		}
	}
	return 0;
}
