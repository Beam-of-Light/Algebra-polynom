/*!
 * @file Polynom.cpp
 * @brief Implementation of class Polynom
 * @details Functional of polynomial
 */

#include "Polynom.h"

using std::cout;
using std::cin;
using std::endl;

Polynom::Polynom() {
	head = nullptr;
	power = 1;
	prime = 1;
}

Polynom::Polynom(long long _prime, long long _power, std::vector<long long> keys) :prime(_prime), power(_power) {
	head = nullptr;
	for (int i = 0; i < keys.size(); i++) {
		addItem(makeItem(i, keys[i]));
	}
}

Polynom::Polynom(long long _prime, long long _power, std::vector<std::pair<long long, long long>> keys) :prime(_prime), power(_power) {
	head = nullptr;
	for (int i = 0; i < keys.size(); i++) {
		if (keys[i].first < power) addItem(makeItem(keys[i].first, keys[i].second));
	}
}

Polynom::PolyTerm* Polynom::makeItem(long long pow, long long value) {
	auto* el = new PolyTerm;
	el->key = value;
	el->pow = pow;
	el->next = nullptr;
	return el;
}

void Polynom::addItem(Polynom::PolyTerm* el) {
	el->key = el->key % prime;
	if (el->key < 0)  el->key += prime;
	if (el->key != 0) {
		//checks if polinom is empty 
		if (head == nullptr)
		{
			head = el;
			return;
		}
		PolyTerm* t = head;
		//checks if polinoms head
		if (t->pow == el->pow)
		{
			t->key = (t->key + el->key) % prime;
			if (t->key == 0) {
				head = t->next;
				//delete t;
			}
			return;
		}
		else if (t->pow > el->pow)
		{
			el->next = t;
			head = el;
			return;
		}
		//checks the rest terms of polinom
		PolyTerm* t1 = t->next;
		while (t1)
		{
			if (t1->pow == el->pow)
			{
				t1->key = (t1->key + el->key) % prime;
				if (t1->key == 0) {
					//delete t->next;
					t->next = t1->next;
				}
				return;
			}
			else if (t1->pow > el->pow)
			{
				t->next = el;
				el->next = t1;
				return;
			}
			t = t1;
			t1 = t1->next;
		}
		t->next = el;
		el->next = nullptr;

	}
}

void Polynom::show() {
	PolyTerm* tmp = head;
	bool isFirst = true;
	while (tmp != nullptr) {
		if (!isFirst) { cout << " + "; }
		else { isFirst = false; }
		cout << tmp->key;
		if (tmp->pow != 0) cout << "*x^" << tmp->pow;
		tmp = tmp->next;
	}
	cout << endl;
}

/*1     operation +       */
Polynom Polynom::addPoly(Polynom const& pol1, Polynom const& pol2) {
	Polynom pol3;
	pol3.copy(pol1);
	PolyTerm* tmp = pol2.getHead();
	while (tmp) {
		pol3.addItem(makeItem(tmp->pow, tmp->key));
		tmp = tmp->next;
	}
	return pol3;
}
/*1     operation +       */
Polynom operator+(Polynom const& pol1, Polynom const& pol2) {
	Polynom c = c.addPoly(pol1, pol2);
	return c;
}
/*1     operation -       */
Polynom Polynom::diffPoly(Polynom const& pol1, Polynom const& pol2) {
	Polynom pol3;
	pol3.copy(pol1);
	PolyTerm* tmp = pol2.getHead();
	while (tmp) {
		pol3.addItem(makeItem(tmp->pow, -tmp->key));
		tmp = tmp->next;
	}
	return pol3;
}
/*1     operation -       */
Polynom operator-(Polynom const& pol1, Polynom const& pol2) {
	Polynom c = c.diffPoly(pol1, pol2);
	return c;
}
/*1     operation *       */
Polynom Polynom::multPoly(Polynom const& pol1, Polynom const& pol2) {

	if (power == 1) {
		return multSimple(pol1, pol2);
	}
	else {
		//TODO: mult when power>1
		Polynom result = multSimple(pol1, pol2);
		// result % irredus pol with power of result pol
		return result;
	}

}
/*1     operation * (for n=1)      */
Polynom Polynom::multSimple(Polynom const& pol1, Polynom const& pol2) {
	long long pow = pol1.getPolyPower() + pol2.getPolyPower() + 1;
	std::vector<long long> num(pow, 0);
	PolyTerm* tmp1 = pol1.getHead();
	PolyTerm* tmp2 = pol2.getHead();

	while (tmp1) {
		while (tmp2) {
			num[tmp1->pow + tmp2->pow] = num[tmp1->pow + tmp2->pow] + (tmp1->key * tmp2->key);
			tmp2 = tmp2->next;
		}
		tmp1 = tmp1->next;
		tmp2 = pol2.getHead();
	}

	return Polynom(pol1.getPrime(), 1, num);
}
/*1     operation *       */
Polynom operator*(Polynom const& pol1, Polynom const& pol2) {
	Polynom c = c.multPoly(pol1, pol2);
	return c;
}
/*1     operation * (number)      */
Polynom Polynom::multNumber(Polynom const& p, long long const& number) {
	Polynom result;
	result.setPower(p.getPower());
	result.setPrime(p.getPrime());
	PolyTerm* tmp = p.getHead();
	while (tmp) {
		result.addItem(makeItem(tmp->pow, (tmp->key * number)));
		tmp = tmp->next;
	}
	return result;
}

