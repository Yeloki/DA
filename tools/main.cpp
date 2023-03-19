#include "containers/vector.hpp"
//#include <vector>
#include <iostream>



//template<typename T>
//std::ostream &operator<<(std::ostream &os, const ::tools::containers::Vector<T> &data) {
//  os << "{";
//  for (size_t i(0); i < data.size(); ++i) {
//    os << data[i] << (i + 1 == data.size() ? "" : ", ");
//  }
//  os << "}";
//  return os;
//}


int main() {
  {
    Vector<int> test;
    test.PushBack(1);
    test.PushBack(2);
    test.PushBack(3);
//    std::cout << test;
    test.Resize(5, 0);
//    std::cout << test;
  }


  return 0;
}