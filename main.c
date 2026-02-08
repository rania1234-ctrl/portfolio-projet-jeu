#include <stdio.h>//Pour afficher du texte dans la console (printf)/Pour lire les entrées utilisateur (scanf)
#include <stdlib.h>//générer des coups aléatoires pour le bot/ initialiser le générateur/quitter proprement
#include <time.h> //Pour initialiser la fonction rand() avec une graine aléatoire/ Grâce à srand(time(NULL)),Le bot joue de manière différente à chaque partie
#define SDL_MAIN_HANDLED//#define permet d’utiliser mon propre main() sans conflit
#include <SDL_image.h>//Pour charger des images (BMP, PNG, JPG)
#include <SDL.h>//Utilisé dans :Toute la partie graphique du jeu/ SDL_Window, SDL_Renderer, SDL_Event
#include <SDL_mixer.h>//Pour jouer LE sons du bouton
#include <math.h>//Pour tracer les cercles (le "O") avec cos() et sin()/Dessin du symbole O dans la grille SDL
//____________________________________________________________________________________________________________________________________________________________

//  Fonctions de menu et de choix:

           char menu();
           //Je voulais que le joueur choisisse s’il joue avec un X ou un O pour personnaliser sa partie.

           void choisir_options(int* Choix);
           //J’ai ajouté cette fonction pour demander au joueur s’il veut jouer contre un bot ou contre un ami.

// Fonctions de logique du jeu en mode console:

            void draw_board(char grille_console[]);
           //Je voulais afficher la grille de jeu dans la console pour que le joueur voie l’état actuel.

           int put_X_O(char grille_console[], char symbole);
           //J’ai créé cette fonction pour demander au joueur où il veut jouer et placer son symbole dans la grille.

           int verifier_victoire(char grille_console[]);
           //J’ai eu besoin de cette fonction pour vérifier à chaque tour si un joueur a gagné en console.

           int verifier_victoire_temp(int grille[], int joueur);
           // j’ai pensé à rendre le bot plus intelligent/ il vérifie s’il peut gagner ou bloquer.

           int check_draw(char grille_console[]);
           //Je voulais savoir si la partie était finie sans gagnant, donc j’ai prévu cette vérification de match nul.

           void start_game(char grille_console[], char symbole_initial);
           //J’ai rassemblé toute la logique du jeu à deux joueurs dans cette fonction pour que tout soit propre

           void start_game_vs_bot(char grille_console[], char symbole_initial, char symbole_bot);
           //J’ai mis cette fonction pour gérer automatiquement les tours du bot quand on joue contre lui.
// Fonctions BOT CONSOLE
           int choisir_coup_aleatoire_console(char grille_console[]);

// Fonctions bot SDL
           int choisir_coup_bot(int grille[]); // J’ai commencé avec un bot simple, qui joue aléatoirement dans une case vide

// Fonctions SDL (mode graphique)
            void afficherImageAvecStart(SDL_Renderer* renderer);//une image fixe avec un bouton invisible “C’est parti !”

            int afficherChoixBotOuAmi(SDL_Renderer* renderer);
           //Après avoir cliqué sur “C’est parti !”, je voulais que le joueur choisisse s’il joue contre un bot ou un ami.

            void lancerJeuInteractif(SDL_Renderer* renderer, int Choix);
            // Cette fonction est le cœur du jeu en SDL, elle gère les clics, le tour par tour, et l’affichage de la grille.

            int verifier_victoire_sdl(int grille[]);
            // J’ai créé cette fonction pour détecter si un joueur a gagné en mode graphique

            int check_draw_sdl(int grille[]);
             // Vérifie s’il y a un match nul en mode SDL

            void afficher_image_victoire(SDL_Renderer* renderer, int joueur);
            //J’ai voulu afficher une image différente selon que X ou O gagne, pour rendre la fin plus visuelle.

            int afficher_choix_rejouer_fermer(SDL_Renderer* renderer);
            //Après la victoire, j’ai pensé à afficher une image avec deux boutons invisibles pour rejouer ou quitter.

