#pragma once
#include <iostream>				// �Ϲ����� c++ ���̺귯��
#include <fstream>				// ��������¿� c++ ���̺귯��
#include <cstdio>				// c �������� ����Ҷ� stdio.h�� ���� ����� �ϴ� ���̺귯��
#include <string>				// c++ ���� ���ڿ��� ������ �� �� ����ϴ� ���̺귯��
using namespace std;

void input_Command();			// �ùķ������� ��ɾ �޴� �Լ�
void memory_Init(int mb_Size);	// �޸� ũ�� �ʱ�ȭ
void memory_Write(int sector_num, char *data);	// ���ʹ��� ����
void memory_Read(int sector_num);				// ���ʹ��� �б�
void memory_Erase(int block_num);				// ��ϴ��� �����