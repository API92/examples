#include <bits/stdc++.h>
using namespace std;

// ---------- Utils. Don't see. -----------

template<typename T>
using normalize = remove_cv_t<remove_reference_t<T>>;

template<typename E>
struct any_ex {
    size_t dummy;
    template<typename D,
        typename = enable_if_t<!is_same_v<normalize<D>, E>>> // C++ 17 is_same_v
    operator D () const;
};

template<typename T, size_t... n,
    typename = decltype(T {any_ex<T>{n}...})>
constexpr bool has_n_fields(index_sequence<n...>) { return true; }

template<typename T, typename U>
constexpr bool has_n_fields(U) { return false; }

template<size_t x>
using size_t_ = integral_constant<size_t, x>;

template<typename T, typename F>
auto unpack_impl(T &&, F &&f, size_t_<0>)
{
    return forward<F>(f)();
}

template<typename T, typename F>
auto unpack_impl(T &&x, F &&f, size_t_<1>)
{
    auto && [m1] = forward<T>(x);
    return forward<F>(f)(m1);
}

template<typename T, typename F>
auto unpack_impl(T &&x, F &&f, size_t_<2>)
{
    auto && [m1, m2] = forward<T>(x); // C++ 17 structured binding declaration
    return forward<F>(f)(m1, m2);
}

template<typename T, typename F>
auto unpack_impl(T &&x, F &&f, size_t_<3>)
{
    auto && [m1, m2, m3] = forward<T>(x);
    return forward<F>(f)(m1, m2, m3);
}

template<typename T, typename F>
auto unpack_impl(T &&x, F &&f, size_t_<4>)
{
    auto && [m1, m2, m3, m4] = forward<T>(x);
    return forward<F>(f)(m1, m2, m3, m4);
}

template<typename T, typename F, size_t n = 0>
auto unpack(T &&x, F &&f)
{
    static_assert(n <= sizeof(T) * 8); // C++ 17 static_assert without message
    if constexpr (!has_n_fields<normalize<T>>(make_index_sequence<n + 1>())) // C++ 17 constexpr if
        return unpack_impl(forward<T>(x), forward<F>(f), size_t_<n>());
    else
        return unpack<T, F, n + 1>(forward<T>(x), forward<F>(f));
}

struct struct_hash {
    template<typename T>
    size_t operator () (T const &x) const
    {
        return unpack(x, [](auto &&...a) {
            size_t res = 0;
            for (size_t h : {hash<normalize<decltype(a)>>()(a)...})
                res ^= h + 0x9e3779b9 + (res << 6) + (res >> 2);
            return res;
        });
    }
};

struct struct_compare {
    template<typename T>
    bool operator () (T const &lhs, T const &rhs) const
    {
        return unpack(lhs, [&rhs](auto &&... al) {
            return unpack(rhs, [&al...](auto &&... ar) {
                return ((al == ar) && ...); // C++ 17 folding expression
            });
        });
    }
};

struct tuple_hash {
    template<typename... T, size_t... n>
    static size_t do_hash(tuple<T...> const &t, index_sequence<n...>)
    {
        size_t res = 0;
        for (size_t h : {hash<T>()(get<n>(t))...})
            res ^= h + 0x9e3779b9 + (res << 6) + (res >> 2);
        return res;
    }

    template<typename T>
    size_t operator () (T &t) const
    {
        return do_hash(t, make_index_sequence<tuple_size_v<T>>()); // C++ 17 tuple_size_v
    }
};

// ------------ Utils end. ----------------
//

int main()
{
    {
    struct S {
        string_view foo; // C++ 17 string_view
        string bar;
        size_t baz;
    };

    unordered_map<S, string, struct_hash, struct_compare> m;
    m[{.foo = "foo", .bar = "bar", .baz = 10}] = "value1";
    m[{.foo = "bar", .bar = "foo", .baz = 10}] = "value2";
    for (auto & [k, v] : m)
        cout << k.foo << " " << k.bar << " " << k.baz << ": " << v << endl;
    auto it = m.find({.foo = "foo", .bar = "bar", .baz = 10});
    if (it != m.end())
        cout << "founded " << it->second << endl;
    }

    {
    // Old style tuples
    unordered_map<tuple<string, string, size_t>, string, tuple_hash> m;
    m[{"foo", "bar", 10}] = "value1";
    m[{"bar", "foo", 10}] = "value2";
    for (auto &kv : m)
        cout << get<0>(kv.first) << " " << get<1>(kv.first) << " "
            << get<2>(kv.first) << ": " << kv.second << endl;
    auto it = m.find({"foo", "bar", 10});
    if (it != m.end())
        cout << "founded " << it->second << endl;
    }

    return 0;
}
