#include <iostream>
#include "graph.hpp"
using namespace std;
int main()
{
	uint size;
	cout << "Введите количесво вершин графа:" << endl;
	cin >> size;
	Graph graph(size);
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
	GraphNOList list(graph);
	cout << "Значения списка:" << endl;
	list.printPairs();
	cout << "Матрица связности с помощью списка:" << endl;
	list.printMatrix();
	cout << "Матрица связности с помошью графа:" << endl;
	graph.print();
	uint from, to;
	cout << "Введите начальную и конечную вершину (от 0 до size - 1):" << endl;
	cin >> from >> to;
	GraphNOList* wide = graph.searchWide(from, to);
	cout << "Поиск в ширину:" << endl;
	wide->printPath(from, to);
	delete wide;
	GraphNOList* deep = graph.searchDeep(from, to);
	cout << "Поиск в глубину:" << endl;
	deep->printPath(from, to);
	delete deep;
	return 0;
} 
