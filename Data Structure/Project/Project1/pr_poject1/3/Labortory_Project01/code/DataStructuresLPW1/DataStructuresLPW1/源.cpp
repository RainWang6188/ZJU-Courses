#include <time.h>
#include <stdio.h>
#include <stdlib.h>

clock_t start, stop; //clock_t�Ǵ�����ʱ��(��ʱ)����������
double duration;  //��¼����������ʱ��(��)


double Algorithm_1(double x, int n)//�㷨1
{
	double result = 1;
	for (int i = 0; i < n; i++)
	{
		result *= x;
	};
	return result;
};

double Algorithm_2_r(double x, int n)//�㷨2(�ݹ�)
{
	double result = 1;

	if (n == 1)//�ݹ�������
		return x;
	else//�ݹ�����
	{
		if ((int)n % 2 == 0)
			return Algorithm_2_r(x, n / 2) * Algorithm_2_r(x, n / 2);
		else
			return Algorithm_2_r(x, (n - 1) / 2) * Algorithm_2_r(x, (n - 1) / 2) * x;
	};
};

double Algorithm_2_i(double x, int n)//�㷨2(����)
{
	double result = 1;
	if (n == 0)
		return result;
	while (n != 0)
	{
		if ((n & 1) == 1)//n�Ķ����Ʊ���λ�Ƿ�Ϊ1
			result *= x;
		x *= x;
		n >>= 1;//λ���㣺��n�Ķ����Ʊ������1λ
	}
	return result;
};

void run(int ch)
{
	double x, n, num = 0, time, UnitTime, ticks;
	printf("Input X(����), N(ָ��), Time(���д���)\n");
	scanf("%lf %lf %lf", &x, &n, &time);

	//ѡ������3���㷨�����
	if (ch == 1)
	{
		//run Algorithm 1 
		start = clock();//��ʱ��1
		for (int i = 0; i < time; i++)//����Time��
		{
			num = Algorithm_1(x, n);
		};
		stop = clock();
		duration = ((double)(stop - start)) / CLK_TCK;
		UnitTime = duration / time;
		printf("�㷨һ��Ϊ�� %f\n", num);
		printf("     ������ʱ�䣺 %lf\n", duration);
		printf("     ƽ����������ʱ�䣺 %lf\n\n", UnitTime);

	}
	else if (ch == 2)
	{
		//run Algorithm 2(����)
		start = clock();//��ʱ��1
		for (int i = 0; i < time; i++)//����Time��
		{
			num = Algorithm_2_i(x, n);
		};
		stop = clock();
		duration = ((double)(stop - start)) / CLK_TCK;
		UnitTime = duration / time;
		printf("�㷨��(����)��Ϊ�� %f\n", num);
		printf("          ������ʱ�䣺 %lf\n", duration);
		printf("          ƽ����������ʱ�䣺 %lf\n\n", UnitTime);
	}
	else
	{
		//run Algorithm 2(�ݹ�)
		start = clock();//��ʱ��2
		for (int i = 0; i < time; i++)//����Time��
		{
			num = Algorithm_2_r(x, n);
		};
		stop = clock();
		duration = ((double)(stop - start)) / CLK_TCK;
		UnitTime = duration / time;
		printf("�㷨��(�ݹ�)��Ϊ�� %f\n", num);
		printf("          ������ʱ�䣺 %lf\n", duration);
		printf("          ƽ����������ʱ�䣺 %lf\n\n", UnitTime);
	};
	printf("Ticks = %f\n", duration * CLK_TCK);
};

int main()
{
	int next = 0;
	while (next == 0)
	{
		int ch;
		printf("------1: �㷨һ  2:�㷨��(����)  3:�㷨��(�ݹ�)------\n");
		scanf("%d", &ch);
		run(ch);
		printf("0: Run Again   1: Exit\n");
		scanf("%d", &next);
	};
};