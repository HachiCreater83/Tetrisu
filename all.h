
#pragma once
//他クラス内で用いるすべてのヘッダーの宣言をする
//このヘッダーを参照するだけで必要な参照が行えるため
#include <windows.h>
#include <vector>
#include "Block.h"
#include "Tetris.h"
#include <stdio.h>
#include <conio.h>
#include "MainWindow.h"

//名前空間の使用
using namespace std;

extern HWND hMainWnd;


/*
 * TIMER_FREE_FALLは1秒経過するごとに1つ下げるために用いる
 * TIMER_SOFT_DROPは下ボタンを押している間、急速落下させるために用いる
 * TIMER_DROP_LINESはブロックを消すときにいきなり上のブロックが落ちてくるのではなく間を空けるために用いる
 */

#define TIMER_FREE_FALL  100
#define TIMER_SOFT_DROP  200
#define TIMER_DROP_LINES 300