#include <SDL2/SDL.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    // Initialisation de la SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Erreur d'initialisation de la SDL: %s", SDL_GetError());
        return 1;
    }

    // Création de la fenêtre de 320x320 pixels
    SDL_Window *window = SDL_CreateWindow("Exercice 2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 320, 320, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Erreur de création de la fenêtre: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Création du rendu pour la fenêtre
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Erreur de création du rendu: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Charger l'image de 16x16 pixels
    SDL_Surface *imageSurface = SDL_LoadBMP("img/bottle.bmp");
    if (imageSurface == NULL) {
        printf("Erreur de chargement de l'image: %s", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Texture *imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    SDL_FreeSurface(imageSurface);
    if (imageTexture == NULL) {
        printf("Erreur de création de la texture: %s", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Boucle principale
    int running = 1;
    SDL_Event event;
    while(running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                // Obtenir les coordonnées du clic de la souris
                int x, y;
                SDL_GetMouseState(&x, &y);

                // Effacer l'écran
                SDL_RenderClear(renderer);

                // Définir la position et la taille de l'image
                SDL_Rect dstrect = {x - 8, y - 8, 16, 16};

                // Afficher l'image
                SDL_RenderCopy(renderer, imageTexture, NULL, &dstrect);

                // Mettre à jour l'écran
                SDL_RenderPresent(renderer);
            }
        }
    }

    // Nettoyer et quitter SDL
    SDL_DestroyTexture(imageTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}