#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Circle Example",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Màu nền đen
        SDL_RenderClear(renderer);

        // Vẽ vòng tròn tại tọa độ (400, 300) với bán kính 50, màu trắng
        filledCircleColor(renderer, 400, 300, 50, 0xFFFFFFFF);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
