/*
Par: William Chung et Edouard Larouche
Date : Octobre 2024 - Novembre 2024

Ce module implémente les fonctions pour la gestion de la plaque thermique.
Voici les sous-fonctions:
- lire_fichier: Lit un fichier et initialise la plaque et ses dimensions.
- moyenne_voisins: Calcule la température moyenne des voisins d'une cellule.
- calculer_nouv_plaque: Calcule la nouvelle plaque de températures et vérifie l'équilibre.
- copier_nouv_plaque: Copie la nouvelle plaque et met à jour les min, max et la moyenne.
*/

#define _CRT_SECURE_NO_WARNINGS

#include "plaque_lib.h"
#include <stdio.h>

void lire_fichier(FILE* fichier, t_matrice plaque, t_matbool pos_fixes,
    int* dimy, int* dimx, double* mint, double* maxt) {

    int i, j, nb = 0;

    // Lecture des dimensions de la matrice
    fscanf(fichier, "%d %d", dimy, dimx);

    // Initialisation des valeurs min et max
    *mint = MAXTEMP;  // Valeur arbitrairement grande pour min
    *maxt = -MAXTEMP; // Valeur arbitrairement petite pour max

    // Remplir la matrice avec les températures et calculer min et max
    for (i = 0; i < *dimy; ++i) {
        for (j = 0; j < *dimx; ++j) {
            fscanf(fichier, "%lf", &plaque[i][j]);
            nb++;
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
    fclose(fichier);
}

double moyenne_voisins(const t_matrice plaque, int y, int x, int mode) {
    double somme = 0;
    int n = 0;
    int i;
    int j;
    if (mode == 4) {
        if (y > 0) {
            somme += plaque[y - 1][x];
            n++;
        }
        if (y < MAXLIG - 1) {
            somme += plaque[y + 1][x];
            n++;
        }
        if (x > 0) {
            somme += plaque[y][x - 1];
            n++;
        }
        if (x < MAXCOL - 1) {
            somme += plaque[y][x + 1];
            n++;
        }

        return (somme / n);
    }
    else if (mode == 8) {
        for (i = -1; i <= 1; i++) {
            for (j = -1; j <= 1; j++) {
                if (!(i == 0 && j == 0)) {
                    if (y + i >= 0 && y + i < MAXLIG && x + j >= 0 && x + j < MAXCOL) {
                        somme += plaque[y + i][x + j];
                        n++;
                    }
                }
            }
        }
        return (somme / n);
    }
    return 0.0;
}

int calculer_nouv_plaque(const t_matrice plaque, t_matrice nouv_plaque,
    t_matbool pos_fixes, int dimy, int dimx, int mode,
    double epsilon, double coeff) {

    int j;
    int i;

    int equilibre = 1;

    for (i = 0; i < dimy; ++i) {
        for (j = 0; j < dimx; ++j) {
            if (pos_fixes[i][j] == 0) {
                nouv_plaque[i][j] = (plaque[i][j] * coeff) +
                    (moyenne_voisins(plaque, i, j, mode)) * (1.0 - coeff);
                // Si la différence dépasse epsilon
                if ((nouv_plaque[i][j] - plaque[i][j]) > epsilon) {
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

    double somme = 0.0;
    int i;
    int j;

    // Initialisation des valeurs min et max
    *mint = MAXTEMP;  // Valeur arbitrairement grande pour min
    *maxt = -MAXTEMP; // Valeur arbitrairement petite pour max

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
    }
    return somme / (dimx * dimy);
}
