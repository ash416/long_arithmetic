#include "BigDoubleBig.h"
#include <ctime>

CBigDoubleBig::CBigDoubleBig()
{
}

CBigDoubleBig::CBigDoubleBig(std::string str)
{
	size_t  j = 0;
	if (str[0] == '-') {
		this->sign = false;
		this->size = str.length() - 2;
		j = 1;
	}
	else {
		this->sign = true;
		this->size = str.length() - 1;
	}
	this->buf = (AtomicT*)malloc(size);
	for (size_t  i = 0; i < size; i++) {
		if (str[j] == '.') {
			j++;
			i--;
			this->point = i + 1;
			continue;
		}
		buf[i] = (str[j++] - '0');
	}
}

/*int & CBigDoubleBig::operator[](const int i)
{
	return buf[i];
}

int CBigDoubleBig::operator[](const int i) const
{
	return buf[i];
}*/

bool operator==(const CBigDoubleBig & a, const CBigDoubleBig & b)
{
	if (a.size != b.size || a.sign != b.sign || a.point != b.point)
		return false;
	else {
		for (int i = 0; i < (int)a.size; i++)
			if (a.buf[i] != b.buf[i])
				return false;
		return true;
	}
}
/*
bool operator<(const CBigDoubleBig & a, const CBigDoubleBig &b) {
	if (a.sign == b.sign) {
		if (a.point != b.point)
			return a.point > b.point ? !a.sign : a.sign;
		else { 
			for (int i = 0; i < (a.size <= b.size ? a.size : b.size); i++) {
				if (a[i] == b[i])
					continue;
				else
					return a.sign ? (a[i] < b[i]) : !(a[i] < b[i]);
			}
			return a.size < b.size ? a.sign : !a.sign;
		}
	}
	else {
		return !a.sign;
	}
}

bool operator>(const CBigDoubleBig & a, const CBigDoubleBig &b) {
	if (a.sign == b.sign) {
		if (a.point != b.point)
			return a.point < b.point ? !a.sign : a.sign;
		else { 
			for (int i = 0; i < (a.size <= b.size ? a.size : b.size); i++) {
				if (a[i] == b[i])
					continue;
				else
					return !a.sign ? (a[i] < b[i]) : !(a[i] < b[i]);
			}
			return a.size > b.size ? a.sign : !a.sign;
		}
	}
	else {
		return a.sign;
	}
}

bool operator >=(const CBigDoubleBig &a, const CBigDoubleBig &b) {
	return(!(a < b));
}

bool operator <=(const CBigDoubleBig &a, const CBigDoubleBig &b) {
	return (!(a > b));
}

bool operator !=(const CBigDoubleBig &a, const CBigDoubleBig &b) {
	return (!(a == b));
}
*/


CBigDoubleBig &CBigDoubleBig::operator =(const CBigDoubleBig &num) {
	free(this->buf);
	this->buf = (AtomicT*)malloc(num.size);
	for (size_t  i = 0; i < num.size; i++)
		this->buf[i] = num.buf[i];
	this->point = num.point;
	this->sign = num.sign;
	this->size = num.size;
	return *this;
}

