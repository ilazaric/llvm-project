
// struct S {};

// template<typename T>
// [[ivl::ufcs]] T tfn(const S&);

// void use(){
//   S s;
//   tfn<int>(s);
//   s.tfn<int>();
// }



struct S {
  // template<typename> void tfn();
};

template<typename>
[[ivl::ufcs]] int tfn(const S&) { return 12; }

int main() {
  S s;
  return s.template tfn<int>();
  // template tfn<int>(s);
}
