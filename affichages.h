#pragma once

#include "plaque_lib.h"
#include <conio.h>
#include <ctype.h> 
/*  */
/* Dimension maximale du nombre de lignes de la plaque (hauteur) */
#define MAXLIG  40         

/* Dimension maximale du nombre de colonnes de la plaque (largeur) */
#define MAXCOL  75          

#define MAXTEMP 500         // La température maximale permise sur la plaque
#define COL_DROITE 80       // Position de la colonne pour l'affichage dans l'interface
#define EPSILON 1.0         // Valeur de epsilon
#define ESPACEMENT_COLONES 3 // Espacement entre les colonnes 
#define ESPACEMENT_RANGEES 3  // Espacement entre les rangées 
#define NB_COUL  15         // Nombre de couleurs utilisées pour visualisé la plaque

/* Nombre de lignes d'information à afficher dans la table d'affichage */
#define LINGNE_INFO_TABLE 40 
#define MIN_EPS 0.01        // Valeur minimale d'epsilon
#define MAX_EPS 10.0        // Valeur maximale d'epsilon
#define MIN_COEF 0.20       // Valeur minimale du coefficient 
#define MAX_COEF 0.80       // Valeur maximale du coefficient 

#define FFLUSH() while(getchar() != '\n') {}  // Macro pour vider les scanf()
/*=========================================================*/
/*
 LIRE_NOM_FICH
 Codée par Larouche Edouard

 Cette fonction permet de lire le nom d'un fichier depuis l'entrée
 standard et retourne un pointeur vers le fichier ouvert.

 PARAMÈTRES :
 Aucun (fonction sans paramètre).

 VALEUR DE RETOUR : Un pointeur de type FILE pointant vers le fichier
 ouvert. Si l'ouverture échoue, retourne NULL.

   Explication de la fonction:
  - Cette fonction ouvre un fichier en mode lecture
   dont le nom est fourni par l'utilisateur.
  - Si le fichier n'existe pas ou qu'il est inaccessible, la fonction retourne
	NULL.
  - Elle renvoie un pointeur FILE* vers le fichier ouvert pour être lu dans
	  d'autres fonctions.

 */
FILE* lire_nom_fich();

/*=========================================================*/
/*
  COLORAGE_PLAQUE
Codée par Larouche Edouard

  Cette fonction colore la plaque en fonction des températures contenues
  dans la matrice, avec des couleurs adaptées à l'intervalle de température
  entre mint et maxt.

  PARAMÈTRES :
	- plaque : Matrice représentant la plaque de températures (type : t_matrice).
	- dimy : Nombre de lignes de la plaque (type : int).
	- dimx : Nombre de colonnes de la plaque (type : int).
	- mint : Température minimale pour l'échelle de couleurs (type : double).
	- maxt : Température maximale pour l'échelle de couleurs (type : double).

  VALEUR DE RETOUR : Aucune.

  Explication de la fonction colorage_plaque :

	- Cette fonction applique une couleur à chaque cellule de la plaque en
	  fonction de sa température.
	- Plus la température d'une cellule est élevée, plus la couleur associée
	  sera chaude (par exemple, rouge pour des températures élevées).

 */
void colorage_plaque(const t_matrice plaque, int dimy, int dimx, double mint,
	double maxt);

/*=========================================================*/
/*
  SIMULATION
  Codée par Larouche Edouard et William Chung .

  Cette fonction simule l'évolution de la plaque thermique en utilisant
  les paramètres donnés. Elle modifie les valeurs des températures dans
  la plaque selon les règles du modèle.

  PARAMÈTRES :
	- plaque : Matrice représentant la plaque de températures (type : t_matrice).
	- pos_fixes : Matrice représentant les positions fixes sur la plaque
	 (type : t_matbool).
	- dimy : Nombre de lignes de la plaque (type : int).
	- dimx : Nombre de colonnes de la plaque (type : int).
	- mint : Température minimale (type : double).
	- maxt : Température maximale (type : double).
	- mode : Mode de voisinage utilisé pour la simulation (type : int).
	- epsilon : Valeur de tolérence (type : double).
	- coeff : Coefficient de resistance (type : double).

  VALEUR DE RETOUR : Le nombre d'itérations effectuées jusqu'à l'équilibre.

	Explication de la fonction:

	- Cette fonction exécute une simulation thermique de la plaque.
	- Elle met à jour la température de chaque cellule en fonction de ses
	  voisines, suivant les règles de diffusion de chaleur.
	- Le nombre d'itérations nécessaires pour atteindre un état stable est
	  retourné, permettant de savoir combien de cycles de calcul ont été
	  nécessaires pour que les températures se stabilisent.
 */
