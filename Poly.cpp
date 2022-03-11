#include "Poly.h"
#include "ctype.h"
#include <iostream>
#include <string>
#include <math.h>
using namespace std;

Poly::Poly()
{
	PolyNode* p = new PolyNode(-1, 0, NULL);
	head = p;
}

Poly::Poly(const std::vector<int>& deg, const std::vector<double>& coeff)
{

	PolyNode* last = NULL;

	for (int i = deg.size() - 1; i >= 0; i--) {
		PolyNode* element = new PolyNode(deg[i], coeff[i], last);
		last = element;
	}

	PolyNode* h = new PolyNode(-1, 0, last);
	head = h;

}

Poly::~Poly()
{
	PolyNode* p = head;
	PolyNode* l = NULL;
	while (p != NULL) {
		l = p;
		p = p->next;
		free(l);
	}
}

void Poly::addMono(int i, double c)
{
	PolyNode* p = head->next;
	PolyNode* l = NULL;
	PolyNode* n = new PolyNode(i, c, NULL);
	bool isIn = false;

	if (head->next == NULL) { //adding to an empty list
		head->next = n;
	}
	else if (head->next->deg < n->deg) { //adding to the beginning of list
		n->next = head->next;
		head->next = n;
	}
	else { //loop through the list and insert where applicable
		l = head;
		p = head->next;
		while (p != NULL && !isIn) {
			if (p->deg == n->deg) { //adding to an existing node
				p->coeff = p->coeff + n->coeff;
				isIn = true;
				if (p->coeff == 0) { //if an added node deletes a term
					l->next = p->next;
					free(p);
					p = l;
				}
			}
			else if (p->deg < n->deg) { //adding a new node to the list
				l->next = n;
				n->next = p;
				isIn = true;
			}
			else {
				l = p;
				p = p->next;
			}
		}
		if (!isIn) { //adding to the end of the list
			l->next = n;
		}
	}
}

void Poly::addPoly(const Poly& p)
{
	PolyNode* point = p.head->next;

	while (point != NULL) { // basically just repeated adding of monomials
		this->addMono(point->deg,point->coeff);
		point = point->next;
	}
}

void Poly::multiplyMono(int i, double c)
{
	PolyNode* p = head->next;
	PolyNode* l = head;

	while (p != NULL) { //multiply each term by the new monomial
		p->deg = p->deg + i;
		p->coeff = p->coeff * c;
		if (p->coeff == 0) {
			l->next = p->next;
			free(p);
			p = l;
		}
		l = p;
		p = p->next; 

	}
}

void Poly::multiplyPoly(const Poly& p)
{
	Poly blank = Poly();

	PolyNode* point = p.head->next;

	while (point != NULL) {
		Poly temp = Poly();
		duplicate(temp);

		temp.multiplyMono(point->deg, point->coeff);
		blank.addPoly(temp);
		point = point->next;
	}

	point = head->next;
	PolyNode* l = head;
	while (point != NULL) {
		l->next = point->next;
		free(point);
		point = l->next;
	}
	addPoly(blank);
}

void Poly::duplicate(Poly& outputPoly)
{
	PolyNode* p = head->next;
	outputPoly.head = new PolyNode(-1, 0, NULL);
	PolyNode* l = outputPoly.head;

	while (p != NULL) {
		l->next = new PolyNode(p->deg, p->coeff, NULL);
		l = l->next;
		p = p->next;
	}
}

int Poly::getDegree()
{
	if (head->next != NULL) {
		return head->next->deg;
	}
	else {
		return head->deg;
	}
}

int Poly::getTermsNo()
{
	int count = 0;
	PolyNode* p = head;
	while (p->next != NULL) {
		p = p->next;
		count++;
	}
	return count;
}

double Poly::evaluate(double x)
{
	double sum = 0.0;
	PolyNode* p = head->next;

	while (p != NULL) { //adds the evaluation of each term
		sum = sum + p->coeff * (pow(x, p->deg));
		p = p->next;
	}
	return sum;
}

std::string Poly::toString()
{
	string s = "";
	PolyNode* p = head;
	while (p != NULL) {
		s = s + "deg: " + to_string(p->deg) + " coeff: " + to_string(p->coeff) + "; ";
		p = p->next;
	}
	return s;//change this after completing this function
}
