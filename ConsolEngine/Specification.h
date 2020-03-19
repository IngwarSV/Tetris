#pragma once

struct Point {
	int x, y;
};

namespace DEF_SETT {
	// Amount of pieces in single Tetromino	
	const int PIECES = 4;

	// All 4 pieces have coordinates in 4x4 matrix
	const int DIMENSION = 4;

	// For console settings
	const int OUTPUT_SIZE_X = 50;
	const int OUTPUT_SIZE_Y = 30;

	// TotalGameField size
	const int GAME_FIELD_WIDTH = 25;
	const int GAME_FIELD_HEIGHT = 26;

	// PlayField size
	const int PLAY_FIELD_WIDTH = 17;
	const int PLAY_FIELD_HEIGHT = 22;

	// Initial Tetromino's position on PlayField
	const Point INIT_BLOCK_POS { 7,0 }; 

	// Initial Tetromino's position on PreviewField
	const Point INIT_PREVIEW_BLOCK_POS { PLAY_FIELD_WIDTH + 2, 2 }; 

	//Tetrominoes types
	const int BLOCKS_AMOUNT = 7;		

	// Initial Tetromino's descending speed, one position every ~0.5 seconds
	const float INIT_SPEED = 0.5f;		

	// for speed incrementing 
	const float SPEED_DELTA = 0.1f;	

	// blocks to initiate incrementing
	const int BLOCKS_PER_ROUND = 10;	
}

















