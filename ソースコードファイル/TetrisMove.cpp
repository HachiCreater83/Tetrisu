
#include "Tetris.h"
#include "BlockFall.h"

extern HWND hMainWnd;

//ブロックが移動する際の座標取得
void Tetris::GetRect(RECT* pRect)
{
	pRect->left = LeftTopBlockPosition.x;
	pRect->top = LeftTopBlockPosition.y;
	pRect->right = LeftTopBlockPosition.x + BLOCK_SIZE * COLUMN_MAX;
	pRect->bottom = LeftTopBlockPosition.y + BLOCK_SIZE * ROW_MAX;
}

BOOL Tetris::HardDrop()
{
	//ブロックを急降下した際のスコア判定処理
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
		// 配列 m_DeleteLineNumbersのなかの-1以外の値がある場合、
		// その行は上にあるブロックで詰められる
		int lineNumber = _deleteLineNumbers[i];
		if (lineNumber == -1)
		{
			continue;
		}

		for (int x = 0; x < COLUMN_MAX; x++)
		{
			// その行をひとつ上にあるブロックで埋める
			for (int row = lineNumber; row >= 1; row--)
			{
				m_FixedBlocks[row][x] = m_FixedBlocks[row - 1][x];
			}
			// 一番上の行はNULLを代入する。必ず一番上の行が空く
			m_FixedBlocks[0][x] = NULL;
		}
		count++;
		_deleteLineNumbers[i] = -1;
	}

	// 1列だけ消した場合は40点、2列:100点、3列:300点、4列:1200点
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

	// ソフトドロップは1つ落下させるたびに1点追加
	if (m_IsSoftDrop)
	{
		m_Score++;

		// スコアが表示される領域を更新する
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
