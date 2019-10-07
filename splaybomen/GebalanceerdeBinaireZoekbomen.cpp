// GebalanceerdeBinaireZoekbomen.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <fstream>
#include <iostream>
#include <string>

#include "zoekboom17.h"
#include "Splayboom.h"
#include <algorithm>
#include <cctype>


void readFileIntoTree(Zoekboom<std::string, int> &tree) {

    std::string word;
    std::ifstream file("splaybomen/Shakespeare.txt");
    while (file >> word) {
        word.erase(std::remove_if(word.begin(), word.end(), [](char letter) { return letter == ';' || letter == ',' || letter == '.'; }), word.end());
        std::transform(word.begin(), word.end(), word.begin(), [](unsigned char c){ return std::tolower(c); });
        tree.voegtoe(word, 0);
    }
}

int main()
{
	SplayBoom<int, int> tree;
	tree.voegtoe(1, 0);
	tree.voegtoe(23, 0);
	tree.voegtoe(25, 0);
	tree.voegtoe(6, 0);
	tree.voegtoe(26, 0);
	tree.voegtoe(0, 0);
	tree.voegtoe(4, 0);
	tree.voegtoe(99, 0);
	tree.voegtoe(-1, 0);
	tree.voegtoe(22, 0);
	tree.voegtoe(2, 0);
	tree.voegtoe(5, 0);
	//tree.voegtoe(-3, 0);

	zoekKnoop<int, int>* ouder;
	Zoekboom<int, int>* plaats;
	tree.zoek(25, ouder, plaats);
	tree.zoek(0, ouder, plaats);
	tree.zoek(23, ouder, plaats);

	tree.teken("splay.txt");


	/*zoekKnoop<int, int>* parent;
	Zoekboom<int, int>* node;
	tree.zoek(2, parent, node);*/

    // used 20 for a visisble graph
    SplayBoom<int, int> tree2;
    for (int i = 1; i <= 20; i++) {
        tree2.voegtoe(i, i);
    }
    tree2.zoek(1, ouder, plaats);
    tree2.teken("eennaarhonderd.txt");

    SplayBoom<std::string, int> wordTree;
    wordTree.hookAfterSplay = [](zoekKnoop<std::string, int> &knoop) { knoop.data++; };
    readFileIntoTree(wordTree);
    wordTree.teken("shake.txt");

    std::cout << wordTree.geefDiepte() << std::endl;
    int c = 0;

    wordTree.inorder([&c](const zoekKnoop<std::string, int> &knoop) { c++; });

    std::cout << c << std::endl;

}

