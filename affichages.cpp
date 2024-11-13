
/*
Par: William Chung et Edouard Larouche
Date : Octobre 2024 - Novembre 2024

Ce module implémente des sous-fonctions pour l'affichage sur l'écran.
Voici les sous-fonctions:
- lire_nom_fich: Lit et retourne un fichier valide.
- colorage_plaque: Affiche la plaque colorée selon les températures.
- simulation: Effectue la simulation thermique et retourne le nombre d'itérations.
- affichage_plaque: Affiche la plaque de températures sans simulation.
- effacer_plaque: Efface la plaque d'affichage.
- dessiner_echelle: Affiche l'échelle de températures à côté de la plaque.
- aff_options: Affiche les options de simulation (mode, epsilon, coefficient).
- aff_minmax_moy: Affiche les températures min, max et moyenne.
- aff_nb_iter: Affiche le nombre d'itérations de la simulation.
- valider_mode_voisins: Valide le mode de calcul (4 ou 8 voisins).
- valider_reel: Valide un nombre réel entre un min et un max.
- afficher_menu: Affiche le menu principal pour choisir les options.
- pt_variation: Permet de modifier la température à une position donnée sur la plaque.

*/

#include "affichages.h"
#include "WinConsole.h"
#include <ctype.h> 

/*=========================================================*/

const int COULEURS[NB_COUL] = { RED, LIGHTRED, MAGENTA, LIGHTMAGENTA, BLUE,
LIGHTBLUE, CYAN, LIGHTCYAN, GREEN, LIGHTGREEN, DARKGRAY, BROWN,
YELLOW, LIGHTGRAY, WHITE };

/*=========================================================*/

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

        ptr_de_fichier = fopen(nom_saisi, "r"); //Ouverture du fichier en mode lecture
        if (!ptr_de_fichier) {
            gotoxy(COL_DROITE, ++ligne_ecriture);
            printf("OUPS... Ca ne marche pas !\n"); 
        }
    }
    
    return ptr_de_fichier;
}

/*=========================================================*/

