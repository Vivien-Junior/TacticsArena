#include "define.h"


/**
 *\file fonc.h
 *\brief prototype des fonctions et structures du jeu
 *\author Marius
 *\author ALexandre
 *\author Vivien-Junior
 *\author Dylan
 *\version 0.1
 *\date 28/02/2019
*/

typedef struct s_personnage t_personnage;

/**
 *\struct t_equipe
 *\brief structure des equipes
 * structure qui contient tout les personnages d'un equipe, le nombre de personnage
 * vivant dans l'équipe et le numéro de l'équipe
 */
typedef struct s_equipe{
		int numEquipe; /** numéro de l'équipe */
		int nbPersoVivant; /** nombre de personnages vivants*/
		t_personnage * perso1; /** personnage 1*/
		t_personnage * perso2; /** personnage 2*/
		int client_socket;
}t_equipe;

/**
 *\struct t_sort
 *\brief structure des sorts
 * structure qui contient toutes les informations utiles sur un sort (nom,portée,dégâts,utilisation par tour..)
 * la structure contient aussi un pointeur sur fonction avec le meme corps que toutes les fonctions
 */
typedef struct s_sort{

	int id; /** numéro du sort*/
	char * nom; /** nom du sort*/
	int portee; /** portée du sort */
	int degat; /** dégât infligés par le sort*/
	int cd; /** cooldown*/
	int upt; /** utilisation par tour*/
  int uptm; /** utilisation par tour maximale*/
	int point_action; /** nombre de points d'actions minimum pour pouvoir utiliser le sort*/
	int (*sort)(char map[N][N], t_personnage * , t_equipe *,t_equipe *,int,int,int,int,int);

}t_sort;


/**
 *\struct t_coordonnees
 *\brief structure contenant les coordonnées du personnage
 */
typedef struct s_coordonnees{

	int x;
	int y;

}t_coordonnees;

/**
 *\struct t_effet
 *\brief structure pour les effets
 *  structure contenant le nom, la durée et les coordonées de la cible sur lequel ont souhaite effectuer l'effet
 */
typedef struct s_effet{

	char * nom;
	int duree;
	t_coordonnees cible;

}t_effet;


/**
 *\struct t_personnage
 *\brief structure des personnages
 * structure d'un personnage ayant un nom,une position x/y, des points de vie, des points d'actions lui permettant d'utiliser ses sorts et 4sorts
 */
struct s_personnage{

	int id; /** numéro du personnage*/
	char* nom; /** nom du personnage */
	int pv; /** nombre de points de vie du personnage */
	int pv_max; /** nombre maximal de points de vie  du personnage*/
	int pa; /** points d'actions disponibles du personnage */
	int pa_max;
	int pm; /** points de mouvement disponibles du personnage */
	t_coordonnees coord; /** position du personnage */
	t_sort * sorts[MAX_NB_SORT]; /** liste des sorts du personnage */
	t_effet effets[MAX_NB_EFFET];/** effets actuels sur le personnage */
};

//fonctions de réseaux
int serveur (int nb_joueur, t_personnage * persos[CLASSES+1], t_equipe * ,t_equipe *);
int client (  int argc, char** argv );
void envoie_map(char map[N][N], t_equipe ** , int , int );




