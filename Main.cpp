/* Copyright(c) 2022, Harry Huang
 * @ MIT License
 */
#define _CRT_SECURE_NO_WARNINGS = 1
#include<stdio.h>
#include<iostream>
using namespace std;
const int SIZE = 4;


/* Whether the Number Cannot Transform to Int. */
bool isFloat(float num) {
	return (num != int(num));
}

/* The Index of the Max Value in a Array. */
int maxIdx(int arr[], bool ignore[], int len) {
	int cur;
	int rst;
	bool flag = false;
	for (int i = 0; i < len; i++)
		if (!ignore[i]) {
			cur = arr[i];
			rst = i;
			flag = true;
			break;
		}
	if (!flag)
		return -1;
	for (int i = 0; i < len; i++)
		if (!ignore[i] && cur < arr[i]) {
			cur = arr[i];
			rst = i;
		}
	return rst;
}

/* The Index of a Nearest Specific Available Value. */
int avaiIdx(bool ignore[], int len, int baseIdx, int offset) {
	int idx = baseIdx;
	int step = offset;
	for (int i = baseIdx; ; i += step) {
		if (idx < 0 || idx >= len)
			break;
		idx = i;
		if (!ignore[idx])
			break;
	}
	return idx;
}

/* Whether the Operator is a Bit Operator. */
bool isBitOper(char oper) {
	return (oper == '&' || oper == '|' || oper == '^' || oper == '<' || oper == '>');
}

/* Whether the Operator List Has Any Bit Operator. */
bool hasBitOper(char oper[], int len) {
	for (int i = 0; i < len; i++)
		if (isBitOper(oper[i]))
			return true;
	return false;
}

/* Calculate with Two Values and a Operator Char. */
float calTow(float val1, float val2, char oper) {
	switch (oper) { // The Four Fundamental Operations.
	case '+':
		return val1 + val2;
	case '-':
		return val1 - val2;
	case '*':
		return val1 * val2;
	case '/':
		return val1 / val2;
	default:
		if (!isFloat(val1) && !isFloat(val2)) {
			int val01 = int(val1);
			int val02 = int(val2);
			switch (oper) { // Bit Operations.
			case '&':
				return val01 & val02;
			case '|':
				return val01 | val02;
			case '^':
				return val01 ^ val02;
			case '<':
				return val01 << val02;
			case '>':
				return val01 >> val02;
			default:
				return 0.0f;
			}
		}
		return 0.0f;
	}
}

/* Calculate with Multi Values and Their Operators. */
float calMulti(int theval[], char theoper[]) {
	float val[SIZE];
	for (int i = 0; i < (SIZE); i++)
		val[i] = theval[i] / 1.0f;
	char oper[SIZE - 1];
	memcpy(oper, theoper, (SIZE - 1)*sizeof(char));
	for (int i = 0; i < (SIZE - 1); i++) {
		val[i + 1] = calTow(val[i], val[i + 1], oper[i]);
	}
	return val[SIZE - 1];
}

/* Calculate with Multi Values, Operators and a Priorities List. */
float calMulti(int theval[], char theoper[], int theprio[]) {
	float val[SIZE];
	for (int i = 0; i < (SIZE); i++)
		val[i] = theval[i] / 1.0f;
	char oper[SIZE - 1];
	memcpy(oper, theoper, (SIZE - 1) * sizeof(char));
	int prio[SIZE - 1];
	memcpy(prio, theprio, (SIZE - 1) * sizeof(int));
	bool ignoreOper[SIZE - 1];
	memset(ignoreOper, 0, (SIZE - 1) * sizeof(bool));
	bool ignoreVal[SIZE];
	memset(ignoreVal, 0, SIZE * sizeof(bool));
	for (int i = 0; i < SIZE - 1; i++) {
		int idx = maxIdx(prio, ignoreOper, SIZE - 1);
		int val1Idx = avaiIdx(ignoreVal, SIZE, idx, -1);
		int val2Idx = avaiIdx(ignoreVal, SIZE, idx+1, 1);
		val[val2Idx] = calTow(val[val1Idx], val[val2Idx], oper[idx]);
		ignoreOper[idx] = true;
		ignoreVal[val1Idx] = true;
	}
	return val[avaiIdx(ignoreVal, SIZE, 0, 1)];
}

/* Whether the Operation is Illegal. */
bool isIllegal(float val1, float val2, char oper) {
	if (oper == '/' && val2 == 0)
		return true; // Divided by 0
	if (isBitOper(oper) && (isFloat(val1) || isFloat(val2)))
		return true; // Bit Operation with Float
	return false;
}