int simulation(t_matrice plaque, t_matbool pos_fixes, int dimy, int dimx,
	double* mint, double* maxt, int mode, double epsilon, double coeff);

/*=========================================================*/
/*
  AFFICHAGE_PLAQUE
  Codée par Larouche Edouard.

  Cette fonction affiche la plaque avec ses températures et les valeurs
  de température minimale et maximale.

  PARAMÈTRES :
	- plaque : Matrice représentant la plaque de températures
	 (type : t_matrice).
	- dimy : Nombre de lignes de la plaque (type : int).
	- dimx : Nombre de colonnes de la plaque (type : int).
	- mint : Température minimale (type : double).
	- maxt : Température maximale (type : double).

  VALEUR DE RETOUR : Aucune.

  Explication de la fonction:

	- Cette fonction affiche la plaque entière en visualisant les températures
	  avec des couleurs associées.
	- Elle montre aussi les températures minimale et maximale pour permettre une
	  interprétation facile des couleurs de la plaque.
 */
void affichage_plaque(const t_matrice plaque, int dimy, int dimx, double mint, double maxt);

/*=========================================================*/
/*
 EFFACER_PLAQUE
 Codée par Larouche Edouard .

 Cette fonction efface l'affichage de la plaque thermique.

 PARAMÈTRES :
  - nblig : Nombre de lignes de la plaque (type : int).

  VALEUR DE RETOUR : Aucune.

   Explication de la fonction:

	- Cette fonction efface l'affichage de la plaque à l'écran.

	  Explication de la fonction :

	- Cette fonction efface l'affichage de la plaque à l'écran.
	- Elle remplit les lignes spécifiées avec des espaces, ce qui est utile pour
	  rafraîchir l'affichage avant de montrer une nouvelle simulation
 */
void effacer_plaque(int nblig);

/*=========================================================*/
/*
 DESSINER_ECHELLE
 Codée par William Chung et Larouche Edouard.

 Cette fonction dessine une échelle de couleurs en fonction des valeurs
 mint et maxt.

 PARAMÈTRES :
   - mint : Température minimale de l'échelle (type : double).
   - maxt : Température maximale de l'échelle (type : double).

  VALEUR DE RETOUR : Aucune.

	  Explication de la fonction  :

	- Cette fonction dessine une échelle de couleurs correspondant aux
	  températures minimale et maximale.

 */
void dessiner_echelle(double mint, double maxt);

/*=========================================================*/
/*
 AFF_OPTIONS
 Codée par William Chung et Larouche Edouard.

 Cette fonction affiche les options actuelles du programme, notamment
 le mode, epsilon et le coefficient.

 PARAMÈTRES :
   - mode : Mode de voisinage (type : int).
   - epsi : Valeur d'epsilon actuelle (type : double).
	- coeff_res : Coefficient actuel (type : double).

 VALEUR DE RETOUR : Aucune.

	 Explication de la fonction:

	 - Cette fonction affiche les options de simulation actuelles : mode de
	  calcul, valeur d'epsilon, et coefficient.

 */
void aff_options(int mode, double epsi, double coeff_res);

/*=========================================================*/
/*
  AFF_MINMAX_MOY
  Codée par William Chung et Larouche Edouard.

  Cette fonction affiche les valeurs minimales, maximales et la moyenne
  des températures de la plaque.

  PARAMÈTRES :
	- mint : Température minimale (type : double).
	- maxt : Température maximale (type : double).
	- moyenne : Moyenne des températures de la plaque (type : double).

  VALEUR DE RETOUR : Aucune.

  Explication de la fonction :

	- Cette fonction affiche les valeurs minimale, maximale et moyenne des
	  températures sur la plaque.

 */
