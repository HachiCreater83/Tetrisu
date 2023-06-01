#pragma once

//ゲーム画面を構成するクラス
class MainWindow
{
    HWND m_hWnd = NULL;
    HINSTANCE m_hInstance = NULL;

    const WCHAR* m_szClassNme = L"MyClassName1";
    const WCHAR* m_szTitle = L"C++テトリス";

    //ゲームの本体になるオブジェクト
    Tetris m_Tetris;

public:
    //メモリの読み込み
    MainWindow(HINSTANCE hInstance);

    //ゲーム情報の生成
    void Create();
    static LRESULT CALLBACK WndProcStatic(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
    LRESULT WndProc(UINT msg, WPARAM wp, LPARAM lp);

    LRESULT OnCreate(WPARAM wp, LPARAM lp);
    LRESULT OnDestroy(WPARAM wp, LPARAM lp);

    LRESULT OnPaint(WPARAM wp, LPARAM lp);

    LRESULT OnTimer(WPARAM wp, LPARAM lp);
    LRESULT OnKeyDown(WPARAM wp, LPARAM lp);
    LRESULT OnKeyUp(WPARAM wp, LPARAM lp);
};