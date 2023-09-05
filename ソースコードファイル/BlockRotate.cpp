
#include "Tetris.h"

//�u���b�N����]�����鏈��
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
	TetriminoPosition BlockSize[4];

	//m_TetriminoPositionX == 0 m_TetriminoPositionY == 0�̏�Ԃɖ߂��čl����
	GetTetriminosPosition(BlockSize, _fieldTetriminoPosition);
	for (int i = 0; i < _fieldTetriminoPosition; i++)
	{
		BlockSize[i].m_column -= m_TetriminoPositionX;
		BlockSize[i].m_row -= m_TetriminoPositionY;
	}

	if (m_CurTetriminoType != TetriminoTypes::I && m_CurTetriminoType != TetriminoTypes::O)
	{
		GetInitBlocksRotate33(BlockSize, _fieldTetriminoPosition, nextAngle);
	}
	else if (m_CurTetriminoType == TetriminoTypes::I)
	{
		GetInitBlocksRotate44(BlockSize, _fieldTetriminoPosition, nextAngle);
	}

	//��]���̃Y����������
	for (int i = 0; i < _fieldTetriminoPosition; i++)
	{
		BlockSize[i].m_column += x;
		BlockSize[i].m_row += y;
	}

	//���݂�m_TetriminoPositionX��m_TetriminoPositionY�̕��������s�ړ�������
	for (int i = 0; i < _fieldTetriminoPosition; i++)
	{
		BlockSize[i].m_column += m_TetriminoPositionX;
		BlockSize[i].m_row += m_TetriminoPositionY;
	}

	//�����ŉ�]��̃u���b�N�̍��W���擾
	//��͓����蔻��������Ȃ�
	//�t�B�[���h�̊O�ɏo����A���łɌŒ肳��Ă���u���b�N������ꍇ�͈ړ��E��]�ł��Ȃ�
	for (int i = 0; i < _fieldTetriminoPosition; i++)
	{
		if (BlockSize[i].m_column > COLUMN_MAX - 1)
		{
			return FALSE;
		}
		if (BlockSize[i].m_column < 0)
		{
			return FALSE;
		}
		if (BlockSize[i].m_row > ROW_MAX - 1)
		{
			return FALSE;
		}
		if (m_FixedBlocks[BlockSize[i].m_row][BlockSize[i].m_column] != NULL)
		{
			return FALSE;
		}
	}
	return TRUE;
}

//�E��]�����ۂ̃u���b�N�̗������~�߂�
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

//��{�I�ȃu���b�N�̉E��]����
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
		{
			return RotateRight(1, 0);
		}
		if (CanRotate(TRUE, 1, -1))
		{
			return RotateRight(1, -1);
		}
		if (CanRotate(TRUE, 0, 2))
		{
			return RotateRight(0, 2);
		}
		if (CanRotate(TRUE, 1, 2))
		{
			return RotateRight(1, 2);
		}
	}
	else if (m_TetriminoAngle == TetriminoAngle::Angle270)
	{
		if (CanRotate(TRUE, -2, 0))
		{
			return RotateRight(-2, 0);
		}
		if (CanRotate(TRUE, -2, 1))
		{
			return RotateRight(-2, 1);
		}
		if (CanRotate(TRUE, 0, -2))
		{
			return RotateRight(0, -2);
		}
		if (CanRotate(TRUE, -1, -2))
		{
			return RotateRight(-1, -2);
		}
	}
	return FALSE;
}

