#include "Headers/logic.h"

int main(int argc, char* argv[]) {
	Snake::FONT_PATH = argv[0];
	size_t lastBackslashPos = Snake::FONT_PATH.find_last_of("\\");
	if (lastBackslashPos != std::string::npos) {
		Snake::FONT_PATH = Snake::FONT_PATH.substr(0, lastBackslashPos) + "/fonts/";
	}

	if (Snake::SDL_main() == 1) {
		return 1;
	}

	initializeTiles();
	placeSnake();
	spawnFruit();

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
					changeDirection(UP);
					break;
				case SDLK_a:
					changeDirection(LEFT);
					break;
				case SDLK_s:
					changeDirection(DOWN);
					break;
				case SDLK_d:
					changeDirection(RIGHT);
					break;
				case SDLK_p:
					Snake::pause = !Snake::pause;
					break;
				}
				break;
			case SDL_KEYUP:
				break;
			}
		}

		// Game logic
		currentTime = SDL_GetTicks();
		int frameTime = 500;
		uint32_t elapsedTime = currentTime - lastTime;
		if (elapsedTime >= frameTime) {
			if (!Snake::pause) {
				moveSnake();
			}
			lastTime = currentTime;
		}

		// Renderer
		SDL_RenderClear(renderer);
		renderGame();

		checkIfLost();
		SDL_RenderPresent(Snake::renderer);

		int delay = elapsedTime - frameTime;
		if (delay > 0) {
			SDL_Delay(delay);
		}
	}

	Snake::freeResources();
	return 0;
}