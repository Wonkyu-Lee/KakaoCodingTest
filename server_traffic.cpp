//
// Created by blazeq on 2018. 2. 10..
//

#include "catch.hpp"
#include <iostream>
#include <map>
#include <queue>

using namespace std;

namespace {

struct Range {
    long s;
    long e;
    Range(long s, long e):s(s),e(e){};

    friend ostream& operator<<(ostream& o, const Range& range) {
        o << "Range: [" << range.s << ", " << range.e << "], ";
        o << "Duration: [" << range.e - range.s << "]";
        return o;
    }

    bool intersectWith(const Range& range) const {
        return range.s <= e && s <= range.e;
    }
};

Range readRange(const string& log) {
    int h, m;
    float s, d;
    sscanf(log.c_str(), "2016-09-15 %2d:%2d:%f %fs", &h, &m, &s, &d);

    long end = (long)((h * 3600 + m * 60 + s) * 1000);
    long duration = (long)(d * 1000);
    long start = end - duration + 1;

    return Range(start, end);
}

int find(const vector<Range>& ranges, int s, int e, int x, bool lowerBound) {
    if (s > e) {
        return lowerBound ? e : s;
    }

    int m = (s + e) / 2;
    int v = ranges[m].s;
    if (x < v) {
        return find(ranges, s, m - 1, x, lowerBound);
    } else if (v < x) {
        return find(ranges, m + 1, e, x, lowerBound);
    } else {
        int x = v;

        if (lowerBound) {
            while (s <= m - 1 && ranges[m - 1].s == x) {
                --m;
            }
            return m;
        } else {
            while (m + 1 <= e && ranges[m + 1].s == x) {
                ++m;
            }
            return m;
        }
    }
}

int countIntersection(const vector<Range>& ranges, const Range& range) {
    int s = 0;
    int e = ranges.size() - 1;
    int lowerBound = max(s, find(ranges, s, e, range.e - 2999, true));
    int upperBound = min(e, find(ranges, s, e, range.e, false));

    int count = 0;
    for (int i = lowerBound; i <= upperBound; ++i) {
        if (ranges[i].intersectWith(range)) {
            count++;
        }
    }

    return count;
}

int calcMaximumThroughputPerSecond(const vector<string>& logs) {
    vector<Range> ranges;
    ranges.reserve(logs.size());
    for (auto& log : logs) {
        ranges.push_back(readRange(log));
    }
    sort(ranges.begin(), ranges.end(), [](const Range& r1, const Range& r2){ return r1.s < r2.s; });

    int maxCount = 0;
    for (auto each : ranges) {
        Range duringOneSec(each.s - 999, each.s);
        int count = countIntersection(ranges, duringOneSec);
        maxCount = max(maxCount, count);
    }
    return maxCount;
}

} // namespace

TEST_CASE("Server traffic", "[server traffic]") {
    SECTION("Test set 1") {
        vector<string> logs({
            "2016-09-15 20:59:57.421 0.351s",
            "2016-09-15 20:59:58.233 1.181s",
            "2016-09-15 20:59:58.299 0.8s",
            "2016-09-15 20:59:58.688 1.041s",
            "2016-09-15 20:59:59.591 1.412s",
            "2016-09-15 21:00:00.464 1.466s",
            "2016-09-15 21:00:00.741 1.581s",
            "2016-09-15 21:00:00.748 2.31s",
            "2016-09-15 21:00:00.966 0.381s",
            "2016-09-15 21:00:02.066 2.62s"});
        REQUIRE(7 == calcMaximumThroughputPerSecond(logs));
    }

    SECTION("Test set 2") {
        vector<string> logs({"2016-09-15 01:00:04.002 2.0s", "2016-09-15 01:00:07.000 2s" });
        REQUIRE(2 == calcMaximumThroughputPerSecond(logs));
    }

    SECTION("Test set 3") {
        vector<string> logs({"2016-09-15 01:00:04.001 2.0s", "2016-09-15 01:00:07.000 2s"});
        REQUIRE(1 == calcMaximumThroughputPerSecond(logs));
    }
}