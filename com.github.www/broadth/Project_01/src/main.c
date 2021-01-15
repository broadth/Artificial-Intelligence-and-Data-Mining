#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#define CSV_ROW 98
#define TXT_ROW 99
#define BUFFER 1024


/* 结构体定义 */
typedef struct Student_Info
{
    int id;
    char name[16];
    char city[16];
    char gender[16];
    
    double height;
    double score[10];

    char constitution[16];
} STUDENT_INFO, *PSTUDENT_INFO;


/* 全局变量定义 */
const char file_name_csv[] = "../data/data.csv";
const char file_name_txt[] = "../data/data.txt";

int iRealRow=0;


/* 函数声明 */
void init(PSTUDENT_INFO psiArray, int iLength);
PSTUDENT_INFO read_file_csv(const char file_name[]);
PSTUDENT_INFO read_file_txt(const char file_name[]);
PSTUDENT_INFO combination(PSTUDENT_INFO psiArrayOne, int iLengthOne, PSTUDENT_INFO psiArrayTwo, int iLengthTwo);
void print(PSTUDENT_INFO psiArray, int iRow);
void save_file_csv(PSTUDENT_INFO psiArray, int iRow);

// 声明解决设置问题的函数
void print_beijing_average_score(PSTUDENT_INFO psiArray, int iRow);
int count_guangzhou_score_number(PSTUDENT_INFO psiArray, int iRow);
void print_constitution_compare_between_the_girl_of_guangzhou_and_shanghai(PSTUDENT_INFO psiArray, int iRow);
void correlation_calculation(PSTUDENT_INFO psiArray, int iRow);


/* 主函数 */
int main(void)
{
    PSTUDENT_INFO psiArrayCsv=NULL;
    PSTUDENT_INFO psiArrayTxt=NULL;
	PSTUDENT_INFO psiArrayTotal=NULL;

	// 读取数据 
    printf("文件 %s 中的数据：\n", file_name_csv);
    psiArrayCsv = read_file_csv(file_name_csv);
    print(psiArrayCsv, CSV_ROW);

    printf("文件 %s 中的数据：\n", file_name_txt);
    psiArrayTxt = read_file_txt(file_name_txt);
    print(psiArrayTxt, TXT_ROW);

	// 整合数据
	psiArrayTotal = combination(psiArrayCsv, CSV_ROW, psiArrayTxt, TXT_ROW);
	printf("整合后共有如下 %d 位同学的数据：\n", iRealRow);
	print(psiArrayTotal, iRealRow);	

	// 打印家乡在 Beijing 的同学各科平均分
	print_beijing_average_score(psiArrayTotal, iRealRow);

	// 计算家乡在广州，C1 成绩大于 80 ，C9 成绩大于 9 的学生的数量
	int iCountNumber;
	iCountNumber = count_guangzhou_score_number(psiArrayTotal, iRealRow);
	printf("家乡在广州，C1 成绩大于80分，C9成绩大于 9 分的学生的数量为：%d\n\n", iCountNumber);

	// 比较家乡在广州和上海的女生的体能成绩评价
	print_constitution_compare_between_the_girl_of_guangzhou_and_shanghai(psiArrayTotal, iRealRow);

	// 成绩与体育相关系数计算
	correlation_calculation(psiArrayTotal, iRealRow);	

	// 保存整理后的数据
	save_file_csv(psiArrayTotal, iRealRow);

	// 释放堆空间
	free(psiArrayCsv);
	free(psiArrayTxt);
	free(psiArrayTotal);

    // system("pause");
    return 0;
}


/* 函数实现 */
void init(PSTUDENT_INFO psiArray, int iLength)
{
    for(int i=0; i<iLength; i++)
    {
        // ID 初始化
        psiArray[i].id = -1;

        // 姓名、城市、性别初始化
        memset(psiArray[i].name, '\0', sizeof(psiArray[i].name));
		strncpy(psiArray[i].name, "null", 4);
        memset(psiArray[i].city, '\0', sizeof(psiArray[i].city));
		strncpy(psiArray[i].city, "null", 4);
        memset(psiArray[i].gender, '\0', sizeof(psiArray[i].gender));
		strncpy(psiArray[i].gender, "null", 4);

        // 身高初始化
        psiArray[i].height = -1;

        // 课程成绩初始化
        for(int j=0; j<10; j++)
        {
            psiArray[i].score[j] = -1;
        }
        
        // 评价初始化
        memset(psiArray[i].constitution, '\0', sizeof(psiArray[i].constitution));
		strncpy(psiArray[i].constitution, "null", 4);
    }
}

