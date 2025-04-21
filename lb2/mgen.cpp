#include <iostream>
#include <random>
#include <vector>

using namespace std;

int main() {
    int size = 5, min = 0, max = 100;
    cin >> size >> min >> max;

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(min, max);

    vector<vector<int>> matrix(size, vector<int>(size, 0));
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (i == j)
                continue;
            matrix[i][j] = dist(gen);
        }
    }

    cout << size << endl;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
    return 0;
}
