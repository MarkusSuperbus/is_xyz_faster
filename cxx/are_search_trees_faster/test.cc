#include "SearchTree.h"

using datastructures::SearchTree;

void releaseAssert(bool b) {
  if (!b) {
    throw 1u;
  }
}

int main() {
  SearchTree<int, int> tree;
  tree.insert(2, 2);
  tree.insert(1, 3);
  tree.insert(3, 1);
  releaseAssert(tree.find(1) == 3);
  releaseAssert(tree.find(2) == 2);
  releaseAssert(tree.find(3) == 1);
  releaseAssert(tree.find(42) == std::nullopt);
  return 0;
}