PSTUDENT_INFO read_file_csv(const char file_name[])
{
    FILE *fpFile;
    char szLines[BUFFER];
    PSTUDENT_INFO psiArray=NULL;

    // 内存申请
    psiArray = (PSTUDENT_INFO)malloc(sizeof(STUDENT_INFO) * CSV_ROW);
    if(psiArray == NULL)
    {
        printf("内存分配失败。\n");
        exit(-1);
    }
    else
    {
        // 内存块初始化
        init(psiArray, CSV_ROW);

        // 读取 CSV 文件中的数据
        fpFile = fopen(file_name, "r");
        if(fpFile != NULL)
        {
            // 丢弃首行的列标签
            fscanf(fpFile, "%s\n", szLines);

            for(int i=0; i<CSV_ROW; i++)
            {
                int status;
                
                // 读取ID
                status = fscanf(fpFile, "%d,", &((psiArray[i]).id));
                if(status == 0)
                {
                    fgetc(fpFile);
                }
				if(psiArray[i].id!=-1 && psiArray[i].id<202000)
				{
					psiArray[i].id += 202000;
				}

                // 读取姓名、城市、性别
                status = fscanf(fpFile, "%[^,],", psiArray[i].name);
                if(status == 0)
                {
                    fgetc(fpFile);
                }
                status = fscanf(fpFile, "%[^,],", psiArray[i].city);
                if(status == 0)
                {
                    fgetc(fpFile);
                }
                status = fscanf(fpFile, "%[^,],", psiArray[i].gender);
                if(status == 0)
                {
                    fgetc(fpFile);
                }
				if(strncmp(psiArray[i].gender, "male", 4) == 0)
				{
					strncpy(psiArray[i].gender, "boy\0", 4);
				}
				else if(strncmp(psiArray[i].gender, "female", 6) == 0)
				{
					strncpy(psiArray[i].gender, "girl\0\0", 6);
				}

                // 读取身高
                status = fscanf(fpFile, "%lf,", &((psiArray[i]).height));
                if(status == 0)
                {
                    fgetc(fpFile);
                }
				if(psiArray[i].height!=-1 && psiArray[i].height<3)
				{
					psiArray[i].height *= 100;
				}

                // 读取成绩
                for(int j=0; j<10; j++)
                {
                    status = fscanf(fpFile, "%lf,", &((psiArray[i]).score[j]));
                    if(status == 0)
                    {
                        fgetc(fpFile);
                    }
                }

                // 读取评价
                fpos_t fpos_tPosition;
                fgetpos(fpFile, &fpos_tPosition);
                if(fgetc(fpFile) == 13)
                {
                    continue;
                }
                else
                {
                    fsetpos(fpFile, &fpos_tPosition);
                    fscanf(fpFile, "%s\n", psiArray[i].constitution);
                }
            }

            fclose(fpFile);

            return psiArray;
        }
        else
        {
            printf("打开文件 %s 失败", file_name);
            exit(-1);
        }
    }
}

