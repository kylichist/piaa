#include <iostream>
#include <unordered_set>
#include <vector>

#define MIN_N 2
#define MAX_N 40

using namespace std;

class Grid {
private:
    vector<vector<int>> grid;
    vector<vector<int>> bestPartition;
    vector<vector<int>> currentPartition;
    int bestCount;

    void fill(int x, int y, int size, int value) {
        for (int i = x; i < x + size; ++i) {
            for (int j = y; j < y + size; ++j) {
                grid[i][j] = value;
            }
        }
    }

public:
    Grid(int n) {
        grid = vector<vector<int>>(n, vector<int>(n, 0));
        bestCount = n * n;
    }

    int getSize() { return grid.size(); }

    int getCurrentCount() { return currentPartition.size(); }

    int getBestCount() { return bestCount; }

    vector<vector<int>>& getBestPartition() { return bestPartition; }

    void place(int x, int y, int size) {
        fill(x, y, size, currentPartition.size() + 1);
        currentPartition.push_back({x, y, size});
    }

    void revert() {
        if (!currentPartition.empty()) {
            auto lastSquare = currentPartition.back();
            fill(lastSquare[0], lastSquare[1], lastSquare[2], 0);
            currentPartition.pop_back();
        }
    }

    void saveAsBest() {
        print();
        cout << endl;

        bestPartition = currentPartition;
        bestCount = currentPartition.size();
    }

    bool canPlace(int x, int y, int size) {
        if (x + size > grid.size() || y + size > grid.size()) {
            return false;
        }
        for (int i = x; i < x + size; ++i) {
            for (int j = y; j < y + size; ++j) {
                if (grid[i][j] != 0) {
                    return false;
                }
            }
        }
        return true;
    }

    pair<int, int> firstNotFilled() {
        for (int i = 0; i < grid.size(); ++i) {
            for (int j = 0; j < grid[i].size(); ++j) {
                if (grid[i][j] == 0) {
                    return {i, j};
                }
            }
        }
        return {-1, -1};
    }

    bool isFilled() { return firstNotFilled() == make_pair(-1, -1); }

    void print() {
        for (auto& row : grid) {
            for (int cell : row) {
                cout << cell << " ";
            }
            cout << endl;
        }
    }
};

void backtrack(Grid& grid) {
    if (grid.getCurrentCount() >= grid.getBestCount()) {
        return;
    }

    int N = grid.getSize();

    pair<int, int> firstNotFilled = grid.firstNotFilled();
    int x = firstNotFilled.first, y = firstNotFilled.second;

    // Если свободных клеток нет - решение найдено
    if (x == -1) {
        grid.saveAsBest();
        return;
    }

    // Пробуем поставить квадраты разных размеров
    for (int size = min(min(N - x, N - y), N - 1); size >= 1; --size) {
        if (grid.canPlace(x, y, size)) {
            grid.place(x, y, size);
            backtrack(grid);
            grid.revert();
        }
    }
}

int minfactor(int n) {
    if (n % 2 == 0) {
        return 1;
    }
    for (int i = 3; i * i <= n; ++i) {
        if (n % i == 0) {
            return i;
        }
    }
    return 1;
}

vector<vector<int>> solve(int n) {
    Grid grid = Grid(n);

    // Оптимизации
    if (n % 2 == 0) {  // N - четное
        int size = n / 2;
        grid.place(0, 0, size);
        grid.place(size, 0, size);
        grid.place(0, size, size);
        grid.place(size, size, size);
        grid.saveAsBest();
    }
    int mf = minfactor(n);
    if (mf == 1) {  // N - простое
        int lsize = (n + 1) / 2, ssize = (n - 1) / 2;
        grid.place(0, 0, lsize);
        grid.place(lsize, 0, ssize);
        grid.place(0, lsize, ssize);
    } else {  // N - нечетное составное
        int lscale = mf / 2 + 1, sscale = mf - lscale;
        int lsize = lscale * n / mf, ssize = sscale * n / mf;
        grid.place(0, 0, lsize);
        grid.place(lsize, 0, ssize);
        grid.place(0, lsize, ssize);
    }

    if (grid.isFilled()) {
        return grid.getBestPartition();
    }

    backtrack(grid);

    return grid.getBestPartition();
}

int main() {
    int N;
    cin >> N;

    if (N < MIN_N || N > MAX_N) {
        return 0;
    }

    vector<vector<int>> solution = solve(N);

    cout << solution.size() << endl;
    for (auto& square : solution) {
        cout << square[0] + 1 << " " << square[1] + 1 << " " << square[2]
             << endl;
    }
    return 0;
}
