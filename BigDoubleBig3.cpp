#include "BigDoubleBig3.h"

#include <ctime>

CBigDoubleBig3::CBigDoubleBig3()
{
}

CBigDoubleBig3::CBigDoubleBig3(std::string str)
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
	this->size = leng - 1 - is_negative;
	size_t i = is_negative;
	if (str[i] == char(0 + '0')) {
		this->order = 0;
		while (str[i] == char(0 + '0') || str[i] == '.') {
			if (str[i] == '.') {
				i++; continue;
			}
			this->size--;
			this->order--;
			i++;
		}
	}
	this->buf = (CBigDoubleBig3::AtomicT*)malloc(this->size);
	size_t j = 0;
	for (; i < leng; i++) {
		if (str[i] == '.') {
			this->order = i - is_negative - 1;
			continue;
		}
		this->buf[j++] = str[i] - '0';
	}
}

/*int & CBigDoubleBig3::operator[](const int i)
{
	return buf[i];
}

int CBigDoubleBig3::operator[](const int i) const
{
	return buf[i];
}*/

bool operator==(const CBigDoubleBig3 & a, const CBigDoubleBig3 & b)
{
	if (a.size != b.size || a.sign != b.sign || a.order != b.order)
		return false;
	else {
		for (size_t i = 0; i < a.size; i++)
			if (a.buf[i] != b.buf[i])
				return false;
		return true;
	}
}



CBigDoubleBig3 &CBigDoubleBig3::operator =(const CBigDoubleBig3 &num) {
	free(this->buf);
	this->buf = (CBigDoubleBig3::AtomicT*)malloc(num.size);
	for (size_t i = 0; i < num.size; i++)
		this->buf[i] = num.buf[i];
	this->order = num.order;
	this->sign = num.sign;
	this->size = num.size;
	return *this;
}

void CBigDoubleBig3::differences(CBigDoubleBig3 & sum, const CBigDoubleBig3 & big, const CBigDoubleBig3 & small)
{
	CBigDoubleBig3::AtomicT carry = 0;
	sum.order = big.order;
	size_t small_size = small.size + sum.order - small.order;
	sum.size = big.size > small_size ? big.size : small_size;
	sum.buf = (CBigDoubleBig3::AtomicT*)malloc(sum.size);
	size_t sum_index = sum.size - 1;
	for (size_t i = sum.size - 1 - (sum.order - small.order), j = sum.size - 1 - (sum.order - big.order); sum_index > 0; i--, j--, sum_index--) {
		if (i >= small.size) sum.buf[sum_index] = big.buf[j];
		else if (j >= big.size) {
			if (i < 0) {
				sum.buf[sum_index] = (10 - carry) % 10;
				carry = -carry >= 0 ? 0 : 1;
			}
			else {
				sum.buf[sum_index] = (-small.buf[i] + 10 - carry) % 10;
				carry = -small.buf[i] - carry >= 0 ? 0 : 1;
			}
		}
		else if (i < 0) {
			CBigDoubleBig3::AtomicT temp = big.buf[j]  - carry + 10;
			sum.buf[sum_index] = temp % 10;
			carry = big.buf[j] - carry >= 0 ? 0 : 1;
		}
		else {
			CBigDoubleBig3::AtomicT temp = big.buf[j] - small.buf[i] - carry + 10;
			sum.buf[sum_index] = temp % 10;
			carry = big.buf[j] - carry >= small.buf[i] ? 0 : 1;
		}
	}
	if (small.size != small_size) {
		sum.buf[sum_index] = (big.buf[0] - carry + 10) % 10;
		carry = (big.buf[0] - carry) >= 0 ? 0 : 1;
	}
	else {
		sum.buf[sum_index] = (big.buf[0] - carry - small.buf[0] + 10) % 10;
		carry = (big.buf[0] - carry >= small.buf[0] ? 0 : 1);
	}
}

