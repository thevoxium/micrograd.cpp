#include "micrograd.h"

vector<Value*> UpdateParameters(vector<Value*>& params, double learningRate){
    for (Value* param : params){
        param->data -= learningRate * param->grad;
        param->grad = 0;
    }
    return params;
}

void train(int epochs, double learningRate, vector<Value*>& p, vector<Value*>& x, vector<Value*>& y) {
    Value w(p[0]->data);
    for (int epoch = 0; epoch < epochs; epoch++) {
        Value xi(x[0]->data);
        Value yi(y[0]->data);
        std::cout<<w.data<<endl;
        Value pred = w * xi; 
        Value error = pred - yi;
        Value squared_error = error*error;

        Value loss = squared_error * 0.2;

        //cout << "Epoch: " << epoch + 1 << " - Loss: " << loss.data << endl;
        
        loss.backward(); 
        vector<Value*> pp = {&w};
        pp = UpdateParameters(pp, learningRate); 
        w.data = pp[0]->data;
    }
    std::cout<<w.data;
}