#include <string_view>
#include <iostream>

namespace ext {
  [[ivl::ufcs]]
  char get_or_null(std::string_view sv, auto idx){
    if (idx < 0 || idx > std::ssize(sv)) return '\0';
    return sv[idx];
  }
}

int main(){
  std::string_view sv = "hello";
  for (int i = -5; i < 10; ++i)
    std::cout << i << "\t-->\t" << (int)sv.ext::get_or_null(i) << std::endl;
}
