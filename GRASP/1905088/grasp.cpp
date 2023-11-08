#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>
#include <bits/stdc++.h>

using namespace std;

int MAX_ITERATION = 200;
double ALPHA = 0.7;

struct Edge
{
    int to;
    int weight;
};

int n, m;
vector<vector<Edge>> adjList;

set<int> X, Y;
int *sigma_x, *sigma_y;

ofstream fout("output3.csv");

void readGraph(string s)
{
    ifstream fin(s);
    fin >> n >> m;
    adjList.resize(n);
    for (int i = 0; i < m; i++)
    {
        int u, v, wt;
        fin >> u >> v >> wt;
        --u;
        --v;
        Edge edge = {v, wt};
        adjList[u].push_back(edge);
        edge = {u, wt};
        adjList[v].push_back(edge);
    }
    fin.close();
}

void CalculateSigma(int v)
{
    for (Edge edge : adjList[v])
    {
        int u = edge.to;
        int wt = edge.weight;
        if (X.count(u))
        {
            sigma_x[v] += wt;
        }
        else if (Y.count(u))
        {
            sigma_y[v] += wt;
        }
    }
}

vector<int> MakeRCL(double alpha, set<int> &CandidateList)
{
    int w_min_x = INT32_MAX;
    int w_min_y = INT32_MAX;
    int w_max_x = INT32_MIN;
    int w_max_y = INT32_MIN;

    memset(sigma_x, 0, sizeof(int) * n);
    memset(sigma_y, 0, sizeof(int) * n);

    set<int>::iterator it;
    for (it = CandidateList.begin(); it != CandidateList.end(); it++)
    {
        CalculateSigma(*it);
        w_min_x = min(w_min_x, sigma_x[*it]);
        w_min_y = min(w_min_y, sigma_y[*it]);
        w_max_x = max(w_max_x, sigma_x[*it]);
        w_max_y = max(w_max_y, sigma_y[*it]);
    }

    int w_min = min(w_min_x, w_min_y);
    int w_max = max(w_max_x, w_max_y);
    int meu = w_min + alpha * (w_max - w_min);

    vector<int> RCL;
    for (it = CandidateList.begin(); it != CandidateList.end(); it++)
    {
        int greedy_value = max(sigma_x[*it], sigma_y[*it]);
        if (greedy_value >= meu)
        {
            RCL.push_back(*it);
        }
    }
    return RCL;
}

void ConstructSemiGreedySolution(double alpha)
{
    X.clear();
    Y.clear();
    set<int> CandidateList;
    for (int i = 0; i < n; i++)
    {
        CandidateList.insert(i);
    }

    while (!CandidateList.empty())
    {
        vector<int> RCL = MakeRCL(alpha, CandidateList);
        int r = rand() % RCL.size();
        int v = RCL[r];
        if (sigma_x[v] > sigma_y[v])
        {
            Y.insert(v);
        }
        else
        {
            X.insert(v);
        }
        CandidateList.erase(v);
    }
}

int LocalSearch()
{
    int iteration = 0;
    bool change = true;

    memset(sigma_x, 0, sizeof(int) * n);
    memset(sigma_y, 0, sizeof(int) * n);

    while (change)
    {
        iteration++;
        change = false;
        for (int i = 0; i < n; i++)
        {
            CalculateSigma(i);
            if (X.count(i) && (sigma_x[i] - sigma_y[i]) > 0)
            {
                X.erase(i);
                Y.insert(i);
                change = true;
                break;
            }
            else if (Y.count(i) && (sigma_y[i] - sigma_x[i]) > 0)
            {
                Y.erase(i);
                X.insert(i);
                change = true;
                break;
            }
        }
    }
    return iteration;
}

