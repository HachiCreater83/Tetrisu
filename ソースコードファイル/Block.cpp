
#include "Tetris.h"
#include "MainWindow.h"


//ブロックを新しく生成する際の処理
void Tetris::NewTetrimino()
{
	m_TetriminoAngle = TetriminoAngle::Angle0;
	m_TetriminoPositionX = 3;
	m_TetriminoPositionY = -1;
	SetNewBlockType();

	//すでに固定されているブロックと衝突している場合ゲームの終了判定を出す
	if (!CanMove(0, 0))
	{
		m_IsGameOver = TRUE;
	}
}

/// <summary>
/// ブロックに合わせた配置を決める
/// </summary>
/// <param name="pos"></param>
/// <param name="size"></param>

void Tetris::GetTetriminosPosition(TetriminoPosition pos[], int size)
{
	// m_TetriminoPositionXとm_TetriminoPositionYが0のときのTetriminoPositionの配列を得る
	if (m_CurTetriminoType == TetriminoTypes::I)
	{
		GetInitBlocksPositionI(pos, size);
	}
	else if (m_CurTetriminoType == TetriminoTypes::J)
	{
		GetInitBlocksPositionJ(pos, size);
	}
	else if (m_CurTetriminoType == TetriminoTypes::L)
	{
		GetInitBlocksPositionL(pos, size);
	}
	else if (m_CurTetriminoType == TetriminoTypes::O)
	{
		GetInitBlocksPositionO(pos, size);
	}
	else if (m_CurTetriminoType == TetriminoTypes::S)
	{
		GetInitBlocksPositionS(pos, size);
	}
	else if (m_CurTetriminoType == TetriminoTypes::T)
	{
		GetInitBlocksPositionT(pos, size);
	}
	else if (m_CurTetriminoType == TetriminoTypes::Z)
	{
		GetInitBlocksPositionZ(pos, size);
	}
	// 回転させる。Oミノは回転しない。Iミノ以外は3×3、Iミノは4×4なので分けて処理をする
	if (m_CurTetriminoType != TetriminoTypes::I)
	{
		GetInitBlocksRotate33(pos, size, m_TetriminoAngle);
	}
	else if (m_CurTetriminoType == TetriminoTypes::I)
	{
		GetInitBlocksRotate44(pos, size, m_TetriminoAngle);
	}

	// m_TetriminoPositionXとm_TetriminoPositionYが現在の値の場合のTetriminoPositionの配列を得る
	for (int i = 0; i < size; i++)
	{
		pos[i].m_column += m_TetriminoPositionX;
		pos[i].m_row += m_TetriminoPositionY;
	}
}

void Tetris::GetInitBlocksPositionI(TetriminoPosition pos[], int size)
{
	if (size != block_deleteLine)
	{
		return;
	}
	pos[0].m_column = 0;
	pos[0].m_row = 1;

	pos[1].m_column = 1;
	pos[1].m_row = 1;

	pos[2].m_column = 2;
	pos[2].m_row = 1;

	pos[3].m_column = 3;
	pos[3].m_row = 1;
}

void Tetris::GetInitBlocksPositionJ(TetriminoPosition pos[], int size)
{
	if (size != block_deleteLine)
	{
		return;
	}
	pos[0].m_column = 0;
	pos[0].m_row = 0;

	pos[1].m_column = 0;
	pos[1].m_row = 1;

	pos[2].m_column = 1;
	pos[2].m_row = 1;

	pos[3].m_column = 2;
	pos[3].m_row = 1;
}

void Tetris::GetInitBlocksPositionL(TetriminoPosition pos[], int size)
{
	if (size != block_deleteLine)
	{
		return;
	}
	pos[0].m_column = 2;
	pos[0].m_row = 0;

	pos[1].m_column = 0;
	pos[1].m_row = 1;

	pos[2].m_column = 1;
	pos[2].m_row = 1;

	pos[3].m_column = 2;
	pos[3].m_row = 1;
}

