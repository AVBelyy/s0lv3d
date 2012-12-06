#pragma once

#include <string>
#include <list>
#include <map>
#include <sstream>
#include <cmath>

using namespace std;

enum NodeType
{
    NONE,
    OPERATOR,
    VAR,
    NUMBER
};

class Node
{
    public:
        NodeType type;
        list<Node> children;

        string name;
        double value;

        Node(NodeType = NONE, string = "", double = 0);
        void setType(NodeType);
        void setName(string);
        void setValue(double);
        const string debug(Node* = 0);
        
        Node& operator=(const Node& rvalue)
        {
            setType(rvalue.type);
            setName(rvalue.name);
            setValue(rvalue.value);
        }
};

class Operator : public Node
{
    public:
        Operator(string);
};

class Var : public Node
{
    public:
        Var(string = "");
};

class Number : public Node
{
    public:
        Number(double);
};


// Auxiliary operators
bool operator<(const Node&, const Node&);
bool operator==(Node&, Node&);

Node& operator<<(Node&, Node&);

// Operators
Node& operator-(Node&);

Node& operator+(Node&, Node&);
Node& operator+(Node&, double);
Node& operator+(double, Node&);

Node& operator-(Node&, Node&);
Node& operator-(Node&, double);
Node& operator-(double, Node&);

Node& operator*(Node&, Node&);
Node& operator*(Node&, double);
Node& operator*(double, Node&);

Node& operator/(Node&, Node&);
Node& operator/(Node&, double);
Node& operator/(double, Node&);

Node& operator^(Node&, Node&);
Node& operator^(Node&, double);
Node& operator^(double, Node&);

// functions
Node& Sin(double);
Node& Sin(Node&);

Node& Cos(double);
Node& Cos(Node&);

Node& Log(double);
Node& Log(Node&);


// Calculating functions
namespace Functions
{
    Node& add(Node&, Node&);
    Node& sub(Node&, Node&);
    Node& mul(Node&, Node&);
    Node& div(Node&, Node&);
    Node& pow(Node&, Node&);
    Node& sin(Node&);
    Node& cos(Node&);
    Node& log(Node&);
}
