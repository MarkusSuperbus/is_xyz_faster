#pragma once

#include <algorithm>
#include <functional>
#include <map>
#include <optional>

template <typename Key, typename Value> class StandardMapWrapper {
public:
  StandardMapWrapper() = default;
  StandardMapWrapper(const StandardMapWrapper &) = default;
  StandardMapWrapper(StandardMapWrapper &&) = default;
  StandardMapWrapper &operator=(const StandardMapWrapper &) = default;
  StandardMapWrapper &operator=(StandardMapWrapper &&) = default;

  bool insert(Key &&key, Value &&value) {
    auto result = map.insert(std::make_pair<Key, Value>(std::move(key), std::move(value)));
    return result.second;
  }

  std::optional<std::reference_wrapper<const Value>> find(Key &&key) const {
    const auto lookup = map.find(std::forward<Key>(key));
    if (lookup == map.cend()) [[unlikely]] {
      return {};
    }
    return {std::cref(lookup->second)};
  }

private:
  std::map<Key, Value> map;
};
