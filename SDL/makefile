CFLAGS = -lm

OBJ_SDL = sdl_main.o sdl_affichage.o sdl_fenetre.o sdl_outils.o sdl_jeu.o sdl_sorts.o sdl_tour_jeu.o sdl_perso_sorts.o maj.o effets.o fin_partie.o

#Chemin à modifier en fonction de l'emplacement de votre librairie SDL
SDL_DIR=${HOME}/Documents/Jeu/SDL2
SDLLIB_DIR=${SDL_DIR}/lib
SDLINC_DIR=${SDL_DIR}/include

LIBS=-L${SDLLIB_DIR} -lSDL2 -lSDL2_ttf -lSDL2_image
INCLUDES=-I${SDLINC_DIR}

#executable SDL
sdl_main: $(OBJ_SDL) sdl_fonc.h fonc.h
	gcc $(OBJ_SDL) -o $@ ${LIBS} ${INCLUDES}

#génération des .o
maj.o: maj.c fonc.h
	gcc -c $<
effets.o :effets.c fonc.h
	gcc -c $<
fin_partie.o : fin_partie.c fonc.h
	gcc -c $<

# SDL
sdl_main.o: sdl_main.c sdl_fonc.h fonc.h
	gcc -c $< ${LIBS} ${INCLUDES}
sdl_affichage.o: sdl_affichage.c sdl_fonc.h fonc.h
	gcc -c $< ${LIBS} ${INCLUDES}
sdl_fenetre.o:	sdl_fenetre.c sdl_fonc.h fonc.h
	 gcc -c $< ${LIBS} ${INCLUDES}
sdl_outils.o: sdl_outils.c sdl_fonc.h fonc.h
	gcc -c $< ${LIBS} ${INCLUDES}
sdl_jeu.o: sdl_jeu.c sdl_fonc.h fonc.h
	gcc -c $< ${LIBS} ${INCLUDES}
sdl_sorts.o: sdl_sorts.c sdl_fonc.h fonc.h
	gcc -c $< ${LIBS} ${INCLUDES}
sdl_tour_jeu.o: sdl_tour_jeu.c sdl_fonc.h fonc.h
	gcc -c $< ${LIBS} ${INCLUDES}
sdl_perso_sorts.o: sdl_perso_sorts.c sdl_fonc.h fonc.h
	gcc -c $< ${LIBS} ${INCLUDES}

all: sdl_main

clean:
	- rm -rf *.o
	- rm sdl_main

mrproper: clean

