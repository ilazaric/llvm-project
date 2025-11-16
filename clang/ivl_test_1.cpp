
struct S {
  // void fn(int);
};

// [[ivl::ufcs]] void fn(long);

// void fn(char);

// [[ivl::ufcs]] void fn2(const S&, long);

// void fn2(char);

[[ivl::ufcs]] void fn3(const S&, auto);

int main(){
  S s;
  // s.fn(1);
  // s.fn(2l);
  // s.fn('a');
  // s.fn2(3l);
  // s.fn3(4); // broken
  // fn3(s, 4);
  s.fn3(4);
}

#if 1
#include <iostream>

// void S::fn(int) {
//   std::cout << "member function\n";
// }

// [[ivl::ufcs]] void fn(long) {
//   std::cout << "[[ivl::ufcs]] free function\n";
// }

// void fn(char) {
//   std::cout << "not-a-[[ivl::ufcs]] free function\n";
// }

// [[ivl::ufcs]] void fn2(const S&, long) {
//   std::cout << "[[ivl::ufcs]] free function\n";
// }

// void fn2(char) {
//   std::cout << "not-a-[[ivl::ufcs]] free function\n";
// }

[[ivl::ufcs]] void fn3(const S&, auto) {
  std::cout << "[[ivl::ufcs]] free function template\n";
}  
#endif
