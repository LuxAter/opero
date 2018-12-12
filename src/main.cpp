#include "opero.hpp"

#include <iostream>

int main(int argc, char* argv[]) {
  // opero::matrix<double, 3, 3> a({1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0});
  opero::matrix<double, 3, 3> b({{1.0, 3.0, 5.0}, {2.0, 4.0, 7.0}, {1.0, 1.0, 0.0}});
  std::cout << b << '\n';
  std::cout << trace(b) << '\n';
  std::cout << det(b) << '\n';
  return 0;
}
