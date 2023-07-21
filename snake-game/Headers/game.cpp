#include <SDL.h>
#include <iostream>

#define LOG(x) std::cout << x << std::endl

namespace Snake {
	// Creating window
	char windowTitle[100] = "Snake Game";
	int windowPosX = SDL_WINDOWPOS_CENTERED;
	int windowPosY = SDL_WINDOWPOS_CENTERED;
	int windowWidth = 900;
	int windowHeight = 900;
	uint32_t windowFlags = SDL_WINDOW_SHOWN;
	SDL_Window* window = SDL_CreateWindow(windowTitle, windowPosX, windowPosY, windowWidth, windowHeight, windowFlags);

	// Creating renderer
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	// Mouse position
	int mouseX, mouseY;
	std::string orientation = "UP";

	int main() {
		if (SDL_Init(SDL_INIT_VIDEO) != 0) {
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "SDL", "Initialization failed!", NULL);
			return 1;
		}
		if (window == NULL) {
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "SDL", "Window creation failed!", NULL);
			SDL_Quit();
			return 1;
		}
		if (renderer == NULL) {
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "SDL", "Renderer creation failed!", NULL);
			SDL_DestroyWindow(window);
			SDL_Quit();
			return 1;
		}

		return 0;
	}
}