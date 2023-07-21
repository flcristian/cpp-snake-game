#include "Headers/logic.h"

int main(int argc, char* argv[]) {
	if (Snake::SDL_main() == 1) {
		return 1;
	}

	initializeTiles();

	bool quit = false;
	SDL_Event event;
	uint32_t lastTime = SDL_GetTicks();
	uint32_t currentTime = 0;

	while (!quit) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
					quit = true;
					break;
				case SDLK_w:
					Snake::orientation = "UP";
					break;
				case SDLK_a:
					Snake::orientation = "LEFT";
					break;
				case SDLK_s:
					Snake::orientation = "DOWN";
					break;
				case SDLK_d:
					Snake::orientation = "RIGHT";
					break;
				}
				break;
			case SDL_KEYUP:
				break;
			}
		}

		// Game logic
		currentTime = SDL_GetTicks();
		uint32_t elapsedTime = currentTime - lastTime;
		if (elapsedTime >= 1000) {
			LOG(Snake::orientation);
			lastTime = currentTime;
		}

		// Renderer
		SDL_RenderClear(renderer);
		renderGame();

		SDL_RenderPresent(Snake::renderer);

		int delay = elapsedTime - 1000;
		if (delay > 0) {
			SDL_Delay(delay);
		}
	}

	SDL_DestroyRenderer(Snake::renderer);
	SDL_DestroyWindow(Snake::window);
	SDL_Quit();
	return 0;
}