#include <bits/stdc++.h>

using namespace std;



//读取文件内数据

double(*File_In_Data(const char filename[], int & col,int & row))[200]

{

	double(*data)[200] = new double[200][200];

	col = 0; row = 0;

	ifstream  file;

	file.open(filename, ios::in);

	string line;

	if (file.is_open())

	{

		while (getline(file, line))     //按行读取

		{

			row = 0;
            col++;

			stringstream ss(line);



			while (getline(ss, line, ','))      //用 , 分隔符分割

			{

				row++;

				data[col][row] = atof(line.c_str());

			}

			
		}

	}

	file.close();

	return data;

}


//计算欧式距离

double distance(double data1[200], double data2[200], int row)

{

	double Distance = 0;

	for (int i = 1; i <= row; i++)
    	Distance += ( (data1[i] - data2[i])* (data1[i] - data2[i]) );

	return sqrt(Distance);

}

//分类到最近的类

void classify(double data[][200], double kind[][200], int col, int row, int k, double & Distortion)

{

	Distortion = 0;	

	double transition;

	for (int len = 1; len <= col; len++)

	{

		transition = 1000000.0;

		for (int i = 1; i <= k; i++)

		{

			double Distance =distance(data[len], kind[i], row);

			if (Distance < transition)

			{

				transition = Distance;

				data[len][0] = i;
			}

		}

		Distortion += transition;

	}

}

//计算聚类中心

void calculate(double data[][200], double kind[][200], int col, int row, int k)

{


	for (int i = 1, j = 1; i <= k; i++)	

	{

		for ( j = 0; j <= row; j++)

		{

			kind[i][j] = 0;

		}

	}

	int k_len = 0;

	for (int i = 1, j = 1; i <= col; i++)

	{

		k_len = int(data[i][0]);

		for ( j = 1; j <= row; j++)

		{

			kind[k_len][j] += data[i][j];

			kind[k_len][0]++;
		}

	}

	for (int i = 1, j = 1; i <= k; i++)

	{

		for (j = 1; j <= row; j++)

		{

			kind[i][j] = kind[i][j] / kind[i][0];

		}

	}
    
}

//k-means算法

void k_means(double data[][200], int col, int row, int k)

{

	double(*kind)[200] = new double[200][200];

	srand((unsigned)time(NULL));

	int ran = rand() % int(col / k) + 1;

	for (int i = 1, j = 1, len; i <= k; i++)		

	{

		len = ran + (i - 1)*k;

		data[len][0] = i;

		for (j = 1; j <= row; j++)

		{

			kind[i][j] = data[len][j]; 

		}

	}
	double Distortion = 0;		//代价

	double Distortion_0 = Distortion;

	classify(data, kind, col, row, k, Distortion);

	while (Distortion_0 != Distortion)

	{

		Distortion_0 = Distortion;

		calculate(data, kind, col, row, k);

		classify(data, kind, col, row, k, Distortion);

	}
    cout<<"聚类中心:"<<endl;
    for (int i=1;i<=k;i++)
    {
        for (int j=1;j<=row;j++)
        {
            cout<<kind[i][j]<<",";
        }
        cout<<'\b'<<endl;
    }

    for (int i=1;i<=k;i++)
    {
        cout<<"第"<<i<<"类:"<<endl;
        for (int len=1;len<=col;len++)
        {
            if (data[len][0]==i)
            {
                for(int j=1;j<=row;j++)
                {
                    cout<<data[len][j]<<",";
                }
                cout<<'\b'<<endl;
            }
        }
    }

}



int main()

{
    double(*data)[200];      //数据
	int col = 0;     //数据行数

	int row = 0;       //数据列数


    //问题一
	const char filename[] = "../data/data.csv";

	data = File_In_Data(filename, col, row);
	cout << "************************* 实验 3.1 *************************"<<endl;
	cout << "行"<<col << "\t" <<"列"<< row << endl;
	int k;

	for ( k = 2; k <= 5; k++)

	{
		cout<<"---------------分为"<<k<<"类：-------------------------"<<endl;
		k_means(data, col, row, k);

	}


 	const char filename_2[] = "../data/data2.txt";
	data = File_In_Data(filename_2, col, row);
	cout << "************************* 实验 3.2 *************************"<<endl;
    	cout << "行"<<col << "\t" <<"列"<< row << endl;
	k = 0;
	for (k = 2; k <= 5; k++)
	{ 
        cout<<"-----------------------------分为"<<k<<"类：----------------------"<<endl;
		k_means(data, col, row, k);
	}

}
