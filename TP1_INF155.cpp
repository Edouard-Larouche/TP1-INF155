#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <assert.h>
#include "WinConsole.h" // Librairie pour la gestion de l'écran et des couleurs

//Macro
#define FFLUSH() while(getchar() != '\n') {}  // Macro pour vider les scanf()

#define MAXLIG  40          //dimension maximale de la plaque
#define MAXCOL  75
#define MAXTEMP 500			  //la température absolue maximale permise
#define COL_DROITE 80

typedef double t_matrice[MAXLIG][MAXCOL];   //type-tableau pour les 2 plaques
typedef int t_matbool[MAXLIG][MAXCOL];      //type-tableau pour les positions fixes

FILE* lire_nom_fich();
void lire_fichier(FILE* fichier, t_matrice plaque, t_matbool pos_fixes,
	int* dimy, int* dimx, double* mint, double* maxt);


void main() {
	int dimy, dimx;
	double mint, maxt;
	t_matrice plaque;
	t_matbool pos_fixes;
	//variable pointeur-fichier
	FILE* fichier = lire_nom_fich();
	lire_fichier(fichier, plaque, pos_fixes, &dimy, &dimx, &mint, &maxt);
}


FILE* lire_nom_fich() {//Cette fonction va demander un nom de fichier(dans la partie droite de l’écran), et essaie d’ouvrir ce fichier.Si l’ouverture du fichier a échoué, on affiche un message d’erreur et on refait la saisie d’un nom de fichier.On répete cette saisie tant que le fichier donné n’est pas valide.Lorsque valide, on garde ce fichier ouvert et on retourne son pointeur(type FILE*).

	FILE* ptr_de_fichier = NULL;
	char nom_saisi[40];
	int ligne_ecriture = 0;

	while (!ptr_de_fichier) {
		gotoxy(COL_DROITE, ++ligne_ecriture);
		printf("entrer le nom du fichier a selectionner");
		gotoxy(COL_DROITE, ++ligne_ecriture);
		scanf("%s", nom_saisi);
		FFLUSH();

		ptr_de_fichier = fopen(nom_saisi, "r");
		if (!ptr_de_fichier) {
			gotoxy(COL_DROITE, ++ligne_ecriture);
			printf("OUPS... Ca marche pas!");
		}
	}
	gotoxy(COL_DROITE, ++ligne_ecriture);
	printf("entree valide");
	return ptr_de_fichier;
}

void lire_fichier(FILE* fichier, t_matrice plaque, t_matbool pos_fixes,
	int* dimy, int* dimx, double* mint, double* maxt) {

	// Lecture des dimensions de la matrice
	fscanf(fichier, "%d %d", dimy, dimx);

	// Initialisation des valeurs min et max
	*mint = 1000000;  // Valeur arbitrairement grande pour min
	*maxt = -1000000; // Valeur arbitrairement petite pour max

	// Remplir la matrice avec les températures et calculer min et max
	for (int i = 0; i < *dimy; ++i) {
		for (int j = 0; j < *dimx; ++j) {
			fscanf(fichier, "%lf", &plaque[i][j]);

			// Mettre à jour les températures min et max
			if (plaque[i][j] < *mint) {
				*mint = plaque[i][j];
			}
			if (plaque[i][j] > *maxt) {
				*maxt = plaque[i][j];
			}

			// Fixer les bords de la matrice comme positions fixes
			if (i == 0 || i == (*dimy - 1) || j == 0 || j == (*dimx - 1)) {
				pos_fixes[i][j] = true;
			}
			else {
				pos_fixes[i][j] = false;
			}
		}
	}
}




