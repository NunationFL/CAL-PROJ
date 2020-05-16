//
// Created by olive on 12/05/2020.
//

#include <fstream>
#include <sstream>
#include <cmath>
#include <iostream>
#include <algorithm>
#include "GraphFuncs.h"


using namespace std;

Graph<Node> loadGraph( string city){
    Graph<Node> graph;
    ifstream coordFile, edgeFile;
    int aux;
    string auxString;

    //open files for reading
    auxString="../mapas/"+city+"/nodes_x_y_"+city+".txt";
    coordFile.open(auxString);

    if(!coordFile) {
        cout << "Couldn't open coordinates file!" << endl;
        return graph;
    }

    auxString="../mapas/"+city+"/edges_"+city+".txt";
    edgeFile.open(auxString);

    if(!edgeFile) {
        cout << "Couldn't open edge file!" << endl;
        return graph;
    }

    //-------------------------------READ VERTEX----------------------


    coordFile >> aux;

    string line;

    getline(coordFile, line);   //clear /n


    while(getline(coordFile, line)) {

        // removes parentheses
        size_t pos = line.find(')');
        if (pos != string::npos)
            line = line.substr(1, pos);


        line.erase(remove(line.begin(), line.end(), ','), line.end());  //removes ','


        int id;
        double x, y;
        stringstream linestream(line);
        linestream >> id >> x >> y;

        graph.addVertex(Node(id, x, y));

    }


    if(graph.getVertexSet().size() != aux) {    //vertex num check
        cout << "Read wrong number of vertex! ";
        return graph;
    }



    //------------------------READ EDGES-----------------------------
    int total=0;
    double distance;

    edgeFile >> aux;

    getline(edgeFile, line);    //clear /n


    while(getline(edgeFile, line)) {

        // removes parentheses
        size_t pos = line.find(')');
        if (pos != string::npos)
            line = line.substr(1, pos);


        line.erase(remove(line.begin(), line.end(), ','), line.end());  //removes ','

        int id1, id2;
        stringstream lineS(line);
        lineS >> id1 >> id2;

        Vertex<Node>* v1 = graph.findVertex(Node(id1)); //search source vertex
        Vertex<Node>* v2 = graph.findVertex(Node(id2)); //search dest vertex

        distance = getEdgeWeight(v1->getInfo().getXCoord(), v1->getInfo().getYCoord(), v2->getInfo().getXCoord(), v2->getInfo().getYCoord());



        if(!graph.addEdge(Node(id1), Node(id2), distance, true)){
            cout<<"Failed to add an edge from node "<<id1<<" to "<<id2<<" !!!";
            return graph;
        }
        total++;
        if(!graph.addEdge(Node(id2), Node(id1), distance, false)){
            cout<<"Failed to add an edge from node "<<id2<<" to "<<id1<<" !!!";
            return graph;
        }
        total++;
    }

    if(total != (aux * 2)) {    //edge num check
        cout << "Read wrong number of edges! ";
        return graph;
    }

    return graph;
}

double getEdgeWeight(double x1, double y1, double x2, double y2) {

    return sqrt(pow(x2 - x1,2)  + pow(y2 - y1,2) );
}

vector<Vertex<Node>*> readFromCityFile(Graph<Node> &graph,string city){
    ifstream cityFile;
    string aux;
    vector<Vertex<Node>*> outIfFail;
    aux="../files/"+city+"/"+city+"_info.txt";
    cityFile.open(aux);
    if(!cityFile){
        cout<<"Couldn't open city file! ";
        return outIfFail;
    }
    getline(cityFile,aux);
    Vertex<Node>* garage=graph.findVertex(Node(stoi(aux)));
    garage->getInfo().setType(Type::GARAGEM);
    return cleanEdgesNVertex(graph,garage);
}

vector<Vertex<Node>*> cleanEdgesNVertex(Graph<Node> graph, Vertex<Node>* garage){
    vector<Vertex<Node>*> visitedVertex;
    //---------------------CLEAN USELESS EDGES---------------------
    for(auto v : graph.getVertexSet()) {
        for (int i = 0; i < v->getAdj().size(); i++) {
            if (v->getAdj().at(i).getWeight() <= 0) {
                v->removeEdge(i);
                i--;
            }
        }
    }
    //----------------------REMOVE VERTEX NOT ACCESSIBLE FROM GARAGE---------------
    for (auto v : graph.getVertexSet())
        v->setVisited(false);

    graph.DepthFirstSearch(garage, visitedVertex);
    return visitedVertex;
}


vector<Vertex<Node>*> readService(vector<Vertex<Node>*> graph, string city) {

    string aux;
    ifstream serviceFile;
    vector<int> notFound;
    int id, total = 0;
    bool found = false;

    do {
        cout << "Insert the target service file name (no need for the directory and sufix but MUST be .txt): " << endl;
        cin >> aux;
        aux = "../files/" + city + "/" + aux + ".txt";
        serviceFile.open(aux);
        if (!serviceFile)
            cout << "Couldn't open file! Please insert another one." << endl;

    } while (!serviceFile);

    int idFactory;
    serviceFile >> idFactory;
    getline(serviceFile, aux);

    int nrPR;
    serviceFile >> nrPR;
    getline(serviceFile, aux);


    while (getline(serviceFile, aux)) {

        id = stoi(aux);

        for (auto i: graph) {
            if (i->getInfo().getId() == id) {
                Node newInfo = i->getInfo();
                newInfo.setType(Type::PRECOLHA);
                i->setInfo(newInfo);
                found = true;
                break;
            }
        }

        if (!found) {
            notFound.push_back(id);
        } else {
            found = false;
        }
        total++;
    }

    for (auto i: graph) {
        if (i->getInfo().getId() == idFactory) {
            i->getInfo().setType(Type::FACTORY);
            break;
        }
    }

    if (total != nrPR)
        cout << "Not counting unaccessible nodes, it wasn't possible to read all nodes, please check file integrity!\n";

    if (!notFound.empty()) {
        cout << "There were " << notFound.size()
             << " id's not accessible from the garage or that simply don't exist in this map.\nThose were:\n";
        for (auto i : notFound) {
            cout << "(" << i << ");\t";
        }
    }
    return graph;
}

unordered_map<VertexPair, double> makeTable(vector<Vertex<Node> *> accessNodes, Graph<Node> graph){
    unordered_map<VertexPair, double> table;
    unsigned int i;
    do {

        cout << "What algorithm should be used?" << endl;
        cout << "0 -> Dijkstra" << endl;
        cout << "1 -> Floyd-Warshall" << endl;
        cout << "Option: ";
        cin >> i;


        if(i > 1)
            cout << endl << endl << "Invalid option! Try again." << endl << endl;

    } while(i > 1);

    for(auto v : accessNodes)
        if(i == 0) {
            graph.dijkstraTable(accessNodes,table, v->getInfo());
    }
    else {
        graph.floydWarshallTable(accessNodes,table);
    }

    return table;

}