PSTUDENT_INFO read_file_txt(const char file_name[])
{
    FILE *fpFile;
    char szLines[BUFFER];
    PSTUDENT_INFO psiArray=NULL;

    // 内存申请
    psiArray = (PSTUDENT_INFO)malloc(sizeof(STUDENT_INFO) * TXT_ROW);
    if(psiArray == NULL)
    {
        printf("内存分配失败。\n");
        exit(-1);
    }
    else
    {
        // 内存初始化
        init(psiArray, TXT_ROW);

        // 读取 TXT 文件中的数据
        fpFile = fopen(file_name, "r");
        if(fpFile != NULL)
        {
            // 丢弃首行的列标签
            fscanf(fpFile, "%s\n", szLines);

            for(int i=0; i<TXT_ROW; i++)
            {
                int status;
                
                // 读取ID
                status = fscanf(fpFile, "%d,", &((psiArray[i]).id));
                if(status == 0)
                {
                    fgetc(fpFile);
                }
				if(psiArray[i].id!=-1 && psiArray[i].id<202000)
				{
					psiArray[i].id += 202000;
				}

                // 读取姓名、城市、性别
                status = fscanf(fpFile, "%[^,],", psiArray[i].name);
                if(status == 0)
                {
                    fgetc(fpFile);
                }
                status = fscanf(fpFile, "%[^,],", psiArray[i].city);
                if(status == 0)
                {
                    fgetc(fpFile);
                }
                status = fscanf(fpFile, "%[^,],", psiArray[i].gender);
                if(status == 0)
                {
                    fgetc(fpFile);
                }
				if(strncmp(psiArray[i].gender, "male", 4) == 0)
				{
					strncpy(psiArray[i].gender, "boy\0", 4);
				}
				else if(strncmp(psiArray[i].gender, "female", 6) == 0)
				{
					strncpy(psiArray[i].gender, "girl\0\0", 6);
				}

                // 读取身高
                status = fscanf(fpFile, "%lf,", &((psiArray[i]).height));
                if(status == 0)
                {
                    fgetc(fpFile);
                }
				if(psiArray[i].height!=-1 && psiArray[i].height<3)
				{
					psiArray[i].height *= 100;
				}

                // 读取成绩
                for(int j=0; j<10; j++)
                {
                    status = fscanf(fpFile, "%lf,", &((psiArray[i]).score[j]));
                    if(status == 0)
                    {
                        fgetc(fpFile);
                    }
                }

                // 读取评价
                fpos_t fpos_tPosition;
                fgetpos(fpFile, &fpos_tPosition);
                if(fgetc(fpFile) == 10)
                {
                    continue;
                }
                else
                {
                    fsetpos(fpFile, &fpos_tPosition);
                    status = fscanf(fpFile, "%s\n", psiArray[i].constitution);
                }
            }

            fclose(fpFile);
            
            return psiArray;
        }
        else
        {
            printf("打开文件 %s 失败", file_name);
            exit(-1);
        }
    }
}

