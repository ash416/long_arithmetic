#include "BigIntLittleBin.h"
#include "BigIntLittle.h"
#include <math.h>
#include <vector>

CBigIntLittleBin::CBigIntLittleBin()
{
	this->buf = new short[2];
	this->buf[0] = 0;
	this->buf[0] = 0;
	this->size = 1;
}

short SDiv(std::string &str, const short b) {
	short r = 0;
	int temp;
	for (size_t i = 0; i < str.size(); i++) {
		temp = r * 10 + int(str[i] - '0');
		str[i] = (char(temp / b) + '0');
		r = temp - temp / b * b;
	}
	int size = str.size();
	size_t i = 0;
	while (size > 1 && str[i] == char(0 + '0')) {
		size--;
		i++;
	}
	while (i-- > 0)
		str.erase(str.begin());
	return r;
}
CBigIntLittleBin::CBigIntLittleBin(std::string str)
{
	std::vector<int> vec;
	this->sign = true;
	if (str[0] == '-') {
		sign = false;
		str.erase(str.begin());
	}
	std::cout << str << std::endl;
	while (!((str[0] == char(0 + '0') || str[0] == char(1 + '0')) && str.size() == 1))
		vec.emplace_back(SDiv(str, 2));
	vec.emplace_back(int(str[0] - '0'));
	this->size = vec.size();
	this->buf = (short*)malloc((this->size) * sizeof(short));
	for (int i = 0; i < this->size; i++) {
		this->buf[i] = vec[i];
	}
}

short & CBigIntLittleBin::operator[](const int i)
{
	return buf[i];
}

short CBigIntLittleBin::operator[](const int i) const
{
	return buf[i];
}

CBigIntLittleBin & CBigIntLittleBin::operator=(const CBigIntLittleBin & num)
{
	free(this->buf);
	this->buf = (short*)malloc(num.size * sizeof(short));
	for (int i = 0; i < num.size; i++)
		this->buf[i] = num[i];
	this->sign = num.sign;
	this->size = num.size;
	return *this;
}
/*
bool operator==(const CBigIntLittleBin & a, const CBigIntLittleBin & b)
{
	return false;
}

bool operator<(const CBigIntLittleBin & a, const CBigIntLittleBin & b)
{
	return false;
}

bool operator>(const CBigIntLittleBin & a, const CBigIntLittleBin & b)
{
	return false;
}

bool operator<=(const CBigIntLittleBin & a, const CBigIntLittleBin & b)
{
	return false;
}

bool operator>=(const CBigIntLittleBin & a, const CBigIntLittleBin & b)
{
	return false;
}

bool operator!=(const CBigIntLittleBin & a, const CBigIntLittleBin & b)
{
	return false;
}

CBigIntLittleBin operator*(const CBigIntLittleBin & a, const int & b)
{
	return CBigIntLittleBin();
}

CBigIntLittleBin operator+(const CBigIntLittleBin & num1, const CBigIntLittleBin & numBin)
{
	return CBigIntLittleBin();
}

CBigIntLittleBin operator-(const CBigIntLittleBin & num1, CBigIntLittleBin & numBin)
{
	return CBigIntLittleBin();
}

CBigIntLittleBin operator*(const CBigIntLittleBin & a, const CBigIntLittleBin & b)
{
	return CBigIntLittleBin();
}

CBigIntLittleBin operator/(const CBigIntLittleBin & a, const CBigIntLittleBin & b)
{
	return CBigIntLittleBin();
}
*/
std::istream & operator >> (std::istream & stream, CBigIntLittleBin & num)
{
	std::string str;
	stream >> str;
	CBigIntLittleBin _int(str);
	num = _int;
	return stream;
}

std::ostream & operator<<(std::ostream & stream, const CBigIntLittleBin & num)
{
	/*CBigIntLittle a("0");
	for (int i = 0; i < num.size; i++) {
		if (num[i] != 0) {
			CBigIntLittle b(std::to_string((int)pow(2, i)));
			a = a + b;
		}
	}
	if (!num.sign)
		stream << '-';
	for (int i = a.getSize() - 1; i >= 0; i--) {
		stream << a[i];
	}*/
	return stream;
}