void CBigDoubleBig::differences(CBigDoubleBig & sum, const CBigDoubleBig & big, const CBigDoubleBig & small)
{
	//clock_t t = std::clock();
	CBigDoubleBig::AtomicT carry = 0;
	size_t  frac1 = big.size - big.point, frac2 = small.size - small.point;
	size_t  frac_size = frac1 > frac2 ? frac1 : frac2;
	sum.point = small.point > big.point ? small.point : big.point;
	sum.size = frac_size + (small.point > big.point ? small.point : big.point);
	sum.buf = (CBigDoubleBig::AtomicT*)malloc(sum.size);
	int  sum_index = (int)sum.size - 1;
	//std::cout << " 1: " << std::clock() - t << " ";
	//t = std::clock();
	for (int  i = (int)small.point + (int)frac_size - 1, j = (int)big.point + (int)frac_size - 1; i >= (int)small.point, j >= (int)big.point; i--, j--) {
		if (i >= (int)small.size) sum.buf[sum_index--] = big.buf[j];
		else if (j >= (int)big.size) {
			sum.buf[sum_index--] = (-small.buf[i] + 10 - carry) % 10;
			carry = -small.buf[i] - carry >= 0 ? 0 : 1;
		}
		else {
			CBigDoubleBig::AtomicT temp = big.buf[j] - small.buf[i] - carry + 10;
			sum.buf[sum_index--] = temp % 10;
			carry = big.buf[j] - carry >= small.buf[i] ? 0 : 1;
		}
	}
	//std::cout << "2: " << std::clock() - t << " ";
	//t = std::clock();
	for (int  i = (int)small.point - 1, j = (int)big.point - 1; sum_index >= 0; i--, j--, sum_index--) {
		if (i < 0) {
			sum.buf[sum_index] = (big.buf[j] - carry + 10) % 10;
			carry = (big.buf[j] - carry) >= 0 ? 0 : 1;
		}
		else {
			sum.buf[sum_index] = (big.buf[j] - carry - small.buf[i] + 10) % 10;
			carry = (big.buf[j] - carry >= small.buf[i] ? 0 : 1);
		}
	}
	//std::cout << "3: " << std::clock() - t << " ";
}

CBigDoubleBig operator+(const CBigDoubleBig & num1, const CBigDoubleBig & num2)
{
	CBigDoubleBig sum;
	CBigDoubleBig::AtomicT carry = 0;
	if (num1.sign == num2.sign) {
		size_t  frac1 = num1.size - num1.point, frac2 = num2.size - num2.point;
		size_t  frac_size = frac1 > frac2 ? frac1 : frac2;
		sum.point = num1.point > num2.point ? num1.point : num2.point;
		sum.size = frac_size + (num1.point > num2.point ? num1.point : num2.point);
		sum.buf = (CBigDoubleBig::AtomicT*)malloc(sum.size);
		int sum_index = (int)sum.size - 1;
		for (size_t i = num1.point + frac_size - 1, j = num2.point + frac_size - 1; i >= num1.point, j >= num2.point; i--, j--) {
			if (i >= num1.size) sum.buf[sum_index--] = num2.buf[j];
			else if (j >= num2.size) sum.buf[sum_index--] = num1.buf[i];
			else {
				CBigDoubleBig::AtomicT temp = num1.buf[i] + num2.buf[j] + carry;
				sum.buf[sum_index--] = temp % 10;
				carry = temp / 10;
			}
		}
		for(int  i = (int)num1.point - 1, j = (int)num2.point - 1; sum_index >= 0; i--, j--, sum_index--){
			CBigDoubleBig::AtomicT temp;
			if (i < 0)
				temp = num2.buf[j] + carry;
			else
				if (j < 0)
					temp = num1.buf[i] + carry;
				else
					temp = num1.buf[i] + num2.buf[j] + carry;
			sum.buf[sum_index] = temp % 10;
			carry = temp / 10;
		}
		if (carry > 0) {
			sum.buf = (CBigDoubleBig::AtomicT*)realloc(sum.buf, (sum.size + 1));
			for (int i = (int)sum.size; i > 0; i--)
				sum.buf[i] = sum.buf[i - 1];
			sum.point++;
			sum.buf[0] = carry;
			sum.size++;
		}
		sum.sign = num1.sign;
	}
	else {
		//clock_t t = std::clock(); 
		bool num1_is_less;
		if (num1.size != num2.size)
			num1_is_less = num1.size > num2.size ? false : true;
		else {
			size_t  i;
			for (i = 0; i < num1.size; i++) {
				if (num1.buf[i] == num2.buf[i])
					continue;
				else {
					num1_is_less = (num1.buf[i] < num2.buf[i]);
					break;
				}
			}
			if (i == num1.size)
				num1_is_less = false;
		}
		//std::cout << "1: " << std::clock() - t << " ";
		//t = std::clock();
		if (num1_is_less) sum.differences(sum, num2, num1);
		else sum.differences(sum, num1, num2);
		//std::cout << "2: " << std::clock() - t << " ";
		//t = std::clock();
		sum.sign = num1_is_less ? num2.sign : num1.sign;
		size_t i = 0;
		while (i < sum.point - 1 && sum.buf[i] == 0) {
			sum.size--; i++;
		}
		if (i > 0) {
			std::cout << " --- ";
			for (size_t j = 0; j < sum.size; j++)
				sum.buf[j] = sum.buf[j + i];
			sum.buf = (CBigDoubleBig::AtomicT*)realloc(sum.buf, sum.size);
			sum.point -= i;
		}
		//std::cout << "3: " << std::clock() - t << " ";
	}
	return sum;
}

