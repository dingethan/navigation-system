#include<iostream>
#include<vector>
using namespace std;
int main()
{
	int n;
	cin >> n;
	vector<vector<int>>matrix(n, vector<int>(n));
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			int temp; cin >> temp;
			matrix[i][j] = temp;
		}
	}
	cout<<"矩阵为："<<endl;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			cout << matrix[i][j] << " ";
		}
		cout << endl;
	}
	// 顺时针旋转90°
	for (int layer = 0; layer < n / 2; layer++)
	{
		int first = layer;	//最左端 
		int last = n - 1 - layer;	//最右端
		for (int i = first; i < last; i++)
		{
			int offset = i - first;	//偏移量（关键变量）！！
			int top = matrix[first][i];	//保存第first行第i列的元素 
			//左上、右上、右下、左下、左上
			matrix[first][i] = matrix[last - offset][first];//左下-左上 
			matrix[last - offset][first] = matrix[last][last - offset];//右下-左下 
			matrix[last][last-offset] = matrix[i][last];//右上-右下
			matrix[i][last] = top;// 左上-右上
		}
	}
	cout<<"旋转90°后矩阵为："<<endl;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			cout << matrix[i][j] << " ";
		}
		cout << endl;
	}
	return 0;
}