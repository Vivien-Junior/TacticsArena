#include "fonc.h"
#include "sdl_fonc.h"

/*******************************
*****FONCTIONS D'AFFICHAGES*****
********************************/

/**
 *\file sdl_affichages.c
 *\brief Fonction des sorts des personnages version SDL 
 *\author Vivien-Junior
 *\version 0.1
 *\date 08/04/2019
*/

/**
 *\fn void SDL_afficher_vainqueur(t_joueur joueur, int select)
 *\brief Affichage du vainqueur
 *\param joueur joueur ayant gagné
 *\param select option selectionnée
 *\return void
 * La fonction affiche le vainqueur et propose de quitter et rejouer
 * Select à -1 si aucune case n'est sélectionnée
*/
void SDL_afficher_vainqueur(t_joueur joueur, int select){

	int c;
	SDL_afficher_bg();
	SDL_afficher_message("Vainqueur", LARGEUR/2 - 160, 75, "Police/Freshman.ttf", 50, 'b', 'N');
	SDL_Rect reccoupe;
	reccoupe.x = LARGEUR/2 + 105;
	reccoupe.y = 65;
	reccoupe.w = 80;
	reccoupe.h = 70;

	if(!SDL_RenderCopy(renderer, coupe, NULL, &reccoupe));

	SDL_affficher_id_perso(joueur.perso1, 90, LARGEUR/2 - 105, 145, 20, joueur.numJoueur);
	SDL_affficher_id_perso(joueur.perso2, 90, LARGEUR/2 + 15, 145, 20, joueur.numJoueur);
	
	if(joueur.numJoueur == 1)
		SDL_afficher_message("Joueur1",LARGEUR/2 - 105, 275, "Police/calendarnote.ttf", 50, 'o', 'N');
	else
		SDL_afficher_message("Joueur2",LARGEUR/2 - 105, 275, "Police/calendarnote.ttf", 50, 'o', 'N');

	SDL_Rect recmenu;
	recmenu.h = 200;
	recmenu.w = 350;
	recmenu.x = (LARGEUR/2) - (recmenu.w/2);
	recmenu.y = 350;

	if(SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255));
	if(SDL_RenderDrawRect(renderer, &recmenu));

	recmenu.h = (recmenu.h/3) - 20;
	recmenu.w -= 80;
	recmenu.x += 40;
	recmenu.y += 40;

	/*Le menu s'affiche en gris, si un élément est sélectionné il s'affiche en bleu*/
	if(SDL_SetRenderDrawColor(renderer, 102, 102, 102, 255));	
	if(select == 1)
		if(SDL_SetRenderDrawColor(renderer, 0, 63, 255, 255));

	if(SDL_RenderFillRect(renderer, &recmenu));

	/*Option rejouer*/
	SDL_choix_jeu[0].num_choix = 1;
	SDL_choix_jeu[0].b.x = recmenu.x;
	SDL_choix_jeu[0].b.y = recmenu.y;
	SDL_choix_jeu[0].a.y = recmenu.y + recmenu.h;
	SDL_choix_jeu[0].c.x = recmenu.x + recmenu.w;

	SDL_afficher_message("Rejouer", recmenu.x + recmenu.w/2 - 40, recmenu.y + recmenu.h/2 - 14, "Police/bombarde.ttf", 25, 'b', 'N');
	SDL_afficher_message("j", recmenu.x + recmenu.w - 30, recmenu.y + recmenu.h/2 - 14, "Police/Aquawax-Pro-Pictograms-Regular.ttf", 25, 'b', 'N');

	recmenu.y += recmenu.h + 30;

	if(SDL_SetRenderDrawColor(renderer, 102, 102, 102, 255));
	if(select == 2)
		if(SDL_SetRenderDrawColor(renderer, 0, 63, 255, 255));
	if(SDL_RenderFillRect(renderer, &recmenu));

	/*Option quitter le jeu*/
	SDL_choix_jeu[1].num_choix = 2;
	SDL_choix_jeu[1].b.x = recmenu.x;
	SDL_choix_jeu[1].b.y = recmenu.y;
	SDL_choix_jeu[1].a.y = recmenu.y + recmenu.h;
	SDL_choix_jeu[1].c.x = recmenu.x + recmenu.w;
	
	SDL_afficher_message("Quitter le jeu", recmenu.x + 70, recmenu.y + recmenu.h/2 - 14, "Police/bombarde.ttf", 25, 'b', 'N');
	SDL_afficher_message("X", recmenu.x + recmenu.w - 30, recmenu.y + recmenu.h/2 - 16, "Police/heydings_icons.ttf", 25, 'r', 'N');
}

/**
 *\fn void SDL_afficher_menu_sort(t_joueur joueur, int numero_perso, int select)
 *\brief Affichage du menu de sorts
 *\param joueur joueur dont c'est le tour
 *\param numero_perso du joueur en cours
 *\param select option selectionnée
 *\return void
 * La fonction affiche le menu des sorts en fonction des sorts du personnage
 * Select à -1 si aucune case n'est sélectionnée
*/
void SDL_afficher_menu_sort(t_joueur joueur, int numero_perso, int select){

	SDL_Rect recsort;

	t_personnage * perso;

	if(numero_perso == 1)
		perso = joueur.perso1;
	else 
		perso = joueur.perso2;

	recsort.h = 310;
	recsort.w = 660;
	recsort.x = (LARGEUR/2) - (recsort.w/2);
	recsort.y = (HAUTEUR/2) - (recsort.h/2);

	if(!SDL_RenderCopy(renderer, cadresort, NULL, &recsort));
	
	SDL_Rect recclose;
	recclose.w = 15;
	recclose.h = 15;
	recclose.x = recsort.x + recsort.w - recclose.w - 10;
	recclose.y = recsort.y + 10;

	int i, j, k = 0, y;

	if(SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255));

	SDL_afficher_message("ÌListe-des-sortsÍ", recsort.x + (recsort.w/2) - 145, recsort.y + 10, "Police/yorkwhiteletter.ttf", 50, 'r', 'N');

	recsort.w = (recsort.w/4) - 13;
	recsort.y = recsort.y + recsort.h - (recsort.h/2) - 70;
	recsort.h = (recsort.h/2) + 50;
	recsort.x += 10;

	char carac[5][50];
	char p[20];

	y = recsort.y + 50; 
	SDL_Rect recbouton;
	recbouton.w = 130;
	recbouton.h = 22;

	/*Affichage de chacun des sorts avec leurs caractéristiques*/
	for(i = 0; i < 4; i++){

		if(SDL_RenderDrawRect(renderer, &recsort));
		SDL_afficher_message(perso->sorts[i]->nom, recsort.x + (recsort.w/2) - 60, recsort.y + 12, "Police/Freshman.ttf", 15, 'b', 'N');

		strcpy(carac[k], "Portee : ");
		sprintf(p, "%i", perso->sorts[i]->portee);
		strcat(carac[k++], p);

		strcpy(carac[k], "Degats : ");
		sprintf(p, "%i", perso->sorts[i]->degat);
		strcat(carac[k++], p);

		strcpy(carac[k], "Utilisation par tour : ");

		if(perso->sorts[i]->upt > perso->sorts[i]->uptm)
			sprintf(p, "%i", 0);
		else 
			sprintf(p, "%i", perso->sorts[i]->upt);

		strcat(carac[k], p);
		strcat(carac[k], "/");

		sprintf(p, "%i", perso->sorts[i]->uptm);
		strcat(carac[k++], p);

		strcpy(carac[k], "Point d'action : ");
		sprintf(p, "%i", perso->sorts[i]->point_action);
		strcat(carac[k++], p);

		k = 0;

		for(j = 0; j < 4; j++){
			SDL_afficher_message(carac[j], recsort.x + 10, y, "Police/bombarde.ttf", 13, 'b', 'N');
			y += 20;
			strcpy(carac[j], "");
		}

		strcpy(p, "");

		recbouton.x = recsort.x + 10;
		recbouton.y = recsort.y + recsort.h - 40; 
		
		if(i == select - 1){
			if(!SDL_RenderCopy(renderer, boutonon, NULL, &recbouton));
		}
		else 
			(!SDL_RenderCopy(renderer, boutonoff, NULL, &recbouton));
			//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());

		/*Afficher bouton utiliser sort*/
		SDL_choix_sort[i].num_choix = i+1;
		SDL_choix_sort[i].b.x = recbouton.x;
		SDL_choix_sort[i].b.y = recbouton.y;
		SDL_choix_sort[i].a.y = recbouton.y + recbouton.h;
		SDL_choix_sort[i].c.x = recbouton.x + recbouton.w;

		SDL_afficher_message("Utiliser le sort", recbouton.x + 22, recbouton.y + 3, "Police/bombarde.ttf", 14, 'b', 'N');

		y = recsort.y + 50; 
		recsort.x += recsort.w + 10;
	}

	/*Bouton fermé si on ne veut pas utiliser de sorts*/
	SDL_afficher_message("X", recclose.x - 5, recclose.y - 5, "Police/heydings_icons.ttf", 25, 'r', 'N');
	SDL_choix_sort[4].num_choix = 5;
	SDL_choix_sort[4].b.x = recclose.x - 3;
	SDL_choix_sort[4].b.y = recclose.y;
	SDL_choix_sort[4].a.y = recclose.y + recclose.h + 3;
	SDL_choix_sort[4].c.x = recclose.x + recclose.w + 3;


}

