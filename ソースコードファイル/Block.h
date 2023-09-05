
#pragma once

//�u���b�N�𔻕ʂ���F
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

//�u���b�N�̈ʒu���
class TetriminoPosition
{
public:
	//�u���b�N�̈ʒu
	int m_column = 0;
	int m_row = 0;

};

//�u���b�N�̉�]���
enum class TetriminoAngle
{
	Angle0,
	Angle90,
	Angle180,
	Angle270,
};

//�o�ꂷ��u���b�N�^�C�v
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

//�u���b�N���\������N���X
//�ʒu����F�̐ݒ�
class Block
{
public:
	int m_column = 0;
	int m_row = 0;

	Color color = Color::Aqua;

	// Tetris�N���X�̃����o�ϐ�
	int m_TetriminoPositionX = 0;
	int m_TetriminoPositionY = 0;
	TetriminoTypes m_CurTetriminoType = TetriminoTypes::I;
	TetriminoAngle m_TetriminoAngle = TetriminoAngle::Angle0;

};
