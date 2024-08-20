#include <iostream>
#include <vector>
#include <cmath>
#include <functional>
#include <memory>
#include <algorithm>

using namespace std;

class Value{
private:
    double data;
    double grad;
    function<void()> _backward;
    vector<shared_ptr<Value>> _prev;
    string _op;

public:
    Value(double data, const vector<shared_ptr<Value>> &children = {}, const string &op = "")
    : data(data), grad(0), _backward([](){}), _prev(children), _op(op) {}
};