CBigDoubleBig3 operator+(const CBigDoubleBig3 & num1, const CBigDoubleBig3 & num2)
{
	CBigDoubleBig3 sum;
	CBigDoubleBig3::AtomicT carry = 0;
	if (num1.sign == num2.sign) {
		sum.order = num1.order > num2.order ? num1.order : num2.order;
		size_t num1_size = num1.size + sum.order - num1.order;
		size_t num2_size = num2.size + sum.order - num2.order;
		sum.size = num1_size > num2_size ? num1_size : num2_size;
		sum.buf = (CBigDoubleBig3::AtomicT*)malloc(sum.size);
		size_t sum_index = sum.size - 1;
		//clock_t t;
		//t = std::clock();
		for (int i = (int)sum.size - 1 - (sum.order - num1.order), j = (int)sum.size - 1 - (sum.order - num2.order); sum_index > 0; i--, j--, sum_index--) {
			if (i >= (int)num1.size) {
				if (j < 0)
					sum.buf[sum_index] = 0;
				else
					sum.buf[sum_index] = num2.buf[j];
			}
			else if (j >= (int)num2.size) {
				if (i < 0)
					sum.buf[sum_index] = 0;
				else
					sum.buf[sum_index] = num1.buf[i];
			}
			else {
				if (i < 0) {
					CBigDoubleBig3::AtomicT temp = num2.buf[i] + carry;
					sum.buf[sum_index] = temp % 10;
					carry = temp / 10;
				}
				else if (j < 0) {
					CBigDoubleBig3::AtomicT temp = num1.buf[i] + carry;
					sum.buf[sum_index] = temp % 10;
					carry = temp / 10;
				}
				else {
					CBigDoubleBig3::AtomicT temp = num1.buf[i] + num2.buf[j] + carry;
					sum.buf[sum_index] = temp % 10;
					carry = temp / 10;
				}
			}
		}
		CBigDoubleBig3::AtomicT temp;
		if (num1.size != num1_size) {
			temp = num2.buf[0] + carry;
		}
		else {
			if (num2.size != num2_size) {
				temp = num1.buf[0] + carry;
			}
			else {
				temp = num1.buf[0] + num2.buf[0] + carry;
			}
		}
		sum.buf[sum_index] = temp % 10;
		carry = temp / 10;
		//std::cout << "1: " << double(std::clock() - t) / CLOCKS_PER_SEC * 1000 << " ";
		//t = std:: clock();
		if (carry > 0) {
			sum.buf = (CBigDoubleBig3::AtomicT*)realloc(sum.buf, (sum.size + 1) );
			for (size_t i = sum.size; i > 0; i--)
				sum.buf[i] = sum.buf[i - 1];
			sum.order++;
			sum.buf[0] = carry;
			sum.size++;
		}
		//std::cout << "2: " << double(std::clock() - t) / CLOCKS_PER_SEC * 1000 << " ";
		sum.sign = num1.sign;
	}
	else {
		bool num1_is_less;
		if (num1.order != num2.order)
			num1_is_less = num1.order < num2.order;
		else {
			size_t i, j;
			for (i = 0, j = 0; i < num1.size && j < num2.size; i++, j++) {
				if (num1.buf[i] == num2.buf[i])
					continue;
				else {
					num1_is_less = (num1.buf[i] < num2.buf[i]);
					break;
				}
			}
			if (i == num1.size || j == num2.size)
				num1_is_less = num1.size < num2.size;
		}
		if (num1_is_less) sum.differences(sum, num2, num1);
		else sum.differences(sum, num1, num2);
		sum.sign = num1_is_less ? num2.sign : num1.sign;
		size_t i = 0;
		while (sum.size > 1 && sum.buf[i] == 0) {
			sum.size--; i++;
		}
		if (i > 0) {
			std::cout << " --- ";
			for (int j = 0; j < (int)sum.size; j++)
				sum.buf[j] = sum.buf[j + i];
			sum.buf = (CBigDoubleBig3::AtomicT*)realloc(sum.buf, sum.size);
			sum.order -= i;
		}
	}
	return sum;
}

CBigDoubleBig3 operator-(const CBigDoubleBig3 & num1, CBigDoubleBig3 & num2)
{
	num2.sign = false;
	CBigDoubleBig3 res;
	res = num1 + num2;
	return res;
}

CBigDoubleBig3 operator*(const CBigDoubleBig3 &num1, const CBigDoubleBig3 &num2) {
	CBigDoubleBig3 result;
	result.size = num1.size + num2.size;
	result.order = num1.order + num2.order;
	result.buf = (CBigDoubleBig3::AtomicT*)malloc(result.size);
	for (size_t i = 0; i < result.size; i++)
		result.buf[i] = 0;
	clock_t t = std::clock();
	for (int i = (int)num2.size - 1; i >= 0; i--) {
		if (num2.buf[i] != 0) {
			CBigDoubleBig3::AtomicT carry = 0;
			for (int j = (int)num1.size - 1; j >= 0; j--) {
				CBigDoubleBig3::AtomicT temp = num1.buf[j] * num2.buf[i] + result.buf[i + j + 1] + carry;
				result.buf[i + j + 1] = temp % 10;
				carry = temp / 10;
			}
			result.buf[i] = carry;
		}
	}
	std::cout << "1: " << std::clock() - t << " ";
	t = std::clock();
	size_t i = 0;
	while (result.size > 1 && result.buf[i] == 0) {
		result.size--;
		i++;
	}
	if (i > 0) {
		std::cout << " --- ";
#pragma loop(ivdep)
		for (size_t j = 0; j <= result.size - 1; j++) {
			result.buf[j] = result.buf[j + i];
		}
		result.buf = (CBigDoubleBig3::AtomicT*)realloc(result.buf, result.size);
	}
	else
		result.order++;
	std::cout << "2: " << std::clock() - t << " ";
	result.sign = num1.sign == num2.sign ? true : false;
	return result;
}
CBigDoubleBig3 operator*(const CBigDoubleBig3 & a, const int & c) {
	if (c == 0)
		return CBigDoubleBig3("0");
	CBigDoubleBig3 result;
	int b = c;
	result.sign = (a.sign && b > 0) || (!a.sign && b < 0) ? true : false;
	if (b < 0) b *= (-1);
	CBigDoubleBig3::AtomicT temp, carry = 0;
	size_t sizeB = 0;
	for (size_t i = b; i > 0; i = i / 10, sizeB++);
	result.size = a.size + sizeB;
	result.buf = (CBigDoubleBig3::AtomicT*)malloc(result.size);
	for (size_t i = 0; i < result.size; i++)
		result.buf[i] = 0;
	size_t i;
	for (i = result.size - 1; i >= sizeB; i--) {
		temp = a.buf[i - sizeB] * b + carry;
		carry = temp / 10;
		result.buf[i] = temp - carry * 10;
	}
	result.buf[i] = carry;
	return result;
}


