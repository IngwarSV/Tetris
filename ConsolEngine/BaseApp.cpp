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

	//Функция SetConsoleScreenBufferSize изменяет размер заданного экранного буфера консоли.
	if(!SetConsoleScreenBufferSize(mConsole,  windowBufSize))
	{
		cout << "SetConsoleScreenBufferSize failed with error " << GetLastError() << endl;
	}

	//Функция SetConsoleWindowInfo устанавливает текущий размер и позицию окна экранного буфера консоли.
	if(!SetConsoleWindowInfo(mConsole, TRUE, &windowSize))
	//Если этот параметр - ИСТИНА (TRUE), координаты устанавливают новые левый верхний и нижний правый углы окна. Если он - ЛОЖЬ (FALSE), координаты являются смещениями текущих координат угла окна
	//Указатель на структуру SMALL_RECT, которая устанавливает новые левый верхний и нижний правый углы окна
	{
		cout << "SetConsoleWindowInfo failed with error " << GetLastError() << endl;
	}

	//Структура CONSOLE_CURSOR_INFO содержит информацию о курсоре консоли? its size and visibility
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(mConsole, &cursorInfo);
	cursorInfo.bVisible = FALSE;
	cursorInfo.dwSize = 1;
	//Функция SetConsoleCursorInfo устанавливает размер и вид курсора для заданного экранного буфера консоли.
	SetConsoleCursorInfo(mConsole, &cursorInfo);


	mChiBuffer = (CHAR_INFO*)malloc((X_SIZE+1)*(Y_SIZE+1)*sizeof(CHAR_INFO));

	mDwBufferSize.X = X_SIZE + 1;
	mDwBufferSize.Y = Y_SIZE + 1;		// размер буфера данных

	mDwBufferCoord.X = 0;
	mDwBufferCoord.Y = 0;				// координаты ячейки

	mLpWriteRegion.Left = 0;
	mLpWriteRegion.Top = 0;
	mLpWriteRegion.Right = X_SIZE + 1;
	mLpWriteRegion.Bottom = Y_SIZE + 1;	// прямоугольник для чтения


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
	//Функция WriteConsoleOutput записывает символ и данные атрибута цвета в заданном прямоугольном блоке символьных знакомест в экранном буфере консоли. 
	//Данные, которые будут записаны, берутся из прямоугольного блока соответствующего размера в заданном месте в исходном буфере.
	if (!WriteConsoleOutput(mConsole, mChiBuffer, mDwBufferSize, mDwBufferCoord, &mLpWriteRegion)) 
	{
		printf("WriteConsoleOutput failed - (%d)\n", GetLastError()); //Функция GetLastError извлекает значение кода последней ошибки вызывающего потока
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
		timer.Start();//текущее значение точного счетчика
		////Функция kbhit() возвращает истину, если нажата какая-либо клавиша на клавиатуре. 
		////В против­ном случае возвращается 0. В любом случае код клавиши не удаляется из входного буфера.
		if (kbhit())
		{
			////Функция getch() возвращает очередной символ, считанный с консоли, 
			////но не выводит этот сим­вол на экран.
			KeyPressed (getch());
			////Функция FlushConsoleInputBuffer выключает из работы консольный буфер ввода. 
			////Все записи вводимых данных теперь в буфере ввода сбрасываются.
			if (!FlushConsoleInputBuffer(mConsoleIn))
				cout<<"FlushConsoleInputBuffer failed with error "<<GetLastError();
		}
		/*эта функция вызывается каждую игровую итерацию, её можно переопределить, в наследнике класса.
		в неё приходит deltaTime - разница во времени между предыдущей итерацией и этой, в секундах*/
		UpdateF((float)deltaTime / 1000.0f);
		Render();
		////Приостанавливает выполнение программы на время, задаваемое параметром time. 
		////Время задается в миллисекундах.
		Sleep(1);

		while (1)
		{
			////возвращает число миллисекунд после вызова Start
			deltaTime = timer.Now();
			if (deltaTime > 20)
				break;
		}

		sum += deltaTime;
		counter++;
		if (sum >= 1000)
		{
			TCHAR  szbuff[255];
			////Функция StringCchPrintf - это замена функции sprintf. 
			////Она принимает форматирующую строку и список параметров и возвращает отформатированную строку. 
			////Размер, в символах, целевого буфера назначается в функцию для того, чтобы гарантировать, что StringCchPrintf не запишет помимо конца этого буфера.
			////3-й параметр - указатель на буфер, содержащий форматирующая строку printf-стиля. Эта строка должна быть завершена символом конца строки ('\0').
			StringCchPrintf(szbuff, 255, TEXT("FPS: %d"), counter);
			SetConsoleTitle(szbuff);

			counter = 0;
			sum = 0;
		}
	}
}