long calculateMaxCut()
{
    long maxcut = 0;
    set<int>::iterator it;
    for (it = X.begin(); it != X.end(); it++)
    {
        for (Edge edge : adjList[*it])
        {
            int u = edge.to;
            int wt = edge.weight;
            if (Y.count(u))
            {
                maxcut += wt;
            }
        }
    }
    return maxcut;
}

void grasp(int max_iter)
{
    sigma_x = new int[n];
    sigma_y = new int[n];

    set<int> X_new, Y_new;

    long maxcutConstruction = 0;
    long maxcutLocal = 0;
    long maxcutBest = 0;
    int iterLocal = 0;

    for (int i = 1; i <= max_iter; i++)
    {
        long temp = 0;
        ConstructSemiGreedySolution(ALPHA); // for semi greedy solution
        maxcutConstruction += calculateMaxCut();
        iterLocal += LocalSearch();
        temp = calculateMaxCut();
        maxcutLocal += temp;
        if (i == 1)
        {
            maxcutBest = temp;
            X_new = X;
            Y_new = Y;
        }
        else if (temp > maxcutBest)
        {
            maxcutBest = temp;
            X_new = X;
            Y_new = Y;
        }
    }
    X = X_new;
    Y = Y_new;

    fout << (maxcutConstruction / max_iter) << "," << (iterLocal / max_iter) << "," << (maxcutLocal / max_iter) << "," << max_iter << "," << maxcutBest << endl;
    ;
    delete[] sigma_x;
    delete[] sigma_y;
}

void randomize(int max_iter)
{
    sigma_x = new int[n];
    sigma_y = new int[n];
    long maxcut = 0;

    for (int i = 1; i <= max_iter; i++)
    {
        ConstructSemiGreedySolution(0); // for randomized solution
        maxcut += calculateMaxCut();
    }

    fout << (maxcut / max_iter) << ",";
    delete[] sigma_x;
    delete[] sigma_y;
}

void greedy(int max_iter)
{
    sigma_x = new int[n];
    sigma_y = new int[n];
    long maxcut = 0;

    for (int i = 1; i <= max_iter; i++)
    {
        ConstructSemiGreedySolution(1); // for greedy solution
        maxcut += calculateMaxCut();
    }

    fout << (maxcut / max_iter) << ",";
    delete[] sigma_x;
    delete[] sigma_y;
}

void multipleGraph()
{
    int data[] = {1, 2, 3, 11, 12, 13, 14, 15, 16, 22, 23, 24, 32, 33, 34, 35, 36, 37, 43, 44, 45, 48, 49, 50};
    for (int i = 0; i < 24; i++)
    {
        string s = "g" + to_string(data[i]) + ".rud";
        readGraph(s);
        string S = "G" + to_string(data[i]);
        fout << S << "," << n << "," << m << ",";
        randomize(MAX_ITERATION);
        greedy(MAX_ITERATION);
        grasp(MAX_ITERATION);
        for (int i = 0; i < n; i++)
        {
            adjList[i].clear();
        }
        adjList.clear();
    }
}

void singleGraph(int a)
{
    string s = "g" + to_string(a) + ".rud";
    readGraph(s);
    string S = "G" + to_string(a);
    fout << S << "," << n << "," << m << ",";
    randomize(MAX_ITERATION);
    greedy(MAX_ITERATION);
    grasp(MAX_ITERATION);
}

int main()
{
    srand(0);

    fout << "Name,"
         << " |V| or n,"
         << " |E| or m,"
         << "Simple Randomized or Randomized-1,"
         << "Simple Greedy or Greedy-1,";
    fout << "Semi-greedy-1,"
         << "Simple Local or Local-1,,"
         << "Grasp-1,,"
         << "Known Best Solution or Upper Bound" << endl;
    fout << ","
         << ","
         << ","
         << ","
         << ","
         << ","
         << "Number of Iterations,"
         << "Best Value,"
         << "Number of Iterations,"
         << "Best Value" << endl;

    multipleGraph();
    // singleGraph(1);

    fout.close();
    return 0;
}
