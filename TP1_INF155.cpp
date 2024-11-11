#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <assert.h>
#include "WinConsole.h" // Librairie pour la gestion de l'écran et des couleurs
#include <ctype.h> 
#include "plaque_lib.h"

// Macro
#define FFLUSH() while(getchar() != '\n') {}  // Macro pour vider les scanf()

#define MAXLIG  40          // Dimension maximale de la plaque
#define MAXCOL  75
#define MAXTEMP 500			  // La température absolue maximale permise
#define COL_DROITE 80
#define EPSILON 1.0
#define ESPACEMENT_COLONES 3 // Pour permettre une lecture plus facile
#define ESPACEMENT_RANGEES 3   // Pour permettre une lecture plus facile
#define NB_COUL  15 
#define LINGNE_INFO_TABLE 40

const int COULEURS[NB_COUL] = { RED, LIGHTRED, MAGENTA, LIGHTMAGENTA, BLUE,
LIGHTBLUE, CYAN, LIGHTCYAN, GREEN, LIGHTGREEN, DARKGRAY, BROWN,
YELLOW, LIGHTGRAY, WHITE };

typedef double t_matrice[MAXLIG][MAXCOL];   // Type-tableau pour les 2 plaques
typedef int t_matbool[MAXLIG][MAXCOL];      // Type-tableau pour les positions fixes

FILE* lire_nom_fich();

int simulation(t_matrice plaque, t_matbool pos_fixes, int dimy, int dimx,
    double* mint, double* maxt, int mode, double epsilon, double coeff);
void colorage_plaque(const t_matrice plaque, int dimy, int dimx, double mint,
    double maxt);
unsigned char afficher_menu(double* epsilon, double* coeff, int* mode, int* init,
    t_matrice plaque, t_matbool pos_fixes, int* dimy, int* dimx, double* mint,
    double* maxt);

int main() {
    int dimy, dimx, mode = 4;
    double coeff = 0.5, mint, maxt, epsilon = EPSILON;
    int init = 0; // Indique si les données ont été chargées
    t_matrice plaque;
    t_matbool pos_fixes;

    textmode(55); // Configure la console en mode 55 lignes X 120 caractères

    while (afficher_menu(&epsilon, &coeff, &mode, &init, plaque, pos_fixes, &dimy, &dimx, &mint, &maxt)) {
        // La boucle continue tant que afficher_menu retourne 1
    }

    return 0;
}

FILE* lire_nom_fich() {
    FILE* ptr_de_fichier = NULL;
    char nom_saisi[40];
    int ligne_ecriture = 12;

    while (!ptr_de_fichier) {
        gotoxy(COL_DROITE, ++ligne_ecriture);
        printf("Entrez le nom du fichier a selectionner : ");
        gotoxy(COL_DROITE, ++ligne_ecriture);
        scanf("%s", nom_saisi);
        FFLUSH();

        ptr_de_fichier = fopen(nom_saisi, "r");
        if (!ptr_de_fichier) {
            gotoxy(COL_DROITE, ++ligne_ecriture);
            printf("OUPS... Ca ne marche pas !\n");
        }
    }
    gotoxy(COL_DROITE, ++ligne_ecriture);
    printf("Entree valide.\n");
    return ptr_de_fichier;
}

int simulation(t_matrice plaque, t_matbool pos_fixes, int dimy, int dimx,
    double* mint, double* maxt, int mode, double epsilon, double coeff) {

    int iteration = 0;
    double moyenne;
    t_matrice nouv_plaque;
    int equilibre;
    int i, j;

    do {

        equilibre = calculer_nouv_plaque(plaque, nouv_plaque, pos_fixes,
            dimy, dimx, mode, EPSILON, coeff);
        moyenne = copier_nouv_plaque(plaque, nouv_plaque,
            dimy, dimx, mint, maxt);
        colorage_plaque(plaque, dimy, dimx, *mint, *maxt);
        gotoxy(COL_DROITE, LINGNE_INFO_TABLE);
        printf("Max : %.2lf | Min : %.2lf | Moyenne : %.2lf", *maxt, *mint, moyenne);
        iteration++;
    } while (equilibre != 1);

    return iteration;
}

