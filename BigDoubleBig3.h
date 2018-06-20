#ifndef CBIGDOUBLEBIG3_H
#define CBIGDOUBLEBIG3_H
#include <iostream>
#include <string>


class CBigDoubleBig3 {
public:
	using AtomicT = char;
private:
	AtomicT* buf;
	size_t size;
	int order;
	bool sign;
public:
	CBigDoubleBig3();
	CBigDoubleBig3(std::string);
	/*int &operator[](const int);
	int operator[](const int)const;*/
	friend bool operator ==(const CBigDoubleBig3 &a, const CBigDoubleBig3 &b);
/*	friend bool operator <(const CBigDoubleBig3 &a, const CBigDoubleBig3 &b);
	friend bool operator >(const CBigDoubleBig3 &a, const CBigDoubleBig3 &b);
	friend bool operator <=(const CBigDoubleBig3 &a, const CBigDoubleBig3 &b);
	friend bool operator >=(const CBigDoubleBig3 &a, const CBigDoubleBig3 &b);
	friend bool operator !=(const CBigDoubleBig3 &a, const CBigDoubleBig3 &b);*/
	CBigDoubleBig3 &operator =(const CBigDoubleBig3 &num);
	friend CBigDoubleBig3 operator*(const CBigDoubleBig3 & a, const int & b);
	void differences(CBigDoubleBig3 & res, const CBigDoubleBig3 & big, const CBigDoubleBig3 & small);
	friend CBigDoubleBig3 operator+(const CBigDoubleBig3& num1, const CBigDoubleBig3& num2);
	friend CBigDoubleBig3 operator-(const CBigDoubleBig3& num1, CBigDoubleBig3& num2);
	friend CBigDoubleBig3 operator *(const CBigDoubleBig3 &a, const CBigDoubleBig3 &b);
	friend CBigDoubleBig3 operator /(const CBigDoubleBig3 &a, const CBigDoubleBig3 &b);
	friend std::istream &operator >> (std::istream &stream, CBigDoubleBig3 &num);
	friend std::ostream &operator <<(std::ostream &stream, const CBigDoubleBig3 &num);
	void deleteNumber() {
		free(buf);
	}
	size_t getSize() { return this->size; }

};

#endif BIGDOUBLEBIG3_H