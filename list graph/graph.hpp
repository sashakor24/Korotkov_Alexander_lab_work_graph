#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <set>
#include <string>
#include <fstream>
using namespace std;

class GraphNOList;
struct Graph;


class GraphNOList // a not oriented graph (не ориентированный граф)
{
	struct EdgeNO
	{
		uint number1;
		uint number2;
		int weight;
		EdgeNO* next;
		EdgeNO(uint n1, uint n2, int w);
	};
	EdgeNO* first;
	//EdgeNO* findPrevForN1(uint n1);
	EdgeNO* findPrevOf(uint n1, uint n2);// возвращает предыдущий от элемента, который больше либо равен (n1, n2)
	//если такой не найден, вернёт последний элемент
	// если first == NULL || first->next == NULL, вернёт first 
	EdgeNO* findV(uint vertex, EdgeNO* exepting = NULL);
public:
	void insert(uint n1, uint n2, int w = 1);
	void printPath(uint from, uint to);
	friend void operator+=(GraphNOList& l, GraphNOList& r);
	void deleteEdge(uint n1, uint n2);
	GraphNOList();
	GraphNOList(uint n1, uint n2, int w = 1);
	GraphNOList(const Graph& g);
// только для симметричной матрицы(в противном случае будет рассмотрена только верхняя диагональ)
	~GraphNOList();
	int maxVertex();
	int minVertex();
	void Balance();
	int** getArray(uint& size);
	void printPairs();
	void printMatrix();
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Graph
{
	int** graph;
	uint size;
	Graph();
	Graph(int** a, uint n);
	Graph(uint n);
	Graph(string const& path);
	Graph(GraphNOList& g);
	void loadGraph(string const& path);
	~Graph();
	int* operator[](uint i);
	int get(uint i, uint j);
	const int get(uint i, uint j) const;
	uint numberOfEdgesOfVertex(uint v);
	void routes(uint v1, uint v2);// пути из v1 в v2
	bool isFullGraph();
	bool isWeighted();
	bool isSymmetric();
	bool hasCicle(int exeption = -1);
	void getGraphType();
	void insert(uint i, uint j, uint w = 1);
	void insertSym(uint i, uint j, uint w = 1);
	void print();
	friend bool areIsomorfic(Graph& g1, Graph& g2);
	GraphNOList* searchDeep(uint from, uint to);
	GraphNOList* searchWide(uint from, uint to);
private:
	bool doSearchDeep(GraphNOList& ans, bool* been, uint v1, uint v2);
	bool doSearchWide(GraphNOList& ans, bool* been, uint v1, uint v2, uint wave);
	bool searchCicle(bool* been, uint vertex, int exeption, int from = -1);
	uint countRoutes(uint v1, uint v2, int from = -1);
};

struct Substitution
{
	uint* order;
	uint* edge;
	uint size;
	Substitution(Substitution& other);
	Substitution(uint n);
	~Substitution();
	uint* getArray();
	void sort(int maxInd = -1);
	void print();
private:
	void swap(uint i, uint j);
	void copy(Substitution& sub1, uint i1, Substitution& sub2, uint i2);
	void merge(Substitution& sub, const uint& l, const uint& r);
};

bool edgesAreEqual(int* from2to1, Graph& g1, uint v1, uint* edgebuf1,
 					Graph& g2, uint v2, uint* edgebuf2, set<uint> been1, set<uint> been2);

uint getExp(uint x);
string createNSpaces(uint n);

#endif //GRAPH_HPP
 
