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
#define EPSILON 1.0
#define ESPACEMENT_COLONES 5
#define ESPACEMENT_RANGEES 5


typedef double t_matrice[MAXLIG][MAXCOL];   //type-tableau pour les 2 plaques

typedef int t_matbool[MAXLIG][MAXCOL];      //type-tableau pour les positions fixes

FILE* lire_nom_fich();
void lire_fichier(FILE* fichier, t_matrice plaque, t_matbool pos_fixes,
	int* dimy, int* dimx, double* mint, double* maxt);

void lire_fichier(FILE* fichier, t_matrice plaque, t_matbool pos_fixes,
	int* dimy, int* dimx, double* mint, double* maxt);

int calculer_nouv_plaque(const t_matrice plaque, t_matrice nouv_plaque,
	t_matbool p_fixes, int dimy, int dimx, int mode, double epsilon, double coeff);

double moyenne_voisins(const t_matrice plaque, int y, int x, int mode);

double copier_nouv_plaque(t_matrice plaque, const t_matrice nouv_plaque,
	int dimy, int dimx, double* mint, double* maxt);

int simulation(t_matrice plaque, t_matbool pos_fixes, int dimy, int dimx,
	double* mint, double* maxt, int mode, double epsilon, double coeff);

void main() {
	int dimy, dimx, mode = 4, coeff = 0.5;
	double mint, maxt;

	t_matrice plaque;
	t_matbool pos_fixes;
	//variable pointeur-fichier
	FILE* fichier = lire_nom_fich();

	lire_fichier(fichier, plaque, pos_fixes, &dimy, &dimx, &mint, &maxt);
	simulation(plaque, pos_fixes, dimy, dimx,
		&mint, &maxt, mode, EPSILON, coeff);
	

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
				pos_fixes[i][j] = 1;
			}
			else {
				pos_fixes[i][j] = 0;
			}
		}
	}
}

double moyenne_voisins(const t_matrice plaque, int y, int x, int mode) {

	double moyenne = 0;
	double valeur = 0;
	double somme = 0;

	if (mode = 4) {
		valeur += plaque[y + 1][x];
		valeur += plaque[y + -1][x];
		valeur += plaque[y][x + 1];
		valeur += plaque[y][x - 1];

		moyenne = valeur / 4;
		return moyenne;
	}

	if (mode = 8) {
		for (y = -1; y <= 1; ++y) {
			for (x = -1; x <= 1; ++x) {
				if (!plaque[0][0]) {
					somme = somme + plaque[y][x];
				}
			}
		}
		moyenne = (somme / 8);
		return moyenne;
	}
}

int calculer_nouv_plaque(const t_matrice plaque, t_matrice nouv_plaque,
	t_matbool pos_fixes, int dimy, int dimx, int mode, double epsilon, double coeff) {
	int j, i;
	int equilibre = 1;

	for (i = 0; i < dimy; ++i) {
		for (j = 0; j < dimx; ++j) {
			if (pos_fixes[i][j] == 0) {
				nouv_plaque[i][j] = (plaque[i][j] * coeff) + (moyenne_voisins(plaque, i, j, mode)) * (1.0 - coeff);
				//si cette valeur depasse la difference epsilon
				if (plaque[i][j] - nouv_plaque[i][j] <= epsilon) {
					equilibre = 0;
				}
			}
			else {
				nouv_plaque[i][j] = plaque[i][j];
			}
		}
	}
	if (equilibre == 0) {
		return 0;
	}
	else {
		return 1;
	}

}

double copier_nouv_plaque(t_matrice plaque, const t_matrice nouv_plaque,
	int dimy, int dimx, double* mint, double* maxt) {

	int j, i;
	int equilibre = 1;
	int somme = 0;

	for (i = 0; i < dimy; ++i) {
		for (j = 0; j < dimx; ++j) {
			plaque[i][j] = nouv_plaque[i][j];

			// Mettre à jour les températures min et max
			if (plaque[i][j] < *mint) {
				*mint = plaque[i][j];
			}
			if (plaque[i][j] > *maxt) {
				*maxt = plaque[i][j];
			}

			somme += plaque[i][j];
		}
		return somme / (dimx * dimy);
	}
}

int simulation(t_matrice plaque, t_matbool pos_fixes, int dimy, int dimx,
	double* mint, double* maxt, int mode, double epsilon, double coeff) {
	int i, j;
	int iteration = 0;
	int equilibre = 0;
	int moyenne;
	t_matrice nouv_plaque;

	do {
		equilibre = calculer_nouv_plaque(plaque, nouv_plaque, pos_fixes, dimy, dimx, mode, EPSILON, coeff);
		moyenne = copier_nouv_plaque(plaque, nouv_plaque,
			dimy, dimx, mint, maxt);

		for (i = 0; i < dimy; ++i) {
			for (j = 0; j < dimx; ++j) {
				gotoxy(j * ESPACEMENT_COLONES,
					i * ESPACEMENT_RANGEES);
				printf("%lf", plaque[i][j]);
			}
		}
		iteration++;

		printf("Max : %lf | Min : %lf | Moyenne : %lf", mint, maxt, moyenne);
	} while (equilibre != 1);
	return iteration;
}