//___________________________________________________________________________________
  void choisir_options(int* Choix) {
    printf("=====================================\n");
    printf("     Bienvenue au Tic-Tac-Toe !      \n");
    printf("=====================================\n\n");

    printf("\nAvec qui veux-tu jouer ?\n");
    printf("1 - Contre un bot\n");
    printf("2 - Avec un ami \n");
    printf("Tape ton choix (1 ou 2) : ");
    scanf("%d", Choix);// Récupère la réponse saisie par l'utilisateur et la stocke à l’adresse donnée
}
//__________________________________________________________________________________
//affiche le plateau de jeu Tic-Tac-Toe avec les cases de 1 à 9
    void draw_board(char grille_console[]) {
    printf("\n"); // Saut de ligne pour espacer l'affichage
    printf(" %c | %c | %c\n", grille_console[0], grille_console[1], grille_console[2]);// Affiche la première ligne de la grille (cases 0, 1, 2)
    printf("---+---+---\n");// Affiche les séparateurs horizontaux
    printf(" %c | %c | %c\n", grille_console[3], grille_console[4], grille_console[5]);// Affiche la deuxième ligne de la grille (cases 3, 4, 5)
    printf("---+---+---\n");// Affiche les séparateurs horizontaux
    printf(" %c | %c | %c\n", grille_console[6], grille_console[7], grille_console[8]);// Affiche la troisième ligne de la grille (cases 6, 7, 8)
    printf("\n");// Saut de ligne pour espacer avec les prochains affichages
}
//__________________________________________________________________
// Fonction pour afficher le menu et retourner le symbole choisi
char menu()
{
    char choix;// Déclaration de la variable pour stocker le choix du joueur
    printf("Quel symbole veux-tu utiliser ?\n");// Affiche un message demandant au joueur de choisir son symbole
    printf("Tape 'X' ou 'O' puis appuie sur Entrée : ");// Donne les instructions claires pour entrer le symbole
    scanf(" %c", &choix);// Lit le caractère tapé par le joueur (avec un espace avant %c pour ignorer les caractères précédents comme '\n')

    // Vérifie si le symbole choisi est valide ('X' ou 'O')
    if (choix == 'X' || choix == 'O') {
        printf("\nTu as choisi de jouer avec %c \n", choix);// Affiche le symbole choisi s'il est valide

    } else {// Si le joueur entre un caractère invalide, on affiche un message et on attribue 'X' par défaut
        printf("\nChoix invalide. Par défaut tu joueras avec X.\n");
        choix = 'X';
    }

    return choix;// Retourne le symbole choisi (ou 'X' par défaut)
}
//____________________________________________________________________
//--------------------------------------------------
//Fonction : put_X_O()
      // Demande au joueur une position (1 à 9),
       //vérifie si la case est libre,
       //et place le symbole (X ou O) dans la case.

