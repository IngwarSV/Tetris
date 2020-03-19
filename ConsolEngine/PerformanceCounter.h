// Copyright 2009-2014 Blam Games, Inc. All Rights Reserved.

#include <windows.h>

class CStopwatch
{
public:
	CStopwatch();
	void Start();
	int Now();
	int Time();
private:
	//typedef union _LARGE_INTEGER. LARGE_INTEGER is a union of a 64-bit integer and a pair of 32-bit integers. 
	////If you want to perform 64-bit arithmetic on one you need to select the 64-bit int from inside the union.
	LARGE_INTEGER m_liPerfFreq;
	LARGE_INTEGER m_liPerfStart;
};

#ifdef MY_PERFORMENCE_COUNTER

CStopwatch::CStopwatch()
{
	//Функция QueryPerformanceFrequency извлекает частоту высокоточного счетчика высокого разрешения, если он существует.
	//&m_liPerfFreq - Указатель на переменную , которая получает текущую частоту высокоточного счетчика, в импульсах за секунду
    QueryPerformanceFrequency(&m_liPerfFreq);
    Start();
}

void CStopwatch::Start()
{
	////Функция QueryPerformanceCounter извлекает текущее значение счетчика высокого разрешения.
	////&m_liPerfStart - Указатель на переменную, которая получает текущее значение точного счетчика , в тиках (импульсах).
    QueryPerformanceCounter(&m_liPerfStart);
}

// возвращает число миллисекунд после вызова Start
int CStopwatch::Now()
{
        LARGE_INTEGER liPerfNow;
        QueryPerformanceCounter(&liPerfNow);
	return (((liPerfNow.QuadPart - m_liPerfStart.QuadPart) * 1000) / m_liPerfFreq.QuadPart);
	//(ticks - ticks)*1000/(ticks/sec)
}

int CStopwatch::Time()
{
    LARGE_INTEGER liPerfNow;
	QueryPerformanceCounter(&liPerfNow);
	return ((liPerfNow.QuadPart * 1000) / m_liPerfFreq.QuadPart);
	//(ticks * 1000)/(ticks/sec)
}

#endif
