#include <SDL2/SDL.h>
#include <array>
#include <chrono>
#include <thread>
#include <algorithm>
#include <iterator>


#define WIDTH 80
#define HEIGHT 45
#define SCALE 10

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Event event;

bool grid[WIDTH][HEIGHT] = {false};
bool backGrid[WIDTH][HEIGHT] = {false};

int mouseX, mouseY;

inline int getNeighbours(const int x, const int y) {
	int liveNeighbours = 0;
	for (int dy = -1; dy <= 1; ++dy) {
		for (int dx = -1; dx <= 1; ++dx) {
			if (dy == 0 && dx == 0) continue;
			const int px = x + dx;
			const int py = y + dy;
			if (px < WIDTH && py < HEIGHT && px >= 0 && py >= 0) {
				if (grid[px][py]) liveNeighbours++;
			}
		}
	}
	return liveNeighbours;
}

inline void tick() {
	for (int y = 0; y < HEIGHT; ++y) {
		for (int x = 0; x < WIDTH; ++x) {
			int liveNeighbours = getNeighbours(x, y);
			bool thisCellAlive = grid[x][y];
			if (thisCellAlive && liveNeighbours < 2) backGrid[x][y] = false;
			else if (thisCellAlive && liveNeighbours > 3) backGrid[x][y] = false;
			else if (!thisCellAlive && liveNeighbours == 3) backGrid[x][y] = true;
		}
	}
	std::copy(&backGrid[0][0], &backGrid[0][0]+HEIGHT*WIDTH, &grid[0][0]);
}

inline void renderGrid() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
	for (int y = 0; y < HEIGHT; ++y) {
		for (int x = 0; x < WIDTH; ++x) {
			if (grid[x][y]) SDL_RenderDrawPoint(renderer, x, y);
		}

	}
	SDL_RenderPresent(renderer);
}


int main() {
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_CreateWindowAndRenderer(WIDTH * SCALE, HEIGHT * SCALE, 0, &window, &renderer);
	SDL_RenderSetScale(renderer, SCALE, SCALE);

	bool mouseDown = false;
	bool quit = false;
	bool run = false;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
        	switch (event.type) {
	            case SDL_QUIT: {
	                quit = true;
	                break;
	            }
	            case SDL_KEYDOWN: {
		            switch( event.key.keysym.sym ){
		                case SDLK_SPACE: {
		                    run = !run;
		                    break;
		                }
		                case SDLK_RIGHT: {
		                	tick();
		                	break;
		                }
		                default:
		                    break;
		            }
		            break;
	            }
	            case SDL_MOUSEMOTION: {
	            	SDL_GetMouseState(&mouseX, &mouseY);
	            	if (mouseDown) {
	            		const int px = mouseX / SCALE;
            			const int py = mouseY / SCALE;
            			if (px < WIDTH && py < HEIGHT && px >= 0 && py >= 0) {
            				grid[px][py] = true;
            				backGrid[px][py] = true;
            			}
	            	}
	            	break;
	            }
	            case SDL_MOUSEBUTTONDOWN: {
	            	SDL_GetMouseState(&mouseX, &mouseY);
	            	const int px = mouseX / SCALE;
	            	const int py = mouseY / SCALE;
	            	if (px < WIDTH && py < HEIGHT && px >= 0 && py >= 0) {
	            		grid[px][py] = !grid[px][py];
	            		backGrid[px][py] = !backGrid[px][py];
	            	}
	                mouseDown = true;
	                break;
	            }
	            case SDL_MOUSEBUTTONUP: {
	            	mouseDown = false;
	            	break;
	            }
	            default:
	            	break;
        	}
        }

        if (run) tick();
		renderGrid();

		if (run) std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

	return 0;
}
