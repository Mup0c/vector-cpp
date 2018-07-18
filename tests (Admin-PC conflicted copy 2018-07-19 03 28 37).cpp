#include "catch.h"
#include "my_vector.h"


template<class T, class U, class = typename T::iterator, class = typename U::iterator>
bool is_same(const T &a, const U &s) {
    if (s.size() != a.size()) {
        return false;
    }

    for (typename my::vector<T>::size_type i = 0; i < a.size(); i++) {
        if (a[i] != s[i]) {
            return false;
        }
    }
    return true;
}

class SomeClass {
public:
    SomeClass(char p) : p_(p) {}

    SomeClass() : p_('p') {}

    char getP() { return p_; }

    friend bool operator==(const SomeClass &lhs, const SomeClass &rhs) { return lhs.p_ == rhs.p_; }

    friend bool operator!=(const SomeClass &lhs, const SomeClass &rhs) { return !(lhs == rhs); }

private:
    char p_;
};


class NotIntegralType {
public:
    NotIntegralType() : a_(new int[100]), b_(new int[100]) {}

    NotIntegralType(NotIntegralType &&a) {
        a_ = a.a_;
        b_ = a.b_;
        a.a_ = nullptr;
        a.b_ = nullptr;
    }

    NotIntegralType(const NotIntegralType &other) : NotIntegralType() {
        std::memcpy(a_, other.a_, 100 * (sizeof(int)));
        std::memcpy(b_, other.b_, 100 * (sizeof(int)));
    }

    ~NotIntegralType() {
        delete[] a_;
        delete[] b_;
    }

    int *a_;
    int *b_;
};

TEST_CASE("Constructors") {

    SECTION("vector()") {
        my::vector<double> test_vector;
        CHECK(test_vector.capacity() == 0);
        CHECK(test_vector.size() == 0);
    }

    SECTION("vector(size_type n) and vector of vector") {
        my::vector<int> test_vector(10000);
        my::vector<my::vector<double>> test_2(10);
        CHECK(test_2.size() == 10);
        CHECK(test_2[9].size() == 0);
        CHECK(test_vector.capacity() == 10000);
        CHECK(test_vector.size() == 10000);
    }

    SECTION("vector(size_type 0)") {
        my::vector<bool> test_vector(0);
        CHECK(test_vector.capacity() == 0);
        CHECK(test_vector.size() == 0);
    }

    SECTION("vector(size_type n, T& val)") {
        my::vector<std::string> test_vector(500, "ababaca");
        std::vector<std::string> stdvec(500, "ababaca");
        CHECK(test_vector.capacity() == 500);
        CHECK(test_vector.size() == 500);
        CHECK(is_same(test_vector, stdvec));
    }

    SECTION("construct with iterators") {
        std::vector<int> std_vec = {0, 10, 132, 228};
        my::vector<int> test_vec(std_vec.begin(), std_vec.end());
        CHECK(test_vec.capacity() == std_vec.size());
        CHECK(is_same(test_vec, std_vec));
    }

    SECTION("copy constuctor") {
        my::vector<NotIntegralType> test_vector_a(10);
        test_vector_a[4].a_[4] = 127;

        my::vector<NotIntegralType> test_vector_b(test_vector_a);

        test_vector_a[4].a_[4] = 128;
        CHECK(test_vector_b[4].a_[4] == 127);
    }

    SECTION("move constructor") {
        my::vector<int> test_vector_b(10);
        test_vector_b[5] = 228;
        my::vector<int> test_vector_a(std::move(test_vector_b));
        CHECK(test_vector_a[5] == 228);
        CHECK(test_vector_b.empty());
    }

    SECTION("init list constructor") {
        my::vector<int> test_vector({0, 228, 1337, INT_MAX});
        CHECK(test_vector[0] == 0);
        CHECK(test_vector[1] == 228);
        CHECK(test_vector[2] == 1337);
        CHECK(test_vector[3] == INT_MAX);
        CHECK(test_vector.size() == 4);
        CHECK(test_vector.capacity() == 4);
    }
}