//--------------------------------------------------
int put_X_O(char grille_console[], char symbole) {
    int position;// Variable pour stocker la position choisie par le joueur

    printf("Joueur %c, choisis une case (1 à 9) : ", symbole);// Affiche un message demandant au joueur de choisir une case
    scanf("%d", &position);// Lecture de la position saisie par le joueur

    // Vérifie si la position choisie est valide (entre 1 et 9)
    if (position < 1 || position > 9) {
        printf("Position invalide. Réessaye.\n");// Si invalide, affiche un message d’erreur
        return put_X_O(grille_console, symbole); // Appel récursif pour redemander une position
    }
     // Vérifie si la case est libre (pas déjà occupée par X ou O)
    if (grille_console[position - 1] != 'X' && grille_console[position - 1] != 'O') {
        grille_console[position - 1] = symbole;// Si libre, place le symbole dans la case
        return position; //on retourne la position
    } else {
        printf("Case déjà occupée. Réessaye.\n");// Si la case est déjà prise, afficher un message d’erreur
        return put_X_O(grille_console, symbole);  // Appel récursif pour redemander une case
    }
}
//_______________________________________________________________
//Fonction : verifier_victoire()
int verifier_victoire(char grille_console[]) {
    // Tableau contenant toutes les combinaisons gagnantes possibles
    int combinaisons[8][3] = {
        {0, 1, 2},// Ligne du haut
        {3, 4, 5}, // Ligne du milieu
        {6, 7, 8},// Ligne du bas
        {0, 3, 6},// Colonne de gauche
         {1, 4, 7},// Colonne du milieu
        {2, 5, 8},// Colonne de droite
        {0, 4, 8},// Diagonale de haut-gauche à bas-droite
         {2, 4, 6}// Diagonale de haut-droite à bas-gauche
    };
    // Boucle pour parcourir toutes les combinaisons
    int i;
    for (i = 0; i < 8; i++) {
         // Récupération des 3 cases de la combinaison actuelle
        int a = combinaisons[i][0];
        int b = combinaisons[i][1];
        int c = combinaisons[i][2];
        // Vérifie si les 3 cases contiennent le même symbole (X ou O)
        if (grille_console[a] == grille_console[b] && grille_console[b] == grille_console[c] &&
            (grille_console[a] == 'X' || grille_console[a] == 'O')) {
            // Si oui, retourne 1 pour indiquer qu’il y a un gagnant
            return 1;
        }
    }
    // Si aucune combinaison gagnante n’est détectée, retourne 0
    return 0;
}
//____________________________________________________________________________________________________________
//Fonction : check_draw()
int check_draw(char grille_console[]) {
    // Déclarer une variable de boucle pour parcourir la grille
    int i;
    // Parcourt toutes les 9 cases de la grille
    for (i = 0; i < 9; i++) {
        // Si une case ne contient pas encore X ni O, alors il reste une case vide
        if (grille_console[i] != 'X' && grille_console[i] != 'O') {
            return 0; // Il reste au moins une case vide, donc ce n’est pas encore un match nul
        }
    }
     // Si on a terminé la boucle sans trouver de case vide, alors la grille est pleine
    return 1; // Et comme personne n’a gagné (cette fonction est appelée après vérifier victoire), c’est une égalité
}
//_______________________________________________________________________________________________________________
//CES DEUX FONCTIONS CONCERNANT LA PARTIE SDL
//===============================================================
int verifier_victoire_temp(int grille[], int joueur) {
    // Tableau contenant toutes les 8 combinaisons gagnantes possibles
    int combinaisons[8][3] = {
        {0,1,2}, {3,4,5}, {6,7,8},// Lignes horizontales
        {0,3,6}, {1,4,7}, {2,5,8},// Colonnes verticales
        {0,4,8}, {2,4,6}// Diagonales
    };
    // Variable pour la boucle
    int i;
    // Parcours des 8 combinaisons gagnantes
    for ( i = 0; i < 8; i++) {
        // Récupère les trois indices de la combinaison courante
        int a = combinaisons[i][0];
        int b = combinaisons[i][1];
        int c = combinaisons[i][2];
        // Cas 1 : les deux premières cases sont remplies par le joueur, la 3e est vide → on renvoie la 3e pour gagner/bloquer
        if (grille[a] == joueur && grille[b] == joueur && grille[c] == 0) return c;
        // Cas 2 : la 1re et la 3e cases sont remplies, la 2e est vide → on renvoie la 2e
        if (grille[a] == joueur && grille[c] == joueur && grille[b] == 0) return b;
        // Cas 3 : les 2e et 3e cases sont remplies, la 1re est vide → on renvoie la 1re
        if (grille[b] == joueur && grille[c] == joueur && grille[a] == 0) return a;
    }
    return -1;// Si aucune combinaison ne permet de gagner ou bloquer immédiatement, on retourne -1
}
//______________________________________________________________________________________________________________
int choisir_coup_bot(int grille[]) {
    // 1 = X, 2 = O (le bot est 2)
    int coup;
    //KEN MANAJAMCH YERBAH
    // Étape 1 : Le bot peut-il gagner immédiatement ?
    coup = verifier_victoire_temp(grille, 2);// Vérifie s’il existe une combinaison gagnante pour le bot
    if (coup != -1) return coup;// Si oui, joue ce coup gagnant
     //S IL N AS PAS PU BLOQUE
    // Étape 2 : Peut-il bloquer le joueur humain ?
    coup = verifier_victoire_temp(grille, 1);// Vérifie si le joueur humain est sur le point de gagner
    if (coup != -1) return coup;// Si oui, bloque en jouant dans la case critique
     //IL VA METTRE ALEATOIRE
    // Étape 3 : Sinon, choisir un coup aléatoire parmi les cases encore libres
    int coups_possibles[9];// Tableau pour stocker les positions libres
    int nb_coups = 0;// Compteur de coups possibles
    int i;
    for (i = 0; i < 9; i++) {
        if (grille[i] == 0) {// Si la case est vide
            coups_possibles[nb_coups++] = i; // Ajoute sa position dans le tableau
        }
    }//2 CAS POSSIBLES
    if (nb_coups == 0) return -1; // Si aucune case libre, retourne -1 (partie finie)
   //SINON RETOURNE UNE CASE ALEATOIRE
    int random_index = rand() % nb_coups;// Sélectionne une case au hasard parmi les coups disponibles (JDIDA)
    return coups_possibles[random_index];// Retourne la position sélectionnée
}
//==================================================================================================
//__________________________________________________________________________________________________

//Fonction : start_game()
//Rôle : Lance une boucle de jeu jusqu’à la victoire
       //ou le match nul. Alterne entre X et O

