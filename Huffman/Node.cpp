#include "Node.h"

Node::Node(char _c, int _freq)
{
    c = _c;
    freq = _freq;
}

Node::Node(int _freq, Node* _left, Node* _right)
{
    freq = _freq;
    left = _left;
    right = _right;
}

std::string Node::getLeaf()
{
    std::string s = &c;
    if (c) return s;
    return "";
}

char Node::getLeafAsChar()
{
    return c;
}

int Node::getFreq()
{
    return freq;
}

Node* Node::getLeft()
{
    return left;
}

Node* Node::getRight()
{
    return right;
}

void Node::setNodeLeft(Node* newNode)
{
    if (left) left = newNode;
}

void Node::setNodeRight(Node* newNode)
{
    if (right) right = newNode;
}

void Node::addPath(int _value)
{
    path.push_back(_value);
}

std::vector<int> Node::getPath()
{
    return path;
}

void Node::makeValue()
{
    std::reverse(path.begin(), path.end());
    for (int i = 0; i < (int)path.size(); i++)
    {
        value += std::to_string(path[i]);
    }
}

std::string Node::getValue()
{
    return value;
}