/**
 *\fn void SDL_afficher_menu(t_joueur joueur, int numero_perso, int select)
 *\brief Affichage du menu
 *\param joueur joueur dont c'est le tour
 *\param numero_perso du joueur en cours
 *\param select option selectionnée
 *\return void
 * La fonction affiche le menu du joueur
 * Select à -1 si aucune case n'est sélectionnée
*/
void SDL_afficher_menu(t_joueur joueur, int numero_perso, int select){

	t_personnage * perso;

	SDL_Rect reccadre;
	reccadre.x = 10;
	reccadre.y = 10;
	reccadre.h = 220;
	reccadre.w = 350;

	if(numero_perso == 1)
		perso = joueur.perso1;
	else 
		perso = joueur.perso2;

	if(SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255));
	if(SDL_RenderDrawRect(renderer, &reccadre));
	
	if(joueur.numJoueur == 1)
		SDL_afficher_message("Joueur 1", reccadre.x + (reccadre.w/2) - 40, reccadre.y + 10, "Police/Freshman.ttf", 20, 'b', 'N');
	else
		SDL_afficher_message("Joueur 2", reccadre.x + (reccadre.w/2) - 40, reccadre.y + 10, "Police/Freshman.ttf", 20, 'b', 'N');
	
	SDL_affficher_id_perso(perso, 70, reccadre.x + 40, reccadre.y + 30, 17, joueur.numJoueur);

	char point[4][100] = {"Point de vie : ", "Point d'action : ", "Point de mouvement : "};
	
	char p[50];

	sprintf(p, "%i", perso->pv);
	strcat(point[0], p);
	strcat(point[0], "/");
	sprintf(p, "%i", perso->pv_max);
	strcat(point[0], p);

	sprintf(p, "%i", perso->pa);
	strcat(point[1], p);

	sprintf(p, "%i", perso->pm);
	strcat(point[2], p);

	int j;
	int deca = 125;
	/*Afficher les point du joueur en dessus de son image (id): Points de vie, points d'action et points de mouvement*/
	for(j = 0; j < 3; j++){
		SDL_afficher_message(point[j], reccadre.x + 15, reccadre.y + deca, "Police/bombarde.ttf", 14, 'o', 'N');
		deca += 20;
	}

	int i;

	if(SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255));
	reccadre.x = 190;
	reccadre.y = 80;
	reccadre.w = 130;
	reccadre.h = 22;
	char tabchaine[4][20] = {"Se déplacer", "Utiliser un sort", "Passer le tour"};
	int dec[4] = {30, 20, 23};

	/*Afficher les options du joueur : se déplacer, utiliser un sort et passer le tour*/
	for(i = 0; i < 3; i++){

		if(!SDL_RenderCopy(renderer, boutonoff, NULL, &reccadre));
			//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());
		if(i == select - 1)
			if(!SDL_RenderCopy(renderer, boutonon, NULL, &reccadre));			

		SDL_choix_menu[i].num_choix = i+1;
		SDL_choix_menu[i].b.x = reccadre.x;
		SDL_choix_menu[i].b.y = reccadre.y;
		SDL_choix_menu[i].a.y = reccadre.y + reccadre.h;
		SDL_choix_menu[i].c.x = reccadre.x + reccadre.w;

		SDL_afficher_message(tabchaine[i], reccadre.x + dec[i], reccadre.y + 3, "Police/bombarde.ttf", 14, 'b', 'N');
		reccadre.y += reccadre.h + 13;
	}



}

/**
 *\fn void SDL_affficher_id_perso(t_personnage * perso, int c, int xi, int yi, int taille_p, int num_j)
 *\brief Affichage de l'identificateur du personnage 
 *\param perso personnage dont on veut afficher l'image
 *\param c mesure du coté de l'image
 *\param xi abscisse d'affichage de l'image
 *\param yi ordonnée d'affichage de l'image
 *\param taille_p la taille de la police de son nom 
 *\param num_j numéro du joueur en dont on veut afficher le personnage
 *\return void
 * La fonction affiche l'image d'identification du perso à une position précise
*/
void SDL_affficher_id_perso(t_personnage * perso, int c, int xi, int yi, int taille_p, int num_j){

	SDL_Rect recperso;
	int xt, yt;
	recperso.w = recperso.h = c;
	char car = perso->nom[0];
	char chaine [50];

	/*Recuperation du premier caractère du nom du perso et verification de la classe pour afficher l'image correspondante*/
	switch(car){
		case 'G':
			recperso.x = xi;
			recperso.y = yi;
			xt = xi + 4;
			if(num_j == 1){
				if(!SDL_RenderCopy(renderer, idguerrier1, NULL, &recperso));
					//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());
			}
			else if (num_j == 2){
				if(!SDL_RenderCopy(renderer, idguerrier2, NULL, &recperso));
				//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());
			}
		break;
		case 'A':
			recperso.x = xi;
			recperso.y = yi;
			xt = xi + 8;
			if(num_j == 1){
				if(!SDL_RenderCopy(renderer, idarcher1, NULL, &recperso));
					//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());
			}
			else if (num_j == 2){
				if(!SDL_RenderCopy(renderer, idarcher2, NULL, &recperso));
				//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());
			}
		break;
		case 'T':
			recperso.x = xi;
			recperso.y = yi;
			xt = xi + 15;
			if(num_j == 1){
				if(!SDL_RenderCopy(renderer, idtank1, NULL, &recperso));
				//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());
			}
			else if (num_j == 2){
				if(!SDL_RenderCopy(renderer, idtank2, NULL, &recperso));
				//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());
			}
		break;
		case 'D':
			recperso.x = xi;
			recperso.y = yi;
			xt = xi + 13;
			if(num_j == 1){
				if(!SDL_RenderCopy(renderer, iddruide1, NULL, &recperso));
				//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());
			}
			else if (num_j == 2){
				if(!SDL_RenderCopy(renderer, iddruide2, NULL, &recperso));
				//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());
			}
		break;
	}
	strcpy(chaine, perso->nom);

	if(strcmp(chaine, "Archer") == 0)
		strcat(chaine, "e");
	if(strcmp(chaine, "Guerrier") == 0 && num_j == 2)
		strcat(chaine, "e");
	yt = yi + c + 2;
	SDL_afficher_message(chaine, xt, yt, "Police/bombarde.ttf", taille_p, 'b', 'N');
}

