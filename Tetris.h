#pragma once

// 10列20行
#define COLUMN_MAX 10
#define ROW_MAX 20
#define BLOCK_SIZE 20

class Tetris
{
	// 初期化
public:
	Tetris();
	void Init();

private:
	void InitFixedBlocks();

	// バッグをつくるために必要な変数と関数
private:
	vector<TetriminoTypes> m_vectorTetriminoTypes;
	void CreateBag();

private:
	void ResetTimer();

	// 落下中のテトロミノの位置・状態を保存する変数
private:
	int m_TetriminoPositionX = 0;
	int m_TetriminoPositionY = 0;
	TetriminoTypes m_CurTetriminoType = TetriminoTypes::I;
	TetriminoAngle m_TetriminoAngle = TetriminoAngle::Angle0;

	// 落下中のテトロミノの位置の取得関連
private:
	void GetTetriminosPosition(TetriminoPosition tetri[], int size);
	void GetInitBlocksPositionI(TetriminoPosition tetri[], int size);
	void GetInitBlocksPositionJ(TetriminoPosition tetri[], int size);
	void GetInitBlocksPositionL(TetriminoPosition tetri[], int size);
	void GetInitBlocksPositionO(TetriminoPosition tetri[], int size);
	void GetInitBlocksPositionS(TetriminoPosition tetri[], int size);
	void GetInitBlocksPositionZ(TetriminoPosition tetri[], int size);
	void GetInitBlocksPositionT(TetriminoPosition tetri[], int size);
	void GetInitBlocksRotate33(TetriminoPosition tetri[], int size, TetriminoAngle angle);
	void GetInitBlocksRotate44(TetriminoPosition tetri[], int size, TetriminoAngle angle);

	// 表示色取得関連
private:
	COLORREF GetColor(Color color);
	Color GetTetriminoColor();

	// 描画関連
public:
	POINT LeftTopBlockPosition;

	void Draw(HDC hdc);
	void GetRect(RECT* pRect);

private:
	void DrawBlock(HDC hdc, int column, int row, Color color);
	void DrawOutsideBlocks(HDC hdc);
	void DrawTetrimino(HDC hdc);
	void DrawFixedBlock(HDC hdc);

	// 移動関連
public:
	BOOL m_IsSoftDrop = FALSE;

	BOOL MoveLeft();
	BOOL MoveRight();
	BOOL MoveUp();
	BOOL MoveDown();

private:
	BOOL CanMove(int x, int y);

	// 回転関連
public:
	BOOL RotateRight();
	BOOL RotateLeft();

private:
	BOOL CanRotate(BOOL isRight, int x, int y);

	BOOL RotateRight(int x, int y);
	BOOL RotateRight3();
	BOOL RotateRightI();

	BOOL RotateLeft(int x, int y);
	BOOL RotateLeft3();
	BOOL RotateLeftI();

	// ラインの消去関連
public:
	void DropLines();

private:
	Block* m_FixedBlocks[ROW_MAX][COLUMN_MAX];
	int m_DeleteLineNumbers[4];
	BOOL CheckDeletingLines();

	void FixBlocks();
	void CheckLine();
	BOOL DeleteLines(int lineNumbers[], int size);

	void NewTetrimino();
	void SetNewBlockType();

public:
	BOOL HardDrop();
	void DrawGhostBlocks(HDC hdc);

private:
	int GetHardDropCount();
	COLORREF GetGhostColor(Color color);
	void GetGhostBlocks(Block* blocks, int size);
	void DrawGhostBlock(HDC hdc, int column, int row, Color color);

	// スコア表示のためのメンバ変数とメンバ関数
private:
	int m_Score = 0;
	void ShowScore(HDC hdc);
	HFONT SetScoreFont(HDC hdc, int height);

	// NEXTブロックの表示をするための関数
private:
	void ShowNext(HDC hdc);
	void DrawNextBlock(HDC hdc, POINT leftTpo, TetriminoTypes tetriminoType);
	void GetColumnsRowsNextBlock(TetriminoTypes tetriminoType, int columns[], int rows[], int size);
	Color GetTetriminoColor(TetriminoTypes tetriminoType);

	// ホールドに関するメンバ変数とメンバ関数
private:
	TetriminoTypes m_holdTetrimino;
	BOOL m_CanHold = TRUE;

	void ShowHold(HDC hdc);
public:
	BOOL Hold();

	// ゲームオーバーとゲーム再挑戦の処理
private:
	BOOL m_IsGameOver = FALSE;
	void ShowGameOver(HDC hdc);

public:
	BOOL IsGameOver();
	void Retry();
};






