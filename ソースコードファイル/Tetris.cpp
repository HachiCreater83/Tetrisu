

// <vector>��Tetris.h�������include����
#include <vector>
#include <time.h>
#include <windows.h>
using namespace std;

#include "Block.h"
#include "BlockFall.h"
#include "Tetris.h"

extern HWND hMainWnd;

// �u���b�N���\������镔���̍���̍��W��]������������悤��
Tetris::Tetris()
{
    LeftTopBlockPosition.x = _blockPositionX;
    LeftTopBlockPosition.y = _blockPositionY;

    Init();
}

void Tetris::Init()
{
    InitFixedBlocks();

    // �Q�[���J�n���ɂ̓z�[���h����Ă���e�g���~�m�͑��݂��Ȃ�
    // �Q�[���J�n����͂��ł��z�[���h�͉\�ł���
    m_holdTetrimino = TetriminoTypes::None;
    m_CanHold = TRUE;

    for (int i = 0; i < block_deleteLine; i++)
        _deleteLineNumbers[i] = -1;

    unsigned int now = (unsigned int)time(0);
    srand(now);

    CreateBag();
    NewTetrimino();
}

void Tetris::InitFixedBlocks()
{
    for (int row = 0; row < 20; row++)
    {
        for (int column = 0; column < 10; column++)
        {
            delete m_FixedBlocks[row][column];
            m_FixedBlocks[row][column] = NULL;
        }
    }
}

void Tetris::ResetTimer()
{
    // �^�C�}�[�����Z�b�g���A�����ɃZ�b�g����
    KillTimer(hMainWnd, TIMER_FREE_FALL);
    SetTimer(hMainWnd, TIMER_FREE_FALL, 1000, NULL);
}

void Tetris::Draw(HDC hdc)
{
    DrawGhostBlocks(hdc);

    DrawFixedBlock(hdc);
    DrawOutsideBlocks(hdc);
    DrawTetrimino(hdc);

    ShowScore(hdc);
    ShowNext(hdc);
    ShowHold(hdc);

    if (m_IsGameOver)
        ShowGameOver(hdc);
}

void Tetris::DrawGhostBlock(HDC hdc, int column, int row, Color color)
{
    COLORREF colorref = GetGhostColor(color);

    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
    HBRUSH hBrush = CreateSolidBrush(colorref);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

    int x = column * BLOCK_SIZE + LeftTopBlockPosition.x;
    int y = row * BLOCK_SIZE + LeftTopBlockPosition.y;
    ::Rectangle(hdc, x, y, x + BLOCK_SIZE, y + BLOCK_SIZE);

    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);

    DeleteObject(hPen);
    DeleteObject(hBrush);
}

void Tetris::DrawGhostBlocks(HDC hdc)
{

    Block blocks[4];
    GetGhostBlocks(blocks, 4);
    for (int i = 0; i < 4; i++)
        DrawGhostBlock(hdc, blocks[i].m_column, blocks[i].m_row, blocks[i].color);
}

