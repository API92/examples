#include <iostream>

struct Tag {};

constexpr auto Flag(Tag);

template<typename T>
struct SetFlag {
    friend constexpr auto Flag(T) { return true; }
};

constexpr bool IsSet(...) { return false; }

template<typename T>
constexpr decltype(Flag(T {})) IsSet(T a) { return true; }


int main()
{
    constexpr bool x = IsSet(Tag {});
    static_assert(!x);
    std::cout << x << std::endl;

    constexpr SetFlag<Tag> s;

    constexpr bool y = IsSet(Tag {});
    static_assert(y);
    std::cout << y << std::endl;

    return 0;
}
