#include <iostream>

#include "micrograd.h"

int main() {
  Value a(3.0);

  Value b(4.0);
  Value c = a + b;
  std::cout << "Addition: " << c.str() << std::endl;

  Value d = a * b;
  std::cout << "Multiplication: " << d.str() << std::endl;

  Value e = a.power(2);
  std::cout << "Power: " << e.str() << std::endl;

  Value f = Value(-2.0).relu();
  std::cout << "ReLU: " << f.str() << std::endl;

  Value g = -a;
  std::cout << "Negation: " << g.str() << std::endl;

  Value h = a - b;
  std::cout << "Subtraction: " << h.str() << std::endl;

  Value i = a / b;
  std::cout << "Division: " << i.str() << std::endl;

  Value s = a.tanh();
  std::cout << "tanh: " << s.str() << std::endl;

  Value x(2.0), y(3.0);
  Value z = x * y;
  Value t = z.tanh();
  t.backward();
  std::cout << "Backward (x): " << x.str() << std::endl;
  std::cout << "Backward (y): " << y.str() << std::endl;

  return 0;
}