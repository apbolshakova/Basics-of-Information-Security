#define _CRT_SECURE_NO_WARNINGS

#include "stdio.h"
#include "conio.h"
#include "math.h"

#define SRC_FILE_PATH "result.bmp"
#define DEST_FILE_PATH "decoded.txt"

#define OFFSET_POS_HEX 0x0A //����� �������, ������� � �������� ���� ����� �����������
#define PALETTE_POS_HEX 0x1C //����� �������
#define SIZE_POS_HEX 0x22 //����� ������� ����������

#define PALETTE 24
#define BITS_IN_BYTE 8
#define LEN_NUMBER_SIZE 8 //���������� ��� ��� �������� ����� �������������� ������