void Tetris::DrawTetrimino(HDC hdc)
{
    TetriminoPosition position[4];

    // ���݂̃e�g���~�m�̈ʒu�ƐF���擾����
    GetTetriminosPosition(position, 4);
    Color color = GetTetriminoColor();

    // �擾�����ʒu��DrawBlock�֐��ɂ킽���ĕ`�悷��
    for (int i = 0; i < 4; i++)
        DrawBlock(hdc, position[i].m_column, position[i].m_row, color);

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

void Tetris::GetGhostBlocks(Block* blocks, int size)
{
    TetriminoPosition pos[4];
    GetTetriminosPosition(pos, 4);
    int fallCount = GetHardDropCount();

    for (int i = 0; i < 4; i++)
    {
        blocks[i].m_column = pos[i].m_column;
        blocks[i].m_row = pos[i].m_row + fallCount;
        blocks[i].color = GetTetriminoColor(); // �������̃e�g���~�m�̐F���i�[���Ă���
    }
}

COLORREF Tetris::GetGhostColor(Color color)
{
    if (color == Color::Aqua)
        return 0x00404000;
    else if (color == Color::Blue)
        return 0x00800000;
    else if (color == Color::Green)
        return 0x00004000;
    else if (color == Color::Orange)
        return 0x00001040;
    else if (color == Color::Red)
        return 0x00000040;
    else if (color == Color::Violet)
        return 0x00200020;
    else if (color == Color::Yellow)
        return 0x00004040;
    else
        return 0x00808080;
}

void Tetris::CreateBag()
{
    if (m_vectorTetriminoTypes.size() > 7)
        return;

    vector<TetriminoTypes> v;
    v.push_back(TetriminoTypes::I);
    v.push_back(TetriminoTypes::J);
    v.push_back(TetriminoTypes::L);
    v.push_back(TetriminoTypes::O);
    v.push_back(TetriminoTypes::S);
    v.push_back(TetriminoTypes::T);
    v.push_back(TetriminoTypes::Z);

    while (true)
    {
        int size = v.size();
        if (size == 0)
        {
            break;
        }
        int type = rand() % size;
        TetriminoTypes selectedType = v[type];
        m_vectorTetriminoTypes.push_back(selectedType);
        v.erase(v.begin() + type);
    }
}

void Tetris::SetNewBlockType()
{
    if (m_IsGameOver)
    {
        return;
    }
    m_CurTetriminoType = m_vectorTetriminoTypes[0];
    m_vectorTetriminoTypes.erase(m_vectorTetriminoTypes.begin());

    CreateBag();
    InvalidateRect(hMainWnd, NULL, TRUE);
}

void Tetris::ShowNext(HDC hdc)
{
    // ������u�m�d�w�s�v��\��
    SetTextColor(hdc, RGB(255, 255, 255));
    HFONT hFont = SetScoreFont(hdc, 16);
    HFONT hFontOld = (HFONT)SelectObject(hdc, hFont);
    const WCHAR* str = L"�m�d�w�s";

    TextOut(hdc, 400, 50, str, lstrlen(str));
    SelectObject(hdc, hFontOld);
    DeleteObject(hFont);

    // �e�g���~�m��\������ꏊ�����߂�
    //�`��̊֌W��80���l�𑝂₷
    POINT leftTop;
    leftTop.x = 400;
    leftTop.y = 100;
    DrawNextBlock(hdc, leftTop, m_vectorTetriminoTypes[0]);

    leftTop.y = 180;
    DrawNextBlock(hdc, leftTop, m_vectorTetriminoTypes[1]);

    leftTop.y = 260;
    DrawNextBlock(hdc, leftTop, m_vectorTetriminoTypes[2]);

    leftTop.y = 340;
    DrawNextBlock(hdc, leftTop, m_vectorTetriminoTypes[3]);
}

void Tetris::DrawNextBlock(HDC hdc, POINT leftTpo, TetriminoTypes tetriminoType)
{
    COLORREF colorref = GetColor(GetTetriminoColor(tetriminoType));
    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
    HBRUSH hBrush = CreateSolidBrush(colorref);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

    int columns[4];
    int rows[4];
    GetColumnsRowsNextBlock(tetriminoType, columns, rows, 4);

    int blockSize = 16; // �t�B�[���h�ɕ\������Ă�����́i=20�j����⏬���߂̃T�C�Y��
    for (int i = 0; i < 4; i++)
    {
        int x = columns[i] * blockSize + leftTpo.x;
        int y = rows[i] * blockSize + leftTpo.y;
        ::Rectangle(hdc, x, y, x + blockSize, y + blockSize);
    }

    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);
    DeleteObject(hPen);
    DeleteObject(hBrush);
}