void start_game(char grille_console[], char symbole_initial) {
    int fin_du_jeu = 0;// Variable pour contrôler si la partie est terminée (0 = en cours, 1 ou 2 = fin)
    char symbole = symbole_initial;// Commencer avec le symbole choisi par le joueur

    while (fin_du_jeu == 0) {// Tant que la partie n’est pas terminée
        draw_board(grille_console);// Affiche l’état actuel de la grille
        int position = put_X_O(grille_console, symbole);// Demande au joueur où il veut jouer et récupère la position

         grille_console[position - 1] = symbole;// Place le symbole (X ou O) dans la case choisie

        if (verifier_victoire(grille_console)) {// Vérifie si le joueur courant a gagné
            draw_board(grille_console);// Réaffiche la grille finale
            printf("Le joueur %c a gagné !\n", symbole);// Message de victoire
            fin_du_jeu = 1;// Marquer la fin du jeu
        } else if (check_draw(grille_console)) {// Vérifie s’il y a match nul (grille pleine sans gagnant)
            draw_board(grille_console);// Réaffiche la grille finale
            printf("Match nul, aucun gagnant !\n");// Message de match nul
            fin_du_jeu = 2;
        } else {
            symbole = (symbole == 'X') ? 'O' : 'X';  // Changement de joueur (alterne X et O)
    }
}
}
//__________________________________________________________________________________
int choisir_coup_aleatoire_console(char grille_console[]) {//BOT FACILE POUR LA PARTIE CONSOLE prend en paramètre la grille DU CONSOLE
    int coups_possibles[9];//Déclarer un tableau pour stocker tous les indices des cases libres (maximum 9 car il y a 9 cases dans la grille)
    int nb_coups = 0;//Initialise un compteur à 0. Il va compter combien de cases sont encore disponibles (ni ‘X’ ni ‘O’)

    int i;
    for (i = 0; i < 9; i++) {//Démarrer une boucle pour parcourir toutes les cases de la grille (de 0 à 8)
        //Vérifie si la case à l’indice i n’est pas occupée par un joueur (donc c’est une case encore libre)
        if (grille_console[i] != 'X' && grille_console[i] != 'O') {
            //Si la case est libre, on stocke son indice dans le tableau coups_possibles, et on incrémente le nombre de coups possibles (nb_coups)
            coups_possibles[nb_coups++] = i;
        }
    }
    //Si aucune case libre n’a été trouvée, la fonction retourne -1 (ce qui signifie qu’il est impossible de jouer, car la grille est pleine)
    if (nb_coups == 0) return -1; // plus de coups possibles
    //Choisit aléatoirement un indice compris entre 0 et nb_coups - 1
    int index = rand() % nb_coups;
    return coups_possibles[index];//On retourne l’indice réel d’une case libre
}
//__________________________________________________________________________________________
void start_game_vs_bot(char grille_console[], char symbole_initial, char symbole_bot) {
    int fin_du_jeu = 0;// Contrôle de fin de partie
    char symbole = symbole_initial;// Symbole courant (commence par le joueur choisi)

    while (fin_du_jeu == 0) {// Boucle de jeu tant que ce n’est pas fini
        draw_board(grille_console);// Affiche l’état actuel de la grille

        int position;// Variable pour stocker la case à jouer

        if (symbole == symbole_bot) {// Si c’est le tour du bot
            printf("Le bot (%c) reflechit...\n", symbole);// Message pour indiquer que le bot joue en cours
            SDL_Delay(1000);// Petite pause pour simuler un délai de réflexion en utilisant la BIB SDL (JDIDA)

           position = choisir_coup_aleatoire_console(grille_console);
           grille_console[position] = symbole;
           printf("Le bot joue en position : %d\n", position + 1);
        } else {
            position = put_X_O(grille_console, symbole); // Si c’est le joueur humain, demande son coup
            grille_console[position - 1] = symbole;// Car put_X_O retourne de 1 à 9
        }

        // Vérification de fin de partie
        if (verifier_victoire(grille_console)) {
            draw_board(grille_console);// Réaffiche la grille
            printf("Le joueur %c a gagné !\n", symbole);// Affiche le gagnant
            fin_du_jeu = 1;
        } else if (check_draw(grille_console)) {//aucun gagnant
            draw_board(grille_console);// Réaffiche la grille
            printf("Match nul, aucun gagnant !\n");// Affiche message match nul
            fin_du_jeu = 2;
        } else {//encore de case vide
            symbole = (symbole == 'X') ? 'O' : 'X'; // Change de joueur
        }
    }
}
//============================================================================================================================
//Pour le bckground ET la partie graphique (PARTIE JDIDA)

