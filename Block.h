
#pragma once

//ブロックの色を指定するクラス
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

//ブロックの初期ポジションの設定するクラス
class TetriminoPosition
{
public:
    int m_column = 0;
    int m_row = 0;
};

//ブロックが回転した際の角度を指定するクラス
enum class TetriminoAngle
{
    Angle0,
    Angle90,
    Angle180,
    Angle270,
};

//ブロックの形を構成するクラス
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

//ブロック情報を構成するクラス
class Block
{
public:
    int m_column = 0;
    int m_row = 0;
    Color color = Color::Aqua;
};