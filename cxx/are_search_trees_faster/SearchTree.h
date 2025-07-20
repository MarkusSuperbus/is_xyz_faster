#pragma once

#include <functional>
#include <optional>
#include <utility>
namespace datastructures {

template <typename Key, typename Value> class InternalTree;
template <typename Key, typename Value>
using InternalTreePtr = InternalTree<Key, Value> *;
template <typename Key, typename Value> class InternalTree {
public:
  using EntryT = std::pair<Key, Value>;
  EntryT entry;
  InternalTreePtr<Key, Value> left = nullptr;
  InternalTreePtr<Key, Value> right = nullptr;

  InternalTree(Key &&key, Value &&value)
      : entry(std::make_pair(std::move(key), std::move(value))) {}
  static void clear(InternalTreePtr<Key, Value> child) {
    if (child != nullptr) {
      child->clear();
      delete child;
    }
  }

  void clear() {
    clear(left);
    clear(right);
  }

  std::optional<std::reference_wrapper<const Value>>
  findIfSearchTree(Key &&key) const {
    const InternalTreePtr<Key, Value> *position = nullptr;
    if (entry.first < key) {
      position = &left;
    } else if (entry.first > key) {
      position = &right;
    } else {
      return {std::ref(entry.second)};
    }
    while (*position != nullptr) {
      InternalTreePtr<Key, Value> value = *position;
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
};

template <typename Key, typename Value> class SearchTree {
public:
  SearchTree() {}
  SearchTree(const SearchTree &) = default;
  SearchTree(SearchTree &&) = default;
  SearchTree &operator=(const SearchTree &) = default;
  SearchTree &operator=(SearchTree &&) = default;

  bool insert(Key &&key, Value &&value) {
    if (root == nullptr) {
      root = new InternalTree{std::move(key), std::move(value)};
      return true;
    }
    InternalTreePtr<Key, Value> position = root;
    while (position != nullptr) {
      if (position->entry.first < key) {
        if (position->left == nullptr) {
          position->left = new InternalTree{std::move(key), std::move(value)};
          return true;
        }
        position = position->left;
      } else if (position->entry.first > key) {
        if (position->right == nullptr) {
          position->right = new InternalTree{std::move(key), std::move(value)};
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
    if (root == nullptr) {
      return {};
    }
    return root->findIfSearchTree(std::move(key));
  }

  ~SearchTree() { InternalTree<Key, Value>::clear(root); }

private:
  InternalTreePtr<Key, Value> root = nullptr;
};
} // namespace datastructures