void colorage_plaque(const t_matrice plaque, int dimy, int dimx, double mint, double maxt) {
    int i, j, n_couleur;
    double ecart = (maxt - mint) / NB_COUL;  // Intervalle de température par couleur
    gotoxy(0, 0);
    // Affichage de la plaque colorée
    for (i = 0; i < dimy; i++) {
        for (j = 0; j < dimx; j++) {

            /* Calcul qui permet d'attribuer la couleur designé*/
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

/*=========================================================*/

int simulation(t_matrice plaque, t_matbool pos_fixes, int dimy, int dimx,
    double* mint, double* maxt, int mode, double epsilon, double coeff) {

    // Variable pour compter le nombre d'itérations de la simulation
    int iteration = 0;

    // Variable pour stocker la moyenne des températures
    double moyenne;

    /* Matrice qui contient les nouvelles 
    températures calculées à chaque itération*/
    t_matrice nouv_plaque;

    // Variable pour vérifier si l'équilibre a été atteint (1 si stable, 0 si non)
    int equilibre;
    int i, j;

    do {

        // Calculer la nouvelle plaque de températures à partir de la plaque actuelle
        // La fonction renvoie si l'équilibre est atteint ou non.
        equilibre = calculer_nouv_plaque(plaque, nouv_plaque, pos_fixes,
            dimy, dimx, mode, EPSILON, coeff);

        /* Copier les nouvelles températures calculées
        dans la plaque et obtenir la moyenne */
    // Cette fonction met à jour la plaque et retourne la température moyenne
        moyenne = copier_nouv_plaque(plaque, nouv_plaque,
            dimy, dimx, mint, maxt);

        // Appliquer le coloriage de la plaque selon les températures actuelles
     /* Cela va mettre à jour l'affichage en fonction
     des températures minimales et maximales */
        colorage_plaque(plaque, dimy, dimx, *mint, *maxt);
        aff_minmax_moy(*mint, *maxt, moyenne); 

        iteration++;
    } while (equilibre != 1);

    return iteration;
}

/*=========================================================*/

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

/*=========================================================*/

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

/*=========================================================*/

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

/*=========================================================*/

void aff_options(int mode, double epsi, double coeff_res) {
    int ligne_affichage = 15;  // Ligne située sous le menu principal
    int col_affichage = COL_DROITE; // Position à droite de l’écran

    gotoxy(col_affichage, ligne_affichage + 1);
    printf("mode : %d voisins", mode);

    gotoxy(col_affichage, ligne_affichage + 2);
    printf("epsilon : %.2lf", epsi);

    gotoxy(col_affichage, ligne_affichage + 3);
    printf("coefficient : %.2lf", coeff_res);

}

/*=========================================================*/

void aff_minmax_moy(double mint, double maxt, double moyenne) {
    // Imprime la valeur du max, min et moyenne
    gotoxy(COL_DROITE, LINGNE_INFO_TABLE);
    printf("Max : %.2lf | Min : %.2lf | Moyenne : %.2lf", maxt, mint, moyenne);
}

/*=========================================================*/

void aff_nb_iter(int nb_iter) {

    // Imprime le nombre d'itération
    gotoxy(COL_DROITE, LINGNE_INFO_TABLE + 1);
    printf("Simulation terminee apres %d iterations.\n", nb_iter);
}

/*=========================================================*/

int valider_mode_voisins() {
    int mode;
    int ligne_affichage = 15;
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

/*=========================================================*/

double valider_reel(const char* message, double b_min, double b_max) {

    double valeur;
    int ligne_affichage = 16;  
    int col_affichage = COL_DROITE;

    do {
        gotoxy(col_affichage, ligne_affichage);
        printf("%s (entre %.2lf et %.2lf) : ", message, b_min, b_max);
        scanf("%lf", &valeur);
        FFLUSH(); 
        gotoxy(col_affichage, ligne_affichage); // Imprime efface
        printf("                                                                   "); 

        if (valeur < b_min || valeur > b_max) {
            gotoxy(col_affichage, ligne_affichage + 1);
            printf("Erreur: valeur hors des limites.");
        }
    } while (valeur < b_min || valeur > b_max);

    // Efface le message d'erreur après saisie correcte
    gotoxy(col_affichage, ligne_affichage + 1);
    printf("                                             ");  // Efface la ligne d'erreur

   

    return valeur;
}

/*=========================================================*/

unsigned char afficher_menu() {

    const char* message = "Nouvelle valeur de epsilon?: ";
    int ligne_ecriture = 0;  // Position du menu en bas de l'écran
    unsigned char choix;

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

    return choix;
}

/*=========================================================*/

void pt_variation(t_matrice plaque, t_matbool pos_fixes, int dimy, int dimx, double* mint, double* maxt, int fixe) {

    int posy = dimy / 2;
    int posx = dimx / 2;
    int touche;
    double nouvelle_temp = 0;
    double ecart = (*maxt - *mint) / NB_COUL;  // Intervalle de température par couleur
    int couleur_de_fond;
    do {
        // Déterminer la couleur de fond en fonction de la température actuelle
        couleur_de_fond = (int)((plaque[posy][posx] - *mint) / ecart);

        if (couleur_de_fond >= NB_COUL) {
            couleur_de_fond = NB_COUL - 1;
        }

        textbackground(COULEURS[couleur_de_fond]);

        // Afficher le curseur « + » à la position actuelle
        gotoxy(posx, posy);
        printf("+");

        // Attendre une touche du clavier
        touche = _getch();

        // Restaurer l’affichage de la température à l’ancienne position
        gotoxy(posx, posy);
        printf(" ");  // Efface le curseur avant de déplacer

        // Déplacement du curseur avec les touches fléchées
        if (touche == 224) {  // Préfixe pour les touches fléchées
            touche = _getch();
            switch (touche) {
            case 72:  // Flèche haut
                if (posy > 0) posy--;
                break;

            case 80:  // Flèche bas
                if (posy < dimy - 1) posy++;
                break;

            case 75:  // Flèche gauche
                if (posx > 0) posx--;
                break;

            case 77:  // Flèche droite
                if (posx < dimx - 1) posx++;
                break;
            }
        }

    } while (touche != 13);  // Quitte la boucle avec entrée (ASCII 13)

    textbackground(BLACK); // Mise a couleur noir de fond noir pour insertion des textes

    // Obtenir la nouvelle température pour la position actuelle
    nouvelle_temp = valider_reel("Nouvelle temperature : ", *mint, *maxt);
    plaque[posy][posx] = nouvelle_temp;

    // Ajuster mint et maxt si nécessaire
    if (nouvelle_temp < *mint) *mint = nouvelle_temp;
    if (nouvelle_temp > *maxt) *maxt = nouvelle_temp;

    // Marquer la position comme fixe ou variable selon le paramètre fixe
    pos_fixes[posy][posx] = fixe;

    // Redessiner la plaque pour afficher les nouvelles températures et échelle
    colorage_plaque(plaque, dimy, dimx, *mint, *maxt);
    dessiner_echelle(*mint, *maxt);
}
