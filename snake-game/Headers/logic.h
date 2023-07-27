#include "game.cpp"

using namespace Snake;

enum TileState {
	EMPTY = 0,
	SNAKEHEAD = 10,
	SNAKEBODY = 100,
	FRUIT = 5
};

int snakePosX[1000], snakePosY[1000];
const int startLength = 3;
int snakeLength = startLength;
int fruitX, fruitY;

int tileCount = 9;
TileState tiles[100][100];

void spawnFruit() 
{
	std::uniform_int_distribution<> distribution(0, tileCount - 1);
	fruitX = distribution(Snake::gen);
	fruitY = distribution(Snake::gen);

	while (tiles[fruitX][fruitY] != EMPTY) {
		fruitX = distribution(Snake::gen);
		fruitY = distribution(Snake::gen);
	}

	tiles[fruitX][fruitY] = FRUIT;
}

void clearEverything() {
	for (int i = 0; i < tileCount; i++) {
		for (int j = 0; j < tileCount; j++) {
			tiles[i][j] = EMPTY;
		}
	}
}

void emptyTiles() {
	for (int i = 0; i < tileCount; i++) {
		for (int j = 0; j < tileCount; j++) {
			if (tiles[i][j] != FRUIT) {
				tiles[i][j] = EMPTY;
			}
		}
	}
}

void placeSnake() {
	if (snakeLength > 0) {
		tiles[snakePosX[0]][snakePosY[0]] = SNAKEHEAD;
	}
	for (int i = 1; i < snakeLength; i++) {
		tiles[snakePosX[i]][snakePosY[i]] = SNAKEBODY;
	}
}

void initializeTiles() 
{
	emptyTiles();

	for (int i = 0; i < 1000; i++) {
		snakePosX[i] = -1;
		snakePosY[i] = -1;
	}

	int mid = tileCount / 2;
	snakePosX[0] = mid, snakePosY[0] = mid;
	for (int i = 1; i < snakeLength; i++) {
		snakePosX[i] = mid;
		snakePosY[i] = mid + i;
	}
}

void changeDirection(Direction dir) {
	int x = 0, y = 0;
	switch (dir) {
	case UP:
		y--;
		break;
	case RIGHT:
		x++;
		break;
	case DOWN:
		y++;
		break;
	case LEFT:
		x--;
		break;
	}

	if (snakePosX[0] + x != snakePosX[1] || snakePosY[0] + y != snakePosY[1]) {
		Snake::orientation = dir;
	}
}

void displayLostGame() {
	Snake::memoryFreed = false;

	// Displaying overlay
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_Rect overlay = { 0, 0, 900, 900 };
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
	SDL_RenderFillRect(renderer, &overlay);

	// Obtaining font
	std::string FONT_NAME = "snake.ttf";
	game_over_font = TTF_OpenFont((FONT_PATH + FONT_NAME).c_str(), 500);
	score_font = TTF_OpenFont((FONT_PATH + FONT_NAME).c_str(), 500);
	restart_font = TTF_OpenFont((FONT_PATH + FONT_NAME).c_str(), 250);


	// Displaying game over text
	SDL_Color game_over_textColor = { 191, 17, 35, 240 };
	game_over_textSurface = TTF_RenderText_Blended(game_over_font, GAME_OVER_TEXT.c_str(), game_over_textColor);
	game_over_textTexture = SDL_CreateTextureFromSurface(renderer, game_over_textSurface);
	int game_over_textWidth = game_over_textSurface->w;
	int game_over_textHeight = game_over_textSurface->h;
	int game_over_centerX = (windowWidth - game_over_textWidth / 10) / 2;
	int game_over_centerY = (windowHeight - game_over_textHeight / 10) / 2 - 50;
	SDL_Rect gameOverRect = { game_over_centerX, game_over_centerY, game_over_textWidth / 10, game_over_textHeight / 10 };
	SDL_RenderCopy(renderer, game_over_textTexture, NULL, &gameOverRect);

	// Displaying score
	SDL_Color score_textColor = { 255, 56, 59, 240 };
	score_textSurface = TTF_RenderText_Blended(score_font, SCORE_TEXT.c_str(), score_textColor);
	score_textTexture = SDL_CreateTextureFromSurface(renderer, score_textSurface);
	int score_textWidth = score_textSurface->w;
	int score_textHeight = score_textSurface->h;
	int score_centerX = (windowWidth - score_textWidth / 15) / 2;
	int score_centerY = game_over_centerY + 50;
	SDL_Rect scoreRect = { score_centerX, score_centerY, score_textWidth / 15, score_textHeight / 15 };
	SDL_RenderCopy(renderer, score_textTexture, NULL, &scoreRect);

	// Displaying restart game
	SDL_Color restart_textColor = { 255, 255, 255, 240 };
	restart_textSurface = TTF_RenderText_Blended(restart_font, RESTART_TEXT.c_str(), restart_textColor);
	restart_textTexture = SDL_CreateTextureFromSurface(renderer, restart_textSurface);
	int restart_textWidth = restart_textSurface->w;
	int restart_textHeight = restart_textSurface->h;
	int restart_centerX = (windowWidth - restart_textWidth / 10) / 2;
	int restart_centerY = score_centerY + 40;
	SDL_Rect restartRect = { restart_centerX, restart_centerY, restart_textWidth / 10, restart_textHeight / 10 };
	SDL_RenderCopy(renderer, restart_textTexture, NULL, &restartRect);

	Snake::cleanupLostGameResources();
}