/* Whether the Operation List Has Any Illegal Operation. */
bool areIllegal(int theval[], char theoper[], int theprio[]) {
	float val[SIZE];
	for (int i = 0; i < (SIZE); i++)
		val[i] = theval[i] / 1.0f;
	char oper[SIZE - 1];
	memcpy(oper, theoper, (SIZE - 1) * sizeof(char));
	int prio[SIZE - 1];
	memcpy(prio, theprio, (SIZE - 1) * sizeof(int));
	bool ignoreOper[SIZE - 1];
	memset(ignoreOper, 0, (SIZE - 1) * sizeof(bool));
	bool ignoreVal[SIZE];
	memset(ignoreVal, 0, SIZE * sizeof(bool));
	for (int i = 0; i < SIZE - 1; i++) {
		int idx = maxIdx(prio, ignoreOper, SIZE - 1);
		int val1Idx = avaiIdx(ignoreVal, SIZE, idx, -1);
		int val2Idx = avaiIdx(ignoreVal, SIZE, idx + 1, 1);
		if (isIllegal(val[val1Idx], val[val2Idx], oper[idx]))
			return true;
		val[val2Idx] = calTow(val[val1Idx], val[val2Idx], oper[idx]);
		ignoreOper[idx] = true;
		ignoreVal[val1Idx] = true;
	}
	return false;
}

/* Print the Answer. */
void printAns(int val[], char oper[], int prio[]) {
	int cur = 0;
	int delta;
	for (int i = 0; i < SIZE; i++) {
		if (i < SIZE - 1) {
			delta = prio[i] - cur;
			cur = prio[i];
		}
		else
			delta = -cur;
		if (delta >= 0)
			for (int j = 0; j < delta; j++)
				printf("(");
		printf("%d", val[i]);
		if (delta < 0)
			for (int j = 0; j < -delta; j++)
				printf(")");
		if (i < SIZE - 1) {
			printf("%c", oper[i]);
			if (oper[i] == '<' || oper[i] == '>')
				printf("%c", oper[i]);
		}
	}
}

/* Solve the 24Points Problem and Print All the Answer. */
int twentyFourPoints(int A, int B, int C, int D, bool doBitOper) {
	// Supported: + - * / & | ^ < >
	char OPER[] = { '+','-','*','/','&','|','^','<','>' };
	int OPERLEN = doBitOper ? 9 : 4;
	int* VAL[] = { &A,&B,&C,&D };
	int cnt = 0;

	printf("\nInput: [%d] [%d] [%d] [%d]\n", A, B, C, D);
	/*** Enumeration.I ***/
	for (int a = 0; a < SIZE; a++) {
		for (int b = 0; b < SIZE; b++) {
			for (int c = 0; c < SIZE; c++) {
				for (int d = 0; d < SIZE; d++) {
					if (a == b || a == c || a == d || b == c || b == d || c == d)
						continue;
					int val[4] = { *VAL[a],*VAL[b],*VAL[c],*VAL[d] };
					/*** Enumeration.II ***/
					for (int x = 0; x < OPERLEN; x++) {
						for (int y = 0; y < OPERLEN; y++) {
							for (int z = 0; z < OPERLEN; z++) {
								char oper[3] = { OPER[x],OPER[y],OPER[z] };
								/*** Enumeration.III ***/
								for (int e = 0; e < SIZE - 1; e++) {
									for (int f = 0; f < SIZE - 1; f++) {
										for (int g = 0; g < SIZE - 1; g++) {
											if (e == f || e == g || f == g)
												continue;
											int prio[3] = { e,f,g };
											if (areIllegal(val, oper, prio))
												continue;
											/*** Judgement ***/
											float rst = calMulti(val, oper, prio);
											if (rst == 24.0f) { // Found a Possible Answer!
												printf("=");
												printAns(val, oper, prio);
												printf("\n");
												cnt++;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	cout << "\n";
	return cnt;
}

/* Main. */
int main() {
	int A, B, C, D;
	char doBitOper = '\0';
	printf("Welcome to 24Points Helper.\nAllow Bit Operation? (Y/n) ");
	scanf("%c", &doBitOper);
	printf("\nInput 4 Numbers and Press Enter...\n");

	while (true) { // True for Loop.
		scanf("%d %d %d %d", &A, &B, &C, &D);
		twentyFourPoints(A, B, C, D, (doBitOper == 'Y' || doBitOper == 'y') ? true : false);
	}

	return 0;
}
