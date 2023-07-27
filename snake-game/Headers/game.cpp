#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <random>
#include <filesystem>
#include <string>

namespace Snake {
	// Creating random number generator
	std::random_device rd;
	std::mt19937 gen(rd());

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

	// Snake directions
	enum Direction {
		UP = 0,
		RIGHT = 1,
		DOWN = 2,
		LEFT = 3
	};

	Direction orientation = UP;

	// Game font
	std::string CURRENT_PATH = std::filesystem::current_path().string();
	std::string FONT_PATH;
	std::string GAME_OVER_TEXT = "YOU LOST", SCORE_TEXT, RESTART_TEXT = "Press R to restart";

	TTF_Font* game_over_font;
	TTF_Font* score_font;
	TTF_Font* restart_font;
	SDL_Surface* game_over_textSurface;
	SDL_Texture* game_over_textTexture;
	SDL_Surface* score_textSurface;
	SDL_Texture* score_textTexture;
	SDL_Surface* restart_textSurface;
	SDL_Texture* restart_textTexture;

	// Game score
	int score = 0;

	// Game states
	bool pause = false, lost = false;

	// Check if memory was cleared
	bool memoryFreed = false;

	// Checking for initialization failures
	int main() {
		TTF_Init();
		return 0;
	}

	void cleanupLostGameResources() {
		TTF_CloseFont(game_over_font);
		TTF_CloseFont(restart_font);
		TTF_CloseFont(score_font);
		SDL_DestroyTexture(game_over_textTexture);
		SDL_DestroyTexture(restart_textTexture);
		SDL_DestroyTexture(score_textTexture);
		SDL_FreeSurface(game_over_textSurface);
		SDL_FreeSurface(score_textSurface);
		SDL_FreeSurface(restart_textSurface);

		memoryFreed = true;
	}

	void freeResources() {
		if (!memoryFreed) {
			cleanupLostGameResources();
		}
		
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);

		TTF_Quit();
		SDL_Quit();
	}
}