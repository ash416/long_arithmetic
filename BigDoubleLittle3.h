#ifndef CBIGDOUBLELITTLE3_H
#define CBIGDOUBLELITTLE3_H
#include <iostream>
#include <string>


class CBigDoubleLittle3 {
public:
	using AtomicT = char;
private:
	AtomicT* buf;
	size_t size;
	int order;
	bool sign;
public:
	CBigDoubleLittle3();
	CBigDoubleLittle3(std::string);
	/*int &operator[](const int);
	int operator[](const int)const;*/
	friend bool operator ==(const CBigDoubleLittle3 &a, const CBigDoubleLittle3 &b);
/*	friend bool operator <(const CBigDoubleLittle3 &a, const CBigDoubleLittle3 &b);
	friend bool operator >(const CBigDoubleLittle3 &a, const CBigDoubleLittle3 &b);
	friend bool operator <=(const CBigDoubleLittle3 &a, const CBigDoubleLittle3 &b);
	friend bool operator >=(const CBigDoubleLittle3 &a, const CBigDoubleLittle3 &b);
	friend bool operator !=(const CBigDoubleLittle3 &a, const CBigDoubleLittle3 &b);*/
	CBigDoubleLittle3 &operator =(const CBigDoubleLittle3 &num);
	friend CBigDoubleLittle3 operator*(const CBigDoubleLittle3 & a, const int & b);
	void differences(CBigDoubleLittle3 & res, const CBigDoubleLittle3 & big, const CBigDoubleLittle3 & small);
	friend CBigDoubleLittle3 operator+(const CBigDoubleLittle3& num1, const CBigDoubleLittle3& num2);
	friend CBigDoubleLittle3 operator-(const CBigDoubleLittle3& num1, CBigDoubleLittle3& num2);
	friend CBigDoubleLittle3 operator *(const CBigDoubleLittle3 &a, const CBigDoubleLittle3 &b);
	friend CBigDoubleLittle3 operator /(const CBigDoubleLittle3 &a, const CBigDoubleLittle3 &b);
	friend std::istream &operator >> (std::istream &stream, CBigDoubleLittle3 &num);
	friend std::ostream &operator <<(std::ostream &stream, const CBigDoubleLittle3 &num);
	void deleteNumber() {
		free(buf);
	}
	int getSize() { return this->size; }

};

#endif BIGDOUBLELITTLE3_H