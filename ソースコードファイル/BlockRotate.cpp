
#include "Tetris.h"

//ブロックを回転させる処理
BOOL Tetris::CanRotate(BOOL isRight, int x, int y)
{
	TetriminoAngle nextAngle;
	if (isRight)
	{
		nextAngle = TetriminoAngle::Angle90;
	}
	else
	{
		nextAngle = TetriminoAngle::Angle270;
	}
	TetriminoPosition BlockSize[4];

	//m_TetriminoPositionX == 0 m_TetriminoPositionY == 0の状態に戻して考える
	GetTetriminosPosition(BlockSize, _fieldTetriminoPosition);
	for (int i = 0; i < _fieldTetriminoPosition; i++)
	{
		BlockSize[i].m_column -= m_TetriminoPositionX;
		BlockSize[i].m_row -= m_TetriminoPositionY;
	}

	if (m_CurTetriminoType != TetriminoTypes::I && m_CurTetriminoType != TetriminoTypes::O)
	{
		GetInitBlocksRotate33(BlockSize, _fieldTetriminoPosition, nextAngle);
	}
	else if (m_CurTetriminoType == TetriminoTypes::I)
	{
		GetInitBlocksRotate44(BlockSize, _fieldTetriminoPosition, nextAngle);
	}

	//回転軸のズレを加える
	for (int i = 0; i < _fieldTetriminoPosition; i++)
	{
		BlockSize[i].m_column += x;
		BlockSize[i].m_row += y;
	}

	//現在のm_TetriminoPositionXとm_TetriminoPositionYの分だけ平行移動させる
	for (int i = 0; i < _fieldTetriminoPosition; i++)
	{
		BlockSize[i].m_column += m_TetriminoPositionX;
		BlockSize[i].m_row += m_TetriminoPositionY;
	}

	//ここで回転後のブロックの座標を取得
	//後は当たり判定をおこなう
	//フィールドの外に出たり、すでに固定されているブロックがある場合は移動・回転できない
	for (int i = 0; i < _fieldTetriminoPosition; i++)
	{
		if (BlockSize[i].m_column > COLUMN_MAX - 1)
		{
			return FALSE;
		}
		if (BlockSize[i].m_column < 0)
		{
			return FALSE;
		}
		if (BlockSize[i].m_row > ROW_MAX - 1)
		{
			return FALSE;
		}
		if (m_FixedBlocks[BlockSize[i].m_row][BlockSize[i].m_column] != NULL)
		{
			return FALSE;
		}
	}
	return TRUE;
}

//右回転した際のブロックの落下を止める
BOOL Tetris::RotateRight()
{
	BOOL ret = FALSE;
	if (m_CurTetriminoType != TetriminoTypes::I && m_CurTetriminoType != TetriminoTypes::O)
	{
		ret = RotateRight3();
		if (ret)
		{
			ResetTimer();
		}
	}
	else if (m_CurTetriminoType == TetriminoTypes::I)
	{
		ret = RotateRightI();
		if (ret)
		{
			ResetTimer();
		}
	}

	return ret;
}

//基本的なブロックの右回転処理
BOOL Tetris::RotateRight3()
{
	if (CanRotate(TRUE, 0, 0))
	{
		return RotateRight(0, 0);
	}
	if (m_TetriminoAngle == TetriminoAngle::Angle0)
	{
		if (CanRotate(TRUE, -1, 0))
		{
			return RotateRight(-1, 0);
		}
		if (CanRotate(TRUE, -1, -1))
		{
			return RotateRight(-1, -1);
		}
		if (CanRotate(TRUE, 0, 2))
		{
			return RotateRight(0, 2);
		}
		if (CanRotate(TRUE, -1, 2))
		{
			return RotateRight(-1, 2);
		}
	}
	else if (m_TetriminoAngle == TetriminoAngle::Angle90)
	{
		if (CanRotate(TRUE, 1, 0))
		{
			return RotateRight(1, 0);
		}
		if (CanRotate(TRUE, 1, 1))
		{
			return RotateRight(1, 1);
		}
		if (CanRotate(TRUE, 0, -2))
		{
			return RotateRight(0, -2);
		}
		if (CanRotate(TRUE, 1, -2))
		{
			return RotateRight(1, -2);
		}
	}
	else if (m_TetriminoAngle == TetriminoAngle::Angle180)
	{
		if (CanRotate(TRUE, 1, 0))
		{
			return RotateRight(1, 0);
		}
		if (CanRotate(TRUE, 1, -1))
		{
			return RotateRight(1, -1);
		}
		if (CanRotate(TRUE, 0, 2))
		{
			return RotateRight(0, 2);
		}
		if (CanRotate(TRUE, 1, 2))
		{
			return RotateRight(1, 2);
		}
	}
	else if (m_TetriminoAngle == TetriminoAngle::Angle270)
	{
		if (CanRotate(TRUE, -2, 0))
		{
			return RotateRight(-2, 0);
		}
		if (CanRotate(TRUE, -2, 1))
		{
			return RotateRight(-2, 1);
		}
		if (CanRotate(TRUE, 0, -2))
		{
			return RotateRight(0, -2);
		}
		if (CanRotate(TRUE, -1, -2))
		{
			return RotateRight(-1, -2);
		}
	}
	return FALSE;
}

