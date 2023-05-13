#include <iostream>

#include "containers/string.hpp"
#include "containers/vector.hpp"
#include "containers/vector_tools.hpp"

namespace tc = tools::containers;
namespace vt = tools::containers::vector_tools;

template <typename Tf, typename Ts> struct Pair {
  Tf first{};
  Ts second{};
};

template <typename Tf, typename Ts>
std::istream &operator>>(std::istream &is, Pair<Tf, Ts> &p) {
  is >> p.first >> p.second;
  return is;
}

using TV = Pair<size_t, tc::String>;

int main() {
  tc::Vector<TV> v;
  v.Reserve(10000);
  TV tmp;
  while (std::cin >> tmp) {
    v.PushBack(tmp);
  }
  vt::LinearStableSort<TV>(v, [](const TV &p) { return p.first; });
  for (size_t i(0); i < v.Size(); ++i) {
    std::cout << v[i].first << ' ' << v[i].second << '\n';
  }
  std::cout << std::endl;

  return 0;
}