// RUN: %clang_cc1 -triple x86_64-unknown-unknown -ast-dump %s \
// RUN: | FileCheck -strict-whitespace %s

// This tests that [[ivl::ufcs]] functions don't come into play when regular member functions would,
// meaning previously well-formed code has the same meaning.
namespace TEST1 {
  struct S {
    void foo(int);
  };

  [[ivl::ufcs]] void foo(char);
  void foo(long);
  
  void test() {
    S s;
    s.foo(1);
    s.foo('a');
    s.foo(2l);
  }
}
// CHECK: `-NamespaceDecl {{.*}} TEST1
// CHECK-NEXT:   |-CXXRecordDecl {{.*}} referenced struct S definition
// CHECK-NEXT:   | |-DefinitionData pass_in_registers empty aggregate standard_layout trivially_copyable pod trivial literal has_constexpr_non_copy_move_ctor can_const_default_init
// CHECK-NEXT:   | | |-DefaultConstructor exists trivial constexpr defaulted_is_constexpr
// CHECK-NEXT:   | | |-CopyConstructor simple trivial has_const_param implicit_has_const_param
// CHECK-NEXT:   | | |-MoveConstructor exists simple trivial
// CHECK-NEXT:   | | |-CopyAssignment simple trivial has_const_param needs_implicit implicit_has_const_param
// CHECK-NEXT:   | | |-MoveAssignment exists simple trivial needs_implicit
// CHECK-NEXT:   | | `-Destructor simple irrelevant trivial needs_implicit
// CHECK-NEXT:   | |-CXXRecordDecl {{.*}} implicit struct S
// CHECK-NEXT:   | |-CXXMethodDecl {{.*}} used foo 'void (int)'
// CHECK-NEXT:   | | `-ParmVarDecl {{.*}} 'int'
// CHECK-NEXT:   | |-CXXConstructorDecl {{.*}} implicit used constexpr S 'void () noexcept' inline default trivial
// CHECK-NEXT:   | | `-CompoundStmt {{.*}}
// CHECK-NEXT:   | |-CXXConstructorDecl {{.*}} implicit constexpr S 'void (const S &)' inline default trivial noexcept-unevaluated {{.*}}
// CHECK-NEXT:   | | `-ParmVarDecl {{.*}} 'const S &'
// CHECK-NEXT:   | `-CXXConstructorDecl {{.*}} implicit constexpr S 'void (S &&)' inline default trivial noexcept-unevaluated {{.*}}
// CHECK-NEXT:   |   `-ParmVarDecl {{.*}} 'S &&'
// CHECK-NEXT:   |-FunctionDecl {{.*}} foo 'void (char)'
// CHECK-NEXT:   | |-ParmVarDecl {{.*}} 'char'
// CHECK-NEXT:   | `-IVLUFCSAttr {{.*}}
// CHECK-NEXT:   |-FunctionDecl {{.*}} foo 'void (long)'
// CHECK-NEXT:   | `-ParmVarDecl {{.*}} 'long'
// CHECK-NEXT:   `-FunctionDecl {{.*}} test 'void ()'
// CHECK-NEXT:     `-CompoundStmt {{.*}}
// CHECK-NEXT:       |-DeclStmt {{.*}}
// CHECK-NEXT:       | `-VarDecl {{.*}} used s 'S' callinit
// CHECK-NEXT:       |   `-CXXConstructExpr {{.*}} 'S' 'void () noexcept'
// CHECK-NEXT:       |-CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT:       | |-MemberExpr {{.*}} '<bound member function type>' .foo {{.*}}
// CHECK-NEXT:       | | `-DeclRefExpr {{.*}} 'S' lvalue Var {{.*}} 's' 'S'
// CHECK-NEXT:       | `-IntegerLiteral {{.*}} 'int' 1
// CHECK-NEXT:       |-CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT:       | |-MemberExpr {{.*}} '<bound member function type>' .foo {{.*}}
// CHECK-NEXT:       | | `-DeclRefExpr {{.*}} 'S' lvalue Var {{.*}} 's' 'S'
// CHECK-NEXT:       | `-ImplicitCastExpr {{.*}} 'int' <IntegralCast>
// CHECK-NEXT:       |   `-CharacterLiteral {{.*}} 'char' 97
// CHECK-NEXT:       `-CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT:         |-MemberExpr {{.*}} '<bound member function type>' .foo {{.*}}
// CHECK-NEXT:         | `-DeclRefExpr {{.*}} 'S' lvalue Var {{.*}} 's' 'S'
// CHECK-NEXT:         `-ImplicitCastExpr {{.*}} 'int' <IntegralCast>
// CHECK-NEXT:           `-IntegerLiteral {{.*}} 'long' 2
