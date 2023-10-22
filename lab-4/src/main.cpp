#include <algorithm>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

std::vector<int64_t> ZFunction(const std::vector<int64_t> &input) {
  std::vector<int64_t> result(input.size());

  int64_t left = 0;
  int64_t right = 0;

  for (int64_t i = 1; i < input.size(); ++i) {
    //
    if (i <= right) {
      result[i] = std::min(right - i + 1, result[i - left]);
    }

    while ( //
        i + result[i] < input.size() &&
        input[result[i]] == input[i + result[i]] //
    ) {
      ++result[i];
    }

    if (i + result[i] - 1 > right) {
      left = i, right = i + result[i] - 1;
    }
  }
  return result;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::vector<int64_t> example;

  { // read example
    std::string string_example;
    getline(std::cin, string_example);

    std::stringstream sin(string_example);
    int64_t tmp;
    while (sin >> tmp) {

      example.push_back(tmp);
    }
  }

  const auto example_size = example.size();

  { // insert divider
    example.push_back(-1);
  }

  std::vector<int> lines(1, 0);
  std::vector<int64_t> text;

  { // read text
    std::string line;
    while (getline(std::cin, line)) {

      std::stringstream sin(line);
      int64_t tmp;
      int k(0);
      while (sin >> tmp) {

        ++k;
        example.push_back(tmp);
      }
      lines.push_back(*lines.rbegin() + k);
    }
  }

  { // merge for Z-function

    // for some reason if I'll make push_back here this -1 will be in the end of
    // the example, don't know why(

    //    example.push_back(-1);

    example.insert(example.end(), std::make_move_iterator(text.begin()),
                   std::make_move_iterator(text.end()));
  }

  { // calculate answer
    const auto out = ZFunction(example);

    for (auto i(example_size); i < example.size(); ++i) {
      const auto real_i = i - example_size;

      if (out[i] == example_size) {
        const auto it = std::lower_bound(lines.begin(), lines.end(), real_i);

        if (it == lines.end() || it == lines.begin()) {
          return 0;
        }
        std::cout << (it - lines.begin()) << ", " << real_i - (*(it - 1))
                  << std::endl;
      }
    }
  }
}
