#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "plaque_lib.h"
#include <conio.h>
#include <ctype.h> 

#define MAXLIG  40          // Dimension maximale de la plaque
#define MAXCOL  75
#define MAXTEMP 500			  // La température absolue maximale permise
#define COL_DROITE 80
#define EPSILON 1.0
#define ESPACEMENT_COLONES 3 // Pour permettre une lecture plus facile
#define ESPACEMENT_RANGEES 3   // Pour permettre une lecture plus facile
#define NB_COUL  15 
#define LINGNE_INFO_TABLE 40

#define FFLUSH() while(getchar() != '\n') {}  // Macro pour vider les scanf()

FILE* lire_nom_fich();

void colorage_plaque(const t_matrice plaque, int dimy, int dimx, double mint,
    double maxt);

int simulation(t_matrice plaque, t_matbool pos_fixes, int dimy, int dimx,
    double* mint, double* maxt, int mode, double epsilon, double coeff);

unsigned char afficher_menu(double* epsilon, double* coeff, int* mode, int* init,
    t_matrice plaque, t_matbool pos_fixes, int* dimy, int* dimx, double* mint,
    double* maxt);
