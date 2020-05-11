//
// Created by olive on 11/05/2020.
//

#ifndef CAL_PROJ_NODE_H
#define CAL_PROJ_NODE_H

#endif //CAL_PROJ_NODE_H

enum Type{
    PARAGEM,
    PRECOLHA,
    INTERSECAO,
    POINT,
    NONE
};

class Node{
private:
    int id;
    double xCoord;
    double yCoord;
    Type type;
    int graphViewerX;
    int graphViewerY;

public:
    Node(int id, double xCoord, double yCoord, Type type, int graphViewerX, int graphViewerY) : id(id), xCoord(xCoord),
                                                                                                yCoord(yCoord),
                                                                                                type(type),
                                                                                                graphViewerX(
                                                                                                        graphViewerX),
                                                                                                graphViewerY(
                                                                                                        graphViewerY) {}

    Node(int id, double xCoord, double yCoord, Type type) : id(id), xCoord(xCoord), yCoord(yCoord), type(type) {}


    int getId() const {
        return id;
    }

    void setId(int id) {
        Node::id = id;
    }

    double getXCoord() const {
        return xCoord;
    }

    void setXCoord(double xCoord) {
        Node::xCoord = xCoord;
    }

    double getYCoord() const {
        return yCoord;
    }

    void setYCoord(double yCoord) {
        Node::yCoord = yCoord;
    }

    Type getType() const {
        return type;
    }

    void setType(Type type) {
        Node::type = type;
    }

    int getGraphViewerX() const {
        return graphViewerX;
    }

    void setGraphViewerX(int graphViewerX) {
        Node::graphViewerX = graphViewerX;
    }

    int getGraphViewerY() const {
        return graphViewerY;
    }

    void setGraphViewerY(int graphViewerY) {
        Node::graphViewerY = graphViewerY;
    }

};