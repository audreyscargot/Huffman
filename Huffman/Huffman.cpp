#include <algorithm>
#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <queue>
#include <vector>

#include "Node.h"

using namespace std;

//var pour récupérer txt sous forme de var utilisables
std::ifstream file("Text.txt");
vector<string> fileLines;
std::string line;

//var pour organiser en fonction des caractères et de leur fréquences
map<char, int> letterApparition;
vector<pair<char, int>> sortedVector;

//var pour arbre huffman
vector<Node*> unsortedNodes;
vector<Node*> sortedNodes;
map<char, string> codingTable;

vector<vector<string>> codedTable;

//---------------Func pour organiser txt--------------------
void createTextVar()
{
    if (file.is_open())
    {
        while (getline(file, line))
        {
            fileLines.push_back(line);
        }
    }
}

void getLettersApparition()
{
    for (int i = 0; i < fileLines.size(); i++)
    {
        for (int j = 0; j < fileLines[i].length(); j++)
        {
            letterApparition.insert({fileLines[i][j], letterApparition[fileLines[i][j]]++}); //on ajoute à chaque caractère identique trouvé car map = une seule key
        }
    }
    
}

void sortMap()
{
    vector<pair<char, int>> sortedLetters(letterApparition.begin(), letterApparition.end());
    sort(sortedLetters.begin(), sortedLetters.end(), [](const auto &a, const auto &b)
    {
        return a.second < b.second;
    });
    
    sortedVector = sortedLetters; //nouveau vector de pair sorted par ordre croissant
}

//---------------Func pour créer Huffman--------------------

void createNodes() //création de tous les nodes feuilles et ajouts dans le tableau unsorted
{
    for (const auto &letter : sortedVector)
    {
        Node* newNode = new Node(letter.first, letter.second);
        unsortedNodes.push_back(newNode);
    }
}

void createTree()
{
    while (unsortedNodes.size() > 1)
    {
        Node* _newNode;
        int _freq = unsortedNodes[0]->getFreq()+unsortedNodes[1]->getFreq(); //ajout des freq des nodes enfnats pour le nouveau node
        Node* _left = unsortedNodes[0]->getFreq() < unsortedNodes[1]->getFreq() ? unsortedNodes[0] : unsortedNodes[1]; //à gauche le node avec la plus petite fréquence
        Node* _right = _left == unsortedNodes[0] ? unsortedNodes[1] : unsortedNodes[0];

        _newNode = new Node(_freq, _left, _right); //créer nouveau node sans char donc pas une feuille

        sortedNodes.push_back(_left); //ajouter au tableau node sorted et enlever du tableau node unsorted les nodes enfants
        sortedNodes.push_back(_right);
        unsortedNodes.erase(find(unsortedNodes.begin(), unsortedNodes.end(), _left));
        unsortedNodes.erase(find(unsortedNodes.begin(), unsortedNodes.end(), _right));

        unsortedNodes.push_back(_newNode);
        sort(unsortedNodes.begin(), unsortedNodes.end(), [](Node* a, Node* b)
        {
            return a->freq < b->freq;
        });
    }
}

void assignValue()
{
    int index = 0;
    reverse(sortedNodes.begin(), sortedNodes.end()); //reverse pour commencer par le node le plus haut (qui aura donc le path le plus petit)
    for (int i = 0; i < (int)sortedNodes.size(); i++)
    {
        int nodeValue;
        index % 2 == 0 ? nodeValue = 0 : nodeValue = 1; //tableau de base organisé tel que %2 == 0 est node de gauche, %2 == 1 est node de droite
        sortedNodes[i]->addPath(nodeValue);
        if (sortedNodes[i]->getLeft())
        {
            for (int j = 0; j < (int)sortedNodes[i]->getPath().size(); j++) //pour chaque node du path du node regardé, on l'ajoute à ses enfants
            {
                sortedNodes[i]->getLeft()->addPath(sortedNodes[i]->getPath()[j]);
            }
            sortedNodes[i]->getLeft()->addPath(nodeValue); //on ajoute à la fin du path, la valeur de node regardé à ses enfants
        }
        if (sortedNodes[i]->getRight()) //on fait pareil à droite
        {
            for (int j = 0; j < (int)sortedNodes[i]->getPath().size(); j++)
            {
                sortedNodes[i]->getRight()->addPath(sortedNodes[i]->getPath()[j]);
            }
            sortedNodes[i]->getRight()->addPath(nodeValue);
        }
        index++;
    }
    for (const auto &node : sortedNodes)
    {
        node->makeValue();
    }
}

