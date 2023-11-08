#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>
#include <array>
#include <algorithm>
#include <random>
#include <cmath>

using namespace std;

struct Node
{
    int attribute; // for leaf node attribute = class_label
    int label;
    vector<Node *> children;
    Node(int attribute)
    {
        this->attribute = attribute;
        this->label = -1;
    }
};

Node *root;

vector<array<int, 7>> dataset;
unordered_map<string, int> buying;
unordered_map<string, int> maint;
unordered_map<string, int> doors;
unordered_map<string, int> persons;
unordered_map<string, int> lug_boot;
unordered_map<string, int> safety;
unordered_map<string, int> class_label;
// unordered_map<int,string>result_check;

double avgAccuracy = 0;
double accuracyArray[20] = {0};

vector<vector<int>> attributeTable = { // 0 = buying, 1 = maint, 2 = doors, 3 = persons, 4 = lug_boot, 5 = safety
    {0, 1, 2, 3}, // 0 = vhigh, 1 = high, 2 = med, 3 = low
    {0, 1, 2, 3}, // 0 = vhigh, 1 = high, 2 = med, 3 = low
    {0, 1, 2, 3}, // 0 = 2, 1 = 3, 2 = 4, 3 = 5more
    {0, 1, 2, 3}, // 0 = 2, 1 = 4, 2 = more
    {0, 1, 2},    // 0 = big, 1 = med, 2 = small
    {0, 1, 2}     // 0 = high, 1 = med, 2 = low
};

void init_map()
{
    buying["vhigh"] = 0;
    buying["high"] = 1;
    buying["med"] = 2;
    buying["low"] = 3;
    maint["vhigh"] = 0;
    maint["high"] = 1;
    maint["med"] = 2;
    maint["low"] = 3;
    doors["2"] = 0;
    doors["3"] = 1;
    doors["4"] = 2;
    doors["5more"] = 3;
    persons["2"] = 0;
    persons["4"] = 1;
    persons["more"] = 2;
    lug_boot["big"] = 0;
    lug_boot["med"] = 1;
    lug_boot["small"] = 2;
    safety["high"] = 0;
    safety["med"] = 1;
    safety["low"] = 2;
    class_label["unacc"] = 0;
    class_label["acc"] = 1;
    class_label["good"] = 2;
    class_label["vgood"] = 3;
}

void inputData()
{
    ifstream fin("car.data");
    string s;
    while (getline(fin, s))
    {
        istringstream iss(s);
        array<int, 7> m;
        int i = 0;
        while (getline(iss, s, ','))
        {
            if (i == 0)
                m[i] = buying[s];
            else if (i == 1)
                m[i] = maint[s];
            else if (i == 2)
                m[i] = doors[s];
            else if (i == 3)
                m[i] = persons[s];
            else if (i == 4)
                m[i] = lug_boot[s];
            else if (i == 5)
                m[i] = safety[s];
            else if (i == 6)
                m[i] = class_label[s];
            i++;
        }
        dataset.push_back(m);
    }
    fin.close();
}

int plularlity_value(vector<array<int, 7>> examples)
{
    int count[4] = {0}; // 0 = unacc, 1 = acc, 2 = good, 3 = vgood
    for (int i = 0; i < examples.size(); i++)
    {
        count[examples[i][6]]++;
    }
    int max = count[0];
    int max_index = 0;
    for (int i = 1; i < 4; i++)
    {
        if (count[i] > max)
        {
            max = count[i];
            max_index = i;
        }
    }
    return max_index; // max_index = class_labels
}

double calculateEntropy(vector<array<int, 7>> examples)
{
    int count[4] = {0}; // 0 = unacc, 1 = acc, 2 = good, 3 = vgood
    for (int i = 0; i < examples.size(); i++)
    {
        count[examples[i][6]]++;
    }
    double entropy = 0.0;
    for (int i = 0; i < 4; i++)
    {
        if (count[i] != 0)
        {
            double p = (double)count[i] / (double)examples.size();
            entropy += -p * log2(p);
        }
    }
    return entropy;
}