void afficherImageAvecStart(SDL_Renderer* renderer) {
    //Chargement d'une image .bmp dans une "surface"
    SDL_Surface* surface = SDL_LoadBMP("assets/game_start_frames_bmp/frame_140_delay-0.03s.bmp");
    //Conversion de la surface en texture
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);//On lie cette texture au renderer espace de dessin SDL.

    SDL_FreeSurface(surface);//Libération de la surface

    Mix_Chunk* son = Mix_LoadWAV("assets/click_sound.wav");//Chargement d’un son du bouton invisible

    SDL_Rect boutonInvisible = {300, 430, 200, 60};  // Zone invisible sur "C'est parti" (bouton cliquable)
    int bouton_clique = 0;//contrôle si le bouton a été pressé
    SDL_Event event;//sert à capter les événements (clic, fermeture)

    while (!bouton_clique) {//Tant que le bouton n’est pas cliqué, on continue à afficher l’image.
        SDL_RenderClear(renderer);//Avant chaque nouveau rendu, on efface ce qu’il y avait.
        SDL_RenderCopy(renderer, texture, NULL, NULL);//Collage (copie) de la texture sur l’écran
        //On colle toute l’image (NULL, NULL = tout prendre et tout afficher)
        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&event)) {//On vérifie s’il y a un clic de souris ou une fermeture de fenêtre
            if (event.type == SDL_QUIT) exit(0);//Si l’utilisateur ferme la fenêtre, on quitte le programme immédiatement
            if (event.type == SDL_MOUSEBUTTONDOWN) {//Détection d’un clic de souris
            // Récupération de la position du clic (x, y).
                int x = event.button.x;
                int y = event.button.y;
            //Vérifie si le clic est à l’intérieur du rectangle invisible.
                if (x >= boutonInvisible.x && x <= boutonInvisible.x + boutonInvisible.w &&
                    y >= boutonInvisible.y && y <= boutonInvisible.y + boutonInvisible.h) {
                    Mix_PlayChannel(-1, son, 0);//Jouer le son de clic (1 seule fois, sur n’importe quel canal libre)
                    //On quitte la boucle (clic détecté) + petite pause pour l’effet
                    bouton_clique = 1;
                    SDL_Delay(200);
                }
            }
        }
    }
    //Nettoyage mémoire On libère la texture et le son maintenant qu’ils ne sont plus nécessaires
    SDL_DestroyTexture(texture);
    Mix_FreeChunk(son);
}
//_________________________________________________________________________________
int afficherChoixBotOuAmi(SDL_Renderer* renderer) {//Elle prend un pointeur SDL_Renderer* en paramètre, c’est l’outil utilisé pour dessiner sur l’écran SDL (la fenêtre graphique)
    SDL_Surface* surface = SDL_LoadBMP("assets/choix_bot_ami.bmp");//SDL_Surface* est une image brute chargée en mémoire RAM (non affichée encore)
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);//Convertir cette surface en texture SDL (prête à être dessinée dans la fenêtre)
    SDL_FreeSurface(surface);//Libérer la mémoire de la surface (plus utile une fois convertie en texture)

    SDL_Rect boutonBot = {200, 580, 240, 60};//Rectangle = {position_x, position_y, largeur, hauteur}
    SDL_Rect boutonAmi = {530, 580, 240, 60};//Définir un rectangle invisible qui recouvre le bouton "Jouer avec un ami"
    SDL_Event event;//Déclaration d'une variable qui servira à écouter les événements SDL (clavier, souris..)
    int choix = -1;//Variable de retour. Elle restera à -1 tant qu’aucun bouton n’a été cliqué

    while (choix == -1) {//-1 me protège contre des erreurs logiques ou des confusions dans les conditions
        SDL_RenderClear(renderer);//Effacer l’ancien affichage dans la fenêtre.
        SDL_RenderCopy(renderer, texture, NULL, NULL);// Copier l’image de fond (texture) dans la fenêtre.
        SDL_RenderPresent(renderer);//Mettre à jour l’écran pour afficher le contenu.

        while (SDL_PollEvent(&event)) {// Boucle d'écoute des événements : elle vérifie tout ce que l’utilisateur fait (clic, fermeture..)
            if (event.type == SDL_QUIT) exit(0);//Si l’utilisateur ferme la fenêtre, on quitte immédiatement le programme
            if (event.type == SDL_MOUSEBUTTONDOWN) {// Si l’utilisateur clique avec la souris, on entre dans ce bloc
                int x = event.button.x;//On récupère les coordonnées x et y du clic souris
                int y = event.button.y;

                //Si le clic est dans la zone du bouton bot, alors
                if (x >= boutonBot.x && x <= boutonBot.x + boutonBot.w &&
                    y >= boutonBot.y && y <= boutonBot.y + boutonBot.h) {
                    choix = 1; // Bot
                } else if (x >= boutonAmi.x && x <= boutonAmi.x + boutonAmi.w &&
                           y >= boutonAmi.y && y <= boutonAmi.y + boutonAmi.h) {
                    choix = 2; // Ami
                }
            }
        }
    }

    SDL_DestroyTexture(texture);//libère la mémoire utilisée par la texture
    return choix;//On retourne le choix final de l’utilisateur (1 ou 2)
}