/**
 *\fn void SDL_afficher_barre_point_de_vie(t_personnage * perso, int c, int xi, int yi)
 *\brief Affichage de la barre de vie
 *\param perso personnage dont on veut afficher l'image
 *\param c mesure du coté de la barre de vie
 *\param xi abscisse d'affichage de la barre de vie
 *\param yi ordonnée d'affichage de la barre de vie
 *\return void
 * La fonction affiche la barre de point de vie des personnages
*/
void SDL_afficher_barre_point_de_vie(t_personnage * perso, int c, int xi, int yi){

	SDL_Rect recvie_max;
	SDL_Rect recvie_c;

	recvie_max.x = recvie_c.x = xi;
	recvie_c.x++;
	recvie_max.y = recvie_c.y = yi + c + 20;
	recvie_c.y++;
	recvie_max.h = recvie_c.h = 7;
	recvie_c.h-= 2;
	recvie_max.w = c;
	recvie_c.w = (perso->pv * c)/perso->pv_max;
	recvie_c.w-= 2;

	if(SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255));
	if(SDL_RenderDrawRect(renderer, &recvie_max));
	/*Afficher la barre de vie en vert si l'état du personnage est en bon*/
	if(SDL_SetRenderDrawColor(renderer, 0, 160, 40, 255));
	/*Afficher la barre de vie en orange si le personnage a atteint la moitié de ses points de vie*/
	if(perso->pv <= perso->pv_max/2)
		if(SDL_SetRenderDrawColor(renderer, 255, 127, 0, 255));
	/*Afficher la barre de vie en rouge si le personnage a atteint le quart de ses points de vie*/
	if(perso->pv <= perso->pv_max/4)
		if(SDL_SetRenderDrawColor(renderer, 168, 8, 8, 255));
	
	if(SDL_RenderFillRect(renderer, &recvie_c));
}

/**
 *\fn void SDL_afficher_point_de_vie(t_joueur joueur1, t_joueur joueur2)
 *\brief Affichage des points de vie
 *\param joueur1 joueur 1
 *\param joueur2 joueur 2
 *\return void
 * La fonction affiche les points de vie des personnages
*/
void SDL_afficher_point_de_vie(t_joueur joueur1, t_joueur joueur2){

	int xi = 415, yi = 45, c = 60;
	char car;

	SDL_Rect recjoueur;
	recjoueur.x = xi + 25;
	recjoueur.y = yi - 35;

	/*Afficher le numéro des joueurs, les images de ses deux personnages et leurs barres de vies*/
	SDL_ecrire_texte("Joueur 1", "Police/dimis.ttf", 25, 'b', 'N');
	SDL_QueryTexture(texte, NULL, NULL, &(recjoueur.w), &(recjoueur.h));
	if(!SDL_RenderCopy(renderer, texte, NULL, &recjoueur));

	SDL_Rect recvie_max;
	SDL_Rect recvie_c;

	SDL_affficher_id_perso(joueur1.perso1, c, xi, yi, 15, 1);
	SDL_afficher_barre_point_de_vie(joueur1.perso1, c, xi+2, yi);

	xi += c + 30;

	SDL_affficher_id_perso(joueur1.perso2, c, xi, yi, 15, 1);
	SDL_afficher_barre_point_de_vie(joueur1.perso2, c, xi+2, yi);

	xi += c + 205;
	recjoueur.x = xi - 125;
	recjoueur.y = 60 ;

	SDL_ecrire_texte("VS", "Police/eugenia.ttf", 50, 'b', 'N');
	SDL_QueryTexture(texte, NULL, NULL, &(recjoueur.w), &(recjoueur.h));
	if(!SDL_RenderCopy(renderer, texte, NULL, &recjoueur));

	recjoueur.x = xi + 20;
	recjoueur.y = yi - 35;

	SDL_ecrire_texte("Joueur 2", "Police/dimis.ttf", 25, 'b', 'N');
	SDL_QueryTexture(texte, NULL, NULL, &(recjoueur.w), &(recjoueur.h));
	if(!SDL_RenderCopy(renderer, texte, NULL, &recjoueur));

	SDL_affficher_id_perso(joueur2.perso1, c, xi, yi, 15, 2);
	SDL_afficher_barre_point_de_vie(joueur2.perso1, c, xi+2, yi);

	xi += c + 30;

	SDL_affficher_id_perso(joueur2.perso2, c, xi, yi, 15, 2);
	SDL_afficher_barre_point_de_vie(joueur2.perso2, c, xi+2, yi);
}

/**
 *\fn void SDL_afficher_degat(t_joueur j1, t_joueur j2, t_personnage * perso, int degat, char type)
 *\brief Affichage des dégâts
 *\param j1 joueur 1
 *\param j2 joueur 2
 *\param perso personnage qui subit les dégâts
 *\param type le type de dégât subit, positif ou négatif
 *\return void
 * La fonction affiche les dégats au dessus de la tête du personnage
*/
void SDL_afficher_degat(t_joueur j1, t_joueur j2, t_personnage * perso, int degat, char type){
	
	/*On recherche la case du perso*/
	t_case casej = recherche_case(perso->coord.y, perso->coord.x);

	SDL_RenderClear(renderer);
	if(j1.numJoueur == 1)
			SDL_afficher_map(map, j1, j2);
		else
			SDL_afficher_map(map, j2, j1);

	char chaine[10];
	char coul;

	/*Si les dégats sont positifs afficher le signe + et les afficher en jaune*/
	if(type == 'p'){
		chaine[0] = '+';
		coul = 'j';
	}

	/*Sinon afficher le signe - et les afficher en rouge*/
	else{
		chaine[0]= '-';
		coul = 'R';
	}

	char c_degat[6]="";
	sprintf(c_degat, "%i", degat);
	strcat(chaine, c_degat);

	SDL_afficher_message(chaine, (casej.a.x + casej.c.x)/2 - 5, casej.b.y - 95, "Police/bombarde.ttf", 20, coul, 'N');
	SDL_RenderPresent(renderer);
	SDL_Delay(700);
}

