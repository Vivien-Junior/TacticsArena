#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "define.h"
#ifndef LARGEUR
#define LARGEUR 1340
#endif
#ifndef HAUTEUR
#define HAUTEUR 800
#endif
#ifndef N
#define N 11
#endif
#ifndef NBPERSOS
#define NBPERSOS 4
#endif
#ifndef NBACTION
#define NBACTION 3
#endif

extern SDL_Window* window;
extern SDL_Renderer *renderer;

extern SDL_Texture * texte;
extern SDL_Texture * bg;
extern SDL_Texture * accueil;
extern SDL_Texture * accueil1;
extern SDL_Texture * case_n;
extern SDL_Texture * case_p;
extern SDL_Texture * case_c;
extern SDL_Texture * case_s;
extern SDL_Texture * obstacle;
extern SDL_Texture * boutonon;
extern SDL_Texture * boutonoff;
extern SDL_Texture * coupe;
extern SDL_Texture * choix_perso;
extern SDL_Texture * cadresort;

extern SDL_Texture * guerrier1;
extern SDL_Texture * archer1;
extern SDL_Texture * tank1;
extern SDL_Texture * druide1;

extern SDL_Texture * guerrier2;
extern SDL_Texture * archer2;
extern SDL_Texture * tank2;
extern SDL_Texture * druide2;

extern SDL_Texture * idguerrier1;
extern SDL_Texture * idarcher1;
extern SDL_Texture * idtank1;
extern SDL_Texture * iddruide1;

extern SDL_Texture * idguerrier2;
extern SDL_Texture * idarcher2;
extern SDL_Texture * idtank2;
extern SDL_Texture * iddruide2;

extern SDL_Texture * felin1;
extern SDL_Texture * felin2;
extern SDL_Texture * minotaure1;
extern SDL_Texture * minotaure2;


extern SDL_Color noir;
extern SDL_Color blanc;
extern SDL_Color orange;
extern SDL_Color rouge;
extern SDL_Color jaune;
extern SDL_Color rouge2;

extern SDL_Texture * bouclier;

extern TTF_Font * police;
extern int nb_tour;
extern char * chaine;

typedef enum {CASE_GRISE, CASE_BLEUE, OBSTACLE, J1P1, J1P2, J2P1, J2P2}t_type_case;

/**
 *\struct t_coord
 *\brief structure de coordonnées
 * structure qui contient des points d'abscisse et d'ordonnée
 */
typedef struct s_coord{
	int x, y;
}t_coord;

/**
 *\struct t_case
 *\brief structure des cases
 * structure qui contient le type de la case, son emplacement dans matrice et ses coordonnées sur la fenêtre
 * vivant dans l'équipe et le numéro de l'équipe
 */
typedef struct s_case {
	t_type_case type;
	int i;/** coordonnées de la ligne*/
	int j;/** coordonnées de la colonne*/
	t_coord a;/** coordonnées point A*/
	t_coord b;/** coordonnées point B*/
	t_coord c;/** coordonnées point C*/
	t_coord d;/** coordonnées point D*/
}t_case;

/**
 *\struct t_choix
 *\brief structure des menus
 * structure qui contient le numéro du choix, et les coordonnées son cadre sur la fenêtre
 * vivant dans l'équipe et le numéro de l'équipe
 */
typedef struct s_choix{
	int num_choix;/** numéro du choix*/
	t_coord a;/** coordonnées point A*/
	t_coord b;/** coordonnées point B*/
	t_coord c;/** coordonnées point C*/
}t_choix;

extern char map[N][N];
t_case SDL_map[N * N];
t_choix SDL_choix_p[NBPERSOS];
t_choix SDL_choix_menu[NBACTION];
t_choix SDL_choix_sort[MAX_NB_SORT + 1];
t_choix SDL_choix_jeu[2];

extern t_personnage * persos[CLASSES+1];
extern t_sort * sorts[SORTS+1];

/****** AFFICHAGES ******/
void SDL_choix_perso();
void SDL_afficher_bg();
void SDL_afficher_acceuil();
void SDL_afficher_map(char map[N][N], t_joueur j1, t_joueur j2);
void SDL_afficher_message(char * message, int x, int y, char * police, int taille, char couleur, char style);
void SDL_afficher_degat(t_joueur j1, t_joueur j2, t_personnage * perso, int degat, char type);
void SDL_affichage_choix_perso(int num_j);
void SDL_afficher_perso_choisi(t_joueur joueur1, t_joueur joueur2);
void SDL_afficher_sort(char * nom, int xsort, int ysort);
void SDL_afficher_point_de_vie(t_joueur joueur1, t_joueur joueur2);
void SDL_affficher_id_perso(t_personnage * perso, int c, int xi, int yi, int taille_p, int num_j);
void SDL_afficher_barre_point_de_vie(t_personnage * perso, int c, int xi, int yi);
void SDL_afficher_menu(t_joueur joueur, int numero_perso, int select);
void SDL_afficher_menu_sort(t_joueur joueur, int numero_perso, int select);
void SDL_afficher_vainqueur(t_joueur joueur, int select);

/****** INITIALISATION DE LA SDL *****/
int SDL_creer_fenetre();
void SDL_creer_texture();
SDL_Texture * SDL_charger_image (char * image);

/********* OUTILS *******/
void SDL_ecrire_texte(char * chaine, char * police_ec, int taille, char color, char style);
float eq_droite(t_coord p1, t_coord p2, int x);
int dans_case(t_coord a, t_coord b, t_coord c, t_coord d, int x, int y);
int acces_possible(int x, int y, int * ymap, int * xmap);
int acces_choix_perso(t_choix SDL_choix[], int x, int y, int * classe);
void effet_clignotement(t_joueur j1, t_joueur j2, t_personnage * perso);
t_case recherche_case(int i, int j);

/****** FONCTION DE JEU *****/
void SDL_choix_perso(t_joueur *joueur1, t_joueur *joueur2);
void SDL_initialisation(t_joueur * joueur1,t_joueur * joueur2);
int SDL_deplacement(t_joueur * j1,t_joueur * j2, int  n, int numero_perso);
void SDL_tour(t_joueur * joueur1,t_joueur * joueur2,int numero_perso);

/*********** SORTS *********/
void SDL_saut(t_joueur * j1,t_joueur * j2, int numero_perso);
void SDL_petit_coup (t_joueur * j1, t_joueur * j2,int numero_perso);
void SDL_soin (t_joueur * j1, t_joueur * j2,int numero_perso);
void SDL_grosCoup (t_joueur * j1, t_joueur * j2,int numero_perso);

void SDL_diago (t_joueur * j1, t_joueur * j2,int numero_perso);
void SDL_ligne (t_joueur * j1, t_joueur * j2,int numero_perso);
void SDL_double_tape (t_joueur * j1, t_joueur * j2,int numero_perso);
void SDL_coup_zone (t_joueur * j1, t_joueur * j2,int numero_perso);

void SDL_armure (t_joueur * j1, t_joueur * j2,int numero_perso);
void SDL_attire(t_joueur * j1, t_joueur * j2,int numero_perso);
void SDL_chenchen (t_joueur * j1, t_joueur * j2,int numero_perso);
void SDL_bigshaq (t_joueur * j1, t_joueur * j2,int numero_perso);

void SDL_minotaure (t_joueur * j1, t_joueur * j2,int numero_perso);
void SDL_felin (t_joueur * j1, t_joueur * j2,int numero_perso);
void SDL_fuego(t_joueur * j1, t_joueur * j2,int numero_perso);
void SDL_revitalisation (t_joueur * j1, t_joueur * j2,int numero_perso);