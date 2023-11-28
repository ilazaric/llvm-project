

constexpr auto MagicFn(auto... args){
  return (0 + ... + args);
}

static_assert(MagicFn(1, 2, 3) == 6);
