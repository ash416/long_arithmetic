#ifndef CBIGINTBIG_H
#define CBIGINTBIG_h
#include <iostream>
#include <string>

#include <ctime>
#include <ratio>
#include <chrono>


class CBigIntBig {
	int* buf;
	int size;
	bool sign;
public:
	CBigIntBig();
	CBigIntBig(std::string);
	int &operator[](const int);
	int operator[](const int)const;
	friend bool operator ==(const CBigIntBig &a, const CBigIntBig &b);
	friend bool operator <(const CBigIntBig &a, const CBigIntBig &b);
	friend bool operator >(const CBigIntBig &a, const CBigIntBig &b);
	friend bool operator <=(const CBigIntBig &a, const CBigIntBig &b);
	friend bool operator >=(const CBigIntBig &a, const CBigIntBig &b);
	friend bool operator !=(const CBigIntBig &a, const CBigIntBig &b);
	CBigIntBig &operator =(const CBigIntBig &num);
	friend CBigIntBig operator+(const CBigIntBig& num1, const CBigIntBig& num2);
	friend CBigIntBig operator-(const CBigIntBig& num1, CBigIntBig& num2);
	friend CBigIntBig operator *(const CBigIntBig &a, const CBigIntBig &b);
	friend CBigIntBig operator *(const CBigIntBig &a, const int &b);
	friend CBigIntBig operator /(const CBigIntBig &a, const CBigIntBig &b);
	friend std::istream &operator >> (std::istream &stream, CBigIntBig &num);
	friend std::ostream &operator <<(std::ostream &stream, const CBigIntBig &num);
	void deleteNumber() {
		free(buf);
	}
	int getSize() { return this->size; }
};

#endif CBIGINTBIG_H