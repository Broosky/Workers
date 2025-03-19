#ifndef _MAIN_H_
#define _MAIN_H_

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#define PROG_NAME "Workers (C++)"
#define ICO_MAIN 100
#define DLG_MAIN 200

#define LOOPS 5

class ITEM {
    public:
        ITEM(void);
        ~ITEM(void);
        bool bInMotion;
        double fXFinal;
        double fYFinal;
        double fXPos;
        double fYPos;
        double fWid;
        double fHgt;
        int iMineralAmount;
        int Dead;
        double fUnitVectorY;
        double fUnitVectorX;
        COLORREF Colour, originalColour;
        void Draw(void);
        void MoveTo(double, double);
        void MoveTo(ITEM);
        void UpdatePosition(void);
};

void Gather(ITEM, ITEM, ITEM);

HINSTANCE hInst;
HWND hWnd;
__int64 iTickLast;
__int64 iFreq;

char szBuffer[200];
int iMineralCount = 0;

ITEM MineralNode[LOOPS];
ITEM CommandCenter;
ITEM Worker[LOOPS];

#endif