//________________________________________________________________________________
// les variables globales
//en dehors de toutes les fonctions CAR grille est une variable globale
//Elle est visible dans toutes les fonctions SDL de mon jeu, sans avoir à la passer en paramètre
// Coordonnées de la grille (3x3)
int grille[9] = {0}; // 0 = vide, 1 = X, 2 = O / toutes les cases du tableau sont mises à zéro
int joueur_actuel = 1; // 1 = X, 2 = O / Pas besoin de le passer en paramètre à chaque fois
//__________________________________________________________________________________
// Affiche le symbole X ou O à la bonne position
void afficher_symbole(SDL_Renderer* renderer, int case_index, int joueur) {
   // Taille d’une case dans la grille (chaque carré fait 100x100 pixels)
    int tailleCase = 100;
    // Marge autour de la grille (espace vide entre le bord de la fenêtre et la grille)
    int marge = 50;
    // Calcul de la ligne (row) : division entière de l’index par 3
    int row = case_index / 3;
    // Calcul de la colonne (col) : reste de la division par 3
    int col = case_index % 3;
    // SDL_Rect est une structure contenant les coordonnées et dimensions du rectangle où dessiner
    // On calcule la position X et Y à partir de la marge + la taille de la case multipliée par l’indice ligne/colonne
    SDL_Rect rect = { marge + col * tailleCase, marge + row * tailleCase, tailleCase, tailleCase };
    // Si c’est le joueur 1 (X)
    if (joueur == 1) {
        //DESSINER X EN ROUGE
        // X en rouge // On choisit la couleur rouge pour le X (r=255, g=0, b=0, opacité=255)
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        // On trace une première ligne de la diagonale haut-gauche vers bas-droit
        SDL_RenderDrawLine(renderer, rect.x + 10, rect.y + 10, rect.x + rect.w - 10, rect.y + rect.h - 10);
        // On trace une deuxième ligne de la diagonale haut-droit vers bas-gauche (pour former un X)
        SDL_RenderDrawLine(renderer, rect.x + rect.w - 10, rect.y + 10, rect.x + 10, rect.y + rect.h - 10);
    } else if (joueur == 2) {
        //DESSINER O EN BLEU
        // O en bleu
        // On choisit la couleur bleue pour le O
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        // Calcul du centre du rectangle (coordonnée x du centre)
        int centerX = rect.x + rect.w / 2;
        // Calcul du centre du rectangle (coordonnée y du centre)
        int centerY = rect.y + rect.h / 2;
        // Rayon du cercle (pour O) = 40 pixels
        int radius = 40;

        // Dessin du cercle : on utilise une approximation en traçant des points sur le cercle
        int w;
        for ( w = 0; w < 360; w++) {
           // On convertit chaque angle en radians (formule : angle * π / 180)
            double angle = w * M_PI / 180;
             // Calcul de la position X d’un point du cercle
            int x = centerX + radius * cos(angle);
            // Calcul de la position Y d’un point du cercle
            int y = centerY + radius * sin(angle);
            // On dessine le point à l’écran
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
}
//___________________________________________________________________________________________________
// Dessine la grille blanche par-dessus le fond
void dessiner_grille(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Blanc
    int tailleCase = 100;
    int marge = 50;

    SDL_RenderDrawLine(renderer, marge + tailleCase, marge, marge + tailleCase, marge + 3 * tailleCase);
    SDL_RenderDrawLine(renderer, marge + 2 * tailleCase, marge, marge + 2 * tailleCase, marge + 3 * tailleCase);
    SDL_RenderDrawLine(renderer, marge, marge + tailleCase, marge + 3 * tailleCase, marge + tailleCase);
    SDL_RenderDrawLine(renderer, marge, marge + 2 * tailleCase, marge + 3 * tailleCase, marge + 2 * tailleCase);
}
//_______________________________________________________________________________________________________
// Donne la case (0-8) selon un clic (x,y) /Cette fonction transforme un clic de souris (coordonnées x et y) en numéro de case dans la grille du jeu (entre 0 et 8
int detecter_case(int x, int y) {
    int tailleCase = 100;//On fixe la taille d'une case de la grille (en pixels)
    int marge = 50;// C’est la marge (espace) entre le bord de la fenêtre et la grille. Cela veut dire que la grille commence à 50 pixels du bord haut et gauche
    //si le clic est en dehors de la grille
    if (x < marge || y < marge || x > marge + 3 * tailleCase || y > marge + 3 * tailleCase)
        return -1;

    int col = (x - marge) / tailleCase;
    int row = (y - marge) / tailleCase;
    return row * 3 + col;
}

//_________________________________________________________________________________
int verifier_victoire_sdl(int grille[]) {//MEME ESPRIT QUE CELLE DE CONSOLE
    int combinaisons[8][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8},
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8},
        {0, 4, 8}, {2, 4, 6}
    };

    int i;
    for (i = 0; i < 8; i++) {
        int a = combinaisons[i][0];
        int b = combinaisons[i][1];
        int c = combinaisons[i][2];

        if (grille[a] != 0 && grille[a] == grille[b] && grille[b] == grille[c]) {
            return 1;
        }
    }
    return 0;
}
//____________________________________________________________________________________________
int check_draw_sdl(int grille[]) {//MEME ESPRIT QUE CELLE DE CONSOLE
    int i;
    for ( i = 0; i < 9; i++) {
        if (grille[i] == 0) {
            return 0; // Il reste une case vide
        }
    }
    return 1; // Match nul
}
//________________________________________________________________________________________

