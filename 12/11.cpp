#include <iostream>
#include <climits>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

struct Node {
    int i, j;
};

void travel(vector<vector<int>> &field, vector<vector<int>> &paths, queue<Node> &q)
{
    int i, j;
    if (q.empty())
        return;
    i = q.front().i;
    j = q.front().j;
    q.pop();

    cout << i << " " << j << "    ";
    if (field[i - 1][j] != 1 && paths[i - 1][j] > paths[i][j])
    {
        int i_new = i;
        while (field[i_new - 1][j] != 1 && field[i_new][j] != 2)
        {
            paths[i_new - 1][j] = paths[i][j] + 1;
            i_new--;
        }
        //travel(field, paths, i_new, j);
        if (field[i_new][j] != 2) q.push({i_new, j});
    }
    if (field[i + 1][j] != 1 && paths[i + 1][j] > paths[i][j])
    {
        int i_new = i;
        while (field[i_new + 1][j] != 1 && field[i_new][j] != 2)
        {
            paths[i_new + 1][j] = paths[i][j] + 1;
            i_new++;
        }
        //travel(field, paths, i_new, j);
        if (field[i_new][j] != 2) q.push({i_new, j});
    }

    if (field[i][j - 1] != 1 && paths[i][j - 1] > paths[i][j])
    {
        int j_new = j;
        while (field[i][j_new - 1] != 1 && field[i][j_new] != 2)
        {
            paths[i][j_new - 1] = paths[i][j] + 1;
            j_new--;
        }
        //travel(field, paths, i, j_new);
        if (field[i][j_new] != 2) q.push({i, j_new});
    }

    if (field[i][j + 1] != 1 && paths[i][j + 1] > paths[i][j])
    {
        int j_new = j;
        while (field[i][j_new + 1] != 1 && field[i][j_new] != 2)
        {
            paths[i][j_new + 1] = paths[i][j] + 1;
            j_new++;
        }
        //travel(field, paths, i, j_new);
        if (field[i][j_new] != 2) q.push({i, j_new});
    }
    travel(field, paths, q);
}

int main() {
    int N, M;
    cin >> N >> M;
    vector<vector<int>> field(N + 2);
    vector<vector<int>> paths(N + 2);
    for (auto &r : field)
        r.resize(M + 2);
    for (auto &r : paths)
        r.resize(M + 2);
    for (int i = 1; i <= N; i++)
        for (int j = 1; j <= M; j++)
            cin >> field[i][j];
    for (int i = 0; i < N + 2; i++)
        field[i][0] = field[i][M + 1] = 1;
    for (int j = 0; j < M + 2; j++)
        field[0][j] = field[N + 1][j] = 1;
    for (int i = 0; i < N + 2; i++)
        for (int j = 0; j < M + 2; j++)
            //if (field[i][j] != 1)
            paths[i][j] = INT_MAX - 100;
    paths[1][1] = 0;

    queue<Node> q;
    q.push({1, 1});
    travel(field, paths, q);

    for (int i = 0; i < N + 2; i++)
    {
        for (int j = 0; j < M + 2; j++)
            cout << field[i][j] << " ";
        cout << endl;
    }
    cout << endl;
    for (int i = 0; i < N + 2; i++)
    {
        for (int j = 0; j < M + 2; j++)
            if (paths[i][j] < INT_MAX - 100) cout << paths[i][j] << " ";
            else cout << "# ";
        cout << endl;
    }

    vector<int> exit;

    for (int i = 1; i < N + 1; i++)
        for (int j = 1; j < M + 1; j++)
        {
            if (field[i][j] == 2)
                exit.push_back(paths[i][j]);
        }
    cout << *min_element(exit.begin(), exit.end());

    return 0;
}