int importance(vector<int> attributes, vector<array<int, 7>> examples)
{
    double gain_max = -1;
    int attribute_max = -1;
    for (int i = 0; i < attributes.size(); i++)
    {
        double remainder = 0.0;
        int attributeName = attributes[i]; // index and name are same
        vector<int> attributeValues = attributeTable[attributeName];
        for (int j = 0; j < attributeValues.size(); j++)
        {
            vector<array<int, 7>> examples_i;
            for (int k = 0; k < examples.size(); k++)
            { // get examples for attribute value j
                if (examples[k][attributeName] == attributeValues[j])
                {
                    examples_i.push_back(examples[k]);
                }
            }
            double entropy = calculateEntropy(examples_i);
            remainder += (double)examples_i.size() / (double)examples.size() * entropy;
        }
        double parentEntropy = calculateEntropy(examples);
        double gain = parentEntropy - remainder;
        if (gain > gain_max)
        {
            gain_max = gain;
            attribute_max = attributeName;
        }
    }
    return attribute_max;
}

Node *decisionTree(vector<array<int, 7>> examples, vector<int> attributes, vector<array<int, 7>> parent_examples)
{
    Node *node = new Node(-1);
    if (examples.empty())
    { // if examples is empty then return plurality_value(parent_examples)
        node->attribute = plularlity_value(parent_examples);
        return node;
    }

    for (int i = 0; i < examples.size(); i++)
    { // if all examples have the same classification then return the classification
        if (examples[i][6] != examples[0][6])
        { // 6 = class_labels
            break;
        }
        if (i == examples.size() - 1)
        {
            node->attribute = examples[0][6];
            return node;
        }
    }

    if (attributes.empty())
    { // if attributes is empty then return plurality_value(examples)
        node->attribute = plularlity_value(examples);
        return node;
    }

    int attributeName = importance(attributes, examples); // A = argmax(importance(attributes,examples))
    attributes.erase(remove(attributes.begin(), attributes.end(), attributeName), attributes.end());
    node->attribute = attributeName;
    vector<int> attributeValues = attributeTable[attributeName];
    for (int i = 0; i < attributeValues.size(); i++)
    { // for each possible value v_i of A do
        vector<array<int, 7>> examples_i;
        for (int j = 0; j < examples.size(); j++)
        {
            if (examples[j][attributeName] == attributeValues[i])
            { // name & index are same
                examples_i.push_back(examples[j]);
            }
        }
        Node *child = decisionTree(examples_i, attributes, examples);
        child->label = attributeValues[i];
        node->children.push_back(child);
    }
    return node;
}

double predict(vector<array<int, 7>> testSet)
{
    int count = 0;
    for (int i = 0; i < testSet.size(); i++)
    {
        Node *curr = root;
        while (curr->children.size() != 0)
        {
            for (int j = 0; j < curr->children.size(); j++)
            {
                if (curr->children[j]->label == testSet[i][curr->attribute])
                {
                    curr = curr->children[j];
                    break;
                }
            }
        }
        if (curr->attribute == testSet[i][6])
        { // attribute = class_label for leaves
            count++;
        }
    }
    // cout<<"Accuracy: "<<(double)count/(double)testSet.size()*100<<"%"<<endl;
    double accuracy = (double)count / (double)testSet.size() * 100;
    return accuracy;
}

int main()
{
    // result_check[0] = "unacc";
    // result_check[1] = "acc";
    // result_check[2] = "good";
    // result_check[3] = "vgood";

    vector<array<int, 7>> trainSet;
    vector<array<int, 7>> testSet;

    init_map();
    inputData();

    int size = dataset.size() * 0.8;
    vector<int> attributes = {0, 1, 2, 3, 4, 5}; // 0 = buying, 1 = maint, 2 = doors, 3 = persons,
                                                 // 4 = lug_boot, 5 = safety

    for (int i = 1; i <= 20; i++)
    {

        random_device rd;
        mt19937 rng(rd());
        shuffle(dataset.begin(), dataset.end(), rng);

        trainSet.assign(dataset.begin(), dataset.begin() + size);
        testSet.assign(dataset.begin() + size, dataset.end());
        root = decisionTree(trainSet, attributes, trainSet);
        accuracyArray[i - 1] = predict(testSet);
        avgAccuracy += accuracyArray[i - 1];
    }

    cout << "Mean Accuracy: " << avgAccuracy / 20 << "%\n"; // 20 = number of iterations

    double sum = 0;
    for (int i = 0; i < 20; i++)
    {
        sum += (accuracyArray[i] - (avgAccuracy / 20)) * (accuracyArray[i] - (avgAccuracy / 20));
    }
    cout << "Standard Deviation: " << sqrt(sum / 20) << "%\n"; // 20 = number of iterations

    return 0;
}