TEST_CASE("Access") {
    SECTION("non const []") {
        my::vector<int> test_vector(10, 1337);
        CHECK(test_vector[5] == 1337);
    }

    SECTION("const []") {
        const my::vector<int> test_vector(10, 1337);
        CHECK(test_vector[5] == 1337);
    }

    SECTION("non const at(pos)") {
        my::vector<int> test_vector(10);
        int val = test_vector.at(5);
        CHECK(val == 0);
        CHECK_THROWS_AS(test_vector.at(11), std::out_of_range);
        CHECK_THROWS_AS(test_vector.at(-1), std::out_of_range);
    }

    SECTION("const at(pos)") {
        const my::vector<int> test_vector(10);
        int val = test_vector.at(5);
        CHECK(val == 0);
        CHECK_THROWS_AS(test_vector.at(11), std::out_of_range);
        CHECK_THROWS_AS(test_vector.at(-1), std::out_of_range);
    }

    SECTION("front/back") {
        my::vector<int> test_vector = {9, 100, 1};
        CHECK(test_vector.front() == 9);
        CHECK(test_vector.front() == *test_vector.begin());
        CHECK(test_vector.back() == 1);
        CHECK(test_vector.back() == *(test_vector.end()-1));

    }

    SECTION("const front/back") {
        const my::vector<int> test_vector = {9, 100, 1};
        CHECK(test_vector.front() == 9);
        CHECK(test_vector.front() == *test_vector.begin());
        CHECK(test_vector.back() == 1);
        CHECK(test_vector.back() == *(test_vector.end() - 1));
    }
}

