#pragma once
#include <iostream>				// 일반적인 c++ 라이브러리
#include <fstream>				// 파일입출력용 c++ 라이브러리
#include <cstdio>				// c 형식으로 사용할때 stdio.h와 같은 기능을 하는 라이브러리
#include <string>				// c++ 형식 문자열을 가지고 놀 때 사용하는 라이브러리
using namespace std;

void input_Command();			// 시뮬레이터의 명령어를 받는 함수
void memory_Init(int mb_Size);	// 메모리 크기 초기화
void memory_Write(int sector_num, char *data);	// 섹터단위 쓰기
void memory_Read(int sector_num);				// 섹터단위 읽기
void memory_Erase(int block_num);				// 블록단위 지우기