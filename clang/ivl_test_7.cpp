#include <string>
#include <iostream>

int main(){
  auto l = '[';
  auto r = ']';
  [[ivl::ufcs]] auto lambda1 = [&](auto&& x){ return l + x + r; };
  [[ivl::ufcs]] auto lambda2 = [&](auto&& x){ return x.lambda1().lambda1(); };
  [[ivl::ufcs]] auto lambda3 = [&](auto&& x){ return x.lambda2().lambda2(); };
  std::cout << std::string("message").lambda3() << std::endl;
}
