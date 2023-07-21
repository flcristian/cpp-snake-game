#include "game.cpp"

using namespace Snake;

enum TileState {
	EMPTY,
	SNAKEHEAD,
	SNAKEBODY,
	FRUIT
};

const int tileCount = 15;
TileState tiles[tileCount][tileCount];

void spawnFruit() {
	LOG("Spawned fruit!");
}

void initializeTiles() {
	for (int i = 0; i < tileCount; i++) {
		for (int j = 0; j < tileCount; j++) {
			tiles[i][j] = EMPTY;
		}
	}

	int mid = tileCount / 2;
	tiles[mid][mid] = SNAKEHEAD;
	tiles[mid][mid + 1] = SNAKEBODY;
	tiles[mid][mid + 2] = SNAKEBODY;
}

void obtainMousePosition(int& mouseX, int& mouseY) {
	uint32_t mouseState = SDL_GetMouseState(&mouseX, &mouseY);
}

void renderBackground(int x, int y, int tileSize) {
	SDL_Rect border = { x * tileSize,y * tileSize,tileSize,tileSize };
	SDL_Rect fill = { x * tileSize + 1,y * tileSize + 1,tileSize - 2,tileSize - 2 };
	SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
	SDL_RenderFillRect(renderer, &border);
	SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
	SDL_RenderFillRect(renderer, &fill);
}

void renderSnakeHead(int x, int y, int tileSize) {
	SDL_Rect fill = { x * tileSize,y * tileSize,tileSize,tileSize };
	SDL_SetRenderDrawColor(renderer, 20, 70, 20, 255);
	SDL_RenderFillRect(renderer, &fill);
}

void renderSnakeBody(int x, int y, int tileSize) {
	SDL_Rect fill = { x * tileSize,y * tileSize,tileSize,tileSize };
	SDL_SetRenderDrawColor(renderer, 30, 100, 30, 255);
	SDL_RenderFillRect(renderer, &fill);
}

void renderGame() {
	int size = windowHeight/tileCount;

	for (int i = 0; i < tileCount; i++) {
		for (int j = 0; j < tileCount; j++) {
			TileState tileState = tiles[i][j];

			switch (tileState) {
			case EMPTY:
				renderBackground(i, j, size);
				break;
			case SNAKEHEAD:
				renderSnakeHead(i, j, size);
				break;
			case SNAKEBODY:
				renderSnakeBody(i, j, size);
				break;
			default:
				break;
			}
		}
	}
}	