//DinoSmasher

#include "bitmaps.h"

#include <Arduboy2.h>
Arduboy2 arduboy;

// game states
#define GAME_TITLE    0
#define GAME_PLAY     1
#define GAME_OVER     2
#define GAME_HIGH     3

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

// set up variables
int gamestate = GAME_TITLE;

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
	    return false;
	    break;
	    
    case WATER:
      return false;
      break;
      
    case TREES:
      return true;
      break;
      
    case STONE:
      return true;
      break;
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
  
  int playerLeft = (playerX);
  int playerRight = (playerX + PLAYER_SIZE);
  int playerTop = (playerY);
  int playerBottom = (playerY + PLAYER_SIZE);

  // move player up and check collision with tile it's about to arrive at
  if (arduboy.pressed(UP_BUTTON) && (playerTop > 0)) {
  	int newPlayerTop = (playerTop - 1);
  	if (!isPointInTile(playerLeft, newPlayerTop) && !isPointInTile(playerRight, newPlayerTop))
  	{
  		mapy += 1;
  	}
  }
  if (arduboy.pressed(DOWN_BUTTON) && (playerBottom < (WORLD_HEIGHT * TILE_SIZE))) {
    int newPlayerBottom = (playerBottom + 1);
    if (!isPointInTile(playerLeft, newPlayerBottom) && !isPointInTile(playerRight, newPlayerBottom)) {
      mapy -= 1;
    }
  }
  if (arduboy.pressed(LEFT_BUTTON) && (playerLeft > 0)) {
    int newPlayerLeft = (playerLeft - 1);
    if (!isPointInTile(newPlayerLeft, playerTop)  && !isPointInTile(newPlayerLeft, playerBottom)) {
      mapx += 1;
    }
  }
  if (arduboy.pressed(RIGHT_BUTTON) && (playerRight < (WORLD_WIDTH * TILE_SIZE))) {
    int newPlayerRight = playerRight + 1;
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
	
	arduboy.print(world[playerBottom / 16][playerRight / 16]);
  
}

void drawPlayer() {
  arduboy.fillRect(PLAYER_X_OFFSET, PLAYER_Y_OFFSET, PLAYER_SIZE, PLAYER_SIZE, BLACK);
}

// void drawWorld() {
//   for (int y = 0; y < tilestall; y++) {
//     for (int x = 0; x < tileswide; x++) {
//       const int tilex = x - mapx / TILE_SIZE;
//       const int tiley = y - mapy / TILE_SIZE;
//       if (tilex >= 0 && tiley >= 0 && tilex < WORLD_WIDTH && tiley < WORLD_HEIGHT) {
//         Sprites::drawOverwrite(x * TILE_SIZE + mapx % TILE_SIZE, y * TILE_SIZE + mapy % TILE_SIZE, tiles, world[tiley][tilex]);
//       }
//     }
//   }

//   arduboy.fillRect(0, 0, 48, 16, BLACK);
//   arduboy.setCursor(0, 0);
//   arduboy.print(0 - mapx);
//   arduboy.print(",");
//   arduboy.print(0 - mapy);
//   arduboy.print("\n");
//   arduboy.print(0 - mapx / TILE_SIZE);
//   arduboy.print(",");
//   arduboy.print(0 - mapy / TILE_SIZE);
// }

void drawWorld()
{
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

// 	arduboy.fillRect(0, 0, 48, 8, BLACK);
// 	arduboy.setCursor(0, 0);
	
// 	// Map coordinates
// 	arduboy.print(mapx);
// 	arduboy.print(',');
// 	arduboy.print(mapy);
// 	arduboy.print('\n');
		
// 	// Map coordinate remainder
// 	arduboy.print(mapx % TILE_SIZE);
// 	arduboy.print(',');
// 	arduboy.print(mapy % TILE_SIZE);
// 	arduboy.print('\n');
	
// 	// Render coordinate without modulo
// 	arduboy.print(1 * TILE_SIZE + mapx);
// 	arduboy.print(',');
// 	arduboy.print(1 * TILE_SIZE + mapy);
// 	arduboy.print('\n');
		
// 	// Render coordinate with modulo
// 	arduboy.print(1 * TILE_SIZE + mapx % TILE_SIZE);
// 	arduboy.print(',');
// 	arduboy.print(1 * TILE_SIZE + mapy % TILE_SIZE);
// 	arduboy.print('\n');
}

void titleScreen() {
  arduboy.setCursor(0, 0);
  arduboy.print("Title Screen\n");
  
  if (arduboy.justPressed(A_BUTTON)) {
    gamestate = GAME_PLAY;
  }
}

void gamePlay() {
  
  drawWorld();
  drawPlayer();
  playerInput();
  
  if (arduboy.justPressed(A_BUTTON)) {
    gamestate = GAME_OVER;
  }
}

void gameOverScreen() {
  arduboy.setCursor(0, 0);
  arduboy.print("Game Over\n");
  if (arduboy.justPressed(A_BUTTON)) {
    gamestate = GAME_HIGH;
  }
}

void highScoreScreen() {
  arduboy.setCursor(0, 0);
  arduboy.print("High Score\n");
  if (arduboy.justPressed(A_BUTTON)) {
    gamestate = GAME_TITLE;
  }
}

void gameloop() {

  switch(gamestate) {
  
    case GAME_TITLE:
      titleScreen();
      break;

    case GAME_PLAY:
      gamePlay();
      break;

    case GAME_OVER:
      gameOverScreen();
      break;

    case GAME_HIGH:
      highScoreScreen();
      break;
  }
  
}

void setup() {
  
  arduboy.begin();
  arduboy.setFrameRate(45);
  arduboy.display();
  arduboy.initRandomSeed();
  arduboy.clear();
  
}

void loop() {

  if(!(arduboy.nextFrame())) {
    return;
  }

  arduboy.pollButtons();
  arduboy.clear();
  gameloop();
  arduboy.display();
}