CBigDoubleBig3 operator/(const CBigDoubleBig3 & a, const CBigDoubleBig3 & b)
{
	clock_t t = std::clock();
	CBigDoubleBig3 q;
	q.sign = a.sign == b.sign ? true : false;
	CBigDoubleBig3 copyA;
	size_t accuracy = 10000;
	size_t frac_diff = b.size - b.order - (a.size - a.order);
	copyA.size = a.size + 1 + (frac_diff > 0 ? frac_diff : 0) + accuracy;
	copyA.buf = (CBigDoubleBig3::AtomicT*)malloc(copyA.size);
	copyA.buf[0] = 0;
	size_t i, j;
	for (i = 1; i <= a.size; i++)
		copyA.buf[i] = a.buf[i - 1];
	for (i = a.size + 1; i < copyA.size; i++)
		copyA.buf[i] = 0;
	size_t a_size = a.size + (frac_diff > 0 ? frac_diff : 0);

	size_t shift = 0;
	CBigDoubleBig3 copyB;
	copyB = b;
	q.order = a.order - b.order - 1;
	CBigDoubleBig3::AtomicT scale;
	CBigDoubleBig3::AtomicT qGuess, r;
	CBigDoubleBig3::AtomicT borrow, carry;

	size_t n = b.size, m = a_size - b.size + accuracy;
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
	
	CBigDoubleBig3::AtomicT *aShift = copyA.buf + i;
	CBigDoubleBig3::AtomicT *bShift = copyB.buf + j;
	if (aShift[0] != 0)
		q.order++;
	else
		q.order += (aShift[1] >= bShift[0] ? 1 : 0);
	q.size = m + 1;
	q.buf = (CBigDoubleBig3::AtomicT*)malloc(q.size);
	std::cout << " 1: " << std::clock() - t << " ";
	t = clock();
	size_t q_ind, a_ind;
	for (q_ind = 0, a_ind = 0; q_ind <= m; q_ind++, a_ind++) {
		if (q_ind - shift == q.order + 1 + accuracy) {
			shift++;
			break;
		}
		CBigDoubleBig3::AtomicT temp = (aShift[a_ind] * 10 + aShift[a_ind + 1]);
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

		CBigDoubleBig3::AtomicT temp1, temp2;
		for (int i = (int)n - 1; i >= 0; i--) {
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
				if (aShift[1] >= bShift[0])
					q.order--;
				shift++;
				continue;
			}
			q.buf[q_ind - shift] = qGuess;
		}
		else {
			if (q_ind - shift == 0 && qGuess - 1 == 0) {
				if (aShift[1] >= bShift[0])
					q.order--;
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
	std::cout << " 2: " << std::clock() - t << " ";
	
	copyA.deleteNumber();
	copyB.deleteNumber();
	t = std::clock();
	m = q.order + accuracy;
	q.size = m + 1;
	if (shift != 0) {
		q.buf = (CBigDoubleBig3::AtomicT*)realloc(q.buf, q.size);
	}
	std::cout << " 3: " << std::clock() - t << " ";
	return q;
}
std::istream &operator >> (std::istream &stream, CBigDoubleBig3 &num) {
	std::string str;
	stream >> str;
	CBigDoubleBig3 _int(str);
	num = _int;
	return stream;
}
std::ostream &operator<<(std::ostream &stream, const CBigDoubleBig3 &num) {
	if (!num.sign)
		stream << '-';
	for (int i = 0; i < (int)num.size; i++) {
		if (i == 1)
			stream << ".";
		stream << (int)num.buf[i];
	}
	stream << "e";
	stream << num.order;
	return stream;
}
