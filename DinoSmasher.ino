//DinoSmasher

#include "bitmaps.h"

#include <Arduboy2.h>
Arduboy2 arduboy;

// game states - belongs in globals.h
#define GAME_TITLE    0
#define GAME_PLAY     1
#define GAME_OVER     2
#define GAME_HIGH     3

// Use instead of the above somehow
// enum class GameState {
// 	Title,
// 	Play,
// 	GameOver,
// 	Highscores
// };

// set up variables
int gamestate = GAME_TITLE;

// belongs in title.h
void gameTitle() {
  arduboy.setCursor(0, 0);
  arduboy.print(F("Title Screen\n"));
  
  if (arduboy.justPressed(A_BUTTON)) {
    gamestate = GAME_PLAY;
  }
}

// belongs in game.h
void gamePlay() {
  
  drawWorld();
  drawPlayer();
  playerInput();
  
  if (arduboy.justPressed(A_BUTTON)) {
    gamestate = GAME_OVER;
  }
}

// belongs in over.h
void gameOver() {
  arduboy.setCursor(0, 0);
  arduboy.print(F("Game Over\n"));
  if (arduboy.justPressed(A_BUTTON)) {
    gamestate = GAME_HIGH;
  }
}

// belongs in high.h
void gameHigh() {
  arduboy.setCursor(0, 0);
  arduboy.print(F("High Score\n"));
  if (arduboy.justPressed(A_BUTTON)) {
    gamestate = GAME_TITLE;
  }
}

typedef void (*FunctionPointer) ();

const FunctionPointer PROGMEM mainGameLoop[] = {
  gameTitle,
  gamePlay,
  gameOver,
  gameHigh
};

// drawing world
#define WORLD_WIDTH   14
#define WORLD_HEIGHT  7
#define TILE_SIZE     16
#define GRASS         0
#define WATER         1
#define TREES         2
#define STONE         3

// drawing player
#define PLAYER_SIZE   16
#define PLAYER_X_OFFSET    WIDTH / 2 - PLAYER_SIZE / 2
#define PLAYER_Y_OFFSET    HEIGHT / 2 - PLAYER_SIZE / 2

int world[WORLD_HEIGHT][WORLD_WIDTH] = {
  { TREES, GRASS, GRASS, WATER, GRASS, GRASS, GRASS, TREES, GRASS, GRASS, GRASS, GRASS, GRASS, TREES },
  { GRASS, WATER, WATER, WATER, GRASS, WATER, GRASS, GRASS, GRASS, GRASS, GRASS, STONE, GRASS, GRASS },
  { GRASS, GRASS, GRASS, GRASS, GRASS, WATER, STONE, GRASS, GRASS, GRASS, TREES, GRASS, GRASS, GRASS },
  { STONE, GRASS, GRASS, STONE, TREES, WATER, WATER, WATER, GRASS, WATER, WATER, GRASS, TREES, GRASS },
  { GRASS, GRASS, GRASS, GRASS, TREES, GRASS, GRASS, GRASS, TREES, WATER, GRASS, GRASS, STONE, TREES },
  { GRASS, GRASS, GRASS, WATER, STONE, GRASS, GRASS, TREES, TREES, TREES, GRASS, GRASS, WATER, WATER },
  { GRASS, WATER, WATER, TREES, GRASS, WATER, WATER, TREES, TREES, GRASS, GRASS, GRASS, GRASS, STONE }
};

bool isSolid(int tileIndex) {
	switch (tileIndex) {
	  case GRASS:
	  case WATER:
	    return false;
      
    case TREES:
    case STONE:
      return true;
      
    default:
      return true;
	}
}

bool isPointInTile(int x, int y) {
	return isSolid(world[y / TILE_SIZE][x / TILE_SIZE]);
}

// map coordinates
int mapx = 0;
int mapy = 0;

// number of pixels that can show on the screen at once
const int tileswide = WIDTH / TILE_SIZE + 1;
const int tilestall = HEIGHT / TILE_SIZE + 1;

void playerInput() {
  int playerX = -mapx + PLAYER_X_OFFSET;
  int playerY = -mapy + PLAYER_Y_OFFSET;
  
  // current player position
  int playerLeft = (playerX) + 1;
  int playerRight = (playerX + PLAYER_SIZE) - 1;
  int playerTop = (playerY) + 1;
  int playerBottom = (playerY + PLAYER_SIZE) - 1;
  
  // upcoming player position
  int newPlayerLeft = (playerLeft - 1);
  int newPlayerRight = playerRight + 1;
  int newPlayerTop = (playerTop - 1);
  int newPlayerBottom = (playerBottom + 1);

  // move player up and check collision with tile it's about to arrive at
  if (arduboy.pressed(UP_BUTTON) && (playerTop > 0)) {
  	if (!isPointInTile(playerLeft, newPlayerTop) && !isPointInTile(playerRight, newPlayerTop)) {
  		mapy += 1;
  	}
  }
  if (arduboy.pressed(DOWN_BUTTON) && (playerBottom < (WORLD_HEIGHT * TILE_SIZE))) {
    if (!isPointInTile(playerLeft, newPlayerBottom) && !isPointInTile(playerRight, newPlayerBottom)) {
      mapy -= 1;
    }
  }
  if (arduboy.pressed(LEFT_BUTTON) && (playerLeft > 0)) {
    if (!isPointInTile(newPlayerLeft, playerTop)  && !isPointInTile(newPlayerLeft, playerBottom)) {
      mapx += 1;
    }
  }
  if (arduboy.pressed(RIGHT_BUTTON) && (playerRight < (WORLD_WIDTH * TILE_SIZE))) {
    if (!isPointInTile(newPlayerRight, playerTop) && !isPointInTile(newPlayerRight, playerBottom)) {
      mapx -= 1;
    }
  }
  
	arduboy.fillRect(0, 0, 48, 8, BLACK);
	arduboy.setCursor(0, 0);
	
	// PLayer coordinates
	arduboy.print(playerX);
	arduboy.print(',');
	arduboy.print(playerTop);
	arduboy.print('\n');
	
	arduboy.print(world[newPlayerTop / 16][playerRight / 16]);
  
}

void drawPlayer() {
  arduboy.fillRect(PLAYER_X_OFFSET, PLAYER_Y_OFFSET, PLAYER_SIZE, PLAYER_SIZE, BLACK);
}

void drawWorld() {
	const int tileswide = WIDTH / TILE_SIZE + 1;
	const int tilestall = HEIGHT / TILE_SIZE + 1;

	for (int y = 0; y < tilestall; y++)
	{
		for (int x = 0; x < tileswide; x++)
		{
			const int tilex = x - mapx / TILE_SIZE;
			const int tiley = y - mapy / TILE_SIZE;
			if (tilex >= 0 && tiley >= 0 && tilex < WORLD_WIDTH && tiley < WORLD_HEIGHT)
			{
				Sprites::drawOverwrite(x * TILE_SIZE + mapx % TILE_SIZE, y * TILE_SIZE + mapy % TILE_SIZE, tiles, world[tiley][tilex]);
			}
		}
	}
}

void setup() {
  
  arduboy.begin();
  arduboy.setFrameRate(45);
  arduboy.initRandomSeed();
  
}

void loop() {

  if(!(arduboy.nextFrame())) {
    return;
  }

  arduboy.pollButtons();
  arduboy.clear();
  ((FunctionPointer) pgm_read_word (&mainGameLoop[gamestate]))();
  arduboy.display();
}
