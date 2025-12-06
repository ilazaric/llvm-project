#include <cstdio>
#include <cassert>

struct S {
  char meaningless_data[64];
  S *constructor_addr;
  
  S() : constructor_addr(this) {
    printf(" S::S():\taddr=%p\n", this);
  }
  
  ~S() {
    printf("~S::S():\taddr=%p\n", this);
    assert(this == constructor_addr);
  }
  
  S(S &&) = delete;
};
