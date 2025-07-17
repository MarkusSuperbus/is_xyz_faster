#include <functional>
#include <optional>
#include <utility>
namespace datastructures {

template <typename Key, typename Value> class SearchTree {
public:
  SearchTree() {}
  SearchTree(const SearchTree &) = default;
  SearchTree(SearchTree &&) = default;
  SearchTree &operator=(const SearchTree &) = default;
  SearchTree &operator=(SearchTree &&) = default;

  using EntryT = std::pair<Key, Value>;

  bool insert(Key &&key, Value &&value) {
    if (root == nullptr) {
      root = new InternalNode{std::move(key), std::move(value)};
      return true;
    }
    InternalNodePtr position = root;
    while (position != nullptr) {
      if (position->entry.first < key) {
        if (position->left == nullptr) {
          position->left = new InternalNode{std::move(key), std::move(value)};
          return true;
        }
        position = position->left;
      } else if (position->entry.first > key) {
        if (position->right == nullptr) {
          position->right = new InternalNode{std::move(key), std::move(value)};
          return true;
        }
        position = position->right;
      } else {
        return false;
      }
    }
    return true;
  }

  std::optional<std::reference_wrapper<const Value>> find(Key &&key) const {
    const InternalNodePtr *position = nullptr;
    if (root != nullptr) {
      position = &root;
    }
    while (*position != nullptr) {
      InternalNodePtr value = *position;
      if (value->entry.first < key) {
        position = &value->left;
      } else if (value->entry.first > key) {
        position = &value->right;
      } else {
        return {std::ref(value->entry.second)};
      }
    }
    return {};
  }

  ~SearchTree() {
      InternalNode::clear(root);
  }

private:
  class InternalNode;
  using InternalNodePtr = InternalNode *;

  class InternalNode {
  public:
    EntryT entry;
    InternalNodePtr left = nullptr;
    InternalNodePtr right = nullptr;

    InternalNode(Key &&key, Value &&value)
        : entry(std::make_pair(std::move(key), std::move(value))) {}
    static void clear(InternalNodePtr child) {
      if (child != nullptr) {
        child->clear();
        delete child;
      }
    }

    void clear() {
      clear(left);
      clear(right);
    }
  };

  InternalNodePtr root = nullptr;
};
} // namespace datastructures
