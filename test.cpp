#include <iostream>
#include <cstdlib>
//#include "micrograd.h"
#include "grad_descent.h"

int main() {


  vector<Value*> x, y, params;
  for (int i=0; i<1; i++){
    Value t(45);
    x.push_back(&t);
  }
  for (int i=0; i<1; i++){
    Value t(90);
    y.push_back(&t);
  }
  for (int i=0; i<1; i++){
    Value t(0);
    params.push_back(&t);
  }


  double learningRate = 0.00001;
  int epochs = 1000;

  train(epochs, learningRate, params, x, y);
  return 0;
}
