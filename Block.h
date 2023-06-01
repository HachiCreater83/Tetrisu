
#pragma once

//�u���b�N�̐F���w�肷��N���X
enum class Color
{
    Aqua, // I�~�m�i���F�j
    Yellow, // O�~�m�i���F�j
    Green, // S�~�m�i�΁j
    Red, // Z�~�m�i�ԁj
    Blue, // J�~�m�i�j
    Orange, // L�~�m�i�I�����W�j
    Violet, // T�~�m�i���j
    Gray, // �O�g�p
};

//�u���b�N�̏����|�W�V�����̐ݒ肷��N���X
class TetriminoPosition
{
public:
    int m_column = 0;
    int m_row = 0;
};

//�u���b�N����]�����ۂ̊p�x���w�肷��N���X
enum class TetriminoAngle
{
    Angle0,
    Angle90,
    Angle180,
    Angle270,
};

//�u���b�N�̌`���\������N���X
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

//�u���b�N�����\������N���X
class Block
{
public:
    int m_column = 0;
    int m_row = 0;
    Color color = Color::Aqua;
};