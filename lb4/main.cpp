#include <iostream>
#include <vector>

using namespace std;

vector<int> prefix_function(const string& pattern) {
    cout << "Calculating prefix function for string: " << pattern << endl
         << endl;
    int m = pattern.size();
    vector<int> lps(m, 0);
    int len = 0;
    for (int i = 1; i < m;) {
        cout << "i: " << i << ", len: " << len << endl;
        cout << "Comparing " << pattern[i] << " and " << pattern[len] << endl;
        if (pattern[i] == pattern[len]) {
            cout << "Symbols match" << endl;
            lps[i++] = ++len;
            cout << "lps[" << i - 1 << "] = " << len << endl;
            cout << "Increase i, i = " << i << endl;
        } else {
            cout << "Symbols do not match" << endl;
            if (len) {
                cout << "len = " << lps[len - 1] << endl;
                len = lps[len - 1];
            } else {
                cout << "lps[" << i << "] = 0" << endl;
                lps[i++] = 0;
                cout << "Increase i, i = " << i << endl;
            }
        }
        cout << endl;
    }

    cout << "Prefix function result:" << endl;
    for (int i = 0; i < m; i++)
        cout << lps[i] << " ";
    cout << endl << endl;

    return lps;
}

vector<int> find(const string& text, const string& pattern) {
    cout << "Searching for: '" << pattern << "' in: '" << text << "'" << endl
         << endl;
    vector<int> result;
    int n = text.size(), m = pattern.size();
    if (m > n) {
        cout << "Text size is less than pattern size" << endl;
        return result;
    }
    vector<int> lps = prefix_function(pattern);

    int i = 0, j = 0;
    while (i < n) {
        cout << "i: " << i << ", j: " << j << endl;
        cout << "Comparing " << text[i] << " and " << pattern[j] << endl;
        if (text[i] == pattern[j]) {
            cout << "Symbols match" << endl;
            i++, j++;
            cout << "Increase i and j, i = " << i << ", j = " << j << endl;
            if (j == m) {
                cout << "Substring found at " << i - j << endl;
                result.push_back(i - j);
                cout << "Reset j, j = " << lps[j - 1] << endl;
                j = lps[j - 1];
            }
        } else {
            cout << "Symbols do not match" << endl;
            if (j) {
                cout << "Reset j, j = " << lps[j - 1] << endl;
                j = lps[j - 1];
            } else {
                i++;
                cout << "j = 0, Increase i, i = " << i << endl;
            }
        }
        cout << endl;
    }
    return result;
}

int main() {
    string pattern, text;
    cin >> pattern >> text;

    vector<int> matches = find(text, pattern);
    if (matches.empty()) {
        cout << "-1" << endl;
    } else {
        for (size_t i = 0; i < matches.size(); i++) {
            if (i > 0)
                cout << ",";
            cout << matches[i];
        }
        cout << endl;
    }

    // if (text.size() != pattern.size()) {
    //     cout << -1 << endl;
    //     return 0;
    // }

    // vector<int> matches = find(pattern + pattern, text);
    // if (matches.empty()) {
    //     cout << -1 << endl;
    //     return 0;
    // }
    // cout << matches[0] << endl;
    return 0;
}