void afficher_image_victoire(SDL_Renderer* renderer, int joueur) {
    const char* chemin = (joueur == 1) ? "assets/JOUEURX.bmp" : "assets/JOUEURO.bmp";
    SDL_Surface* victoire = SDL_LoadBMP(chemin);
    SDL_Texture* textureVictoire = SDL_CreateTextureFromSurface(renderer, victoire);
    SDL_FreeSurface(victoire);

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, textureVictoire, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_Delay(2000); // Pause pour montrer la victoire

    SDL_DestroyTexture(textureVictoire);
}
//_____________________________________________________________________________________
int afficher_choix_rejouer_fermer(SDL_Renderer* renderer) {
    SDL_Surface* surface = SDL_LoadBMP("assets/choix_rejouerfermer - Copie.bmp");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    SDL_Rect boutonRejouer = {265, 550, 150, 40};
    SDL_Rect boutonFermer = {460, 550, 150, 40};

    SDL_Event event;
    int choix = -1;

    while (choix == -1) {
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) exit(0);
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x = event.button.x;
                int y = event.button.y;

                if (x >= boutonRejouer.x && x <= boutonRejouer.x + boutonRejouer.w &&
                    y >= boutonRejouer.y && y <= boutonRejouer.y + boutonRejouer.h) {
                    choix = 1; // Rejouer
                } else if (x >= boutonFermer.x && x <= boutonFermer.x + boutonFermer.w &&
                           y >= boutonFermer.y && y <= boutonFermer.y + boutonFermer.h) {
                    choix = 2; // Fermer
                }
            }
        }
    }
    SDL_DestroyTexture(texture);
    return choix;
}
//____________________________________________________________________________
void lancerJeuInteractif(SDL_Renderer* renderer, int Choix) {
    SDL_Event event;  // Déclaration de la variable pour gérer les événements (clics, fermeture fenêtre
    int running = 1; // Booléen pour contrôler la boucle de jeu (1 = continuer, 0 = arrêter)
    int jeu_termine = 0; // Booléen pour savoir si la partie est terminée (0 = non, 1 = oui)
    int coup_bot;// Variable pour stocker la position choisie par le bot
    int i;// Variable d’itération

    // Charger l'image de fond de la grille
    SDL_Surface* fond = SDL_LoadBMP("assets/menu.bmp");// Charge l’image BMP dans une surface temporaire
    SDL_Texture* fondTexture = SDL_CreateTextureFromSurface(renderer, fond);// Convertit la surface en texture pour l’affichage
    SDL_FreeSurface(fond);// Libère la mémoire occupée par la surface, maintenant que la texture est créée

    srand(time(NULL));// Initialise le générateur de nombres aléatoires (pour le bot)

    // Réinitialiser le contenu de la grille à 0 (cases vides)
    for (i = 0; i < 9; i++) grille[i] = 0;// Chaque case de la grille vaut 0 = vide
    joueur_actuel = 1;// Le joueur X commence toujours (1 = X, 2 = O)

    while (running) {// Boucle principale du jeu SDL
        SDL_RenderClear(renderer);// Nettoie l’écran (efface ce qui est affiché avant)
        SDL_RenderCopy(renderer, fondTexture, NULL, NULL);// Affiche le fond d’écran (image fixe de fond)

        dessiner_grille(renderer);// Affiche le fond d’écran (image fixe de fond)
        for (i = 0; i < 9; i++) {
            if (grille[i] != 0)// Si la case n’est pas vide
                afficher_symbole(renderer, i, grille[i]);// Affiche X ou O dans cette case
        }
        SDL_RenderPresent(renderer);// Affiche tout sur l’écran (rendu)

        if (!jeu_termine) {// Si la partie n’est pas encore terminée
            if (Choix == 1 && joueur_actuel == 2) {// Si le joueur a choisi le mode contre bot et que c’est au bot de jouer
                SDL_Delay(500);// Petite pause pour simuler que le bot “réfléchit”
                coup_bot = choisir_coup_bot(grille);// Le bot choisit une case intelligente
                if (coup_bot != -1) {// Si le bot a pu jouer
                    grille[coup_bot] = 2;// On place le symbole du bot (2 = O)
                    if (verifier_victoire_sdl(grille)) {
                        afficher_image_victoire(renderer, 2); // Affiche l’image “JOUEURO.bmp”
                        jeu_termine = 1;
                    } else if (check_draw_sdl(grille)) {
                        jeu_termine = 1; // Match nul
                    }
                    joueur_actuel = 1;// À X de jouer
                }
            } else {
                while (SDL_PollEvent(&event)) {// On écoute les événements SDL (clics souris)
                    if (event.type == SDL_QUIT) return;// Si l’utilisateur ferme la fenêtre
                    else if (event.type == SDL_MOUSEBUTTONDOWN) {// Si clic de souris
                        int x = event.button.x;
                        int y = event.button.y;
                        int case_index = detecter_case(x, y);// On déduit quelle case a été cliquée

                        if (case_index != -1 && grille[case_index] == 0) {
                            grille[case_index] = joueur_actuel;// Place le symbole dans la case vide
                            if (verifier_victoire_sdl(grille)) {
                                afficher_image_victoire(renderer, joueur_actuel);// Affiche image du gagnant
                                jeu_termine = 1;
                            } else if (check_draw_sdl(grille)) {
                                jeu_termine = 1;// Match nul
                            }
                            joueur_actuel = (joueur_actuel == 1) ? 2 : 1;// Change de joueur
                        }
                    }
                }
            }
        }
        if (jeu_termine) {
            SDL_Delay(1000);// Pause avant d’afficher les options rejouer/fermer
            int action = afficher_choix_rejouer_fermer(renderer);// Affiche l’image avec 2 boutons invisibles
            if (action == 1) {
                lancerJeuInteractif(renderer, Choix); // Si le joueur clique "Rejouer", on relance la fonction
            } else {
                printf("\nRetour au mode console...\n");// Sinon, message et retour console
                return;
            }
            running = 0;// On arrête la boucle du jeu
        }
    }

    SDL_DestroyTexture(fondTexture);// Libère la mémoire de la texture fond
}
//=================================================================================================
int main() {
    int Choix = -1;
    int choix_mode = -1;

    printf("=====================================\n");
    printf("      Bienvenue au Tic-Tac-Toe !      \n");
    printf("=====================================\n\n");
    printf("Souhaites-tu jouer en version texte ou graphique ?\n");
    printf("1 - Noir FACILE (console)\n");
    printf("2 - Couleur DIFFICILE (SDL)\n");
    printf("Tape ton choix (1 ou 2) : ");
    scanf("%d", &choix_mode);

    // ========= CONSOLE ============================================================
    if (choix_mode == 1) {
        int rejouer = 1;
        while (rejouer) {
            choisir_options(&Choix);
            char symbole_courant;
            char grille_console[9] = {'1','2','3','4','5','6','7','8','9'};

            symbole_courant = menu();//Le joueur choisit son symbole préféré (‘X’ ou ‘O’)

            if (Choix == 1) {//joueur contre bot
                char symbole_bot = (symbole_courant == 'X') ? 'O' : 'X';//APRES LE CHOIX DE L UTILISATEUR Le bot prend l’autre symbole automatiquement
                start_game_vs_bot(grille_console, symbole_courant, symbole_bot);
            } else {
                start_game(grille_console, symbole_courant);
            }

            printf("\nVeux-tu rejouer ? (1 = Oui / 0 = Non) : ");
            scanf("%d", &rejouer);
        }
    }
    // ========= SDL ===============================================================
    if (choix_mode == 2) {
        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
        IMG_Init(IMG_INIT_PNG);
        Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

        SDL_Window* window = SDL_CreateWindow("Tic Tac Toe", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
        SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

        afficherImageAvecStart(renderer);              // Image "Game Start"
        int choixSDL = afficherChoixBotOuAmi(renderer); // 2e image : Bot ou Ami

        if (choixSDL == 1) {
            lancerJeuInteractif(renderer, 1); // contre bot
        } else {
            lancerJeuInteractif(renderer, 2); // contre ami
        }

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        Mix_CloseAudio();
        IMG_Quit();
        SDL_Quit();
    }

    return 0;
}

