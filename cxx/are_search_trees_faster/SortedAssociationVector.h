#include <algorithm>
#include <functional>
#include <optional>
#include <vector>

namespace datastructures {
template <typename Key, typename Value> class SortedAssociationVector {
public:
  SortedAssociationVector() = default;
  SortedAssociationVector(const SortedAssociationVector &) = default;
  SortedAssociationVector(SortedAssociationVector &&) = default;
  SortedAssociationVector &operator=(const SortedAssociationVector &) = default;
  SortedAssociationVector &operator=(SortedAssociationVector &&) = default;

  bool insert(Key &&key, Value &&value) {
    const auto lowerBound = findIterator(key);
    if (isKey(lowerBound, key)) [[unlikely]] {
      return false;
    }
    mVector.push_back(
        std::make_pair<Key, Value>(std::move(key), std::move(value)));
    std::ranges::sort(mVector.begin(), mVector.end());
    return true;
  }

  std::optional<std::reference_wrapper<const Value>> find(Key &&key) const {
    const auto lowerBound = findIterator(key);
    if (!isKey(lowerBound, key)) [[unlikely]] {
      return {};
    }
    return {std::cref(lowerBound->second)};
  }

private:
  std::vector<std::pair<Key, Value>> mVector;

  template <typename Iterator>
  bool isKey(Iterator iterator, const Key &key) const {
    return iterator != mVector.end() && key == iterator->first;
  }

  auto findIterator(const Key &key) const {
    constexpr auto projection = [](const std::pair<Key, Value> &a) -> Key {
      return a.first;
    };
    auto lowerBound = std::ranges::lower_bound(
        mVector.begin(), mVector.end(), key, std::less<Key>(), projection);
    return lowerBound;
  }
};
} // namespace datastructures
