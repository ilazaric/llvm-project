# trying to implement generalized pack

`Ts` is pack of types  
```
auto... args = Ts{};
Ts... args;
using... Ts2 = Ts;
using... Ts3 = ...{Ts...};
struct S {Ts... elems;};
Ts...[1:] args;
Ts...[:] <=> Ts
...{Ts...} <=> Ts <=> ...{Ts...[0], Ts...[1:]...} (last one if Ts not empty)
```

packs of packs seem disgusting?

```
...{int, float, char}... v1;
auto... v2 = ...{v1, v1, v1}; // wtf is this
```

dont think ^ would work tbh
though smth like:

```
std::tuple... v2 = {...{v1, v1, v1}};
```

from the proposal it seems like nested packs cant be created so explicitly

perhaps:

```
auto... ... v2 = ...{v1, v1, v1};
```

looks pretty funny
ignore nested packs for now
idea
introduce type and value packs
`...{a, b, c}` would be a TypePackExpr or ValuePackExpr
`Ts... args;` would be a ValuePackDecl?
`using... Ts = ...{int, int};` a TypePackDecl?

`ivl clang -Xclang -ast-dump -std=c++23 -fsyntax-only ivl/examples/fold.cpp`
```
...
|   |-ParmVarDecl 0x564c8c4f50a0 <col:24, col:32> col:32 used args 'int':'int'
|   |-ParmVarDecl 0x564c8c4f5150 <col:24, col:32> col:32 used args 'int':'int'
|   |-ParmVarDecl 0x564c8c4f5200 <col:24, col:32> col:32 used args 'int':'int'
...
```
kinda hate ^
would love if identifiers were `args#0`, `args#1`, `args#2`
perhaps a `constexpr std::size_t args## = 3;` as well
