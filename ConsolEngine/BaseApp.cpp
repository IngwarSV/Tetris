// Copyright 2009-2014 Blam Games, Inc. All Rights Reserved.

#include "BaseApp.h"

#define MY_PERFORMENCE_COUNTER

#include "PerformanceCounter.h"

BaseApp::BaseApp(int xSize, int ySize) : X_SIZE(xSize), Y_SIZE(ySize)
{
	SMALL_RECT windowSize = {0, 0, X_SIZE, Y_SIZE};
	COORD windowBufSize = {X_SIZE+1, Y_SIZE+1};

	mConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	mConsoleIn = GetStdHandle(STD_INPUT_HANDLE);

	//������� SetConsoleScreenBufferSize �������� ������ ��������� ��������� ������ �������.
	if(!SetConsoleScreenBufferSize(mConsole,  windowBufSize))
	{
		cout << "SetConsoleScreenBufferSize failed with error " << GetLastError() << endl;
	}

	//������� SetConsoleWindowInfo ������������� ������� ������ � ������� ���� ��������� ������ �������.
	if(!SetConsoleWindowInfo(mConsole, TRUE, &windowSize))
	//���� ���� �������� - ������ (TRUE), ���������� ������������� ����� ����� ������� � ������ ������ ���� ����. ���� �� - ���� (FALSE), ���������� �������� ���������� ������� ��������� ���� ����
	//��������� �� ��������� SMALL_RECT, ������� ������������� ����� ����� ������� � ������ ������ ���� ����
	{
		cout << "SetConsoleWindowInfo failed with error " << GetLastError() << endl;
	}

	//��������� CONSOLE_CURSOR_INFO �������� ���������� � ������� �������? its size and visibility
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(mConsole, &cursorInfo);
	cursorInfo.bVisible = FALSE;
	cursorInfo.dwSize = 1;
	//������� SetConsoleCursorInfo ������������� ������ � ��� ������� ��� ��������� ��������� ������ �������.
	SetConsoleCursorInfo(mConsole, &cursorInfo);


	mChiBuffer = (CHAR_INFO*)malloc((X_SIZE+1)*(Y_SIZE+1)*sizeof(CHAR_INFO));

	mDwBufferSize.X = X_SIZE + 1;
	mDwBufferSize.Y = Y_SIZE + 1;		// ������ ������ ������

	mDwBufferCoord.X = 0;
	mDwBufferCoord.Y = 0;				// ���������� ������

	mLpWriteRegion.Left = 0;
	mLpWriteRegion.Top = 0;
	mLpWriteRegion.Right = X_SIZE + 1;
	mLpWriteRegion.Bottom = Y_SIZE + 1;	// ������������� ��� ������


	for (int x=0; x<X_SIZE+1; x++)
	{
		for (int y=0; y<Y_SIZE+1; y++)
		{
			SetChar(x, y, L' ');
		}
	}
}

BaseApp::~BaseApp()
{
	free(mChiBuffer);
}

void BaseApp::SetChar(int x, int y, wchar_t c)
{
	mChiBuffer[x + (X_SIZE+1)*y].Char.UnicodeChar = c;
	mChiBuffer[x + (X_SIZE+1)*y].Attributes = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED;

}

wchar_t BaseApp::GetChar(int x, int y)
{
	return mChiBuffer[x + (X_SIZE+1)*y].Char.AsciiChar;
}

void BaseApp::Render()
{
	//������� WriteConsoleOutput ���������� ������ � ������ �������� ����� � �������� ������������� ����� ���������� ��������� � �������� ������ �������. 
	//������, ������� ����� ��������, ������� �� �������������� ����� ���������������� ������� � �������� ����� � �������� ������.
	if (!WriteConsoleOutput(mConsole, mChiBuffer, mDwBufferSize, mDwBufferCoord, &mLpWriteRegion)) 
	{
		printf("WriteConsoleOutput failed - (%d)\n", GetLastError()); //������� GetLastError ��������� �������� ���� ��������� ������ ����������� ������
	}
}

void BaseApp::Run()
{
	CStopwatch timer;
	int sum = 0;
	int counter = 0;

	int deltaTime = 0;
	while (1)
	{
		timer.Start();//������� �������� ������� ��������
		////������� kbhit() ���������� ������, ���� ������ �����-���� ������� �� ����������. 
		////� ��������� ������ ������������ 0. � ����� ������ ��� ������� �� ��������� �� �������� ������.
		if (kbhit())
		{
			////������� getch() ���������� ��������� ������, ��������� � �������, 
			////�� �� ������� ���� ������ �� �����.
			KeyPressed (getch());
			////������� FlushConsoleInputBuffer ��������� �� ������ ���������� ����� �����. 
			////��� ������ �������� ������ ������ � ������ ����� ������������.
			if (!FlushConsoleInputBuffer(mConsoleIn))
				cout<<"FlushConsoleInputBuffer failed with error "<<GetLastError();
		}
		/*��� ������� ���������� ������ ������� ��������, � ����� ��������������, � ���������� ������.
		� �� �������� deltaTime - ������� �� ������� ����� ���������� ��������� � ����, � ��������*/
		UpdateF((float)deltaTime / 1000.0f);
		Render();
		////���������������� ���������� ��������� �� �����, ���������� ���������� time. 
		////����� �������� � �������������.
		Sleep(1);

		while (1)
		{
			////���������� ����� ����������� ����� ������ Start
			deltaTime = timer.Now();
			if (deltaTime > 20)
				break;
		}

		sum += deltaTime;
		counter++;
		if (sum >= 1000)
		{
			TCHAR  szbuff[255];
			////������� StringCchPrintf - ��� ������ ������� sprintf. 
			////��� ��������� ������������� ������ � ������ ���������� � ���������� ����������������� ������. 
			////������, � ��������, �������� ������ ����������� � ������� ��� ����, ����� �������������, ��� StringCchPrintf �� ������� ������ ����� ����� ������.
			////3-� �������� - ��������� �� �����, ���������� ������������� ������ printf-�����. ��� ������ ������ ���� ��������� �������� ����� ������ ('\0').
			StringCchPrintf(szbuff, 255, TEXT("FPS: %d"), counter);
			SetConsoleTitle(szbuff);

			counter = 0;
			sum = 0;
		}
	}
}