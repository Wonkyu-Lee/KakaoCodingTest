//
// Created by blazeq on 2018. 2. 8..
//

#include "catch.hpp"
#include <iostream>
#include <unordered_map>
using namespace std;

namespace {

template <typename T>
class Set {
    unordered_map<T, int> frequency;
public:
    Set() = default;

    Set(const vector<T>& elements) {
        for (auto each : elements) {
            frequency[each]++;
        }
    }

    void add(const T& element) {
        frequency[element]++;
    }

    static double jaccardSimilarity(const Set& s1, const Set& s2) {
        Set setUnion = s2;
        Set setIntersection;

        for (auto e1 : s1.frequency) {
            auto e2 = s2.frequency.find(e1.first);
            if (e2 != s2.frequency.end()) {
                setIntersection.frequency[e1.first] = min(e1.second, e2->second);
                setUnion.frequency[e1.first] = max(e1.second, e2->second);
            } else {
                setUnion.frequency[e1.first] = e1.second;
            }
        }

        int x = setIntersection.size();
        int y = setUnion.size();

        return (y == 0) ? 1 : (x/(double)y);
    }

    int size() const {
        int count = 0;
        for (auto e : frequency) {
            count += e.second;
        }
        return count;
    }
};

Set<string> buildSet(const string& str) {
    if (str.length() < 2)
        return Set<string>();

    string lower = str;
    transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

    Set<string> s;
    for (int i = 0; i < lower.length() -1; ++i) {
        string element = lower.substr(i, 2);
        if (isalpha(element[0]) && isalpha(element[1])) {
            s.add(element);
        }
    }

    return s;
}

int calcSimilarity(const string& str1, const string& str2) {
    Set<string> s1 = buildSet(str1);
    Set<string> s2 = buildSet(str2);

    return static_cast<int>(Set<string>::jaccardSimilarity(s1, s2) * 65536);
}

} // namespace

TEST_CASE("News clustering", "[news clustering]") {
    SECTION("Test set") {
        Set<int> s1({1, 1, 2, 2, 3});
        Set<int> s2({1, 2, 2, 4, 5});
        cout << Set<int>::jaccardSimilarity(s1, s2) << endl;
    }

    SECTION("Solve") {
        REQUIRE(16384 == calcSimilarity("FRANCE", "french"));
        REQUIRE(65536 == calcSimilarity("handshake", "shake hands"));
        REQUIRE(43690 == calcSimilarity("aa1+aa2", "AAAA12"));
        REQUIRE(65536 == calcSimilarity("E=M*C^2", "e=m*c^2"));
    }
}