#define _CRT_SECURE_NO_WARNINGS

#define MIN_N 2
#define MAX_N 24

#define MIN_VALUE 0
#define MAX_VALUE 50000000

#define MIN_S -1000000000
#define MAX_S 1000000000

#include <stdio.h>
#include <math.h>

void PrintResult(int *value, int arrayLen, char *sign, int requiredResult)
{
	printf("%i", *value);
	for (int i = 1; i < arrayLen; i++)
	{
		printf("%c%i", *(sign + i), *(value + i));
	}
	printf("=%i", requiredResult);
	return;
}

void ChangeSign(char *sign, int indexToChange)
{
	if (*(sign + indexToChange) == '+') *(sign + indexToChange) = '-';
	else *(sign + indexToChange) = '+';
	return;
}

int IsSolution(int *value, int arrayLen, char *sign, int requiredResult)
{
	int curResult = 0;
	for (int i = 0; i < arrayLen; i++)
	{
		if (*(sign + i) == '+') curResult += *(value + i);
		else curResult -= *(value + i);
	}
	if (curResult != requiredResult) return 0;
	else return 1;
}

int FindFirstPossibleSolution(int *value, int arrayLen, char *sign, int requiredResult)
{
	int curResult = *value;
	for (int i = 1; i < arrayLen; i++)
	{
		if (abs((curResult + value[i]) - requiredResult) < abs((curResult - value[i]) - requiredResult)) //выбор знака так, чтобы стать ближе к requiredResult
		{
			curResult += value[i];
			sign[i] = '+';
		}
		else
		{
			curResult -= value[i];
			sign[i] = '-';
		}
	}

	if (curResult == requiredResult)
	{
		PrintResult(value, arrayLen, sign, requiredResult);
		return 1;
	}
	else return 0;
}

int FindSolution(int indexToChange, int *value, int arrayLen, char *sign, int requiredResult)
{
	if (indexToChange == 0) return 0; //знак у первого числа мен€ть нельз€ => решение не было найдено

	int sucess = 0;
	sucess = FindSolution(indexToChange - 1, value, arrayLen, sign, requiredResult); //уход вглубь рекурсии (при возможности), вернЄт 1 в случае нахождени€ решени€
	if (!sucess)
	{
		ChangeSign(sign, indexToChange);
		if (IsSolution(value, arrayLen, sign, requiredResult)) //проверка решени€ с изменением знака по текущему индексу
		{
			PrintResult(value, arrayLen, sign, requiredResult);
			return 1;
		}
		sucess = FindSolution(indexToChange - 1, value, arrayLen, sign, requiredResult); //уход вглубь рекурсии (при возможности) с изменЄнным знаком
	}

	return sucess;
}

int RequiredResultCanBeReached(int *value, int arrayLen, int requiredResult)
{
	int maxPossibleResult = 0; //точно входит в границы типа signed int (abs(5 * 10^7 * 24) < MAX_INT, MAX_S < MAX_INT)
	for (int i = 0; i < arrayLen; i++)
	{
		maxPossibleResult += *(value + i);
	}
	if (maxPossibleResult < abs(requiredResult)) return 0;
	return 1;
}

int main(void)
{
	int N = 0;
	scanf("%i", &N);
	if (!(MIN_N <= N && N <= MAX_N))
	{
		printf("Incorrect N (number of values): %i <= N <= %i.\n", MIN_N, MAX_N);
		return 0;
	}
	
	int value[MAX_N - MIN_N + 1] = { 0 };
	for (int i = 0; i < N; i++)
	{
		scanf("%i", &value[i]);
		if (!(MIN_VALUE <= value[i] && value[i] <= MAX_VALUE))
		{
			printf("Incorrect value: %i <= number <= %i. This value will be ignored and recognized as 0.\n", MIN_VALUE, MAX_VALUE);
			value[i] = 0;
		}
	}

	int S = 0;
	scanf("%i", &S);
	if (!(MIN_S <= S && S <= MAX_S))
	{
		printf("Incorrect S (required result): %i <= S <= %i.\n", MIN_S, MAX_S);
		return 0;
	}

	if (!RequiredResultCanBeReached(value, N, S))
	{
		printf("no solution");
		return 0;
	}

	char sign[MAX_N - MIN_N + 1] = { '+' };

	int sucess = FindFirstPossibleSolution(value, N, sign, S);
	if (!sucess) sucess = FindSolution(N - 1, value, N, sign, S); //запуск рекурсивного перебора
	if (!sucess) printf("no solution");
	return 0;
}	