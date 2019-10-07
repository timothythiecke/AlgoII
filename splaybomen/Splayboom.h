#pragma once

#include "zoekboom17.h"
#include <functional>



enum class EChildDirection
{
	LEFT,	// kind is de linker deelboom
	RIGHT,	// kind is de rechter deelboom
	INVALID // opgegeven argument is niet geldig
};



enum class ERotationDirection
{
	LEFT,	// Klokwijze rotatie
	RIGHT,	// Tegenklokwijze rotatie
	INVALID
};


enum class ESplayState
{
	INVALID = -1,
	LEFTLEFT,
	LEFTRIGHT,
	RIGHTLEFT,
	RIGHTRIGHT
};


template <class Sleutel, class Data>
class SplayBoom : public Zoekboom<Sleutel, Data>
{
public:
	virtual Zoekboom<Sleutel, Data>* voegtoe(const Sleutel& sleutel, const Data& data, bool dubbelsToestaan = false) override;
	virtual void zoek(const Sleutel& sleutel, zoekKnoop<Sleutel, Data>*& ouder, Zoekboom<Sleutel, Data>*& plaats) override;
    std::function<void (zoekKnoop<Sleutel, Data> &)> hookAfterSplay;

protected:
	void splay(Zoekboom<Sleutel, Data>* node);

	//EChildDirection giveChildDirectionType()
};



template<class Sleutel, class Data>
Zoekboom<Sleutel, Data>* SplayBoom<Sleutel, Data>::voegtoe(const Sleutel& sleutel, const Data& data, bool dubbelsToestaan)
{
	this->splay(Zoekboom<Sleutel, Data>::voegtoe(sleutel, data, dubbelsToestaan));

    // function oproepen geef uw knoop mee

    if (this->hookAfterSplay) {
        this->hookAfterSplay(**this);
    }
	return nullptr;
}



template<class Sleutel, class Data>
void SplayBoom<Sleutel, Data>::zoek(const Sleutel& sleutel, zoekKnoop<Sleutel, Data>*& ouder, Zoekboom<Sleutel, Data>*& plaats)
{
	Zoekboom<Sleutel, Data>::zoek(sleutel, ouder, plaats);
	this->splay(plaats);


	ouder = (*this)->ouder;
	plaats = this;

    if (this->hookAfterSplay) {
        this->hookAfterSplay(**this);
    }
    // function oproepen geef uw knoop mee
}



