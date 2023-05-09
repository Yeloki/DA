#include <cassert>
#include <cctype>
#include <iostream>
#include <string>

namespace tools::containers {

class String {
public:
  String() {
    data_ = new char[4];
    capacity_ = 4;
    size_ = 0;
  }

  String(const String &string) {
    data_ = new char[string.size_];
    capacity_ = string.capacity_;
    size_ = string.size_;
    for (size_t i(0); i < size_; ++i) {
      data_[i] = string.data_[i];
    }
  }

  explicit String(const char string[]) {
    data_ = new char[strlen(string)];
    capacity_ = strlen(string);
    size_ = strlen(string);
    for (size_t i(0); i < size_; ++i) {
      data_[i] = string[i];
    }
  }

  size_t Size() const { return size_; }

  ~String() { delete data_; }

  char &operator[](size_t index) {
    if (index >= size_)
      throw std::runtime_error("Bad index");
    return data_[index];
  }

  const char &operator[](size_t index) const {
    if (index >= size_)
      throw std::runtime_error("Bad index");
    return data_[index];
  }

  void Insert(char v) {
    if (size_ + 1 > capacity_) {
      Resize();
    }
    data_[size_++] = v;
  }

  void Erase(size_t i, size_t cnt) {
    if (i + cnt > size_)
      throw std::runtime_error("Bad index");
    for (size_t i_v(i + cnt); i_v < size_; ++i_v) {
      data_[i_v - cnt] = data_[i_v];
    }
    size_ -= cnt;
  }

private:
  char *data_;
  size_t size_;
  size_t capacity_;

  void Resize() {
    capacity_ *= 2;
    char *n_data = new char[capacity_];
    for (size_t i(0); i < size_; ++i) {
      n_data[i] = data_[i];
    }
    delete data_;
    data_ = n_data;
  }
};

std::ostream &operator<<(std::ostream &os, String s) {
  for (size_t i(0); i < s.Size(); ++i) {
    os << s[i];
  }
  return os;
}

std::istream &operator>>(std::istream &is, String s) {
  for (size_t i(0); i < s.Size(); ++i) {
    char tmp;
    is >> tmp;
    s.Insert(tmp);
  }
  return is;
}

bool operator==(const String &sl, const String &sr) {
  if (sl.Size() != sr.Size())
    return false;
  for (size_t i(0); i < sl.Size(); ++i) {
    if (sl[i] != sr[i])
      return false;
  }
  return true;
}

#ifdef DEBUG

namespace string_test {
namespace {

void AssertEq(const String &str, const std::string &expected) {
  assert(str.Size() == expected.size());
  for (int i(0); i < expected.size(); ++i) {
    assert(str[i] == expected[i]);
  }
}
} // namespace

void Test() {
  {
    String s1;
    { String s2 = s1; }
    { String s3(s1); }
  }

  String s1;
  s1.Insert('a');

  std::string expected_result_1 = "a";
  assert(s1[0] == expected_result_1[0]);
  s1.Insert('b');
  s1.Insert('c');
  std::string expected_result_2 = "abc";
  AssertEq(s1, expected_result_2);

  s1.Insert('d');
  s1.Insert('e');
  s1.Insert('f');
  std::string expected_result_3 = "abcdef";
  AssertEq(s1, expected_result_3);

  s1.Erase(1, 3);
  std::string expected_result_4 = "aef";
  AssertEq(s1, expected_result_4);

  s1.Erase(1, 2);
  std::string expected_result_5 = "a";
  AssertEq(s1, expected_result_5);
}

} // namespace string_test
#endif

} // namespace tools::containers