/**
 *\fn void SDL_afficher_sort(char * nom, int xsort, int ysort)
 *\brief Affichage des sorts
 *\param nom nom du personnage
 *\param xsort abscisse des affichages
 *\param ysort ordonnée des affichages
 *\return void
 * La fonction affiche les sorts en fontion du personnage
*/
void SDL_afficher_sort(char * nom, int xsort, int ysort){

	char c = nom[0];

	switch(c){
		case 'G':
			SDL_afficher_message("Saut : Portee [3] - Degat[0]", xsort + 22, ysort, "Police/big_noodle_titling.ttf", 13, 'n', 'N');
	  	SDL_afficher_message("Soin : Portee [0] - Degat[6]", xsort + 23, ysort + 16, "Police/big_noodle_titling.ttf", 13, 'n', 'N');
	  	SDL_afficher_message("Petit Coup : Portee [3] - Degat[10]", xsort + 10, ysort + 32, "Police/big_noodle_titling.ttf", 13, 'n', 'N');
	  	SDL_afficher_message("Gros Coup : Portee [1] - Degat[25]", xsort + 11, ysort + 48, "Police/big_noodle_titling.ttf", 13, 'n', 'N');
		break;
		case 'A':
			SDL_afficher_message("Diago : Portee [7] - Degat[14]", xsort + 22, ysort, "Police/big_noodle_titling.ttf", 13, 'n', 'N');
	  	SDL_afficher_message("Ligne : Portee [7] - Degat[14]", xsort + 24, ysort + 16, "Police/big_noodle_titling.ttf", 13, 'n', 'N');
	  	SDL_afficher_message("Double Tape : Portee [5] - Degat[6]", xsort + 10, ysort + 32, "Police/big_noodle_titling.ttf", 13, 'n', 'N');
	  	SDL_afficher_message("Coup Zone : Portee [5] - Degat[12]", xsort + 13, ysort + 48, "Police/big_noodle_titling.ttf", 13, 'n', 'N');
		break;
		case 'T':
			SDL_afficher_message("Armure : Portee [0] - Degat[0]", xsort + 19, ysort, "Police/big_noodle_titling.ttf", 13, 'n', 'N');
	 		SDL_afficher_message("Attire : Portee [15] - Degat[0]", xsort + 20, ysort + 16, "Police/big_noodle_titling.ttf", 13, 'n', 'N');
	 		SDL_afficher_message("ChenChen : Portee [2] - Degat[10]", xsort + 16, ysort + 32, "Police/big_noodle_titling.ttf", 13, 'n', 'N');
	 		SDL_afficher_message("BigShaq : Portee [1] - Degat[0]", xsort + 20, ysort + 48, "Police/big_noodle_titling.ttf", 13, 'n', 'N');
		break;
		case 'D':
			SDL_afficher_message("Minotaure : Portee [0] - Degat[0]", xsort + 14, ysort, "Police/big_noodle_titling.ttf", 13, 'n', 'N');
	 		SDL_afficher_message("Felin : Portee [15] - Degat[0]", xsort + 25, ysort + 16, "Police/big_noodle_titling.ttf", 13, 'n', 'N');
	 		SDL_afficher_message("Fuego : Portee [4] - Degat[12]", xsort + 25, ysort + 32, "Police/big_noodle_titling.ttf", 13, 'n', 'N');
	 		SDL_afficher_message("Revitalisation : Portee [2] - Degat[8]", xsort + 6, ysort + 48, "Police/big_noodle_titling.ttf", 13, 'n', 'N');
		break;
	}
}

/**
 *\fn void SDL_afficher_message(char * message, int x, int y, char * police, int taille, char couleur, char style)
 *\brief Affichage de message
 *\param message message à afficher
 *\param x abscisse d'affichage
 *\param y ordonnée d'affichage
 *\param police la police d'écriture
 *\param taille la taille de la police
 *\param couleur la couleur que l'on souhaite donner au message
 *\param style le style d'écriture du message (Gras, Italic, Souligné ou Normal)
 *\return void
 * La fonction affiche un message selon des cordonnées en fontion de la police, de la couleur choisie, du style et de la taille
*/
void SDL_afficher_message(char * message, int x, int y, char * police, int taille, char couleur, char style){

	SDL_Rect recquestion;
	recquestion.x = x;
	recquestion.y = y;
	SDL_ecrire_texte(message, police, taille, couleur, style);
	SDL_QueryTexture(texte, NULL, NULL, &(recquestion.w), &(recquestion.h));
	if(!SDL_RenderCopy(renderer, texte, NULL, &recquestion));
}

/**
 *\fn void SDL_afficher_perso_choisi(t_joueur joueur1, t_joueur joueur2)
 *\brief Affichage du personnage choisi
 *\param joueur1 joueur 1
 *\param joueur2 joueur 2
 *\return void
 * La fonction affiche les personnages choisis lors de l'initialisation
*/
void SDL_afficher_perso_choisi(t_joueur joueur1, t_joueur joueur2){

	int xi = 400, yi = 75, c = 75;

	SDL_Rect recjoueur;
	recjoueur.x = xi + 20;
	recjoueur.y = 30 ;

	SDL_ecrire_texte("Joueur 1", "Police/dimis.ttf", 30, 'b', 'N');
	SDL_QueryTexture(texte, NULL, NULL, &(recjoueur.w), &(recjoueur.h));
	
	if(!SDL_RenderCopy(renderer, texte, NULL, &recjoueur));

	/*Afficher les images d'identifications des personnages s'ils ne sont pas NULL, donc s'ils existent*/
	if(joueur1.perso1)
		SDL_affficher_id_perso(joueur1.perso1, c, xi, yi, 20, 1);
	/*Si le perso 1 du joueur 1 n'existe par demander à l'utilisateur de faire un choix*/
	else {
		SDL_afficher_message("Choix personnage 1", 410, HAUTEUR - 115, "Police/calendarnote.ttf", 50, 'o', 'N');
		SDL_afficher_message("ÌJoueur1Í",571, 592, "Police/yorkwhiteletter.ttf", 60, 'r', 'N');
	}

	xi += c + 10;

	if(joueur1.perso2)
		SDL_affficher_id_perso(joueur1.perso2, c, xi, yi, 20, 1);
	/*Si le perso 2 du joueur 1 n'existe par demander à l'utilisateur de faire un choix*/
	else if(joueur1.perso1){
		SDL_afficher_message("Choix personnage 2", 410, HAUTEUR - 115, "Police/calendarnote.ttf", 50, 'o', 'N');
		SDL_afficher_message("ÌJoueur1Í",571, 592, "Police/yorkwhiteletter.ttf", 60, 'r', 'N');
	}

	xi += c + 190;
	recjoueur.x = xi - 130;
	recjoueur.y = 90 ;

	SDL_ecrire_texte("VS", "Police/eugenia.ttf", 75, 'b', 'N');
	SDL_QueryTexture(texte, NULL, NULL, &(recjoueur.w), &(recjoueur.h));
	
	if(!SDL_RenderCopy(renderer, texte, NULL, &recjoueur));

	recjoueur.x = xi + 20;
	recjoueur.y = 30 ;

	SDL_ecrire_texte("Joueur 2", "Police/dimis.ttf", 30, 'b', 'N');
	SDL_QueryTexture(texte, NULL, NULL, &(recjoueur.w), &(recjoueur.h));
	if(!SDL_RenderCopy(renderer, texte, NULL, &recjoueur));

	if(joueur2.perso1)
		SDL_affficher_id_perso(joueur2.perso1, c, xi, yi, 20, 2);
	/*Si le perso 1 du joueur 2 n'existe par demander à l'utilisateur de faire un choix*/
	else if(joueur1.perso1 && joueur1.perso2){
		SDL_afficher_message("Choix personnage 1", 410, HAUTEUR - 115, "Police/calendarnote.ttf", 50, 'o', 'N');
		SDL_afficher_message("ÌJoueur2Í",571, 592, "Police/yorkwhiteletter.ttf", 60, 'r', 'N');
	}

	xi += c + 10;

	if(joueur2.perso2)
		SDL_affficher_id_perso(joueur2.perso2, c, xi, yi, 20, 2);
	/*Si le perso 2 du joueur 2 n'existe par demander à l'utilisateur de faire un choix*/
	else if(joueur1.perso1 && joueur1.perso2 && joueur2.perso1){
		SDL_afficher_message("Choix personnage 2", 410, HAUTEUR - 115, "Police/calendarnote.ttf", 50, 'o', 'N');
		SDL_afficher_message("ÌJoueur2Í",571, 592, "Police/yorkwhiteletter.ttf", 60, 'r', 'N');
	}
}

