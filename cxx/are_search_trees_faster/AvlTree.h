#pragma once

#include "SearchTree.h"
#include <cassert>
#include <optional>
#include <stdexcept>
#include <utility>
namespace datastructures {

template <typename Key, typename Value> class AvlTree {
  struct InternalValue {
    Value value;
    int64_t height;
  };

  enum InsertionPosition { left, right, here, none };

public:
  AvlTree() {}
  AvlTree(const AvlTree &) = default;
  AvlTree(AvlTree &&) = default;
  AvlTree &operator=(const AvlTree &) = default;
  AvlTree &operator=(AvlTree &&) = default;

  static int64_t height(InternalTreePtr<Key, InternalValue> internal) {
    if (internal == nullptr) {
      return 0;
    }
    return internal->entry.second.height;
  }

  static int64_t balance(InternalTreePtr<Key, InternalValue> internal) {
    if (internal == nullptr) {
      return 0;
    }
    return height(internal->right) - height(internal->left);
  }

  static void rebalance(InternalTreePtr<Key, InternalValue>& internal, InsertionPosition nextInsertionPosition, InsertionPosition result) {
auto bal = balance(internal);
    switch (nextInsertionPosition) {
    case InsertionPosition::here:
      break;
    case InsertionPosition::left:
      if (bal <= -2) {
        // single rotation
        switch (result) {
        case InsertionPosition::none:
        case InsertionPosition::here:
          assert(false);
          throw std::runtime_error("Unreachable");
        case InsertionPosition::left: {
          // clang-format off
            /*
             *      x
             *     / \
             *    y   C
             *   / \
             *  A   B
             *  –––––––––––––
             *      y
             *     / \
             *    A   x
             *       / \
             *      B   C
             * */
          // clang-format on
          // auto formerLeft = internal->left;   // Store [y, A, B]
          // internal->left = formerLeft->right; // [x, B, C], [y, A, B]
          // formerLeft->right = internal;       // [y, A, [x, B, C]]
          // internal = formerLeft;              // internal node is overwritten
          // internal->entry.second.balance = 0;
          break;
        }
        case InsertionPosition::right: {
          // double rotation
          // clang-format off
            /*
             *      x
             *     / \
             *    y   D
             *   / \
             *  A   z
             *     / \
             *    B   C
             *  –––––––––––––
             *      y
             *     / \
             *    A   x
             *       / \
             *      B   C
             * */
          // clang-format on
          break;
        }
        }
      }
      break;
    case InsertionPosition::right: {
      if (bal >= 2) {
        // clang-format off
            /*
             *      x
             *     / \
             *    A   y
             *       / \
             *      B   C
             *  –––––––––––––
             *      y
             *     / \
             *    x   C
             *   / \
             *  A   B
             * */
        // clang-format on
        // auto formerRight = internal->right;   // Store [y, B, C]
        // internal->right = formerRight->right; // [x, B, C], [y, A, B]
        // formerRight->right = internal;        // [y, A, [x, B, C]]
        // internal = formerRight;               // internal node is overwritten
        // internal->entry.second.balance = 0;
      }
      break;
    } break;
    case InsertionPosition::none:
      break;
    }
  }

  static InsertionPosition
  recursiveInsert(Key &&key, Value &&value,
                  InternalTreePtr<Key, InternalValue> &internal) {
    auto nextInsertionPosition = insertionPosition(key, internal);
    InsertionPosition result = InsertionPosition::none;
    switch (nextInsertionPosition) {
    case InsertionPosition::here:
      internal = new InternalTree<Key, InternalValue>(
          std::move(key),
          InternalValue{.value = std::move(value), .height = 1});
      return InsertionPosition::here;
    case InsertionPosition::left:
      result =
          recursiveInsert(std::move(key), std::move(value), internal->left);
      break;
    case InsertionPosition::right:
      result =
          recursiveInsert(std::move(key), std::move(value), internal->right);
      break;
    case InsertionPosition::none:
      return InsertionPosition::none;
    }
    if (result != InsertionPosition::none) {
      internal->entry.second.height++;
    }
    // Rebalance
    rebalance(internal, nextInsertionPosition, result);
    return nextInsertionPosition;
  }

  bool insert(Key &&key, Value &&value) {
    return recursiveInsert(std::move(key), std::move(value), root) !=
           InsertionPosition::none;
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

  static InsertionPosition
  insertionPosition(const Key &key,
                    InternalTreePtr<Key, InternalValue> position) {
    if (position == nullptr) {
      return InsertionPosition::here;
    }
    if (position->entry.first < key) {
      return InsertionPosition::left;
    } else if (position->entry.first > key) {
      return InsertionPosition::right;
    }
    return InsertionPosition::none;
  }
};
} // namespace datastructures
