#include "AvlTree.h"
#include "ISearchTree.h"
#include "SearchTree.h"
#include "SortedAssociationVector.h"
#include "StandardMapWrapper.h"
#include <cstdint>
#include <gtest/gtest.h>
#include <optional>

using datastructures::AvlTree;
using datastructures::SearchTree;
using datastructures::SortedAssociationVector;

template <ISearchTree<int64_t, int64_t> ImplT> void test() {
  ImplT tree;
  tree.insert(2, 2);
  tree.insert(1, 3);
  tree.insert(3, 1);
  ASSERT_EQ(tree.find(1), 3);
  ASSERT_EQ(tree.find(2), 2);
  ASSERT_EQ(tree.find(3), 1);
  ASSERT_EQ(tree.find(42), std::nullopt);
}

TEST(SortedAssociationVectorTest, InsertionAndLookup) {
  test<SortedAssociationVector<int64_t, int64_t>>();
}

TEST(SortedAssociationVectorTest, InsertionAndLookup2) {
  SortedAssociationVector<int64_t, int64_t> tree;
  tree.insert(0, 0);
  tree.insert(1, 1);
  tree.insert(2, 2);
  ASSERT_EQ(tree.find(0), 0);
  ASSERT_EQ(tree.find(1), 1);
  ASSERT_EQ(tree.find(2), 2);
}

TEST(SearchTreeTest, InsertionAndLookup) {
  test<SearchTree<int64_t, int64_t>>();
}

TEST(StandardMapWrapper, InsertionAndLookup) {
  test<StandardMapWrapper<int64_t, int64_t>>();
}

TEST(AvlTreeTest, InsertionAndLookup) { test<AvlTree<int64_t, int64_t>>(); }
