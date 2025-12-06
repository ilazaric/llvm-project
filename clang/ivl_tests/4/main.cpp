#include "type.hpp"

S fn();

int main() {
  S s = fn();
  printf("manual:\t\taddr=%p\n", &s);
}
