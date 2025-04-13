#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>

using namespace std;

vector<vector<int>> getMatrix(const string& s1, const string& s2,
                              int insertCost, int deleteCost, int replaceCost) {
    cout << "Calculating Levenshtein distance matrix" << endl << endl;
    size_t M = s1.size(), N = s2.size();
    vector<vector<int>> D(M + 1, vector<int>(N + 1, 0));

    for (int i = 1; i <= N; ++i) {
        D[0][i] = D[0][i - 1] + insertCost;
    }

    for (int i = 1; i <= M; ++i) {
        D[i][0] = D[i - 1][0] + deleteCost;
    }

    for (int i = 1; i <= M; ++i) {
        for (int j = 1; j <= N; ++j) {
            cout << "Comparing " << s1[i - 1] << " and " << s2[j - 1] << endl;
            if (s1[i - 1] != s2[j - 1]) {
                cout << "Symbols do not match" << endl;
                cout << "Insert cost: " << D[i][j - 1] + insertCost << endl;
                cout << "Delete cost: " << D[i - 1][j] + deleteCost << endl;
                cout << "Replace cost: " << D[i - 1][j - 1] + replaceCost
                     << endl;
                D[i][j] =
                    min(min(D[i - 1][j] + deleteCost, D[i][j - 1] + insertCost),
                        D[i - 1][j - 1] + replaceCost);
            } else {
                cout << "Symbols match" << endl;
                D[i][j] = D[i - 1][j - 1];
            }
            cout << "Cost D[" << i << "][" << j << "]: " << D[i][j] << endl
                 << endl;
        }
    }
    cout << "Matrix D:" << endl;
    for (int i = 0; i <= M; ++i) {
        for (int j = 0; j <= N; ++j) {
            cout << D[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
    return D;
}

string getPath(const vector<vector<int>>& D, const string& s1, const string& s2,
               int insertCost, int deleteCost, int replaceCost) {
    string path;
    int i = s1.size(), j = s2.size();

    while (i > 0 || j > 0) {
        cout << "i: " << i << ", j: " << j << endl;
        if (i > 0 && j > 0 && s1[i - 1] == s2[j - 1] &&
            D[i][j] == D[i - 1][j - 1]) {  // Match
            cout << "Match" << endl;
            path.push_back('M');
            --i;
            --j;
        } else if (i > 0 && D[i][j] == D[i - 1][j] + deleteCost) {  // Delete
            cout << "Delete" << endl;
            path.push_back('D');
            --i;
        } else if (j > 0 && D[i][j] == D[i][j - 1] + insertCost) {  // Insert
            cout << "Insert" << endl;
            path.push_back('I');
            --j;
        } else if (i > 0 && j > 0 &&
                   D[i][j] == D[i - 1][j - 1] + replaceCost) {  // Replace
            cout << "Replace" << endl;
            path.push_back('R');
            --i;
            --j;
        }
        cout << endl;
    }

    reverse(path.begin(), path.end());
    cout << "Path: " << path << endl;
    return path;
}

void getAllPaths(const vector<vector<int>>& D, const string& s1,
                 const string& s2, int insertCost, int deleteCost,
                 int replaceCost, int i, int j, string& currentPath,
                 vector<string>& allPaths) {
    if (i == 0 && j == 0) {
        cout << "Reached start, storing path: " << currentPath << endl;
        allPaths.push_back(currentPath);  // Reached start, store the path
        return;
    }

    if (i > 0 && j > 0 && s1[i - 1] == s2[j - 1] &&
        D[i][j] == D[i - 1][j - 1]) {  // Match
        cout << "Match" << endl;
        currentPath.push_back('M');
        getAllPaths(D, s1, s2, insertCost, deleteCost, replaceCost, i - 1,
                    j - 1, currentPath, allPaths);
        currentPath.pop_back();
    }

    if (i > 0 && D[i][j] == D[i - 1][j] + deleteCost) {  // Delete
        cout << "Delete" << endl;
        currentPath.push_back('D');
        getAllPaths(D, s1, s2, insertCost, deleteCost, replaceCost, i - 1, j,
                    currentPath, allPaths);
        currentPath.pop_back();
    }

    if (j > 0 && D[i][j] == D[i][j - 1] + insertCost) {  // Insert
        cout << "Insert" << endl;
        currentPath.push_back('I');
        getAllPaths(D, s1, s2, insertCost, deleteCost, replaceCost, i, j - 1,
                    currentPath, allPaths);
        currentPath.pop_back();
    }

    if (i > 0 && j > 0 && s1[i - 1] != s2[j - 1] &&
        D[i][j] == D[i - 1][j - 1] + replaceCost) {  // Replace
        cout << "Replace" << endl;
        currentPath.push_back('R');
        getAllPaths(D, s1, s2, insertCost, deleteCost, replaceCost, i - 1,
                    j - 1, currentPath, allPaths);
        currentPath.pop_back();
    }
}

void printPath(const string& path, const string& s1, const string& s2) {
    cout << "Printing path: " << path << endl;
    string operations, top, bottom;
    int i = 0, j = 0;

    for (char c : path) {
        operations.push_back(c);
        switch (c) {
            case 'M':
            case 'R':
                top.push_back(s1[i++]);
                bottom.push_back(s2[j++]);
                break;
            case 'I':
                top.push_back(' ');
                bottom.push_back(s2[j++]);
                break;
            case 'D':
                top.push_back(s1[i++]);
                bottom.push_back(' ');
                break;
        }
    }

    for (char c : operations) {
        cout << c << " ";
    }
    cout << endl;
    for (char c : top) {
        cout << c << " ";
    }
    cout << endl;
    for (char c : bottom) {
        cout << c << " ";
    }
    cout << endl << endl;

    string current = s1;
    i = 0, j = 0;

    cout << current;

    for (char c : path) {
        switch (c) {
            case 'M':
                i++;
                j++;
                break;
            case 'R':
                current[i] = s2[j];
                i++;
                j++;
                break;
            case 'I':
                current.insert(current.begin() + i, s2[j]);
                i++;
                j++;
                break;
            case 'D':
                current.erase(current.begin() + i);
                break;
        }
        cout << " -> " << current;
    }
    cout << endl;
}

int main() {
    int insertCost = 1, deleteCost = 1, replaceCost = 1;
    string s1, s2;

    cin >> replaceCost >> insertCost >> deleteCost;
    cin >> s1 >> s2;

    size_t M = s1.size(), N = s2.size();

    vector<vector<int>> D =
        getMatrix(s1, s2, insertCost, deleteCost, replaceCost);

    // Levenstein Distance (Stepik 1)
    cout << D[M][N] << endl;

    // Levenstein Path (Stepik 2)
    string path = getPath(D, s1, s2, insertCost, deleteCost, replaceCost);
    cout << path << endl << s1 << endl << s2 << endl;

    // All Levenstein Paths (Individual Task)
    string currentPath;
    vector<string> allPaths;
    cout << "Calculating all paths" << endl;
    getAllPaths(D, s1, s2, insertCost, deleteCost, replaceCost, s1.size(),
                s2.size(), currentPath, allPaths);
    cout << endl;
    for (auto& path : allPaths) {
        reverse(path.begin(), path.end());
    }

    for (const auto& path : allPaths) {
        cout << path << endl;
    }

    cout << endl;
    printPath(allPaths[0], s1, s2);

    return 0;
}
