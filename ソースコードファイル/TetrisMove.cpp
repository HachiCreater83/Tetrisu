
#include "Tetris.h"
#include "BlockFall.h"

extern HWND hMainWnd;

//�u���b�N���ړ�����ۂ̍��W�擾
void Tetris::GetRect(RECT* pRect)
{
	pRect->left = LeftTopBlockPosition.x;
	pRect->top = LeftTopBlockPosition.y;
	pRect->right = LeftTopBlockPosition.x + BLOCK_SIZE * COLUMN_MAX;
	pRect->bottom = LeftTopBlockPosition.y + BLOCK_SIZE * ROW_MAX;
}

BOOL Tetris::HardDrop()
{
	//�u���b�N���}�~�������ۂ̃X�R�A���菈��
	int fallCount = GetHardDropCount();
	m_Score += fallCount * 2;

	m_TetriminoPositionY += fallCount;
	InvalidateRect(hMainWnd, NULL, TRUE);
	FixBlocks();
	return TRUE;
}

int Tetris::GetHardDropCount()
{
	TetriminoPosition blocks[4];
	GetTetriminosPosition(blocks, 4);

	int count = 0;
	while (true)
	{
		BOOL failure = FALSE;
		for (int i = 0; i < 4; i++)
		{
			blocks[i].m_row++;

			if (0 <= blocks[i].m_row && blocks[i].m_row < ROW_MAX && 0 <= blocks[i].m_column && blocks[i].m_column < COLUMN_MAX)
			{
				if (blocks[i].m_row < ROW_MAX && m_FixedBlocks[blocks[i].m_row][blocks[i].m_column] != NULL)
				{
					failure = TRUE;
				}
			}
			else
			{
				failure = TRUE;
			}
			if (failure)
			{
				break;
			}
		}
		if (!failure)
		{
			count++;
		}
		else
		{
			break;
		}
	}
	return count;
}

void Tetris::DropLines()
{
	int count = 0;
	for (int i = 0; i < 4; i++)
	{
		// �z�� m_DeleteLineNumbers�̂Ȃ���-1�ȊO�̒l������ꍇ�A
		// ���̍s�͏�ɂ���u���b�N�ŋl�߂���
		int lineNumber = _deleteLineNumbers[i];
		if (lineNumber == -1)
		{
			continue;
		}

		for (int x = 0; x < COLUMN_MAX; x++)
		{
			// ���̍s���ЂƂ�ɂ���u���b�N�Ŗ��߂�
			for (int row = lineNumber; row >= 1; row--)
			{
				m_FixedBlocks[row][x] = m_FixedBlocks[row - 1][x];
			}
			// ��ԏ�̍s��NULL��������B�K����ԏ�̍s����
			m_FixedBlocks[0][x] = NULL;
		}
		count++;
		_deleteLineNumbers[i] = -1;
	}

	// 1�񂾂��������ꍇ��40�_�A2��:100�_�A3��:300�_�A4��:1200�_
	if (count == 1)
	{
		m_Score += 40;
	}
	if (count == 2)
	{
		m_Score += 100;
	}
	if (count == 3)
	{
		m_Score += 300;
	}
	if (count == 4)
	{
		m_Score += 1200;
	}

	KillTimer(hMainWnd, TIMER_DROP_LINES);
}

BOOL Tetris::MoveRight()
{
	if (CanMove(1, 0))
	{
		m_TetriminoPositionX += 1;
		m_TetriminoPositionY += 0;
		ResetTimer();
		return TRUE;
	}
	return FALSE;
}

BOOL Tetris::MoveUp()
{
	return 0;
}

BOOL Tetris::MoveLeft()
{
	if (CanMove(-1, 0))
	{
		m_TetriminoPositionX += -1;
		m_TetriminoPositionY += 0;
		ResetTimer();
		return TRUE;
	}
	return FALSE;
}

BOOL Tetris::MoveDown()
{
	if (CheckDeletingLines())
	{
		return FALSE;
	}

	// �\�t�g�h���b�v��1���������邽�т�1�_�ǉ�
	if (m_IsSoftDrop)
	{
		m_Score++;

		// �X�R�A���\�������̈���X�V����
		RECT rect;
		rect.left = 0;
		rect.top = 0;
		rect.right = 300;
		rect.bottom = 70;
		InvalidateRect(hMainWnd, &rect, TRUE);
	}

	ResetTimer();

	if (CanMove(0, 1))
	{
		m_TetriminoPositionX += 0;
		m_TetriminoPositionY += 1;
	}
	else
	{
		FixBlocks();
		CheckLine();
		NewTetrimino();
	}
	return TRUE;
}
