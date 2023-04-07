#include <iostream>

template<int n>
struct Tag;

template<int n>
struct SetFlag {
    friend constexpr auto Flag(Tag<n> x) { return x; }
};

template<int n, typename TagT, typename Tail>
struct Base : Tail {
    constexpr operator int () const { return n; }

    friend constexpr auto Flag(TagT);

    template<typename T = TagT>
    friend constexpr decltype(Flag(T {}))
    Last(TagT x = {}, T = {}) { return x; }

    template<int s = n + 1>
    constexpr static Tag<s> Increment(SetFlag<s> = {}) { return {}; }
};

template<>
struct Tag<0> : Base<0, Tag<0>, SetFlag<0>> {};

template<int n>
struct Tag : Base<n, Tag<n>, Tag<n - 1>> {};

using Limit = Tag<100>;


int main()
{
    constexpr int last0 = Last(Limit {});
    static_assert(last0 == 0);

    constexpr auto inc1 = Last(Limit {}).Increment();
    constexpr int last1 = Last(Limit {});
    static_assert(last1 == 1);

    constexpr auto inc2 = Last(Limit {}).Increment();
    constexpr int last2 = Last(Limit {});
    static_assert(last2 == 2);

    constexpr auto inc3 = Last(Limit {}).Increment();
    constexpr int last3 = Last(Limit {});
    static_assert(last3 == 3);

    std::cout
        << last0 << " "
        << last1 << " "
        << last2 << " "
        << last3 << std::endl;

    return 0;
}
