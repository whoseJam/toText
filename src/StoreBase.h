#pragma once
#include <type_traits>
#include <functional>
#include <fstream>
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

namespace Store {
    template<typename T>
    T& get_ref() {
    }
    template<typename Base, typename T>
    T getter_trait(T(Base::*)()) {
    }
    template<typename Base, typename T>
    T getter_trait(T(Base::*)()const) {
    }

    template<typename T, typename Comp, typename V = void>
    struct type_func{
        type_func(std::function<void(void)>) {}
        void operator()() {}
    };
    template<typename T, typename Comp>
    struct type_func<T, Comp, std::enable_if_t<std::is_same_v<T, Comp>>> {
        std::function<void(void)> func;
        type_func(std::function<void(void)> func) { this->func = func; }
        void operator()() { func(); }
    };
    template<typename T, typename Comp, typename V = void>
    struct type_func_else {
        type_func_else(std::function<void(void)>) {}
        void operator()() {}
    };
    template<typename T, typename Comp>
    struct type_func_else<T, Comp, std::enable_if_t<!std::is_same_v<T, Comp>>> {
        std::function<void(void)> func;
        type_func_else(std::function<void(void)> func) { this->func = func; }
        void operator()() { func(); }
    };


    template<typename T>
    using get_origin_t = std::remove_pointer_t<std::remove_reference_t<T>>;

    template <typename T, typename V = void>
    struct is_basic_type : std::false_type {};

    template <>
    struct is_basic_type<int> : std::true_type {};
    template <>
    struct is_basic_type<unsigned int> : std::true_type {};
    template <>
    struct is_basic_type<float> : std::true_type {};
    template <>
    struct is_basic_type<double> : std::true_type {};
    template <>
    struct is_basic_type<long> : std::true_type {};
    template <>
    struct is_basic_type<long long> : std::true_type {};
    template <>
    struct is_basic_type<unsigned long long> : std::true_type {};
    template <>
    struct is_basic_type<long double> : std::true_type {};
    template<typename T>
    constexpr bool is_basic_type_v = is_basic_type<T>::value;

    template <typename T, typename V = void>
    struct has_fstream_in : std::false_type {};
    template <typename T>
    struct has_fstream_in<T, std::void_t<
        decltype(
            operator>>(
                get_ref<decltype(std::declval<std::ifstream>())>(),
                get_ref<decltype(std::declval<T>())>())
            )>> : std::true_type{};

    template <typename T>
    struct has_fstream_in<T, std::enable_if_t<is_basic_type_v<T>>> : std::true_type {};
    template<typename T>
    constexpr bool has_fstream_in_v = has_fstream_in<T>::value;

    template <typename T, typename V = void>
    struct has_fstream_out : std::false_type {};
    template <typename T>
    struct has_fstream_out<T, std::void_t<
        decltype(
            operator<<(
                get_ref<decltype(std::declval<std::ofstream>())>(),
                get_ref<decltype(std::declval<T>())>())
            )>> : std::true_type {};
    template <typename T>
    struct has_fstream_out<T, std::enable_if_t<is_basic_type_v<T>>> : std::true_type {};
    template<typename T>
    constexpr bool has_fstream_out_v = has_fstream_out<T>::value;

    template<typename T>
    constexpr bool has_fstream_v = has_fstream_in_v<T> && has_fstream_out_v<T>;

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

    void file2string(void* ptr);
    std::string string2file(void* str);
    Storable* getStorable(StoreID id);
    void addStorable(Storable* item);
    void addDependency(Storable* item);
    void afterDecode();
}