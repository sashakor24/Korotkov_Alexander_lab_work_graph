#include<iostream>
using namespace std;

struct Array2D  // NxN
{
	uint size;
	int** arr = NULL;
	int* operator[](uint i)
	{
		return arr[i];
	}
};

void createArr(Array2D& a, uint size, int init = 0)
{
	a.arr = new int*[size];
	for (uint i = 0; i < size; ++i)
		a.arr[i] = new int[size];
	a.size = size;
	for (uint i = 0; i < size; ++i)
		for (uint j = 0; j < size; ++j)
			a.arr[i][j] = init;
}

void deleteArr(Array2D& a)
{
	for (uint i = 0; i < a.size; ++i)
		delete[] a.arr[i];
	delete[] a.arr;
	a.size = 0;
}



int main()
{
	Array2D graph;
	uint size;
	cout << "Введите количесво вершин графа:" << endl;
	cin >> size;
	createArr(graph, size);
	int i,j;
	cout << "Введите попарно связанные вершины (-1 - остановка цикла):" << endl;
	while(1)
	{
		cin >> i;
		if (i == -1 || i >= size)
			break;
		cin >> j;
		if (j == -1 || j >= size)
			break;
		graph[i][j] = 1;
		graph[j][i] = 1;
	}
	cout << "связи:" << endl;
	for (uint i = 0; i < size; ++i)
		for (uint j = i + 1; j < size; ++j)
			if (graph[i][j] != 0)
				cout << i << " " << j << endl;
	deleteArr(graph);
	return 0;
} 