PSTUDENT_INFO combination(PSTUDENT_INFO psiArrayOne, int iLengthOne, PSTUDENT_INFO psiArrayTwo, int iLengthTwo)
{
	PSTUDENT_INFO psiArrayTotal=NULL;

	psiArrayTotal = (PSTUDENT_INFO)malloc(sizeof(STUDENT_INFO)*(iLengthOne+iLengthTwo));
	if(psiArrayTotal == NULL)
	{
		printf("内存分配失败。\n");
		exit(-1);
	}
	init(psiArrayTotal, iLengthOne+iLengthTwo);

	for(int i=0,j=0; i<iLengthOne||j<iLengthTwo;)
	{
		if(psiArrayOne[i].id == psiArrayTwo[j].id)
		{
			// 整合ID 
			psiArrayTotal[iRealRow].id = psiArrayOne[i].id;

			// 整合姓名、城市、性别
			if(strncmp(psiArrayOne[i].name, "null", 4) != 0)
			{
				strcpy(psiArrayTotal[iRealRow].name, psiArrayOne[i].name);
			}
			else
			{
				strcpy(psiArrayTotal[iRealRow].name, psiArrayTwo[j].name);
			}
			if(strncmp(psiArrayOne[i].city, "null", 4) != 0)
			{
				strcpy(psiArrayTotal[iRealRow].city, psiArrayOne[i].city);
			}
			else
			{
				strcpy(psiArrayTotal[iRealRow].city, psiArrayTwo[j].city);
			}
			if(strncmp(psiArrayOne[i].gender, "null", 4) != 0)
			{
				strcpy(psiArrayTotal[iRealRow].gender, psiArrayOne[i].gender);
			}
			else
			{
				strcpy(psiArrayTotal[iRealRow].gender, psiArrayTwo[j].gender);
			}

			// 整合身高
			if(psiArrayOne[i].height != -1)
			{
				psiArrayTotal[iRealRow].height = psiArrayOne[i].height;
			}
			else
			{
				psiArrayTotal[iRealRow].height = psiArrayTwo[j].height;
			}

			// 整合成绩
			for(int k=0; k<10; k++)
			{	
				if(psiArrayOne[i].score[k] != -1)
				{
					psiArrayTotal[iRealRow].score[k] = psiArrayOne[i].score[k];
				}
				else
				{
					psiArrayTotal[iRealRow].score[k] = psiArrayTwo[j].score[k];
				}
			}

			// 整合评价
			if(strncmp(psiArrayOne[i].constitution, "null", 4) != 0)
			{
				strcpy(psiArrayTotal[iRealRow].constitution, psiArrayOne[i].constitution);
			}
			else
			{
				strcpy(psiArrayTotal[iRealRow].constitution, psiArrayTwo[j].constitution);
			}

			i += 1;
			j += 1;
		}
		else
		{
			if(psiArrayOne[i].id < psiArrayTwo[j].id)
			{
				memcpy(&psiArrayTotal[iRealRow], &psiArrayOne[i], sizeof(STUDENT_INFO));

				i += 1;
			}
			else
			{
				memcpy(&psiArrayTotal[iRealRow], &psiArrayTwo[j], sizeof(STUDENT_INFO));

				j += 1;
			}
		}
		iRealRow += 1;
	}

	int iTempRow=0;
	// 整合相邻的相同ID的数据
	for(int i=1; i<iRealRow; i++)
	{
		if(psiArrayTotal[iTempRow].id == psiArrayTotal[i].id)
		{
			// 整合姓名、城市、性别
			if(strncmp(psiArrayTotal[iTempRow].name, "null", 4) == 0)
			{
				strcpy(psiArrayTotal[iTempRow].name, psiArrayTotal[i].name);
			}
			if(strncmp(psiArrayTotal[iTempRow].city, "null", 4) == 0)
			{
				strcpy(psiArrayTotal[iTempRow].city, psiArrayTotal[i].city);
			}
			if(strncmp(psiArrayTotal[iTempRow].gender, "null", 4) == 0)
			{
				strcpy(psiArrayTotal[iTempRow].gender, psiArrayTotal[i].gender);
			}

			// 整合身高
			if(psiArrayTotal[iTempRow].height == -1)
			{
				psiArrayTotal[iTempRow].height = psiArrayTotal[i].height;
			}

			// 整合成绩
			for(int j=0; j<10; j++)
			{
				if(psiArrayTotal[iTempRow].score[j] == -1)
				{
					psiArrayTotal[iTempRow].score[j] = psiArrayTotal[i].score[j];
				}
			}

			// 整合评价
			if(strncmp(psiArrayTotal[iTempRow].constitution, "null", 4) == 0)
			{
				strcpy(psiArrayTotal[iTempRow].constitution, psiArrayTotal[i].constitution);
			}
		}
		else
		{
			iTempRow += 1;
			
			memcpy(&psiArrayTotal[iTempRow], &psiArrayTotal[i], sizeof(STUDENT_INFO));
		}
	}

	iRealRow = iTempRow+1;

	return psiArrayTotal;
}

void print(PSTUDENT_INFO psiArray, int iRow)
{
    printf("ID  \t name    \t city    \t gender \t height \t c1 \t c2 \t c3 \t c4 \t c5 \t c6 \t c7 \t c8 \t c9 \t c10 \t constitution \n\n");
    for(int i=0; i<iRow; i++)
    {
        printf("%d\t %s    \t %s \t %s    \t %.2f \t %.2f \t %.2f \t %.2f \t %.2f \t %.2f \t %.2f \t %.2f \t %.2f \t %.2f \t %.2f \t %s \n\n",
            psiArray[i].id, psiArray[i].name, psiArray[i].city, psiArray[i].gender,
            psiArray[i].height, psiArray[i].score[0], psiArray[i].score[1],
            psiArray[i].score[2], psiArray[i].score[3], psiArray[i].score[4],
            psiArray[i].score[5], psiArray[i].score[6], psiArray[i].score[7],
            psiArray[i].score[8], psiArray[i].score[9], psiArray[i].constitution);
    }
}

void save_file_csv(PSTUDENT_INFO psiArray, int iRow)
{
	FILE *fpFile=NULL;

	fpFile = fopen("../data/final_data.csv", "w");
	if(fpFile == NULL)
	{
		if(fpFile == NULL)
		{
			printf("创建文件 final_data_csv 失败。\n");
			exit(-1);
		}
	}

	fprintf(fpFile, "ID,name,city,gender,height,C1,C2,C3,C4,C5,C6,C7,C8,C9,C10,constitution\n");
	for(int i=0; i<iRow; i++)
	{
		fprintf(fpFile, "%d,%s,%s,%s,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%s\n",
				psiArray[i].id, psiArray[i].name, psiArray[i].city, psiArray[i].gender,
				psiArray[i].height, psiArray[i].score[0], psiArray[i].score[1],
				psiArray[i].score[2], psiArray[i].score[3], psiArray[i].score[4],
				psiArray[i].score[5], psiArray[i].score[6], psiArray[i].score[7],
				psiArray[i].score[8], psiArray[i].score[9], psiArray[i].constitution);
	}

	fclose(fpFile);
}


