

#include "Tetris.h"
#include "BlockFall.h"

extern HWND hMainWnd;

void Tetris::ShowHold(HDC hdc)
{
    // 文字列「ＨＯＬＤ」を表示
    SetTextColor(hdc, RGB(255, 255, 255));
    HFONT hFont = SetScoreFont(hdc, 16);
    HFONT hFontOld = (HFONT)SelectObject(hdc, hFont);
    const WCHAR* str = L"ＨＯＬＤ";

    TextOut(hdc, 30, 50, str, lstrlen(str));
    SelectObject(hdc, hFontOld);
    DeleteObject(hFont);

    POINT leftTop;
    leftTop.x = 40;
    leftTop.y = 100;
    if (m_holdTetrimino != TetriminoTypes::None)
        DrawNextBlock(hdc, leftTop, m_holdTetrimino);
}

BOOL Tetris::Hold()
{
    // ホールドできない場合はなにもしない
    if (m_CanHold == FALSE)
        return FALSE;

    // ホールドしたら設置されるまでホールドはできなくなる
    m_CanHold = FALSE;

    // なにもホールドされていない場合は落下中のテトロミノをホールドして次のテトリミノを降らせる
    // すでになにかがホールドされている場合は落下中のテトロミノをホールドして
    // いままでホールドされていたテトリミノを一番上から降らせる
    if (m_holdTetrimino == TetriminoTypes::None)
    {
        m_holdTetrimino = m_CurTetriminoType;
        NewTetrimino();
    }
    else
    {
        TetriminoTypes oldHold = m_holdTetrimino;
        m_holdTetrimino = m_CurTetriminoType;
        m_TetriminoAngle = TetriminoAngle::Angle0;
        m_TetriminoPositionX = 3;
        m_TetriminoPositionY = -1;

        m_CurTetriminoType = oldHold;
    }
    InvalidateRect(hMainWnd, NULL, TRUE);
    return TRUE;
}