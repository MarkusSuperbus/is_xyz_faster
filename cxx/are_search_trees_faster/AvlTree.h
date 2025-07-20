#pragma once

#include "SearchTree.h"
#include <optional>
#include <utility>
namespace datastructures {

template <typename Key, typename Value> class AvlTree {
  struct InternalValue {
    Value value;
    int64_t balance;
  };

public:
  AvlTree() {}
  AvlTree(const AvlTree &) = default;
  AvlTree(AvlTree &&) = default;
  AvlTree &operator=(const AvlTree &) = default;
  AvlTree &operator=(AvlTree &&) = default;

  bool insert(Key &&key, Value &&value) {
    if (root == nullptr) {
      root = new InternalTree<Key, InternalValue>(
          std::move(key),
          InternalValue{.value = std::move(value), .balance = 0});
      return true;
    }
    // TODO: Implement insertion logic with rotations
    return false;
  }

  std::optional<std::reference_wrapper<const Value>> find(Key &&key) const {
    if (root == nullptr) {
      return {};
    }
    return root->findIfSearchTree(std::move(key))
        .transform(
            [](const std::reference_wrapper<const InternalValue> internal) {
              return std::ref(internal.get().value);
            });
  }

  ~AvlTree() = default;

private:
  InternalTreePtr<Key, InternalValue> root = nullptr;
};
} // namespace datastructures
