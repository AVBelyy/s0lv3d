#include "Expression.h"
#include <iostream>

const double phi = (1 + sqrt(5.0)) / 2;
const double resphi = 2 - phi;

const double eps = 10e-32;
const double tau = sqrt(eps);

typedef map<Node, Node> NodeDict;
typedef map<Node, FunctionCaller> CRulesDict;

Var _A_ = Var("_A_"), _X_ = Var("_X_");

// Derivate rules
NodeDict dRules = createMap<Node, Node>
    (Sin(_X_),  Cos(_X_))
    (Cos(_X_), -Sin(_X_))
    (Log(_X_),  1/_X_)
    (_X_ ^ _A_, _A_ * _X_ ^ _A_-1)
    (_A_ ^ _X_, _A_ ^ _X_ * Log(_A_));

// Calculate rules
CRulesDict cRules = createMap<Node, FunctionCaller>
    (Operator("+"),   Arg_2(Functions::add))
    (Operator("-"),   Arg_2(Functions::sub))
    (Operator("*"),   Arg_2(Functions::mul))
    (Operator("/"),   Arg_2(Functions::div))
    (Operator("^"),   Arg_2(Functions::pow))
    (Operator("sin"), Arg_1(Functions::sin))
    (Operator("cos"), Arg_1(Functions::cos))
    (Operator("log"), Arg_1(Functions::log));

class Expression
{
    Node &eq;

    public:
        Expression(Node &eq) : eq(eq) {}

        const string debug() { return eq.debug(); }

        Expression& derivate(Var var = Var("x"))
        {
            Node *d = new Node();
            // TODO: check whether *var* is present in the equation
            derivateCheckRecur(eq, var);
            derivateCalcRecur(*d, eq, var);
            return *(new Expression(*d));
        }

        Expression& substitute(NodeDict dict)
        {
            Node *c = new Node();
            substituteRecur(*c, eq, dict);
            return *(new Expression(*c));
        }

        Expression& calculate()
        {
            return *(new Expression(calculateRecur(eq)));
        }

    private:
        static Node &arithmOp(string op, Node &lvalue, Node &rvalue)
        {
            Node *res = new Node(OPERATOR, op);
            *res << lvalue << rvalue;
            return *res;
        }

        Node& calculateRecur(Node &cur)
        {
            CRulesDict::iterator calc = cRules.find(cur);
            if(calc != cRules.end())
            {
                FunctionCaller caller = calc->second;
                vector<Node*> calculated;
                for(list<Node>::iterator it = cur.children.begin(); it != cur.children.end(); it++)
                {
                    calculated.push_back(&calculateRecur(*it));
                }
                if(caller.argsCount == 1)
                {
                    return caller.args.arg1(*calculated[0]);
                } else if(caller.argsCount == 2) {
                    return caller.args.arg2(*calculated[0], *calculated[1]);
                }
            } else {
                return cur;
            }
        }

        void substituteRecur(Node &curS, Node &curEq, NodeDict dict)
        {
            NodeDict::iterator subst = dict.find(curEq);
            if(subst != dict.end())
            {
                curS = subst->second;
            } else {
                curS = curEq;
            }
            for(list<Node>::iterator it = curEq.children.begin(); it != curEq.children.end(); it++)
            {
                Node *node = new Node();
                substituteRecur(*node, *it, dict);
                curS << *node;
            }
        }
 
        void derivateCheckRecur(Node &cur, Var var)
        {
        }

        void derivateCalcRecur(Node &curD, Node &curEq, Var var)
        {
            switch(curEq.type)
            {
                case NUMBER:
                    curD.setType(NUMBER);
                    curD.setValue(0);
                    break;
                case VAR:
                    curD.setType(NUMBER);
                    curD.setValue(curEq == var ? 1 : 0);
                    break;
                case OPERATOR:
                    Node *lvalue = new Node(), *rvalue = new Node();
                    list<Node>::iterator it = curEq.children.begin();
                    curD.setType(OPERATOR);
                    if(curEq.name == "+" || curEq.name == "-")
                    {
                        curD.setName(curEq.name);
                        derivateCalcRecur(*lvalue,   *it, var);
                        derivateCalcRecur(*rvalue, *++it, var);
                        curD << *lvalue << *rvalue;
                    }
                    else if(curEq.name == "*")
                    {
                        // u'v + uv'
                        curD.setName("+");
                        derivateCalcRecur(*lvalue,   *it, var);
                        derivateCalcRecur(*rvalue, *++it, var);
                        curD << Expression::arithmOp("*", *lvalue, *it);
                        curD << Expression::arithmOp("*", *--it, *rvalue);
                    }
                    else if(curEq.name == "/")
                    {
                        Node *d1 = new Node(OPERATOR, "-"), *d2 = new Node(OPERATOR, "*");
                        // u'v - uv'    (1)
                        derivateCalcRecur(*lvalue,   *it, var);
                        derivateCalcRecur(*rvalue, *++it, var);
                        *d1 << Expression::arithmOp("*", *lvalue, *it);
                        *d1 << Expression::arithmOp("*", *--it, *rvalue);
                        // v**2         (2)
                        it++;
                        *d2 << *it << *it;
                        // (1)/(2)
                        curD.setName("/");
                        curD << *d1 << *d2;
                    } else {
                        // Check whether it's a function
                        NodeDict::iterator dIt;
                        dRules[Sin(_X_)] << _A_;
                        for(dIt = dRules.begin(); dIt != dRules.end(); dIt++)
                        {
                            Node key = dIt->first;
                            Node val = dIt->second;
                            if(key == curEq)
                            {
                                // TODO: implement
                            }
                        }
                    }
                    break;
            }
        }

        static double goldenSectionSearch(double a, double b, double c, double (*f)(double))
        {
            double x;
            if(c - b > b - a)
                x = b + resphi*(c - b);
            else
                x = b - resphi*(b - a);
            if(fabs(c - a) < tau*(fabs(b) + fabs(x))) 
                return (c + a) / 2; 
            if(f(x) < f(b))
            {
                if(c - b > b - a) return goldenSectionSearch(b, x, c, f);
                else return goldenSectionSearch(a, x, b, f);
            } else {
                if(c - b > b - a) return goldenSectionSearch(a, b, x, f);
                else return goldenSectionSearch(x, b, c, f);
            }
        }
};

int main()
{
    Var x = Var("x");
    Expression eq( 1/x );
    Expression eq2( Sin((1+2)*3)/4 );

    cout << "(1/x)' = " << eq.derivate().debug() << endl;
    cout << "Sin((1+2)*3)/4 = " << eq2.calculate().debug() << endl;
    return 0;
}
