//
// Created by blazeq on 2018. 2. 8..
//

#include "catch.hpp"
#include <iostream>
using namespace std;

namespace {

void printMap(int n, int arr1[], int arr2[]) {
    int map[n];
    for (int i = 0; i < n; ++i) {
        map[i] = arr1[i] | arr2[i];
    }

    for (int i = 0; i < n; ++i) {
        int mask = (1 << (n - 1));
        for (int j = 0; j < n; ++j) {
            bool wall = (map[i] & (mask >> j)) != 0;
            cout << (wall ? '#' : ' ');
        }
        cout << endl;
    }
}

} // namespace

TEST_CASE("Secret map", "[secret map]") {
    int n = 5;
    int arr1[] = {9, 20, 28, 18, 11};
    int arr2[] = {30, 1, 21, 17, 28};

    printMap(n, arr1, arr2);
}