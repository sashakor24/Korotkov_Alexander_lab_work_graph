#include "graph.hpp"

GraphNOList::EdgeNO::EdgeNO(uint n1, uint n2, int w)
{
	number1 = n1;
	number2 = n2;
	weight = w;
	next = NULL;
}
GraphNOList::EdgeNO* GraphNOList::findPrevOf(uint n1, uint n2)
{
	if (!first || first->number1 == n1 && first->number2 == n2
	|| first->number1 > n1 || first->number1 == n1 && first->number2 > n2 || !first->next)
		return first;
	EdgeNO* prev = first;
	while (prev->next && prev->next->number1 < n1)
		prev = prev->next;
	if (!prev->next || prev->next->number2 >= n2)
		return prev;
	while (prev->next && prev->next->number1 == n1 && prev->next->number2 < n2)
		prev = prev->next;
	return prev;
}
void GraphNOList::insert(uint n1, uint n2, int w /*= 1*/)
{
	if (n1 < n2)
		swap(n1, n2);
	EdgeNO* prev = findPrevOf(n1, n2);
	if (!prev)
	{
		first = new EdgeNO(n1, n2, w);
		return;
	}
	if (prev == first)
	{
		if (prev->number1 == n1 && prev->number2 == n2)
			return;
		if (prev->number1 > n1 || prev->number1 == n1 && prev->number2 > n2)
		{
			EdgeNO* next = first;
			first = new EdgeNO(n1, n2, w);
			first->next = next;
			return;
		}
	}
	if (!prev->next)
	{
		prev->next = new EdgeNO(n1, n2, w);
		return;
	}
	if (prev->next->number1 != n1 || prev->next->number2 != n2)
	{
		EdgeNO* next = prev->next;
		prev->next = new EdgeNO(n1, n2, w);
		prev->next->next = next;
	}
}
GraphNOList::EdgeNO* GraphNOList::findV(uint vertex, EdgeNO* exepting /*= NULL*/)
{
	if (!first)
		return NULL;
	EdgeNO* cur = first;
	while (cur && (cur == exepting || cur->number1 != vertex && cur->number2 != vertex))
		cur = cur->next;
	return cur;
}
void GraphNOList::printPath(uint from, uint to)
{
	int max = maxVertex() + 1;
	if (max <= 1 || from > max - 1 || to > max - 1)
		return;
	string out = "";
	EdgeNO* cur = findV(from);
	if (cur->number1 == from)
		out += to_string(cur->number1) + " ";
	else
		out += to_string(cur->number2) + " ";
	uint dest = from;
	while (max > 0 && cur)
	{
		if (cur->number1 == from)
		{
			dest = cur->number2;
			out += to_string(dest) + " ";
			cur = findV(dest, cur);
		}
		else
		{
			dest = cur->number1;
			out += to_string(dest) + " ";
			cur = findV(dest, cur);
		}
		--max;
	}
	if (max >= 0 && dest == to)
		cout << out << endl;
	else cout << "Ошибка. Данная структура не является путём" << endl;
}
void GraphNOList::deleteEdge(uint n1, uint n2)
{
	if (!first) return;
	if (n1 < n2)
		swap(n1, n2);
	EdgeNO* prev = findPrevOf(n1, n2);
	if (prev == first && first->number1 == n1 && first->number2 == n2)
	{
		EdgeNO* next = first->next;
		delete first;
		first = next;
		return;
	}
	if (!prev->next || prev->next->number1 != n1 || prev->next->number2 != n2)
		return;
	EdgeNO* next = prev->next->next;
	delete prev->next;
	prev->next = next;
}
GraphNOList::GraphNOList()
{
	first = NULL;
}
GraphNOList::GraphNOList(uint n1, uint n2, int w /*= 1*/)
{
	first = NULL;
	insert(n1, n2, w);
}
GraphNOList::GraphNOList(const Graph& g)
{
	first = NULL;
	for (uint i = 0; i < g.size; ++i)
		for (uint j = i + 1; j < g.size; ++j)
			if (g.get(i, j))
				insert(i, j, g.get(i, j));
}
GraphNOList::~GraphNOList()
{
	EdgeNO* next;
	while (first)
	{
		next = first->next;
		delete first;
		first = next;
	}
}
int GraphNOList::maxVertex()
{
	if (!first)
		return -1;
	uint m = 0;
	EdgeNO* cur = first;
	while(cur)
	{
		if (cur->number1 > m)
			m = cur->number1;
		if (cur->number2 > m)
			m = cur->number2;
		cur = cur->next;
	}
	return m;
}
int GraphNOList::minVertex()
{
	if (!first)
		return -1;
	uint m = maxVertex();
	EdgeNO* cur = first;
	while(cur)
	{
		if (cur->number1 < m)
			m = cur->number1;
		if (cur->number2 < m)
			m = cur->number2;
		cur = cur->next;
	}
	return m;
}
void GraphNOList::Balance()
{
	int shift = minVertex();
	if(shift > 0)
	{
		EdgeNO* cur = first;
		while (cur)
		{
			cur->number1 -= shift;
			cur->number2 -= shift;
			cur = cur->next;
		}
	}
}
int** GraphNOList::getArray(uint& size)
{
	Balance();
	size = maxVertex() + 1;
	if(!size)
		return NULL;

	int** graph = new int*[size];
	for (uint i = 0; i < size; ++i)
		graph[i] = new int[size];
	for (uint i = 0; i < size; ++i)
		for (uint j = 0; j < size; ++j)
			graph[i][j] = 0;
	EdgeNO* cur = first;
	while(cur)
	{
		graph[cur->number1][cur->number2] = cur->weight;
		graph[cur->number2][cur->number1] = cur->weight;
		cur = cur->next;
	}
	return graph;
}
void GraphNOList::printPairs()
{
	EdgeNO* cur = first;
	while (cur)
	{
		cout << '(' << cur->number1 << ", " << cur->number2 << ')' << ' ';
		cur = cur->next;
	}
	cout << endl;
}
void GraphNOList::printMatrix()
{
	uint size;
	int** a = getArray(size);
	if (!size)
		return;
	cout << "_|";
	for (uint i = 0; i < size; ++i)
		cout << i << ' ';
	cout << endl;
	for (uint i = 0; i <= 2 * size; ++i)
		cout << '-';
	cout << endl;
	for (uint i = 0; i < size; ++i)
	{
		cout << i << '|';
		for (uint j = 0; j < size; ++j)
			if (a[i][j])
				cout << a[i][j] << ' ';
			else cout << "- ";
		cout << endl;
	}
	cout << endl;
	for (uint i = 0; i < size; ++i)
		delete[] a[i];
	delete[] a;
}

