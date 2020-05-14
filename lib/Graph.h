/*
 * Graph.h
 */
#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <iostream>
#include <queue>
#include <list>
#include <limits>
#include <cmath>
#include "MutablePriorityQueue.h"

using namespace std;

template <class T> class Edge;
template <class T> class Graph;
template <class T> class Vertex;

#define INF std::numeric_limits<double>::max()

/************************* Vertex  **************************/

template <class T>
class Vertex {
	T info;						// content of the vertex
	vector<Edge<T> > adj;		// outgoing edges
	
	double dist = 0;
	Vertex<T> *path = NULL;
	int queueIndex = 0; 		// required by MutablePriorityQueue

	bool visited = false;		// auxiliary field
	bool processing = false;	// auxiliary field

	void addEdge(Vertex<T> *dest, double w);
    void addEdge(Vertex<T> *dest, double w, bool display);

public:
	Vertex(T in);
	T getInfo() const;
	double getDist() const;
	Vertex *getPath() const;
    vector<Edge<T> > getAdj() const;

	bool operator<(Vertex<T> & vertex) const; // // required by MutablePriorityQueue
	friend class Graph<T>;
	friend class MutablePriorityQueue<Vertex<T>>;
};


template <class T>
Vertex<T>::Vertex(T in): info(in) {}

template <class T>
vector<Edge<T> > Vertex<T>::getAdj() const {
    return adj;
}

/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */
template <class T>
void Vertex<T>::addEdge(Vertex<T> *d, double w) {
	adj.push_back(Edge<T>(d, w));
}

template <class T>
void Vertex<T>::addEdge(Vertex<T> *dest, double w, bool display) {
    adj.push_back(Edge<T>(dest, w, display));
}


template <class T>
bool Vertex<T>::operator<(Vertex<T> & vertex) const {
	return this->dist < vertex.dist;
}

template <class T>
T Vertex<T>::getInfo() const {
	return this->info;
}

template <class T>
double Vertex<T>::getDist() const {
	return this->dist;
}

template <class T>
Vertex<T> *Vertex<T>::getPath() const {
	return this->path;
}

/********************** Edge  ****************************/

template <class T>
class Edge {
	Vertex<T> * dest;      // destination vertex
	double weight;         // edge weight
	bool displayGV;     //needed because we add the same edge twice and should only display 1
public:
	Edge(Vertex<T> *d, double w);
	Edge(Vertex<T> *d, double w, bool disp);
	Vertex<T>* getDest() {return dest;}
	bool displayEdge(){return displayGV;}
	friend class Graph<T>;
	friend class Vertex<T>;
};

template <class T>
Edge<T>::Edge(Vertex<T> *d, double w): dest(d), weight(w) {}
template<class T>
Edge<T>::Edge(Vertex<T> *d, double w, bool disp): dest(d), weight(w), displayGV(disp) {}


/*************************** Graph  **************************/

template <class T>
class Graph {
	vector<Vertex<T> *> vertexSet;    // vertex set
    double ** W = nullptr; // distance
    int **P = nullptr; // path

public:
	Vertex<T> *findVertex(const T &in) const;
	bool addVertex(const T &in);
	bool addEdge(const T &sourc, const T &dest, double w);
	bool addEdge(const T &sourc, const T &dest, double w, bool disp);
	int getNumVertex() const;
	vector<Vertex<T> *> getVertexSet() const;

	// Fp05 - single source
	void unweightedShortestPath(const T &orig);
	void dijkstraShortestPath(const T &orig);
	void bellmanFordShortestPath(const T &orig);
	vector<T> getPathTo(const T &dest) const;

	// Fp05 - all pairs
	void floydWarshallShortestPath();
	vector<T> getfloydWarshallPath(const T &origin, const T &dest) const;

};

template <class T>
int Graph<T>::getNumVertex() const {
	return vertexSet.size();
}

template <class T>
vector<Vertex<T> *> Graph<T>::getVertexSet() const {
	return vertexSet;
}

/*
 * Auxiliary function to find a vertex with a given content.
 */
template <class T>
Vertex<T> * Graph<T>::findVertex(const T &in) const {
	for (auto v : vertexSet)
		if (v->info == in)
			return v;
	return NULL;
}

/*
 *  Adds a vertex with a given content or info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */
template <class T>
bool Graph<T>::addVertex(const T &in) {
	if ( findVertex(in) != NULL)
		return false;
	vertexSet.push_back(new Vertex<T>(in));
	return true;
}

/*
 * Adds an edge to a graph (this), given the contents of the source and
 * destination vertices and the edge weight (w).
 * Returns true if successful, and false if the source or destination vertex does not exist.
 */
template <class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest, double w) {
	auto v1 = findVertex(sourc);
	auto v2 = findVertex(dest);
	if (v1 == NULL || v2 == NULL)
		return false;
	v1->addEdge(v2,w);
	return true;
}

