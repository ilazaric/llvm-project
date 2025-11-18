
int* create();

struct A {
  void fn();
};

struct B {
  A* operator->();
};

struct C {
  B operator->();
};

void fn(){
  1 + 2 + 3;
  *create();

  C c;
  c->fn();
}
