#ifndef CBIGINTLITTLEBIN_H
#define CBIGINTLITTLEBIN_H
#include <iostream>
#include <string>


class CBigIntLittleBin {
	short* buf;
	int size;
	bool sign;
public:
	CBigIntLittleBin();
	CBigIntLittleBin(std::string);
	short &operator[](const int);
	short operator[](const int)const;
	friend bool operator ==(const CBigIntLittleBin &a, const CBigIntLittleBin &b);
	friend bool operator <(const CBigIntLittleBin &a, const CBigIntLittleBin &b);
	friend bool operator >(const CBigIntLittleBin &a, const CBigIntLittleBin &b);
	friend bool operator <=(const CBigIntLittleBin &a, const CBigIntLittleBin &b);
	friend bool operator >=(const CBigIntLittleBin &a, const CBigIntLittleBin &b);
	friend bool operator !=(const CBigIntLittleBin &a, const CBigIntLittleBin &b);
	CBigIntLittleBin &operator =(const CBigIntLittleBin &num);
	friend CBigIntLittleBin operator*(const CBigIntLittleBin & a, const int & b);
	friend CBigIntLittleBin operator+(const CBigIntLittleBin& num1, const CBigIntLittleBin& numBin);
	friend CBigIntLittleBin operator-(const CBigIntLittleBin& num1, CBigIntLittleBin& numBin);
	friend CBigIntLittleBin operator *(const CBigIntLittleBin &a, const CBigIntLittleBin &b);
	friend CBigIntLittleBin operator /(const CBigIntLittleBin &a, const CBigIntLittleBin &b);
	friend std::istream &operator >> (std::istream &stream, CBigIntLittleBin &num);
	friend std::ostream &operator <<(std::ostream &stream, const CBigIntLittleBin &num);
	void deleteNumber() {
		free(buf);
	}
	int getSize() { return this->size; }

};

#endif BIGINTLITTLEBIN_H