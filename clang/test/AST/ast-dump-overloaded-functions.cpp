// Test without serialization:
// RUN: %clang_cc1 -triple x86_64-unknown-unknown -ast-dump %s \
// RUN: | FileCheck -strict-whitespace %s
//
// Test with serialization:
// RUN: %clang_cc1 -triple x86_64-unknown-unknown -emit-pch -o %t %s
// RUN: %clang_cc1 -x c++ -triple x86_64-unknown-unknown -include-pch %t -ast-dump-all /dev/null \
// RUN: | FileCheck -strict-whitespace %s

// void first();

//  [[ivl::ufcs]] void fn(int);
// void fn(char);
// void fn(long);

// void test_free() {
//   fn(123);
//   fn('a');
//   fn(321l);
//   // fn(67ll);
// }

// a non-static member function namespace-qualifiers::class-qualifiers::class::identifier ( args... )
// is findable as-if a static member function namespace-qualifiers::class-qualifiers::class::identifier ( qualified-class-type , args... )

// a free function namespace-qualifiers::identifier ( args... )
// is findable in all unqualified member function call overload resolution
// and all qualified member function call overload resolution if qualifiers::identifier sees it in context

// suppose we declare a free function:
// [[ivl::ufcs]] namespace-qualifiers::identifier ( args... )
// consider a unqualified member function call expression.indentifier ( args... )
// our free function participates in overload resolution if
// 1) it is visible in current context
// 2) ADL on identifier ( expresion , args... ) would find it
//
// consider a qualified member function call expression.qualifier::identifier ( args... )
// out free function participates in overload resolution if
// 1) qualifier::identifier sees it from current context
//
// suppose we declare a member function:
// [[ivl::ufcs]] namespace-qualifiers::class-qualifiers::identifier ( args... )

namespace NS {
  // struct S { void fn(); void fn(int); };
  struct S {};
  // [[ivl::ufcs]] void foobar();
  // void foobar(int);
  // [[ivl::ufcs]] void foobar(char);
  [[ivl::ufcs]] void foobar(const S&);
  // namespace NS2 { int foobar; }
  // using namespace NS2;
  void test() {
    // S s;
    // S{}.foobar();
    // auto foobar = []{};
    // foobar();
    // foobar();
    // foobar();
    S s;
    s.foobar();
  }
}

// namespace NS {
// struct S {
//   [[ivl::ufcs]] void fn(int) const;
//   [[ivl::ufcs]] void fn(char) const;
//   [[ivl::ufcs]] void fn(short) const;
// };

// [[ivl::ufcs]] void fn(const S&, int);
// [[ivl::ufcs]] void fn(const S&, char);
// [[ivl::ufcs]] void fn(const S&, short);

// void fn();
// void fn(int, int, int);

//   [[ivl::ufcs]] void foobar();

//   namespace NS2 {
//     struct T {};
//     void blatruc(T);
//   }

// void use(){
//   const S s;
//   // NS2::T t;
//   // s.::NS::S::fn(1);
//   s.foobar();
//   // foobar();
//   // blatruc(t);
//   // ::NS::fn(s, 2);
// }
// }

// struct S {
//   void fn(int);
//   void fn(char);
//   void fn(long);
// };

// void test_member() {
//   S s;
//   s.fn(123);
//   s.fn('a');
//   s.fn(321l);
//   // s.fn(67ll);
// }

// template<typename T>
// void test_template() {
//   T t;
//   t.fn(123);
//   t.fn('a');
//   t.fn(321l);
//   // t.fn(67ll);
// }

// void test_template_instantiate() {
//   test_template<S>();
// }
