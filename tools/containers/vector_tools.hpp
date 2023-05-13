#pragma once

#include <algorithm>
#include <functional>
#include <iostream>

#include "vector.hpp"

namespace tools::containers::vector_tools {

template <typename T>
void LinearStableSort(Vector<T> &v, std::function<size_t(T)> GetVal) {
  if (v.Size() < 2) {
    return;
  }

  size_t max_val = 0;
  for (size_t i(0); i < v.Size(); ++i) {
    max_val = std::max(max_val, GetVal(v[i]));
  }

  Vector<int> count(max_val + 1, 0);
  for (size_t i(0); i < v.Size(); ++i) {
    ++count[GetVal(v[i])];
  }

#ifdef DEBUG
  std::cout << count << std::endl;
#endif

  for (size_t i(1); i < count.Size(); ++i) {
    count[i] += count[i - 1];
  }

#ifdef DEBUG
  std::cout << count << std::endl;
#endif
  Vector<T> res(v.Size());

  for (int64_t i = v.Size() - 1; i >= 0; --i) {
    res[--count[GetVal(v[i])]] = v[i];
  }

#ifdef DEBUG
  std::cout << res << std::endl;
#endif

  v = std::move(res);
}

void LinearStableSort(Vector<int> &v) {
  LinearStableSort<int>(v,
                        [](int a) -> size_t { return static_cast<size_t>(a); });
}

} // namespace tools::containers::vector_tools


namespace tools::containers::vector_tools_test {
#ifdef DEBUG

namespace {

constexpr const char *kRunning = "[RUNNING]";
constexpr const char *kOk = "[OK]";
constexpr const char *kFailed = "[FAILED]";
constexpr const char *kReason = "Reason: ";

} // namespace

void Test() {
  { /////////////////////////////////////////////////////////////////
    constexpr const char *kTestName = "test linear stable sort vector";
    std::cout << kRunning << ' ' << kTestName << std::endl;
    bool is_success(false);
    try {
      {
        const std::vector<int> expected = {0, 1, 2, 4, 4, 5, 9};
        Vector v = {4, 2, 0, 9, 5, 1, 4};
        vector_tools::LinearStableSort(v);
        AssertEqual(v, expected, std::to_string(__LINE__));
      }
      {
        const std::vector<std::pair<int, int>> expected = {
            {0, 3}, {1, 6}, {2, 2}, {4, 1}, {4, 7}, {5, 5}, {9, 4}};

        Vector<std::pair<int, int>> v = {{4, 1}, {2, 2}, {0, 3}, {9, 4},
                                         {5, 5}, {1, 6}, {4, 7}};

        vector_tools::LinearStableSort<std::pair<int, int>>(v, [](std::pair<int, int> p) {
          return static_cast<size_t>(p.first);
        });

        AssertEqual(v, expected, std::to_string(__LINE__));
      }

      is_success = true;
    } catch (const TestError &te) {
      std::cout << kFailed << ' ' << kTestName << std::endl;
      std::cout << kReason << ' ' << te.what() << std::endl;
    }
    if (is_success)
      std::cout << kOk << ' ' << kTestName << std::endl;
  } /////////////////////////////////////////////////////////////////
}

#endif

}