// 实现解决设置问题的函数
void print_beijing_average_score(PSTUDENT_INFO psiArray, int iRow)
{
	int *iArrayBeijingIndex=NULL;
	int iCount=0;
	double dfArrayAverageScore[10];

	// 初始化
	iArrayBeijingIndex = (int *)malloc(sizeof(int)*iRow);
	if(iArrayBeijingIndex == NULL)
	{
		printf("内存分配失败。\n");
		exit(-1);
	}
	for(int i=0; i<10; i++)
	{
		dfArrayAverageScore[i] = 0;
	}
	for(int i=0; i<iRow; i++)
	{
		if(strncmp(psiArray[i].city, "Beijing", 7) == 0)
		{
			iArrayBeijingIndex[iCount] = i;
			iCount += 1;
		}
	}

	// 计算平均值
	for(int i=0; i<iCount; i++)
	{
		for(int j=0; j<10; j++)
		{
			dfArrayAverageScore[j] += psiArray[iArrayBeijingIndex[i]].score[j];
		}
	}
	printf("家乡在 Beijing 的同学共有 %d 个。\n", iCount);
	printf("他们总的各科平均分为：\n");
	printf("C1 \t C2 \t C3 \t C4 \t C5 \t C6 \t C7 \t C8 \t C9 \t C10\n");
	if(iCount != 0)
	{
		for(int i=0; i<10; i++)
		{
			dfArrayAverageScore[i] /= iCount;
			printf("%.2f \t ", dfArrayAverageScore[i]);
		}
		printf("\n\n");
	}
	
	free(iArrayBeijingIndex);
	iArrayBeijingIndex = NULL;
}

int count_guangzhou_score_number(PSTUDENT_INFO psiArray, int iRow)
{
	int iCount=0;

	for(int i=0; i<iRow; i++)
	{
		if(strncmp(psiArray[i].city, "Guangzhou", 9)==0 && psiArray[i].score[0]>80 && psiArray[i].score[8]>9)
		{
			iCount += 1;
		}
	}

	return iCount;
}

void print_constitution_compare_between_the_girl_of_guangzhou_and_shanghai(PSTUDENT_INFO psiArray, int iRow)
{
	double dfScoreGuangzhou=0;
	int iNumberGuangzhou=0;
	double dfScoreShanghai=0;
	int iNumberShanghai=0;

	for(int i=0; i<iRow; i++)
	{
		if(strncmp(psiArray[i].city, "Guangzhou", 9) == 0)
		{
			iNumberGuangzhou += 1;

			if(strncmp(psiArray[i].constitution, "bad", 3) == 0)
			{
				dfScoreGuangzhou += 1;
			}
			else if(strncmp(psiArray[i].constitution, "general", 7) == 0)
			{
				dfScoreGuangzhou += 2;
			}
			else if(strncmp(psiArray[i].constitution, "good", 4) == 0)
			{
				dfScoreGuangzhou += 3;
			}
			else if(strncmp(psiArray[i].constitution, "excellent", 9) == 0)
			{
				dfScoreGuangzhou += 4;
			}
			else
			{
				dfScoreGuangzhou += 0;
			}
		}
		else if(strncmp(psiArray[i].city, "Shanghai", 8) == 0)
		{
			iNumberShanghai += 1;

			if(strncmp(psiArray[i].constitution, "bad", 3) == 0)
			{
				dfScoreShanghai += 1;
			}
			else if(strncmp(psiArray[i].constitution, "general", 7) == 0)
			{
				dfScoreShanghai += 2;
			}
			else if(strncmp(psiArray[i].constitution, "good", 4) == 0)
			{
				dfScoreShanghai += 3;
			}
			else if(strncmp(psiArray[i].constitution, "excellent", 9) == 0)
			{
				dfScoreShanghai += 4;
			}
			else
			{
				dfScoreShanghai += 0;
			}
		}
	}

	if(iNumberGuangzhou != 0)
	{
		dfScoreGuangzhou /= iNumberGuangzhou;
	}
	if(iNumberShanghai != 0)
	{
		dfScoreShanghai /= iNumberShanghai;
	}

	printf("家乡在广州的女生，总人数为：%d, 体育总分为：%.2f, 体育平均分为：%.2f\n", iNumberGuangzhou, iNumberGuangzhou*dfScoreGuangzhou, dfScoreGuangzhou);
	printf("家乡在上海的女生，总人数为：%d, 体育总分为：%.2f, 体育平均分为：%.2f\n", iNumberShanghai, iNumberShanghai*dfScoreShanghai, dfScoreShanghai);
	if(dfScoreGuangzhou == dfScoreShanghai)
	{
		printf("广州、上海两地女生的体育情况相当。\n\n");
	}
	else
	{
		if(dfScoreGuangzhou > dfScoreShanghai)
		{
			printf("总体上，广州女生的体育强于上海女生的体育。\n\n");
		}
		else
		{
			printf("总体上，上海女生的体育强于广州女生的体育。\n\n");
		}
	}
}

