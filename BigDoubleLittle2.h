#ifndef CBIGDOUBLELITTLE2_H
#define CBIGDOUBLELITTLE2_H
#include <iostream>
#include <string>


class CBigDoubleLittle2 {
public:
	using AtomicT = unsigned char;
private:
	AtomicT* int_part;
	size_t int_size;
	AtomicT* frac_part;
	size_t frac_size;
	bool sign;
public:
	CBigDoubleLittle2();
	CBigDoubleLittle2(std::string);
	AtomicT &operator[](const size_t);
	AtomicT operator[](const size_t)const;
	friend bool operator ==(const CBigDoubleLittle2 &a, const CBigDoubleLittle2 &b);
//	friend bool operator <(const CBigDoubleLittle2 &a, const CBigDoubleLittle2 &b);
//	friend bool operator >(const CBigDoubleLittle2 &a, const CBigDoubleLittle2 &b);
//	friend bool operator <=(const CBigDoubleLittle2 &a, const CBigDoubleLittle2 &b);
//	friend bool operator >=(const CBigDoubleLittle2 &a, const CBigDoubleLittle2 &b);
//	friend bool operator !=(const CBigDoubleLittle2 &a, const CBigDoubleLittle2 &b);
	CBigDoubleLittle2 &operator =(const CBigDoubleLittle2 &num);
	friend CBigDoubleLittle2 operator*(const CBigDoubleLittle2 & a, const int & b);
	void differences(CBigDoubleLittle2 & res, const CBigDoubleLittle2 & big, const CBigDoubleLittle2 & small);
	friend CBigDoubleLittle2 operator+(const CBigDoubleLittle2& num1, const CBigDoubleLittle2& num2);
	friend CBigDoubleLittle2 operator-(const CBigDoubleLittle2& num1, CBigDoubleLittle2& num2);
	friend CBigDoubleLittle2 operator *(const CBigDoubleLittle2 &a, const CBigDoubleLittle2 &b);
	friend CBigDoubleLittle2 operator /(const CBigDoubleLittle2 &a, const CBigDoubleLittle2 &b);
	friend std::istream &operator >> (std::istream &stream, CBigDoubleLittle2 &num);
	friend std::ostream &operator <<(std::ostream &stream, const CBigDoubleLittle2 &num);
	void deleteNumber() {
		free(int_part);
		free(frac_part);
	}
	//int getSizeInt() { return size - point - 1; }
	//int getSizeFrac() { return point + 1; }
	size_t getSize() { return this->int_size + this->frac_size; }

};

#endif BIGDOUBLELITTLE2_H