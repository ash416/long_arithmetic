#include "BigDoubleLittle2.h"
#include <algorithm>
CBigDoubleLittle2::CBigDoubleLittle2()
{
}

CBigDoubleLittle2::CBigDoubleLittle2(std::string str)
{
	size_t leng = str.length();
	size_t is_negative = 0;
	if (str[0] == '-') {
		this->sign = false;
		is_negative = 1;
	}
	else {
		this->sign = true;
	}
	size_t point = (size_t)str.find('.');
	this->int_size = point - is_negative;
	this->frac_size = str.length() - point - 1;
	this->int_part = (AtomicT*)malloc(this->int_size);
	this->frac_part = (AtomicT*)malloc(this->frac_size);
	size_t j = this->int_size - 1;
	for (size_t i = is_negative; i < point; i++)
		this->int_part[j--] = str[i] - '0';
	j = this->frac_size - 1;
	for (size_t i = point + 1; i < str.length(); i++)
		this->frac_part[j--] = str[i] - '0';
	
}
CBigDoubleLittle2::AtomicT & CBigDoubleLittle2::operator[](const size_t i)
{
	if (i >= this->frac_size)
		return this->int_part[i - this->frac_size];
	else
		return this->frac_part[i];
}

CBigDoubleLittle2::AtomicT CBigDoubleLittle2::operator[](const size_t i) const
{
	if (i >= this->frac_size)
		return this->int_part[i - this->frac_size];
	else
		return this->frac_part[i];
}

bool operator==(const CBigDoubleLittle2 & a, const CBigDoubleLittle2 & b)
{
	if (a.int_size != b.int_size || a.sign != b.sign || a.frac_size != b.frac_size)
		return false;
	else {
		for (size_t i = 0; i < a.int_size; i++)
			if (a.int_part[i] != b.int_part[i])
				return false;
		for (size_t i = 0; i < a.frac_size; i++)
			if (a.frac_part[i] != b.frac_part[i])
				return false;
		return true;
	}
}

/*bool operator<(const CBigDoubleLittle2 & a, const CBigDoubleLittle2 &b) {
	if (a.sign == b.sign) {
		if (a.size - a.point != b.size - b.point)
			return a.size - a.point > b.size - b.point ? !a.sign : a.sign;
		else {
			for (int i = a.size - 1, j = b.size - 1; i >= 0 && j >= 0; i--, j--) {
				if (a[i] == b[j])
					continue;
				else
					return a.sign ? (a[i] < b[j]) : !(a[i] < b[j]);
			}
			return a.size < b.size ? a.sign : !a.sign;
		}
	}
	else {
		return !a.sign;
	}
}

bool operator>(const CBigDoubleLittle2 & a, const CBigDoubleLittle2 &b) {
	if (a.sign == b.sign) {
		if (a.size - a.point != b.size - b.point)
			return a.size - a.point > b.size - b.point ? a.sign : !a.sign;
		else {
			for (int i = a.size - 1, j = b.size - 1; i >= 0 && j >= 0; i--, j--) {
				if (a[i] == b[j])
					continue;
				else
					return !a.sign ? (a[i] < b[j]) : !(a[i] < b[j]);
			}
			return a.size > b.size ? a.sign : !a.sign;
		}
	}
	else {
		return a.sign;
	}
}

bool operator >=(const CBigDoubleLittle2 &a, const CBigDoubleLittle2 &b) {
	return(!(a < b));
}

bool operator <=(const CBigDoubleLittle2 &a, const CBigDoubleLittle2 &b) {
	return (!(a > b));
}

bool operator !=(const CBigDoubleLittle2 &a, const CBigDoubleLittle2 &b) {
	return (!(a == b));
}
*/