void correlation_calculation(PSTUDENT_INFO psiArray, int iRow)
{
	double dfArrayAverage[10], dfArrayStandardDeviation[10];
	double dfArrayCorrelationCoefficient[9];
	double dfArrayTemp[10][iRow];

	// 初始化
	for(int i=0; i<10; i++)
	{
		dfArrayAverage[i] = 0;
		if(i != 9)
		{
			dfArrayCorrelationCoefficient[i] = 0;
		}
	}

	// 各科成绩转化存储
	for(int i=0; i<iRow; i++)
	{
		for(int j=0; j<10; j++)
		{
			if(j != 9)
			{
				dfArrayTemp[j][i] = psiArray[i].score[j];
			}
			else
			{
				if(strncmp(psiArray[i].constitution, "bad", 3) == 0)
				{
					dfArrayTemp[j][i] = 1;
				}
				else if(strncmp(psiArray[i].constitution, "general", 7) == 0)
				{
					dfArrayTemp[j][i] = 2;
				}
				else if(strncmp(psiArray[i].constitution, "good", 4) == 0)
				{
					dfArrayTemp[j][i] = 3;
				}
				else if(strncmp(psiArray[i].constitution, "excellent", 9) == 0)
				{
					dfArrayTemp[j][i] = 4;
				}
				else
				{
					dfArrayTemp[j][i] = 0;
				}
			}

			dfArrayAverage[j] += dfArrayTemp[j][i];
		}
	}

	// 各科平均值计算
	for(int i=0; i<10; i++)
	{
		dfArrayAverage[i] /= iRow;
	}

	// 各科标准差计算
	for(int i=0; i<10; i++)
	{
		double dfRowSum=0;
		for(int j=0; j<iRow; j++)
		{
			dfRowSum += pow(dfArrayTemp[i][j]-dfArrayAverage[i], 2);
		}
		dfArrayStandardDeviation[i] = sqrt(dfRowSum/iRow);
	}

	// a[k]计算
	for(int i=0; i<10; i++)
	{
		for(int j=0; j<iRow; j++)
		{
			if(dfArrayStandardDeviation[i] != 0)
			{
				dfArrayTemp[i][j] = (dfArrayTemp[i][j]-dfArrayAverage[i])/dfArrayStandardDeviation[i];
			}
			else
			{
				dfArrayTemp[i][j] = 0;
			}
		}
	}

	// 各科成绩数组与体育评价数组的相关系数计算
	for(int i=0; i<9; i++)
	{
		for(int j=0; j<iRow; j++)
		{
			dfArrayCorrelationCoefficient[i] += dfArrayTemp[i][j]*dfArrayTemp[9][j];
		}
	}
	printf("C1 ~ C9 课程成绩与体育评价各自的相关系数如下：\n");
	printf("C1\tC2\tC3\tC4\tC5\tC6\tC7\tC8\tC9\n");
	for(int i=0; i<9; i++)
	{
		printf("%.2f\t", dfArrayCorrelationCoefficient[i]);
	}
	printf("\n\n");
}
