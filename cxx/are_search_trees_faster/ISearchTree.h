#include <concepts>
#include <functional>
#include <optional>

template <typename Structure, typename Key, typename Value>
concept ISearchTree =
    requires(Structure structure, const Structure constStructure, Key &&key, Value &&value) {
      { structure.insert(std::move(key), std::move(value)) } -> std::convertible_to<bool>;
      {
        constStructure.find(std::move(key))
      } -> std::convertible_to<
          std::optional<std::reference_wrapper<const Value>>>;
    } and std::copy_constructible<Structure> and
    std::move_constructible<Structure> and std::copy_constructible<Key> and
    std::move_constructible<Key> and std::copy_constructible<Value> and
    std::move_constructible<Value> and std::default_initializable<Structure> and
    std::default_initializable<Structure>;