CBigDoubleLittle2 &CBigDoubleLittle2::operator =(const CBigDoubleLittle2 &num) {
	free(this->int_part);
	free(this->frac_part);
	this->int_size = num.int_size;
	this->frac_size = num.frac_size;
	this->int_part = (CBigDoubleLittle2::AtomicT*)malloc(this->int_size);
	this->frac_part = (CBigDoubleLittle2::AtomicT*)malloc(this->frac_size);
	for (size_t i = 0; i < this->int_size; i++)
		this->int_part[i] = num.int_part[i];
	for (size_t i = 0; i < this->frac_size; i++)
		this->frac_part[i] = num.frac_part[i];
	this->sign = num.sign;
	return *this;
}


void CBigDoubleLittle2::differences(CBigDoubleLittle2 & sum, const CBigDoubleLittle2 & big, const CBigDoubleLittle2 & small)
{
	CBigDoubleLittle2::AtomicT carry = 0;
	sum.int_size = std::max(small.int_size, big.int_size);
	sum.frac_size = std::max(small.frac_size, big.frac_size);
	sum.int_part = (CBigDoubleLittle2::AtomicT*)malloc(sum.int_size);
	sum.frac_part = (CBigDoubleLittle2::AtomicT*)malloc(sum.frac_size);
	size_t sum_index = 0;
	for (size_t i = small.frac_size - sum.frac_size, j = big.frac_size - sum.frac_size; i < small.frac_size && j < big.frac_size; i++, j++) {
		if (i < 0) {
			sum.frac_part[sum_index++] = big.frac_part[j];
		}
		else
			if (j < 0) {
				sum.frac_part[sum_index++] = (-small.frac_part[i] + 10 - carry) % 10;
				carry = -small.frac_part[i] - carry >= 0 ? 0 : 1;
			}
			else {
				CBigDoubleLittle2::AtomicT temp = big.frac_part[j] - small.frac_part[i] - carry + 10;
				sum.frac_part[sum_index++] = temp % 10;
				carry = big.frac_part[j] - carry >= small.frac_part[i] ? 0 : 1;
			}
	}
	sum_index = 0;
	for (size_t i = 0, j = 0; sum_index < sum.int_size; i++, j++, sum_index++) {
		if (i >= small.int_size) {
			sum.int_part[sum_index] = (big.int_part[j] - carry + 10) % 10;
			carry = (big.int_part[j] - carry) >= 0 ? 0 : 1;
		}
		else {
			sum.int_part[sum_index] = (big.int_part[j] - carry - small.int_part[i] + 10) % 10;
			carry = (big.int_part[j] - carry >= small.int_part[i] ? 0 : 1);
		}
	}
}

