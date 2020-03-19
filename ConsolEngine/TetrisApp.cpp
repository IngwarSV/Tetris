#include "TetrisApp.h"

void TetrisApp::SetCurrentBlockCoords(Point blockCoords[], int blockNum) {
	for (int piece = 0; piece < PIECES; ++piece) {
		blockCoords[piece].x = mTetrominoes[blockNum][piece] % DIMENSION;
		blockCoords[piece].y = mTetrominoes[blockNum][piece] / DIMENSION;
	}
}

void TetrisApp::SetTotalGameField() {
	for (int x = 0; x < GAME_FIELD_WIDTH; ++x) {
		for (int y = 0; y < GAME_FIELD_HEIGHT; ++y) {
			if (x == 0 || x == GAME_FIELD_WIDTH - 1 || y == 0 || y == GAME_FIELD_HEIGHT - 1 || y == PLAY_FIELD_HEIGHT - 1) {
				SetChar(x, y, L'#');
			}
		}
	}
	for (int y = 1; y < PLAY_FIELD_HEIGHT - 1; ++y) {
		SetChar(PLAY_FIELD_WIDTH - 1, y, L'#');
	}
	for (int x = 1; x < PLAY_FIELD_WIDTH - 1; ++x) {
		for (int y = 1; y < PLAY_FIELD_HEIGHT - 1; ++y) {
			SetChar(x, y, L'.');
		}
	}
}

void TetrisApp::SetPreviewField() {
	// Erasing previous block
	for (int piece = 0; piece < PIECES; ++piece) {
		SetChar(mCurrentBlockCoords[piece].x + INIT_PREVIEW_BLOCK_POS.x,
			mCurrentBlockCoords[piece].y + INIT_PREVIEW_BLOCK_POS.y, L' ');
	}

	// Setting next block
	for (int piece = 0; piece < PIECES; ++piece) {
		SetChar(mTempBlockCoords[piece].x + INIT_PREVIEW_BLOCK_POS.x,
			mTempBlockCoords[piece].y + INIT_PREVIEW_BLOCK_POS.y, L'X');
	}
}

void TetrisApp::SetScore(int linesAmount) {
	wstring temp_score = L"   > Score: ";
	
	switch (linesAmount) {
	case 1:
		mScore += 100;
		break;
	case 2:
		mScore += 300;
		break;
	case 3:
		mScore += 500;
		break;
	case 4:
		mScore += 800;
		break;
	}

	temp_score += to_wstring(mScore);

	for (size_t i = 1; i < temp_score.length(); ++i) {
		SetChar(i, PLAY_FIELD_HEIGHT + 1, temp_score[i]);
	}
}

void TetrisApp::RewriteCurrentBlock(Point blockCoords[], wchar_t symbol) {
	for (int piece = 0; piece < PIECES; ++piece) {
		SetChar(blockCoords[piece].x + mCurrentFieldPos.x, blockCoords[piece].y + mCurrentFieldPos.y, symbol);
	}
}

void TetrisApp::Rotate(Point blockCoords[], int rotation) {
	switch (rotation % 4) {
	case 0: // 0 degrees												
			// index = DIMENSION * blockY + blockX;		
		for (int piece = 0; piece < PIECES; ++piece) {
			int index = 4 * blockCoords[piece].y + blockCoords[piece].x;

			blockCoords[piece].x = index % 4;
			blockCoords[piece].y = index / 4;
		}
		break;

	case 1: // 90 degrees												
			// index = DIMENSION * (DIMENSION - 1) + blockY - (blockX * DIMENSION);		
		for (int piece = 0; piece < PIECES; ++piece) {
			int index = 12 + blockCoords[piece].y - 4 * blockCoords[piece].x;

			blockCoords[piece].x = index % 4;
			blockCoords[piece].y = index / 4;
		}
		break;

	case 2: // 180 degrees, including x - 1 to reduce spinning			
			// index = (DIMENSION * DIMENSION - 1) - (blockY * DIMENSION) - blockX - 1;
		for (int piece = 0; piece < PIECES; ++piece) {
			int index = 15 - 4 * blockCoords[piece].y - blockCoords[piece].x - 1;

			blockCoords[piece].x = index % 4;
			blockCoords[piece].y = index / 4;
		}
		break;

	case 3: // 270 degrees, including y + 1 to reduce spinning			
			// index = (DIMENSION - 1) - blockY + (blockX * DIMENSION) + 4;		
		for (int piece = 0; piece < PIECES; ++piece) {
			int index = 3 - blockCoords[piece].y + 4 * blockCoords[piece].x + 4;

			blockCoords[piece].x = index % 4;
			blockCoords[piece].y = index / 4;
		}
		break;
	}
}

