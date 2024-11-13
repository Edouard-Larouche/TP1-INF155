#pragma once

#include "plaque_lib.h"
#include <conio.h>
#include <ctype.h> 

#define MAXLIG  40          // Dimension maximale du nombre de lignes de la plaque (hauteur)
#define MAXCOL  75          // Dimension maximale du nombre de colonnes de la plaque (largeur)
#define MAXTEMP 500         // La température maximale permise sur la plaque
#define COL_DROITE 80       // Position de la colonne pour l'affichage dans l'interface
#define EPSILON 1.0         // Valeur de epsilon
#define ESPACEMENT_COLONES 3 // Espacement entre les colonnes 
#define ESPACEMENT_RANGEES 3  // Espacement entre les rangées 
#define NB_COUL  15         // Nombre de couleurs utilisées pour visualisé la plaque
#define LINGNE_INFO_TABLE 40 // Nombre de lignes d'information à afficher dans la table d'affichage
#define MIN_EPS 0.01        // Valeur minimale d'epsilon
#define MAX_EPS 10.0        // Valeur maximale d'epsilon
#define MIN_COEF 0.20       // Valeur minimale du coefficient 
#define MAX_COEF 0.80       // Valeur maximale du coefficient 

#define FFLUSH() while(getchar() != '\n') {}  // Macro pour vider les scanf()

/*
 LIRE_NOM_FICH
 Codée par Larouche Edouard
 
 Cette fonction permet de lire le nom d'un fichier depuis l'entrée
 standard et retourne un pointeur vers le fichier ouvert.
 
 PARAMÈTRES :
 Aucun (fonction sans paramètre).
 
 VALEUR DE RETOUR : Un pointeur de type FILE pointant vers le fichier
 ouvert. Si l'ouverture échoue, retourne NULL.

 */
FILE* lire_nom_fich();

void colorage_plaque(const t_matrice plaque, int dimy, int dimx, double mint,
    double maxt);

int simulation(t_matrice plaque, t_matbool pos_fixes, int dimy, int dimx,
    double* mint, double* maxt, int mode, double epsilon, double coeff);

void affichage_plaque(const t_matrice plaque, int dimy, int dimx, double mint, double maxt);

void effacer_plaque(int nblig);

void dessiner_echelle(double mint, double maxt);

void aff_options(int mode, double epsi, double coeff_res);

void aff_minmax_moy(double mint, double maxt, double moyenne);

void aff_nb_iter(int nb_iter);

int valider_mode_voisins();

double valider_reel(const char* message, double b_min, double b_max);

unsigned char afficher_menu();

void pt_variation(t_matrice plaque, t_matbool pos_fixes, int dimy, int dimx, double* mint, double* maxt, int fixe);
