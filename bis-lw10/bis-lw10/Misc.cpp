#include "Header.h"

size_t getParityBitsNum(size_t dataBitsNum)
{
	size_t result = (size_t)ceil(log2(dataBitsNum));
	if (result < MIN_PARITY_BITS) result = MIN_PARITY_BITS; //для обработки по 8 бит
	return result;
}

BOOL isPowerOf2(int n)
{
	return (n > 0 && (n & (n - 1)) == 0);
}

void toggleParityBits(char* block, size_t num)
{
	size_t curPos = 0;
	while (num)
	{
		if ((num & (0x01 << 0)) != 0)
			if (block[pow2(curPos)] == '0') block[pow2(curPos)] = '1';
			else block[pow2(curPos)] = '0';
		num >>= 1;
		curPos++;
	}
}

int pow2(int num)
{
	if (num == 0) return 1;
	return 2 << (num - 1);
}

int getNOD(int n1, int n2)
{
	int div;
	if (n1 == n2)  return n1;
	int d = n1 - n2;
	if (d < 0) {
		d = -d;  div = getNOD(n1, d);
	}
	else
		div = getNOD(n2, d);
	return div;
}

int getNOK(int n1, int n2)
{
	return n1 * n2 / getNOD(n1, n2);
}