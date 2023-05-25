#include "containers/avl_tree.hpp"
#include <fstream>
#include <iostream>
#include <optional>
#include <string>

class Dict {
public:
  Dict() : data() {}

  bool AddWord(const std::string &word, uint64_t payload) {
    if (data.Find(word)() != nullptr) {
      return false;
    }

    data[word] = payload;
    return true;
  }

  bool RemoveWord(const std::string &word) {
    if (data.Find(word)() == nullptr) {
      return false;
    }
    data.Remove(word);
    return true;
  }

  [[nodiscard]] std::optional<uint64_t> Find(const std::string &word) const {
    const auto &it = data.Find(word);
    if (it()) {
      return it()->value;
    } else {
      return std::nullopt;
    }
  }

  void Dump(const std::string &filename) {
    std::ofstream fout(filename, std::ios::binary);
    auto iter = data.Begin();
    fout << data.Size() << ' ';
    if (iter() != nullptr) {
      do {
        fout << iter()->key << " " << iter()->value << " ";
      } while (iter.next());
    }
    fout.close();
  }

  void Load(const std::string &filename) {
    std::ifstream fin(filename, std::ios::binary);
    if (fin) {
      size_t size;
      fin >> size;
      data.Clear();
      for (size_t i(0); i < size; ++i) {
        std::string key;
        uint64_t val;
        fin >> key >> val;
        data[key] = val;
      }
    } else {
      throw std::runtime_error("Can't open file");
    }
  }

private:
  tools::containers::AVLTree<std::string, uint64_t> data;
};

std::string str_tolower(std::string s) {
  std::transform(s.begin(), s.end(), s.begin(),
                 [](unsigned char c){ return std::tolower(c); }
  );
  return s;
}

int main() {
  Dict dict;
  std::string token;
  while (std::cin >> token) {
    try {
      if (token == "+") {
        std::string key;
        uint64_t val;
        std::cin >> key >> val;
        const auto res = dict.AddWord(str_tolower(key), val);
        if (res) {
          std::cout << "OK" << std::endl;
        } else {
          std::cout << "Exist" << std::endl;
        }
      } else if (token == "-") {
        std::string key;
        std::cin >> key;
        const auto res = dict.RemoveWord(str_tolower(key));
        if (res) {
          std::cout << "OK" << std::endl;
        } else {
          std::cout << "NoSuchWord" << std::endl;
        }
      } else if (token == "!") {
        std::string token2, path;
        std::cin >> token2 >> path;
        if (token2 == "Save") {
          dict.Dump(path);
          std::cout << "OK" << std::endl;
        } else if (token2 == "Load") {
          dict.Load(path);
          std::cout << "OK" << std::endl;
        } else {
          throw std::runtime_error("Wrong general operand");
        }
      } else {
        const auto res = dict.Find(str_tolower(token));
        if (res) {
          std::cout << "OK: " << res.value() << std::endl;
        } else {
          std::cout << "NoSuchWord" << std::endl;
        }
      }
    } catch (const std::exception &ex) {
      std::cout << "ERROR: " << ex.what() << std::endl;
    }
  }
}