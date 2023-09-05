
#pragma once

/*
 * TIMER_FREE_FALLは1秒経過するごとに1つ下げる
 * TIMER_SOFT_DROPは下ボタンを押している間急速落下させる
 * TIMER_DROP_LINESは横一列にブロックを消すときにいきなり上のブロックが落ちてくるのではなく間を空ける
 */
#define TIMER_FREE_FALL  100
#define TIMER_SOFT_DROP  200
#define TIMER_DROP_LINES 300