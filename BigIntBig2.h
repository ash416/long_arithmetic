#ifndef CBIGINTBIG2_H
#define CBIGINTBIG2_h
#include <iostream>
#include <string>


class CBigIntBig2 {
	int* buf;
	int* num;
	int size;
	bool sign;
public:
	CBigIntBig2();
	CBigIntBig2(const std::string &);
	int &operator[](const int);
	int operator[](const int)const;
	friend bool operator ==(const CBigIntBig2 &a, const CBigIntBig2 &b);
	friend bool operator <(const CBigIntBig2 &a, const CBigIntBig2 &b);
	friend bool operator >(const CBigIntBig2 &a, const CBigIntBig2 &b);
	friend bool operator <=(const CBigIntBig2 &a, const CBigIntBig2 &b);
	friend bool operator >=(const CBigIntBig2 &a, const CBigIntBig2 &b);
	friend bool operator !=(const CBigIntBig2 &a, const CBigIntBig2 &b);
	CBigIntBig2 &operator =(const CBigIntBig2 &num);
	friend CBigIntBig2 operator+(const CBigIntBig2& num1, const CBigIntBig2& num2);
	friend CBigIntBig2 operator-(const CBigIntBig2& num1, CBigIntBig2& num2);
	friend CBigIntBig2 operator *(const CBigIntBig2 &a, const CBigIntBig2 &b);
	friend CBigIntBig2 operator *(const CBigIntBig2 &a, const int &b);
	friend CBigIntBig2 operator /(const CBigIntBig2 &a, const CBigIntBig2 &b);
	friend std::istream &operator >> (std::istream &stream, CBigIntBig2 &num);
	friend std::ostream &operator <<(std::ostream &stream, const CBigIntBig2 &num);
	void deleteNumber() {
		free(buf);
	}
	int getSize() { return this->size; }
};

#endif CBIGINTBIG2_H