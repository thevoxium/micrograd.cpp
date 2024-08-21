#include <iostream>                                                                                                                                                                                                                              
#include <vector>                                                                                                                                                                                                                                
#include <set>                                                                                                                                                                                                                                   
#include <functional>                                                                                                                                                                                                                            
#include <string>                                                                                                                                                                                                                                
#include <sstream>                                                                                                                                                                                                                               
#include <cmath>     

using namespace std;

class Value{
    public:
    double data;
    mutable double grad;
    function<void()> _backward;
    set<const Value*> _prev;
    string _op;

    void build_topo(const Value* v, vector<const Value*> &topo, set<const Value*> &visited) const {
        if (v == nullptr) {
            cerr << "Warning: Null pointer passed to build_topo. Skipping this node." << endl;
            return; 
        }

        if (visited.find(v) != visited.end()) {
            cerr << "Error: Cycle detected in the computational graph." << endl;
            return; // We still need to stop for cycles to avoid infinite loops
        }

        visited.insert(v);

        for (auto x : v->_prev) {
            if (x != nullptr) { 
                build_topo(x, topo, visited);
            } else {
                cerr << "Warning: Null pointer encountered in _prev set. Skipping this connection." << endl;
                // Continue building the topological order even with null pointers
            }
        }

        topo.push_back(v);
    }

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

    Value operator-(const Value &other) const{
        Value out(this->data -other.data);
        out._prev = {this, &other};
        out._op = "-";

        out._backward = [this, &other, &out](){
            const_cast<Value*>(this)->grad += out.grad;
            const_cast<Value&>(other).grad -= out.grad;
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

    Value sigmoid() const{
        double sig = 1.0/(1+std::exp(-1*this->data));
        double der = sig * (1-sig);
        Value out(sig);
        out._prev = {this};
        out._op = "sig";
        
        out._backward = [this, &out, der](){
            const_cast<Value*>(this)->grad += der*out.grad;
        };
        return out;
    }


    Value tanh() const{
        double t = (std::exp(this->data)-std::exp(-1.0*this->data)) / (std::exp(this->data)+std::exp(-1.0*this->data));
        Value out(t);
        out._prev = {this};
        out._op = "tanh";

        out._backward = [this, t, &out](){
            const_cast<Value*>(this)->grad += (1-t*t)*out.grad;
        };
        return out;
    };


    Value log() const {                                                                                                                                                                                                                          
        Value out(std::log(this->data));                                                                                                                                                                                                              
        out._prev = {this};                                                                                                                                                                                                                      
        out._op = "log";                                                                                                                                                                                                                         
                                                                                                                                                                                                                                                
        out._backward = [this, &out]() {                                                                                                                                                                                                         
            const_cast<Value*>(this)->grad += (1.0 / this->data) * out.grad;                                                                                                                                                                     
        };                                                                                                                                                                                                                                       
        return out;                                                                                                                                                                                                                              
    }

    Value exp() const {                                                                                                                                                                                                                          
        double exp_val = std::exp(this->data);                                                                                                                                                                                                        
        Value out(exp_val);                                                                                                                                                                                                                      
        out._prev = {this};                                                                                                                                                                                                                      
        out._op = "exp";                                                                                                                                                                                                                         
                                                                                                                                                                                                                                                
        out._backward = [this, &out, exp_val]() {                                                                                                                                                                                                
            const_cast<Value*>(this)->grad += exp_val * out.grad;                                                                                                                                                                                
        };                                                                                                                                                                                                                                       
        return out;                                                                                                                                                                                                                              
    }

    Value sqrt() const {                                                                                                                                                                                                                         
        double sqrt_val = std::sqrt(this->data);                                                                                                                                                                                                      
        Value out(sqrt_val);                                                                                                                                                                                                                     
        out._prev = {this};                                                                                                                                                                                                                      
        out._op = "sqrt";                                                                                                                                                                                                                        
                                                                                                                                                                                                                                                
        out._backward = [this, &out, sqrt_val]() {                                                                                                                                                                                               
            const_cast<Value*>(this)->grad += (0.5 / sqrt_val) * out.grad;                                                                                                                                                                       
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

    bool operator!=(std::nullptr_t) const {
        return this != nullptr; // Compare the 'this' pointer to nullptr
    }


                                                                                                                                                                                                                                                
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
                                                                                                                                                                                                                                                
    Value operator/(const Value& other) const {                                                                                                                                                                                                  
        return *this * other.power(-1);                                                                                                                                                                                                            
    }   
};