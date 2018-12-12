#include "opero.hpp"

#include <iostream>

int main(int argc, char* argv[]) {
  opero::function<opero::real, opero::real> func(argv[1]);
  std::cout << func(3.1415) << "<<\n";
  return 0;
}
