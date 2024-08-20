#include <iostream>                                                                                                                                                                                                                              
#include <vector>                                                                                                                                                                                                                                
#include <set>                                                                                                                                                                                                                                   
#include <functional>                                                                                                                                                                                                                            
#include <string>                                                                                                                                                                                                                                
#include <sstream>                                                                                                                                                                                                                               
#include <cmath>     

using namespace std;

class Value{
    private:
    double data;
    mutable double grad;
    function<void()> _backward;
    set<const Value*> _prev;
    string _op;

    void build_topo(const Value* v, vector<const Value*> &topo, set<const Value*> &visited) const{
        if (visited.find(v) == visited.end()){
            visited.insert(v);
            
            for (auto x: v->_prev){
                build_topo(x, topo, visited);
            }
            topo.push_back(v);
        }
    }

    public:
    Value(double data, set<const Value*> children = {}, string op = ""):
        data(data), grad(0), _prev(children), _op(op) {
             _backward = [](){};
    }

    string str() const {
        stringstream ss;
        ss << "Value(data="<<data<<", grad=" <<grad<<")";
        return ss.str();
    }

    Value operator+(const Value &other) const{
        Value out(this->data +other.data);
        out._prev = {this, &other};
        out._op = "+";

        out._backward = [this, &other, &out](){
            const_cast<Value*>(this)->grad += out.grad;
            const_cast<Value&>(other).grad += out.grad;
        };

        return out;
    }
    

    Value relu() const{
        Value out(this->data > 0 ? 1:0);
        out._prev = {this};
        out._op = "Relu";
        
        out._backward = [this, &out](){
            const_cast<Value*>(this)->grad += (out.data > 0) * out.grad;
        };

        return out;
    }

    Value operator*(const Value &other) const {
        Value out(this->data*other.data);
        out._prev = {this, &other};
        out._op = "*";

        out._backward = [this, &other, &out](){
            const_cast<Value*>(this)->grad += other.data * out.grad;
            const_cast<Value&>(other).grad += this->data * out.grad;
        };
        return out;
    }

    Value power(int other) const{
        Value out(pow(this->data, other));
        out._prev = {this};
        out._op = "**" + to_string(other);

        out._backward = [this, other, &out]() {                                                                                                                                                                                                  
            const_cast<Value*>(this)->grad += other * pow(this->data, other - 1) * out.grad;                                                                                                                                                
        };
        return out;
    }

    void backward() const {                                                                                                                                                                                                                      
        std::vector<const Value*> topo;                                                                                                                                                                                                          
        std::set<const Value*> visited;                                                                                                                                                                                                          
        build_topo(this, topo, visited);                                                                                                                                                                                                         
                                                                                                                                                                                                                                                
        const_cast<Value*>(this)->grad = 1;                                                                                                                                                                                                      
        for (auto it = topo.rbegin(); it != topo.rend(); ++it) {                                                                                                                                                                                 
            (*it)->_backward();                                                                                                                                                                                                                  
        }                                                                                                                                                                                                                                        
    }  


    Value operator-() const {                                                                                                                                                                                                                    
        return *this * Value(-1);                                                                                                                                                                                                                
    }                                                                                                                                                                                                                                            
                                                                                                                                                                                                                                                
    Value operator-(const Value& other) const {                                                                                                                                                                                                  
        return *this + (-other);                                                                                                                                                                                                                 
    }                                                                                                                                                                                                                                            
                                                                                                                                                                                                                                                
    Value operator/(const Value& other) const {                                                                                                                                                                                                  
        return *this * other.power(-1);                                                                                                                                                                                                            
    }   
};