void checkIfLost() {
	if (snakeLength == 0) {
		if (!lost) {
			SCORE_TEXT = "Score : " + std::to_string(score);
		}
		Snake::pause = true;
		Snake::lost = true;
	}
}

bool snakeAteFood(int headX, int headY) {
	if (tiles[headX][headY] == FRUIT) {
		Snake::score++;
		spawnFruit();
		return true;
	}
	return false;
}

void moveSnake()
{
	int x = 0, y = 0;
	switch (Snake::orientation) {
	case UP:
		y--;
		break;
	case RIGHT:
		x++;
		break;
	case DOWN:
		y++;
		break;
	case LEFT:
		x--;
		break;
	}

	// Storing snake's head positions for easy usage
	int headX = (tileCount + snakePosX[0] + x) % tileCount, headY = (tileCount + snakePosY[0] + y) % tileCount;

	if (snakeAteFood(headX, headY)) {
		snakePosX[snakeLength] = snakePosX[snakeLength - 1];
		snakePosY[snakeLength] = snakePosY[snakeLength - 1];
		snakeLength++;
	}

	for (int i = snakeLength - 1; i > 0; i--) {
		snakePosX[i] = snakePosX[i - 1];
		snakePosY[i] = snakePosY[i - 1];
	}

	// Check if the snake collides with itself or the arena borders
	if (tiles[headX][headY] == SNAKEBODY) {
		snakeLength = 0;
	}
	else {
		snakePosX[0] = headX;
		snakePosY[0] = headY;
	}

	emptyTiles();
	placeSnake();
}

void obtainMousePosition(int& mouseX, int& mouseY) 
{
	uint32_t mouseState = SDL_GetMouseState(&mouseX, &mouseY);
}

void renderBackground(int x, int y, int tileSize)
{
	SDL_Rect border = { x * tileSize,y * tileSize,tileSize,tileSize };
	SDL_Rect fill = { x * tileSize + 1,y * tileSize + 1,tileSize - 2,tileSize - 2 };
	SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
	SDL_RenderFillRect(renderer, &border);
	SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
	SDL_RenderFillRect(renderer, &fill);
}

void renderSnakeHead(int x, int y, int tileSize) 
{
	SDL_Rect fill = { x * tileSize,y * tileSize,tileSize,tileSize };
	SDL_SetRenderDrawColor(renderer, 20, 70, 20, 255);
	SDL_RenderFillRect(renderer, &fill);
}

void renderSnakeBody(int x, int y, int tileSize) 
{
	SDL_Rect fill = { x * tileSize,y * tileSize,tileSize,tileSize };
	SDL_SetRenderDrawColor(renderer, 30, 100, 30, 255);
	SDL_RenderFillRect(renderer, &fill);
}

void renderFruit(int x, int y, int tileSize)
{
	SDL_Rect fill = { x * tileSize,y * tileSize,tileSize,tileSize };
	SDL_SetRenderDrawColor(renderer, 207, 6, 27, 255);
	SDL_RenderFillRect(renderer, &fill);
}

void renderGame() 
{
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
			case FRUIT:
				renderFruit(i, j, size);
				break;
			}
		}
	}

	if (lost) {
		displayLostGame();
	}
}

void resetGame() {
	// Clearing board
	clearEverything();

	// Resetting board
	snakeLength = startLength;
	initializeTiles();
	placeSnake();
	spawnFruit();

	// Reseting game state
	Snake::score = 0;
	Snake::pause = false;
	Snake::lost = false;
	Snake::orientation = UP;
}