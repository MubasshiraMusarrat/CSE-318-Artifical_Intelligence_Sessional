#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <vector>

using namespace std;

class Node
{
private:
    int k;
    int moves;
    int **block;
    Node *parent;

public:
    Node(int n, int **node, Node *parent, int moves)
    {
        k = n;
        this->parent = parent;
        this->moves = moves;

        block = new int *[n];
        for (int i = 0; i < n; i++)
        {
            block[i] = new int[n];
            for (int j = 0; j < n; j++)
            {
                block[i][j] = node[i][j];
            }
        }
    }

    Node(const Node &node)
    {
        this->k = node.k;
        this->parent = node.parent;
        this->moves = node.moves;

        block = new int *[k];
        for (int i = 0; i < k; i++)
        {
            block[i] = new int[k];
            for (int j = 0; j < k; j++)
            {
                block[i][j] = node.block[i][j];
            }
        }
    }

    ~Node()
    {
        for (int i = 0; i < k; i++)
        {
            delete[] block[i];
        }
        delete[] block;
    }

    int getMoves()
    {
        return moves;
    }

    int getK()
    {
        return k;
    }

    int **getBlock()
    {
        return block;
    }

    Node *getParent()
    {
        return parent;
    }

    int **getNewBlock()
    {
        int **newBlock = new int *[k];
        for (int i = 0; i < k; i++)
        {
            newBlock[i] = new int[k];
            for (int j = 0; j < k; j++)
            {
                newBlock[i][j] = block[i][j];
            }
        }
        return newBlock;
    }

    int HammingDistance()
    {
        int dist = 0;
        int index = 1;
        for (int i = 0; i < k; i++)
        {
            for (int j = 0; j < k; j++)
            {
                if (block[i][j])
                    block[i][j] != index ? dist++ : dist;
                index++;
            }
        }
        return dist;
    }

    int calculateFunction_Hamming()
    {
        return moves + HammingDistance();
    }

    int ManhattanDistance()
    {
        int rowIdx, columnIdx, dist = 0;
        for (int i = 0; i < k; i++)
        {
            for (int j = 0; j < k; j++)
            {
                if (block[i][j] != 0)
                {
                    rowIdx = (block[i][j] - 1) / k;
                    columnIdx = (block[i][j] - 1) % k;
                    dist += abs(rowIdx - i) + abs(columnIdx - j);
                }
            }
        }
        return dist;
    }

    int calculateFunction_Manhattan()
    {
        return moves + ManhattanDistance();
    }

    bool isGoal()
    {
        int index = 1;
        for (int i = 0; i < k; i++)
        {
            for (int j = 0; j < k; j++)
            {
                if (block[i][j] && block[i][j] != index)
                {
                    return false;
                }
                index++;
            }
        }
        return true;
    }

vector<Node *> getSuccesors()
{
    vector<Node *> successors;

    int blanki, blankj;
    bool blankFound = false;

    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < k; j++)
        {
            if (block[i][j] == 0)
            {
                blanki = i;
                blankj = j;
                blankFound = true;
                break;
            }
        }
        if (blankFound)
        {
            break;
        }
    }

    if (blanki > 0)
    {
        int **newBlock = getNewBlock();
        newBlock[blanki][blankj] = newBlock[blanki - 1][blankj];
        newBlock[blanki - 1][blankj] = 0;
        successors.push_back(new Node(k, newBlock, this, moves + 1));
    }

    if (blanki < k - 1)
    {
        int **newBlock = getNewBlock();
        newBlock[blanki][blankj] = newBlock[blanki + 1][blankj];
        newBlock[blanki + 1][blankj] = 0;
        successors.push_back(new Node(k, newBlock, this, moves + 1));
    }

    if (blankj > 0)
    {
        int **newBlock = getNewBlock();
        newBlock[blanki][blankj] = newBlock[blanki][blankj - 1];
        newBlock[blanki][blankj - 1] = 0;
        successors.push_back(new Node(k, newBlock, this, moves + 1));
    }

    if (blankj < k - 1)
    {
        int **newBlock = getNewBlock();
        newBlock[blanki][blankj] = newBlock[blanki][blankj + 1];
        newBlock[blanki][blankj + 1] = 0;
        successors.push_back(new Node(k, newBlock, this, moves + 1));
    }
    //cout<<"Successors: "<<successors.size()<<endl;
    return successors;
}

    void printBlock()
    {
        for (int i = 0; i < k; i++)
        {
            for (int j = 0; j < k; j++)
            {
                std::cout << block[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
};

#endif // NODE_H