
#include "all.h"

//��ʕ\���̃}�N��
//��ʃT�C�Y�̐ݒ���s��
#define WIDTH 500
#define HEIGHT 300



/// <summary>
/// �Q�[�����J�n����ۂɍs�����\�b�h
/// </summary>
MainWindow::MainWindow(HINSTANCE hInstance)
{
	Create();
}

/// <summary>
/// �Q�[����ʂ̐������s��
/// </summary>
void MainWindow::Create()
{
	//�E�B���h�E�N���X�Ɋւ���錾���s��
	WNDCLASS myProg;
	myProg.style = CS_HREDRAW | CS_VREDRAW;
	myProg.lpfnWndProc = WndProcStatic;
	myProg.cbClsExtra = 0;
	myProg.cbWndExtra = 0;
	myProg.hInstance = m_hInstance;
	myProg.hIcon = NULL;
	myProg.hCursor = LoadCursor(NULL, IDC_ARROW);

	// �w�i�����ɂ���
	myProg.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	myProg.lpszMenuName = NULL;
	myProg.lpszClassName = m_szClassNme;
	RegisterClass(&myProg);

	CreateWindow(m_szClassNme,
		m_szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 1060, 600,
		NULL,
		NULL,
		m_hInstance,
		this);

	//��ʍX�V���s��
	ShowWindow(m_hWnd, SW_SHOWNORMAL);
	UpdateWindow(m_hWnd);
}

/// <summary>
/// �E�B���h�E�ւ̃n���h��(�^�O)�ƃ��b�Z�[�W�R�[�h
/// </summary>
LRESULT MainWindow::WndProcStatic(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	MainWindow* pMainWindow = (MainWindow*)GetWindowLongPtr(hWnd, GWL_USERDATA);

	if (msg == WM_CREATE)
	{
		LONG_PTR longPtr = (LONG_PTR)LPCREATESTRUCT(lp)->lpCreateParams;
		SetWindowLongPtr(hWnd, GWLP_USERDATA, longPtr);
		pMainWindow = reinterpret_cast<MainWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		pMainWindow->m_hWnd = hWnd;
	}

	if (pMainWindow != NULL)
	{
		return pMainWindow->WndProc(msg, wp, lp);
	}

	else
	{
		return DefWindowProc(hWnd, msg, wp, lp);
	}

	return LRESULT();
}

/// <summary>
/// �E�B���h�E�ɑ΂��鐶���Ə��������s��
/// </summary>
LRESULT MainWindow::WndProc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
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

/// <summary>
/// �E�B���h�E���������ɍs���郁�\�b�h
/// </summary>
LRESULT MainWindow::OnCreate(WPARAM wp, LPARAM lp)
{
	// �^�C�}�[���Z�b�g����
	// TIMER_FREE_FALL�̃^�C�}�[��1�b�o�߂��邲�ƂɂЂƂ�����
	// �u���b�N�������Ƃ��ɂ����Ȃ��̃u���b�N�������Ă���̂ł͂Ȃ��Ԃ��󂯂�
	
	hMainWnd = m_hWnd;

	SetTimer(hMainWnd, TIMER_FREE_FALL, 1000, NULL);
	SetTimer(hMainWnd, TIMER_SOFT_DROP, 50, NULL);

	return 0L;
}

/// <summary>
/// �Q�[�����I���������Ƃ��V�X�e���Ɏ������\�b�h
/// </summary>
LRESULT MainWindow::OnDestroy(WPARAM wp, LPARAM lp)
{
	PostQuitMessage(0);
	return 0L;
}

/// <summary>
/// �L�[�������ꂽ�Ƃ��̏������s�����\�b�h
/// </summary>
LRESULT MainWindow::OnKeyDown(WPARAM wp, LPARAM lp)
{
	if (m_Tetris.IsGameOver())
	{
		UINT vk = (UINT)(wp);
		if (vk == 'S')
		{
			m_Tetris.Retry();
		}
		return 0L;
	}

	UINT vk = (UINT)(wp);

	// �ړ��̏��������ۂɂ����Ȃ�ꂽ�ꍇ�����ĕ`��̏���������
	BOOL ret = FALSE;

	switch (vk)
	{
	case VK_UP:
		ret = m_Tetris.MoveUp();
		break;

	case VK_DOWN:
		m_Tetris.m_IsSoftDrop = FALSE;
		ret = m_Tetris.MoveDown();
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

	case 'C':
		ret = m_Tetris.Hold();
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

	//�[���G��,long�^�Ƃ��Ė�������A�[���ŏ�����
	return 0L;
}

/// <summary>
/// �L�[�{�[�h���痣�ꂽ�ꍇ
/// </summary>
LRESULT MainWindow::OnKeyUp(WPARAM wp, LPARAM lp)
{
	UINT vk = (UINT)(wp);
	switch (vk)
	{
	case VK_DOWN:
		m_Tetris.m_IsSoftDrop = FALSE;
		break;
	default:
		break;
	}
	//�[���G��,long�^�Ƃ��Ė�������A�[���ŏ�����
	return 0L;
}

/// <summary>
/// �E�B���h�E�̕\�����s�����\�b�h
/// </summary>
LRESULT MainWindow::OnPaint(WPARAM wp, LPARAM lp)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(m_hWnd, &ps);

	m_Tetris.Draw(hdc);

	EndPaint(m_hWnd, &ps);
	//�[���G��,long�^�Ƃ��Ė�������A�[���ŏ�����
	return 0L;
}

//�E�B���h�E�̃^�C�}�[���Ă΂ꂽ�������s�����\�b�h
LRESULT MainWindow::OnTimer(WPARAM wp, LPARAM lp)
{
	if (m_Tetris.IsGameOver())
	{
		return 0L;
	}

	// ������艺�Ɉړ�
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

	// �}���ɉ��Ɉړ�
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

	// ���C�������������Ƌ�Ԃ��߂�
	if (wp == TIMER_DROP_LINES)
	{
		m_Tetris.DropLines();
		InvalidateRect(m_hWnd, NULL, TRUE);
	}
	//�[���G��,long�^�Ƃ��Ė�������A�[���ŏ�����
	return 0L;
}