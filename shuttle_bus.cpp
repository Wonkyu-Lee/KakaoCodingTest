//
// Created by blazeq on 2018. 2. 8..
//

#include "catch.hpp"
#include <iostream>

using namespace std;

namespace {

struct Time {
    int mm;

    Time(const string& text) {
        int hours;
        int minutes;
        stringstream ss(text);
        ss >> hours;
        char colon;
        ss >> colon;
        ss >> minutes;
        mm = 60 * hours + minutes;
    }

    bool operator<(const Time& other) const {
        return mm < other.mm;
    }

    bool operator==(const Time& other) const {
        return mm == other.mm;
    }

    bool operator<=(const Time& other) const {
        return mm <= other.mm;
    }

    void addMinutes(int minutes) {
        mm += minutes;
    }

    string toString() const {
        if (mm < 0) {
            return "Impossible";
        }

        int hours = mm / 60;
        int minutes = mm % 60;
        char buffer[8];
        sprintf(buffer, "%02d:%02d", hours, minutes);
        return buffer;
    }

    bool isImpossible() const {
        return mm < 0;
    }
};

Time calcLatestTime(int n, int t, int m, const vector<string>& strTimeTable) {
    // 마지막 버스에 타는 마지막 사람보다 빠르면 됨.


    vector<Time> timeTable;
    for (auto& strTime : strTimeTable) {
        timeTable.push_back(Time(strTime));
    }
    sort(timeTable.begin(), timeTable.end());

    Time departure("09:00");
    departure.addMinutes(-t);

    int crew = 0;
    int lastCrew = -1;
    int remainCount = 0;
    for (int i = 0; i < n; ++i) {
        departure.addMinutes(t);
        int capacity = m;
        int count = 0;
        remainCount = m;

        while (count < capacity && crew < timeTable.size()) {
            const Time& crewTime = timeTable[crew];
            if (crewTime <= departure) {
                lastCrew = crew;
                ++count;
                ++crew;
                --remainCount;
            } else {
                break;
            }
        }
    }

    Time lastTime("00:00");

    if (remainCount == 0) {
        lastTime = timeTable[lastCrew];
        lastTime.addMinutes(-1);
        return lastTime;
    } else {
        lastTime = departure;
        return lastTime;
    }
}

} // namespace

TEST_CASE("Shuttle bus", "[shuttle bus]") {
    REQUIRE(Time("09:00").mm == calcLatestTime(1, 1, 5, {"08:00", "08:01", "08:02", "08:03"}).mm);
    REQUIRE(Time("09:09").mm == calcLatestTime(2, 10, 2, {"09:10", "09:09", "08:00"}).mm);
    REQUIRE(Time("08:59").mm == calcLatestTime(2, 1, 2, {"09:00", "09:00", "09:00", "09:00"}).mm);
    REQUIRE(Time("00:00").mm == calcLatestTime(1, 1, 5, {"00:01", "00:01", "00:01", "00:01", "00:01"}).mm);
    REQUIRE(Time("09:00").mm == calcLatestTime(1, 1, 1, {"23:59"}).mm);
    REQUIRE(Time("18:00").mm == calcLatestTime(10, 60, 45, {"23:59","23:59", "23:59", "23:59", "23:59", "23:59", "23:59", "23:59", "23:59", "23:59", "23:59", "23:59", "23:59", "23:59", "23:59", "23:59"}).mm);
    REQUIRE(calcLatestTime(1, 1, 5, {"00:00", "00:00", "00:00", "00:00", "00:00"}).isImpossible());
}