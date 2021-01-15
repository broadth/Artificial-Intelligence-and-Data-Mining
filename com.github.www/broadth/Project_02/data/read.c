#include <stdio.h>
#include <stdlib.h>

#define ARRAY_ROW       106
#define ARRAY_COLUMN    11

const char file_name[] = "z_score_normalization.csv";

double ** read_csv(const char file_name[]);

int main(void)
{
	double **dfpArray = NULL;
	
	dfpArray = read_csv(file_name);
	
	return 0;
}


double ** read_csv(const char file_name[])
{
	double **dfpArray = NULL;
	
	// 申请存放读取数据的数组空间
	dfpArray = (double **)malloc(sizeof(double *)*ARRAY_ROW);
	if(dfpArray == NULL)
	{
		printf("内存分配失败。\n");
		exit(-1);
	}
	for(int i=0; i<ARRAY_ROW; i++)
	{
		dfpArray[i] = (double *)malloc(sizeof(double)*ARRAY_COLUMN);
		if(dfpArray[i] == NULL)
		{
			printf("内存分配失败。\n");
			exit(-1);
		}
	}
	
	FILE *fpFile=NULL;
	char szBuffer[1024];
	
	fpFile = fopen(file_name, "r");
	if(fpFile == NULL)
	{
		printf("打开文件 %s 失败。\n", file_name);
		exit(-1);
	}
	fscanf(fpFile, "%s\n", szBuffer);
	for(int i=0; i<ARRAY_ROW; i++)
	{
		for(int j=0; j<ARRAY_COLUMN; j++)
		{
			if(j != ARRAY_COLUMN-1)
			{
				fscanf(fpFile, "%lf,", &dfpArray[i][j]);
			}
			else
			{
				fscanf(fpFile, "%lf\n", &dfpArray[i][j]);
			}
		}
	}

	// 打印看看是否正确读取
	for(int i=0; i<ARRAY_ROW; i++)
	{
		for(int j=0; j<ARRAY_COLUMN; j++)
		{
			printf("%.2f\t", dfpArray[i][j]);
		}
		putchar('\n');
	}
	
	return dfpArray;
}