TEST_CASE("Modify") {

    SECTION("assign(n, elem)") {
        my::vector<int> test_vector(10, 8);
        std::vector<int> stdvec;
        stdvec.assign(11, 272);
        test_vector.assign(11, 272);
        CHECK(test_vector.capacity() == 11);
        CHECK(is_same(stdvec, test_vector));
    }

    SECTION("assign Iterators") {
        my::vector<int> test_vector(10, 8);
        std::vector<int> std_vector = {8, 9, 10, 11, 12, 78099090, -288};
        test_vector.assign(std_vector.begin(), std_vector.end());
        CHECK(is_same(test_vector, std_vector));

    }

    SECTION("assign initializer_list") {
        my::vector<int> test_vector(10, 8);
        test_vector.assign({-2235235, 123456});
        CHECK(test_vector[0] == -2235235);
        CHECK(test_vector[1] == 123456);
    }

    SECTION("reserve") {
        my::vector<NotIntegralType> test_vector(10);
        CHECK(test_vector.capacity() == 10);

        test_vector.reserve(100);
        CHECK(test_vector.capacity() == 100);

        test_vector.reserve(10);
        CHECK(test_vector.capacity() == 100);
    }

    SECTION("resize") {
        my::vector<int> test_vector = {10, 45};
        CHECK(test_vector.size() == 2);

        test_vector.resize(0);
        CHECK(test_vector.size() == 0);
        CHECK(test_vector.capacity() == 2);

        test_vector.resize(15, 98);
        CHECK(test_vector.size() == 15);
        CHECK(test_vector.capacity() > test_vector.size());

        for (auto &val : test_vector) {
            CHECK(val == 98);
        }
    }

    SECTION("shrink_to_fit") {
        my::vector<int> test_vector(11, 45);
        test_vector.reserve(500);
        test_vector.resize(0);
        CHECK(test_vector.size() == 0);
        CHECK(test_vector.capacity() == 500);
        test_vector.shrink_to_fit();
        CHECK(test_vector.size() == 0);
        CHECK(test_vector.capacity() == 0);
    }

    SECTION("push_back") {
        my::vector<int> test_vector;
        std::vector<int> stdvec;
        stdvec.push_back(5);
        stdvec.push_back(10);
        test_vector.push_back(5);
        test_vector.push_back(10);

        CHECK(test_vector.size() == 2);
        CHECK(test_vector.capacity() == 2);

        CHECK(test_vector[0] == 5);
        CHECK(test_vector[1] == 10);

        for (int i = 0; i < 10; i++) {
            test_vector.push_back(i);
            stdvec.push_back(i);
        }

        CHECK(test_vector.size() == 12);
        CHECK(test_vector.capacity() == 16);
        CHECK(is_same(stdvec, test_vector));
    }

    SECTION("emplace_back") {
        my::vector<SomeClass> test_vector;
        SomeClass smclss;
        test_vector.emplace_back('a');
        test_vector.emplace_back('b');
        test_vector.emplace_back('c');
        test_vector.emplace_back(smclss);

        CHECK(test_vector[0].getP() == 'a');
        CHECK(test_vector[1].getP() == 'b');
        CHECK(test_vector[2].getP() == 'c');
        CHECK(test_vector[3].getP() == 'p');
    }

    SECTION("pop_back") {
        my::vector<SomeClass> test_vector;

        test_vector.emplace_back('a');
        test_vector.emplace_back('b');
        test_vector.emplace_back('c');

        test_vector.pop_back();
        test_vector.pop_back();

        CHECK(test_vector.size() == 1);
        CHECK(test_vector.capacity() == 4);
        CHECK(test_vector.front().getP() == 'a');
    }

    SECTION("emplace") {
        my::vector<SomeClass> test_vector;

        test_vector.emplace_back('a');
        test_vector.emplace_back('b');
        test_vector.emplace_back('c');
        auto cit = static_cast<typename my::vector<SomeClass>::const_iterator>(test_vector.begin() + 2);
        test_vector.emplace(cit, 'z');
        auto it = test_vector.begin() + 2;
        test_vector.emplace(it, 'o');
        for (int i = 0; i < 3; i++) {
            auto it = test_vector.begin() + 2;
            test_vector.emplace(it, 'z');
        }
        CHECK(test_vector.size() == 8);
        CHECK(test_vector[0].getP() == 'a');
        CHECK(test_vector[1].getP() == 'b');
        CHECK(test_vector[2].getP() == 'z');
        CHECK(test_vector[3].getP() == 'z');
        CHECK(test_vector[4].getP() == 'z');
        CHECK(test_vector[5].getP() == 'o');
        CHECK(test_vector[6].getP() == 'z');
        CHECK(test_vector[7].getP() == 'c');
    }

    SECTION("insert(pos, cost T& elem)") {
        my::vector<SomeClass> test_vector;
        test_vector.push_back('k');
        test_vector.push_back('l');
        SomeClass tmp('p');
        test_vector.insert(test_vector.begin(), tmp);
        CHECK(test_vector.size() == 3);
        CHECK(test_vector[0].getP() == 'p');
        CHECK(test_vector[1].getP() == 'k');
        CHECK(test_vector[2].getP() == 'l');
    }

    SECTION("insert(pos, T&& elem)") {
        my::vector<SomeClass> test_vector;
        test_vector.push_back('k');
        test_vector.push_back('l');

        test_vector.insert(test_vector.end() - 2, SomeClass('p'));
        CHECK(test_vector.size() == 3);
        CHECK(test_vector[0].getP() == 'p');
        CHECK(test_vector[1].getP() == 'k');
        CHECK(test_vector[2].getP() == 'l');
    }

    SECTION("insert(pos, size, const& elem)") {
        my::vector<SomeClass> test_vector;
        test_vector.push_back('k');
        test_vector.push_back('l');
        test_vector.push_back('z');

        test_vector.insert(test_vector.end() - 2, 10, SomeClass('m'));
        CHECK(test_vector.size() == 13);
        CHECK(test_vector[0].getP() == 'k');

        for (my::vector<SomeClass>::size_type i = 1; i < test_vector.size() - 2; i++) {
            CHECK(test_vector[i].getP() == 'm');
        }

        CHECK(test_vector[test_vector.size() - 2].getP() == 'l');
        CHECK(test_vector.back().getP() == 'z');
    }

    SECTION("insert(iterator, iterator)") {
        my::vector<SomeClass> test_vector;
        std::vector<SomeClass> std_vector;
        std::vector<SomeClass> insert_vector = {'f', 'z', 's', 'q'};

        test_vector = {'k', 'l', 'm'};
        std_vector = {'k', 'l', 'm'};

        std_vector.insert(std_vector.begin() + 1, insert_vector.begin(), insert_vector.end());
        test_vector.insert(test_vector.begin() + 1, insert_vector.begin(), insert_vector.end());

        CHECK(is_same(test_vector, std_vector));
    }

    SECTION("insert initlist{}") {
        my::vector<SomeClass> test_vector;
        std::vector<SomeClass> std_vector;

        test_vector = {'k', 'l', 'm'};
        std_vector = {'k', 'l', 'm'};

        std_vector.insert(std_vector.begin() + 1, {'f', 'z', 's', 'q'});
        test_vector.insert(test_vector.begin() + 1, {'f', 'z', 's', 'q'});

        CHECK(is_same(test_vector, std_vector));
    }

    SECTION("erase(pos)") {
        my::vector<SomeClass> test_vector;
        std::vector<SomeClass> std_vector;

        test_vector = {'k', 'l', 'b', 'r', 'm', 'n'};
        std_vector = {'k', 'l', 'b', 'r', 'm', 'n'};

        auto a = test_vector.begin() + 2;
        auto b = std_vector.begin() + 2;

        for (int i = 0; i < 3; i++) {
            a = test_vector.erase(a);
            b = std_vector.erase(b);
            CHECK(*a == *b);
        }
        CHECK(is_same(test_vector, std_vector));
    }

    SECTION("erase(iterator)") {
        my::vector<SomeClass> test_vector;
        std::vector<SomeClass> std_vector;

        test_vector = {'k', 'l', 'b', 'r', 'm', 'n', 'j', 'v', 'l'};
        std_vector = {'k', 'l', 'b', 'r', 'm', 'n', 'j', 'v', 'l'};

        auto first1 = test_vector.begin() + 2;
        auto first2 = std_vector.begin() + 2;

        test_vector.erase(first1, first1 + 3);
        std_vector.erase(first2, first2 + 3);
        CHECK(is_same(test_vector, std_vector));

        auto last1 = test_vector.erase(first1, test_vector.end());
        auto last2 = std_vector.erase(first2, std_vector.end());

        CHECK(is_same(test_vector, std_vector));
        CHECK(last1 == test_vector.end());
    }

    SECTION("swap") {
        my::vector<SomeClass> test_vector_a = {'a', 'b', 'c'};
        my::vector<SomeClass> test_vector_b = {'z', 'x', 'y'};

        test_vector_a.swap(test_vector_b);

        CHECK(test_vector_a[0].getP() == 'z');
        CHECK(test_vector_a[1].getP() == 'x');
        CHECK(test_vector_a[2].getP() == 'y');

        CHECK(test_vector_b[0].getP() == 'a');
        CHECK(test_vector_b[1].getP() == 'b');
        CHECK(test_vector_b[2].getP() == 'c');

        std::vector<float> test_vector_c = {1.337, 22.8, 14.87, 555.0005};
        std::vector<float> test_vector_d = {0.0000001};
        auto a = test_vector_d;
        auto b = test_vector_c;

        test_vector_c.swap(test_vector_d);
        CHECK(a.capacity() == test_vector_c.capacity());
        CHECK(a == test_vector_c);
        CHECK(b.capacity() == test_vector_d.capacity());
        CHECK(b == test_vector_d);

    }
}

