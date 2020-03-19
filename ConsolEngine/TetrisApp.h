#pragma once

#include "BaseApp.h"
#include "Specification.h"

using namespace DEF_SETT;

class TetrisApp : public BaseApp {
	typedef BaseApp Parent;

private:
	int mTetrominoes[BLOCKS_AMOUNT][PIECES];
	int mCurrentBlockNum;
	int mNextBlockNum;
	Point mCurrentFieldPos;
	int mCurrentRotation;
	Point mCurrentBlockCoords[4];
	Point mTempBlockCoords[4];
	int mScore;
	bool mButDownPressed;

	void SetCurrentBlockCoords(Point blockCoords[], int blockNum);
	void SetTotalGameField();
	void SetPreviewField();
	void SetScore(int linesAmount = 0);
		
	void RewriteCurrentBlock(Point blockCoords[], wchar_t symbol);
	void Rotate(Point blockCoords[], int rotation);
	bool MoveIsAllowed(int fieldX, int fieldY, Point blockCoords[]);
	

public:
	TetrisApp();

	virtual void KeyPressed(int btnCode) override;
	virtual void UpdateF(float deltaTime) override;
};
