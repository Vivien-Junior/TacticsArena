#include "fonc.h"
#include "sdl_fonc.h"

/**
 *\file sdl_tour_jeu.c
 *\brief Fonction du tour de jeu d'un personnage version SDL
 *\author Vivien-Junior
 *\version 0.1
 *\date 13/04/2019
*/

/**
 *\fn void SDL_tour(t_joueur * joueur1,t_joueur * joueur2, int numero_perso)
 *\brief fonction de gestion d'un tour de jeu
 *\param joueur1 joueur actif pendant le tour jeu
 *\param joueur2 joueur passif durant le tour de jeu
 *\param numero_perso numéro du personnage dont c'est le tour
 *\return void
 * la fonction demande au personne s'il souhaite effectuer un déplacement/utiliser un ou des sort(s)/passer son tour
 * une mise à jour de la carte est effectué après avoir effectué une action (déplacement,sort) pour faire disparaitre un joueur qui serait mort après l'utilisation d'un sort
 */
void SDL_tour(t_joueur * joueur1,t_joueur * joueur2, int numero_perso){

	t_personnage * temp;

	if(numero_perso == 1 )
        temp = joueur1->perso1;
    else
        temp = joueur1->perso2;
    /* variable qui compte le nombre de déplacement max possible par personnage*/
	int pm = temp->pm;
    /* variable qui compte le nombre de points d'actions max du personnage 1 */
    int point_action = temp->pa, mouv;

    int choix_action = 0,choix_sort =0;
    temp->sorts[0]->upt=temp->sorts[0]->uptm;
    temp->sorts[1]->upt=temp->sorts[1]->uptm;
    temp->sorts[2]->upt=temp->sorts[2]->uptm;
    temp->sorts[3]->upt=temp->sorts[3]->uptm;

	if (strcmp("Minotaure",temp->statut) == 0)
		temp->pa++;
	if (strcmp("Felin",temp->statut) == 0)
		temp->pm *= 2;
    int x, y;
    /* tant que le joueur ne passe pas son tour OU s'il lui reste des points de déplacements et d'actions */
    while(choix_action != 3){
        x = 0;
        y = 0;
        SDL_RenderClear(renderer);

        if(joueur1->numJoueur == 1)
            SDL_afficher_map(map, * joueur1, * joueur2);
        else
            SDL_afficher_map(map, * joueur2, * joueur1);
        SDL_afficher_menu(* joueur1, numero_perso, -1);
        SDL_RenderPresent(renderer);
        /*Attente du choix de l'action*/
        do{
            SDL_Event f;
            while(SDL_PollEvent(&f)){
                if(f.type == SDL_QUIT){
                    SDL_Quit();
                    return;
                }
                else if(f.type == SDL_MOUSEBUTTONDOWN){
                    x = f.button.x;
                    y = f.button.y;
                }
                if(acces_choix_perso(SDL_choix_menu, x, y, &choix_action)){
                    SDL_afficher_menu(* joueur1, numero_perso, choix_action);
                    SDL_RenderPresent(renderer);
                }
            }
        }while(!acces_choix_perso(SDL_choix_menu, x, y, &choix_action));

        switch(choix_action){
            case 1:
                if(temp->pm > 0 ){
                    if(temp->pm <= 3)
                        temp->pm = SDL_deplacement(joueur1, joueur2, temp->pm, numero_perso);
                    else if(temp->pm > 3){
                        mouv = SDL_deplacement(joueur1, joueur2, 3, numero_perso);
                        temp->pm -= (3 - mouv); 
                    }
                }
                else{
                    SDL_afficher_message("!", 25, 195, "Police/heydings_icons.ttf", 24, 'r', 'N');
                    SDL_afficher_message("Attention : points de mouvements épuisés", 52, 202, "Police/bombarde.ttf", 15, 'b', 'N');
                    SDL_RenderPresent(renderer);
                    SDL_Delay(2500);
                }
            break;
            case 2:
                if(temp->pa > 0){
                    SDL_afficher_menu_sort(* joueur1, numero_perso, -1);
                    SDL_RenderPresent(renderer); 
                    x = 0;
                    y = 0;
                    /*Attente du choix du sort*/
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
                            if(acces_choix_perso(SDL_choix_sort, x, y, &choix_sort)){
                                SDL_afficher_menu_sort(* joueur1, numero_perso, choix_sort);
                                SDL_RenderPresent(renderer);                                
                            }
                        }
                    } while(!acces_choix_perso(SDL_choix_sort, x, y, &choix_sort));

                    switch(choix_sort){
						case 1:
                            if (temp->sorts[0]->point_action <= temp->pa && temp->sorts[0]->upt > 0 ) {
                                temp->sorts[0]->sort(joueur1,joueur2,numero_perso);
								temp->pa -= temp->sorts[0]->point_action ;
								temp->sorts[0]->upt-=1;
                            }
                            else if(temp->sorts[0]->upt == 0){
                                SDL_afficher_message("!", 25, 195, "Police/heydings_icons.ttf", 24, 'r', 'N');
                                SDL_afficher_message("Impossible d'utiliser ce sort pour ce tour", 52, 202, "Police/bombarde.ttf", 15, 'b', 'N');
                                SDL_RenderPresent(renderer);
                                SDL_Delay(2500);
                            }
                            else{
                                SDL_afficher_message("!", 25, 195, "Police/heydings_icons.ttf", 24, 'r', 'N');
                                SDL_afficher_message("Attention : points d'action insuffisants", 52, 202, "Police/bombarde.ttf", 15, 'b', 'N');
                                SDL_RenderPresent(renderer);
                                SDL_Delay(2500);
                            }
                        break;
                        case 2:
                          	if (temp->sorts[1]->point_action <= temp->pa && temp->sorts[1]->upt > 0 ) {
                                temp->sorts[1]->sort(joueur1,joueur2,numero_perso);
								temp->pa -= temp->sorts[1]->point_action ;
								temp->sorts[1]->upt-=1;
                            }
                            else if(temp->sorts[1]->upt == 0){
                                SDL_afficher_message("!", 25, 195, "Police/heydings_icons.ttf", 24, 'r', 'N');
                                SDL_afficher_message("Impossible d'utiliser ce sort pour ce tour", 52, 202, "Police/bombarde.ttf", 15, 'b', 'N');
                                SDL_RenderPresent(renderer);
                                SDL_Delay(2500);
                            }
                            else{
                                SDL_afficher_message("!", 25, 195, "Police/heydings_icons.ttf", 24, 'r', 'N');
                                SDL_afficher_message("Attention : points d'action insuffisants", 52, 202, "Police/bombarde.ttf", 15, 'b', 'N');
                                SDL_RenderPresent(renderer);
                                SDL_Delay(2500);
                            }
                        break;
                        case 3:
                            if (temp->sorts[2]->point_action <= temp->pa && temp->sorts[2]->upt > 0 ) {
								temp->sorts[2]->sort(joueur1,joueur2,numero_perso);
								temp->pa -= temp->sorts[2]->point_action ;
								temp->sorts[2]->upt-=1;
                            }
                            else if(temp->sorts[2]->upt == 0){
                                SDL_afficher_message("!", 25, 195, "Police/heydings_icons.ttf", 24, 'r', 'N');
                                SDL_afficher_message("Impossible d'utiliser ce sort pour ce tour", 52, 202, "Police/bombarde.ttf", 15, 'b', 'N');
                                SDL_RenderPresent(renderer);
                                SDL_Delay(2500);
                            }
                            else{
                               SDL_afficher_message("!", 25, 195, "Police/heydings_icons.ttf", 24, 'r', 'N');
                                SDL_afficher_message("Attention : points d'action insuffisants", 52, 202, "Police/bombarde.ttf", 15, 'b', 'N');
                                SDL_RenderPresent(renderer);
                                SDL_Delay(2500);
                            }     
                        break;
                        case 4:
							if (temp->sorts[3]->point_action <= temp->pa && temp->sorts[3]->upt > 0 ) {
    							temp->sorts[3]->sort(joueur1,joueur2,numero_perso);
    							temp->pa -= temp->sorts[3]->point_action ;
    							temp->sorts[3]->upt-=1;
                            }
                            else if(temp->sorts[3]->upt == 0){
                                SDL_afficher_message("!", 25, 195, "Police/heydings_icons.ttf", 24, 'r', 'N');
                                SDL_afficher_message("Impossible d'utiliser ce sort pour ce tour", 52, 202, "Police/bombarde.ttf", 15, 'b', 'N');
                                SDL_RenderPresent(renderer);
                                SDL_Delay(2500);
                            }
                            else{
                                SDL_afficher_message("!", 25, 195, "Police/heydings_icons.ttf", 24, 'r', 'N');
                                SDL_afficher_message("Attention : points d'action insuffisants", 52, 202, "Police/bombarde.ttf", 15, 'b', 'N');
                                SDL_RenderPresent(renderer);
                                SDL_Delay(2500);
                            }       
                        break;
                        case 5: 
                        break;
                    }
                }
				else{
                    SDL_afficher_message("!", 25, 195, "Police/heydings_icons.ttf", 24, 'r', 'N');
					SDL_afficher_message("Attention : points d'action insuffisants", 52, 202, "Police/bombarde.ttf", 15, 'b', 'N');
                    SDL_RenderPresent(renderer);
                    SDL_Delay(2500);
                }
				case 3:
				break;
        }
		if(joueur1->numJoueur == 1)
	       maj(map,joueur1,joueur2);
        else 
            maj(map,joueur2,joueur1);
    }
    temp->pm = pm;
    temp->pa = point_action;
}