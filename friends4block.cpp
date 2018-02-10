//
// Created by blazeq on 2018. 2. 8..
//

#include "catch.hpp"
#include <iostream>
#include <list>

using namespace std;

namespace {

class Board {
    vector<vector<char>> M;
    int rows;
    int cols;

public:
    Board(const vector<string>& board) {
        rows = board.size();
        cols = 0;
        for (auto& row : board) {
            cols = max(cols, (int)row.length());
        }

        M = vector<vector<char>>(vector<vector<char>>(rows, vector<char>(cols)));

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < board[i].length(); ++j) {
                M[i][j] = board[i][j];
            }
        }
    }

    friend ostream& operator<<(ostream& o, const Board& board) {
        for (int i = 0; i < board.rows; ++i) {
            for (int j = 0; j < board.cols; ++j) {
                o << board.M[i][j] << " ";
            }
            o << endl;
        }
        for (int j = 0; j < board.cols; ++j) {
            o << "--";
        }
        o << endl;

        return o;
    }

    int evaluate() {
        int count = 0;
        int delta = 0;
        while ((delta = nextStep()) != 0) {
            count += delta;
        }

        return count;
    }

    int nextStep() {
        int removed = removeSquares();
        if (removed > 0) {
            align();
        }
        return removed;
    }

    int removeSquares() {
        list<pair<int, int>> positions; // left-top positions

        for (int i = 0; i < rows - 1; ++i) {
            for (int j = 0; j < cols - 1; ++j) {
                char blocks[4];
                blocks[0] = M[i][j];
                blocks[1] = M[i + 1][j];
                blocks[2] = M[i][j + 1];
                blocks[3] = M[i + 1][j + 1];

                bool equals = true;
                for (int k = 1; k < 4; ++k) {
                    if (blocks[k] == ' ') {
                        equals = false;
                        break;
                    }

                    if (blocks[0] != blocks[k]) {
                        equals = false;
                        break;
                    }
                }

                if (equals) {
                    positions.push_back(make_pair(i, j));
                }
            }
        }

        set<pair<int, int>> toRemoves;
        for (auto& p : positions) {
            int i = p.first;
            int j = p.second;
            M[i][j] = ' ';
            M[i + 1][j] = ' ';
            M[i][j + 1] = ' ';
            M[i + 1][j + 1] = ' ';

            toRemoves.insert(make_pair(i, j));
            toRemoves.insert(make_pair(i + 1, j));
            toRemoves.insert(make_pair(i, j + 1));
            toRemoves.insert(make_pair(i + 1, j + 1));
        }

        return toRemoves.size();
    }

    void align() {
        for (int j = 0; j < cols; ++j) {
            list<char> column;
            for (int i = rows - 1; i >= 0; --i) {
                if (M[i][j] != ' ') {
                    column.push_back(M[i][j]);
                }
            }

            int row = rows - 1;
            for (auto each : column) {
                M[row--][j] = each;
            }
            while (0 <= row) {
                M[row--][j] = ' ';
            }
        }
    }
};

int calcCountOfBlocksCleared(const vector<string>& strBoard) {
    Board board(strBoard);
    return board.evaluate();
}

} // namespace

TEST_CASE("Friends4Block", "[friends4block]") {
    REQUIRE(14 == calcCountOfBlocksCleared({"CCBDE", "AAADE", "AAABF", "CCBBF"}));
    REQUIRE(15 == calcCountOfBlocksCleared({"TTTANT", "RRFACC", "RRRFCC", "TRRRAA", "TTMMMF", "TMMTTJ"}));
}