//棒ブロックの右回転処理
BOOL Tetris::RotateRightI()
{
	if (CanRotate(TRUE, 0, 0))
	{
		return RotateRight(0, 0);
	}

	if (m_TetriminoAngle == TetriminoAngle::Angle0)
	{
		if (CanRotate(TRUE, -2, 0))
		{
			return RotateRight(-2, 0);
		}
		if (CanRotate(TRUE, 1, 0))
		{
			return RotateRight(1, 0);
		}
		if (CanRotate(TRUE, -2, 1))
		{
			return RotateRight(-2, 1);
		}
		if (CanRotate(TRUE, 1, -2))
		{
			return RotateRight(1, -2);
		}
	}
	else if (m_TetriminoAngle == TetriminoAngle::Angle90)
	{
		if (CanRotate(TRUE, -1, 0))
		{
			return RotateRight(-1, 0);
		}
		if (CanRotate(TRUE, 2, 0))
		{
			return RotateRight(2, 0);
		}
		if (CanRotate(TRUE, -1, -2))
		{
			return RotateRight(-1, -2);
		}
		if (CanRotate(TRUE, 2, 1))
		{
			return RotateRight(2, 1);
		}
	}
	else if (m_TetriminoAngle == TetriminoAngle::Angle180)
	{
		if (CanRotate(TRUE, 2, 0))
		{
			return RotateRight(2, 0);
		}
		if (CanRotate(TRUE, -1, 0))
		{
			return RotateRight(-1, 0);
		}
		if (CanRotate(TRUE, 2, -1))
		{
			return RotateRight(2, -1);
		}
		if (CanRotate(TRUE, -1, 2))
		{
			return RotateRight(-1, 2);
		}
	}
	else if (m_TetriminoAngle == TetriminoAngle::Angle270)
	{
		if (CanRotate(TRUE, -2, 0))
		{
			return RotateRight(-2, 0);
		}
		if (CanRotate(TRUE, 1, 0))
		{
			return RotateRight(1, 0);
		}
		if (CanRotate(TRUE, 1, 2))
		{
			return RotateRight(1, 2);
		}
		if (CanRotate(TRUE, -2, -1))
		{
			return RotateRight(-2, -1);
		}
	}
	return FALSE;
}

//右回転時の回転の移動処理
BOOL Tetris::RotateRight(int x, int y)
{
	m_TetriminoPositionX += x;
	m_TetriminoPositionY += y;

	if (m_TetriminoAngle == TetriminoAngle::Angle0)
	{
		m_TetriminoAngle = TetriminoAngle::Angle90;
	}
	else if (m_TetriminoAngle == TetriminoAngle::Angle90)
	{
		m_TetriminoAngle = TetriminoAngle::Angle180;
	}
	else if (m_TetriminoAngle == TetriminoAngle::Angle180)
	{
		m_TetriminoAngle = TetriminoAngle::Angle270;
	}
	else if (m_TetriminoAngle == TetriminoAngle::Angle270)
	{
		m_TetriminoAngle = TetriminoAngle::Angle0;
	}
	return TRUE;
}

