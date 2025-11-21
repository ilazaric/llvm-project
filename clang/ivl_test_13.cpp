

namespace A {
template <typename> struct X {
  template <typename> void fn2();
};
} // namespace A

template <typename T> struct Y {
  A::X<T> x;

  // TODO: fix this
  template <typename> void fn1() { return x.template fn2<T>(); }
};
