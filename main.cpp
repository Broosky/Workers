#ifndef _MAIN_CPP_
#define _MAIN_CPP_

#include "main.h"

#include <stdio.h>
#include <complex>
#include <math.h>

ITEM::ITEM(void) {
    return;
}

ITEM::~ITEM(void) {
    return;
}

void ITEM::Draw(void) {
    HDC hDC = GetDC(hWnd);
    HBRUSH hBrush = CreateSolidBrush(Colour);
    HGDIOBJ hTemp = SelectObject(hDC, hBrush);
    Rectangle(hDC, (int)fXPos, (int)fYPos, (int)fXPos + (int)fWid, (int)fYPos + (int)fHgt);
    SelectObject(hDC, hTemp);
    DeleteObject(hBrush);
    ReleaseDC(hWnd, hDC);
}   

void ClearScreen(void) {
    HDC hDC = GetDC(hWnd);
    HBRUSH hBrush = CreateSolidBrush(RGB(200, 200, 200));
    HGDIOBJ hTemp = SelectObject(hDC, hBrush);
    RECT t;
    GetClientRect(hWnd, &t);
    Rectangle(hDC, t.left, t.top, t.right, t.bottom);
    SelectObject(hDC, hTemp);
    DeleteObject(hBrush);
    ReleaseDC(hWnd, hDC);
}
    
void ITEM::MoveTo(double fXPos, double fYPos) {
    bInMotion = true;
    fXFinal = fXPos;
    fYFinal = fYPos;
    double fVectorX = fXPos - (*this).fXPos;
    double fVectorY = fYPos - (*this).fYPos;
    fUnitVectorX = fVectorX / sqrt(fVectorX * fVectorX + fVectorY * fVectorY);
    fUnitVectorY = fVectorY / sqrt(fVectorX * fVectorX + fVectorY * fVectorY); 
} 

void WhereToGo(ITEM & Worker, ITEM & MineralNode, ITEM CommandCenter, int i) {
    if(Worker.fXPos == MineralNode.fXPos && Worker.fYPos == MineralNode.fYPos) {
        MineralNode.iMineralAmount -= 5;
        if(MineralNode.iMineralAmount <= 100) {
            MineralNode.Colour = RGB(200, 200, 0);
        }
        if(MineralNode.iMineralAmount <= 50) {
            MineralNode.Colour = RGB(0, 200, 200);
        }
        if(MineralNode.iMineralAmount <= 0) {
            MineralNode.Colour = RGB(200, 200, 200);
        }
        printf("Mineral Node[%d]: %d\n", i, MineralNode.iMineralAmount);
        Worker.Colour = RGB(0, 0, 200);
        Worker.MoveTo(CommandCenter.fXPos, CommandCenter.fYPos);
    }
    else if(Worker.fXPos == CommandCenter.fXPos && Worker.fYPos == CommandCenter.fYPos) {
        Worker.Colour = Worker.originalColour;
        iMineralCount += 5;
        if(MineralNode.iMineralAmount <= 0) {
            return;
        }
        Worker.MoveTo(MineralNode.fXPos, MineralNode.fYPos);
    }
    else {
        Worker.Colour = Worker.originalColour;
        Worker.MoveTo(CommandCenter.fXPos, CommandCenter.fYPos);
    }
}

void ITEM::UpdatePosition(void) {
    fXPos += fUnitVectorX * 4.0;
    fYPos += fUnitVectorY * 4.0;     
    if(abs((int)fXPos - (int)fXFinal) <= 2.0 && abs((int)fYPos - (int)fYFinal) <= 2.0) {
        fXPos = fXFinal;
        fYPos = fYFinal;
        bInMotion = false;
    }
}

int TimeBase(__int64* iTickLast) {
    __int64 iTickNow;
    double fMSPerFrame = 1.0 / 40.0;
    double fDifference;
    QueryPerformanceCounter((LARGE_INTEGER*)&iTickNow);
    fDifference = (double)(iTickNow - (*iTickLast)) / (double)iFreq;
    if(fDifference >= fMSPerFrame) {
        (*iTickLast) = iTickNow;
        return 1;
    }
    return 0;
}

INT_PTR CALLBACK DlgMainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if(uMsg == WM_INITDIALOG) {
        HICON hIcon = LoadIcon(hInst, MAKEINTRESOURCE(ICO_MAIN));
        PostMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
        PostMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
        DestroyIcon(hIcon);
    }
    else if(uMsg == WM_KEYDOWN) {
        if(wParam == VK_ESCAPE) {
            PostMessage(hWnd, WM_CLOSE, 0, 0);
        }           
    }
    else if(uMsg == WM_CLOSE) {
        PostQuitMessage(0);
        EndDialog(hWnd, 0);
    }
    return 0;
}

int __cdecl RndNum(int iMin, int iMax) {
    return rand() % (iMax - iMin + 1) + iMin;
}