/**
 *\fn void SDL_afficher_bg()
 *\brief Affichage de l'arrière plan
 *\return void
*/
void SDL_afficher_bg(){
	
	SDL_Rect recbg;
	if(!SDL_QueryTexture(bg, NULL, NULL, &recbg.w, &recbg.h));
		//fprintf(stderr, "Texture non chargée : %s\n", SDL_GetError());
	if(!SDL_RenderCopy(renderer, bg, NULL, NULL));
		//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());*/
}

/**
 *\fn void SDL_afficher_acceuil()
 *\brief Affichage d'accueil
 *\return void
*/
void SDL_afficher_acceuil(){

    SDL_Rect recaccueil;
	srand(time(NULL));
	int i = rand()%2;
	/*Deux images d'accueil affichées aléatoirement au lancement du jeu*/
	if(i == 0){
	    if(!SDL_QueryTexture(accueil, NULL, NULL, &recaccueil.w, &recaccueil.h));
	    	//fprintf(stderr, "Texture non chargée : %s\n", SDL_GetError());
    	if(!SDL_RenderCopy(renderer, accueil, NULL, NULL));
          //fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());*/
	}
	else {
		if(!SDL_QueryTexture(accueil1, NULL, NULL, &recaccueil.w, &recaccueil.h))
          ;//fprintf(stderr, "Texture non chargée : %s\n", SDL_GetError());
    	if(!SDL_RenderCopy(renderer, accueil1, NULL, NULL));
          //fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());*/
	}
  SDL_RenderPresent(renderer);
  SDL_Delay(3000);
  SDL_RenderClear(renderer);
}

/**
 *\fn SDL_affichage_choix_perso(int num_j)
 *\brief Affichage des choix de personnages
 *\param num_j numéro du joueur dont c'est le tour de faire le choix
 *\return void
 * La fonction affiche les possibilités de choix des personnages en fonction des joueurs
*/
void SDL_affichage_choix_perso(int num_j){

	SDL_afficher_bg();
	SDL_Rect recchoix_perso;

	int x = 90, w = (LARGEUR/5) + 5, y = 160, h = (HAUTEUR/2) + 5 + 30;

	recchoix_perso.w = w;
	recchoix_perso.h = h;
	recchoix_perso.x = x;
	recchoix_perso.y = y;

	int ximg = recchoix_perso.x + 23, yimg = recchoix_perso.y + 37;

	SDL_choix_p[0].num_choix = 1;
	SDL_choix_p[0].b.x = ximg;
	SDL_choix_p[0].b.y = yimg;
	SDL_choix_p[0].a.y = yimg + h - (37*2);
	SDL_choix_p[0].c.x = ximg + w - (23*2);

	SDL_Rect rectexte;

	int xsort = 142, ysort = 460;

	/*Affiche le panel des personnages du joueur 1 et leurs sorts*/
	if(num_j == 1){
		if(!SDL_RenderCopy(renderer, choix_perso, NULL, &recchoix_perso));
			//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());
		SDL_ecrire_texte("Guerrier", "Police/bombarde.ttf", 22, 'o', 'N');
		rectexte.x = ximg + 78;
		rectexte.y = yimg + 20;
		SDL_QueryTexture(texte, NULL, NULL, &(rectexte.w), &(rectexte.h));
		if(!SDL_RenderCopy(renderer, texte, NULL, &rectexte));

		SDL_Rect recguerrier1;
		recguerrier1.w = 135;
		recguerrier1.h = 150;
		recguerrier1.x = recchoix_perso.x + 67;
		recguerrier1.y = recchoix_perso.y + 115;

		if(!SDL_RenderCopy(renderer, guerrier1, NULL, &recguerrier1));
			//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());
		SDL_afficher_sort("Guerrier", xsort, ysort);

		recchoix_perso.x = recchoix_perso.x + w + 15;
		ximg = recchoix_perso.x + 23;

		SDL_choix_p[1].num_choix = 2;
		SDL_choix_p[1].b.x = ximg;
		SDL_choix_p[1].b.y = yimg;
		SDL_choix_p[1].a.y = yimg + h - (37*2);
		SDL_choix_p[1].c.x = ximg + w - (23*2);

		if(!SDL_RenderCopy(renderer, choix_perso, NULL, &recchoix_perso));
			//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());
		SDL_ecrire_texte("Archere", "Police/bombarde.ttf", 22, 'o', 'N');
		
		rectexte.x = ximg + 78;
		
		SDL_QueryTexture(texte, NULL, NULL, &(rectexte.w), &(rectexte.h));
		if(!SDL_RenderCopy(renderer, texte, NULL, &rectexte));

		SDL_Rect recarcher1;
		recarcher1.w = 140;
		recarcher1.h = 155;
		recarcher1.x = recchoix_perso.x + 75;
		recarcher1.y = recchoix_perso.y + 110;

		if(!SDL_RenderCopy(renderer, archer1, NULL, &recarcher1));
			//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());
		xsort += 288;
		
		SDL_afficher_sort("Archere", xsort, ysort);
		
		recchoix_perso.x = recchoix_perso.x + w + 15;
		ximg = recchoix_perso.x + 23;

		SDL_choix_p[2].num_choix = 3;
	 	SDL_choix_p[2].b.x = ximg;
		SDL_choix_p[2].b.y = yimg;
		SDL_choix_p[2].a.y = yimg + h - (37*2);
		SDL_choix_p[2].c.x = ximg + w - (23*2);

		if(!SDL_RenderCopy(renderer, choix_perso, NULL, &recchoix_perso));
		  	//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());
		SDL_ecrire_texte("Tank", "Police/bombarde.ttf", 22, 'o', 'N');
		rectexte.x = ximg + 92;
		SDL_QueryTexture(texte, NULL, NULL, &(rectexte.w), &(rectexte.h));
		if(!SDL_RenderCopy(renderer, texte, NULL, &rectexte));

		SDL_Rect rectank1;

		rectank1.w = 155;
		rectank1.h = 180;
		rectank1.x = recchoix_perso.x + 62;
		rectank1.y = recchoix_perso.y + 98;

		if(!SDL_RenderCopy(renderer, tank1, NULL, &rectank1));
		//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());
		xsort += 288;
		
		SDL_afficher_sort("Tank", xsort, ysort);

		recchoix_perso.x = recchoix_perso.x + w + 15;
		ximg = recchoix_perso.x + 23;

		SDL_choix_p[3].num_choix = 4;
		SDL_choix_p[3].b.x = ximg;
		SDL_choix_p[3].b.y = yimg;
		SDL_choix_p[3].a.y = yimg + h - (37*2);
		SDL_choix_p[3].c.x = ximg + w - (23*2);

		if(!SDL_RenderCopy(renderer, choix_perso, NULL, &recchoix_perso));
		//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());
		SDL_ecrire_texte("Druide", "Police/bombarde.ttf", 22, 'o', 'N');
		rectexte.x = ximg + 90;
		
		SDL_QueryTexture(texte, NULL, NULL, &(rectexte.w), &(rectexte.h));
		if(!SDL_RenderCopy(renderer, texte, NULL, &rectexte));

		SDL_Rect recdruide1;
		recdruide1.w = 120;
		recdruide1.h = 150;
		recdruide1.x = recchoix_perso.x + 65;
		recdruide1.y = recchoix_perso.y + 110;

		if(!SDL_RenderCopy(renderer, druide1, NULL, &recdruide1));
			//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());
		xsort += 288;
		
		SDL_afficher_sort("Druide", xsort, ysort);
	}

	/*Affiche le panel des personnages du joueur 2 et leurs sorts*/
	else if(num_j == 2){
		if(!SDL_RenderCopy(renderer, choix_perso, NULL, &recchoix_perso));
			//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());
		SDL_ecrire_texte("Guerriere", "Police/bombarde.ttf", 22, 'o', 'N');
		
		rectexte.x = ximg + 73;
		rectexte.y = yimg + 20;
		
		SDL_QueryTexture(texte, NULL, NULL, &(rectexte.w), &(rectexte.h));
		if(!SDL_RenderCopy(renderer, texte, NULL, &rectexte));

		SDL_Rect recguerrier2;
		recguerrier2.w = 155;
		recguerrier2.h = 150;
		recguerrier2.x = recchoix_perso.x + 55;
		recguerrier2.y = recchoix_perso.y + 110;

		if(!SDL_RenderCopy(renderer, guerrier2, NULL, &recguerrier2));
			//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());
		SDL_afficher_sort("Guerrier", xsort, ysort);
		
		recchoix_perso.x = recchoix_perso.x + w + 15;
		ximg = recchoix_perso.x + 23;

		SDL_choix_p[1].num_choix = 2;
		SDL_choix_p[1].b.x = ximg;
		SDL_choix_p[1].b.y = yimg;
		SDL_choix_p[1].a.y = yimg + h - (37*2);
		SDL_choix_p[1].c.x = ximg + w - (23*2);

		if(!SDL_RenderCopy(renderer, choix_perso, NULL, &recchoix_perso));
			//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());
		SDL_ecrire_texte("Archere", "Police/bombarde.ttf", 22, 'o', 'N');
		
		rectexte.x = ximg + 78;
		
		SDL_QueryTexture(texte, NULL, NULL, &(rectexte.w), &(rectexte.h));
		if(!SDL_RenderCopy(renderer, texte, NULL, &rectexte));

		SDL_Rect recarcher2;
		recarcher2.w = 125;
		recarcher2.h = 145;
		recarcher2.x = recchoix_perso.x + 95;
		recarcher2.y = recchoix_perso.y + 112;

		if(!SDL_RenderCopy(renderer, archer2, NULL, &recarcher2));
			//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());
		xsort += 287;
		
		SDL_afficher_sort("Archere", xsort, ysort);

		recchoix_perso.x = recchoix_perso.x + w + 15;
		ximg = recchoix_perso.x + 23;

		SDL_choix_p[2].num_choix = 3;
	 	SDL_choix_p[2].b.x = ximg;
		SDL_choix_p[2].b.y = yimg;
		SDL_choix_p[2].a.y = yimg + h - (37*2);
		SDL_choix_p[2].c.x = ximg + w - (23*2);

		if(!SDL_RenderCopy(renderer, choix_perso, NULL, &recchoix_perso));
		  	//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());
		SDL_ecrire_texte("Tank", "Police/bombarde.ttf", 22, 'o', 'N');
		
		rectexte.x = ximg + 92;
		
		SDL_QueryTexture(texte, NULL, NULL, &(rectexte.w), &(rectexte.h));
		if(!SDL_RenderCopy(renderer, texte, NULL, &rectexte));

		SDL_Rect rectank2;
		rectank2.w = 155;
		rectank2.h = 165;
		rectank2.x = recchoix_perso.x + 57;
		rectank2.y = recchoix_perso.y + 103;

		if(!SDL_RenderCopy(renderer, tank2, NULL, &rectank2));
			//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());
		xsort += 288;
		
		SDL_afficher_sort("Tank", xsort, ysort);

		recchoix_perso.x = recchoix_perso.x + w + 15;
		ximg = recchoix_perso.x + 23;

		SDL_choix_p[3].num_choix = 4;
		SDL_choix_p[3].b.x = ximg;
		SDL_choix_p[3].b.y = yimg;
		SDL_choix_p[3].a.y = yimg + h - (37*2);
		SDL_choix_p[3].c.x = ximg + w - (23*2);

		if(!SDL_RenderCopy(renderer, choix_perso, NULL, &recchoix_perso));
			//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());
		SDL_ecrire_texte("Druide", "Police/bombarde.ttf", 22, 'o', 'N');
		
		rectexte.x = ximg + 90;
		
		SDL_QueryTexture(texte, NULL, NULL, &(rectexte.w), &(rectexte.h));
		if(!SDL_RenderCopy(renderer, texte, NULL, &rectexte));

		SDL_Rect recdruide2;
		recdruide2.w = 155;
		recdruide2.h = 160;
		recdruide2.x = recchoix_perso.x + 55;
		recdruide2.y = recchoix_perso.y + 105;

		if(!SDL_RenderCopy(renderer, druide2, NULL, &recdruide2));
			//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());
		xsort += 288;
		
		SDL_afficher_sort("Druide", xsort, ysort);
	}
}

