#include "Parser.h"

Node::Node(NodeType type, string name, double value) : type(type), name(name), value(value) {}

void Node::setType(NodeType type) { this->type = type; }

void Node::setName(string name) { this->name = name; }

void Node::setValue(double value) { this->value = value; }

const string Node::debug(Node* top)
{
    ostringstream s;
    list<Node>::iterator it;
    if(!top) top = this;
    switch(top->type) {
        case OPERATOR:
            s << "(" << top->name;
            for(it = top->children.begin(); it != top->children.end(); it++)
            {
                s << " " << debug(&*it);
            }
            s << ")";
            break;
        case VAR:
            s << top->name;
            break;
        case NUMBER:
            s << top->value;
            break;
    }
    return s.str();
}

bool operator<(const Node& lvalue, const Node& rvalue)
{
    if(lvalue.type  != rvalue.type)  return lvalue.type < rvalue.type;
    if(lvalue.name  != rvalue.name)  return lvalue.name < rvalue.name;
    if(lvalue.value != rvalue.value) return lvalue.value < rvalue.value;
    return false;
}

bool operator==(Node& lvalue, Node& rvalue)
{
    if(lvalue.type == rvalue.type &&
       lvalue.name == rvalue.name &&
       lvalue.value == rvalue.value)
        return true;
    else
        return false;
}

Node& operator<<(Node& lvalue, Node& rvalue)
{
    lvalue.children.push_back(rvalue);
    return lvalue;
}

inline Node& nn_operator(string op, Node& lvalue, Node& rvalue)
{
    Operator *root = new Operator(op);
    return *root << lvalue << rvalue;
}

inline Node& ndr_operator(string op, Node& lvalue, double number)
{
    Number *numNode = new Number(number);
    return nn_operator(op, lvalue, *numNode);
}

inline Node& ndl_operator(string op, double number, Node& rvalue)
{
    Number *numNode = new Number(number);
    return nn_operator(op, *numNode, rvalue);
}

Node& operator+(Node& lvalue, Node& rvalue)
{
    return nn_operator("+", lvalue, rvalue);
}

Node& operator+(Node& lvalue, double rvalue)
{
    return ndr_operator("+", lvalue, rvalue);
}

Node& operator+(double lvalue, Node& rvalue)
{
    return rvalue + lvalue;
}

Node& operator-(Node& lvalue, Node& rvalue)
{
    return nn_operator("-", lvalue, rvalue);
}

Node& operator-(Node& lvalue, double rvalue)
{
    return ndr_operator("-", lvalue, rvalue);
}

Node& operator-(double lvalue, Node& rvalue)
{
    return ndl_operator("-", lvalue, rvalue);
}

Node& operator*(Node& lvalue, Node& rvalue)
{
    return nn_operator("*", lvalue, rvalue);
}

Node& operator*(Node& lvalue, double rvalue)
{
    return ndr_operator("*", lvalue, rvalue);
}

Node& operator*(double lvalue, Node& rvalue)
{
    return rvalue*lvalue;
}

Node& operator/(Node& lvalue, Node& rvalue)
{
    return nn_operator("/", lvalue, rvalue);
}

Node& operator/(Node& lvalue, double rvalue)
{
    return ndr_operator("/", lvalue, rvalue);
}

Node& operator/(double lvalue, Node& rvalue)
{
    return ndl_operator("/", lvalue, rvalue);
}

Node& operator^(Node& lvalue, Node& rvalue)
{
    return nn_operator("^", lvalue, rvalue);
}

Node& operator^(Node& lvalue, double rvalue)
{
    return ndr_operator("^", lvalue, rvalue);
}

Node& operator^(double lvalue, Node& rvalue)
{
    return ndl_operator("^", lvalue, rvalue);
}

Node& operator-(Node& x)
{
    Operator *op = new Operator("-");
    *op << *(new Number(0)) << x;
    return *op;
}

Node& Sin(double x)
{
    return *(new Number(sin(x)));
}

Node& Sin(Node& x)
{
    Operator *op = new Operator("sin");
    return *op << x;
}

Node& Cos(double x)
{
    return *(new Number(cos(x)));
}

Node& Cos(Node& x)
{
    Operator *op = new Operator("cos");
    return *op << x;
}

Node& Log(double x)
{
    return *(new Number(log(x)));
}

Node& Log(Node& x)
{
    Operator *op = new Operator("log");
    return *op << x;
}


Node& Functions::add(Node& lvalue, Node& rvalue)
{
    if(lvalue.type == NUMBER && rvalue.type == NUMBER)
    {
        return *(new Number(lvalue.value + rvalue.value));
    } else {
        return *(new Operator("+")) << lvalue << rvalue;
    }
}

Node& Functions::sub(Node& lvalue, Node& rvalue)
{
    if(lvalue.type == NUMBER && rvalue.type == NUMBER)
    {
        return *(new Number(lvalue.value - rvalue.value));
    } else {
        return *(new Operator("-")) << lvalue << rvalue;
    }
}

Node& Functions::mul(Node& lvalue, Node& rvalue)
{
    if(lvalue.type == NUMBER && rvalue.type == NUMBER)
    {
        return *(new Number(lvalue.value * rvalue.value));
    } else {
        return *(new Operator("*")) << lvalue << rvalue;
    }
}

Node& Functions::div(Node& lvalue, Node& rvalue)
{
    if(lvalue.type == NUMBER && rvalue.type == NUMBER)
    {
        return *(new Number(lvalue.value / rvalue.value));
    } else {
        return *(new Operator("/")) << lvalue << rvalue;
    }
}

Node& Functions::pow(Node& lvalue, Node& rvalue)
{
    if(lvalue.type == NUMBER && rvalue.type == NUMBER)
    {
        return *(new Number(::pow(lvalue.value, rvalue.value)));
    } else {
        return *(new Operator("^")) << lvalue << rvalue;
    }
}

Node& Functions::sin(Node& value)
{
    if(value.type == NUMBER)
    {
        return Sin(value.value);
    } else {
        return *(new Operator("sin")) << value;
    }
}

Node& Functions::cos(Node& value)
{                                                                                                                         
    if(value.type == NUMBER)
    {
        return Cos(value.value);
    } else {
        return *(new Operator("cos")) << value;
    }
}

Node& Functions::log(Node& value)
{                                                                                                                         
    if(value.type == NUMBER)
    {
        return Log(value.value);
    } else {
        return *(new Operator("log")) << value;
    }
}


Operator::Operator(string name) : Node(OPERATOR, name) {}

Var::Var(string name) : Node(VAR, name) {}

Number::Number(double value) : Node(NUMBER, "", value) {}
