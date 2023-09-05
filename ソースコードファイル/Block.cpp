
#include "Tetris.h"
#include "MainWindow.h"


//�u���b�N��V������������ۂ̏���
void Tetris::NewTetrimino()
{
	m_TetriminoAngle = TetriminoAngle::Angle0;
	m_TetriminoPositionX = 3;
	m_TetriminoPositionY = -1;
	SetNewBlockType();

	//���łɌŒ肳��Ă���u���b�N�ƏՓ˂��Ă���ꍇ�Q�[���̏I��������o��
	if (!CanMove(0, 0))
	{
		m_IsGameOver = TRUE;
	}
}

/// <summary>
/// �u���b�N�ɍ��킹���z�u�����߂�
/// </summary>
/// <param name="pos"></param>
/// <param name="size"></param>

void Tetris::GetTetriminosPosition(TetriminoPosition pos[], int size)
{
	// m_TetriminoPositionX��m_TetriminoPositionY��0�̂Ƃ���TetriminoPosition�̔z��𓾂�
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
	// ��]������BO�~�m�͉�]���Ȃ��BI�~�m�ȊO��3�~3�AI�~�m��4�~4�Ȃ̂ŕ����ď���������
	if (m_CurTetriminoType != TetriminoTypes::I)
	{
		GetInitBlocksRotate33(pos, size, m_TetriminoAngle);
	}
	else if (m_CurTetriminoType == TetriminoTypes::I)
	{
		GetInitBlocksRotate44(pos, size, m_TetriminoAngle);
	}

	// m_TetriminoPositionX��m_TetriminoPositionY�����݂̒l�̏ꍇ��TetriminoPosition�̔z��𓾂�
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

//�u���b�N�̕`����s�����\�b�h
void Tetris::DrawBlock(HDC hdc, int column, int row, Color color)
{
	if (row < 0)
	{
		return;
	}

	COLORREF colorref = GetColor(color);

	// �y���ƃu���V�𐶐��E�I��
	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
	HBRUSH hBrush = CreateSolidBrush(colorref);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

	// 0�s0��̈ʒu�������E�B���h�E�̓����Ɉړ�������
	int x = column * BLOCK_SIZE + LeftTopBlockPosition.x;
	int y = row * BLOCK_SIZE + LeftTopBlockPosition.y;
	::Rectangle(hdc, x, y, x + BLOCK_SIZE, y + BLOCK_SIZE);

	// �y���ƃu���V�����Ƃɖ߂�
	SelectObject(hdc, hOldPen);
	SelectObject(hdc, hOldBrush);

	// �����ō쐬�����y���ƃu���V���폜����
	DeleteObject(hPen);
	DeleteObject(hBrush);
}

//���ɐ��������u���b�N�̕`����s��
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

//�u���b�N�̕`����s����������
void Tetris::DrawFixedBlock(HDC hdc)
{
	for (int row = 0; row < ROW_MAX; row++)
	{
		for (int column = 0; column < COLUMN_MAX; column++)
		{
			Block* pBlock = m_FixedBlocks[row][column];

			// NULL�łȂ���΃u���b�N�����݂���Ƃ������ƂȂ̂ŕ`�悷��
			if (pBlock != NULL)
				DrawBlock(hdc, column, row, pBlock->color);
		}
	}
}

//�u���b�N�̐������s������
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