//Créer la table d'encodage
void makeHuffmanTable() //récupère uniquement les nodes feuilles pour en créer une table d'encodage avec le caractère et son écriture simplifiée
{
    for (auto node : sortedNodes)
    {
        if (node->getLeafAsChar())
        {
            codingTable.insert({node->getLeafAsChar(), node->getValue()});
        }
    }
}

void transformToHuffman()
{
    for (int i = 0; i < (int)fileLines.size(); i++)
    {
        vector<string> lineChar;
        for (int j = 0; j < (int)fileLines[i].length(); j++)
        {
            string str = codingTable.find({fileLines[i][j]})->second;
            lineChar.push_back(str);
        }
        codedTable.push_back(lineChar);
    }
}

//---------------Déchiffrer Huffman--------------------

map<string, char> makeTranslationMap(map<char, string> _codingTable) //inverse la map reçue d'encodage
{
    map<string, char> translationMap;
    for (auto it = _codingTable.begin(); it != _codingTable.end(); it++)
    {
        translationMap.insert({it->second, it->first});
    }
    return translationMap;
}

void decodingHuffman(map<char, string> _codingTable, vector<vector<string>> _codedTable)
{
    map<string, char> translationMap = makeTranslationMap(_codingTable);
    vector<string> translatedTable;
    for (int i = 0; i < (int)_codedTable.size(); i++) //pour chaque ligne 
    {
        string s;
        for (int j = 0; j < (int)_codedTable[i].size(); j++) //pour chaque caractère
        {
            char c = translationMap.find(_codedTable[i][j])->second; //chercher la correspondance grâce ) la map inversée
            s += c; //ajouter au string 'ligne'
        }
        translatedTable.push_back(s);
        cout << s << endl;
    }
}

//---------------Debug func--------------------
void checkMap()
{
    for (map<char, int>::const_iterator it = letterApparition.begin(); it != letterApparition.end(); it++)
    {
        char keys = it->first;
        cout << keys  << " " << letterApparition[keys] << endl;
    }
}

void checkSortedLetters()
{
    for (const auto &letter : sortedVector)
    {
        cout << letter.first << " " << letter.second << endl;
    }
}

void checkSortedNodes()
{
    for (const auto &node : sortedNodes)
    {
        cout << node->getLeaf() << " " << node->getFreq() << endl;
    }
}

void checkValueFromTree()
{
    for (const auto &node : sortedNodes)
    {
        cout << node->getLeaf() << " " << node->getValue() << endl;
    }
}

void checkHuffmanTable()
{
    for (map<char, string>::const_iterator it = codingTable.begin(); it != codingTable.end(); it++)
    {
        cout << it->first << " " << it->second << endl;
    }
}

void checkCodedTable()
{
    int index = 0;
    for (const auto s : codedTable)
    {
        cout << index << " " << endl;
        index++;
        for (int i = 0; i < s.size(); i++)
        {
            cout << s[i] << " next ";
        }
        cout << endl;
    }
}

//------------------MAIN----------------

int main()
{
    createTextVar();
    getLettersApparition();
    sortMap();
    createNodes();
    createTree();
    assignValue();
    makeHuffmanTable();
    transformToHuffman();
    cout << "TRANSLATED TEXT" << endl;
    cout << endl;
    decodingHuffman(codingTable, codedTable);
    return 0;
}
