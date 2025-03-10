#include <iostream>
#include <list>
#include <map>
#include <algorithm>
#include <vector>

using namespace std;

class AdjacencyGraph
{
    map<int, list<int>> adjList;

public:
    void addEdge(int u, int v)
    {
        adjList[u].push_back(v);
        adjList[v].push_back(u);
    }

    void
    print()
    {
        cout << "Adjacency List: " << endl;
        for (auto i : adjList)
        {
            cout << i.first << " -> ";
            for (int entry : i.second)
            {
                cout << entry << ", ";
            }
            cout << endl;
        }
    }

    // < - - - - - - - Cycles - - - - - - - > \\ 

    int cycleAmount = 0;
    map<vector<int>, int> cycles;

    // Permutation
    vector<int> permutationBlackList;

    void permutation(int a, vector<int> it)
    {
        list<int> temp = this->adjList[a];

        for (int val : temp)
        {
            if (find(permutationBlackList.begin(), permutationBlackList.end(), val) != permutationBlackList.end())
                continue;

            if (find(it.begin(), it.end(), val) == it.end())
            {
                vector<int> t(it);
                t.push_back(val);

                permutation(val, t);
            }
            else if (val == it[0] && it.size() > 2)
            {
                vector<int> t(it);
                t.push_back(val);

                vector<int> inv;
                for (int i = t.size() - 1; i >= 0; i--)
                {
                    inv.push_back(t[i]);
                }

                if (cycles.find(inv) == cycles.end())
                {
                    for (size_t i = 0; i < t.size(); i++)
                    {
                        cout << (char)('A' + t[i]);
                    }
                    for (size_t i = 0; i < (20 - t.size()); i++)
                    {
                        cout << " ";
                    }

                    cycles.insert(pair<vector<int>, int>(t, 1));
                    cout << " : " << ++cycleAmount << endl;
                }
            }
        }
    }

    void countCycles_Permutation()
    {
        cycleAmount = 0;
        cycles = map<vector<int>, int>();

        for (size_t i = 0; i < adjList.size(); i++)
        {
            vector<int> t;
            t.push_back(i);
            permutation(i, t);
            permutationBlackList.push_back(i);
        }

        cout << endl
             << "< Permutation > Amount of cycles : " << cycleAmount << endl;
    }

    vector<bool> walked;

    vector<int> normalize(vector<int> cycle)
    {
        int min = 0;
        for (size_t i = 0; i < cycle.size(); i++)
        {
            if (cycle[min] > cycle[i])
            {
                min = i;
            }
        }

        vector<int> ret;

        for (size_t i = 0; i < cycle.size(); i++)
        {
            ret.push_back(cycle[(min + i) % cycle.size()]);
        }

        return ret;
    }

    void depthFirstSearch(int a, int start, vector<int> it)
    {
        list<int> temp = this->adjList[a];
        walked[a] = 1;

        for (int val : temp)
        {

            if (start == val && it.size() >= 3)
            {
                vector<int> t(it);
                t.push_back(val);

                t = normalize(t);

                vector<int> inv;
                for (int i = t.size() - 1; i >= 0; i--)
                {
                    inv.push_back(t[i]);
                }

                if (cycles.find(inv) == cycles.end())
                {
                    for (size_t i = 0; i < t.size(); i++)
                    {
                        cout << (char)('A' + t[i]);
                    }
                    for (size_t i = 0; i < (20 - t.size()); i++)
                    {
                        cout << " ";
                    }

                    cycles.insert(pair<vector<int>, int>(t, 1));
                    cout << " : " << ++cycleAmount << endl;
                }
            }
            else if (!walked[val] && val > start)
            {
                /*
                TODO : Optmize memory using global vector
                */
                vector<int> t(it);
                t.push_back(val);

                depthFirstSearch(val, start, t);
            }
        }

        walked[a] = 0;
    }

    void countCycles_DFS()
    {
        cycleAmount = 0;
        cycles = map<vector<int>, int>();

        walked = vector<bool>(adjList.size());

        for (size_t i = 0; i < adjList.size(); i++)
        {
            adjList[i].sort();
        }

        vector<int> t(1);

        for (size_t i = 0; i < adjList.size(); i++)
        {
            t[0] = i;
            depthFirstSearch(i, i, t);
        }

        cout << endl
             << "< Depth First Search > Amount of cycles : " << cycleAmount << endl;
    }
};

int main()
{
    AdjacencyGraph g;
    g.addEdge(3, 0);
    g.addEdge(3, 1);
    g.addEdge(3, 2);
    g.addEdge(3, 5);
    g.addEdge(4, 0);
    g.addEdge(4, 1);
    g.addEdge(4, 2);
    g.addEdge(4, 5);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 5);

    /*
    g.addEdge(5, 6);
    g.addEdge(5, 7);
    g.addEdge(6, 7);
    g.addEdge(6, 8);
    g.addEdge(7, 8);
    g.addEdge(8, 9);
    g.addEdge(9, 10);
    g.addEdge(14, 5);
    g.addEdge(11, 5);
    g.addEdge(10, 5);
    g.addEdge(0, 11);
    g.addEdge(0, 13);
    g.addEdge(14, 13);
    g.addEdge(6, 11);
    g.addEdge(6, 9);
    g.addEdge(6, 14);
    g.addEdge(6, 12);
    g.addEdge(6, 10);
    */

    g.print();

    cout << endl;

    // Permutation

    cout << "< Permutation > Cycle counting..." << endl;
    clock_t startTime = clock();

    g.countCycles_Permutation();

    clock_t endTime = clock();
    double elapsedTime = ((double)(endTime - startTime)) / CLOCKS_PER_SEC;

    cout << "< Permutation > Elapsed time : " << elapsedTime << " sec" << endl;

    cout << endl;

    // DFS

    cout << "< Depth First Search > Cycle counting..." << endl;
    startTime = clock();

    g.countCycles_DFS();

    endTime = clock();
    elapsedTime = ((double)(endTime - startTime)) / CLOCKS_PER_SEC;

    cout << "< Depth First Search > Elapsed time : " << elapsedTime << " sec" << endl;

    cout << endl;

    return 0;
}