#include <algorithm>
#include <bitset>
#include <iostream>
#include <limits>
#include <vector>

using namespace std;

const int INF = numeric_limits<int>::max();

class TSP {
private:
    int n;
    const vector<vector<int>>& graph;
    vector<vector<int>> dp;
    vector<vector<int>> parent;
    vector<string> tspLog;

public:
    TSP(int n, const vector<vector<int>>& graph) : n(n), graph(graph) {}

    pair<int, vector<int>> solveExact() {
        dp.assign(1 << n, vector<int>(n, -1));
        parent.assign(1 << n, vector<int>(n, -1));

        int minCost = tsp(1, 0);
        if (minCost >= INF) {
            return make_pair(-1, vector<int>{});
        }

        reverse(tspLog.begin(), tspLog.end());
        for (string& log : tspLog) {
            cout << log << endl;
        }
        tspLog.clear();

        return make_pair(minCost, reconstructPath());
    }

    pair<int, vector<int>> solveApproximate(int start) { return als(start); }

private:
    int tsp(int mask, int pos) {
        if (mask == (1 << n) - 1) {
            tspLog.push_back("[tsp] Trying to return to start from " +
                             to_string(pos));
            if (graph[pos][0] == 0) {
                tspLog.push_back(
                    "[tsp] No path to start, cycle not found, returning "
                    "INF");
                return INF;
            } else {
                tspLog.push_back(
                    "[tsp] Cycle found, returning to start with cost " +
                    to_string(graph[pos][0]));
                return graph[pos][0];
            }
        }

        if (dp[mask][pos] != -1) {
            tspLog.push_back("[tsp] Using memoized for mask: " +
                             bitset<32>(mask).to_string().substr(32 - n) +
                             ", pos: " + to_string(pos) +
                             ", value: " + to_string(dp[mask][pos]));
            return dp[mask][pos];
        }

        int ans = INF;
        for (int city = 0; city < n; ++city) {
            if (!(mask & (1 << city)) && graph[pos][city] > 0) {
                int newMask = mask | (1 << city);
                int nextCost = tsp(newMask, city);
                if (nextCost != INF) {
                    int newCost = graph[pos][city] + nextCost;
                    tspLog.push_back("  From " + to_string(pos) + " to " +
                                     to_string(city) +
                                     " | cost: " + to_string(graph[pos][city]) +
                                     ", total cost: " + to_string(newCost));
                    if (newCost < ans) {
                        ans = newCost;
                        parent[mask][pos] = city;
                        tspLog.push_back(
                            "[tsp] Updating best next city from " +
                            to_string(pos) + " with mask " +
                            bitset<32>(mask).to_string().substr(32 - n) +
                            " to " + to_string(city) +
                            " (new cost: " + to_string(ans) + ")");
                    }
                } else {
                    tspLog.push_back("No path from " + to_string(pos) + " to " +
                                     to_string(city) + ", skipping...");
                }
            }
        }

        dp[mask][pos] = ans;
        return ans;
    }

    vector<int> reconstructPath() {
        vector<int> path = {0};
        int mask = 1, pos = 0;

        cout << "[reconstructPath] Reconstructing path:" << endl;
        while (mask != (1 << n) - 1) {
            int next = parent[mask][pos];
            if (next == -1) {
                cout << "   Incomplete path: parent[" << mask << "][" << pos
                     << "] = -1" << endl;
                return {};
            }
            cout << "   At mask=" << mask << ", pos=" << pos
                 << " to next=" << next << endl;
            path.push_back(next);
            mask |= (1 << next);
            pos = next;
        }

        cout << "[reconstructPath] Returning to start (0)" << endl;
        path.push_back(0);
        return path;
    }

    pair<int, vector<int>> als(int start) {
        cout << "[als] Starting approximation from city " << start << endl;
        vector<bool> visited(n, false);
        vector<int> path;
        int cost = 0;
        int current = start;
        visited[current] = true;
        path.push_back(current);

        for (int step = 1; step < n; ++step) {
            int nextCity = -1;
            int minDist = INF;

            cout << "   Step " << step << ": from city " << current
                 << ", checking neighbors..." << endl;

            for (int i = 0; i < n; ++i) {
                if (!visited[i] && graph[current][i] > 0 &&
                    graph[current][i] < minDist) {
                    minDist = graph[current][i];
                    nextCity = i;
                }
            }

            if (nextCity == -1) {
                cout << "   No unvisited neighbors found. Approximate solution "
                        "failed."
                     << endl;
                return make_pair(-1, vector<int>{});
            }
            cout << "   Next city " << nextCity << " with cost " << minDist
                 << endl;
            cost += minDist;
            visited[nextCity] = true;
            current = nextCity;
            path.push_back(current);
        }

        if (graph[current][start] > 0) {
            cost += graph[current][start];
            path.push_back(start);
            cout << "   Returning to start city " << start << " with cost "
                 << graph[current][start] << endl;
            cout << "   Final cost: " << cost << endl;
            return make_pair(cost, path);
        } else {
            cout << "   Cannot return to start city. Edge from " << current
                 << " to " << start << " is missing." << endl;
            return make_pair(-1, vector<int>{});
        }
    }
};

int main() {
    int n;
    cin >> n;

    vector<vector<int>> graph(n, vector<int>(n));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            cin >> graph[i][j];

    TSP tsp(n, graph);

    cout << "Exact solution:" << endl;
    pair<int, vector<int>> exactResult = tsp.solveExact();
    if (exactResult.first == -1) {
        cout << "no path" << endl;
    } else {
        cout << exactResult.first << endl;
        for (int city : exactResult.second)
            cout << city << " ";
        cout << endl;
    }

    cout << "Approximate solution:" << endl;
    pair<int, vector<int>> approximateResult = tsp.solveApproximate(0);
    if (approximateResult.first == -1) {
        cout << "no path" << endl;
    } else {
        cout << approximateResult.first << endl;
        for (int city : approximateResult.second)
            cout << city << " ";
        cout << endl;
    }

    return 0;
}
