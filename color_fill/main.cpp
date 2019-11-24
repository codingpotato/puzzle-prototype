#include <iostream>

#include "algorithm.hpp"

int main() {
  board b{{{1, 1, 1, 1, 1, 2, 2},
           {3, 1, 1, 4, 4, 2, 2},
           {3, 4, 4, 4, 4, 2, 2},
           {3, 3, 3, 3, 3, 2, 5},
           {6, 6, 7, 7, 7, 5, 5},
           {6, 6, 7, 8, 8, 5, 5},
           {6, 6, 8, 8, 8, 5, 5}}};
  std::cout << b;
  std::cout << generate_enclosure_board(b);
}