CBigDoubleLittle2 operator+(const CBigDoubleLittle2 & num1, const CBigDoubleLittle2 & num2)
{
	CBigDoubleLittle2 sum;
	int carry = 0;
	if (num1.sign == num2.sign) {
		sum.frac_size = std::max(num1.frac_size, num2.frac_size);
		sum.int_size = std::max(num1.int_size, num2.int_size);
		sum.int_part = (CBigDoubleLittle2::AtomicT*)malloc(sum.int_size);
		sum.frac_part = (CBigDoubleLittle2::AtomicT*)malloc(sum.frac_size);
		int sum_index = 0;
		for (size_t i = num1.frac_size - sum.frac_size, j = num2.frac_size - sum.frac_size; i < num1.frac_size && j < num2.frac_size; i++, j++) {
			if (i < 0) 	sum.frac_part[sum_index++] = num2.frac_part[j];
			else if (j < 0) sum.frac_part[sum_index++] = num1.frac_part[i];
			else {
				size_t temp = num1.frac_part[i] + num2.frac_part[j] + carry;
				sum.frac_part[sum_index++] = temp % 10;
				carry = temp / 10;
			}
		}
		for (size_t i = 0, sum_index = 0; sum_index < sum.int_size; i++,sum_index++) {
			CBigDoubleLittle2::AtomicT temp;
			if (i >= num1.int_size)
				temp = num2.int_part[i] + carry;
			else
				if (i >= num2.int_size)
					temp = num1.int_part[i] + carry;
				else
					temp = num1.int_part[i] + num2.int_part[i] + carry;
			sum.int_part[sum_index] = temp % 10;
			carry = temp / 10;
		}
		if (carry > 0) {
			sum.int_size++;
			sum.int_part = (CBigDoubleLittle2::AtomicT*)realloc(sum.int_part, sum.int_size);
			sum.int_part[sum.int_size - 1] = carry;
		}
		sum.sign = num1.sign;
	}
	else {
		bool num1_is_less;
		bool flag = false;
		if (num1.int_size != num2.int_size)
			num1_is_less = num1.int_size < num2.int_size;
		else {
			size_t i;
			for (i = num1.int_size - 1; i >= 0; i--) {
				if (num1.int_part[i] == num2.int_part[i])
					continue;
				else {
					num1_is_less = num1.int_part[i] < num2.int_part[i];
					flag = true;
					break;
				}
			}
			if (!flag) {
				for (i = num1.frac_size - 1; i >= 0; i--) {
					if (num1.frac_part[i] == num2.frac_part[i])
						continue;
					else {
						num1_is_less = num1.int_part[i] < num2.int_part[i];
						break;
					}
				}
			}
			if (i < 0)
				num1_is_less = num1.frac_size < num2.frac_size;
		}
		if (num1_is_less) sum.differences(sum, num2, num1);
		else sum.differences(sum, num1, num2);
		sum.sign = num1_is_less ? num2.sign : num1.sign;
		flag = false;
		size_t i = sum.int_size - 1;
		while (i > 0 && sum.int_part[i--] == 0) {
			sum.int_size--;
			flag = true;
		}
		if (flag)
			sum.int_part = (CBigDoubleLittle2::AtomicT*)realloc(sum.int_part, sum.int_size);
	}
	return sum;
}


CBigDoubleLittle2 operator-(const CBigDoubleLittle2 & num1, CBigDoubleLittle2 & num2)
{
	num2.sign = false;
	CBigDoubleLittle2 res;
	res = num1 + num2;
	return res;
}

CBigDoubleLittle2 operator*(const CBigDoubleLittle2 &num1, const CBigDoubleLittle2 &num2) {
	CBigDoubleLittle2 result;
	result.int_size = num1.int_size + num2.int_size;
	result.frac_size = num1.frac_size + num2.frac_size;
	result.int_part = (CBigDoubleLittle2::AtomicT*)malloc(result.int_size);
	result.frac_part = (CBigDoubleLittle2::AtomicT*)malloc(result.frac_size);
	for (size_t i = 0; i < std::max(result.int_size, result.frac_size); i++) {
		if (i < result.int_size)
			result.int_part[i] = 0;
		if (i < result.frac_size)
			result.frac_part[i] = 0;
	}
	for (size_t i = 0; i < num2.int_size + num2.frac_size; ++i) {
		if (num2[i] != 0) {
			CBigDoubleLittle2::AtomicT carry = 0;
			for (size_t j = 0; j < num1.int_size + num1.frac_size; ++j) {
				CBigDoubleLittle2::AtomicT temp = num1[j] * num2[i] + result[i + j] + carry;
				result[i + j] = temp % 10;
				carry = temp / 10;
			}
			result[i + num1.int_size + num1.frac_size] = carry;
		}
	}
	bool flag = false;
	size_t i = result.int_size - 1;
	while (i > 0 && result.int_part[i] == 0) {
		result.int_size--; flag = true;
		i--;
	}
	if (flag) {
		result.int_part = (CBigDoubleLittle2::AtomicT*)realloc(result.int_part, result.int_size);
	}

	result.sign = num1.sign == num2.sign ? true : false;
	return result;
}

