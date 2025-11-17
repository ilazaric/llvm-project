struct S {};

namespace NS {
  [[ivl::ufcs]]
  int mem(const S&) { return 12; }
}

int main(){
  // [[ivl::ufcs]] auto mem = [](const S&) { return 12; };
  // return mem(S{});
  return S{}.NS::mem();
}
