#pragma once
#include <cstdint>
#include <optional>

namespace tools::containers {
namespace {

template <typename Tk, typename Tv> struct Node {
  const Tk key;
  Tv value;
  uint8_t height;
  Node *left;
  Node *right;
  Node *parent;

  explicit Node(const Tk &k, const Tv &v) : key(k) {
    value = v;
    left = right = nullptr;
    height = 1;
    parent = nullptr;
  }

  explicit Node(const Tk &k, const Tv &v, Node<Tk, Tv> p) : key(k) {
    value = v;
    left = right = nullptr;
    height = 1;
    parent = p;
  }
};

template <typename Tk, typename Tv> uint8_t Height(const Node<Tk, Tv> *node) {
  return node ? node->height : 0;
}

template <typename Tk, typename Tv> int BFactor(const Node<Tk, Tv> *node) {
  return Height(node->right) - Height(node->left);
}

template <typename Tk, typename Tv> void FixHeight(Node<Tk, Tv> *p) {
  uint8_t hl = Height(p->left);
  uint8_t hr = Height(p->right);
  p->height = (hl > hr ? hl : hr) + 1;
}

template <typename Tk, typename Tv> Node<Tk, Tv> *RightPivot(Node<Tk, Tv> *p) {
  Node<Tk, Tv> *q = p->left;

  p->parent = nullptr;
  q->parent = p;
  if (p->right) {
    p->right->parent = q;
  }
  p->left = q->right;
  q->right = p;
  FixHeight(p);
  FixHeight(q);
  return q;
}

template <typename Tk, typename Tv> Node<Tk, Tv> *LeftPivot(Node<Tk, Tv> *q) {
  Node<Tk, Tv> *p = q->right;

  p->parent = nullptr;
  q->parent = p;
  if (p->left) {
    p->left->parent = q;
  }

  q->right = p->left;
  p->left = q;
  FixHeight(q);
  FixHeight(p);
  return p;
}

template <typename Tk, typename Tv> Node<Tk, Tv> *Balance(Node<Tk, Tv> *p) {
  FixHeight(p);
  if (BFactor(p) == 2) {
    if (BFactor(p->right) < 0) {
      p->right = RightPivot(p->right);
      p->right->parent = p;
    }
    return LeftPivot(p);
  }
  if (BFactor(p) == -2) {
    if (BFactor(p->left) > 0) {
      p->left = LeftPivot(p->left);
      p->left->parent = p;
    }
    return RightPivot(p);
  }
  return p;
}

template <typename Tk, typename Tv>
Node<Tk, Tv> *Insert(Node<Tk, Tv> *p, const Tk &key, const Tv &val) {
  if (!p)
    return new Node<Tk, Tv>(key, val);
  if (key < p->key && !p->left) {
    p->left = new Node<Tk, Tv>(key, val);
    p->left->parent = p;
    return Balance(p);
  } else if (key >= p->key && !p->right) {
    p->right = new Node<Tk, Tv>(key, val);
    p->right->parent = p;
    return Balance(p);
  }
  if (key < p->key) {
    p->left = Insert(p->left, key, val);
  } else {
    p->right = Insert(p->right, key, val);
  }
  return Balance(p);
}

template <typename Tk, typename Tv>
Node<Tk, Tv> *Get(Node<Tk, Tv> *n, const Tk &key) {
  if (!n)
    return nullptr;
  if (key < n->key) {
    return Get(n->left, key);
  } else if (key > n->key) {
    return Get(n->right, key);
  } else {
    return n;
  }
}

template <typename Tk, typename Tv> Node<Tk, Tv> *FindMin(Node<Tk, Tv> *p) {
  return p->left ? FindMin(p->left) : p;
}

template <typename Tk, typename Tv> Node<Tk, Tv> *RemoveMin(Node<Tk, Tv> *p) {
  if (p->left == nullptr)
    return p->right;
  p->left = RemoveMin(p->left);
  if (p->left) {
    p->left->parent = p;
  }
  return Balance(p);
}

template <typename Tk, typename Tv>
Node<Tk, Tv> *RemoveNode(Node<Tk, Tv> *p, const Tk &k) {
  if (!p)
    return nullptr;
  if (k < p->key) {
    p->left = RemoveNode(p->left, k);
    if (p->left) {
      p->left->parent = p;
    }
  }

  else if (k > p->key) {
    p->right = RemoveNode(p->right, k);
    if (p->right) {
      p->right->parent = p;
    }
  } else {
    Node<Tk, Tv> *l = p->left;
    Node<Tk, Tv> *r = p->right;
    delete p;
    if (l) {
      l->parent = nullptr;
    }
    if (!r) {
      return l;
    }
    r->parent = nullptr;

    Node<Tk, Tv> *min = FindMin(r);
    min->right = RemoveMin(r);
    if (min->right) {
      min->right->parent = min;
    }
    min->left = l;
    if (min->left) {
      min->left->parent = min;
    }
    return Balance(min);
  }
  return Balance(p);
}

} // namespace

template <typename Tk, typename Tv> class AVLTreeIterator {
public:
  explicit AVLTreeIterator(Node<Tk, Tv> *node) {
    if (!node) {
      current = nullptr;
      prev = nullptr;
      return;
    }
    current = node;
    prev = current->left;
  }

  Node<Tk, Tv> *operator()() { return current; }
  const Node<Tk, Tv> *operator()() const { return current; }
  Node<Tk, Tv> *next() {
    if (current == nullptr)
      return nullptr;
    if (current->left == prev) {
      prev = current;
      if (current->right) {
        current = current->right;
        while (current->left && current->left) {
          current = current->left;
        }
        prev = current->parent;
      } else {
        current = current->parent;
      }
    } else if (current->right == prev) {
      prev = current;
      current = current->parent;
    } else if (current->parent == prev) {
      prev = current;
      if (current->left) {
        current = current->left;
      } else if (current->right) {
        current = current->right;
      } else {
        while (current->parent && current->parent->right == current) {
          current = current->parent;
        }
        prev = current;
        if (current->parent) {
          current = current->parent;
        } else {
          current = nullptr;
        }
      }
    }
    return current;
  }

private:
  Node<Tk, Tv> *current;
  Node<Tk, Tv> *prev;
};

template <typename Tk, typename Tv> class AVLTree {
public:
  AVLTree() {
    root = nullptr;
    size = 0;
  }

  const Tv &operator[](const Tk &key) const {
    auto node = Get(root, key);
    if (node == nullptr) {
      throw std::runtime_error("Node with this key doesn't exists");
    } else {
      return node->value;
    }
  }

  Tv &operator[](const Tk &key) {
    auto node = Get(root, key);
    if (node == nullptr) {
      root = Insert(root, key, Tv());
      ++size;
      return Get(root, key)->value;
    } else {
      return node->value;
    }
  }

  AVLTreeIterator<Tk, Tv> Begin() {
    if (!root) {
      return AVLTreeIterator<Tk, Tv>(nullptr);
    }
    auto n = root;
    while (n->left != nullptr) {
      n = n->left;
    }
    return AVLTreeIterator(n);
  }

  void Remove(const Tk &key) {
    root = RemoveNode(root, key);
    --size;
  }

  AVLTreeIterator<Tk, Tv> Find(const Tk &key) {
    auto node = Get(root, key);
    return AVLTreeIterator(node);
  }

  AVLTreeIterator<Tk, Tv> Find(const Tk &key) const {
    auto node = Get(root, key);
    return AVLTreeIterator(node);
  }

  ~AVLTree() { Clear(); }

  void Clear() {
    Clear(root);
    root = nullptr;
    size = 0;
  }
  size_t Size() { return size; }

private:
  void Clear(Node<Tk, Tv> *node) {
    if (!node) {
      return;
    }
    if (node->left) {
      Clear(node->left);
    }
    if (node->right) {
      Clear(node->right);
    }

    delete node;
  }

  Node<Tk, Tv> *root;
  size_t size;
};

namespace avl_tree {

#ifdef DEBUG
namespace {

constexpr const char *kRunning = "[RUNNING]";
constexpr const char *kOk = "[OK]";
constexpr const char *kFailed = "[FAILED]";
constexpr const char *kReason = "Reason: ";

} // namespace

void Test() {
  { /////////////////////////////////////////////////////////////////
    tools::containers::AVLTree<int, int> tree;
    for (int i(0); i < 100000; ++i) {
      std::vector<int> e;
      for (int j(0); j < i; ++j) {
        tree[j] = j;
        e.push_back(j);
      }
      std::vector<int> v;

      auto it = tree.Begin();
      if (it()) {
        do {
          v.push_back(it()->value);
        } while (it.next() != nullptr);
      }
      if (v.size() != e.size()) {
        std::cout << "FAIL ON ELEM: " << i << std::endl;
        throw std::runtime_error("1");
      }
      for (int k(0); k < e.size(); ++k) {
        if (v[k] != e[k]) {
          std::cout << "FAIL ON ELEM: " << i << " WITH INDEX " << k
                    << std::endl;
          throw std::runtime_error("2");
        }
      }
    }
  } /////////////////////////////////////////////////////////////////

  { /////////////////////////////////////////////////////////////////
    tools::containers::AVLTree<int, int> tree;
    for (int i(0); i < 10000; ++i) {
      std::vector<int> e;
      for (int j(0); j < i; ++j) {
        tree[j] = j;
        e.push_back(j);
      }
      std::vector<int> v;
      for (int j(i - 1); j >= 0; --j) {
        auto it = tree.Begin();
        e.pop_back();
        if (it()) {
          do {
            v.push_back(it()->value);
          } while (it.next() != nullptr);
        }
        if (v.size() != e.size()) {
          std::cout << "FAIL ON ELEM: " << i << std::endl;
          throw std::runtime_error("1");
        }
        for (int k(0); k < e.size(); ++k) {
          if (v[k] != e[k]) {
            std::cout << "FAIL ON ELEM: " << i << " WITH INDEX " << k
                      << std::endl;
            throw std::runtime_error("2");
          }
        }
      }
    }
  } /////////////////////////////////////////////////////////////////
}
#endif
} // namespace avl_tree

} // namespace tools::containers