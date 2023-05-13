#define DEBUG
#include <cassert>
#include <cctype>
#include <iostream>

#include "containers/string.hpp"
#include "containers/vector.hpp"
#include "containers/vector_tools.hpp"

int main() {
  tools::containers::string_test::Test();
  tools::containers::vector_test::Test();
  tools::containers::vector_tools_test::Test();

  return 0;
}