
#undef UNICODE
#undef _UNICODE

#include <Windows.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define NTHRD 2

int n;
double* matrix;

HANDLE hlthrd[NTHRD];
long idthrd[NTHRD];
int dg[NTHRD];
int gg[NTHRD];

void fillMatrix();
double getSum();
void setToZero();
DWORD WINAPI thrd(VOID*);

int main(int argc, char* argv[])
{
	int i, wp;
	time_t t1, t2,t3,t4;
	double sum1,sum2;
	printf("Unos reda NxN matrice: ");
	scanf("%d", &n);
	printf("\n");
	matrix = (double*)malloc(n * n * sizeof(double));
	setToZero();	
	t1 = clock();
	fillMatrix();	
	t2 = clock();
	sum1 = getSum();
	printf("Sum: %lf\n",sum1);
	printf("Time : %dms ms\n\n", (t2 - t1) );
	setToZero();

	
	t3 = clock();
	wp = n / NTHRD;
	for (i = 0; i < NTHRD; i++)
	{
		dg[i] = wp * i;
		gg[i] = wp * (i + 1);
		if (i == NTHRD - 1) gg[i] = n;
		idthrd[i] = i;
		hlthrd[i] = CreateThread(NULL, 0, thrd, &idthrd[i], 0, NULL);
	}
	WaitForMultipleObjects(NTHRD, hlthrd, true, INFINITE);
	t4 = clock();
	sum2 = getSum();
	printf("Sum: %lf\n", sum2);
	printf("Time : %dms ms\n\n", (t4 - t3));
	free(matrix);
	printf("omjer ubrzanja:%lf\n", (double)(t4 - t3) / (t2 - t1));
	double tocnost = sum2 / sum1;
	if (tocnost < 0)
	{
		tocnost = sum1 / sum2;
	}
	printf("Tocnost:%f\n", fabs(tocnost));
	
	printf(" Tocnost postotak: %lf \n", fabs((double)(tocnost * 100)));
	
	
	return 0;
}

void fillMatrix()
{
	int i, j, k;
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			for (k = 0; k < i; k++)
			{
				matrix[i * n + j] += (double)k * sin((double)j) - (double)j * cos((double)k);
			}
		}
	}
}

double getSum()
{
	int i, j;
	double sum = 0;
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			sum += matrix[i * n + j];
		}
	}
	return sum;
}

void setToZero()
{
	int i, j;
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			matrix[i * n + j] = 0;
		}
	}
}

DWORD WINAPI thrd(VOID* p)
{
	int i, j, k;
	int* id = (int*)p;
	for (i = 0; i < n; i++)
	{
		for (j = dg[*id]; j < gg[*id]; j++)
		{
			for (k = 0; k < i; k++)
			{
				matrix[i * n + j] += (double)k * sin((double)j) - (double)j * cos((double)k);
			}
		}
	}
	ExitThread(0);
}