CBigDoubleLittle2 operator*(const CBigDoubleLittle2 & a, const int & c) {
	if (c == 0)
		return CBigDoubleLittle2("0");
	CBigDoubleLittle2 result;
	int b = c;
	result.sign = (a.sign && b > 0) || (!a.sign && b < 0) ? true : false;
	if (b < 0) b *= (-1);
	CBigDoubleLittle2::AtomicT temp, carry = 0;
	size_t sizeB = 0;
	for (size_t i = b; i > 0; i = i / 10, sizeB++);
	result.int_size = a.int_size + sizeB;
	result.frac_size = a.frac_size;
	result.int_part = (CBigDoubleLittle2::AtomicT*)malloc(result.int_size);
	result.frac_part = (CBigDoubleLittle2::AtomicT*)malloc(result.frac_size);
	for (size_t i = 0; i < std::max(result.int_size, result.frac_size); i++) {
		if (i < result.int_size)
			result.int_part[i] = 0;
		if (i < result.frac_size)
			result.frac_part[i] = 0;
	}
	size_t i;
	for (i = 0; i < a.int_size + a.frac_size; i++) {
		temp = a[i] * b + carry;
		carry = temp / 10;
		result[i] = temp - carry * 10;
	}
	while (carry > 0) {
		result[i++] = carry % 10;
		carry /= 10;
	}

	return result;
}

