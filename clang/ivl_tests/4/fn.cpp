#include "type.hpp"

S generate();

S fn() {
  [[ivl::nrvo]] S s = generate();
  return s;
}
