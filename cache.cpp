//
// Created by blazeq on 2018. 2. 8..
//

#include "catch.hpp"
#include <iostream>
#include <deque>
#include <list>

using namespace std;

namespace {

class Cache {
    list<string> cities;
    int size;

public:
    Cache(int size): size(size) {}

    bool update(const string& city) {
        auto found = find(cities.begin(), cities.end(), city);
        if (found != cities.end()) {
            cities.erase(found);
            cities.push_front(city);
            return true;
        } else {
            if (cities.size() > size) {
                cities.pop_back();
            }

            cities.push_front(city);
            return false;
        }
    }
};

int calcRunningTime(int cacheSize, const vector<string>& cities) {
    Cache cache(cacheSize);

    int runningTime = 0;
    for (auto& city : cities) {
        if (cache.update(city)) {
            runningTime += 1;
        } else {
            runningTime += 5;
        }
    }

    return runningTime;
}

} // namespace

TEST_CASE("Cache", "[cache]") {
    REQUIRE(50 == calcRunningTime(3, {"Jeju", "Pangyo", "Seoul", "NewYork", "LA", "Jeju", "Pangyo", "Seoul", "NewYork", "LA"}));
    REQUIRE(21 == calcRunningTime(3, {"Jeju", "Pangyo", "Seoul", "Jeju", "Pangyo", "Seoul", "Jeju", "Pangyo", "Seoul"}));
    REQUIRE(25 == calcRunningTime(0, {"Jeju", "Pangyo", "Seoul", "NewYork", "LA"}));
}