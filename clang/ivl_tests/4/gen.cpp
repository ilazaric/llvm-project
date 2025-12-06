#include "type.hpp"

S generate() {
  [[ivl::nrvo]] S s;
  printf("manual:\t\taddr=%p\n", &s);
  return s;
}