TEST_CASE("Operators") {
    SECTION("copy assignment = ") {
        my::vector<int> test_vector_a;
        my::vector<int> test_vector_b;
        test_vector_a.push_back(5);
        test_vector_a.push_back(10);

        test_vector_b = test_vector_a;

        CHECK(is_same(test_vector_a, test_vector_b));

        test_vector_a[1] = 500;
        CHECK_FALSE(is_same(test_vector_a, test_vector_b));

    }

    SECTION(" = initializer_list") {
        my::vector<std::string> test_vector = {"ababaca", "abc", "baobab"};
        test_vector = {"mem", "kek"};

        CHECK(test_vector.size() == 2);
        CHECK(test_vector[0] == "mem");
        CHECK(test_vector[1] == "kek");
    }

    SECTION("==") {
        my::vector<SomeClass> test_vector_a = {'a', 'b', 'c'};
        my::vector<SomeClass> test_vector_b = {'a', 'b', 'c'};

        CHECK(test_vector_a == test_vector_b);

        test_vector_a.pop_back();
        CHECK_FALSE(test_vector_a == test_vector_b);
        test_vector_b.pop_back();
        CHECK(test_vector_a == test_vector_b);

        test_vector_a.clear();
        test_vector_b.clear();
        CHECK(test_vector_a == test_vector_b);

        test_vector_a.emplace_back('z');
        test_vector_b.emplace_back('g');
        CHECK_FALSE(test_vector_a == test_vector_b);
    }

    SECTION("<") {
        my::vector<std::string> test_vector_a = {"abcd"};
        my::vector<std::string> test_vector_b = {"abcd"};

        CHECK_FALSE(test_vector_a < test_vector_b);

        test_vector_b = {"abc", "bd"};
        test_vector_a = {"a"};

        CHECK(test_vector_a < test_vector_b);
    }

    SECTION("<=") {
        my::vector<std::string> test_vector_a = {"abcd"};
        my::vector<std::string> test_vector_b = {"abcd"};

        CHECK(test_vector_a <= test_vector_b);

        test_vector_a = {"abc", "bd"};
        test_vector_b = {"abcd", "zdas"};

        CHECK(test_vector_a <= test_vector_b);
    }

    SECTION(">") {
        my::vector<std::string> test_vector_a = {"abcd"};
        my::vector<std::string> test_vector_b = {"abcd"};

        CHECK_FALSE(test_vector_a > test_vector_b);

        test_vector_a = {"abc", "bd"};
        test_vector_b = {"abcd", "zdas"};

        CHECK(test_vector_b > test_vector_a);
    }

    SECTION(">=") {
        my::vector<std::string> test_vector_a = {"abcd"};
        my::vector<std::string> test_vector_b = {"abcd"};

        CHECK(test_vector_a >= test_vector_b);

        test_vector_a = {"abc", "bd"};
        test_vector_b = {"abcd", "zdas"};

        CHECK(test_vector_b >= test_vector_a);
    }

    SECTION("!=") {
        my::vector<std::string> test_vector_a = {"abcd"};
        my::vector<std::string> test_vector_b = {"abcd"};

        CHECK_FALSE(test_vector_a != test_vector_b);

        test_vector_a = {"abc", "bd"};
        test_vector_b = {"abcd", "zdas"};

        CHECK(test_vector_a != test_vector_b);
    }
}

