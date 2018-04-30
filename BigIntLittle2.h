#ifndef CBIGINTLITTLE2_H
#define CBIGINTLITTLE2_H
#include <iostream>
#include <string>


class CBigIntLittle2 {
	int* buf;
	int* num;
	int size;
	bool sign;
public:
	CBigIntLittle2();
	CBigIntLittle2(std::string);
	int &operator[](const int);
	int operator[](const int)const;
	friend bool operator ==(const CBigIntLittle2 &a, const CBigIntLittle2 &b);
	friend bool operator <(const CBigIntLittle2 &a, const CBigIntLittle2 &b);
	friend bool operator >(const CBigIntLittle2 &a, const CBigIntLittle2 &b);
	friend bool operator <=(const CBigIntLittle2 &a, const CBigIntLittle2 &b);
	friend bool operator >=(const CBigIntLittle2 &a, const CBigIntLittle2 &b);
	friend bool operator !=(const CBigIntLittle2 &a, const CBigIntLittle2 &b);
	CBigIntLittle2 &operator =(const CBigIntLittle2 &num);
	friend CBigIntLittle2 operator*(const CBigIntLittle2 & a, const int & b);
	friend CBigIntLittle2 operator+(const CBigIntLittle2& num1, const CBigIntLittle2& num2);
	friend CBigIntLittle2 operator-(const CBigIntLittle2& num1, CBigIntLittle2& num2);
	friend CBigIntLittle2 operator *(const CBigIntLittle2 &a, const CBigIntLittle2 &b);
	friend CBigIntLittle2 operator /(const CBigIntLittle2 &a, const CBigIntLittle2 &b);
	friend std::istream &operator >> (std::istream &stream, CBigIntLittle2 &num);
	friend std::ostream &operator <<(std::ostream &stream, const CBigIntLittle2 &num);
	void deleteNumber() {
		free(buf);
	}
	int getSize() { return this->size; }

};

#endif BIGINTLITTLE2_H