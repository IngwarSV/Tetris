// Copyright 2009-2014 Blam Games, Inc. All Rights Reserved.

#pragma once

#include <iostream>
#include <Windows.h>
#include <time.h>
#include <vector>
#include <string>
#include <conio.h>
#include <strsafe.h>

using namespace std;

class BaseApp
{
private:
	HANDLE mConsole;
	HANDLE mConsoleIn;

	//Структура CHAR_INFO устанавливает символ Unicode или ANSI и его атрибуты. 
	//Эта структура используется консольными функциями, чтобы читать из и записывать в экранный буфер консоли. Атрибуты символа. 
	//Этот член может быть нулевой или любой комбинацией из ниже перечисленных атрибутов. 
	CHAR_INFO* mChiBuffer;
	COORD mDwBufferSize;
	COORD mDwBufferCoord;

	//Структура SMALL_RECT определяет координаты верхнего левого и нижнего правого углов прямоугольника.
	SMALL_RECT mLpWriteRegion;

	void Render();

public:
	//размеры области вывода по горизонтали и вертикали в символах
	const int X_SIZE;
	const int Y_SIZE;

	//аргументами являются размеры области вывода по горизонтали и вертикали в символах
	BaseApp(int xSize=100, int ySize=80);
	virtual ~BaseApp();

	//запускает игровой цикл
	void Run();

	//можно заполнить x,y-символ экрана определенным символом, или считать его
	void SetChar(int x, int y, wchar_t c);
	wchar_t GetChar(int x, int y);

	/*эта функция вызывается каждую игровую итерацию, её можно переопределить, в наследнике класса.
	в неё приходит deltaTime - разница во времени между предыдущей итерацией и этой, в секундах*/
	virtual void UpdateF (float deltaTime){}
	/*эта функция вызывается при нажатии клавиши на клавиатуре, в неё приходит код клавиши - btnCode.
	если использовать стрелки или функциональные клавиши, то придет общее для них число, например 224, а следующее за ним - 
	будет уже непосредственно код самой клавиши, его можно получить, вызвав метод getch().
	Метод KeyPressed так же можно переопределить в наследнике*/
	virtual void KeyPressed (int btnCode){}
};