
#include "all.h"

//�E�B���h�E�n���h�����擾����
HWND hMainWnd = NULL;

//windows�̃��C���֐�
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    //�f�[�^�^�Ƃ��ăn���h���̐錾
    MainWindow mainWindow(hInstance);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (msg.wParam);
}