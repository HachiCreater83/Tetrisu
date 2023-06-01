
#include "all.h"
#include <time.h>

/// <summary>
/// LeftTopBlockPositionに座標をセット
/// ウィンドウの左上の座標に余白を持たせたかったのでこのような設定にしました
/// </summary>
Tetris::Tetris()
{
	LeftTopBlockPosition.x = 150;
	LeftTopBlockPosition.y = 70;

	//初期化を呼び出す
	Init();
}

//ゲーム開始時の初期化
void Tetris::Init()
{
	//ブロックの初期化を呼び出す
	InitFixedBlocks();

	// ゲーム開始時にはホールドされているテトリミノは存在しないためNone
	// ゲーム開始直後はいつでもホールドは可能である
	m_holdTetrimino = TetriminoTypes::None;
	m_CanHold = TRUE;

	for (int i = 0; i < 4; i++)
	{
		m_DeleteLineNumbers[i] = -1;
	}

	unsigned int now = (unsigned int)time(0);
	srand(now);

	CreateBag();
	NewTetrimino();
}

//ブロックの初期化
void Tetris::InitFixedBlocks()
{
	for (int row = 0; row < 20; row++)
	{
		for (int column = 0; column < 10; column++)
		{
			delete m_FixedBlocks[row][column];
			m_FixedBlocks[row][column] = NULL;
		}
	}
}

// タイマーをリセットし、すぐにセットする
void Tetris::ResetTimer()
{
	KillTimer(hMainWnd, TIMER_FREE_FALL);
	SetTimer(hMainWnd, TIMER_FREE_FALL, 1000, NULL);
}

//ブロックのタイプで判別して
//ブロックに色をつける設定
Color Tetris::GetTetriminoColor()
{
	if (m_CurTetriminoType == TetriminoTypes::I)
	{
		return Color::Aqua;
	}

	else if (m_CurTetriminoType == TetriminoTypes::J)
	{
		return Color::Blue;
	}

	else if (m_CurTetriminoType == TetriminoTypes::L)
	{
		return Color::Orange;
	}

	else if (m_CurTetriminoType == TetriminoTypes::O)
	{
		return Color::Yellow;
	}

	else if (m_CurTetriminoType == TetriminoTypes::S)
	{
		return Color::Green;
	}

	else if (m_CurTetriminoType == TetriminoTypes::T)
	{
		return Color::Violet;
	}

	else if (m_CurTetriminoType == TetriminoTypes::Z)
	{
		return Color::Red;
	}

	//どれにも当てはまらない場合壁として認識して色をつける
	else
	{
		return Color::Gray;
	}
}

//ブロックを判別した後、実際人色をつける設定
COLORREF Tetris::GetColor(Color color)
{
	if (color == Color::Aqua)
	{
		return 0x00FFFF00;
	}

	else if (color == Color::Blue)
	{
		return 0x00FF0000;
	}

	else if (color == Color::Green)
	{
		return 0x0000FF00;
	}

	else if (color == Color::Orange)
	{
		return 0x000045FF;
	}

	else if (color == Color::Red)
	{
		return 0x000000FF;
	}

	else if (color == Color::Violet)
	{
		return 0x00800080;
	}

	else if (color == Color::Yellow)
	{
		return 0x0000FFFF;
	}

	else if (color == Color::Gray)
	{
		return 0x00808080;
	}

	else
	{
		return 0x00AAAAAA;
	}
}

// Tetrisクラスのメンバ変数
int m_TetriminoPositionX = 0;
int m_TetriminoPositionY = 0;
int Bytesize = 4;
TetriminoTypes m_CurTetriminoType = TetriminoTypes::I;

//ブロックは最初の段階で回転していない状態で生成する
TetriminoAngle m_TetriminoAngle = TetriminoAngle::Angle0;

//ブロックの形を判別してポジション、サイズを反映する
void Tetris::GetTetriminosPosition(TetriminoPosition pos[], int Blocksize)
{
	// m_TetriminoPositionXとm_TetriminoPositionYが0のときのTetriminoPositionの配列を得る
	if (m_CurTetriminoType == TetriminoTypes::I)
	{
		GetInitBlocksPositionI(pos, Blocksize);
	}

	else if (m_CurTetriminoType == TetriminoTypes::J)
	{
		GetInitBlocksPositionJ(pos, Blocksize);
	}

	else if (m_CurTetriminoType == TetriminoTypes::L)
	{
		GetInitBlocksPositionL(pos, Blocksize);
	}

	else if (m_CurTetriminoType == TetriminoTypes::O)
	{
		GetInitBlocksPositionO(pos, Blocksize);
	}

	else if (m_CurTetriminoType == TetriminoTypes::S)
	{
		GetInitBlocksPositionS(pos, Blocksize);
	}

	else if (m_CurTetriminoType == TetriminoTypes::T)
	{
		GetInitBlocksPositionT(pos, Blocksize);
	}

	else if (m_CurTetriminoType == TetriminoTypes::Z)
	{
		GetInitBlocksPositionZ(pos, Blocksize);
	}

	// 回転させる。Oミノは回転しない。Iミノ以外は3×3、Iミノは4×4なので分けて処理をする
	if (m_CurTetriminoType != TetriminoTypes::I)
	{
		GetInitBlocksRotate33(pos, Blocksize, m_TetriminoAngle);
	}

	else if (m_CurTetriminoType == TetriminoTypes::I)
	{
		GetInitBlocksRotate44(pos, Blocksize, m_TetriminoAngle);
	}

	// m_TetriminoPositionXとm_TetriminoPositionYが現在の値の場合のTetriminoPositionの配列を得る
	for (int i = 0; i < Blocksize; i++)
	{
		pos[i].m_column += m_TetriminoPositionX;
		pos[i].m_row += m_TetriminoPositionY;
	}
}