//左回転した際のブロックの落下を止める
BOOL Tetris::RotateLeft()
{
	BOOL ret = FALSE;
	if (m_CurTetriminoType != TetriminoTypes::I && m_CurTetriminoType != TetriminoTypes::O)
	{
		ret = RotateLeft3();
		if (ret)
		{
			ResetTimer();
		}
	}
	else if (m_CurTetriminoType == TetriminoTypes::I)
	{
		ret = RotateLeftI();
		if (ret)
		{
			ResetTimer();
		}
	}
	return ret;
}

//基本的なブロックの左回転処理
BOOL Tetris::RotateLeft3()
{
	if (CanRotate(FALSE, 0, 0))
	{
		return RotateLeft(0, 0);
	}
	if (m_TetriminoAngle == TetriminoAngle::Angle0)
	{
		if (CanRotate(FALSE, 1, 0))
		{
			return RotateLeft(1, 0);
		}
		if (CanRotate(FALSE, 1, -1))
		{
			return RotateLeft(1, -1);
		}
		if (CanRotate(FALSE, 0, 2))
		{
			return RotateLeft(0, 2);
		}
		if (CanRotate(FALSE, 1, 2))
		{
			return RotateLeft(1, 2);
		}
	}
	else if (m_TetriminoAngle == TetriminoAngle::Angle90)
	{
		if (CanRotate(FALSE, 1, 0))
		{
			return RotateLeft(1, 0);
		}
		if (CanRotate(FALSE, 1, 1))
		{
			return RotateLeft(1, 1);
		}
		if (CanRotate(FALSE, 0, -2))
		{
			return RotateLeft(0, -2);
		}
		if (CanRotate(FALSE, 1, -2))
		{
			return RotateLeft(1, -2);
		}
	}
	else if (m_TetriminoAngle == TetriminoAngle::Angle180)
	{
		if (CanRotate(FALSE, -1, 0))
		{
			return RotateLeft(-1, 0);
		}
		if (CanRotate(FALSE, -1, -1))
		{
			return RotateLeft(-1, -1);
		}
		if (CanRotate(FALSE, 0, 2))
		{
			return RotateLeft(0, 2);
		}
		if (CanRotate(FALSE, -1, 2))
		{
			return RotateLeft(-1, 2);
		}
	}
	else if (m_TetriminoAngle == TetriminoAngle::Angle270)
	{
		if (CanRotate(FALSE, -1, 0))
		{
			return RotateLeft(-1, 0);
		}
		if (CanRotate(FALSE, -1, 1))
		{
			return RotateLeft(-1, 1);
		}
		if (CanRotate(FALSE, 0, -2))
		{
			return RotateLeft(0, -2);
		}
		if (CanRotate(FALSE, -1, -2))
		{
			return RotateLeft(-1, -2);
		}
	}
	return FALSE;
}

//棒ブロックの左回転処理
BOOL Tetris::RotateLeftI()
{
	if (CanRotate(FALSE, 0, 0))
	{
		return RotateLeft(0, 0);
	}
	if (m_TetriminoAngle == TetriminoAngle::Angle0)
	{
		if (CanRotate(FALSE, -1, 0))
		{
			return RotateLeft(-1, 0);
		}
		if (CanRotate(FALSE, 2, 0))
		{
			return RotateLeft(2, 0);
		}
		if (CanRotate(FALSE, -2, -2))
		{
			return RotateLeft(-2, -2);
		}
		if (CanRotate(FALSE, 2, 1))
		{
			return RotateLeft(2, 1);
		}
	}
	else if (m_TetriminoAngle == TetriminoAngle::Angle90)
	{
		if (CanRotate(FALSE, 2, 0))
		{
			return RotateLeft(2, 0);
		}
		if (CanRotate(FALSE, -1, 0))
		{
			return RotateLeft(-1, 0);
		}
		if (CanRotate(FALSE, 2, -1))
		{
			return RotateLeft(2, -1);
		}
		if (CanRotate(FALSE, -1, 2))
		{
			return RotateLeft(-1, 2);
		}
	}
	else if (m_TetriminoAngle == TetriminoAngle::Angle180)
	{
		if (CanRotate(FALSE, 1, 0))
		{
			return RotateLeft(1, 0);
		}
		if (CanRotate(FALSE, -2, 0))
		{
			return RotateLeft(-2, 0);
		}
		if (CanRotate(FALSE, 1, 2))

		{
			return RotateLeft(1, 2);
		}
		if (CanRotate(FALSE, -2, -1))
		{
			return RotateLeft(-2, -1);
		}
	}
	else if (m_TetriminoAngle == TetriminoAngle::Angle270)
	{
		if (CanRotate(FALSE, 1, 0))
		{
			return RotateLeft(1, 0);
		}
		if (CanRotate(FALSE, -2, 0))
		{
			return RotateLeft(-2, 0);
		}
		if (CanRotate(FALSE, -2, 1))
		{
			return RotateLeft(-2, 1);
		}
		if (CanRotate(FALSE, 1, -2))
		{
			return RotateLeft(1, -2);
		}
	}
	return FALSE;
}