bool TetrisApp::MoveIsAllowed(int fieldX, int fieldY, Point blockCoords[]) {
	RewriteCurrentBlock(mCurrentBlockCoords, L'.');
	
	for (int piece = 0; piece < PIECES; ++piece) {

		// Getting piece coordinates on PlayField
		int blockX = blockCoords[piece].x + fieldX;
		int blockY = blockCoords[piece].y + fieldY;

		// Checking in boundaries
		if (blockX >= 0 && blockX < PLAY_FIELD_WIDTH) {
			if (blockY >= 0 && blockY < PLAY_FIELD_HEIGHT) {

				// Collision check
				if (GetChar(blockX, blockY) != L'.') {
					return false;
				}
			}
		}
	}
		
	return true;
}


TetrisApp::TetrisApp() : Parent(OUTPUT_SIZE_X, OUTPUT_SIZE_Y), mTetrominoes { 
	{4,5,6,7},		// I
	{5,8,9,10},		// T
	{5,6,9,10},		// O
	{6,8,9,10},		// L
	{4,5,9,10},		// Z
	{4,8,9,10},		// J
	{5,6,8,9} }		// S
{
	srand(static_cast<size_t>(time(NULL)));

	this->mCurrentBlockNum = rand() % BLOCKS_AMOUNT;
	this->mNextBlockNum = rand() % BLOCKS_AMOUNT;
	this->mCurrentFieldPos = INIT_BLOCK_POS;
	this->mCurrentRotation = 0;
	this->mScore = 0;
	this->mButDownPressed = false;
	
	// Setting current block coordinates
	SetCurrentBlockCoords(mCurrentBlockCoords, mCurrentBlockNum);

	// Setting next block coordinates
	SetCurrentBlockCoords(mTempBlockCoords, mNextBlockNum);

	// Setting totalGameField
	SetTotalGameField();

	// Setting previewField
	SetPreviewField();

	// Setting score
	SetScore();
		
	// Setting console font
	CONSOLE_FONT_INFOEX cfi;
		cfi.cbSize = sizeof(cfi);
		cfi.nFont = 0;
		cfi.dwFontSize.X = 8;
		cfi.dwFontSize.Y = 12;
		cfi.FontFamily = FF_DONTCARE;
		cfi.FontWeight = FW_NORMAL;
		wcscpy_s(cfi.FaceName, L"Terminal");
	if (!SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi)) {
			cout << "SetCurrentConsoleFontEx failed with error " << GetLastError() << endl;
	}
	
	// Setting console window size
	SMALL_RECT windowSize = { 0, 0, GAME_FIELD_WIDTH - 1, GAME_FIELD_HEIGHT - 1 };
	if (!SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &windowSize)) {
		cout << "SetConsoleWindowInfo failed with error " << GetLastError() << endl;
	}
}

