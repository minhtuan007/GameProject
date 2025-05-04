#include <SDL.h>
#include <iostream>

const int TILE_SIZE = 50;
const int TILE_SPACING = 10;
const int GRID_ROWS = 4;
const int GRID_COLS = 4;
const int WINDOW_WIDTH = GRID_COLS * (TILE_SIZE + TILE_SPACING) + TILE_SPACING;
const int WINDOW_HEIGHT = GRID_ROWS * (TILE_SIZE + TILE_SPACING) + TILE_SPACING;

void drawShadowBox(SDL_Renderer* renderer, int x, int y) {
    SDL_Rect tile = {x, y, TILE_SIZE, TILE_SIZE};

    // Nền tile
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderFillRect(renderer, &tile);

    // Viền sáng (trên, trái)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawLine(renderer, x, y, x + TILE_SIZE - 1, y); // top
    SDL_RenderDrawLine(renderer, x, y, x, y + TILE_SIZE - 1); // left

    // Viền tối (dưới, phải)
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderDrawLine(renderer, x, y + TILE_SIZE - 1, x + TILE_SIZE - 1, y + TILE_SIZE - 1); // bottom
    SDL_RenderDrawLine(renderer, x + TILE_SIZE - 1, y, x + TILE_SIZE - 1, y + TILE_SIZE - 1); // right
}

void drawBlueInnerBlurTile(SDL_Renderer* renderer, int x, int y) {
    const int BLUR_SIZE = 10;

    for (int i = 0; i < BLUR_SIZE; ++i) {
        int offset = i;
        SDL_Rect r = {x + offset, y + offset, TILE_SIZE - 2 * offset, TILE_SIZE - 2 * offset};

        // Màu xanh dương blur vào trong
        int alpha = 25 + i * (200 / BLUR_SIZE);  // alpha tăng
        int blue = 255 - i * (60 / BLUR_SIZE);   // xanh giảm nhẹ
        int rColor = 100 - i * (40 / BLUR_SIZE); // đỏ giảm

        SDL_SetRenderDrawColor(renderer, rColor, 150, blue, alpha);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_RenderFillRect(renderer, &r);
    }

    // Phần lõi ở giữa
    SDL_SetRenderDrawColor(renderer, 60, 120, 255, 255);
    SDL_Rect core = {x + BLUR_SIZE, y + BLUR_SIZE, TILE_SIZE - 2 * BLUR_SIZE, TILE_SIZE - 2 * BLUR_SIZE};
    SDL_RenderFillRect(renderer, &core);
}

bool isMouseOver(int mouseX, int mouseY, int tileX, int tileY) {
    return (mouseX >= tileX && mouseX < tileX + TILE_SIZE &&
            mouseY >= tileY && mouseY < tileY + TILE_SIZE);
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << '\n';
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Blur Hover Tile Effect",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << '\n';
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << '\n';
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = true;
        }

        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        // Vẽ nền
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        for (int row = 0; row < GRID_ROWS; ++row) {
            for (int col = 0; col < GRID_COLS; ++col) {
                int x = TILE_SPACING + col * (TILE_SIZE + TILE_SPACING);
                int y = TILE_SPACING + row * (TILE_SIZE + TILE_SPACING);

                if (isMouseOver(mouseX, mouseY, x, y)) {
                    drawBlueInnerBlurTile(renderer, x, y);
                } else {
                    drawShadowBox(renderer, x, y);
                }
            }
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
