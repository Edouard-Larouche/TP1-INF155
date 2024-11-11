#define _CRT_SECURE_NO_WARNINGS

#include "WinConsole.h" // Librairie pour la gestion de l'écran et des couleurs, A AJUSTER
#include "plaque_lib.h"
#include "affichages.h"

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
