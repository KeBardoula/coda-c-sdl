#include <SDL2/SDL.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    // Initialisation de la SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Erreur d'initialisation de la SDL: %s\n", SDL_GetError());
        return 1;
    }

    // Création de la fenêtre en 160x160 pixels
    SDL_Window *window = SDL_CreateWindow("Exercice 1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 160, 160, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Erreur de création de la fenétre: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Création du rendu pour la fenêtre
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Erreur de création du rendu: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Charger l'image de 16X16 pixels
    SDL_Surface *imageSurface = SDL_LoadBMP("img/bottle.bmp");
    if (imageSurface == NULL) {
        printf("Erreur de chargement de l'image: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Texture *imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    SDL_FreeSurface(imageSurface);
    if (imageTexture == NULL) {
        printf("Erreur de création de la texture: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Déterminer les coordonnées de l'image
    int x = (160 - 16) / 2;
    int y = (160 - 16) / 2;

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

        // Définir la position et la taille de l'image
        SDL_Rect dstrect = {x, y, 16, 16};

        // Afficher l'image
        SDL_RenderCopy(renderer, imageTexture, NULL, &dstrect);

        // Mettre à jour l'écran
        SDL_RenderPresent(renderer);
    }

    // Nettoyer et quitter SDL
    SDL_DestroyTexture(imageTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}