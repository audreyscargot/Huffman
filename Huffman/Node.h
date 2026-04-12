#pragma once
#include <string>
#include <vector>

class Node
{
public:
    char c;
    int freq;
    std::vector<int> path;
    std::string value;
    Node* left;
    Node* right;

    Node(char _c, int _freq);
    Node(int _freq, Node* _left, Node* _right);

    std::string getLeaf();
    char getLeafAsChar();
    int getFreq();
    Node* getLeft();
    Node* getRight();
    void setNodeLeft(Node* newNode);
    void setNodeRight(Node* newNode);
    void addPath(int _value);
    std::vector<int> getPath();

    void makeValue();
    std::string getValue();
};