//左回転時の回転の移動処理
BOOL Tetris::RotateLeft(int x, int y)
{
	m_TetriminoPositionX += x;
	m_TetriminoPositionY += y;

	if (m_TetriminoAngle == TetriminoAngle::Angle0)
	{
		m_TetriminoAngle = TetriminoAngle::Angle270;
	}
	else if (m_TetriminoAngle == TetriminoAngle::Angle90)
	{
		m_TetriminoAngle = TetriminoAngle::Angle0;
	}
	else if (m_TetriminoAngle == TetriminoAngle::Angle180)
	{
		m_TetriminoAngle = TetriminoAngle::Angle90;
	}
	else if (m_TetriminoAngle == TetriminoAngle::Angle270)
	{
		m_TetriminoAngle = TetriminoAngle::Angle180;
	}
	return TRUE;
}

/// <summary>
/// ブロックの回転に合わせた位置情報の更新を行う
/// </summary>
/// <param name="pos"></param>
/// <param name="size"></param>
/// <param name="angle"></param>

void Tetris::GetInitBlocksRotate33(TetriminoPosition pos[], int size, TetriminoAngle angle)
{
	//ブロックが消せる状況なら処理を終える
	if (size != block_deleteLine)
	{
		return;
	}
	if (angle == TetriminoAngle::Angle90)
	{
		for (int i = 0; i < size; i++)
		{
			int x = pos[i].m_column;
			int y = pos[i].m_row;
			pos[i].m_column = 3 - y - 1;
			pos[i].m_row = x;
		}
	}
	else if (angle == TetriminoAngle::Angle180)
	{
		for (int i = 0; i < size; i++)
		{
			int x = pos[i].m_column;
			int y = pos[i].m_row;
			pos[i].m_column = 3 - x - 1;
			pos[i].m_row = 3 - y - 1;
		}
	}
	else if (angle == TetriminoAngle::Angle270)
	{
		for (int i = 0; i < size; i++)
		{
			int x = pos[i].m_column;
			int y = pos[i].m_row;
			pos[i].m_column = y;
			pos[i].m_row = 3 - x - 1;
		}
	}
}

void Tetris::GetInitBlocksRotate44(TetriminoPosition pos[], int size, TetriminoAngle angle)
{
	//ブロックが消せる状況なら処理を終える
	if (size != block_deleteLine)
	{
		return;
	}
	if (angle == TetriminoAngle::Angle90)
	{
		for (int i = 0; i < size; i++)
		{
			int x = pos[i].m_column;
			int y = pos[i].m_row;
			pos[i].m_column = 4 - y - 1;
			pos[i].m_row = x;
		}
	}
	else if (angle == TetriminoAngle::Angle180)
	{
		for (int i = 0; i < size; i++)
		{
			int x = pos[i].m_column;
			int y = pos[i].m_row;
			pos[i].m_column = 4 - x - 1;
			pos[i].m_row = 4 - y - 1;
		}
	}
	else if (angle == TetriminoAngle::Angle270)
	{
		for (int i = 0; i < size; i++)
		{
			int x = pos[i].m_column;
			int y = pos[i].m_row;
			pos[i].m_column = y;
			pos[i].m_row = 4 - x - 1;
		}
	}
}
