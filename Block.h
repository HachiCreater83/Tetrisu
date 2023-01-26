#pragma once
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

class TetriminoPosition
{
public:
    int m_column = 0;
    int m_row = 0;
};

enum class TetriminoAngle
{
    Angle0,
    Angle90,
    Angle180,
    Angle270,
};

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

class Block
{
public:
    int m_column = 0;
    int m_row = 0;
    Color color = Color::Aqua;
};