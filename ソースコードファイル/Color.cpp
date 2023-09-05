
#include "Tetris.h"


//色とブロックを関係づける
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
	else
	{
		return Color::Gray;
	}
}

//ブロックの色情報を設定
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