void operator+=(GraphNOList& l, GraphNOList& r)
{
	GraphNOList::EdgeNO* cur = r.first;
	while(cur)
		l.insert(cur->number1, cur->number2, cur->weight);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Graph::Graph()
{
	size = 0;
	graph = NULL;
}
Graph::Graph(uint n)
{
	size = n;
	if (size)
	{
		graph = new int*[size];
		for (auto i = 0; i < size; ++i)
			graph[i] = new int[size];
		for (auto i = 0; i < size; ++i)
			for (auto j = 0; j < size; ++j)
				graph[i][j] = 0;
	} else graph = NULL;
}
Graph::Graph(string const& path)
{
	ifstream file(path);
	if(file.is_open())
	{
		file >> size;
		if (size)
		{
			graph = new int*[size];
			for (uint i = 0; i < size; ++i)
			graph[i] = new int[size];
			for (uint i = 0; i < size; ++i)
				for (auto j = 0; j < size; ++j)
					file >> graph[i][j];
			for (uint i = 0; i < size; ++i)
				graph[i][i] = 0;
		} else graph = NULL;
	} else
	{
		cerr << "Ошибка открытия файла" << endl;
		exit(0);
	}
}
Graph::Graph(GraphNOList& g)
{
	graph = g.getArray(size);
}

void Graph::loadGraph(string const& path)
{
	if (size)
	{
		for (auto i = 0; i < size; ++i)
			delete[] graph[i];
		delete[] graph;
	}
	ifstream file(path);
	if(file.is_open())
	{
		file >> size;
		graph = new int*[size];
		for (uint i = 0; i < size; ++i)
		graph[i] = new int[size];
		for (uint i = 0; i < size; ++i)
			for (auto j = 0; j < size; ++j)
				file >> graph[i][j];
		for (uint i = 0; i < size; ++i)
			graph[i][i] = 0;
	} else
	{
		cerr << "Ошибка открытия файла" << endl;
		exit(0);
	}
}
Graph::~Graph()
{
	for (auto i = 0; i < size; ++i)
		delete[] graph[i];
	delete[] graph;
}
int* Graph::operator[](uint i)
{
	return graph[i];
}
int Graph::get(uint i, uint j)
{
	return graph[i][j];
}
const int Graph::get(uint i, uint j) const
{
	return graph[i][j];
}
uint Graph::numberOfEdgesOfVertex(uint v)
{
	uint ans = 0;
	for (uint i = 0; i < size; ++i)
		if (graph[v][i] != 0)
		//if (i != v && graph[v][i] != 0) зависит от того,
		//рассматриваем ли мы кольца из вершины в саму себя
			++ans;
	return ans;
}
void Graph::routes(uint v1, uint v2)
{
	if (size == 0 || v1 > size - 1 || v2 > size - 1)
		cout << "0" << endl;
	else if (hasCicle(v2))// цикл, за исключением v2
		cout << "inf" << endl;
	else
		cout << countRoutes(v1, v2) << endl;
}
bool Graph::isFullGraph()
{
	for (uint i = 0; i < size; ++i)
		for (uint j = 0; j < size; ++j)
			if (i != j && graph[i][j] == 0)
				return false;
	return true;	
}
bool Graph::isWeighted()
{
	for (uint i = 0; i < size; ++i)
		for (uint j = 0; j < size; ++j)
			if (graph[i][j] != 0 && graph[i][j] != 1)
				return true;
	return false;
}
bool Graph::isSymmetric()
{
	for (auto i = 0; i < size; ++i)
		for (auto j = i + 1; j < size; ++j)
			if (graph[i][j] != graph[j][i]) 
				return false;
	return true;
}
bool Graph::hasCicle(int exeption /*= -1*/)
{
	if (size == 0) return false;
	bool* been = new bool[size]; // вершины, которые уже встречались
	for (uint i = 0; i < size; ++i)
	{
		for (uint j = 0; j < size; ++j)
			been[j] = false;
		if (searchCicle(been, i, exeption))
		{
			delete[] been;
			return true;
		}
	}
	delete[] been;
	return false;
}
void Graph::getGraphType()
{
	if (size == 0)
	{
		cout << "Пустой граф" << endl;
		return;
	}
	bool hasFreeVertex = false;
	bool hasUnfreeVertex = false;
	for (uint i = 0; i < size; ++i)
		for (uint j = 0; j < size; ++j)
		{
			if (i != j && graph[i][j] == 0)
				hasFreeVertex = true;
			else if (i != j && graph[i][j] != 0)
				hasUnfreeVertex = true;
		}
	if (hasFreeVertex && hasUnfreeVertex)
		cout << "Не все вершины имеют рёбра" << endl;
	else if (hasFreeVertex)
	{ 
		cout << "Пустой граф" << endl;
		return;
	}
	else cout << "Каждая вершина имеет рёбра" << endl;
	if (isFullGraph()) cout << "полный граф (каждая вершина соединена с каждой хотя бы в одну сторону)" << endl;
	else cout << "Неполный граф (не существует пути между какими-либо двумя вершинами)" << endl;
	if (isWeighted()) cout << "Взвешенный(нагруженный) граф" << endl;
	else cout << "Невзвешенный(ненагруженный) граф" << endl;
	if (isSymmetric())
		cout << "Неориентированный(симметричный) граф" << endl;
	else cout << "Ориентированный(несимметричный) граф" << endl;
	if (hasCicle()) cout << "Цикличный граф" << endl;
	else cout << "Граф-дерево" << endl;

}
void Graph::insertSym(uint i, uint j, uint w /*= 1*/)
{
	if (i >= size || j >= size)
		return;
	graph[i][j] = w;
	graph[j][i] = w;
}
void Graph::insert(uint i, uint j, uint w /*= 1*/)
{
	if (i >= size || j >= size)
		return;
	graph[i][j] = w;
}
void Graph::print()
{
	if (!size)
		return;
	cout << "_|";
	for (uint i = 0; i < size; ++i)
		cout << i << ' ';
	cout << endl;
	for (uint i = 0; i <= 2 * size; ++i)
		cout << '-';
	cout << endl;
	for (uint i = 0; i < size; ++i)
	{
		cout << i << '|';
		for (uint j = 0; j < size; ++j)
			if (graph[i][j])
				cout << graph[i][j] << ' ';
			else cout << "- ";
		cout << endl;
	}
	cout << endl;
}
bool Graph::searchCicle(bool* been, uint vertex, int exeption, int from /*= -1*/)
//только для графов, у которых вершины не зациклены сами на себя
{
	if (vertex != exeption)//необходимо для алгоритма подсчёта путей, чтобы не включать exeption вершину
	{
		been[vertex] = true;
		for (uint i = 0; i < size; ++i)
			if (i != vertex && graph[vertex][i] != 0)
// если добавить условие i != vertex, то мы не будем учитывать циклы из вершины в саму себя
			{
				if (!been[i])
					searchCicle(been, i, exeption, vertex);
				else if (i != from)
					return true;
			}
	}
	return false;
}

GraphNOList* Graph::searchDeep(uint from, uint to)
{
	if (size <= 1 || from == to)
		return new GraphNOList();
	if (graph[from][to])
		return new GraphNOList(from, to, graph[from][to]);
	bool* been = new bool[size];
	for (uint i = 0; i < size; ++i)
	{
		GraphNOList* ans = new GraphNOList();
		for(uint j = 0; j < size; ++j)
			if (j == from)
				been[j] = true;
			else been[j] = false;
		if(from != i && graph[from][i] && doSearchDeep(*ans, been, i, to))
		{
			ans->insert(from, i, graph[from][i]);
			delete[] been;
			return ans;
		}
		delete ans;
	}
	return new GraphNOList();
}
GraphNOList* Graph::searchWide(uint from, uint to)
{
	if (size <= 1)
		return new GraphNOList();
	if (graph[from][to])
		return new GraphNOList(from, to, graph[from][to]);
	bool* been = new bool[size];
	for(uint i = 1; i < size; ++i)
	{
		GraphNOList* ans = new GraphNOList();
		for(uint j = 0; j < size; ++j)
			been[j] = false;
		if(doSearchWide(*ans, been, from, to, i))
		{
			delete[] been;
			return ans;
		}
		delete ans;
	}
	return new GraphNOList();
}
bool Graph::doSearchDeep(GraphNOList& ans, bool* been, uint v1, uint v2)
{
	if (v1 == v2)
		return true;
	if (graph[v1][v2])
	{
		ans.insert(v1, v2, graph[v1][v2]);
		return true;
	}
	been[v1] = true;
	for (uint i = 0; i < size; ++i)
		if (!been[i] && graph[v1][i] && doSearchDeep(ans, been, i, v2))
		{
			ans.insert(v1, i, graph[v1][i]);
			return true;
		}
	return false;
}
bool Graph::doSearchWide(GraphNOList& ans, bool* been, uint v1, uint v2, uint wave)
{
	if (v1 == v2)
		return true;
	if (wave == 0)
		return false;
	been[v1] = true;
	for (uint i = 0; i < size; ++i)
		if (!been[i] && graph[v1][i] && doSearchWide(ans, been, i, v2, wave - 1))
		{
			ans.insert(v1, i, graph[v1][i]);
			return true;
		}
	return false;
			
}

uint Graph::countRoutes(uint v1, uint v2, int from /*= -1*/)
{
	if (v1 == v2) return 1;
	uint s = 0;
	for (uint i = 0; i < size; ++i)
		if (i != from && graph[v1][i] != 0) s += countRoutes(i, v2, v1);
	return s;
}

Substitution::Substitution(Substitution& other)
{
	size = other.size;
	order = new uint[size];
	edge = new uint[size];
	for (uint i = 0; i < size; ++i)
	{
		order[i] = other.order[i];
		edge[i] = other.edge[i];
	}
}
Substitution::Substitution(uint n)
{
	size = n;
	order = new uint[size];
	edge = new uint[size];
	for (uint i = 0; i < size; ++i)
	{
		order[i] = i;
		edge[i] = 0;
	}
}
Substitution::~Substitution()
{
	delete[] order;
	delete[] edge;
}
uint* Substitution::getArray()
{
	uint* a = new uint[size];
	for (uint i = 0; i < size; ++i)
		a[i] = edge[i];
	return a;
}
void Substitution::sort(int maxInd /*= -1*/)// сортировка : переходы подстановки остаются не тронутыми,
//меняется только их порядок
{
	if (maxInd == -1)
		merge(*this, 0, size - 1);
	else 
		merge(*this, 0, maxInd);
}

void Substitution::print()
{
	for (uint i = 0; i < size; ++i)
		cout << order[i] << ' ';
	cout << endl;
	for (uint i = 0; i < size; ++i)
		cout << edge[i] << ' ';
	cout << endl;
}
void Substitution::swap(uint i, uint j)
{
	uint t = order[i];
	order[i] = order[j];
	order[j] = t;
	t = edge[i];
	edge[i] = edge[j];
	edge[j] = t;
}
void Substitution::copy(Substitution& sub1, uint i1, Substitution& sub2, uint i2)
{
	sub1.edge[i1] = sub2.edge[i2];
	sub1.order[i1] = sub2.order[i2];
}
void Substitution::merge(Substitution& sub, const uint& l, const uint& r)
{
	if (l >= r) return;
	if (l == r - 1)
	{
		if(sub.edge[l] > sub.edge[r])
			swap(l, r);
		return;
	}
	uint m = (r - l) / 2 + l;
	merge(sub, l, m);
	merge(sub, m + 1, r);
	uint i1 = l;
	uint i2 = m + 1;
	Substitution subbuf(r - l + 1);
	uint k = 0;
	bool b = true;
	while(b)
	{
		if(i1 <= m && i2 <= r)
		{
			if(sub.edge[i1] > sub.edge[i2])
			{
				copy(subbuf, k, sub, i2);
				++i2;
			}
			else
			{
				copy(subbuf, k, sub, i1);
				++i1;
			}
			++k;
		} else if(i1 <= m)
		{
			copy(subbuf, k, sub, i1);
			++i1;
			++k;
		} else if(i2 <= r)
		{
			copy(subbuf, k, sub, i2);
			++i2;
			++k;
		} else b = false;
	}
	for(int j = 0; j < k; ++j)
		copy(sub, l + j, subbuf, j);
}

bool edgesAreEqual(int* from2to1, Graph& g1, uint v1, uint* edgebuf1,
 					Graph& g2, uint v2, uint* edgebuf2, set<uint> been1, set<uint> been2)
{
	if (from2to1[v2] == v1) return true; // для v2 уже нашли прообраз и он совпадает с v1
	else if (from2to1[v2] != -1) return false; // для v2 уже нашли прообраз и он НЕ совпадает с v1
	uint size = g1.size;
	uint numberOfEdges = edgebuf1[v1]; // степень вершины v1
	been1.insert(v1); // множество в вершин, в которых мы были
	been2.insert(v2);
	Substitution neighbors1(numberOfEdges); // подстановка индекса соседа и его степени (кол-ва соседей соседа)
	Substitution neighbors2(numberOfEdges);
	uint counter1 = 0;
	uint counter2 = 0;
	for (uint i = 0; i < size; ++i)
	{
		if (g1[v1][i] != 0 && !been1.count(i))  // если не были в i
		{
			neighbors1.edge[counter1] = edgebuf1[i];
			neighbors1.order[counter1] = i;
			++counter1;
		}
		if (g2[v2][i] != 0 && !been2.count(i))
		{
			neighbors2.edge[counter2] = edgebuf2[i];
			neighbors2.order[counter2] = i;
			++counter2;
		}
	}
	if (counter1 == 0 && counter2 == 0) 
		return true;
	uint success = 0;
	if (counter1 == counter2)
	{
		neighbors1.sort(counter1 - 1); // отсортировать подстановку до последнего элемента, который был вставлен
		neighbors2.sort(counter2 - 1);
		uint counter = 0;
		while(counter < counter1)
			if (neighbors1.edge[counter] != neighbors2.edge[counter])
				return false;
			else
			{
				uint edge = neighbors1.edge[counter];
				uint sup = counter + 1;//верхняя граница
				while(sup < counter1 && neighbors1.edge[sup] == edge && neighbors2.edge[sup] == edge)
					++sup;
				for (uint i = counter; i < sup; ++i)
					for (uint j = counter; j < sup; ++j)
						if (from2to1[neighbors2.order[j]] == neighbors1.order[i]
					 	|| edgesAreEqual(from2to1, g1, neighbors1.order[i], edgebuf1,
					 					g2, neighbors2.order[j], edgebuf2, been1, been2))
						{
							++success;
							break;
						}
				counter = sup;
			}
	}
	if (success == counter1 && success == counter2) // если все вершины соответствуют
		return true;
	return false;
}
bool areIsomorfic(Graph& g1, Graph& g2)// только для симметричных невзвешенных графов
{
	if (g1.size != g2.size) return false;
	if (g1.size == 0 && g2.size == 0) return true;
	uint size = g1.size;
	Substitution sub1(size), sub2(size); // подстановка индексов с количеством рёбер у i-той вершины
	for (uint i = 0; i < size; ++i)
	{
		sub1.edge[i] = g1.numberOfEdgesOfVertex(i);
		sub2.edge[i] = g2.numberOfEdgesOfVertex(i);
	}
	uint* edgebuf1 = sub1.getArray(); // буфер степеней(кол-ва соседей) у i вершины
	uint* edgebuf2 = sub2.getArray();
	sub1.sort();
	sub2.sort();
	int* from2to1 = new int[size]; // отображение индексов второго графа в индексы первого
	for (uint i = 0; i < size; ++i)
		from2to1[i] = -1; // отображение не определено
	for (uint i = 0; i < size; ++i)
		if (sub1.edge[i] != sub2.edge[i])
		{
			delete[] from2to1;
			delete[] edgebuf1;
			delete[] edgebuf2;
			return false;
		}
		else
			for (uint j = 0; j < size; ++j)
				if (from2to1[j] == -1 && edgesAreEqual(from2to1, g1, i, edgebuf1,
													g2, j, edgebuf2, set<uint>(), set<uint>()))
					from2to1[j] = i;
	for (uint i = 0; i < size; ++i)
		if (from2to1[i] == -1) // если отображение не биективно
		{
			delete[] from2to1;
			delete[] edgebuf1;
			delete[] edgebuf2;
			return false;
		}
	delete[] from2to1;
	delete[] edgebuf1;
	delete[] edgebuf2;
	return true;
}

uint getExp(uint x)
{
	uint ans = 1;
	while(x /= 10)
		++ans;
	return ans;
}

string createNSpaces(uint n)
{
	string * s = new string();
	for (uint i = 0; i < n; ++i)
		*s += " ";
	return *s;
}