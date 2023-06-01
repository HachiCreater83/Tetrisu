#pragma once

//�Q�[����ʂ��\������N���X
class MainWindow
{
    HWND m_hWnd = NULL;
    HINSTANCE m_hInstance = NULL;

    const WCHAR* m_szClassNme = L"MyClassName1";
    const WCHAR* m_szTitle = L"C++�e�g���X";

    //�Q�[���̖{�̂ɂȂ�I�u�W�F�N�g
    Tetris m_Tetris;

public:
    //�������̓ǂݍ���
    MainWindow(HINSTANCE hInstance);

    //�Q�[�����̐���
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