

struct S {};

[[ivl::ufcs]]
void fn_ref(const S&);

// [[ivl::ufcs]]
// void fn_ptr(const S*);

struct T { S* operator->(); };

struct U { T operator->(); };

void use() {
  S s;
  s.fn_ref(); // should already work
  (&s)->fn_ref(); // no idea, doesnt work, want it to work
  // (&s).fn_ptr(); // no idea, doesnt work, unsure if want to work

  U u;
  u->fn_ref();
}
