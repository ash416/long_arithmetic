#ifndef CBIGINTLITTLE_H
#define CBIGINTLITTLE_H
#include <iostream>
#include <string>

#include <ctime>
#include <ratio>
#include <chrono>

class CBigIntLittle {
public:
	using AtomicT = unsigned char;

private:
	AtomicT* buf;
	size_t size;
	bool sign;

public:
	CBigIntLittle();
	CBigIntLittle(std::string);
	AtomicT &operator[](const size_t);
	AtomicT operator[](const size_t)const;
	friend bool operator ==(const CBigIntLittle &a, const CBigIntLittle &b);
	friend bool operator <(const CBigIntLittle &a, const CBigIntLittle &b);
	friend bool operator >(const CBigIntLittle &a, const CBigIntLittle &b);
	friend bool operator <=(const CBigIntLittle &a, const CBigIntLittle &b);
	friend bool operator >=(const CBigIntLittle &a, const CBigIntLittle &b);
	friend bool operator !=(const CBigIntLittle &a, const CBigIntLittle &b);
	CBigIntLittle &operator =(const CBigIntLittle &num);
	friend CBigIntLittle operator*(const CBigIntLittle & a, const int & b);
	friend CBigIntLittle operator+(const CBigIntLittle& num1, const CBigIntLittle& num2);
	friend CBigIntLittle operator-(const CBigIntLittle& num1, CBigIntLittle& num2);
	friend CBigIntLittle operator *(const CBigIntLittle &a, const CBigIntLittle &b);
	friend CBigIntLittle operator /(const CBigIntLittle &a, const CBigIntLittle &b);
	friend std::istream &operator >> (std::istream &stream, CBigIntLittle &num);
	friend std::ostream &operator <<(std::ostream &stream, const CBigIntLittle &num);
	void deleteNumber() {
		free(buf);
	}
	int getSize() { return this->size; }

};

#endif BIGINTLITTLE_H