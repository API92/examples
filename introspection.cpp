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


template<typename char_t, char_t... c>
struct comp_str
{
    static constexpr char_t c_str[] = {c..., 0};
};


template<typename char_t, char_t... c> // non-standard extension
constexpr auto operator "" _s () { return comp_str<char_t, c...>::c_str; }


template<typename T>
struct field_primitive_base {
    template<typename... Args>
    field_primitive_base(Args &&... args) : value(forward<Args>(args)...) {}

    operator T & () { return value; }
    operator T const & () const { return value; }

    T value;
};


template<auto name_, typename T, auto flags_ = 0>
struct field : conditional_t<is_fundamental_v<T>, field_primitive_base<T>, T>
{
    typedef conditional_t<is_fundamental_v<T>, field_primitive_base<T>, T> base;

    using base::base;

    typedef T type;
    static constexpr auto name = name_;
    static constexpr auto flags = flags_;
};

// ------------ Utils end. ----------------
//


constexpr int PK = 1;

void init(int &id, string &surname, double &weight)
{
    id = 1;
    surname = "hello";
    weight = 42;
}

struct printer {
    void operator () () {}

    template<typename T, typename... Tail>
    void operator () (T &field, Tail &... tail)
    {
        if constexpr (field.flags & PK)
            cout << "PK ";
        cout << field.name << " = " << field << endl;
        (*this)(tail...);
    }
};

int main()
{
    struct Foo {
        field<"id"_s, int, PK> id;
        field<"surname"_s, string> surname;
        field<"weight"_s, double> weight;
    };

    Foo f;

    unpack(f, init);
    unpack(f, printer());

    f.surname += " world";
    f.weight += 146;
    unpack(f, printer());

    return 0;
}
