#include <algorithm>
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

public:
    TSP(int n, const vector<vector<int>>& graph) : n(n), graph(graph) {}

    pair<int, vector<int>> solveExact() {
        dp.assign(1 << n, vector<int>(n, -1));
        parent.assign(1 << n, vector<int>(n, -1));

        int minCost = tsp(1, 0);
        if (minCost >= INF) {
            return make_pair(-1, vector<int>{});
        }
        return make_pair(minCost, reconstructPath());
    }

    pair<int, vector<int>> solveApproximate(int start) { return als(start); }

private:
    int tsp(int mask, int pos) {
        if (mask == (1 << n) - 1) {
            cout << "[tsp] Trying to return to start from " << pos << endl;
            if (graph[pos][0] == 0) {
                cout << "[tsp] No path to start, cycle not found" << endl;
                return INF;
            } else {
                cout << "[tsp] Cycle found, returning to start" << endl;
                return graph[pos][0];
            }
        }

        if (dp[mask][pos] != -1) {
            cout << "[tsp] Using memoized value for mask=" << mask
                 << ", pos=" << pos << " is " << dp[mask][pos] << endl;
            return dp[mask][pos];
        }

        int ans = INF;
        for (int city = 0; city < n; ++city) {
            if (!(mask & (1 << city)) && graph[pos][city] > 0) {
                int newMask = mask | (1 << city);
                int nextCost = tsp(newMask, city);

                if (nextCost != INF) {
                    int newCost = graph[pos][city] + nextCost;
                    cout << "[tsp] From " << pos << " to " << city
                         << " | cost: " << graph[pos][city]
                         << ", total cost: " << newCost << endl;
                    if (newCost < ans) {
                        ans = newCost;
                        parent[mask][pos] = city;
                        cout << "   Updating best next city from " << pos
                             << " with mask " << mask << " to " << city
                             << " (new cost: " << ans << ")\n";
                    }
                } else {
                    cout << "[tsp] No path from " << pos << " to " << city
                         << ", skipping..." << endl;
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

    pair<int, vector<int>> exactResult = tsp.solveExact();
    cout << "Exact solution:" << endl;
    if (exactResult.first == -1) {
        cout << "no path" << endl;
    } else {
        cout << exactResult.first << endl;
        for (int city : exactResult.second)
            cout << city << " ";
        cout << endl;
    }

    pair<int, vector<int>> approximateResult = tsp.solveApproximate(0);
    cout << "Approximate solution:" << endl;
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
