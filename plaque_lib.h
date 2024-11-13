#pragma once
#define _CRT_SECURE_NO_WARNINGS // Pour désactiver des alertes 
#include <stdio.h>

#define MAXLIG  40         // Dimension maximale en lignes de la plaque
#define MAXCOL  75         // Dimension maximale en colonnes de la plaque
#define MAXTEMP 500        // Température maximale permise

typedef double t_matrice[MAXLIG][MAXCOL];   // Matrice pour les températures de la plaque
typedef int t_matbool[MAXLIG][MAXCOL];      // Matrice pour les positions fixes (0 ou 1)

/*=========================================================*/
/*
 LIRE_FICHIER
 Codée par Larouche Edouard.
 
 Cette fonction lit les données d'un fichier pour initialiser la matrice
 représentant la plaque de température et la matrice des positions fixes.
 Elle enregistre les dimensions de la plaque, ainsi que les
 températures minimale et maximale lues.
 
 PARAMÈTRES :+
   - fichier : Pointeur vers le fichier à lire (type : FILE*).
   - plaque : Matrice de températures de la plaque (type : t_matrice).
   - pos_fixes : Matrice des positions fixes (type : t_matbool).
   - dimy : Pointeur vers la hauteur de la plaque (type : int*).
   - dimx : Pointeur vers la largeur de la plaque (type : int*).
   - mint : Pointeur vers la température minimale (type : double*).
   - maxt : Pointeur vers la température maximale (type : double*).
 
  VALEUR DE RETOUR : Aucune. 

  Explication de la fonction:

    - Cette fonction lit un fichier contenant les données pour initialiser la plaque
      et les positions fixes.
    - Elle remplit la matrice des températures (plaque) et la matrice des
      positions fixes (pos_fixes) avec les valeurs lues.
    - Elle enregistre également les dimensions de la plaque (dimy et dimx) ainsi
      que les températures minimale et maximale (mint et maxt) trouvées dans le
      fichier.

 */
void lire_fichier(FILE* fichier, t_matrice plaque, t_matbool pos_fixes,
    int* dimy, int* dimx, double* mint, double* maxt);

/*=========================================================*/
/*
  MOYENNE_VOISINS
  Codée par William Chung et Larouche Edouard.
 
  Cette fonction calcule la moyenne des températures des voisins
  immédiats d'un point de la plaque, selon le mode spécifié (4 ou 8
  voisins).
 
  PARAMÈTRES :
   - plaque : Matrice de températures de la plaque (type : t_matrice).
   - y : Position en ligne du point (type : int).
   - x : Position en colonne du point (type : int).
   - mode : Mode de voisinage (4 ou 8 voisins) (type : int).
  VALEUR DE RETOUR : La moyenne des températures des voisins (type : double).

  Explication de la fonction moyenne_voisins :

    - Cette fonction calcule la moyenne des températures des voisins d'un point
      (y, x) dans la plaque.
    - Le mode (4 ou 8) détermine si on prend en compte les 4 voisins
     ou les 8 voisins.
    - Elle retourne cette moyenne sous forme de double.
 */
double moyenne_voisins(const t_matrice plaque, int y, int x, int mode);

/*=========================================================*/
/*
  CALCULER_NOUV_PLAQUE
  Codée par Larouche Edouard.
 
  Cette fonction calcule une nouvelle plaque en fonction des valeurs
  de la plaque actuelle et des positions fixes. Elle applique le
  coefficient et epsilon pour chaque point non fixe de la plaque.
 
  PARAMÈTRES :
    - plaque : Matrice de températures de la plaque actuelle (type : t_matrice).
    - nouv_plaque : Matrice pour la nouvelle plaque (type : t_matrice).
    - pos_fixes : Matrice des positions fixes (type : t_matbool).
   - dimy : Nombre de lignes de la plaque (type : int).
*   - dimx : Nombre de colonnes de la plaque (type : int).
    - mode : Mode de voisinage (4 ou 8 voisins) (type : int).
    - epsilon : Valeur de tolérence (type : double).
    - coeff : Coefficient de resistance (type : double).
 
  VALEUR DE RETOUR : Le nombre de points mis à jour dans la nouvelle plaque
  (type : int).
  
   Explication de la fonction:

    - Cette fonction calcule la nouvelle plaque en fonction des températures de
      la plaque actuelle.
    - Pour chaque point non fixe, la température est ajustée selon le
      coefficient et epsilon.
    - Elle met à jour les points de la plaque et retourne le nombre de points
      modifiés.
 */
int calculer_nouv_plaque(const t_matrice plaque, t_matrice nouv_plaque,
    t_matbool pos_fixes, int dimy, int dimx, int mode,
    double epsilon, double coeff);

/*=========================================================*/
/*
 COPIER_NOUV_PLAQUE
 Codée par Larouche Edouard.

 Cette fonction copie les valeurs de la nouvelle plaque dans la
 plaque actuelle et met à jour les températures minimale et maximale
 de la plaque.

 PARAMÈTRES :
   - plaque : Matrice de la plaque actuelle à mettre à jour (type : t_matrice).
   - nouv_plaque : Matrice de la nouvelle plaque (type : t_matrice).
   - dimy : Nombre de lignes de la plaque (type : int).
   - dimx : Nombre de colonnes de la plaque (type : int).
   - mint : Pointeur vers la température minimale (type : double*).
   - maxt : Pointeur vers la température maximale (type : double*).

 VALEUR DE RETOUR : La moyenne des valeurs copiées (type : double).

 Explication de la fonction:

    - Cette fonction copie les valeurs de la nouvelle plaque (nouv_plaque) dans
      la plaque actuelle (plaque).
    - Elle met à jour les températures minimale (mint) et maximale (maxt) de la
      plaque après la copie.
    - La fonction retourne la moyenne des températures de la plaque mise à jour.
 */
double copier_nouv_plaque(t_matrice plaque, const t_matrice nouv_plaque,
    int dimy, int dimx, double* mint, double* maxt);