/*6 divisions for numbers in field*/
long long Polynom::division_for_numbers(long long a, long long b, long long prime)
{
	a *= inverse(b, prime);
	return a;
}

long long Polynom::inverse(long long number, long long prime)
{
	long long a = number;
	long long b = prime;
	long long a_1 = 1;
	long long b_1 = 0;
	long long result = 0;
	while ((a != 1) && (b != 1)) {
		if ((a == 0) || (b == 0)) {
			return 0;
		}
		if (a >= b) {
			decrease(b, a, b_1, a_1);
		}
		else {
			decrease(a, b, a_1, b_1);
		}
	}
	if (a == 1) {
		result = a_1;
	}
	else {
		result = b_1;
	}
	result %= prime;
	if (result < 0) {
		result += prime;
	}
	return result;
}
void Polynom::decrease(long long& a, long long& b, long long& a_count_in_a, long long& a_count_in_b) const
{
	while (b >= a) {
		b -= a;
		a_count_in_b -= a_count_in_a;
	}
}

/*9 Function to check if n is prime or not*/
bool Polynom::isPrime(int n)
{
	if (n < 2)
		return false;
	for (int i = 2; i * i <= n; i++)
		if (n % i == 0)
			return false;
	return true;
}

/*9 Mobius Function */
int Polynom::mobius(int N)
{
	// Base Case 
	if (N == 1)
		return 1;
	// For a prime factor i check if i^2 is also 
	// a factor. 
	int p = 0;
	for (int i = 1; i <= N; i++) {
		if (N % i == 0 && isPrime(i)) {
			// Check if N is divisible by i^2 
			if (N % (i * i) == 0)
				return 0;
			else
				// i occurs only once, increase p 
				p++;
		}
	}
	// All prime factors are contained only once 
	// Return 1 if p is even else -1 
	return (p % 2 != 0) ? -1 : 1;
}

/*6 operation for division*/
Polynom Polynom::multPolyforDivide(Polynom const& pol1, Polynom const& pol2) {
	long long pow = pol1.getPolyPower() + pol2.getPolyPower() + 1;
	std::vector<long long> num(pow, 0);
	PolyTerm* tmp1 = pol1.getHead();
	PolyTerm* tmp2 = pol2.getHead();

	while (tmp1) {
		while (tmp2) {
			num[tmp1->pow + tmp2->pow] = num[tmp1->pow + tmp2->pow] + (tmp1->key * tmp2->key);
			tmp2 = tmp2->next;
		}
		tmp1 = tmp1->next;
		tmp2 = pol2.getHead();
	}

	return Polynom(pol1.getPrime(), pow, num);
}
/*1     operation * (number)      */
Polynom operator *(Polynom const& p, long long const& number) {
	Polynom c = c.multNumber(p, number);
	return c;
}
/*1     operation * (number)      */
Polynom operator *(long long const& number, Polynom const& p) {
	Polynom c = c.multNumber(p, number);
	return c;
}

