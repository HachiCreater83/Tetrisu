
#pragma once
//���N���X���ŗp���邷�ׂẴw�b�_�[�̐錾������
//���̃w�b�_�[���Q�Ƃ��邾���ŕK�v�ȎQ�Ƃ��s���邽��
#include <windows.h>
#include <vector>
#include "Block.h"
#include "Tetris.h"
#include <stdio.h>
#include <conio.h>
#include "MainWindow.h"

//���O��Ԃ̎g�p
using namespace std;

extern HWND hMainWnd;


/*
 * TIMER_FREE_FALL��1�b�o�߂��邲�Ƃ�1�����邽�߂ɗp����
 * TIMER_SOFT_DROP�͉��{�^���������Ă���ԁA�}�����������邽�߂ɗp����
 * TIMER_DROP_LINES�̓u���b�N�������Ƃ��ɂ����Ȃ��̃u���b�N�������Ă���̂ł͂Ȃ��Ԃ��󂯂邽�߂ɗp����
 */

#define TIMER_FREE_FALL  100
#define TIMER_SOFT_DROP  200
#define TIMER_DROP_LINES 300