TEST_CASE("Iterators") {
    SECTION("Reverse non-const iterators") {
        my::vector<int> test_vector = {10, 12, 13, 229};

        my::vector<int>::size_type i = test_vector.size() - 1;
        for (auto it = test_vector.rbegin(); it != test_vector.rend(); it++) {
            CHECK(*it == test_vector[i--]);
        }
    }

    SECTION("Reverse const iterators") {
        const my::vector<int> test_vector = {10, 12, 13, 229};

        my::vector<int>::size_type i = test_vector.size() - 1;
        for (auto it = test_vector.rbegin(); it != test_vector.rend(); it++) {
            CHECK(*it == test_vector[i--]);
        }
    }

    SECTION("crbegin, crend") {


        my::vector<int> test_vector = {10, 12, 13, 229};

        my::vector<int>::size_type i = test_vector.size() - 1;

        for (auto it = test_vector.crbegin(); it != test_vector.crend(); it++) {
            CHECK(*it == test_vector[i--]);
        }
    }
}

TEST_CASE("Sort") {

    SECTION("std sort string") {
        std::vector<std::string> std_vector = {"drtfyghjkn", "pollkjfka", "1wefgjkjksdh", "0"};
        my::vector<std::string> my_vector = {"drtfyghjkn", "pollkjfka", "1wefgjkjksdh", "0"};
        std::sort(std_vector.begin(), std_vector.end());
        CHECK_FALSE(is_same(my_vector, std_vector));
        std::sort(my_vector.begin(), my_vector.end());
        CHECK(is_same(my_vector, std_vector));
    }

    SECTION("std sort double") {
        std::vector<double> std_vector = {1337.1337, 22.8, 14.87, 555.0005, 0, -1111, -22.8};
        my::vector<double> my_vector = {1337.1337, 22.8, 14.87, 555.0005, 0, -1111, -22.8};
        std::sort(std_vector.begin(), std_vector.end());
        CHECK_FALSE(is_same(my_vector, std_vector));
        std::sort(my_vector.begin(), my_vector.end());
        CHECK(is_same(my_vector, std_vector));
    }

}