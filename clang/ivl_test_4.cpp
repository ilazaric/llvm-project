
struct S {};

[[ivl::ufcs]]
int fn1(const S& s) { return 12; }

[[ivl::ufcs]]
auto fn2(auto&& s) { return s.fn1(); }

int main(){
  return S{}.fn2();
}