//ブロックを生成する際の位置の調整に用いる関数
//「I」型のブロックを生成した際の設定
void Tetris::GetInitBlocksPositionI(TetriminoPosition pos[], int size)
{
	if (size != Bytesize)
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

//「J」型のブロックを生成した際の設定
void Tetris::GetInitBlocksPositionJ(TetriminoPosition pos[], int size)
{
	if (size != Bytesize)
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

//「L」型のブロックを生成した際の設定
void Tetris::GetInitBlocksPositionL(TetriminoPosition pos[], int size)
{
	if (size != Bytesize)
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

//「O」型のブロックを生成した際の設定
void Tetris::GetInitBlocksPositionO(TetriminoPosition pos[], int size)
{
	if (size != Bytesize)
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

//「S」型のブロックを生成した際の設定
void Tetris::GetInitBlocksPositionS(TetriminoPosition pos[], int size)
{
	if (size != Bytesize)
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

//「T」型のブロックを生成した際の設定
void Tetris::GetInitBlocksPositionT(TetriminoPosition pos[], int size)
{
	if (size != Bytesize)
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

//「Z」型のブロックを生成した際の設定
void Tetris::GetInitBlocksPositionZ(TetriminoPosition pos[], int size)
{
	if (size != Bytesize)
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

//「I」型のブロックの挙動についての設定
void Tetris::GetInitBlocksRotate33(TetriminoPosition pos[], int size, TetriminoAngle angle)
{

	if (size != Bytesize)
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

//「I」型ブロック以外の設定
void Tetris::GetInitBlocksRotate44(TetriminoPosition pos[], int size, TetriminoAngle angle)
{
	if (size != Bytesize)
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

/// <summary>
/// ブロックを表示するのに用いるメソッド
/// カラー関数から色の情報も取得して反映する
/// </summary>
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

/// <summary>
/// 画面周囲の灰色のブロックを表示するのに用いるメソッド
/// </summary>
void Tetris::DrawOutsideBlocks(HDC hdc)
{
	for (int row = 0; row < ROW_MAX + 1; row++)
	{
		DrawBlock(hdc, -1, row, Color::Gray);
		DrawBlock(hdc, COLUMN_MAX, row, Color::Gray);
	}
	for (int colum = -1; colum < COLUMN_MAX + 1; colum++)
	{
		DrawBlock(hdc, colum, ROW_MAX, Color::Gray);
	}
}

/// <summary>
/// ブロックが着地した後の処理を行うメソッド
/// </summary>
void Tetris::DrawFixedBlock(HDC hdc)
{
	for (int row = 0; row < ROW_MAX; row++)
	{
		for (int column = 0; column < COLUMN_MAX; column++)
		{
			Block* pBlock = m_FixedBlocks[row][column];

			// NULLでなければブロックが存在するということなので描画する
			if (pBlock != NULL)
			{
				DrawBlock(hdc, column, row, pBlock->color);
			}
		}
	}
}

/// <summary>
/// ブロックを移動させるのに用いる判定達
/// </summary>
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

/// <summary>
/// ブロックが着地した際に固定させるメソッド
/// </summary>
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

/// <summary>
/// 新しいブロックを作るメソッド
/// </summary>
void Tetris::NewTetrimino()
{
	m_TetriminoAngle = TetriminoAngle::Angle0;
	m_TetriminoPositionX = 3;
	m_TetriminoPositionY = -1;
	SetNewBlockType();

	// すでに固定されているブロックと衝突していないかの判定
	if (!CanMove(0, 0))
	{
		m_IsGameOver = TRUE;
	}
}

/// <summary>
/// ブロックがそろったか判定を行うメソッド
/// </summary>
void Tetris::CheckLine()
{
	int index = 0;

	// チェックする前に配列m_DeleteLineNumbersを-1で初期化する
	for (int i = 0; i < 4; i++)
	{
		m_DeleteLineNumbers[i] = -1;
	}

	for (int row = 0; row < ROW_MAX; row++)
	{
		BOOL isComplete = TRUE;
		for (int column = 0; column < COLUMN_MAX; column++)
		{
			// 同じ行にひとつでもNULLがあればその部分が空いている
			// すなわちそろっていない
			if (m_FixedBlocks[row][column] == NULL)
			{
				isComplete = FALSE;
				break;
			}
		}
		// ループから抜けたときisComplete == TRUEであればその段はブロックがそろっているといえる
		// その場合は配列にその行の番号を格納する
		if (isComplete)
		{
			m_DeleteLineNumbers[index] = row;
			index++;
		}
	}

	// ブロックを削除する。実際に削除の処理がおこなわれたらDeleteLines関数はTRUEを返す
	BOOL ret = DeleteLines(m_DeleteLineNumbers, 4);
	if (ret)
	{
		// DeleteLines関数が実行されたらウィンドウ全体を再描画する
		// 同時にタイマーをセットして0.1秒後に上の段にあるブロックを下に移動させる
		SetTimer(hMainWnd, TIMER_DROP_LINES, 100, NULL);
		InvalidateRect(hMainWnd, NULL, TRUE);
	}
}

/// <summary>
/// ブロックがそろったら削除するメソッド
/// </summary>
BOOL Tetris::DeleteLines(int lineNumbers[], int size)
{
	BOOL done = FALSE;
	for (int i = 0; i < size; i++)
	{
		// 要素が-1のときはなにもしない
		// なにかしたときはこの関数はTRUEを返す
		int lineNumber = lineNumbers[i];
		if (lineNumber == -1)
		{
			continue;
		}

		done = TRUE;

		for (int x = 0; x < COLUMN_MAX; x++)
		{
			// deleteしたあとNULLを代入。ここにはブロックはないと認識される
			delete m_FixedBlocks[lineNumber][x];
			m_FixedBlocks[lineNumber][x] = NULL;
		}
	}
	return done;
}

/// <summary>
/// 削除が行われているか確認するメソッド
/// 削除してすぐにブロックを落とさない処理があるため
/// </summary>
BOOL Tetris::CheckDeletingLines()
{
	for (int i = 0; i < 4; i++)
	{
		int lineNumber = m_DeleteLineNumbers[i];
		if (lineNumber != -1)
		{
			return TRUE;
		}
	}
	return FALSE;
}

/// <summary>
/// ブロックの回転を判別するメソッド達
/// </summary>
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

	TetriminoPosition tetri[4];

	// m_TetriminoPositionX == 0
	//m_TetriminoPositionY == 0の状態に戻す
	GetTetriminosPosition(tetri, 4);

	for (int i = 0; i < 4; i++)
	{
		tetri[i].m_column -= m_TetriminoPositionX;
		tetri[i].m_row -= m_TetriminoPositionY;
	}

	if (m_CurTetriminoType != TetriminoTypes::I && m_CurTetriminoType != TetriminoTypes::O)
	{
		GetInitBlocksRotate33(tetri, 4, nextAngle);
	}

	else if (m_CurTetriminoType == TetriminoTypes::I)
	{
		GetInitBlocksRotate44(tetri, 4, nextAngle);
	}

	// SRSを適用した場合の回転軸のズレを加える
	for (int i = 0; i < 4; i++)
	{
		tetri[i].m_column += x;
		tetri[i].m_row += y;
	}

	// 現在のm_TetriminoPositionXとm_TetriminoPositionYの分だけ平行移動させる
	for (int i = 0; i < 4; i++)
	{
		tetri[i].m_column += m_TetriminoPositionX;
		tetri[i].m_row += m_TetriminoPositionY;
	}

	// 回転後のブロックの座標を取得
	// 当たり判定をおこなiい
	// フィールドの外に出たり、すでに固定されているブロックがある場合は移動・回転できないようにする
	for (int i = 0; i < 4; i++)
	{
		if (tetri[i].m_column > COLUMN_MAX - 1)
		{
			return FALSE;
		}

		if (tetri[i].m_column < 0)
		{
			return FALSE;
		}

		if (tetri[i].m_row > ROW_MAX - 1)
		{
			return FALSE;
		}

		if (m_FixedBlocks[tetri[i].m_row][tetri[i].m_column] != NULL)
		{
			return FALSE;
		}
	}
	return TRUE;
}

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
			return RotateRight(1, 0);
		if (CanRotate(TRUE, 1, -1))
			return RotateRight(1, -1);
		if (CanRotate(TRUE, 0, 2))
			return RotateRight(0, 2);
		if (CanRotate(TRUE, 1, 2))
			return RotateRight(1, 2);
	}
	else if (m_TetriminoAngle == TetriminoAngle::Angle270)
	{
		if (CanRotate(TRUE, -2, 0))
			return RotateRight(-2, 0);
		if (CanRotate(TRUE, -2, 1))
			return RotateRight(-2, 1);
		if (CanRotate(TRUE, 0, -2))
			return RotateRight(0, -2);
		if (CanRotate(TRUE, -1, -2))
			return RotateRight(-1, -2);
	}
	return FALSE;
}

BOOL Tetris::RotateRightI()
{
	if (CanRotate(TRUE, 0, 0))
		return RotateRight(0, 0);

	if (m_TetriminoAngle == TetriminoAngle::Angle0)
	{
		if (CanRotate(TRUE, -2, 0))
			return RotateRight(-2, 0);
		if (CanRotate(TRUE, 1, 0))
			return RotateRight(1, 0);
		if (CanRotate(TRUE, -2, 1))
			return RotateRight(-2, 1);
		if (CanRotate(TRUE, 1, -2))
			return RotateRight(1, -2);
	}
	else if (m_TetriminoAngle == TetriminoAngle::Angle90)
	{
		if (CanRotate(TRUE, -1, 0))
			return RotateRight(-1, 0);
		if (CanRotate(TRUE, 2, 0))
			return RotateRight(2, 0);
		if (CanRotate(TRUE, -1, -2))
			return RotateRight(-1, -2);
		if (CanRotate(TRUE, 2, 1))
			return RotateRight(2, 1);
	}
	else if (m_TetriminoAngle == TetriminoAngle::Angle180)
	{
		if (CanRotate(TRUE, 2, 0))
			return RotateRight(2, 0);
		if (CanRotate(TRUE, -1, 0))
			return RotateRight(-1, 0);
		if (CanRotate(TRUE, 2, -1))
			return RotateRight(2, -1);
		if (CanRotate(TRUE, -1, 2))
			return RotateRight(-1, 2);
	}
	else if (m_TetriminoAngle == TetriminoAngle::Angle270)
	{
		if (CanRotate(TRUE, -2, 0))
			return RotateRight(-2, 0);
		if (CanRotate(TRUE, 1, 0))
			return RotateRight(1, 0);
		if (CanRotate(TRUE, 1, 2))
			return RotateRight(1, 2);
		if (CanRotate(TRUE, -2, -1))
			return RotateRight(-2, -1);
	}
	return FALSE;
}

BOOL Tetris::RotateRight(int x, int y)
{
	m_TetriminoPositionX += x;
	m_TetriminoPositionY += y;

	if (m_TetriminoAngle == TetriminoAngle::Angle0)
		m_TetriminoAngle = TetriminoAngle::Angle90;
	else if (m_TetriminoAngle == TetriminoAngle::Angle90)
		m_TetriminoAngle = TetriminoAngle::Angle180;
	else if (m_TetriminoAngle == TetriminoAngle::Angle180)
		m_TetriminoAngle = TetriminoAngle::Angle270;
	else if (m_TetriminoAngle == TetriminoAngle::Angle270)
		m_TetriminoAngle = TetriminoAngle::Angle0;

	return TRUE;
}

BOOL Tetris::RotateLeft()
{
	BOOL ret = FALSE;
	if (m_CurTetriminoType != TetriminoTypes::I && m_CurTetriminoType != TetriminoTypes::O)
	{
		ret = RotateLeft3();
		if (ret)
			ResetTimer();
	}
	else if (m_CurTetriminoType == TetriminoTypes::I)
	{
		ret = RotateLeftI();
		if (ret)
			ResetTimer();
	}
	return ret;
}

BOOL Tetris::RotateLeft3()
{
	if (CanRotate(FALSE, 0, 0))
		return RotateLeft(0, 0);

	if (m_TetriminoAngle == TetriminoAngle::Angle0)
	{
		if (CanRotate(FALSE, 1, 0))
			return RotateLeft(1, 0);
		if (CanRotate(FALSE, 1, -1))
			return RotateLeft(1, -1);
		if (CanRotate(FALSE, 0, 2))
			return RotateLeft(0, 2);
		if (CanRotate(FALSE, 1, 2))
			return RotateLeft(1, 2);
	}
	else if (m_TetriminoAngle == TetriminoAngle::Angle90)
	{
		if (CanRotate(FALSE, 1, 0))
			return RotateLeft(1, 0);
		if (CanRotate(FALSE, 1, 1))
			return RotateLeft(1, 1);
		if (CanRotate(FALSE, 0, -2))
			return RotateLeft(0, -2);
		if (CanRotate(FALSE, 1, -2))
			return RotateLeft(1, -2);
	}
	else if (m_TetriminoAngle == TetriminoAngle::Angle180)
	{
		if (CanRotate(FALSE, -1, 0))
			return RotateLeft(-1, 0);
		if (CanRotate(FALSE, -1, -1))
			return RotateLeft(-1, -1);
		if (CanRotate(FALSE, 0, 2))
			return RotateLeft(0, 2);
		if (CanRotate(FALSE, -1, 2))
			return RotateLeft(-1, 2);
	}
	else if (m_TetriminoAngle == TetriminoAngle::Angle270)
	{
		if (CanRotate(FALSE, -1, 0))
			return RotateLeft(-1, 0);
		if (CanRotate(FALSE, -1, 1))
			return RotateLeft(-1, 1);
		if (CanRotate(FALSE, 0, -2))
			return RotateLeft(0, -2);
		if (CanRotate(FALSE, -1, -2))
			return RotateLeft(-1, -2);
	}
	return FALSE;
}

BOOL Tetris::RotateLeftI()
{
	if (CanRotate(FALSE, 0, 0))
		return RotateLeft(0, 0);

	if (m_TetriminoAngle == TetriminoAngle::Angle0)
	{
		if (CanRotate(FALSE, -1, 0))
			return RotateLeft(-1, 0);
		if (CanRotate(FALSE, 2, 0))
			return RotateLeft(2, 0);
		if (CanRotate(FALSE, -2, -2))
			return RotateLeft(-2, -2);
		if (CanRotate(FALSE, 2, 1))
			return RotateLeft(2, 1);
	}
	else if (m_TetriminoAngle == TetriminoAngle::Angle90)
	{
		if (CanRotate(FALSE, 2, 0))
			return RotateLeft(2, 0);
		if (CanRotate(FALSE, -1, 0))
			return RotateLeft(-1, 0);
		if (CanRotate(FALSE, 2, -1))
			return RotateLeft(2, -1);
		if (CanRotate(FALSE, -1, 2))
			return RotateLeft(-1, 2);
	}
	else if (m_TetriminoAngle == TetriminoAngle::Angle180)
	{
		if (CanRotate(FALSE, 1, 0))
			return RotateLeft(1, 0);
		if (CanRotate(FALSE, -2, 0))
			return RotateLeft(-2, 0);
		if (CanRotate(FALSE, 1, 2))
			return RotateLeft(1, 2);
		if (CanRotate(FALSE, -2, -1))
			return RotateLeft(-2, -1);
	}
	else if (m_TetriminoAngle == TetriminoAngle::Angle270)
	{
		if (CanRotate(FALSE, 1, 0))
			return RotateLeft(1, 0);
		if (CanRotate(FALSE, -2, 0))
			return RotateLeft(-2, 0);
		if (CanRotate(FALSE, -2, 1))
			return RotateLeft(-2, 1);
		if (CanRotate(FALSE, 1, -2))
			return RotateLeft(1, -2);
	}
	return FALSE;
}

BOOL Tetris::RotateLeft(int x, int y)
{
	m_TetriminoPositionX += x;
	m_TetriminoPositionY += y;

	if (m_TetriminoAngle == TetriminoAngle::Angle0)
		m_TetriminoAngle = TetriminoAngle::Angle270;
	else if (m_TetriminoAngle == TetriminoAngle::Angle90)
		m_TetriminoAngle = TetriminoAngle::Angle0;
	else if (m_TetriminoAngle == TetriminoAngle::Angle180)
		m_TetriminoAngle = TetriminoAngle::Angle90;
	else if (m_TetriminoAngle == TetriminoAngle::Angle270)
		m_TetriminoAngle = TetriminoAngle::Angle180;

	return TRUE;
}

/// <summary>
/// ブロックをどこまで落下させるか確認するメソッド
/// </summary>
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
				failure = TRUE;

			if (failure)
				break;
		}
		if (!failure)
			count++;
		else
			break;
	}
	return count;
}

/// <summary>
/// ブロックを描画するメソッド
/// 落下してるかを判別するため同時に判別用のブロック(Ghost)も表示する
/// </summary>
void Tetris::Draw(HDC hdc)
{
	DrawGhostBlocks(hdc);

	DrawFixedBlock(hdc);
	DrawOutsideBlocks(hdc);
	DrawTetrimino(hdc);

	ShowScore(hdc);
	ShowNext(hdc);
	ShowHold(hdc);

	if (m_IsGameOver)
	{
		ShowGameOver(hdc);
	}
}

/// <summary>
/// 落下判定が続いている際その場所を表示するメソッド
/// </summary>
void Tetris::DrawGhostBlock(HDC hdc, int column, int row, Color color)
{
	COLORREF colorref = GetGhostColor(color);

	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
	HBRUSH hBrush = CreateSolidBrush(colorref);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

	int x = column * BLOCK_SIZE + LeftTopBlockPosition.x;
	int y = row * BLOCK_SIZE + LeftTopBlockPosition.y;
	::Rectangle(hdc, x, y, x + BLOCK_SIZE, y + BLOCK_SIZE);

	SelectObject(hdc, hOldPen);
	SelectObject(hdc, hOldBrush);

	DeleteObject(hPen);
	DeleteObject(hBrush);
}

/// <summary>
/// ブロックの落下する位置を判別するメソッド
/// </summary>
void Tetris::DrawGhostBlocks(HDC hdc)
{
	for (int row = 0; row < ROW_MAX + 1; row++)
	{
		DrawBlock(hdc, -1, row, Color::Gray);
		DrawBlock(hdc, COLUMN_MAX, row, Color::Gray);
	}

	for (int colum = -1; colum < COLUMN_MAX + 1; colum++)
	{
		DrawBlock(hdc, colum, ROW_MAX, Color::Gray);
	}

	Block blocks[4];
	GetGhostBlocks(blocks, 4);
	for (int i = 0; i < 4; i++)
		DrawGhostBlock(hdc, blocks[i].m_column, blocks[i].m_row, blocks[i].color);
}

/// <summary>
/// ブロックを表示するメソッド
/// 落下時にブロックがあるなら上に乗せるように表示する
/// </summary>
void Tetris::DrawTetrimino(HDC hdc)
{
	TetriminoPosition position[4];

	// 現在のテトロミノの位置と色を取得する
	GetTetriminosPosition(position, 4);
	Color color = GetTetriminoColor();

	// 取得した位置をDrawBlock関数にわたして描画する
	for (int i = 0; i < 4; i++)
		DrawBlock(hdc, position[i].m_column, position[i].m_row, color);

	for (int row = 0; row < ROW_MAX; row++)
	{
		for (int column = 0; column < COLUMN_MAX; column++)
		{
			Block* pBlock = m_FixedBlocks[row][column];

			// NULLでなければブロックが存在するということなので描画する
			if (pBlock != NULL)
			{
				DrawBlock(hdc, column, row, pBlock->color);
			}
		}
	}
}

/// <summary>
/// ブロック位置を判別するメソッド
/// </summary>
void Tetris::GetGhostBlocks(Block* blocks, int size)
{
	TetriminoPosition pos[4];
	GetTetriminosPosition(pos, 4);
	int fallCount = GetHardDropCount();

	for (int i = 0; i < 4; i++)
	{
		blocks[i].m_column = pos[i].m_column;
		blocks[i].m_row = pos[i].m_row + fallCount;
		blocks[i].color = GetTetriminoColor(); // 落下中のテトロミノの色を格納しておく
	}
}

/// <summary>
/// 判定用のブロックの色情報を取得
/// </summary>
COLORREF Tetris::GetGhostColor(Color color)
{
	if (color == Color::Aqua)
		return 0x00404000;
	else if (color == Color::Blue)
		return 0x00800000;
	else if (color == Color::Green)
		return 0x00004000;
	else if (color == Color::Orange)
		return 0x00001040;
	else if (color == Color::Red)
		return 0x00000040;
	else if (color == Color::Violet)
		return 0x00200020;
	else if (color == Color::Yellow)
		return 0x00004040;
	else
		return 0x00808080;
}

/// <summary>
/// 外画面の点滅を抑えるメソッド
/// </summary>
void Tetris::GetRect(RECT* pRect)
{
	pRect->left = LeftTopBlockPosition.x;
	pRect->top = LeftTopBlockPosition.y;
	pRect->right = LeftTopBlockPosition.x + BLOCK_SIZE * COLUMN_MAX;
	pRect->bottom = LeftTopBlockPosition.y + BLOCK_SIZE * ROW_MAX;
}

/// <summary>
/// ブロックの急降下を行うメソッド
/// </summary>
BOOL Tetris::HardDrop()
{
	int fallCount = GetHardDropCount();
	m_Score += fallCount * 2; //　スコア追加

	m_TetriminoPositionY += fallCount;
	InvalidateRect(hMainWnd, NULL, TRUE);
	FixBlocks();
	return TRUE;
}

/// <summary>
/// ブロックが着地した際に行うメソッド
/// </summary>
void Tetris::DropLines()
{
	int count = 0;
	for (int i = 0; i < 4; i++)
	{
		// 配列 m_DeleteLineNumbersのなかの-1以外の値がある場合、
		// その行は上にあるブロックで詰められる
		int lineNumber = m_DeleteLineNumbers[i];
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
		m_DeleteLineNumbers[i] = -1;
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

/// <summary>
/// ブロックを保管する際に行うメソッド
/// </summary>
void Tetris::CreateBag()
{
	if (m_vectorTetriminoTypes.size() > 7)
	{
		return;
	}

	vector<TetriminoTypes> v;
	v.push_back(TetriminoTypes::I);
	v.push_back(TetriminoTypes::J);
	v.push_back(TetriminoTypes::L);
	v.push_back(TetriminoTypes::O);
	v.push_back(TetriminoTypes::S);
	v.push_back(TetriminoTypes::T);
	v.push_back(TetriminoTypes::Z);

	while (true)
	{
		int size = v.size();
		if (size == 0)
			break;
		int type = rand() % size;
		TetriminoTypes selectedType = v[type];
		m_vectorTetriminoTypes.push_back(selectedType);
		v.erase(v.begin() + type);
	}
}

/// <summary>
/// 新しいブロックを判定するメソッド
/// </summary>
void Tetris::SetNewBlockType()
{
	if (m_IsGameOver)
	{
		return;
	}

	m_CurTetriminoType = m_vectorTetriminoTypes[0];
	m_vectorTetriminoTypes.erase(m_vectorTetriminoTypes.begin());

	CreateBag();
	InvalidateRect(hMainWnd, NULL, TRUE);
}

/// <summary>
/// 新しいブロックを次のブロックとして表示するメソッド
/// </summary>
void Tetris::ShowNext(HDC hdc)
{
	// 文字列「ＮＥＸＴ」を表示
	SetTextColor(hdc, RGB(255, 255, 255));
	HFONT hFont = SetScoreFont(hdc, 16);
	HFONT hFontOld = (HFONT)SelectObject(hdc, hFont);
	const WCHAR* str = L"ＮＥＸＴ";

	TextOut(hdc, 400, 50, str, lstrlen(str));
	SelectObject(hdc, hFontOld);
	DeleteObject(hFont);

	// テトリミノを表示する場所を決める
	POINT leftTop;
	leftTop.x = 400;
	leftTop.y = 100;
	DrawNextBlock(hdc, leftTop, m_vectorTetriminoTypes[0]);

	leftTop.y = 180;
	DrawNextBlock(hdc, leftTop, m_vectorTetriminoTypes[1]);

	leftTop.y = 260;
	DrawNextBlock(hdc, leftTop, m_vectorTetriminoTypes[2]);

	leftTop.y = 340;
	DrawNextBlock(hdc, leftTop, m_vectorTetriminoTypes[3]);
}

/// <summary>
/// 新しいブロックの表示を行うメソッド
/// </summary>
void Tetris::DrawNextBlock(HDC hdc, POINT leftTpo, TetriminoTypes tetriminoType)
{
	COLORREF colorref = GetColor(GetTetriminoColor(tetriminoType));
	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
	HBRUSH hBrush = CreateSolidBrush(colorref);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

	int columns[4];
	int rows[4];
	GetColumnsRowsNextBlock(tetriminoType, columns, rows, 4);

	int blockSize = 16; // フィールドに表示されているもの
	for (int i = 0; i < 4; i++)
	{
		int x = columns[i] * blockSize + leftTpo.x;
		int y = rows[i] * blockSize + leftTpo.y;
		::Rectangle(hdc, x, y, x + blockSize, y + blockSize);
	}

	SelectObject(hdc, hOldPen);
	SelectObject(hdc, hOldBrush);
	DeleteObject(hPen);
	DeleteObject(hBrush);
}

/// <summary>
/// 次のブロックの判別を行うメソッド
/// </summary>
void Tetris::GetColumnsRowsNextBlock(TetriminoTypes tetriminoType, int columns[], int rows[], int size)
{
	if (size != 4)
	{
		return;
	}

	if (tetriminoType == TetriminoTypes::I)
	{
		columns[0] = 0;
		columns[1] = 1;
		columns[2] = 2;
		columns[3] = 3;
		rows[0] = 1;
		rows[1] = 1;
		rows[2] = 1;
		rows[3] = 1;
		return;
	}
	if (tetriminoType == TetriminoTypes::J)
	{
		columns[0] = 0;
		columns[1] = 0;
		columns[2] = 1;
		columns[3] = 2;
		rows[0] = 0;
		rows[1] = 1;
		rows[2] = 1;
		rows[3] = 1;
		return;
	}
	if (tetriminoType == TetriminoTypes::L)
	{
		columns[0] = 0;
		columns[1] = 1;
		columns[2] = 2;
		columns[3] = 2;
		rows[0] = 1;
		rows[1] = 1;
		rows[2] = 1;
		rows[3] = 0;
		return;
	}
	if (tetriminoType == TetriminoTypes::O)
	{
		columns[0] = 0;
		columns[1] = 0;
		columns[2] = 1;
		columns[3] = 1;
		rows[0] = 0;
		rows[1] = 1;
		rows[2] = 0;
		rows[3] = 1;
		return;
	}
	if (tetriminoType == TetriminoTypes::S)
	{
		columns[0] = 0;
		columns[1] = 1;
		columns[2] = 1;
		columns[3] = 2;
		rows[0] = 1;
		rows[1] = 0;
		rows[2] = 1;
		rows[3] = 0;
		return;
	}
	if (tetriminoType == TetriminoTypes::T)
	{
		columns[0] = 0;
		columns[1] = 1;
		columns[2] = 1;
		columns[3] = 2;
		rows[0] = 1;
		rows[1] = 0;
		rows[2] = 1;
		rows[3] = 1;
		return;
	}
	if (tetriminoType == TetriminoTypes::Z)
	{
		columns[0] = 0;
		columns[1] = 1;
		columns[2] = 1;
		columns[3] = 2;
		rows[0] = 0;
		rows[1] = 0;
		rows[2] = 1;
		rows[3] = 1;
		return;
	}
}

/// <summary>
/// ブロックの形に応じて色を判別するメソッド
/// </summary>
Color Tetris::GetTetriminoColor(TetriminoTypes tetriminoType)
{
	if (tetriminoType == TetriminoTypes::I)
	{
		return Color::Aqua;
	}

	else if (tetriminoType == TetriminoTypes::J)
	{
		return Color::Blue;
	}

	else if (tetriminoType == TetriminoTypes::L)
	{
		return Color::Orange;
	}

	else if (tetriminoType == TetriminoTypes::O)
	{
		return Color::Yellow;
	}

	else if (tetriminoType == TetriminoTypes::S)
	{
		return Color::Green;
	}

	else if (tetriminoType == TetriminoTypes::T)
	{
		return Color::Violet;
	}

	else if (tetriminoType == TetriminoTypes::Z)
	{
		return Color::Red;
	}

	else
	{
		return Color::Gray;
	}
}

/// <summary>
/// ブロックが動く際に判別するメソッド
/// ダメなら動かさないように返す
/// </summary>
BOOL Tetris::CanMove(int x, int y)
{
	// 移動先のTetriminoPositionの配列を取得する
	TetriminoPosition pos[4];
	GetTetriminosPosition(pos, 4);

	// 外に出たり固定されているブロックとぶつかってしまう場合は移動不可とする
	for (int i = 0; i < 4; i++)
	{
		if (pos[i].m_column + x > COLUMN_MAX - 1)
		{
			return FALSE;
		}

		if (pos[i].m_column + x < 0)
		{
			return FALSE;
		}

		if (pos[i].m_row + y > ROW_MAX - 1)
		{
			return FALSE;
		}

		if (pos[i].m_row + y < 0) 
		{
			continue;
		}

		if (m_FixedBlocks[pos[i].m_row + y][pos[i].m_column + x] != NULL)
		{
			return FALSE;
		}
	}
	return TRUE;
}

/// <summary>
/// 保管しているブロックを表示するメソッド
/// </summary>
void Tetris::ShowHold(HDC hdc)
{
	// 文字列「ＨＯＬＤ」を表示
	SetTextColor(hdc, RGB(255, 255, 255));
	HFONT hFont = SetScoreFont(hdc, 16);
	HFONT hFontOld = (HFONT)SelectObject(hdc, hFont);
	const WCHAR* str = L"ＨＯＬＤ";

	TextOut(hdc, 30, 50, str, lstrlen(str));
	SelectObject(hdc, hFontOld);
	DeleteObject(hFont);

	POINT leftTop;
	leftTop.x = 40;
	leftTop.y = 100;
	if (m_holdTetrimino != TetriminoTypes::None)
		DrawNextBlock(hdc, leftTop, m_holdTetrimino);
}

/// <summary>
/// ブロックを保管する際に行うメソッド
/// </summary>
BOOL Tetris::Hold()
{
	// ホールドできない場合はなにもしない
	if (m_CanHold == FALSE)
	{
		return FALSE;
	}

	// ホールドしたら設置されるまでホールドはできなくなる
	m_CanHold = FALSE;

	// なにもホールドされていない場合は落下中のテトロミノをホールドして次のテトリミノを降らせる
	// すでになにかがホールドされている場合は落下中のテトロミノをホールドして
	// いままでホールドされていたテトリミノを一番上から降らせる
	if (m_holdTetrimino == TetriminoTypes::None)
	{
		m_holdTetrimino = m_CurTetriminoType;
		NewTetrimino();
	}

	else
	{
		TetriminoTypes oldHold = m_holdTetrimino;
		m_holdTetrimino = m_CurTetriminoType;
		m_TetriminoAngle = TetriminoAngle::Angle0;
		m_TetriminoPositionX = 3;
		m_TetriminoPositionY = -1;

		m_CurTetriminoType = oldHold;
	}
	InvalidateRect(hMainWnd, NULL, TRUE);
	return TRUE;
}

/// <summary>
/// ゲームの失敗を判別する判定
/// </summary>
BOOL Tetris::IsGameOver()
{
	return m_IsGameOver;
}

/// <summary>
/// ゲームのリトライを行うメソッド
/// </summary>
void Tetris::Retry()
{
	m_Score = 0;
	m_CanHold = TRUE;
	m_holdTetrimino = TetriminoTypes::None;
	m_IsGameOver = FALSE;
	m_vectorTetriminoTypes.clear();

	Init();
}

/// <summary>
/// ゲームが失敗したことを表示するメソッド
/// </summary>
void Tetris::ShowGameOver(HDC hdc)
{
	HFONT hFont = SetScoreFont(hdc, 20);
	HFONT hFontOld = (HFONT)SelectObject(hdc, hFont);
	SetTextColor(hdc, RGB(255, 255, 255));

	const WCHAR* str1 = L"ＧＡＭＥ ＯＶＥＲ";
	TextOut(hdc, 160, 100, str1, lstrlen(str1));
	SelectObject(hdc, hFontOld);
	DeleteObject(hFont);

	hFont = SetScoreFont(hdc, 15);
	hFontOld = (HFONT)SelectObject(hdc, hFont);
	const WCHAR* str2 = L"ＰＲＥＳＳ Ｓ ＫＥＹ";
	TextOut(hdc, 170, 130, str2, lstrlen(str2));
	SelectObject(hdc, hFontOld);
	DeleteObject(hFont);
}

/// <summary>
/// スコアの文字を表示する際に行うメソッド
/// </summary>
HFONT Tetris::SetScoreFont(HDC hdc, int height)
{
	HFONT hFont;
	hFont = CreateFont(height, // フォント高さ
		0, 0, 0,    // 文字幅、テキストの角度、ベースラインとｘ軸との角度
		FW_BOLD, FALSE, FALSE, FALSE, // フォントの太さ、イタリック体、アンダーライン、打ち消し線
		SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,  //文字セット、出力精度、クリッピング精度
		PROOF_QUALITY, FIXED_PITCH | FF_MODERN, // 出力品質、ピッチとファミリー
		L"ＭＳ ゴシック");    //書体名
	return hFont;
};

/// <summary>
/// ゲーム内のスコアを表示するメソッド
/// </summary>
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