CBigDoubleLittle2 operator/(const CBigDoubleLittle2 & a, const CBigDoubleLittle2 & b)
{
	CBigDoubleLittle2 q;
	q.sign = a.sign == b.sign ? true : false;
	CBigDoubleLittle2 copyA;
	size_t accuracy = 10;
	copyA.int_size = a.int_size + a.frac_size + 1 + (b.frac_size - a.frac_size > 0 ? b.frac_size - a.frac_size : 0) + accuracy;
	copyA.int_part = (CBigDoubleLittle2::AtomicT*)malloc(copyA.int_size);
	size_t j;
	for (j = 0; j < (b.frac_size - a.frac_size > 0 ? b.frac_size - a.frac_size : 0) + accuracy; j++)
		copyA[j] = 0;
	for (size_t i = 0; i < a.int_size + a.frac_size; i++)
		copyA[j++] = a[i];
	size_t a_size = a.int_size + a.frac_size + (b.frac_size - a.frac_size > 0 ? b.frac_size - a.frac_size : 0);
	if (copyA[j - 1] == 0) {
		a_size--;
		j--;
		copyA.int_size--;
	}
	copyA[j] = 0;
	size_t shift = 0;
	CBigDoubleLittle2 copyB;
	copyB.int_size = b.int_size + b.frac_size;
	copyB.int_part = (CBigDoubleLittle2::AtomicT*)malloc(copyB.int_size);
	for (size_t i = 0; i < copyB.int_size; i++) {
		copyB.int_part[i] = b[i];
	}
	CBigDoubleLittle2::AtomicT scale;
	CBigDoubleLittle2::AtomicT qGuess, r;
	CBigDoubleLittle2::AtomicT borrow, carry;
	size_t diff = a.int_size - b.int_size;
	size_t n = b.int_size + b.frac_size, m = a_size - b.int_size - b.frac_size + accuracy;
	scale = 10 / (b[n - 1] + 1);
	if (scale > 1) {
		copyA = copyA * scale;
		if (copyA[copyA.int_size - 1] == 0) {
			copyA.int_size--;
		}
		copyB = copyB * scale;
		if (copyB[copyB.int_size - 1] == 0) {
			copyB.int_size--;
		}
	}
	if (copyA.int_part[copyA.int_size - 1] != 0)
		diff++;
	else
		diff += (copyA.int_part[copyA.int_size - 2] >= copyB.int_part[copyB.int_size - 1] ? 1 : 0);

	size_t new_m = m;
	q.frac_size = m - diff + 1;
	q.int_size = diff;
	q.int_part = (CBigDoubleLittle2::AtomicT*)malloc(q.int_size);
	q.frac_part = (CBigDoubleLittle2::AtomicT*)malloc(q.frac_size);


	size_t q_ind, a_ind;
	for (q_ind = new_m, a_ind = a_size + accuracy; q_ind >= 0; q_ind--, a_ind--) {
		if (q_ind + shift == q.frac_size - 1 - accuracy) {
			shift++;
			break;
		}
		CBigDoubleLittle2::AtomicT temp = (copyA[a_ind] * 10 + copyA[a_ind - 1]);
		qGuess = temp / copyB[n - 1];
		r = temp % copyB[n - 1];
		while (r < 10) {
			if (copyB[n - 2] * qGuess > r * 10 + copyA[a_ind - 2] || qGuess == 10) {
				qGuess--;
				r += copyB[n - 1];
			}
			else break;
		}
		carry = 0;
		borrow = 0;
		CBigDoubleLittle2::AtomicT *aShift = copyA.int_part + q_ind + (m - new_m);

		CBigDoubleLittle2::AtomicT temp1, temp2;
		for (size_t i = 0; i < n; i++) {
			temp1 = copyB[i] * qGuess + carry;
			carry = temp1 / 10;
			temp1 -= carry * 10;
			temp2 = aShift[i] - temp1 + borrow;
			if (temp2 < 0) {
				aShift[i] = temp2 + 10;
				borrow = -1;
			}
			else {
				aShift[i] = temp2;
				borrow = 0;
			}
		}

		temp2 = aShift[n] - carry + borrow;
		if (temp2 < 0) {
			aShift[n] = temp2 + 10;
			borrow = -1;
		}
		else {
			aShift[n] = temp2;
			borrow = 0;
		}
		if (borrow == 0) {
			if (q_ind + shift == new_m && qGuess == 0) {
				if (copyA.int_part[copyA.int_size - 2] >= copyB.int_part[copyB.int_size - 1]) {
					q.frac_size++;
					q.int_size--;
					q.frac_part = (CBigDoubleLittle2::AtomicT*)realloc(q.frac_part, q.frac_size);
					q.int_part = (CBigDoubleLittle2::AtomicT *)realloc(q.int_part, q.int_size);
				}
				shift++;
				continue;
			}
			q[q_ind + shift] = qGuess;
		}
		else {
			if (q_ind + shift == new_m && qGuess - 1 == 0) {
				if (copyA.int_part[copyA.int_size - 2] >= copyB.int_part[copyB.int_size - 1]) {
					q.frac_size++;
					q.int_size--;
					q.frac_part = (CBigDoubleLittle2::AtomicT *)realloc(q.frac_part, q.frac_size );
					q.int_part = (CBigDoubleLittle2::AtomicT *)realloc(q.int_part, q.int_size );
				}
				shift++;
			}
			else {
				q[q_ind + shift] = qGuess - 1;
			}
			carry = 0;
			for (size_t i = 0; i < n; i++) {
				temp = aShift[i] + copyB[i] + carry;
				if (temp >= 10) {
					aShift[i] = temp - 10;
					carry = 1;
				}
				else {
					aShift[i] = temp;
					carry = 0;
				}
			}
			aShift[n] += carry - 10;
		}
	}
	copyA.deleteNumber();
	copyB.deleteNumber();
	if (shift != 0) {
		size_t sh = q.frac_size - accuracy;
		for (size_t i = 0; i < accuracy; i++)
			q[i] = q[i + sh];
		q.frac_size -= sh;
		q.frac_part = (CBigDoubleLittle2::AtomicT*)realloc(q.frac_part, q.frac_size);
	}

	return q;
}

std::istream &operator >> (std::istream &stream, CBigDoubleLittle2 &num) {
	std::string str;
	stream >> str;
	CBigDoubleLittle2 _int(str);
	num = _int;
	return stream;
}
std::ostream &operator<<(std::ostream &stream, const CBigDoubleLittle2 &num) {
	if (!num.sign)
		stream << '-';
	for (int i = num.int_size - 1; i >= 0; i--) {
		stream << num.int_part[i];
	}
	stream << '.';
	for (int i = num.frac_size - 1; i >= 0; i--) {
		stream << num.frac_part[i];
	}
	return stream;
}
