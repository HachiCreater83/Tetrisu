
#include "Tetris.h"
#include "BlockFall.h"

extern HWND hMainWnd;

void Tetris::CheckLine()
{
    int index = 0;

    // �`�F�b�N����O�ɔz��m_DeleteLineNumbers��-1�ŏ���������
    for (int i = 0; i < block_deleteLine; i++)
        _deleteLineNumbers[i] = -1;

    for (int row = 0; row < ROW_MAX; row++)
    {
        BOOL isComplete = TRUE;
        for (int column = 0; column < COLUMN_MAX; column++)
        {
            // �����s�ɂЂƂł�NULL������΂��̕������󂢂Ă���
            // ���Ȃ킿������Ă��Ȃ�
            if (m_FixedBlocks[row][column] == NULL)
            {
                isComplete = FALSE;
                break;
            }
        }
        // ���[�v���甲�����Ƃ�isComplete == TRUE�ł���΂��̒i�̓u���b�N��������Ă���Ƃ�����
        // ���̏ꍇ�͔z��ɂ��̍s�̔ԍ����i�[����
        if (isComplete)
        {
            _deleteLineNumbers[index] = row;
            index++;
        }
    }

    // �u���b�N���폜����B���ۂɍ폜�̏����������Ȃ�ꂽ��DeleteLines�֐���TRUE��Ԃ�
    BOOL ret = DeleteLines(_deleteLineNumbers, block_deleteLine);
    if (ret)
    {
        // DeleteLines�֐������s���ꂽ��E�B���h�E�S�̂��ĕ`�悷��
        // �����Ƀ^�C�}�[���Z�b�g����0.1�b��ɏ�̒i�ɂ���u���b�N�����Ɉړ�������
        SetTimer(hMainWnd, TIMER_DROP_LINES, 100, NULL);
        InvalidateRect(hMainWnd, NULL, TRUE);
    }
}

BOOL Tetris::DeleteLines(int lineNumbers[], int size)
{
    BOOL done = FALSE;
    for (int i = 0; i < size; i++)
    {
        // �v�f��-1�̂Ƃ��͂Ȃɂ����Ȃ�
        // �Ȃɂ������Ƃ��͂��̊֐���TRUE��Ԃ�
        int lineNumber = lineNumbers[i];
        if (lineNumber == -1)
            continue;

        done = TRUE;

        for (int x = 0; x < COLUMN_MAX; x++)
        {
            // delete��������NULL�����B�����ɂ̓u���b�N�͂Ȃ��ƔF�������
            delete m_FixedBlocks[lineNumber][x];
            m_FixedBlocks[lineNumber][x] = NULL;
        }
    }
    return done;
}

BOOL Tetris::CheckDeletingLines()
{
    //�����͈͂̊m�F,�ő吔��4
    for (int i = 0; i < block_deleteLine; i++)
    {
        int lineNumber = _deleteLineNumbers[i];
        if (lineNumber != -1)
        {
            return TRUE;
        }
    }
    return FALSE;
}