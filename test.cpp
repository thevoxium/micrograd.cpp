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
    Value t(91.789);
    y.push_back(&t);
  }
  for (int i=0; i<1; i++){
    Value t(0);
    params.push_back(&t);
  }


  double learningRate = 0.00001;
  int epochs = 100;

  train(epochs, learningRate, params, x, y, 0.0);
  return 0;
}
