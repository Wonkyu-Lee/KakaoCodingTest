//
// Created by blazeq on 2018. 2. 8..
//

#include "catch.hpp"
#include <iostream>
using namespace std;

namespace {

bool readScore(const string& dartResult, int& i, int& score) {
    stringstream ss;
    while (i < dartResult.length()) {
        char c = dartResult[i];
        if (isdigit(c)) {
            ss << c;
            ++i;
        } else {
            break;
        }
    }

    string s = ss.str();
    if (s.empty()) {
        return false;
    } else {
        score = stoi(ss.str());
        return true;
    }
}

int readBonus(const string& dartResult, int& i, int& bonus) {
    char c = dartResult[i];
    if (c == 'S' || c == 'D' || c == 'T') {
        switch (c) {
            case 'S': bonus = 1; break;
            case 'D': bonus = 2; break;
            case 'T': bonus = 3; break;
        }

        ++i;
        return true;
    }

    return false;
}

bool readOption(const string& dartResult, int& i, char& option) {
    char c = dartResult[i];
    if (c == '*' || c == '#') {
        option = c;
        ++i;
        return true;
    }

    return false;
}

int calcTotalScore(const string& dartResult) {
    int index = 0;
    int scores[3];
    for (int i = 0; i < 3; ++i) {
        int score;
        int bonus;
        char option;

        if (!readScore(dartResult, index, score)) {
            return -1;
        }

        if (!readBonus(dartResult, index, bonus)) {
            return -1;
        }

        scores[i] = pow(score, bonus);

        // apply option
        if (readOption(dartResult, index, option)) {
            switch (option) {
                case '*':
                    scores[i] *= 2;
                    if (i - 1 >= 0) {
                        scores[i - 1] *= 2;
                    }
                    break;
                case '#':
                    scores[i] *= -1;
                    break;
            }
        }
    }

    int totalScore = 0;
    for (auto score : scores) {
        totalScore += score;
    }

    return totalScore;
}

} // namespace

TEST_CASE("Dart game", "[dart game]") {
    REQUIRE(37 == calcTotalScore("1S2D*3T"));
    REQUIRE( 9 == calcTotalScore("1D2S#10S"));
    REQUIRE( 3 == calcTotalScore("1D2S0T"));
    REQUIRE(23 == calcTotalScore("1S*2T*3S"));
    REQUIRE( 5 == calcTotalScore("1D#2S*3S"));
    REQUIRE(-4 == calcTotalScore("1T2D3D#"));
    REQUIRE(59 == calcTotalScore("1D2S3T*"));
}