
#pragma once

//ブロックを判別する色
enum class Color
{
	Aqua, // Iミノ（水色）
	Yellow, // Oミノ（黄色）
	Green, // Sミノ（緑）
	Red, // Zミノ（赤）
	Blue, // Jミノ（青）
	Orange, // Lミノ（オレンジ）
	Violet, // Tミノ（紫）
	Gray, // 外枠用
};

//ブロックの位置情報
class TetriminoPosition
{
public:
	//ブロックの位置
	int m_column = 0;
	int m_row = 0;

};

//ブロックの回転情報
enum class TetriminoAngle
{
	Angle0,
	Angle90,
	Angle180,
	Angle270,
};

//登場するブロックタイプ
enum class TetriminoTypes
{
	I,
	O,
	S,
	Z,
	J,
	L,
	T,
	None,
};

//ブロックを構成するクラス
//位置情報や色の設定
class Block
{
public:
	int m_column = 0;
	int m_row = 0;

	Color color = Color::Aqua;

	// Tetrisクラスのメンバ変数
	int m_TetriminoPositionX = 0;
	int m_TetriminoPositionY = 0;
	TetriminoTypes m_CurTetriminoType = TetriminoTypes::I;
	TetriminoAngle m_TetriminoAngle = TetriminoAngle::Angle0;

};
