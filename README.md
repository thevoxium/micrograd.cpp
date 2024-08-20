# micrograd.cpp

It is Micrograd but written in C++

## Sample Usage


``` bash
Value a(3.0);
Value b(4.0);
Value c = a + b;
std::cout << "Addition: " << c.str() << std::endl;

Value x(2.0), y(3.0);
Value z = x * y;
z.backward();
std::cout << "Backward (x): " << x.str() << std::endl;
std::cout << "Backward (y): " << y.str() << std::endl;
```

Or compile the test file with:

```
g++ -std=c++11 test.cpp -o test;
./test
```

## Current Abilities

* Basic operations like add, mul, div, sub
* Power
* Relu
