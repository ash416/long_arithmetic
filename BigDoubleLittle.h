#ifndef CBIGDOUBLELITTLE_H
#define CBIGDOUBLELITTLE_H
#include <iostream>
#include <string>


class CBigDoubleLittle {
	int* buf;
	int size;
	int point;
	bool sign;
public:
	CBigDoubleLittle();
	CBigDoubleLittle(std::string);
	int &operator[](const int);
	int operator[](const int)const;
	friend bool operator ==(const CBigDoubleLittle &a, const CBigDoubleLittle &b);
	friend bool operator <(const CBigDoubleLittle &a, const CBigDoubleLittle &b);
	friend bool operator >(const CBigDoubleLittle &a, const CBigDoubleLittle &b);
	friend bool operator <=(const CBigDoubleLittle &a, const CBigDoubleLittle &b);
	friend bool operator >=(const CBigDoubleLittle &a, const CBigDoubleLittle &b);
	friend bool operator !=(const CBigDoubleLittle &a, const CBigDoubleLittle &b);
	CBigDoubleLittle &operator =(const CBigDoubleLittle &num);
	friend CBigDoubleLittle operator*(const CBigDoubleLittle & a, const int & b);
	void differences(CBigDoubleLittle & res, const CBigDoubleLittle & big, const CBigDoubleLittle & small);
	friend CBigDoubleLittle operator+(const CBigDoubleLittle& num1, const CBigDoubleLittle& num2);
	friend CBigDoubleLittle operator-(const CBigDoubleLittle& num1, CBigDoubleLittle& num2);
	friend CBigDoubleLittle operator *(const CBigDoubleLittle &a, const CBigDoubleLittle &b);
	friend CBigDoubleLittle operator /(const CBigDoubleLittle &a, const CBigDoubleLittle &b);
	friend std::istream &operator >> (std::istream &stream, CBigDoubleLittle &num);
	friend std::ostream &operator <<(std::ostream &stream, const CBigDoubleLittle &num);
	void deleteNumber() {
		free(buf);
	}
	int getSizeInt() { return size - point - 1; }
	int getSizeFrac() { return point + 1; }
	int getSize() { return this->size; }
	
};

#endif BIGDOUBLELITTLE_H