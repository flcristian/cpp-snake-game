#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <random>
#include <filesystem>
#include <string>

#define LOG(x) std::cout << x << std::endl

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
	std::string GAME_OVER_TEXT = "YOU LOST";
	std::string SCORE_TEXT;

	TTF_Font* game_over_font;
	TTF_Font* score_font;
	SDL_Surface* game_over_textSurface;
	SDL_Texture* game_over_textTexture;
	SDL_Surface* score_textSurface;
	SDL_Texture* score_textTexture;

	// Game score
	int score = 0;

	// Game states
	bool pause = false, lost = false;

	// Checking for initialization failures
	int main() {
		TTF_Init();
		return 0;
	}

	void freeResources() {
		TTF_CloseFont(game_over_font);
		TTF_CloseFont(score_font);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_DestroyTexture(game_over_textTexture);
		SDL_FreeSurface(game_over_textSurface);
		SDL_DestroyTexture(score_textTexture);
		SDL_FreeSurface(score_textSurface);
		TTF_Quit();
		SDL_Quit();
	}
}