CBigDoubleBig operator-(const CBigDoubleBig & num1, CBigDoubleBig & num2)
{
	num2.sign = false;
	CBigDoubleBig res;
	res = num1 + num2;
	return res;
}

CBigDoubleBig operator*(const CBigDoubleBig &num1, const CBigDoubleBig &num2) {
	CBigDoubleBig result;
	result.size = num1.size + num2.size;
	result.buf = (CBigDoubleBig::AtomicT*)malloc(result.size);
	for (size_t  i = 0; i < result.size; i++)
		result.buf[i] = 0;
	for (int  i = (int)num2.size - 1; i >= 0; i--) {
		if (num2.buf[i] != 0) {
			CBigDoubleBig::AtomicT carry = 0;
			for (int  j = (int)num1.size - 1; j >= 0; j--) {
				CBigDoubleBig::AtomicT  temp = num1.buf[j] * num2.buf[i] + result.buf[i + j + 1] + carry;
				result.buf[i + j + 1] = temp % 10;
				carry = temp / 10;
			}
			result.buf[i] = carry;
		}
	}
	result.point = result.size - (num1.size + num2.size - num1.point - num2.point);
	size_t i = 0;
	while (i < result.point - 1 && result.buf[i] == 0) {
		result.size--;
		i++;
	}
	if (i > 0) {
		for (size_t  j = 0; j <= result.size - 1; j++) {
			result.buf[j] = result.buf[j + i];
		}
		result.buf = (CBigDoubleBig::AtomicT*)realloc(result.buf, result.size);
		result.point -= i;
	}
	result.sign = num1.sign == num2.sign ? true : false;
	return result;
}

CBigDoubleBig operator*(const CBigDoubleBig & a, const int & c) {
	if (c == 0)
		return CBigDoubleBig("0");
	CBigDoubleBig result;
	int b = c;
	result.sign = (a.sign && b > 0) || (!a.sign && b < 0) ? true : false;
	if (b < 0) b *= (-1);
	CBigDoubleBig::AtomicT temp, carry = 0;
	size_t  sizeB = 0;
	for (int  i = b; i > 0; i = i / 10, sizeB++);
	result.size = a.size + sizeB;
	result.buf = (CBigDoubleBig::AtomicT*)malloc(result.size);
	for (size_t i = 0; i < result.size; i++)
		result.buf[i] = 0;
	size_t  i;
	for (i = result.size - 1; i >= sizeB; i--) {
		temp = a.buf[i - sizeB] * b + carry;
		carry = temp / 10;
		result.buf[i] = temp - carry * 10;
	}
	while (carry > 0) {
		result.buf[i--] = carry % 10;
		carry /= 10;
	}
	return result;
}