void TetrisApp::KeyPressed(int btnCode) {
	if (!mButDownPressed) {
		if (btnCode == 0 || btnCode == 224) {
			switch (_getch()) {
			case 77: // Right arrow
				if (MoveIsAllowed(mCurrentFieldPos.x + 1, mCurrentFieldPos.y, mCurrentBlockCoords)) {
					mCurrentFieldPos.x += 1;
				}
				break;
			case 75: // Left arrow
				if (MoveIsAllowed(mCurrentFieldPos.x - 1, mCurrentFieldPos.y, mCurrentBlockCoords)) {
					mCurrentFieldPos.x -= 1;
				}
				break;
			case 80: // Down arrow
				if (MoveIsAllowed(mCurrentFieldPos.x, mCurrentFieldPos.y + 1, mCurrentBlockCoords)) {
					mButDownPressed = true;
				}
				break;
			}
		}
		else if (btnCode == 32) { // Space
			int temp_rotation = mCurrentRotation + 1;
			
			if (!(mCurrentBlockNum % 2)) { // for I, Z, S, O Tetrominoes
				temp_rotation %= 2;
			}

			SetCurrentBlockCoords(mTempBlockCoords, mCurrentBlockNum);

			Rotate(mTempBlockCoords, temp_rotation);
			
			if (MoveIsAllowed(mCurrentFieldPos.x, mCurrentFieldPos.y, mTempBlockCoords)) {
				mCurrentRotation = temp_rotation;
				for (int piece = 0; piece < PIECES; ++piece) {
					mCurrentBlockCoords[piece] = mTempBlockCoords[piece];
				}
			}
		}
	}
}

void TetrisApp::UpdateF(float deltaTime) {
	static float s_GameTick = 0.0;
	static float s_GameSpeed = INIT_SPEED;
	static int s_BlocksCounter = 1;

	if (mButDownPressed) {
		s_GameTick += deltaTime * 15; // if button DOWN is pressed, increasing descending speed
	}
	else {
		s_GameTick += deltaTime;
	}
	
	if (s_GameTick > s_GameSpeed) {
		// Descending current block
		if (MoveIsAllowed(mCurrentFieldPos.x, mCurrentFieldPos.y + 1, mCurrentBlockCoords)) {
			mCurrentFieldPos.y += 1;
		}
		else {
			// Landing current block
			for (int piece = 0; piece < PIECES; ++piece) {
				SetChar(mCurrentBlockCoords[piece].x + mCurrentFieldPos.x, mCurrentBlockCoords[piece].y + mCurrentFieldPos.y, L'X');
			}
					
			// Looking for lines (four rows deep)
			std::vector<int> vFilledLines;
			for (int fieldY = mCurrentFieldPos.y; fieldY < (mCurrentFieldPos.y + 4) && fieldY < PLAY_FIELD_HEIGHT - 1; ++fieldY) {
				bool isLine = true;
				for (int fieldX = 1; fieldX < PLAY_FIELD_WIDTH - 1; ++fieldX) {
					if (GetChar(fieldX, fieldY) != L'X') {
						isLine = false;
						break;
					}
				}
				if (isLine) {
					vFilledLines.push_back(fieldY);
				}
			}

			if (!vFilledLines.empty()) {
				// Removing lines
				for (int var : vFilledLines) {
					for (int fieldX = 1; fieldX < PLAY_FIELD_WIDTH - 1; ++fieldX) {
						for (int fieldY = var; fieldY > 1; --fieldY) {
							SetChar(fieldX, fieldY, GetChar(fieldX, fieldY - 1));
						}
					}
				}

				// Updating score
				SetScore(vFilledLines.size());
			}

			mCurrentBlockNum = mNextBlockNum;
			mNextBlockNum = rand() % BLOCKS_AMOUNT;
			mCurrentFieldPos = INIT_BLOCK_POS;
			mCurrentRotation = 0;
			mButDownPressed = false;

			// Setting current block coordinates
			SetCurrentBlockCoords(mCurrentBlockCoords, mCurrentBlockNum);

			// Setting next block coordinates
			SetCurrentBlockCoords(mTempBlockCoords, mNextBlockNum);

			// Setting new previewField
			SetPreviewField();

			// Cheking for Game Over
			for (int piece = 0; piece < PIECES; ++piece) {
				if (GetChar(INIT_BLOCK_POS.x + mCurrentBlockCoords[piece].x, INIT_BLOCK_POS.y + mCurrentBlockCoords[piece].y) == L'X') {
					exit(0);
				}
			}

			// Increasing speed every BLOCKS_PER_ROUND
			s_BlocksCounter += 1;
			if (!(s_BlocksCounter % BLOCKS_PER_ROUND)) {
				s_GameSpeed -= SPEED_DELTA;
				s_BlocksCounter = 1;
			}
		}
		s_GameTick = 0.0f;
	}

	// Setting current block on PlayField
	RewriteCurrentBlock(mCurrentBlockCoords, L'X');
}
