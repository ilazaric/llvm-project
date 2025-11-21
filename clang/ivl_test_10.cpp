#include <concepts>
#include <iostream>
#include <memory>
#include <ranges>
#include <type_traits>
#include <vector>

// some utils
namespace ufcs {

struct _this_t {};
inline constexpr _this_t _this;

template <typename Call, typename Head, typename... Tail>
decltype(auto) moveable_this_impl(const Call &callable, Head &&head,
                                  Tail &&...tail) {
  auto id_or_head = [&]<typename T>(T &&arg) -> decltype(auto) {
    if constexpr (std::same_as<_this_t, std::remove_cvref_t<T>>) {
      return std::forward<Head>(head);
    } else {
      return std::forward<T>(arg);
    }
  };
  return callable(id_or_head(std::forward<Tail>(tail))...);
}

auto moveable_this(const auto &callable) {
  return [&]<typename... Ts>(Ts &&...args) -> decltype(auto) {
    constexpr auto this_count =
        (0 + ... + (int)std::same_as<_this_t, std::remove_cvref_t<Ts>>);
    static_assert(this_count <=
                  2); // >= 2 could make sense if Head is not forwarded
    if constexpr (this_count == 0) {
      return callable(std::forward<Ts>(args)...);
    } else
      return moveable_this_impl(callable, std::forward<Ts>(args)...);
  };
}

template <typename T> [[ivl::ufcs]] auto convert(auto &&...args) {
  return moveable_this([](auto &&...args) {
    return T(std::forward<decltype(args)>(args)...);
  })(std::forward<decltype(args)>(args)...);
}

template <template <typename...> typename TT>
[[ivl::ufcs]] auto convert(auto &&...args) {
  return moveable_this([](auto &&...args) {
    return TT(std::forward<decltype(args)>(args)...);
  })(std::forward<decltype(args)>(args)...);
}

[[ivl::ufcs]]
auto filter(auto &&rg, auto &&pred) {
  return std::forward<decltype(rg)>(rg) |
         std::views::filter(std::forward<decltype(pred)>(pred));
}

} // namespace ufcs

int main() {
  auto vec =
      std::views::iota(0, 20)
          .ufcs::filter([](int x) { return x % 3 == 1; })
          .ufcs::template convert<std::vector>(std::from_range, ufcs::_this);

  std::cout << "Vector size: " << vec.size() << std::endl;
  for (auto el : vec)
    std::cout << "Element: " << el << std::endl;
}
