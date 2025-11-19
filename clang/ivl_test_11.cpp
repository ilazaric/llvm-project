
// struct S {};

// template<typename T>
// [[ivl::ufcs]] T tfn(const S&);

// void use(){
//   S s;
//   tfn<int>(s);
//   s.tfn<int>();
// }



struct S {
  template<typename>
  void tfn();
};

void use(){
  S s;
  s.tfn<int>();
}
