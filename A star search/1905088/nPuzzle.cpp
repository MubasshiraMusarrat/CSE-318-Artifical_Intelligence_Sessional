#include <queue>
#include <functional>
#include <unordered_set>

#include "node.h"

class nPuzzle
{
private:
    int k;

    struct hammingComparator
    {
        bool operator()(Node *a, Node *b)
        {
            return a->calculateFunction_Hamming() > b->calculateFunction_Hamming();
        }
    };

    struct manhattanComparator
    {
        bool operator()(Node *a, Node *b)
        {
            return a->calculateFunction_Manhattan() > b->calculateFunction_Manhattan();
        }
    };

public:
    nPuzzle(int n)
    {
        k = n;
    }

    int cntInversion(int *arr)
    {
        int cnt = 0;
        for (int i = 0; i < k * k; i++)
        {
            for (int j = i + 1; j < k * k; j++)
            {
                if (arr[i] && arr[j] && arr[i] > arr[j])
                {
                    cnt++;
                }
            }
        }
        return cnt;
    }

   bool isSolveable(int **block)
    {
        int *arr = new int[k * k];
        int cnt = 0;
        int blankRow = -1;
        for (int i = 0; i < k; i++)
        {
            for (int j = 0; j < k; j++)
            {
                arr[cnt++] = block[i][j];
                if (block[i][j] == 0)
                {
                    blankRow = i;
                }
            }
        }
        cnt = cntInversion(arr);
        delete[] arr;

        // If the grid size is odd, solvability depends on the number of inversions
        if (k % 2 != 0)
            return cnt % 2 == 0;

        // For even grid size (4x4), solvability depends on the number of inversions and the row of the blank tile
        if (blankRow % 2 == 0) // blank on even row; counting from bottom
            return cnt % 2 != 0; // number of inversions odd
        else
            return cnt % 2 == 0; // blank on odd row; counting from bottom
                                // number of inversions even
    }

    void printPath(Node *node)
    {
        if (node == NULL)
            return;
        printPath(node->getParent());
        node->printBlock();
        cout << endl;
    }

    void deleteNode(Node *node)
    {
        if (node == NULL)
            return;
        deleteNode(node->getParent());
        delete node;
    }
   
    void solveByHamming(int **block)
    {
        if (!(isSolveable(block)))
        {
            cout << "Unsolvable puzzle" << endl;
            return;
        }
        else
        {
            int expandedNodes = 0;
            int exploredNodes = 0;
            int iteration=1;
            Node *root = new Node(k, block, NULL, 0);
            priority_queue<Node *, vector<Node *>, hammingComparator> openList;
            openList.push(root);
            unordered_set<Node *> closedList;
            expandedNodes++;

            while (!openList.empty())
            {
                Node *curr = openList.top();
                openList.pop();
                exploredNodes++;
                

                if (curr->isGoal())
                {
                    cout << "Minimum number of moves: " << curr->getMoves() << endl;
                    cout << endl;
                    printPath(curr);
                    cout << "Number of expanded nodes: " << expandedNodes << endl;
                    cout << "Number of explored nodes: " << exploredNodes << endl;
                    deleteNode(curr);
                    return;
                }

                //cout<<"iteration no: "<<iteration++<<endl;
                closedList.insert(curr);
                vector<Node *> succesors = curr->getSuccesors();
                for (int i = 0; i < succesors.size(); i++)
                {
                    Node *succ = succesors[i];
                    if (closedList.find(succ) == closedList.end())
                    {
                        openList.push(succ);
                        expandedNodes++;
                    }
                }
            }
        }
    }

    void solveByManhattan(int **block)
    {
        Node *root = new Node(k, block, NULL, 0);
        if (!(isSolveable(block)))
    {
        cout << "Unsolvable puzzle" << endl;
        return;
    }
    else
    {
        int expandedNodes = 0;
        int exploredNodes = 0;
        int iteration=1;
        priority_queue<Node *, vector<Node *>, manhattanComparator> openList;
        openList.push(root);
        unordered_set<Node *> closedList;
        expandedNodes++;
        //cout << "expanded at start" << expandedNodes << endl;

        while (!openList.empty())
        {
            Node *curr = openList.top();
            openList.pop();
            exploredNodes++;
            //cout << "explored nodes" << exploredNodes << endl;

            if (curr->isGoal())
            {
                cout << "Minimum number of moves: " << curr->getMoves() << endl;
                printPath(curr);
                cout << "Number of expanded nodes: " << expandedNodes << endl;
                cout << "Number of explored nodes: " << exploredNodes << endl;
                deleteNode(root);
                return;
            }


            //cout<<"iteration no: "<<iteration++<<endl;
            vector<Node *> successors = curr->getSuccesors();
            for (int i = 0; i < successors.size(); i++)
            {
                //Node *succ = successors[i];
                if (closedList.find(successors[i]) == closedList.end())
                {
                    openList.push(successors[i]);
                    expandedNodes++;
                    //cout << "expanded nodes" << expandedNodes << endl;
                    //if(expandedNodes>50){
                   //     cout<<"Unsolvable puzzle"<<endl;
                   //     return;
                   // }
                }
                else
                {
                    delete successors[i]; // Clean up unused successor nodes
                }
            }
            closedList.insert(curr);
        }
    }
    }
};

int main(){
    int n;
    cout << "Enter the value of n: ";
    cin >> n;
    cout << "Enter initial board: " << endl;
    int **block = new int *[n];
    for (int i = 0; i < n; i++)
    {
        block[i] = new int[n];
        for (int j = 0; j < n; j++)
        {
            cin >> block[i][j];
        }
    }
    nPuzzle puzzle(n);
    
    cout << "Solving by Hamming Distance: " << endl;
    puzzle.solveByHamming(block);
    
    cout << "Solving by Manhattan Distance: " << endl;
    puzzle.solveByManhattan(block);

    return 0;
}