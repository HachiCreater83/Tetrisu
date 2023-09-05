
#include "Tetris.h"

BOOL Tetris::IsGameOver()
{
    return m_IsGameOver;
}

void Tetris::ShowGameOver(HDC hdc)
{
    HFONT hFont = SetScoreFont(hdc, 20);
    HFONT hFontOld = (HFONT)SelectObject(hdc, hFont);
    SetTextColor(hdc, RGB(255, 255, 255));

    const WCHAR* str1 = L"‚f‚`‚l‚d ‚n‚u‚d‚q";
    TextOut(hdc, 160, 100, str1, lstrlen(str1));
    SelectObject(hdc, hFontOld);
    DeleteObject(hFont);

    hFont = SetScoreFont(hdc, 15);
    hFontOld = (HFONT)SelectObject(hdc, hFont);
    const WCHAR* str2 = L"‚o‚q‚d‚r‚r ‚r ‚j‚d‚x";
    TextOut(hdc, 170, 130, str2, lstrlen(str2));
    SelectObject(hdc, hFontOld);
    DeleteObject(hFont);
}

void Tetris::Retry()
{
    m_Score = 0;
    m_CanHold = TRUE;
    m_holdTetrimino = TetriminoTypes::None;
    m_IsGameOver = FALSE;
    m_vectorTetriminoTypes.clear();

    Init();
}
