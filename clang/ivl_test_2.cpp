#include <string>
#include <vector>
#include <ranges>
#include <iostream>

[[ivl::ufcs]]
std::vector<std::string> split(std::string_view sv) {
  std::vector<std::string> res{""};
  for (auto c : sv) {
    if (std::isspace(c))
      res.emplace_back();
    else
      res.back().push_back(c);
  }
  return res;
}

[[ivl::ufcs]]
auto wrap_each(auto &&r) {
  return r | std::views::transform(
                 [](std::string s) { return "[" + s + "]"; });
}

[[ivl::ufcs]]
std::string join(auto &&r, std::string_view sep) {
  std::string out;
  bool is_first = true;
  for (auto &&el : r) {
    if (!is_first)
      out += sep;
    is_first = false;
    out += el;
  }
  return out;
}

int main() {
  auto wrap_each = [])(auto&& r){
    return r | std::views::transform([](std::string s) { return "[" + s + "]"; });
  };
  
  std::cout << std::string("hello world  this is a message 123").split().wrap_each().join(", ") << std::endl;
}
