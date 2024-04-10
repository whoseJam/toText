#pragma once
#include <type_traits>
#include <functional>
#include <iostream>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <stack>
#include <queue>
#include <unordered_set>
#include <unordered_map>

struct Storable;

using StoreID = unsigned int;

namespace ttx {


template<typename Base, typename T>
T getter_trait(T(Base::*)()) {
}
template<typename Base, typename T>
T getter_trait(T(Base::*)()const) {
}

// 用来移除类型T上的所有引用和指针
template<typename T>
using get_origin_t = std::remove_pointer_t<std::remove_reference_t<T>>;


// 用来推导是否存在 << 和 >> 输入输出方法
template<typename T>
T& to_lvalue(T&& rvalue) {
    return rvalue;
}
template<typename T>
using input_exist_t = decltype(
    std::declval<std::istream>() >> to_lvalue<T>(std::declval<T>())
);
template<typename T>
using output_exist_t = decltype(
    std::declval<std::ostream>() << std::declval<T>()
);
template<typename T>
using output_exist_czz_t = decltype(std::declval<std::ostream>() << std::declval<T>());

template <typename T, typename V = void>
struct has_stream_out_t: public std::false_type {
    static void call() {
        std::cout << "has not stream out\n";
    }
};
template <typename T>
struct has_stream_out_t<T, std::void_t<output_exist_czz_t<T>>> : public std::true_type {
    static void call() {
        std::cout << "has stream out\n";
    }
};
template<typename T, typename V = void>
struct has_stream_in_t : public std::false_type {};
template<typename T>
struct has_stream_in_t<T, std::void_t<input_exist_t<T>>> : public std::true_type {};
template<typename T>
constexpr bool has_stream_out_v = has_stream_out_t<T>::value;
template<typename T>
constexpr bool has_stream_in_v = has_stream_in_t<T>::value;
template<typename T>
constexpr bool has_stream_v = has_stream_in_v<T> && has_stream_out_v<T>;


template<typename T>
struct is_container : std::false_type {
};
template<typename T, typename ...Types>
struct is_container<std::vector<T, Types...>> : std::true_type {
    using value_type = T;
};
template<typename T, typename ...Types>
struct is_container<std::list<T, Types...>> : std::true_type {
    using value_type = T;
};
template<typename T, typename ...Types>
struct is_container<std::stack<T, Types...>> : std::true_type {
    using value_type = T;
};
template<typename T, typename ...Types>
struct is_container<std::queue<T, Types...>> : std::true_type {
    using value_type = T;
};
template<typename T, typename ...Types>
struct is_container<std::deque<T, Types...>> : std::true_type {
    using value_type = T;
};
template<typename T, typename ...Types>
struct is_container<std::set<T, Types...>> : std::true_type {
    using value_type = T;
};
template<typename T, typename ...Types>
struct is_container<std::multiset<T, Types...>> : std::true_type {
    using value_type = T;
};
template<typename ...Types>
constexpr bool is_container_v = is_container<Types...>::value;

template<typename T, typename V = void>
struct has_push_back : std::false_type {
};
template<typename T>
struct has_push_back<T, std::void_t<
    decltype(std::declval<T>().push_back(std::declval<typename T::value_type>()))
    >> : std::true_type {
};
template<typename T>
constexpr bool has_push_back_v = has_push_back<T>::value;

}