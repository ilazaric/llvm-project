// Test without serialization:
// RUN: %clang_cc1 -triple x86_64-unknown-unknown -ast-dump %s \
// RUN: | FileCheck -strict-whitespace %s
//
// Test with serialization:
// RUN: %clang_cc1 -triple x86_64-unknown-unknown -emit-pch -o %t %s
// RUN: %clang_cc1 -x c++ -triple x86_64-unknown-unknown -include-pch %t -ast-dump-all /dev/null \
// RUN: | FileCheck -strict-whitespace %s

void first();

 [[ivl::ufcs]] void fn(int);
void fn(char);
void fn(long);

void test_free() {
  fn(123);
  fn('a');
  fn(321l);
  // fn(67ll);
}

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
