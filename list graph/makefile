DEBUG = -g

app: main.o graph.o
	g++ $(DEBUG) -o app main.o graph.o
main.o: main.cpp
	g++ $(DEBUG) -c  main.cpp
graph.o: graph.cpp
	g++ $(DEBUG) -c  graph.cpp
rm:
	rm main.o graph.o app