template <class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest, double w, bool shouldDisplay) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == NULL || v2 == NULL)
        return false;
    v1->addEdge(v2,w, shouldDisplay);
    return true;
}


/**************** Single Source Shortest Path algorithms ************/

template<class T>
void Graph<T>::unweightedShortestPath(const T &orig) {
    queue<Vertex<T> *> q;
    for (auto v : vertexSet) {
        v->dist = INF;
        v->path = nullptr;
    }
    auto s = findVertex(orig);
    s->dist = 0;

    q.push(s);
    while (!q.empty()) {
        auto v = q.front();
        q.pop();
        for (auto e : v->adj)
            if (v->dist + 1 < e.dest->dist) {
                e.dest->dist = v->dist + 1;
                e.dest->path = v;
                q.push(e.dest);
            }
    }
}


template<class T>
void Graph<T>::dijkstraShortestPath(const T &origin) {
    MutablePriorityQueue<Vertex<T> > q;
    for (auto v : vertexSet) {
        v->dist = INF;
        v->path = nullptr;
    }
    auto s = findVertex(origin);
    s->dist = 0;

    q.insert(s);
    while(!q.empty()){
        auto v = q.extractMin();
        for(auto e : v->adj){
            auto oldDist = e.dest->dist;
            if (v->dist + e.weight < e.dest->dist) {
                e.dest->dist = v->dist + e.weight;
                e.dest->path = v;
                if(oldDist == INF) q.insert(e.dest);
            }
        }
    }
}


template<class T>
void Graph<T>::bellmanFordShortestPath(const T &orig) {
    for (auto v : vertexSet) {
        v->dist = INF;
        v->path = nullptr;
    }
    auto s = findVertex(orig);
    s->dist = 0;

    for(unsigned i = 1; i < vertexSet.size(); i++)
        for(auto v : vertexSet)
            for(auto e : v->adj)
                if (v->dist + e.weight < e.dest->dist) {
                    e.dest->dist = v->dist + e.weight;
                    e.dest->path = v;
                }
            for(auto v : vertexSet)
                for(auto e : v->adj)
                    if (v->dist + e.weight < e.dest->dist) {
                        e.dest->dist = v->dist + e.weight;
                        e.dest->path = v;

                    }
}


template<class T>
vector<T> Graph<T>::getPathTo(const T &dest) const{
    vector<T> res;
    auto v = findVertex(dest);
    if(v == nullptr || v->dist == INF)
        return res;
    for( ; v != nullptr; v = v->path)
        res.push_back(v->info);
    reverse(res.begin(), res.end());
    return res;
}



/**************** All Pairs Shortest Path  ***************/


template<class T>
void Graph<T>::floydWarshallShortestPath() {
    unsigned n = vertexSet.size();
    if (W != nullptr) {
        for (int i = 0; i < n; i++)
            if (W[i] != nullptr)
                delete [] W[i];
        delete [] W;
    }
    if (P != nullptr) {
        for (int i = 0; i < n; i++)
            if (P[i] != nullptr)
                delete [] P[i];
        delete [] P;
    }
    W = new double *[n];
    P = new int *[n];
    for (unsigned i = 0; i < n; i++) {
        W[i] = new double[n];
        P[i] = new int[n];
        for (unsigned j = 0; j < n; j++) {
            W[i][j] = i == j? 0 : INF;
            P[i][j] = -1;
        }
        for (auto e : vertexSet[i]->adj) {
            for (unsigned k = 0; k < vertexSet.size(); k++)
                if (vertexSet[k]->info == e.dest->info){
                    unsigned int l = k;
                    W[i][l] = e.weight;
                    P[i][l] = i;
                }
        }
        for(unsigned k = 0; k < n ; k++)
            for(unsigned i = 0; i < n; i++)
                for(unsigned j = 0; j < n; j++){
                    if(W[i][k] == INF || W[k][j] == INF)
                        continue;
                    int val = W[i][k] + W[k][j];
                    if(val < W[i][j]){
                        W[i][j] = val;
                        P[i][j] = P[k][j];

                    }
                }
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < n; j++) {
                std::cout << P[i][j] << " ";
                if(j == n - 1)
                    std::cout << endl;
            }
        }
    }

}

template<class T>
vector<T> Graph<T>::getfloydWarshallPath(const T &orig, const T &dest) const{
	vector<T> res;
	unsigned i = 0;
	unsigned j = 0;
    for (unsigned k = 0; k < vertexSet.size(); k++)
        if (vertexSet[k]->info == orig)
            i = k;
    for (unsigned l = 0; l < vertexSet.size(); l++)
        if (vertexSet[l]->info == dest)
            j = l;
    if (i == -1 || j == -1 || W[i][j] == INF) return res;
    for ( ; j != -1; j = P[i][j])
        res.push_back(vertexSet[j]->info);
    reverse(res.begin(), res.end());
    return res;
}


#endif /* GRAPH_H_ */
