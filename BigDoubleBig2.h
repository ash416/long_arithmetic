#ifndef CBIGDOUBLEBIG2_H
#define CBIGDOUBLEBIG2_H
#include <iostream>
#include <string>


class CBigDoubleBig2 {
public:
	using AtomicT = unsigned char;
private:
	AtomicT* int_part;
	size_t int_size;
	AtomicT* frac_part;
	size_t frac_size;
	bool sign;
public:
	CBigDoubleBig2();
	CBigDoubleBig2(std::string);
	AtomicT &operator[](const size_t);
	AtomicT operator[](const size_t)const;
	friend bool operator ==(const CBigDoubleBig2 &a, const CBigDoubleBig2 &b);
/*	friend bool operator <(const CBigDoubleBig2 &a, const CBigDoubleBig2 &b);
	friend bool operator >(const CBigDoubleBig2 &a, const CBigDoubleBig2 &b);
	friend bool operator <=(const CBigDoubleBig2 &a, const CBigDoubleBig2 &b);
	friend bool operator >=(const CBigDoubleBig2 &a, const CBigDoubleBig2 &b);
	friend bool operator !=(const CBigDoubleBig2 &a, const CBigDoubleBig2 &b);*/
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
	size_t getSize() { return this->int_size + this->frac_size; }

};

#endif BIGDOUBLEBIG2_H