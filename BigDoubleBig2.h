#ifndef CBIGDOUBLEBIG2_H
#define CBIGDOUBLEBIG2_H
#include <iostream>
#include <string>


class CBigDoubleBig2 {
	int* int_part;
	int int_size;
	int* frac_part;
	int frac_size;
	bool sign;
public:
	CBigDoubleBig2();
	CBigDoubleBig2(std::string);
	int &operator[](const int);
	int operator[](const int)const;
	friend bool operator ==(const CBigDoubleBig2 &a, const CBigDoubleBig2 &b);
	friend bool operator <(const CBigDoubleBig2 &a, const CBigDoubleBig2 &b);
	friend bool operator >(const CBigDoubleBig2 &a, const CBigDoubleBig2 &b);
	friend bool operator <=(const CBigDoubleBig2 &a, const CBigDoubleBig2 &b);
	friend bool operator >=(const CBigDoubleBig2 &a, const CBigDoubleBig2 &b);
	friend bool operator !=(const CBigDoubleBig2 &a, const CBigDoubleBig2 &b);
	CBigDoubleBig2 &operator =(const CBigDoubleBig2 &num);
	friend CBigDoubleBig2 operator*(const CBigDoubleBig2 & a, const int & b);
	void differences(CBigDoubleBig2 & res, const CBigDoubleBig2 & big, const CBigDoubleBig2 & small);
	friend CBigDoubleBig2 operator+(const CBigDoubleBig2& num1, const CBigDoubleBig2& num2);
	friend CBigDoubleBig2 operator-(const CBigDoubleBig2& num1, CBigDoubleBig2& num2);
	friend CBigDoubleBig2 operator *(const CBigDoubleBig2 &a, const CBigDoubleBig2 &b);
	friend CBigDoubleBig2 operator /(const CBigDoubleBig2 &a, const CBigDoubleBig2 &b);
	friend std::istream &operator >> (std::istream &stream, CBigDoubleBig2 &num);
	friend std::ostream &operator <<(std::ostream &stream, const CBigDoubleBig2 &num);
	void deleteNumber() {
		free(int_part);
		free(frac_part);
	}
	/*int getSizeInt() { return size - point - 1; }
	int getSizeFrac() { return point + 1; }*/
	int getSize() { return this->int_size + this->frac_size; }

};

#endif BIGDOUBLEBIG2_H