#include "fonc.h"
#include "sdl_fonc.h"

/*******************************
*******VARIABLES GLOBALES*******
********************************/

/* Fenetre et rendu */
SDL_Window* window = NULL;
SDL_Renderer *renderer = NULL;


/* Texture élements de jeu : objets de la map, bouton, image d'accueil, cadre de choix*/
SDL_Texture * texte = NULL;
SDL_Texture * bg = NULL;
SDL_Texture * accueil = NULL;
SDL_Texture * accueil1 = NULL;
SDL_Texture * case_n = NULL;
SDL_Texture * case_p = NULL;
SDL_Texture * case_c = NULL;
SDL_Texture * case_s = NULL;
SDL_Texture * obstacle = NULL;
SDL_Texture * boutonon = NULL;
SDL_Texture * boutonoff = NULL;
SDL_Texture * cadresort = NULL;
SDL_Texture * choix_perso = NULL;
SDL_Texture * coupe = NULL;

/*Texture images joueur 1*/
SDL_Texture * guerrier1 = NULL;
SDL_Texture * archer1 = NULL;
SDL_Texture * tank1 = NULL;
SDL_Texture * druide1 = NULL;

/*Texture images joueur 2*/
SDL_Texture * guerrier2 = NULL;
SDL_Texture * archer2 = NULL;
SDL_Texture * tank2 = NULL;
SDL_Texture * druide2 = NULL;

/*Texture image id joueur 1*/
SDL_Texture * idguerrier1 = NULL;
SDL_Texture * idarcher1 = NULL;
SDL_Texture * idtank1 = NULL;
SDL_Texture * iddruide1 = NULL;

/*Texture image id joueur 2*/
SDL_Texture * idguerrier2 = NULL;
SDL_Texture * idarcher2 = NULL;
SDL_Texture * idtank2 = NULL;
SDL_Texture * iddruide2 = NULL;

/*Texture image de transformations*/ 
SDL_Texture * felin1 = NULL;
SDL_Texture * felin2 = NULL;
SDL_Texture * minotaure1 = NULL;
SDL_Texture * minotaure2 = NULL;

/*Texture bouclier pour le sort Armure */ 
SDL_Texture * bouclier = NULL;

/*Police*/
TTF_Font * police = NULL;

/*Tableaux dans lesquels sont chargés charger les sorts et les persos depuis les fichiers*/
t_personnage * persos[CLASSES+1];
t_sort * sorts[SORTS+1];

/*Variable representant le nombre de tour*/
int nb_tour = 1;

/*Matrice de jeu*/
char map[N][N] = {{'.','.','.','.','.','.','.','.','.','.','.'},
				{'.','.','.','.','.','.','.','.','.','.','.'},

				{'.','.','.','.','.','.','.','.','.','.','.'},

				{'.','.','.','.','.','.','.','.','.','.','.'},

				{'.','.','.','.','.','.','.','.','.','.','.'},

				{'.','.','.','.','.','.','.','.','.','.','.'},

				{'.','.','.','.','.','.','.','.','.','.','.'},

				{'.','.','.','.','.','.','.','.','.','.','.'},

				{'.','.','.','.','.','.','.','.','.','.','.'},

				{'.','.','.','.','.','.','.','.','.','.','.'},

				{'.','.','.','.','.','.','.','.','.','.','.'}};

/*Liste de couleurs*/
SDL_Color noir = {0, 0, 0};
SDL_Color blanc = {255, 255, 255};
SDL_Color orange = {204, 102, 0};
SDL_Color rouge = {139, 0, 0};
SDL_Color jaune = {255, 255, 0};
SDL_Color rouge2 = {255, 0, 0};