//�_�u���b�N�̉E��]����
BOOL Tetris::RotateRightI()
{
	if (CanRotate(TRUE, 0, 0))
	{
		return RotateRight(0, 0);
	}

	if (m_TetriminoAngle == TetriminoAngle::Angle0)
	{
		if (CanRotate(TRUE, -2, 0))
		{
			return RotateRight(-2, 0);
		}
		if (CanRotate(TRUE, 1, 0))
		{
			return RotateRight(1, 0);
		}
		if (CanRotate(TRUE, -2, 1))
		{
			return RotateRight(-2, 1);
		}
		if (CanRotate(TRUE, 1, -2))
		{
			return RotateRight(1, -2);
		}
	}
	else if (m_TetriminoAngle == TetriminoAngle::Angle90)
	{
		if (CanRotate(TRUE, -1, 0))
		{
			return RotateRight(-1, 0);
		}
		if (CanRotate(TRUE, 2, 0))
		{
			return RotateRight(2, 0);
		}
		if (CanRotate(TRUE, -1, -2))
		{
			return RotateRight(-1, -2);
		}
		if (CanRotate(TRUE, 2, 1))
		{
			return RotateRight(2, 1);
		}
	}
	else if (m_TetriminoAngle == TetriminoAngle::Angle180)
	{
		if (CanRotate(TRUE, 2, 0))
		{
			return RotateRight(2, 0);
		}
		if (CanRotate(TRUE, -1, 0))
		{
			return RotateRight(-1, 0);
		}
		if (CanRotate(TRUE, 2, -1))
		{
			return RotateRight(2, -1);
		}
		if (CanRotate(TRUE, -1, 2))
		{
			return RotateRight(-1, 2);
		}
	}
	else if (m_TetriminoAngle == TetriminoAngle::Angle270)
	{
		if (CanRotate(TRUE, -2, 0))
		{
			return RotateRight(-2, 0);
		}
		if (CanRotate(TRUE, 1, 0))
		{
			return RotateRight(1, 0);
		}
		if (CanRotate(TRUE, 1, 2))
		{
			return RotateRight(1, 2);
		}
		if (CanRotate(TRUE, -2, -1))
		{
			return RotateRight(-2, -1);
		}
	}
	return FALSE;
}

//�E��]���̉�]�̈ړ�����
BOOL Tetris::RotateRight(int x, int y)
{
	m_TetriminoPositionX += x;
	m_TetriminoPositionY += y;

	if (m_TetriminoAngle == TetriminoAngle::Angle0)
	{
		m_TetriminoAngle = TetriminoAngle::Angle90;
	}
	else if (m_TetriminoAngle == TetriminoAngle::Angle90)
	{
		m_TetriminoAngle = TetriminoAngle::Angle180;
	}
	else if (m_TetriminoAngle == TetriminoAngle::Angle180)
	{
		m_TetriminoAngle = TetriminoAngle::Angle270;
	}
	else if (m_TetriminoAngle == TetriminoAngle::Angle270)
	{
		m_TetriminoAngle = TetriminoAngle::Angle0;
	}
	return TRUE;
}

//����]�����ۂ̃u���b�N�̗������~�߂�
BOOL Tetris::RotateLeft()
{
	BOOL ret = FALSE;
	if (m_CurTetriminoType != TetriminoTypes::I && m_CurTetriminoType != TetriminoTypes::O)
	{
		ret = RotateLeft3();
		if (ret)
		{
			ResetTimer();
		}
	}
	else if (m_CurTetriminoType == TetriminoTypes::I)
	{
		ret = RotateLeftI();
		if (ret)
		{
			ResetTimer();
		}
	}
	return ret;
}

//��{�I�ȃu���b�N�̍���]����
BOOL Tetris::RotateLeft3()
{
	if (CanRotate(FALSE, 0, 0))
	{
		return RotateLeft(0, 0);
	}
	if (m_TetriminoAngle == TetriminoAngle::Angle0)
	{
		if (CanRotate(FALSE, 1, 0))
		{
			return RotateLeft(1, 0);
		}
		if (CanRotate(FALSE, 1, -1))
		{
			return RotateLeft(1, -1);
		}
		if (CanRotate(FALSE, 0, 2))
		{
			return RotateLeft(0, 2);
		}
		if (CanRotate(FALSE, 1, 2))
		{
			return RotateLeft(1, 2);
		}
	}
	else if (m_TetriminoAngle == TetriminoAngle::Angle90)
	{
		if (CanRotate(FALSE, 1, 0))
		{
			return RotateLeft(1, 0);
		}
		if (CanRotate(FALSE, 1, 1))
		{
			return RotateLeft(1, 1);
		}
		if (CanRotate(FALSE, 0, -2))
		{
			return RotateLeft(0, -2);
		}
		if (CanRotate(FALSE, 1, -2))
		{
			return RotateLeft(1, -2);
		}
	}
	else if (m_TetriminoAngle == TetriminoAngle::Angle180)
	{
		if (CanRotate(FALSE, -1, 0))
		{
			return RotateLeft(-1, 0);
		}
		if (CanRotate(FALSE, -1, -1))
		{
			return RotateLeft(-1, -1);
		}
		if (CanRotate(FALSE, 0, 2))
		{
			return RotateLeft(0, 2);
		}
		if (CanRotate(FALSE, -1, 2))
		{
			return RotateLeft(-1, 2);
		}
	}
	else if (m_TetriminoAngle == TetriminoAngle::Angle270)
	{
		if (CanRotate(FALSE, -1, 0))
		{
			return RotateLeft(-1, 0);
		}
		if (CanRotate(FALSE, -1, 1))
		{
			return RotateLeft(-1, 1);
		}
		if (CanRotate(FALSE, 0, -2))
		{
			return RotateLeft(0, -2);
		}
		if (CanRotate(FALSE, -1, -2))
		{
			return RotateLeft(-1, -2);
		}
	}
	return FALSE;
}

