#pragma once
#define _CRT_SECURE_NO_WARNINGS // Pour désactiver des alertes 
#include <stdio.h>

#define MAXLIG  40         // Dimension maximale en lignes de la plaque
#define MAXCOL  75         // Dimension maximale en colonnes de la plaque
#define MAXTEMP 500        // Température maximale permise

typedef double t_matrice[MAXLIG][MAXCOL];   // Matrice pour les températures de la plaque
typedef int t_matbool[MAXLIG][MAXCOL];      // Matrice pour les positions fixes (0 ou 1)

void lire_fichier(FILE* fichier, t_matrice plaque, t_matbool pos_fixes,
    int* dimy, int* dimx, double* mint, double* maxt);

double moyenne_voisins(const t_matrice plaque, int y, int x, int mode);

int calculer_nouv_plaque(const t_matrice plaque, t_matrice nouv_plaque,
    t_matbool pos_fixes, int dimy, int dimx, int mode,
    double epsilon, double coeff);

double copier_nouv_plaque(t_matrice plaque, const t_matrice nouv_plaque,
    int dimy, int dimx, double* mint, double* maxt);

