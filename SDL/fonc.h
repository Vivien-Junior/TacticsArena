#include "define.h"

/**
 *\file fonc.h
 *\brief Prototype des fonctions et structures du jeu
 *\author Marius
 *\author ALexandre
 *\author Vivien-Junior
 *\author Dylan
 *\version 0.1
 *\date 28/02/2019
*/

typedef struct s_personnage t_personnage;

/**
 *\struct t_joueur
 *\brief structure des joueurs
 * structure qui contient tous les personnages d'un joueur, le nombre de personnages vivants dans l'équipe et le numéro de l'équipe
 */
typedef struct s_joueur{
		int numJoueur;/** numéro du joueur */
		int nbPersoVivant;/** nombre de personnages vivants*/
		t_personnage * perso1;/** personnage 1*/
		t_personnage * perso2;/** personnage 2*/
}t_joueur;

/**
 *\struct t_sort
 *\brief Structure des sorts
 * structure qui contient toutes les informations utiles pour un sort (nom,portée,dégâts,utilisation par tour..)
 * la structure contient aussi un pointeur sur fonction avec le meme corps que toutes les fonctions
*/
typedef struct s_sort{
	int id;/** numéro du sort*/
	char * nom;/** nom du sort*/
	int portee;/** portée du sort */
	int degat;/** dégâts infligés par le sort*/
	int cd; /** cooldown*/
	int upt; /** utilisation par tour*/
  	int uptm;/** utilisation par tour maximale*/
	int point_action;/** nombre de points d'actions minimum pour pouvoir utiliser le sort*/
	void (*sort)(t_joueur *,t_joueur *,int);/** fonction du sort*/
}t_sort;

/**
 *\struct t_coordonnees
 *\brief Structure contenant les coordonnées du personnage
*/
typedef struct s_coordonnees{
	int x;
	int y;
}t_coordonnees;

/**
 *\struct t_effet
 *\brief Structure pour les effets
 *  structure contenant le nom, la durée et les coordonées de la cible sur lequel ont souhaite effectuer l'effet
*/
typedef struct s_effet{
	char * nom;
	int duree;
	t_coordonnees cible;
}t_effet;

/**
 *\struct t_personnage
 *\brief Structure des personnages
 * structure d'un personnage ayant un nom,une position (x;y), des points de vie, des points d'actions lui permettant d'utiliser ses sorts et quatre sorts
*/
struct s_personnage{
	int id;/** numéro du personnage*/
	char* nom;/** nom du personnage*/
	int pv;/** nombre de points de vie du personnage*/
	int pv_max;/** nombre maximal de points de vie  du personnage*/
	int pa;/** points d'actions disponibles du personnage*/
	int pm;/** points de mouvement disponibles du personnage */
	t_coordonnees coord;/** position du personnage*/
	t_sort * sorts[MAX_NB_SORT];/** liste des sorts du personnage*/
	t_effet effets[MAX_NB_EFFET];/** effets actuels sur le personnage */
	char statut[50];/** Statut de transformation du personnage*/
};

/*fonction qui crée un personnage(affection des sorts,vie,points d'actions pour le type de personnage choisi) */
t_personnage * creer_perso(int);
/*affichage de tout les sorts du personnage choisi pour le joueur*/
void affichage_sort(t_personnage *);
/* affichage de la matrice */
void affichage_map(char map[N][N]);
/* fonction qui retourne le caractère du personnage i du joueur n */
char carac_perso(int ,int );
/* fonction booleenne qui renvoie vrai si la vie du personnage est inférieur ou égale à zéro */
int est_mort(t_joueur * , int );
/* mise à jour de la carte suite à une déplacement d'un joueur */
void maj(char map[N][N], t_joueur * , t_joueur * );
/*fonction d'initialisation (positionnement,création des obstacles) */
void initialisation(char matriceJeu[N][N],t_joueur * ,t_joueur * );
/* affichage des coordonnées actuelles du personnage */
void affichage_coord(t_joueur * );
/* affichage des sorts utilisable par le joueur */
void sort_uti(t_personnage * );
/*fonction de tour de jeu (se déplacer,utiliser un sort, passer son tour) */
void tour(char map[N][N],t_joueur * ,t_joueur * ,int  );
/* fonction qui teste si la case ciblé existe sur la carte temporaire et retourne les coordonnées de la case ciblée */
int existe(char mat[N][N], char , int * , int * );
/* fonction qui verifie si le joueur passé en paramètre n'a plus de personnages */
int partie_finie(t_joueur * );
void creer_effet(t_personnage *,int ,int ,int );
void suppr_sort(t_sort ** );
void suppr_perso(t_personnage ** );
void (*creer_sort(int id_sort))(t_joueur* ,t_joueur* ,int);
void creation_sorts(t_sort * sorts[]);
void creation_classes(t_personnage * persos[],t_sort * sorts[]);
t_sort * copie_sort(t_sort * sort);
t_personnage * copie_perso(t_personnage * );
/* sorts */
void armure  (char map[N][N], t_personnage * ,t_joueur *, t_joueur *,int ,int ,int );
void felin  (char map[N][N], t_personnage * ,t_joueur *, t_joueur *,int ,int ,int );
void chouette  (char map[N][N], t_personnage * ,t_joueur *, t_joueur *,int ,int ,int );
void attire  (char map[N][N], t_personnage * ,t_joueur *, t_joueur *,int ,int ,int );
void chenchen  (char map[N][N], t_personnage * ,t_joueur *, t_joueur *,int ,int ,int );
void bigshaq  (char map[N][N], t_personnage * ,t_joueur *, t_joueur *,int ,int ,int );
void diago  (char map[N][N], t_personnage * ,t_joueur *, t_joueur *,int ,int ,int );
void coup_zone  (char map[N][N], t_personnage * ,t_joueur *, t_joueur *,int ,int ,int );
void double_tape  (char map[N][N], t_personnage * ,t_joueur *, t_joueur *,int ,int ,int );
void petit_coup  (char map[N][N], t_personnage * ,t_joueur *, t_joueur *,int ,int ,int );
void ligne  (char map[N][N], t_personnage * ,t_joueur *, t_joueur *,int ,int ,int );;
void grosCoup  (char map[N][N], t_personnage * ,t_joueur *, t_joueur *,int ,int ,int );
void soin  (char map[N][N], t_personnage * ,t_joueur *, t_joueur *,int ,int ,int );
void saut  (char map[N][N], t_personnage * ,t_joueur *, t_joueur *,int ,int ,int );
void fuego  (char map[N][N], t_personnage * ,t_joueur *, t_joueur *,int ,int ,int );
void revitalisation  (char map[N][N], t_personnage * ,t_joueur *, t_joueur *,int ,int ,int );
/* fonction déplacement case par case pour le joueur */
int deplacement(t_joueur * j1,t_joueur * j2,char map[N][N],int ,int  );
