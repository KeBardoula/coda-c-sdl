#include <SDL2/SDL.h>
#include <stdio.h>

// Foonction pour charger une texture à partir d'un fichier
SDL_Texture* loadTexture(const char* file, SDL_Renderer* renderer) {
    SDL_Surface* surface = SDL_LoadBMP(file);
    if (!surface) {
        printf("Erreur de chargement de l'image: %s\n", SDL_GetError());
        return NULL;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

int main(int argc, char *argv[]) {
    // Initialisation de la SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Erreur d'initialisation de la SDL: %s\n", SDL_GetError());
        return 1;
    }

    // Création de la fenêtre en 160x160 pixels
    SDL_Window *window = SDL_CreateWindow("Exercice 3", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 160, 160, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Erreur de création de la fenêtre: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Créer un rendur pour la fenêtre
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        printf("Erreur de création du rendu: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Charger l'image
    SDL_Texture *imgUp = loadTexture("img/ballUp.bmp", renderer);
    SDL_Texture *imgDown = loadTexture("img/ballDown.bmp", renderer);
    SDL_Texture *imgRight = loadTexture("img/ballRight.bmp", renderer);
    SDL_Texture *imgLeft = loadTexture("img/ballLeft.bmp", renderer);
    if (!imgUp || !imgDown || !imgLeft || !imgRight) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Position de l'image
    int x = 72, y = 72;
    SDL_Texture *currentTexture = imgDown;

    // Boucle principale
    int running = 1;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_z:
                        y -= 5;
                        currentTexture = imgUp;
                        break;
                    case SDLK_s:
                        y += 5;
                        currentTexture = imgDown;
                        break;
                    case SDLK_q:
                        x -= 5;
                        currentTexture = imgLeft;
                        break;
                    case SDLK_d:
                        x += 5;
                        currentTexture = imgRight;
                        break;
                }
            }
        }

        // Effacer l'écran
        SDL_RenderClear(renderer);

        // Définir la position et la taille de l'image 
        SDL_Rect dest = {x, y, 16, 16};

        // Afficher l'image
        SDL_RenderCopy(renderer, currentTexture, NULL, &dest);

        // Mettre à jour l'écran
        SDL_RenderPresent(renderer);
    }

    // Libérer la mémoire
    SDL_DestroyTexture(imgUp);
    SDL_DestroyTexture(imgDown);
    SDL_DestroyTexture(imgLeft);
    SDL_DestroyTexture(imgRight);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}