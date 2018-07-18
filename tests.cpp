#include "catch.h"
#include "my_vector.h"

template <class T, class U, class = typename T::iterator, class = typename U::iterator>
bool is_same(const T& a, const U& s)
{
    if (s.size() != a.size()) { return false; }
    for (typename my::vector<T>::size_type i = 0; i < a.size(); i++) {
        if (a[i] != s[i]) { return false; }
    }
    return true;
}

TEST_CASE("Sort")
{
    std::vector<std::string> std_vector = {"prive", "poka", "kak dela", "mmmm"};
    my::vector<std::string> atl_vector = {"prive", "poka", "kak dela", "mmmm"};
    std::sort(std_vector.begin(), std_vector.end());
    std::sort(atl_vector.begin(), atl_vector.end());
    REQUIRE(is_same(atl_vector, std_vector));
}