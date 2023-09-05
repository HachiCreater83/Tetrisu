
#include "Tetris.h"
#include "BlockFall.h"

extern HWND hMainWnd;

void Tetris::CheckLine()
{
    int index = 0;

    // チェックする前に配列m_DeleteLineNumbersを-1で初期化する
    for (int i = 0; i < block_deleteLine; i++)
        _deleteLineNumbers[i] = -1;

    for (int row = 0; row < ROW_MAX; row++)
    {
        BOOL isComplete = TRUE;
        for (int column = 0; column < COLUMN_MAX; column++)
        {
            // 同じ行にひとつでもNULLがあればその部分が空いている
            // すなわちそろっていない
            if (m_FixedBlocks[row][column] == NULL)
            {
                isComplete = FALSE;
                break;
            }
        }
        // ループから抜けたときisComplete == TRUEであればその段はブロックがそろっているといえる
        // その場合は配列にその行の番号を格納する
        if (isComplete)
        {
            _deleteLineNumbers[index] = row;
            index++;
        }
    }

    // ブロックを削除する。実際に削除の処理がおこなわれたらDeleteLines関数はTRUEを返す
    BOOL ret = DeleteLines(_deleteLineNumbers, block_deleteLine);
    if (ret)
    {
        // DeleteLines関数が実行されたらウィンドウ全体を再描画する
        // 同時にタイマーをセットして0.1秒後に上の段にあるブロックを下に移動させる
        SetTimer(hMainWnd, TIMER_DROP_LINES, 100, NULL);
        InvalidateRect(hMainWnd, NULL, TRUE);
    }
}

BOOL Tetris::DeleteLines(int lineNumbers[], int size)
{
    BOOL done = FALSE;
    for (int i = 0; i < size; i++)
    {
        // 要素が-1のときはなにもしない
        // なにかしたときはこの関数はTRUEを返す
        int lineNumber = lineNumbers[i];
        if (lineNumber == -1)
            continue;

        done = TRUE;

        for (int x = 0; x < COLUMN_MAX; x++)
        {
            // deleteしたあとNULLを代入。ここにはブロックはないと認識される
            delete m_FixedBlocks[lineNumber][x];
            m_FixedBlocks[lineNumber][x] = NULL;
        }
    }
    return done;
}

BOOL Tetris::CheckDeletingLines()
{
    //消す範囲の確認,最大数が4
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