/*fonction qui crée un personnage(affection des sorts,vie,points d'actions pour le type de personnage choisi) */
t_personnage * creer_perso(int);
/*affichage de tout les sorts du personnage choisi pour le equipe*/
void affichage_sort(t_personnage *);
/* affichage de la matrice */
void affichage_map(char map[N][N]);
/* fonction qui retourne le caractère du personnage i du equipe n */
char carac_perso(int ,int );
/* fonction booleenne qui renvoie vrai si la vie du personnage est inférieur ou égale à zéro */
int est_mort(t_equipe * , int );
/* mise à jour de la carte suite à une déplacement d'un equipe */
void maj(char map[N][N], t_equipe * , t_equipe * );
/*fonction d'initialisation (positionnement,création des obstacles) */
void initialisation(char matriceJeu[N][N],t_equipe * ,t_equipe * );
/* affichage des coordonnées actuelles du personnage */
void affichage_coord(t_equipe * );
/* affichage des sorts utilisable par le equipe */
void sort_uti(t_personnage * );
/*fonction de tour de jeu (se déplacer,utiliser un sort, passer son tour) */
void tour(char map[N][N],t_equipe * ,t_equipe * ,int  );
/* fonction qui teste si la case ciblé existe sur la carte temporaire et retourne les coordonnées de la case ciblée */
int existe(char mat[N][N], char , int * , int * );
/* fonction qui verifie si le equipe passé en paramètre n'a plus de personnages */
int partie_finie(t_equipe * );

void creer_effet(t_personnage *,int ,int ,int );

void suppr_sort(t_sort ** );
void suppr_perso(t_personnage ** );
int (*creer_sort(int id_sort))(char [N][N], t_personnage * , t_equipe* ,t_equipe* ,int,int ,int,int,int);
void creation_sorts(t_sort * sorts[]);
void creation_classes(t_personnage * persos[],t_sort * sorts[]);
t_sort * copie_sort(t_sort * sort);
t_personnage * copie_perso(t_personnage * );




/* sorts */
int armure  (char map[N][N], t_personnage * ,t_equipe *, t_equipe *,int ,int ,int ,int , int );
int minotaure  (char map[N][N], t_personnage * ,t_equipe *, t_equipe *,int ,int ,int  ,int, int );
int felin  (char map[N][N], t_personnage * ,t_equipe *, t_equipe *,int ,int ,int ,int, int );
int attire  (char map[N][N], t_personnage * ,t_equipe *, t_equipe *,int ,int ,int ,int, int );
int chenchen  (char map[N][N], t_personnage * ,t_equipe *, t_equipe *,int ,int ,int ,int, int );
int bigshaq  (char map[N][N], t_personnage * ,t_equipe *, t_equipe *,int ,int ,int ,int, int );
int diago  (char map[N][N], t_personnage * ,t_equipe *, t_equipe *,int ,int ,int ,int, int );
int coup_zone  (char map[N][N], t_personnage * ,t_equipe *, t_equipe *,int ,int ,int ,int, int );
int double_tape  (char map[N][N], t_personnage * ,t_equipe *, t_equipe *,int ,int ,int ,int, int );
int petit_coup  (char map[N][N], t_personnage * ,t_equipe *, t_equipe *,int ,int ,int ,int, int );
int ligne  (char map[N][N], t_personnage * ,t_equipe *, t_equipe *,int ,int ,int ,int, int );;
int grosCoup  (char map[N][N], t_personnage * ,t_equipe *, t_equipe *,int ,int ,int ,int, int );
int soin  (char map[N][N], t_personnage * ,t_equipe *, t_equipe *,int ,int ,int ,int, int );
int saut  (char map[N][N], t_personnage * ,t_equipe *, t_equipe *,int ,int ,int ,int, int );
int fuego  (char map[N][N], t_personnage * ,t_equipe *, t_equipe *,int ,int ,int ,int, int );
int revitalisation  (char map[N][N], t_personnage * ,t_equipe *, t_equipe *,int ,int ,int ,int, int );

/* fonction déplacement case par case pour le equipe */
int deplacement(t_equipe * ,t_equipe * ,char map[N][N],int ,int  );

/** affichage portee et application des degats */

int portee_degat (char map[N][N], t_personnage * ,t_equipe * , t_equipe * ,int ,int ,int , int , int );

t_coordonnees range(char map[N][N], t_personnage * ,t_equipe * , t_equipe * ,int ,int ,int,int, int );

int damage(char map[N][N], t_personnage * ,t_equipe * , t_equipe * ,int ,int ,int , int ,t_coordonnees, int );
