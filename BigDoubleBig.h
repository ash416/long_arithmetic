#ifndef CBIGDOUBLEBIG_H
#define CBIGDOUBLEBIG_H
#include <iostream>
#include <string>


class CBigDoubleBig {
	int* buf;
	int size;
	int point;
	bool sign;
public:
	CBigDoubleBig();
	CBigDoubleBig(std::string);
	int &operator[](const int);
	int operator[](const int)const;
	friend bool operator ==(const CBigDoubleBig &a, const CBigDoubleBig &b);
	friend bool operator <(const CBigDoubleBig &a, const CBigDoubleBig &b);
	friend bool operator >(const CBigDoubleBig &a, const CBigDoubleBig &b);
	friend bool operator <=(const CBigDoubleBig &a, const CBigDoubleBig &b);
	friend bool operator >=(const CBigDoubleBig &a, const CBigDoubleBig &b);
	friend bool operator !=(const CBigDoubleBig &a, const CBigDoubleBig &b);
	CBigDoubleBig &operator =(const CBigDoubleBig &num);
	friend CBigDoubleBig operator*(const CBigDoubleBig & a, const int & b);
	void differences(CBigDoubleBig & res, const CBigDoubleBig & big, const CBigDoubleBig & small);
	friend CBigDoubleBig operator+(const CBigDoubleBig& num1, const CBigDoubleBig& num2);
	friend CBigDoubleBig operator-(const CBigDoubleBig& num1, CBigDoubleBig& num2);
	friend CBigDoubleBig operator *(const CBigDoubleBig &a, const CBigDoubleBig &b);
	friend CBigDoubleBig operator /(const CBigDoubleBig &a, const CBigDoubleBig &b);
	friend std::istream &operator >> (std::istream &stream, CBigDoubleBig &num);
	friend std::ostream &operator <<(std::ostream &stream, const CBigDoubleBig &num);
	void deleteNumber() {
		free(buf);
	}
	int getSizeInt() { return size - point - 1; }
	int getSizeFrac() { return point + 1; }
	int getSize() { return this->size; }

};

#endif BIGDOUBLEBIG_H