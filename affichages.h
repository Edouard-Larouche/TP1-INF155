#define _CRT_SECURE_NO_WARNINGS
#include "affichages.h"
#include "WinConsole.h"
#include <ctype.h> 
#define NB_COUL  15 

const int COULEURS[NB_COUL] = { RED, LIGHTRED, MAGENTA, LIGHTMAGENTA, BLUE,
LIGHTBLUE, CYAN, LIGHTCYAN, GREEN, LIGHTGREEN, DARKGRAY, BROWN,
YELLOW, LIGHTGRAY, WHITE };

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

unsigned char afficher_menu(double* epsilon, double* coeff, int* mode, int* init,
    t_matrice plaque, t_matbool pos_fixes, int* dimy, int* dimx,
    double* mint, double* maxt) {

    int iteration_n;
    unsigned char choix;
    int ligne_ecriture = 0;  // Position du menu en bas de l'écran

    textmode(55); // Configure la console en mode 55 lignes X 120 caractères

    gotoxy(COL_DROITE, ++ligne_ecriture);
    printf("Menu:\n");
    gotoxy(COL_DROITE, ++ligne_ecriture);
    printf("1. (L)ire donnees initiales d'un fichier\n");
    gotoxy(COL_DROITE, ++ligne_ecriture);
    printf("2. (M)ethode: 4 voisins ou 8 voisins\n");
    gotoxy(COL_DROITE, ++ligne_ecriture);
    printf("3. Changer (E)psilon");
    gotoxy(COL_DROITE, ++ligne_ecriture);
    printf("4. Changer (C)oefficient");
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
            valider_mode_voisins();
      
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


void affichage_plaque(const t_matrice plaque, int dimy, int dimx, double mint, double maxt) {
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
}

void effacer_plaque(int nblig) {
    int i;
    int j;
    gotoxy(0, 0);  // Revenir au coin supérieur gauche

    for (i = 0; i < nblig; i++) {
        printf("\r");           // Retourner au début de la ligne
        for (j = 0; j < COL_DROITE; j++) {
            printf(" ");        // Afficher des espaces pour effacer la ligne
        }
        printf("\n");           // Passer à la ligne suivante
    }

    gotoxy(0, 0);  // Revenir au coin supérieur gauche après l'effacement
}

void dessiner_echelle(double mint, double maxt) {

    int ligne_echelle = MAXLIG - NB_COUL - 2;  // Position de l’échelle en bas
    int col_echelle = COL_DROITE + 10;         // Position de l’échelle à droite
    double temp_min, temp_max;
    double ecart = (maxt - mint) / NB_COUL;
    int i;
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

void aff_options(int mode, double epsi, double coeff_res) {
    int ligne_affichage = 15;  // Ligne située sous le menu principal
    int col_affichage = COL_DROITE; // Position à droite de l’écran

    gotoxy(col_affichage, ligne_affichage);
    printf("Options actuelles:");

    gotoxy(col_affichage, ligne_affichage + 1);
    printf("Mode : %d voisins", mode);

    gotoxy(col_affichage, ligne_affichage + 2);
    printf("Epsilon : %.2lf", epsi);

    gotoxy(col_affichage, ligne_affichage + 3);
    printf("Coefficient : %.2lf", coeff_res);

}

void aff_minmax_moy(double mint, double maxt, double moyenne) {
    gotoxy(COL_DROITE, LINGNE_INFO_TABLE);
    printf("Max : %.2lf | Min : %.2lf | Moyenne : %.2lf", maxt, mint, moyenne);
}

void aff_nb_iter(int nb_iter) {

    gotoxy(COL_DROITE, LINGNE_INFO_TABLE + 1);
    printf("Simulation terminee apres %d iterations.\n", nb_iter);
}

int valider_mode_voisins() {
    int mode;
    int ligne_affichage = 15;  // Ligne des options « mode, epsilon, coeff_res »
    int col_affichage = COL_DROITE;

    do {
        gotoxy(col_affichage, ligne_affichage);
        printf("Mode de calcul (4 ou 8 voisins) : ");
        scanf("%d", &mode);
        gotoxy(col_affichage, ligne_affichage);
        printf("                                            "); // Effacer la ligne d'erreur

        if (mode != 4 && mode != 8) {
            gotoxy(col_affichage, ligne_affichage + 1);
            printf("Erreur: entrez 4 ou 8 uniquement.");
            FFLUSH(); 
        }
        gotoxy(col_affichage, ligne_affichage);
        printf("                                ");
    } while (mode != 4 && mode != 8);

    // Efface le message d'erreur après saisie correcte
    gotoxy(col_affichage, ligne_affichage + 1);
    printf("                                    ");  // Efface la ligne d'erreur

    return mode;
}

double valider_reel(const char* message, double b_min, double b_max) {

    double valeur;
    int ligne_affichage = 16;  // Ligne des options « mode, epsilon, coeff_res »
    int col_affichage = COL_DROITE;

    do {
        gotoxy(col_affichage, ligne_affichage);
        printf("%s (entre %.2lf et %.2lf) : ", message, b_min, b_max);
        scanf("%lf", &valeur);

        if (valeur < b_min || valeur > b_max) {
            gotoxy(col_affichage, ligne_affichage + 1);
            printf("Erreur: valeur hors des limites.");
        }
    } while (valeur < b_min || valeur > b_max);

    // Efface le message d'erreur après saisie correcte
    gotoxy(col_affichage, ligne_affichage + 1);
    printf("                                    ");  // Efface la ligne d'erreur

    return valeur;
}
