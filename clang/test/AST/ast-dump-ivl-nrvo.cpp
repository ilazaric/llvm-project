// RUN: %clang_cc1 -std=c++23 -triple x86_64-linux-gnu -ast-dump %s \
// RUN: | FileCheck -strict-whitespace %s

template<typename T>
struct ST {
  ST();
  ST(const ST&);
  ST(ST&&);
  ST& operator=(const ST&);
  ST& operator=(ST&&);
  ~ST();
};

using S = ST<int>;

S generate();

S passthrough() {
   S obj [[ivl::nrvo]] = generate();
  return obj;
}

// check we don't odr-use constructors, if we did we would see `used` plastered on them
// we don't check the destructor because it probably should be odr-used, for unwinding
// CHECK: | `-ClassTemplateSpecializationDecl {{.*}} <line:4:1, line:12:1> line:5:8 struct ST definition instantiated_from {{.*}} implicit_instantiation
// CHECK-NEXT: |   |-DefinitionData empty standard_layout has_user_declared_ctor can_const_default_init
// CHECK-NEXT: |   | |-DefaultConstructor exists non_trivial user_provided defaulted_is_constexpr
// CHECK-NEXT: |   | |-CopyConstructor non_trivial user_declared has_const_param needs_overload_resolution implicit_has_const_param
// CHECK-NEXT: |   | |-MoveConstructor exists non_trivial user_declared
// CHECK-NEXT: |   | |-CopyAssignment non_trivial has_const_param user_declared needs_overload_resolution implicit_has_const_param
// CHECK-NEXT: |   | |-MoveAssignment exists non_trivial user_declared
// CHECK-NEXT: |   | `-Destructor non_trivial user_declared
// CHECK-NEXT: |   |-TemplateArgument type 'int'
// CHECK-NEXT: |   | `-BuiltinType {{.*}} 'int'
// CHECK-NEXT: |   |-CXXRecordDecl {{.*}} <col:1, col:8> col:8 implicit struct ST
// CHECK-NEXT: |   |-CXXConstructorDecl {{.*}} <line:6:3, col:6> col:3 ST 'void ()' implicit_instantiation instantiated_from {{.*}}
// CHECK-NEXT: |   |-CXXConstructorDecl {{.*}} <line:7:3, col:15> col:3 ST 'void (const ST<int> &)' implicit_instantiation instantiated_from {{.*}}
// CHECK-NEXT: |   | `-ParmVarDecl {{.*}} <col:6, col:14> col:15 'const ST<int> &'
// CHECK-NEXT: |   |-CXXConstructorDecl {{.*}} <line:8:3, col:10> col:3 ST 'void (ST<int> &&)' implicit_instantiation instantiated_from {{.*}}
// CHECK-NEXT: |   | `-ParmVarDecl {{.*}} <col:6, col:8> col:10 'ST<int> &&'
// CHECK-NEXT: |   |-CXXMethodDecl {{.*}} <line:9:3, col:26> col:7 operator= 'ST<int> &(const ST<int> &)' implicit_instantiation instantiated_from {{.*}}
// CHECK-NEXT: |   | `-ParmVarDecl {{.*}} <col:17, col:25> col:26 'const ST<int> &'
// CHECK-NEXT: |   |-CXXMethodDecl {{.*}} <line:10:3, col:21> col:7 operator= 'ST<int> &(ST<int> &&)' implicit_instantiation instantiated_from {{.*}}
// CHECK-NEXT: |   | `-ParmVarDecl {{.*}} <col:17, col:19> col:21 'ST<int> &&'

// TODO: add checking of the passthrough() AST
