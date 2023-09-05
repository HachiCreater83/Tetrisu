
#include "Tetris.h"

HFONT Tetris::SetScoreFont(HDC hdc, int height)
{
    HFONT hFont;
    //�t�H���g����
    hFont = CreateFont(height, 0, 0, 0,
        //�������A�e�L�X�g�̊p�x�A�x�[�X���C���Ƃ����Ƃ̊p�x
        FW_BOLD, FALSE, FALSE, FALSE, 
        //�t�H���g�̑����A�C�^���b�N�́A�A���_�[���C���A�ł�������
        SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        //�����Z�b�g�A�o�͐��x�A�N���b�s���O���x
        PROOF_QUALITY, FIXED_PITCH | FF_MODERN,
        //�o�͕i���A�s�b�`�ƃt�@�~���[
        L"�l�r �S�V�b�N");
    //���̖�
    return hFont;
};

void Tetris::ShowScore(HDC hdc)
{
    WCHAR scoreText[32];
    wsprintf(scoreText, L"SCORE  %d", m_Score);

    HFONT hFont = SetScoreFont(hdc, 20);
    HFONT hFontOld = (HFONT)SelectObject(hdc, hFont);

    SetBkColor(hdc, RGB(0, 0, 0));
    SetTextColor(hdc, RGB(255, 255, 255));

    TextOut(hdc, 20, 20, scoreText, lstrlen(scoreText));

    SelectObject(hdc, hFontOld);
    DeleteObject(hFont);
}