void Tetris::GetColumnsRowsNextBlock(TetriminoTypes tetriminoType, int columns[], int rows[], int size)
{
    if (size != 4)
    {
        return;
    }
    if (tetriminoType == TetriminoTypes::I)
    {
        columns[0] = 0;
        columns[1] = 1;
        columns[2] = 2;
        columns[3] = 3;
        rows[0] = 1;
        rows[1] = 1;
        rows[2] = 1;
        rows[3] = 1;
        return;
    }
    if (tetriminoType == TetriminoTypes::J)
    {
        columns[0] = 0;
        columns[1] = 0;
        columns[2] = 1;
        columns[3] = 2;
        rows[0] = 0;
        rows[1] = 1;
        rows[2] = 1;
        rows[3] = 1;
        return;
    }
    if (tetriminoType == TetriminoTypes::L)
    {
        columns[0] = 0;
        columns[1] = 1;
        columns[2] = 2;
        columns[3] = 2;
        rows[0] = 1;
        rows[1] = 1;
        rows[2] = 1;
        rows[3] = 0;
        return;
    }
    if (tetriminoType == TetriminoTypes::O)
    {
        columns[0] = 0;
        columns[1] = 0;
        columns[2] = 1;
        columns[3] = 1;
        rows[0] = 0;
        rows[1] = 1;
        rows[2] = 0;
        rows[3] = 1;
        return;
    }
    if (tetriminoType == TetriminoTypes::S)
    {
        columns[0] = 0;
        columns[1] = 1;
        columns[2] = 1;
        columns[3] = 2;
        rows[0] = 1;
        rows[1] = 0;
        rows[2] = 1;
        rows[3] = 0;
        return;
    }
    if (tetriminoType == TetriminoTypes::T)
    {
        columns[0] = 0;
        columns[1] = 1;
        columns[2] = 1;
        columns[3] = 2;
        rows[0] = 1;
        rows[1] = 0;
        rows[2] = 1;
        rows[3] = 1;
        return;
    }
    if (tetriminoType == TetriminoTypes::Z)
    {
        columns[0] = 0;
        columns[1] = 1;
        columns[2] = 1;
        columns[3] = 2;
        rows[0] = 0;
        rows[1] = 0;
        rows[2] = 1;
        rows[3] = 1;
        return;
    }
}

Color Tetris::GetTetriminoColor(TetriminoTypes tetriminoType)
{
    if (tetriminoType == TetriminoTypes::I)
    {
        return Color::Aqua;
    }
    else if (tetriminoType == TetriminoTypes::J)
    {
        return Color::Blue;
    }
    else if (tetriminoType == TetriminoTypes::L)
    {
        return Color::Orange;
    }
    else if (tetriminoType == TetriminoTypes::O)
    {
        return Color::Yellow;
    }
    else if (tetriminoType == TetriminoTypes::S)
    {
        return Color::Green;
    }
    else if (tetriminoType == TetriminoTypes::T)
    {
        return Color::Violet;
    }
    else if (tetriminoType == TetriminoTypes::Z)
    {
        return Color::Red;
    }
    else
    {
        return Color::Gray;
    }
}

BOOL Tetris::CanMove(int x, int y)
{
    // �ړ����TetriminoPosition�̔z����擾����
    TetriminoPosition pos[4];
    GetTetriminosPosition(pos, 4);

    // �O�ɏo����Œ肳��Ă���u���b�N�ƂԂ����Ă��܂��ꍇ�͈ړ��s�Ƃ���
    for (int i = 0; i < 4; i++)
    {
        if (pos[i].m_column + x > COLUMN_MAX - 1)
            return FALSE;
        if (pos[i].m_column + x < 0)
            return FALSE;
        if (pos[i].m_row + y > ROW_MAX - 1)
            return FALSE;

        if (pos[i].m_row + y < 0) // ���̏ꍇ�͕]�����Ȃ�
            continue;

        if (m_FixedBlocks[pos[i].m_row + y][pos[i].m_column + x] != NULL)
            return FALSE;
    }
    return TRUE;
}