/**
 *\fn void SDL_afficher_map(char map[N][N], t_joueur j1, t_joueur j2)
 *\brief Affichage de la map
 *\param map[N][N] matrice de jeu à afficher
 *\param j1 joueur 1
 *\param j2 joueur 2
 *\return void
 * La fonction affiche le plan de jeu isométrique
*/
void SDL_afficher_map(char map[N][N], t_joueur j1, t_joueur j2){

	/*Affichage de l'arrière plan, des point de vies des persos au dessus de la fenêtre et du nombre de tours en dessous*/
	SDL_afficher_bg();
	SDL_afficher_point_de_vie(j1, j2);
	char t[3];
	sprintf(t, "%i", nb_tour);
	SDL_afficher_message(t, (LARGEUR/2) - 13, HAUTEUR - 80, "Police/squealer.ttf", 65, 'b', 'N');
	
	int i, j, w = 80, h = 40, x = 628, y = 175, k = 0; //x = 100, y = (HAUTEUR/2) - 50, k = 0;

	char car;
	/*Déclaration de tous les éléments de la map : cases, obstacles et images des persos et bouclier pour le sorts Armure*/
	SDL_Rect reccase_n;
	reccase_n.w = w;
	reccase_n.h = h;
	reccase_n.x = x;
	reccase_n.y = y;
	
	int depx = reccase_n.x, depy = reccase_n.y;

	SDL_Rect reccase_p;
	reccase_p.w = w;
	reccase_p.h = h;
	reccase_p.x = x;
	reccase_p.y = y;

	SDL_Rect reccase_c;
	reccase_c.w = w;
	reccase_c.h = h;
	reccase_c.x = x;
	reccase_c.y = y;

	int xo = x+3, yo = y-38, wo = w - 5, ho = h + 40;

	SDL_Rect recobstacle;
	recobstacle.w = wo;
	recobstacle.h = ho;
	recobstacle.x = xo;
	recobstacle.y = yo;

	int ws = w - 15, hs = h - 15;

	SDL_Rect reccase_s;
	reccase_s.w = ws;
	reccase_s.h = hs;

	SDL_Rect recguerrier1;
	recguerrier1.w = 95;
	recguerrier1.h = 100;

	SDL_Rect rectank1;
	rectank1.w = 115;
	rectank1.h = 115;

	SDL_Rect recarcher1;
	recarcher1.w = 100;
	recarcher1.h = 100;

	SDL_Rect recdruide1;
	recdruide1.w = 93;
	recdruide1.h = 95;

	SDL_Rect recguerrier2;
	recguerrier2.w = 105;
	recguerrier2.h = 103;

	SDL_Rect rectank2;
	rectank2.w = 110;
	rectank2.h = 110;

	SDL_Rect recarcher2;
	recarcher2.w = 90;
	recarcher2.h = 92;

	SDL_Rect recdruide2;
	recdruide2.w = 110;
	recdruide2.h = 107;

	SDL_Rect recbouclier;
	recbouclier.w = 35;
	recbouclier.h = 40;

	/*Affichage de la map en mettant à jour les coordonnées de la case : cordonnées des points, type de case et numéro de case dans la matrice*/
	for(i = 0; i < N; i++){
		for(j = N-1; j >= 0 ; j--){
			/*Si c'est un obstacle : afficher l'image d'obstacle*/
			if(map[i][j] == 'o'){
				if(!SDL_RenderCopy(renderer, obstacle, NULL, &recobstacle));
					//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());

				SDL_map[k].i = i;
				SDL_map[k].j = j;
				SDL_map[k].type = OBSTACLE;
				SDL_map[k].a.x = reccase_n.x;
				SDL_map[k].a.y = reccase_n.y + (h/2);
				SDL_map[k].c.x = reccase_n.x + w;
				SDL_map[k].c.y = reccase_n.y + (h/2);
				SDL_map[k].b.y = reccase_n.y;
				SDL_map[k].b.x = reccase_n.x + (w/2);
				SDL_map[k].d.y = reccase_n.y + ho;
				SDL_map[k].d.x = reccase_n.x + (w/2);
				k++;
			}
			/*Sinon, si c'est une portée : afficher l'image de la portée*/
			else if(map[i][j] >= 'A' && map[i][j] <= 'Z'){
				if(!SDL_RenderCopy(renderer, case_p, NULL, &reccase_p));
					//fprintf(stderr, "Textur1e non copiée : %s\n", SDL_GetError());
				SDL_map[k].i = i;
				SDL_map[k].j = j;
				SDL_map[k].type = CASE_BLEUE;
				SDL_map[k].a.x = reccase_n.x;
				SDL_map[k].a.y = reccase_n.y + (h/2);
				SDL_map[k].c.x = reccase_n.x + w;
				SDL_map[k].c.y = reccase_n.y + (h/2);
				SDL_map[k].b.y = reccase_n.y;
				SDL_map[k].b.x = reccase_n.x + (w/2);
				SDL_map[k].d.y = reccase_n.y + h;
				SDL_map[k].d.x = reccase_n.x + (w/2);
				k++;
			}
			/*Sinon, si c'est une case choisie : afficher l'image de la case choisie*/
			else if(map[i][j] == 'x'){
				if(!SDL_RenderCopy(renderer, case_c, NULL, &reccase_c));
					//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());
			}
			/*Sinon afficher personnage*/
			else{
				/*Si le personnage est dans la portée afficher une case de portée en dessous de lui*/
				if(map[i][j] >= 'a' && map[i][j] <= 'z'){
					if(!SDL_RenderCopy(renderer, case_p, NULL, &reccase_p));
						//fprintf(stderr, "Textur1e non copiée : %s\n", SDL_GetError());
				}
				/*Sinon afficher une case classique*/
				else {
					if(!SDL_RenderCopy(renderer, case_n, NULL, &reccase_n));
						//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());
				}
				SDL_map[k].i = i;
				SDL_map[k].j = j;
				SDL_map[k].type = CASE_GRISE;
				SDL_map[k].a.x = reccase_n.x;
				SDL_map[k].a.y = reccase_n.y + (h/2);
				SDL_map[k].c.x = reccase_n.x + w;
				SDL_map[k].c.y = reccase_n.y + (h/2);
				SDL_map[k].b.y = reccase_n.y;
				SDL_map[k].b.x = reccase_n.x + (w/2);
				SDL_map[k].d.y = reccase_n.y + h;
				SDL_map[k].d.x = reccase_n.x + (w/2);

				if(map[i][j] == '1' || map[i][j] == 'a'){

					car = j1.perso1->nom[0];
					/*Afficher personnage*/
					switch(car){
						case 'A':
							recarcher1.x = reccase_n.x - 15;
							recarcher1.y = reccase_n.y - 72;

							if(!SDL_RenderCopy(renderer, archer1, NULL, &recarcher1));
								//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());
						break;
						case 'G':
							recguerrier1.x = reccase_n.x - 10;
							recguerrier1.y = reccase_n.y - 72;

							if(!SDL_RenderCopy(renderer, guerrier1, NULL, &recguerrier1));
								//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());
						break;
						case 'T':
							rectank1.x = reccase_n.x - 17;
							rectank1.y = reccase_n.y - 82;

							if(!SDL_RenderCopy(renderer, tank1, NULL, &rectank1));
								//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());
							/*Si c'est un Tank et qu'il a activer le sort armure lui afficher un bouclier*/
							if(strcmp(j1.perso1->statut, "Armure") == 0){
								recbouclier.x = rectank1.x + 50;
								recbouclier.y = rectank1.y + (rectank1.h/2) - (recbouclier.h/2) - 5;
								if(!SDL_RenderCopy(renderer, bouclier, NULL, &recbouclier));
									//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());
							}
						break;
						case 'D':
							/*S'il n'y a pas de transformation : afficher le druide classique*/
							if(strcmp(j1.perso1->statut, "Inconnu") == 0){
								recdruide1.x = reccase_n.x - 30;
								recdruide1.y = reccase_n.y - 70;
								if(!SDL_RenderCopy(renderer, druide1, NULL, &recdruide1));
								//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());
							}
							/*Sinon, si le druide est transformé en felin afficher : le félin*/
							else if(strcmp(j1.perso1->statut, "Felin") == 0){
								recdruide1.w = 100;
								recdruide1.h = 102;
								recdruide1.x = reccase_n.x - 12;
								recdruide1.y = reccase_n.y - 75;
								if(!SDL_RenderCopy(renderer, felin1, NULL, &recdruide1));
									//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());
							}
							/*Sinon, si le druide est transformé en minotaure : afficher le minotaure*/
							else if(strcmp(j1.perso1->statut, "Minotaure") == 0){
								recdruide1.x = reccase_n.x - 4;
								recdruide1.y = reccase_n.y - 77;
								recdruide1.w -= 10;
								recdruide1.h += 8;
								if(!SDL_RenderCopy(renderer, minotaure1, NULL, &recdruide1));
									//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());
							}
							recdruide1.w = 93;
							recdruide1.h = 95;
						break;
					}
					SDL_map[k].i = i;
					SDL_map[k].j = j;
					SDL_map[k].type = J1P1;
				}

				else if (map[i][j] == '2' || map[i][j] == 'b'){

					car = j2.perso1->nom[0];

					switch(car){
						case 'A':
							recarcher2.x = reccase_n.x + 15;
							recarcher2.y = reccase_n.y - 67;

							if(!SDL_RenderCopy(renderer, archer2, NULL, &recarcher2));
								//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());
						break;
						case 'G':
							recguerrier2.x = reccase_n.x - 18;
							recguerrier2.y = reccase_n.y - 77;

							if(!SDL_RenderCopy(renderer, guerrier2, NULL, &recguerrier2));
								//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());
						break;
						case 'T':
							rectank2.x = reccase_n.x - 17;
							rectank2.y = reccase_n.y - 79;

							if(!SDL_RenderCopy(renderer, tank2, NULL, &rectank2));
								//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());
							/*Si c'est un Tank et qu'il a activer le sort armure lui afficher un bouclier*/
							if(strcmp(j2.perso1->statut, "Armure") == 0){
								recbouclier.x = rectank2.x + 50;
								recbouclier.y = rectank2.y + (rectank2.h/2) - (recbouclier.h/2) - 5;
								if(!SDL_RenderCopy(renderer, bouclier, NULL, &recbouclier));
									//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());
							}
						break;
						case 'D':
							/*S'il n'y a pas de transformation : afficher le druide classique*/
							if(strcmp(j2.perso1->statut, "Inconnu") == 0){
								recdruide2.x = reccase_n.x - 20;
								recdruide2.y = reccase_n.y - 77;
								if(!SDL_RenderCopy(renderer, druide2, NULL, &recdruide2));
								//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());
							}
							/*Sinon, si le druide est transformé en felin afficher : le félin*/
							else if(strcmp(j2.perso1->statut, "Felin") == 0){
								recdruide2.x = reccase_n.x - 10;
								recdruide2.y = reccase_n.y - 72;
								recdruide2.w = 100;
								recdruide2.h = 102;
								if(!SDL_RenderCopy(renderer, felin2, NULL, &recdruide2));
								//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());
							}
							/*Sinon, si le druide est transformé en minotaure : afficher le minotaure*/
							else if(strcmp(j2.perso1->statut, "Minotaure") == 0){
								recdruide2.x = reccase_n.x - 15;
								recdruide2.y = reccase_n.y - 87;
								recdruide2.w = 120;
								recdruide2.h = 112;
								if(!SDL_RenderCopy(renderer, minotaure2, NULL, &recdruide2));
									//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());
							}
							recdruide2.w = 110;
							recdruide2.h = 107;
						break;
					}
					SDL_map[k].i = i;
					SDL_map[k].j = j;
					SDL_map[k].type = J2P1;
				}

				else if(map[i][j] == '3'|| map[i][j] == 'c'){

					car = j1.perso2->nom[0];

					switch(car){
						case 'A':
							recarcher1.x = reccase_n.x - 10;
							recarcher1.y = reccase_n.y - 72;

							if(!SDL_RenderCopy(renderer, archer1, NULL, &recarcher1));
								//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());
						break;
						case 'G':
							recguerrier1.x = reccase_n.x - 10;
							recguerrier1.y = reccase_n.y - 72;

							if(!SDL_RenderCopy(renderer, guerrier1, NULL, &recguerrier1));
								//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());
						break;
						case 'T':
							rectank1.x = reccase_n.x - 17;
							rectank1.y = reccase_n.y - 82;

							if(!SDL_RenderCopy(renderer, tank1, NULL, &rectank1));
								//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());
							/*Si c'est un Tank et qu'il a activer le sort armure lui afficher un bouclier*/
							if(strcmp(j1.perso2->statut, "Armure") == 0){
								recbouclier.x = rectank1.x + 50;
								recbouclier.y = rectank1.y + (rectank1.h/2) - (recbouclier.h/2) - 5;
								if(!SDL_RenderCopy(renderer, bouclier, NULL, &recbouclier));
									//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());
							}
						break;
						case 'D':
							/*S'il n'y a pas de transformation : afficher le druide classique*/
							if(strcmp(j1.perso2->statut, "Inconnu") == 0){
								recdruide1.x = reccase_n.x - 30;
								recdruide1.y = reccase_n.y - 70;
								if(!SDL_RenderCopy(renderer, druide1, NULL, &recdruide1));
								//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());
							}
							/*Sinon, si le druide est transformé en felin afficher : le félin*/
							else if(strcmp(j1.perso2->statut, "Felin") == 0){
								recdruide1.w = 100;
								recdruide1.h = 102;
								recdruide1.x = reccase_n.x - 12;
								recdruide1.y = reccase_n.y - 75;
								if(!SDL_RenderCopy(renderer, felin1, NULL, &recdruide1));
									//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());
							}
							/*Sinon, si le druide est transformé en minotaure : afficher le minotaure*/
							else if(strcmp(j1.perso2->statut, "Minotaure") == 0){
								recdruide1.x = reccase_n.x - 2;
								recdruide1.y = reccase_n.y - 77;
								recdruide1.w -= 10;
								recdruide1.h += 8;
								if(!SDL_RenderCopy(renderer, minotaure1, NULL, &recdruide1));
									//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());
							}					
							recdruide1.w = 93;
							recdruide1.h = 95;
						break;

					}
					SDL_map[k].i = i;
					SDL_map[k].j = j;
					SDL_map[k].type = J1P2;
				}

				else if(map[i][j] == '4' || map[i][j] == 'd'){

					car = j2.perso2->nom[0];

					switch(car){
						case 'A':
							recarcher2.x = reccase_n.x + 10;
							recarcher2.y = reccase_n.y - 67;

							if(!SDL_RenderCopy(renderer, archer2, NULL, &recarcher2));
								//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());
						break;
						case 'G':
							recguerrier2.x = reccase_n.x - 18;
							recguerrier2.y = reccase_n.y - 77;

							if(!SDL_RenderCopy(renderer, guerrier2, NULL, &recguerrier2));
								//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());
						break;
						case 'T':
							rectank2.x = reccase_n.x - 17;
							rectank2.y = reccase_n.y - 79;

							if(!SDL_RenderCopy(renderer, tank2, NULL, &rectank2));
								//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());
							/*Si c'est un Tank et qu'il a activer le sort armure lui afficher un bouclier*/
							if(strcmp(j2.perso2->statut, "Armure") == 0){
								recbouclier.x = rectank2.x + 50;
								recbouclier.y = rectank2.y + (rectank2.h/2) - (recbouclier.h/2) - 5;
								if(!SDL_RenderCopy(renderer, bouclier, NULL, &recbouclier));
									//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());
							}
						break;
						case 'D':
							/*S'il n'y a pas de transformation : afficher le druide classique*/
							if(strcmp(j2.perso2->statut, "Inconnu") == 0){
								recdruide2.x = reccase_n.x - 20;
								recdruide2.y = reccase_n.y - 77;
								if(!SDL_RenderCopy(renderer, druide2, NULL, &recdruide2));
								//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());
							}
							/*Sinon, si le druide est transformé en felin afficher : le félin*/
							else if(strcmp(j2.perso2->statut, "Felin") == 0){

								recdruide2.x = reccase_n.x - 10;
								recdruide2.y = reccase_n.y - 72;
								recdruide2.w = 100;
								recdruide2.h = 102;
								if(!SDL_RenderCopy(renderer, felin2, NULL, &recdruide2));
								//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());
							}
							/*Sinon, si le druide est transformé en minotaure : afficher le minotaure*/
							else if(strcmp(j2.perso2->statut, "Minotaure") == 0){
								recdruide2.x = reccase_n.x - 15;
								recdruide2.y = reccase_n.y - 87;
								recdruide2.w = 120;
								recdruide2.h = 112;
								if(!SDL_RenderCopy(renderer, minotaure2, NULL, &recdruide2));
									//fprintf(stderr, "Texture non copiée : %s\n", SDL_GetError());
							}
							recdruide2.w = 110;
							recdruide2.h = 107;
						break;
					}
					SDL_map[k].i = i;
					SDL_map[k].j = j;
					SDL_map[k].type = J2P2;
				}
				if(map[i][j] >= 'a' && map[i][j] <= 'z')
					SDL_map[k].type = CASE_BLEUE;
				k++;
			}
			reccase_n.y = reccase_n.y + (h/2) + 3;
			reccase_n.x = reccase_n.x - (w/2) - 8;
			reccase_p.y = reccase_p.y + (h/2) + 3;
			reccase_p.x = reccase_p.x - (w/2) - 8;
			reccase_c.y = reccase_c.y + (h/2) + 3;
			reccase_c.x = reccase_c.x - (w/2) - 8;
			recobstacle.y = recobstacle.y + (h/2) + 3;
			recobstacle.x = recobstacle.x - (w/2) - 8;
		}
	  	
		reccase_n.y = y + (((h/2) + 3)*(i+1));
		reccase_n.x = x + (((w/2) + 8)*(i+1));
		reccase_p.y = y + (((h/2) + 3)*(i+1));
		reccase_p.x = x + (((w/2) + 8)*(i+1));
		reccase_c.y = y + (((h/2) + 3)*(i+1));
		reccase_c.x = x + (((w/2) + 8)*(i+1));
		recobstacle.y = yo + (((h/2) + 3)*(i+1));
		recobstacle.x = xo + (((w/2) + 8)*(i+1));
	}
}