// Bottom up implementatie
// Gegeven: de meest recent toegevoegde knoop
template<class Sleutel, class Data>
void SplayBoom<Sleutel, Data>::splay(Zoekboom<Sleutel, Data>* node)
{
	while (node != nullptr && *node && (*node)->ouder != nullptr)
	{
		// Case 1: de ouder van c is de wortel, zig
		if ((*node)->ouder->ouder == nullptr)
		{
			ERotationDirection direction = ERotationDirection::INVALID;

			// Als de linker kind geldig is en dit kind heeft dezelfde sleutel als de laatst toegevoegde
			if ((*node)->ouder->giveChildTree(true) && (*node)->ouder->giveChildTree(true)->sleutel == (*node)->sleutel)
			{
				direction = ERotationDirection::LEFT;
			}
			// Anders als rechter kind geldig is en dit kind heeft dezelfde sleutel als de laatst toegevoegde
			else if ((*node)->ouder->giveChildTree(false) && (*node)->ouder->giveChildTree(false)->sleutel == (*node)->sleutel)
			{
				direction = ERotationDirection::RIGHT;
			}

			if (direction != ERotationDirection::INVALID)
			{
				// Ik denk niet dat dit zeer efficient is, maar dat is misschien de aard van splay trees
				// Alternatief misschien door een stack bij te houden?
				// De geefBoomBovenKnoop functie kan niet verder gaan
				zoekKnoop<Sleutel, Data>* o;
				Zoekboom<Sleutel, Data>* plaats;
				Zoekboom<Sleutel, Data>::zoek((*node)->ouder->sleutel, o, plaats);
				if (plaats)
				{
					if (direction == ERotationDirection::LEFT)
						plaats->rotate(true);
					else
						plaats->rotate(false);
				}
				else
				{
					//throw "Already root or could not find parent";
				}

				node = nullptr;
			}
		}
		else // Knoop c heeft nog een grootouder
		{
			// 4 gevallen
			zoekKnoop<Sleutel, Data>* k;
			Zoekboom<Sleutel, Data>* p;
			Zoekboom<Sleutel, Data>::zoek((*node)->ouder->sleutel, k, p);

			Zoekboom<Sleutel, Data>* g;
			Zoekboom<Sleutel, Data>::zoek((*node)->ouder->ouder->sleutel, k, g);


			// c rechterkind van p en p linkerkind van g
			// -> roteer p tegenwijzer(links), roteer g met de wijzer mee (rechts)
			/*if ((*node)->ouder->giveChildTree(false) && (*node)->ouder->giveChildTree(false)->sleutel == (*node)->sleutel) // c rechterkind van p
			{
				if ((*node)->ouder->sleutel == (*node)->ouder->ouder->giveChildTree(true)->sleutel) // p linkerkind van g
				{
					p->rotate(false);
					g->rotate(true);
				}
			}

			// c linkerkind van p en p linkerkind van g
			// -> roteer g met de wijzer mee (rechts), roteer p met de wijzer mee (rechts)
			if ((*node)->ouder->giveChildTree(true) && (*node)->ouder->giveChildTree(true)->sleutel == (*node)->sleutel) // c linkerkind van p
			{
				if ((*node)->ouder->sleutel == (*node)->ouder->ouder->giveChildTree(true)->sleutel) // p linkerkind van g
				{
					g->rotate(true);
					p->rotate(true);
				}
			}*/

			//ESplayState state = ESplayState::INVALID;

			// Needs cleanup
			if ((*node)->ouder->ouder->giveChildTree(true) && (*node)->ouder->sleutel == (*node)->ouder->ouder->giveChildTree(true)->sleutel) // p linkerkind van g
			{
				if ((*node)->ouder->giveChildTree(false) && (*node)->ouder->giveChildTree(false)->sleutel == (*node)->sleutel) // c rechterkind van p
				{
					p->rotate(false);
					g->rotate(true);

					node = g;
					//return;
				}
				else if ((*node)->ouder->giveChildTree(true) && (*node)->ouder->giveChildTree(true)->sleutel == (*node)->sleutel) // c linkerkind van p
				{
					g->rotate(true);
					g->rotate(true); // Waarom werkt dit, vermoedelijk omdat we eerst de grootouder roteren! waardoor dat p niet meer geldig is
					//p->rotate(true);

					node = g;
					//return;
				}
				else
				{
					//throw "Could not splay, child not child of p";
				}
			}
			else if ((*node)->ouder->ouder->giveChildTree(false) && (*node)->ouder->sleutel == (*node)->ouder->ouder->giveChildTree(false)->sleutel) // p rechterkind van g
			{
				if ((*node)->ouder->giveChildTree(false) && (*node)->ouder->giveChildTree(false)->sleutel == (*node)->sleutel) // c rechterkind van p
				{
					g->rotate(false);
					g->rotate(false); // Waarom werkt dit
					//p->rotate(false);

					node = g;
					//return;
				}
				else if ((*node)->ouder->giveChildTree(true) && (*node)->ouder->giveChildTree(true)->sleutel == (*node)->sleutel) // c linkerkind van p
				{
					p->rotate(true);
					g->rotate(false);

					node = g;
					//return;
				}
				else
				{
					//throw "Could not splay, child not child of p";
				}
			}
			else
			{
				//throw "Could not splay, child not grandchild of g!";
			}
		}
	}
}