//�_�u���b�N�̍���]����
BOOL Tetris::RotateLeftI()
{
	if (CanRotate(FALSE, 0, 0))
	{
		return RotateLeft(0, 0);
	}
	if (m_TetriminoAngle == TetriminoAngle::Angle0)
	{
		if (CanRotate(FALSE, -1, 0))
		{
			return RotateLeft(-1, 0);
		}
		if (CanRotate(FALSE, 2, 0))
		{
			return RotateLeft(2, 0);
		}
		if (CanRotate(FALSE, -2, -2))
		{
			return RotateLeft(-2, -2);
		}
		if (CanRotate(FALSE, 2, 1))
		{
			return RotateLeft(2, 1);
		}
	}
	else if (m_TetriminoAngle == TetriminoAngle::Angle90)
	{
		if (CanRotate(FALSE, 2, 0))
		{
			return RotateLeft(2, 0);
		}
		if (CanRotate(FALSE, -1, 0))
		{
			return RotateLeft(-1, 0);
		}
		if (CanRotate(FALSE, 2, -1))
		{
			return RotateLeft(2, -1);
		}
		if (CanRotate(FALSE, -1, 2))
		{
			return RotateLeft(-1, 2);
		}
	}
	else if (m_TetriminoAngle == TetriminoAngle::Angle180)
	{
		if (CanRotate(FALSE, 1, 0))
		{
			return RotateLeft(1, 0);
		}
		if (CanRotate(FALSE, -2, 0))
		{
			return RotateLeft(-2, 0);
		}
		if (CanRotate(FALSE, 1, 2))

		{
			return RotateLeft(1, 2);
		}
		if (CanRotate(FALSE, -2, -1))
		{
			return RotateLeft(-2, -1);
		}
	}
	else if (m_TetriminoAngle == TetriminoAngle::Angle270)
	{
		if (CanRotate(FALSE, 1, 0))
		{
			return RotateLeft(1, 0);
		}
		if (CanRotate(FALSE, -2, 0))
		{
			return RotateLeft(-2, 0);
		}
		if (CanRotate(FALSE, -2, 1))
		{
			return RotateLeft(-2, 1);
		}
		if (CanRotate(FALSE, 1, -2))
		{
			return RotateLeft(1, -2);
		}
	}
	return FALSE;
}

//����]���̉�]�̈ړ�����
BOOL Tetris::RotateLeft(int x, int y)
{
	m_TetriminoPositionX += x;
	m_TetriminoPositionY += y;

	if (m_TetriminoAngle == TetriminoAngle::Angle0)
	{
		m_TetriminoAngle = TetriminoAngle::Angle270;
	}
	else if (m_TetriminoAngle == TetriminoAngle::Angle90)
	{
		m_TetriminoAngle = TetriminoAngle::Angle0;
	}
	else if (m_TetriminoAngle == TetriminoAngle::Angle180)
	{
		m_TetriminoAngle = TetriminoAngle::Angle90;
	}
	else if (m_TetriminoAngle == TetriminoAngle::Angle270)
	{
		m_TetriminoAngle = TetriminoAngle::Angle180;
	}
	return TRUE;
}

/// <summary>
/// �u���b�N�̉�]�ɍ��킹���ʒu���̍X�V���s��
/// </summary>
/// <param name="pos"></param>
/// <param name="size"></param>
/// <param name="angle"></param>

void Tetris::GetInitBlocksRotate33(TetriminoPosition pos[], int size, TetriminoAngle angle)
{
	//�u���b�N��������󋵂Ȃ珈�����I����
	if (size != block_deleteLine)
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

void Tetris::GetInitBlocksRotate44(TetriminoPosition pos[], int size, TetriminoAngle angle)
{
	//�u���b�N��������󋵂Ȃ珈�����I����
	if (size != block_deleteLine)
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