void colorage_plaque(const t_matrice plaque, int dimy, int dimx, double mint, double maxt) {
    int i, j, n_couleur;
    double ecart = (maxt - mint) / NB_COUL;  // Intervalle de température par couleur
    gotoxy(0, 0);
    // Affichage de la plaque colorée
    for (i = 0; i < dimy; i++) {
        for (j = 0; j < dimx; j++) {
            n_couleur = (int)((plaque[i][j] - mint) / ecart);
            if (n_couleur >= NB_COUL) {
                n_couleur = NB_COUL - 1;  // Pour éviter le dépassement
            }
            textbackground(COULEURS[n_couleur]);
            printf(" "); // Imprime la couleur
        }
        printf("\n");
    }

    // Réinitialise la couleur de fond
    textbackground(0);

    // Affichage de l'échelle des couleurs en bas à droite de l’écran
    int ligne_echelle = MAXLIG - NB_COUL - 2;  // Position de l’échelle en bas
    int col_echelle = COL_DROITE + 10;         // Position de l’échelle à droite
    double temp_min, temp_max;
    gotoxy(col_echelle, ligne_echelle);

    for (i = 0; i < NB_COUL; i++) {
        temp_min = mint + i * ecart;  // Température minimale de la couleur actuelle
        temp_max = temp_min + ecart;  // Température maximale de la couleur actuelle

        // Affiche la couleur
        textbackground(COULEURS[i]);
        printf("  ");  // Bloc de couleur
        textbackground(0);  // Réinitialise la couleur de fond pour la légende

        // Affiche la légende avec la plage de températures
        printf(" %.2lf - %.2lf", temp_min, temp_max);
        gotoxy(col_echelle, ligne_echelle + i + 1);  // Passe à la ligne suivante
    }


    textbackground(0); // Réinitialiser la couleur de fond après l’échelle

}

unsigned char afficher_menu(double* epsilon, double* coeff, int* mode, int* init,
    t_matrice plaque, t_matbool pos_fixes, int* dimy, int* dimx,
    double* mint, double* maxt) {

    int iteration_n;
    unsigned char choix;
    int ligne_ecriture = 0;  // Position du menu en bas de l'écran


    gotoxy(COL_DROITE, ++ligne_ecriture);
    printf("Menu:\n");
    gotoxy(COL_DROITE, ++ligne_ecriture);
    printf("1. (L)ire donnees initiales d'un fichier\n");
    gotoxy(COL_DROITE, ++ligne_ecriture);
    printf("2. (M)ethode: 4 voisins ou 8 voisins\n");
    gotoxy(COL_DROITE, ++ligne_ecriture);
    printf("3. Changer (E)psilon (actuel: %.2lf)\n", *epsilon);
    gotoxy(COL_DROITE, ++ligne_ecriture);
    printf("4. Changer (C)oefficient (actuel: %.2lf)\n", *coeff);
    gotoxy(COL_DROITE, ++ligne_ecriture);
    printf("5. (V)ariation de temperature\n");
    gotoxy(COL_DROITE, ++ligne_ecriture);
    printf("6. (F)ixer temperature\n");
    gotoxy(COL_DROITE, ++ligne_ecriture);
    printf("7. (S)imulation\n");
    gotoxy(COL_DROITE, ++ligne_ecriture);
    printf("8. (Q)uitter le programme\n");

    gotoxy(COL_DROITE, ++ligne_ecriture);
    printf("Choisissez une option: ");
    choix = toupper(_getch());

    switch (choix) {
    case 'L': case '1': {
        FILE* fichier = lire_nom_fich();
        if (fichier) {
            lire_fichier(fichier, plaque, pos_fixes, dimy, dimx, mint, maxt);
            *init = 1;
        }
        colorage_plaque(plaque, *dimy, *dimx, *mint, *maxt);

        break;
    }
    case 'M': case '2':
        do {
            printf("Choisissez la methode (4 ou 8): ");
            scanf("%d", mode);
        } while (*mode != 4 && *mode != 8);
        break;
    case 'E': case '3':
        do {
            printf("Entrez une valeur pour epsilon (0.01 - 10.0): ");
            scanf("%lf", epsilon);
        } while (*epsilon < 0.01 || *epsilon > 10.0);
        break;
    case 'C': case '4':
        do {
            printf("Entrez une valeur pour le coefficient (0.20 - 0.80): ");
            scanf("%lf", coeff);
        } while (*coeff < 0.20 || *coeff > 0.80);
        break;
    case 'V': case '5':
        if (*init) {
            printf("Fonction de variation de temperature à implementer.\n");
        }
        else {
            printf("Veuillez d'abord charger les donnees.\n");
        }
        break;
    case 'F': case '6':
        if (*init) {
            printf("Fonction de fixation de la temperature à implementer.\n");
        }
        else {
            printf("Veuillez d'abord charger les donnees.\n");
        }
        break;
    case 'S': case '7':
        if (*init) {
            iteration_n = simulation(plaque, pos_fixes, *dimy,
                *dimx, mint, maxt, *mode, *epsilon, *coeff);
            gotoxy(COL_DROITE, LINGNE_INFO_TABLE + 1);
            printf("Simulation terminee apres %d iterations.\n", iteration_n);
        }
        else {
            printf("Veuillez d'abord charger les données.\n");
        }
        break;
    case 'Q': case '8':
        printf("Quitter le programme.\n");
        return 0;
    default:
        printf("Choix invalide. Essayez à nouveau.\n");
        break;
    }

    return 1;
}
