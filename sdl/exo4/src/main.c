#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

// Dimensions de la carte (à adapter si besoin)
#define MAP_WIDTH 20
#define MAP_HEIGHT 10
#define TILE_SIZE 32 // Taille d'une image (par exemple, 32x32 pixels)

// Fonction pour lire le fichier map_level.txt et stocker les niveaux dans une matrice
void lire_fichier_map(int map[MAP_HEIGHT][MAP_WIDTH], const char* chemin) {
    FILE* fichier = fopen(chemin, "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            char c = fgetc(fichier);
            if (c == '\n') c = fgetc(fichier); // Ignorer les nouvelles lignes
            map[y][x] = c - '0'; // Convertir le caractère en entier
        }
    }

    fclose(fichier);
}

int main(int argc, char* argv[]) {
    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erreur SDL: %s\n", SDL_GetError());
        return 1;
    }

    // Création de la fenêtre
    SDL_Window* window = SDL_CreateWindow("Exercice 4", 
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                                          MAP_WIDTH * TILE_SIZE, MAP_HEIGHT * TILE_SIZE, 0);
    if (!window) {
        printf("Erreur lors de la création de la fenêtre: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Création du renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Erreur lors de la création du renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Chargement des images .bmp pour chaque niveau
    SDL_Surface* surfaces[5];
    surfaces[0] = SDL_LoadBMP("img/level0.bmp");
    surfaces[1] = SDL_LoadBMP("img/level1.bmp");
    surfaces[2] = SDL_LoadBMP("img/level2.bmp");
    surfaces[3] = SDL_LoadBMP("img/level3.bmp");
    surfaces[4] = SDL_LoadBMP("img/level4.bmp");

    SDL_Texture* textures[5];
    for (int i = 0; i < 5; i++) {
        if (!surfaces[i]) {
            printf("Erreur lors du chargement de l'image level%d.bmp: %s\n", i, SDL_GetError());
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return 1;
        }
        textures[i] = SDL_CreateTextureFromSurface(renderer, surfaces[i]);
        SDL_FreeSurface(surfaces[i]); // On peut libérer la surface après avoir créé la texture
    }

    // Lire la carte des niveaux à partir du fichier map_level.txt
    int map[MAP_HEIGHT][MAP_WIDTH];
    lire_fichier_map(map, "src/map_level.txt");

    // Boucle principale
    int running = 1;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        // Effacer l'écran
        SDL_RenderClear(renderer);

        // Afficher la carte
        for (int y = 0; y < MAP_HEIGHT; y++) {
            for (int x = 0; x < MAP_WIDTH; x++) {
                int niveau = map[y][x];
                SDL_Rect dstRect = {x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
                SDL_RenderCopy(renderer, textures[niveau], NULL, &dstRect);
            }
        }

        // Mettre à jour l'écran
        SDL_RenderPresent(renderer);

        SDL_Delay(100); // Petite pause pour éviter de trop consommer de ressources
    }

    // Nettoyage
    for (int i = 0; i < 5; i++) {
        SDL_DestroyTexture(textures[i]);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