int main(int argc, char* argv[], char* envp[]) {
    MSG Msg;
    hInst = GetModuleHandle(NULL);
    
    hWnd = CreateDialog(hInst, MAKEINTRESOURCE(DLG_MAIN), NULL, DlgMainProc);
    QueryPerformanceFrequency((LARGE_INTEGER*)&iFreq);
    QueryPerformanceCounter((LARGE_INTEGER*)&iTickLast);
    
    srand(iTickLast);

    MineralNode[0].fXPos = 20;
    MineralNode[0].fYPos = 40;
    MineralNode[0].fWid = 20;
    MineralNode[0].fHgt = 20;
    MineralNode[0].Colour = RGB(0, 0, 200);
    MineralNode[0].iMineralAmount = 100; // Multiples of 5!
    
    MineralNode[1].fXPos = 200;
    MineralNode[1].fYPos = 20;
    MineralNode[1].fWid = 20;
    MineralNode[1].fHgt = 20;
    MineralNode[1].Colour = RGB(0, 0, 200);
    MineralNode[1].iMineralAmount = 100;
    
    MineralNode[2].fXPos = 30;
    MineralNode[2].fYPos = 300;
    MineralNode[2].fWid = 20;
    MineralNode[2].fHgt = 20;
    MineralNode[2].Colour = RGB(0, 0, 200);
    MineralNode[2].iMineralAmount = 100;
    
    MineralNode[3].fXPos = 400;
    MineralNode[3].fYPos = 10;
    MineralNode[3].fWid = 20;
    MineralNode[3].fHgt = 20;
    MineralNode[3].Colour = RGB(0, 0, 200);
    MineralNode[3].iMineralAmount = 100;
    
    MineralNode[4].fXPos = 170;
    MineralNode[4].fYPos = 350;
    MineralNode[4].fWid = 20;
    MineralNode[4].fHgt = 20;
    MineralNode[4].Colour = RGB(0, 0, 200);
    MineralNode[4].iMineralAmount = 100;
    
    MineralNode[5].fXPos = 60;
    MineralNode[5].fYPos = 250;
    MineralNode[5].fWid = 20;
    MineralNode[5].fHgt = 20;
    MineralNode[5].Colour = RGB(0, 0, 200);
    MineralNode[5].iMineralAmount = 100;
    
    CommandCenter.fXPos = 380;
    CommandCenter.fYPos = 290;
    CommandCenter.fWid = 60;
    CommandCenter.fHgt = 60;
    CommandCenter.Colour = RGB(200, 0, 0);
    
    Worker[0].fXPos = 220;
    Worker[0].fYPos = 160;
    Worker[0].fWid = 20.0;
    Worker[0].fHgt = 20.0;
    Worker[0].Colour = RGB(0, 255, 0);
    Worker[0].originalColour = RGB(0, 255, 0);
    Worker[0].Dead = 0;
    
    Worker[1].fXPos = 220;
    Worker[1].fYPos = 190;
    Worker[1].fWid = 20.0;
    Worker[1].fHgt = 20.0;
    Worker[1].Colour = RGB(0, 220, 0);
    Worker[1].originalColour = RGB(0, 220, 0);
    Worker[1].Dead = 0;
    
    Worker[2].fXPos = 220;
    Worker[2].fYPos = 220;
    Worker[2].fWid = 20.0;
    Worker[2].fHgt = 20.0;
    Worker[2].Colour = RGB(0, 180, 0);
    Worker[2].originalColour = RGB(0, 180, 0);
    Worker[2].Dead = 0;
    
    Worker[3].fXPos = 220;
    Worker[3].fYPos = 250;
    Worker[3].fWid = 20.0;
    Worker[3].fHgt = 20.0;
    Worker[3].Colour = RGB(0, 140, 0);
    Worker[3].originalColour = RGB(0, 140, 0);
    Worker[3].Dead = 0;
    
    Worker[4].fXPos = 220;
    Worker[4].fYPos = 280;
    Worker[4].fWid = 20.0;
    Worker[4].fHgt = 20.0;
    Worker[4].Colour = RGB(0, 90, 0);
    Worker[4].originalColour = RGB(0, 90, 0);
    Worker[4].Dead = 0;
    
    for(int i = 0; i < LOOPS; i++) {
        MineralNode[i].Draw();
        Worker[i].Draw();
        WhereToGo(Worker[i], MineralNode[0], CommandCenter, i);
    }
    
    CommandCenter.Draw();

    while(1) {
        if(PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE)) {
            if(Msg.message == WM_QUIT) {
                break;
            }
            else {
                TranslateMessage(&Msg);
                DispatchMessage(&Msg);
            }
        }
        else {
            if(TimeBase(&iTickLast)) {
                ClearScreen();
                for(int i = 0; i < LOOPS; i++) {
                    if(Worker[i].bInMotion) {
                        Worker[i].UpdatePosition();
                    }
                    else {
                        WhereToGo(Worker[i], MineralNode[i], CommandCenter, i);
                    }
                    MineralNode[i].Draw();
                    Worker[i].Draw();    
                }
                CommandCenter.Draw();
            }             
        }
    }
    return Msg.wParam;
}

#endif
