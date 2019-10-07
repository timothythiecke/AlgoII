// GebalanceerdeBinaireZoekbomen.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>

#include "zoekboom17.h"
#include "Splayboom.h"

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


}