CBigDoubleBig operator/(const CBigDoubleBig & a, const CBigDoubleBig & b)
{
	CBigDoubleBig q;
	q.sign = a.sign == b.sign ? true : false;
	CBigDoubleBig copyA;
	size_t  accuracy = 10;
	size_t  frac_diff = b.size - b.point - (a.size - a.point);
	copyA.size = a.size + 1 + (frac_diff > 0 ? frac_diff : 0) + accuracy;
	copyA.buf = (CBigDoubleBig::AtomicT*)malloc(copyA.size);
	copyA.buf[0] = 0;
	size_t  i, j;
	for (i = 1; i <= a.size; i++)
		copyA.buf[i] = a.buf[i - 1];
	for (i = a.size + 1; i < copyA.size; i++)
		copyA.buf[i] = 0;
	size_t a_size = a.size + frac_diff;
	
	size_t shift = 0;
	CBigDoubleBig copyB;
	copyB = b;

	CBigDoubleBig::AtomicT scale;
	CBigDoubleBig::AtomicT qGuess, r;
	CBigDoubleBig::AtomicT borrow, carry;
	size_t  diff = a.point - b.point;
	size_t  n = b.size, m = a_size - b.size + accuracy;
	scale = 10 / (b.buf[0] + 1);
	i = 0; j = 0;
	if (scale > 1) {
		copyA = copyA * scale;
		if (copyA.buf[i] == 0 && copyA.size > 1) {
			copyA.size--;
			i++;
		}
		copyB = copyB * scale;
		if (copyB.buf[j] == 0 && copyB.size > 1) {
			copyB.size--;
			j++;
		}
	}
	if (copyA.buf[0] != 0)
		diff++;
	else
		diff += (copyA.buf[1] >= copyB.buf[0] ? 1 : 0);
	CBigDoubleBig::AtomicT *aShift = copyA.buf + i;
	CBigDoubleBig::AtomicT *bShift = copyB.buf + j;
	q.size = m + 1;
	q.buf = (CBigDoubleBig::AtomicT*)malloc(q.size);
	q.point = diff;
	
	size_t q_ind, a_ind;
	for (q_ind = 0, a_ind = 0; q_ind <= m; q_ind++, a_ind++) {
		if (q_ind - shift == q.point + accuracy) {
			shift++;
			break;
		}
		CBigDoubleBig::AtomicT temp = (aShift[a_ind] * 10 + aShift[a_ind + 1]);
		qGuess = temp / bShift[0];
		r = temp % bShift[0];
		while (r < 10) {
			if (bShift[1] * qGuess > r * 10 + aShift[a_ind + 2] || qGuess == 10) {
				qGuess--;
				r += bShift[0];
			}
			else break;
		}
		carry = 0;
		borrow = 0;

		CBigDoubleBig::AtomicT temp1, temp2;
		for (size_t i = n - 1; i >= 0; i--) {
			temp1 = bShift[i] * qGuess + carry;
			carry = temp1 / 10;
			temp1 -= carry * 10;
			temp2 = aShift[q_ind + i + 1] - temp1 + borrow;
			if (temp2 < 0) {
				aShift[q_ind + i + 1] = temp2 + 10;
				borrow = -1;
			}
			else {
				aShift[q_ind + i + 1] = temp2;
				borrow = 0;
			}
		}
		temp2 = aShift[q_ind] - carry + borrow;
		if (temp2 < 0) {
			aShift[q_ind] = temp2 + 10;
			borrow = -1;
		}
		else {
			aShift[q_ind] = temp2;
			borrow = 0;
		}
		if (borrow == 0) {
			if (q_ind - shift == 0 && qGuess == 0) {
				if (copyA.buf[1] >= copyB.buf[0])
					q.point--;
				shift++;
				continue;
			}
			q.buf[q_ind - shift] = qGuess;
		}
		else {
			if (q_ind - shift == 0 && qGuess - 1 == 0) {
				if (copyA.buf[1] >= copyB.buf[0])
					q.point--;
				shift++;
			}
			else {
				q.buf[q_ind - shift] = qGuess - 1;
			}
			carry = 0;
			for (int i = (int)n - 1; i >= 0; i--) {
				temp = aShift[q_ind + i + 1] + bShift[i] + carry;
				if (temp >= 10) {
					aShift[q_ind + i + 1] = temp - 10;
					carry = 1;
				}
				else {
					aShift[q_ind + i + 1] = temp;
					carry = 0;
				}
			}
			aShift[q_ind] += carry - 10;
		}
	}
	copyA.deleteNumber();
	copyB.deleteNumber();

	m -= shift;
	q.size = m + 1;
	if (shift != 0) {
		q.buf = (CBigDoubleBig::AtomicT*)realloc(q.buf, q.size);
	}

	return q;
}

std::istream &operator >> (std::istream &stream, CBigDoubleBig &num) {
	std::string str;
	stream >> str;
	CBigDoubleBig _int(str);
	num = _int;
	return stream;
}
std::ostream &operator<<(std::ostream &stream, const CBigDoubleBig &num) {
	if (!num.sign)
		stream << '-';
	for (size_t i = 0; i < num.size; i++) {
		if (i == num.point)
			stream << ".";
		stream << (int)num.buf[i];
	}
	return stream;
}