void aff_minmax_moy(double mint, double maxt, double moyenne);

/*=========================================================*/
/*
  AFF_NB_ITER
  Codée par William Chung et Larouche Edouard.

  Cette fonction affiche le nombre d'itérations réalisées lors de la
  simulation.

  PARAMÈTRES :
	- nb_iter : Nombre d'itérations effectuées (type : int).

  VALEUR DE RETOUR : Aucune. La fonction modifie l'affichage.

   Explication de la fonction:

	- Cette fonction affiche le nombre d'itérations effectuées jusqu'à
	  maintenant dans la simulation.
 */
void aff_nb_iter(int nb_iter);

/*=========================================================*/
/*
  VALIDER_MODE_VOISINS
  Codée par William Chung et Larouche Edouard.

  Cette fonction permet de valider et de sélectionner le mode de
  voisinage souhaité pour la simulation (4 voisins ou 8 voisins).

  PARAMÈTRES :
	Aucun.

  VALEUR DE RETOUR : Le mode sélectionné (type : int).

  Explication de la fonction:
  
  - Cette fonction demande à l'utilisateur de choisir entre deux modes de
	  voisinage : 4 voisins ou 8 voisins.
  - Elle valide que l'entrée de l'utilisateur est correcte avant de retourner
	  le mode sélectionné.

 */
int valider_mode_voisins();

/*=========================================================*/
/*
 VALIDER_REEL
 Codée par Larouche Edouard.

  Cette fonction permet de valider une entrée réelle (double) en
  s'assurant qu'elle est dans un intervalle spécifié.

  PARAMÈTRES :
	- message : Message à afficher pour guider l'utilisateur
 (type : const char*).
	- b_min : Limite inférieure de l'intervalle (type : double).
	- b_max : Limite supérieure de l'intervalle (type : double).

  VALEUR DE RETOUR : La valeur réelle validée (type : double).

  Explication de la fonction:
  
  - Cette fonction valide un nombre réel (double) entré par l'utilisateur,
	  s'assurant qu'il se situe entre les bornes spécifiées.
  - Elle renvoie la valeur validée pour utilisation dans la simulation.
 */
double valider_reel(const char* message, double b_min, double b_max);

/*=========================================================*/
/*
 AFFICHER_MENU
  Codée par William Chung.

  Cette fonction affiche le menu principal et permet à l'utilisateur
  de choisir une option.

  PARAMÈTRES :
	Aucun.

  VALEUR DE RETOUR : Le choix de l'utilisateur (type : unsigned char).

	  Explication de la fonction:
	  - Cette fonction affiche le menu principal de l'application, présentant les
	  différentes options de simulation.
	  - Elle demande à l'utilisateur de choisir une option, puis retourne le choix
	  sous forme de caractère.
 */
unsigned char afficher_menu();

/*=========================================================*/
/*
  PT_VARIATION
  Codée par William Chung.

  Cette fonction permet de modificer la température à une position donnée sur la plaque


  PARAMÈTRES :
	- plaque : Matrice représentant la plaque de températures
	(type : t_matrice).
	- pos_fixes : Matrice représentant les positions fixes sur la plaque
	(type : t_matbool).
	- dimy : Nombre de lignes de la plaque (type : int).
	- dimx : Nombre de colonnes de la plaque (type : int).
	- mint : Température minimale (type : double).
	- maxt : Température maximale (type : double).
	- fixe : Indicateur des positions fixes (type : int).

  VALEUR DE RETOUR : Aucune.

  Explication de la fonction:
  
  - Cette fonction met à jour la température d'un point de la plaque en
	  fonction de la température de ses voisins.
  - Si le point est fixe, il garde sa température actuelle; sinon, sa
	  température est ajustée en fonction des règles de diffusion.
 */
void pt_variation(t_matrice plaque, t_matbool pos_fixes, int dimy, int dimx, double* mint, double* maxt, int fixe);