std::pair<Polynom, Polynom> Polynom::simple_division(Polynom const& p1, Polynom const& p2) const
{
	Polynom result(p1.getPrime(), p1.getPower(), std::vector<long long>{0});
	Polynom rest;
	Polynom temp_1 = p1;
	Polynom temp_2 = p2;
	while (temp_1.getPolyPower() >= temp_2.getPolyPower()) {
		Polynom multiply(p1.getPrime(), p1.getPower(), std::vector<long long>{0});
		multiply.addItem(multiply.makeItem(temp_1.getPolyPower() - temp_2.getPolyPower(), temp_1.division_for_numbers(temp_1.getTermKey(temp_1.getPolyPower()), temp_2.getTermKey(temp_2.getPolyPower()), p2.getPrime())));
		temp_2 = temp_2.multPolyforDivide(temp_2, multiply);
		temp_1 = temp_1 - temp_2;
		temp_2 = p2;
		result = result + multiply;
	}
	rest = temp_1;
	return std::pair<Polynom, Polynom>(result, rest);
}
/*1     operation / (number)      */
Polynom operator/(Polynom const& p1, Polynom const& p2)
{
	return p1.simple_division(p1, p2).first;
}
/*1     operation % (number)      */
Polynom operator%(Polynom const& p1, Polynom const& p2)
{
	return p1.simple_division(p1, p2).second;
}

/*9 Equal operator*/
bool operator==(Polynom const& p1, Polynom const& p2)
{
	if (p1.getPolyPower() != p2.getPolyPower()) return false;
	Polynom::PolyTerm* cur1 =  p1.getHead();
	Polynom::PolyTerm* cur2 = p2.getHead();
	while (cur1!=nullptr&&cur2!=nullptr)
	{
		if (cur1->key != cur2->key)
			return false;
		cur1 = cur1->next;
		cur2 = cur2->next;
	}
	return true;
}


/**/

/*4     Number of roots       */
long long Polynom::rootsNumber() {
	long long pow = getPrime() - 1;
	Matrix AMatrix(pow, pow);

	for (long long i = 0, shift = 0; i < pow; i++, shift++) {
		for (long long j = 0; j < pow; j++) {
		    if ( (j + shift) % pow == 0) {
                AMatrix.setElement(i, j, (getTermKey(0) + getTermKey(pow)) % (pow + 1));
		    } else {
		        AMatrix.setElement(i, j, getTermKey((j + shift) % pow));
		    }
		}
	}

	long long matrixRank = AMatrix.rank();
	return (pow - matrixRank);
}

Polynom Polynom::gcd(Polynom p2) {
	Polynom p1(*this);
	while (p1.getPolyPower() != 0 && p2.getPolyPower() != 0) {
		if (p1.getPolyPower() > p2.getPolyPower()) {
			p1 = p1 % p2;
		}
		else {
			p2 = p2 % p1;
		}
	}

	if (p1.getPolyPower() == 0) {
		return p1;
	}

	return p2;
}

/*9 This method calculates nth �yclotomic polynomial*/
Polynom Polynom::CyclotomicPolynomial(int prime, int n)
{
	// if (n % prime == 0) return Polynom();
	std::vector<long long> keys{ 1 };
	Polynom result(prime, 1, keys);
	int mob;
	if (isPrime(n))
		return Polynom(prime, 1, std::vector<long long>(n, 1));
	for (int d = 1; d <= n; d++) {
		if (n % d == 0 && mobius(n / d) == 1) {
			std::vector<long long> keys(d + 1, 0);
			keys[d] = 1;
			keys[0] = -1;
			Polynom multiplier(prime, 1, keys);
			result = result * multiplier;
		}
	}
	for (int d = 1; d <= n; d++) {
		if (n % d == 0 && mobius(n / d) == -1) {
			std::vector<long long> keys(d + 1, 0);
			keys[d] = 1;
			keys[0] = -1;
			Polynom divider(prime, 1, keys);
			result = result / divider;
		}
	}
	return result;
}
