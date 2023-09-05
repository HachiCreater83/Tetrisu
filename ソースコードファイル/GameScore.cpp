
#include "Tetris.h"

HFONT Tetris::SetScoreFont(HDC hdc, int height)
{
    HFONT hFont;
    //フォント高さ
    hFont = CreateFont(height, 0, 0, 0,
        //文字幅、テキストの角度、ベースラインとｘ軸との角度
        FW_BOLD, FALSE, FALSE, FALSE, 
        //フォントの太さ、イタリック体、アンダーライン、打ち消し線
        SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        //文字セット、出力精度、クリッピング精度
        PROOF_QUALITY, FIXED_PITCH | FF_MODERN,
        //出力品質、ピッチとファミリー
        L"ＭＳ ゴシック");
    //書体名
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
