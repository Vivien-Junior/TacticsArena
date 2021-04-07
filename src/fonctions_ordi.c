#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "fonc.h"

#define NB_PERSONNAGES 2
#define NB_CLASSE 4

/*
 * Fonction de choix des classes et
 *
 */
int choix_personnage_ordi(t_equipe * ordi,int nb_classe,int nb_personnages){
    srand(time(NULL)); // initialisation de rand
    int classe = 0,i = 1;
    while(i <= nb_personnages){
        classe = rand()%nb_classe+1;
        printf("choix: %i\n",classe);
        switch(i) {
            //case 1: ordi->perso1 = copie_perso(classe);break;
            //case 2: ordi->perso2 = copie_perso(classe);break;
        }
        i++;
    }
    ordi->numEquipe = 2;
    ordi->nbPersoVivant = NB_PERSONNAGES;
}



int main() {
    t_equipe * ordi;

    choix_personnage_ordi(ordi,NB_CLASSE,NB_PERSONNAGES);
    affichage_coord(ordi);



    return 0;
}
