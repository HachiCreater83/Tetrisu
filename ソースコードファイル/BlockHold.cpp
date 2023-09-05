

#include "Tetris.h"
#include "BlockFall.h"

extern HWND hMainWnd;

void Tetris::ShowHold(HDC hdc)
{
    // ������u�g�n�k�c�v��\��
    SetTextColor(hdc, RGB(255, 255, 255));
    HFONT hFont = SetScoreFont(hdc, 16);
    HFONT hFontOld = (HFONT)SelectObject(hdc, hFont);
    const WCHAR* str = L"�g�n�k�c";

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
    // �z�[���h�ł��Ȃ��ꍇ�͂Ȃɂ����Ȃ�
    if (m_CanHold == FALSE)
        return FALSE;

    // �z�[���h������ݒu�����܂Ńz�[���h�͂ł��Ȃ��Ȃ�
    m_CanHold = FALSE;

    // �Ȃɂ��z�[���h����Ă��Ȃ��ꍇ�͗������̃e�g���~�m���z�[���h���Ď��̃e�g���~�m���~�点��
    // ���łɂȂɂ����z�[���h����Ă���ꍇ�͗������̃e�g���~�m���z�[���h����
    // ���܂܂Ńz�[���h����Ă����e�g���~�m����ԏォ��~�点��
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