int main (int argc, char** argv){


	t_joueur joueur1, joueur2;

	/*Création de la fenetre et des Textures*/
	if(!SDL_creer_fenetre())
		fprintf(stderr, "Erreur à la création de la fenetre : %s\n", SDL_GetError());
	SDL_creer_texture();

	int i, j;
	/*Chargement des sorts et des personnages depuis les fichiers sorts.txt classe.txt*/
	for(int i = 1; i <= SORTS;i++)
		sorts[i] = malloc(sizeof(t_sort));
	creation_sorts(sorts);

	for(int i = 1; i <= CLASSES;i++)
		persos[i] = malloc(sizeof(t_personnage));
	creation_classes(persos,sorts);

	int x = 0, y = 0;
	int numero_perso = 1, indice_joueur = 1, p, q, choix;
	int fin = 0;

	if( window ){

		/*Page d'accueil*/
		SDL_afficher_acceuil();

		int running = 1;

		/*Tant que l'on ne quitte pas la fenêtre ou que l'on ne quitte pas le jeu*/
		while(running && fin == 0){	

			//SDL_GetMouseState(&x,&y);
			
			numero_perso = indice_joueur = nb_tour = 1;
			choix = 0, p = 0, q = 0;

			/*Choix de personnages par joueur*/
			SDL_choix_perso(&joueur1, &joueur2);
			/*Choix des emplacements de personnages par joueur*/
			SDL_initialisation(&joueur1,&joueur2);
			/*Message de début de partie*/
			SDL_afficher_message("Go!", 519, 306, "Police/RunDemo.ttf", 110, 'r', 'N');
			SDL_RenderPresent(renderer);
			SDL_Delay(1000);

			/*Verifier que la fenêtre n'a pas été quittée*/
			SDL_Event e;
			while(SDL_PollEvent(&e)) {
				switch(e.type) {
					case SDL_QUIT: 
						running = 0;
					break;
				}
			}
	        /*Tant qu'aucun des joueurs n'a perdu*/    
			while(! partie_finie(&joueur1) && !partie_finie(&joueur2)){
				
				/*Tour du joueur 1*/
				if(indice_joueur == 1){
					
					SDL_afficher_map(map, joueur1, joueur2);
					SDL_RenderPresent(renderer);

					if(!est_mort(&joueur1, numero_perso))
						SDL_tour(&joueur1,&joueur2,numero_perso);
					
					maj(map,&joueur1,&joueur2);
					SDL_afficher_map(map, joueur1, joueur2);
				}

				/*Tour du joueur 2*/
				else if(indice_joueur == 2){
					
					SDL_afficher_map(map, joueur2, joueur1);
					SDL_RenderPresent(renderer);
					if(!est_mort(&joueur2, numero_perso))
						SDL_tour(&joueur2,&joueur1,numero_perso);

					maj(map,&joueur1,&joueur2);
					SDL_afficher_map(map, joueur1, joueur2);
				}

				/*Tour par tour, un personnage par tour*/
				if(indice_joueur == 2 && numero_perso == 1){
					indice_joueur--;
					numero_perso++;
				}
				else if(indice_joueur == 2 && numero_perso == 2){
					indice_joueur--;
					numero_perso--;
				}
				else if(indice_joueur == 1){
					indice_joueur++;
				}

				/*Verification et mise à jour du nombre de personnages vivant*/
				if(est_mort(&joueur1, 1) || est_mort(&joueur1, 2)){
					if(!est_mort(&joueur1, 1)||!est_mort(&joueur1,2))
						joueur1.nbPersoVivant = 1;
					else 
						joueur1.nbPersoVivant = 0;
				}

				if(est_mort(&joueur2, 1) || est_mort(&joueur2, 2)){
					if(!est_mort(&joueur2, 1)||!est_mort(&joueur2,2))
						joueur2.nbPersoVivant = 1;
					else 
						joueur2.nbPersoVivant = 0;
				}
				nb_tour++;
			}
			/*Message de fin de partie*/
			SDL_afficher_message("Fin de partie", 370, 306, "Police/RunDemo.ttf", 70, 'r', 'N');	
			SDL_RenderPresent(renderer);
			SDL_Delay(1000);

			/*Affichage du vainqueur*/
			SDL_RenderClear(renderer);
			if(partie_finie(&joueur2))
				SDL_afficher_vainqueur(joueur1, -1);	
			else 
				SDL_afficher_vainqueur(joueur2, -1);

			SDL_RenderPresent(renderer);

			/*Attente du choix de l'utilisateur, rejouer ou quitter le jeu*/
			do{
				SDL_Event f;
            	while(SDL_PollEvent(&f)){
	                if(f.type == SDL_QUIT){
	                    SDL_Quit();
	                }
	                else if(f.type == SDL_MOUSEBUTTONDOWN){
	                    p = f.button.x;
	                    q = f.button.y;
	                }
	                if(acces_choix_perso(SDL_choix_jeu, p, q, &choix)){
		                
		                SDL_RenderClear(renderer);
		                
		                if(partie_finie(&joueur2))
							SDL_afficher_vainqueur(joueur1, choix);
							
						else 
							SDL_afficher_vainqueur(joueur2, choix);

	                    SDL_RenderPresent(renderer);
	                }
	            }
        	}while(!acces_choix_perso(SDL_choix_jeu, p, q, &choix));

        	/*Si l'utilisateur souhaite quitter le jeu*/
        	if(choix == 2)
        		fin++;
		}
	} else
		fprintf(stderr,"Erreur de création de la fenêtre: %s\n",SDL_GetError());

	/*Libération les espaces aloués*/
	for(i = 1; i <= SORTS; i++)
		free(sorts[i]);
	
	for(i = 1; i <= CLASSES; i++)
		free(persos[i]);

	/*Destruction de la fenêtre et du rendu*/
	SDL_DestroyRenderer(renderer);
  	SDL_DestroyWindow(window);
  	SDL_Quit();

	return EXIT_SUCCESS;
}
