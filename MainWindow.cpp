
#include "all.h"

MainWindow::MainWindow(HINSTANCE hInstance)
{
    Create();
}

void MainWindow::Create()
{
    WNDCLASS myProg;
    myProg.style = CS_HREDRAW | CS_VREDRAW;
    myProg.lpfnWndProc = WndProcStatic;
    myProg.cbClsExtra = 0;
    myProg.cbWndExtra = 0;
    myProg.hInstance = m_hInstance;
    myProg.hIcon = NULL;
    myProg.hCursor = LoadCursor(NULL, IDC_ARROW);
    myProg.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // 背景を黒にする
    myProg.lpszMenuName = NULL;
    myProg.lpszClassName = m_szClassNme;
    RegisterClass(&myProg);

    CreateWindow(m_szClassNme,
        m_szTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 530, 600,
        NULL,
        NULL,
        m_hInstance,
        this);

    ShowWindow(m_hWnd, SW_SHOWNORMAL);
    UpdateWindow(m_hWnd);
}

LRESULT MainWindow::WndProcStatic(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    MainWindow* pMainWindow = (MainWindow*)GetWindowLongPtr(hWnd, GWL_USERDATA);

    if (msg == WM_CREATE) {
        LONG_PTR longPtr = (LONG_PTR)LPCREATESTRUCT(lp)->lpCreateParams;
        SetWindowLongPtr(hWnd, GWLP_USERDATA, longPtr);
        pMainWindow = reinterpret_cast<MainWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        pMainWindow->m_hWnd = hWnd;
    }

    if (pMainWindow != NULL)
        return pMainWindow->WndProc(msg, wp, lp);
    else
        return DefWindowProc(hWnd, msg, wp, lp);

    return LRESULT();
}

LRESULT MainWindow::WndProc(UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg) {
    case WM_CREATE:
        return OnCreate(wp, lp);
    case WM_DESTROY:
        return OnDestroy(wp, lp);
    case WM_PAINT:
        return OnPaint(wp, lp);
    case WM_KEYDOWN:
        return OnKeyDown(wp, lp);
    case WM_KEYUP:
        return OnKeyUp(wp, lp);
    case WM_TIMER:
        return OnTimer(wp, lp);

    default:
        return(DefWindowProc(m_hWnd, msg, wp, lp));
    }
    return 0L;
}

LRESULT MainWindow::OnCreate(WPARAM wp, LPARAM lp)
{
    /// <summary>
    /// タイマーをセット
    /// TIMER_FREE_FALLのタイマーは1秒経過するごとにひとつ下げる
    /// ブロックを消すときにいきなり上のブロックが落ちてくるのではなく間を空ける
    /// </summary>
    /// <param name="wp"></param>
    /// <param name="lp"></param>
    /// <returns></returns>666
    hMainWnd = m_hWnd;

    SetTimer(hMainWnd, TIMER_FREE_FALL, 1000, NULL);
    SetTimer(hMainWnd, TIMER_SOFT_DROP, 50, NULL);

    return 0L;
}

LRESULT MainWindow::OnDestroy(WPARAM wp, LPARAM lp)
{
    PostQuitMessage(0);
    return 0L;
}
/// <summary>
/// キーが押されたときの処理をおこないます
/// 移動させる処理
/// </summary>
/// <param name="wp"></param>
/// <param name="lp"></param>
/// <returns></returns>
LRESULT MainWindow::OnKeyDown(WPARAM wp, LPARAM lp)
{
    if (m_Tetris.IsGameOver())
    {
        UINT vk = (UINT)(wp);
        if (vk == 'S')
            m_Tetris.Retry();
        return 0L;
    }

    UINT vk = (UINT)(wp);

    // 移動の処理が実際におこなわれた場合だけ再描画の処理をする
    BOOL ret = FALSE;
    
    switch (vk) {
    case VK_UP:
        ret = m_Tetris.MoveUp();
        break;

    case VK_DOWN:
        m_Tetris.m_IsSoftDrop = FALSE;
        break;

    case VK_LEFT:
        ret = m_Tetris.MoveLeft();
        break;

    case VK_RIGHT:
        ret = m_Tetris.MoveRight();
        break;

    case 'Z':
        ret = m_Tetris.RotateLeft();
        break;

    case 'X':
        ret = m_Tetris.RotateRight();
        break;

    case VK_SPACE:
        ret = m_Tetris.HardDrop();
        break;

    default:
        break;
    }

    if (ret)
    {
        RECT rect;
        m_Tetris.GetRect(&rect);
        InvalidateRect(m_hWnd, &rect, TRUE);
    }

    return 0L;
}

LRESULT MainWindow::OnKeyUp(WPARAM wp, LPARAM lp)
{
    UINT vk = (UINT)(wp);
    switch (vk) {
    case VK_DOWN:
        m_Tetris.m_IsSoftDrop = FALSE;
        break;
    default:
        break;
    }
    return 0L;
}

LRESULT MainWindow::OnPaint(WPARAM wp, LPARAM lp)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(m_hWnd, &ps);

    m_Tetris.Draw(hdc);

    EndPaint(m_hWnd, &ps);
    return 0L;
}

LRESULT MainWindow::OnTimer(WPARAM wp, LPARAM lp)
{
    if (m_Tetris.IsGameOver()) {
        return 0L;
    }

    // ゆっくり下に移動
    if (wp == TIMER_FREE_FALL)
    {
        if (!m_Tetris.m_IsSoftDrop)
        {
            m_Tetris.MoveDown();

            RECT rect;
            m_Tetris.GetRect(&rect);
            InvalidateRect(m_hWnd, &rect, TRUE);
        }
    }

    // 急速に下に移動
    if (wp == TIMER_SOFT_DROP)
    {
        if (m_Tetris.m_IsSoftDrop)
        {
            m_Tetris.MoveDown();

            RECT rect;
            m_Tetris.GetRect(&rect);
            InvalidateRect(m_hWnd, &rect, TRUE);
        }
    }

    // ラインが消えたあと空間をつめる
    if (wp == TIMER_DROP_LINES)
    {
        m_Tetris.DropLines();
        InvalidateRect(m_hWnd, NULL, TRUE);
    }
    return 0L;
}