void Tetris::GetInitBlocksPositionO(TetriminoPosition pos[], int size)
{
	if (size != block_deleteLine)
	{
		return;
	}
	pos[0].m_column = 1;
	pos[0].m_row = 0;

	pos[1].m_column = 1;
	pos[1].m_row = 1;

	pos[2].m_column = 2;
	pos[2].m_row = 0;

	pos[3].m_column = 2;
	pos[3].m_row = 1;
}

void Tetris::GetInitBlocksPositionS(TetriminoPosition pos[], int size)
{
	if (size != block_deleteLine)
	{
		return;
	}

	pos[0].m_column = 1;
	pos[0].m_row = 0;

	pos[1].m_column = 2;
	pos[1].m_row = 0;

	pos[2].m_column = 0;
	pos[2].m_row = 1;

	pos[3].m_column = 1;
	pos[3].m_row = 1;
}

void Tetris::GetInitBlocksPositionT(TetriminoPosition pos[], int size)
{
	if (size != block_deleteLine)
	{
		return;
	}

	pos[0].m_column = 1;
	pos[0].m_row = 0;

	pos[1].m_column = 0;
	pos[1].m_row = 1;

	pos[2].m_column = 1;
	pos[2].m_row = 1;

	pos[3].m_column = 2;
	pos[3].m_row = 1;
}

void Tetris::GetInitBlocksPositionZ(TetriminoPosition pos[], int size)
{
	if (size != block_deleteLine)
	{
		return;
	}
	pos[0].m_column = 0;
	pos[0].m_row = 0;

	pos[1].m_column = 1;
	pos[1].m_row = 0;

	pos[2].m_column = 1;
	pos[2].m_row = 1;

	pos[3].m_column = 2;
	pos[3].m_row = 1;
}

//ブロックの描画を行うメソッド
void Tetris::DrawBlock(HDC hdc, int column, int row, Color color)
{
	if (row < 0)
	{
		return;
	}

	COLORREF colorref = GetColor(color);

	// ペンとブラシを生成・選択
	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
	HBRUSH hBrush = CreateSolidBrush(colorref);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

	// 0行0列の位置を少しウィンドウの内側に移動させる
	int x = column * BLOCK_SIZE + LeftTopBlockPosition.x;
	int y = row * BLOCK_SIZE + LeftTopBlockPosition.y;
	::Rectangle(hdc, x, y, x + BLOCK_SIZE, y + BLOCK_SIZE);

	// ペンとブラシをもとに戻す
	SelectObject(hdc, hOldPen);
	SelectObject(hdc, hOldBrush);

	// 自分で作成したペンとブラシを削除する
	DeleteObject(hPen);
	DeleteObject(hBrush);
}

//次に生成されるブロックの描画を行う
void Tetris::DrawOutsideBlocks(HDC hdc)
{
	for (int row = 0; row < ROW_MAX + 1; row++)
	{
		DrawBlock(hdc, -1, row, Color::Gray);
		DrawBlock(hdc, COLUMN_MAX, row, Color::Gray);
	}
	for (int colum = -1; colum < COLUMN_MAX + 1; colum++)
		DrawBlock(hdc, colum, ROW_MAX, Color::Gray);
}

//ブロックの描画を行う判定を取る
void Tetris::DrawFixedBlock(HDC hdc)
{
	for (int row = 0; row < ROW_MAX; row++)
	{
		for (int column = 0; column < COLUMN_MAX; column++)
		{
			Block* pBlock = m_FixedBlocks[row][column];

			// NULLでなければブロックが存在するということなので描画する
			if (pBlock != NULL)
				DrawBlock(hdc, column, row, pBlock->color);
		}
	}
}

//ブロックの生成を行う判定
void Tetris::FixBlocks()
{
	TetriminoPosition blocks[4];
	GetTetriminosPosition(blocks, 4);
	for (int i = 0; i < 4; i++)
	{
		Block* pBlock = new Block();
		pBlock->color = GetTetriminoColor();
		m_FixedBlocks[blocks[i].m_row][blocks[i].m_column] = pBlock;
	}
	m_CanHold = TRUE;
}
