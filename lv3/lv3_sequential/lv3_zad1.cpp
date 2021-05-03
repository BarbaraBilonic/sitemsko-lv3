// lv3_sequential.cpp : Defines the entry point for the console application.
//
#undef UNICODE
#undef _UNICODE

#include <Windows.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define NTHRD 2

int n;
float* A;

HANDLE hlthrd[NTHRD];
long idthrd[NTHRD];
int dg[NTHRD];
int gg[NTHRD];

void fillA();
float sumA();
void killA();
DWORD WINAPI thrd(VOID*);

int main(int argc, char* argv[])
{
	int i, wp;
	time_t t1, t2;
	printf("Unos reda NxN matrice: ");
	scanf("%d", &n);
	printf("\n");
	A = (float*)malloc(n*n*sizeof(float));
	killA();	//postavlja matricu na 0
	t1 = clock();
	fillA();	//popunjavanje s jednom niti
	t2 = clock();
	printf("Suma svih elemenata: %f\n", sumA());
	printf("Vrijeme : %f s\n\n", (float)(t2 - t1) / 1000);
	killA();

	//popunjavanja s više niti(2)
	t1 = clock();
	wp = n/NTHRD;
	for (i = 0; i < NTHRD; i++)
	{
		dg[i] = wp * i;
		gg[i] = wp * (i + 1);
		if (i == NTHRD - 1) gg[i] = n;
		idthrd[i] = i;
		hlthrd[i] = CreateThread(NULL, 0, thrd, &idthrd[i], 0, NULL);
	}
	WaitForMultipleObjects(NTHRD, hlthrd, true, INFINITE);
	t2 = clock();
	printf("Suma svih elemenata: %f\n", sumA());
	printf("Vrijeme : %f s\n\n", (float)(t2 - t1) / 1000);
	free(A);

	scanf("%d", &n);
	return 0;
}

void fillA()
{
	int i, j, k;
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			for (k = 0; k < i; k++)
			{
				A[i * n + j] +=  (float)k * sin((float)j) - (float)j * cos((float)k);
			}
		}
	}
}

float sumA()
{
	int i, j;
	float sum = 0;
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			sum += A[i * n + j];
		}
	}
	return sum;
}

void killA()
{
	int i, j;
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			A[i * n + j] = 0;
		}
	}
}

DWORD WINAPI thrd(VOID* p)
{
	int i, j, k;
	int *id = (int*)p;
	for (i = 0; i < n; i++)
	{
		for (j = dg[*id]; j < gg[*id]; j++)
		{
			for (k = 0; k < i; k++)
			{
				A[i * n + j] +=  (float)k * sin((float)j) - (float)j * cos((float)k);
			}
		}
	}
	ExitThread(0);
}