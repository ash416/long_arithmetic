#include "BigDoubleLittle3.h"

#include <ctime>

CBigDoubleLittle3::CBigDoubleLittle3()
{
}

CBigDoubleLittle3::CBigDoubleLittle3(std::string str)
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
	this->buf = (CBigDoubleLittle3::AtomicT*)malloc(this->size);
	size_t j = this->size - 1;
	for (; i < leng; i++) {
		if (str[i] == '.') {
			this->order = i - is_negative - 1;
			continue;
		}
		this->buf[j--] = str[i] - '0';
	}
}

/*int & CBigDoubleLittle3::operator[](const int i)
{
	return buf[i];
}

int CBigDoubleLittle3::operator[](const int i) const
{
	return buf[i];
}
*/
bool operator==(const CBigDoubleLittle3 & a, const CBigDoubleLittle3 & b)
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


CBigDoubleLittle3 &CBigDoubleLittle3::operator =(const CBigDoubleLittle3 &num) {
	free(this->buf);
	this->buf = (CBigDoubleLittle3::AtomicT*)malloc(num.size);
	for (size_t i = 0; i < num.size; i++)
		this->buf[i] = num.buf[i];
	this->order = num.order;
	this->sign = num.sign;
	this->size = num.size;
	return *this;
}


void CBigDoubleLittle3::differences(CBigDoubleLittle3 & sum, const CBigDoubleLittle3 & big, const CBigDoubleLittle3 & small)
{
	CBigDoubleLittle3::AtomicT carry = 0;
	sum.order = big.order;
	size_t small_size = small.size + sum.order - small.order;
	sum.size = big.size > small_size ? big.size : small_size;
	sum.buf = (CBigDoubleLittle3::AtomicT*)malloc(sum.size);
	size_t sum_index = 0;
	for (size_t i = small_size - sum.size, j = big.size - sum.size; sum_index < sum.size - 1; i++, j++, sum_index++) {
		if (i < 0) {
			sum.buf[sum_index] = big.buf[j];
		}
		else
			if (j < 0) {
				if (i >= small.size) {
					sum.buf[sum_index] = (10 - carry) % 10;
					carry = -carry >= 0 ? 0 : 1;
				}
				else {
					sum.buf[sum_index] = (-small.buf[i] + 10 - carry) % 10;
					carry = -small.buf[i] - carry >= 0 ? 0 : 1;
				}
			}
			else if (i >= small.size) {
				CBigDoubleLittle3::AtomicT temp = big.buf[j] - carry + 10;
				sum.buf[sum_index] = temp % 10;
				carry = big.buf[j] - carry >= 0 ? 0 : 1;
			}
			else {
				CBigDoubleLittle3::AtomicT temp = big.buf[j] - small.buf[i] - carry + 10;
				sum.buf[sum_index] = temp % 10;
				carry = big.buf[j] - carry >= small.buf[i] ? 0 : 1;
			}
	}
	if (small_size > small.size) {
		sum.buf[sum_index] = (big.buf[big.size - 1] - carry + 10) % 10;
		carry = (big.buf[big.size - 1] - carry) >= 0 ? 0 : 1;
	}
	else {
		sum.buf[sum_index] = (big.buf[big.size - 1] - carry - small.buf[small.size - 1] + 10) % 10;
		carry = (big.buf[big.size - 1] - carry >= small.buf[small.size - 1] ? 0 : 1);
	}
}

CBigDoubleLittle3 operator+(const CBigDoubleLittle3 & num1, const CBigDoubleLittle3 & num2)
{
	CBigDoubleLittle3 sum;
	CBigDoubleLittle3::AtomicT carry = 0;
	if (num1.sign == num2.sign) {
		sum.order = num1.order > num2.order ? num1.order : num2.order;
		size_t num1_size = num1.size + sum.order - num1.order;
		size_t num2_size = num2.size + sum.order - num2.order;
		sum.size = num1_size > num2_size ? num1_size : num2_size;
		sum.buf = (CBigDoubleLittle3::AtomicT*)malloc(sum.size);
		size_t sum_index = 0;
		//clock_t t;
		//t = std::clock();
		for (int i = (int)num1_size - (int)sum.size, j = (int)num2_size - (int)sum.size; sum_index < sum.size - 1; i++, j++, sum_index++) {
			if (i < 0) {
				if (j >= (int)num2.size)
					sum.buf[sum_index] = 0;
				else
					sum.buf[sum_index] = num2.buf[j];
			}
			else if (j < 0) {
				if (i >= (int)num1.size)
					sum.buf[sum_index] = 0;
				else
					sum.buf[sum_index] = num1.buf[i];
			}
			else {
				if (j >= (int)num2.size) {
					CBigDoubleLittle3::AtomicT temp = num1.buf[i] + carry;
					sum.buf[sum_index] = temp % 10;
					carry = temp / 10;
				}
				else if (i >= (int)num1.size) {
					CBigDoubleLittle3::AtomicT temp = num2.buf[i] + carry;
					sum.buf[sum_index] = temp % 10;
					carry = temp / 10;
				}
				else {
					CBigDoubleLittle3::AtomicT temp = num1.buf[i] + num2.buf[j] + carry;
					sum.buf[sum_index] = temp % 10;
					carry = temp / 10;
				}
			}
		}
		
		
		CBigDoubleLittle3::AtomicT temp;
		if (num1_size > num1.size) {
			temp = num2.buf[num2.size - 1] + carry;
		}
		else {
			if (num2_size > num2.size) {
				temp = num1.buf[num1.size - 1] + carry;
			}
			else {
				temp = num1.buf[num1.size - 1] + num2.buf[num2.size - 1] + carry;
			}
		}
		sum.buf[sum_index] = temp % 10;
		carry = temp / 10;
		//std::cout << "1: " << double(std::clock() - t) / CLOCKS_PER_SEC * 1000 << " ";
		//t = std::clock();
		if (carry > 0) {
			sum.size++;
			sum.buf = (CBigDoubleLittle3::AtomicT*)realloc(sum.buf, sum.size);
			sum.buf[sum.size - 1] = carry;
			sum.order++;
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
			for (i = num1.size - 1, j = num2.size - 1; i >= 0 && j >= 0; i--, j--) {
				if (num1.buf[i] == num2.buf[j])
					continue;
				else {
					num1_is_less = num1.buf[i] < num2.buf[j];
					break;
				}
			}
			if (i < 0 || j < 0)
				num1_is_less = num1.size < num2.size;
		}
		if (num1_is_less) sum.differences(sum, num2, num1);
		else sum.differences(sum, num1, num2);
		sum.sign = num1_is_less ? num2.sign : num1.sign;
		bool flag = false;
		size_t i = sum.size - 1;
		while (i > 0 && sum.buf[i--] == 0) {
			sum.size--;
			sum.order--;
			flag = true;
		}
		if (flag)
			sum.buf = (CBigDoubleLittle3::AtomicT*)realloc(sum.buf, sum.size);
	}
	return sum;
}


CBigDoubleLittle3 operator-(const CBigDoubleLittle3 & num1, CBigDoubleLittle3 & num2)
{
	num2.sign = false;
	CBigDoubleLittle3 res;
	res = num1 + num2;
	return res;
}

CBigDoubleLittle3 operator*(const CBigDoubleLittle3 &num1, const CBigDoubleLittle3 &num2) {
	CBigDoubleLittle3 result;
	result.size = num1.size + num2.size;
	result.order = num1.order + num2.order;
	result.buf = (CBigDoubleLittle3::AtomicT*)malloc(result.size);
	for (size_t i = 0; i < result.size; i++)
		result.buf[i] = 0;
	clock_t t = std::clock();
	for (size_t i = 0; i < num2.size; ++i) {
		if (num2.buf[i] != 0) {
			CBigDoubleLittle3::AtomicT carry = 0;
			for (size_t j = 0; j < num1.size; ++j) {
				CBigDoubleLittle3::AtomicT temp = num1.buf[j] * num2.buf[i] + result.buf[i + j] + carry;
				result.buf[i + j] = temp % 10;
				carry = temp / 10;
			}
			result.buf[i + num1.size] = carry;
		}
	}
	std::cout << "1: " << std::clock() - t << " ";
	t = std::clock();
	bool flag = false;
	size_t i = result.size - 1;
	while (i > 1 && result.buf[i] == 0) {
		result.size--; flag = true;
		i--;
	}
	if (flag) {
		result.buf = (CBigDoubleLittle3::AtomicT*)realloc(result.buf, result.size);
	}
	else {
		result.order++;
	}
	std::cout << "2: " << std::clock() - t << " " ;
	result.sign = num1.sign == num2.sign ? true : false;
	return result;
}
CBigDoubleLittle3 operator*(const CBigDoubleLittle3 & a, const int & c) {
	if (c == 0)
		return CBigDoubleLittle3("0");
	CBigDoubleLittle3 result;
	int b = c;
	result.sign = (a.sign && b > 0) || (!a.sign && b < 0) ? true : false;
	result.order = a.order;
	if (b < 0) b *= (-1);
	CBigDoubleLittle3::AtomicT temp, carry = 0;
	size_t sizeB = 0;
	for (size_t i = b; i > 0; i = i / 10, sizeB++);
	result.size = a.size + sizeB;
	result.buf = (CBigDoubleLittle3::AtomicT*)malloc(result.size);
	for (size_t i = 0; i < result.size; i++)
		result.buf[i] = 0;
	size_t i;
	for (i = 0; i < a.size; i++) {
		temp = a.buf[i] * b + carry;
		carry = temp / 10;
		result.buf[i] = temp - carry * 10;
	}
	result.buf[i] = carry;
	return result;
}

CBigDoubleLittle3 operator/(const CBigDoubleLittle3 & a, const CBigDoubleLittle3 & b)
{
	clock_t t = std::clock();
	CBigDoubleLittle3 q;
	q.sign = a.sign == b.sign ? true : false;
	CBigDoubleLittle3 copyA;
	size_t accuracy = 10000;
	size_t frac_diff = b.size - b.order - (a.size - a.order);
	copyA.size = a.size + 1 + (frac_diff > 0 ? frac_diff : 0) + accuracy;
	copyA.buf = (CBigDoubleLittle3::AtomicT*)malloc(copyA.size);
	size_t j;
	for (j = 0; j < (frac_diff > 0 ? frac_diff : 0) + accuracy; j++)
		copyA.buf[j] = 0;
	for (size_t i = 0; i < a.size; i++)
		copyA.buf[j++] = a.buf[i];
	size_t a_size = a.size + (frac_diff > 0 ? frac_diff  : 0);
	if (copyA.buf[j - 1] == 0) {
		a_size--;
		j--;
		copyA.size--;
	}
	copyA.buf[j] = 0;

	size_t shift = 0;
	CBigDoubleLittle3 copyB;
	copyB = b;
	q.order = a.order - b.order - 1;
	CBigDoubleLittle3::AtomicT scale;
	CBigDoubleLittle3::AtomicT qGuess, r;
	CBigDoubleLittle3::AtomicT borrow, carry;
	size_t n = b.size, m = a_size - b.size + accuracy;
	scale = 10 / (b.buf[n - 1] + 1);
	if (scale > 1) {
		copyA = copyA * scale;
		if (copyA.buf[copyA.size - 1] == 0) {
			copyA.size--;
		}
		copyB = copyB * scale;
		if (copyB.buf[copyB.size - 1] == 0) {
			copyB.size--;
		}
	}
	if (copyA.buf[copyA.size - 1] != 0)
		q.order++;
	else
		q.order += (copyA.buf[copyA.size - 2] >= copyB.buf[copyB.size - 1] ? 1 : 0);
	q.size = m + 1;
	q.buf = (CBigDoubleLittle3::AtomicT*)malloc(q.size);
	
	std::cout << " 1: " << std::clock() - t << " ";
	t = std::clock();
	size_t q_ind, a_ind;
	for (q_ind = m, a_ind = a_size + accuracy; q_ind >= 0; q_ind--, a_ind--) {
		if (q_ind + shift == m - q.order - 1 - accuracy) {
			shift++;
			break;
		}
		CBigDoubleLittle3::AtomicT temp = (copyA.buf[a_ind] * 10 + copyA.buf[a_ind - 1]);
		qGuess = temp / copyB.buf[n - 1];
		r = temp % copyB.buf[n - 1];
		while (r < 10) {
			if (copyB.buf[n - 2] * qGuess > r * 10 + copyA.buf[a_ind - 2] || qGuess == 10) {
				qGuess--;
				r += copyB.buf[n - 1];
			}
			else break;
		}
		carry = 0;
		borrow = 0;
		CBigDoubleLittle3::AtomicT *aShift = copyA.buf + q_ind;

		CBigDoubleLittle3::AtomicT temp1, temp2;
		for (size_t i = 0; i < n; i++) {
			temp1 = copyB.buf[i] * qGuess + carry;
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
			if (q_ind + shift == m && qGuess == 0) {
				if (copyA.buf[copyA.size - 2] >= copyB.buf[copyB.size - 1])
					q.order--;
				shift++;
				continue;
			}
			q.buf[q_ind + shift] = qGuess;
		}
		else {
			if (q_ind + shift == m && qGuess - 1 == 0) {
				if (copyA.buf[copyA.size - 2] >= copyB.buf[copyB.size - 1])
					q.order--;
				shift++;
			}
			else {
				q.buf[q_ind + shift] = qGuess - 1;
			}
			carry = 0;
			for (size_t i = 0; i < n; i++) {
				temp = aShift[i] + copyB.buf[i] + carry;
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
	std::cout << " 2: " << std::clock() - t << " ";
	
	copyA.deleteNumber();
	copyB.deleteNumber();
	t = std::clock();
	if (shift != 0) {
		std::cout << "---";
		size_t sh = m - q.order - accuracy;
#pragma loop(no_vector)
		for (size_t i = 0; i <= m - sh; i++)
			q.buf[i] = q.buf[i + sh];
		m -= sh;
		q.size = m + 1;
		q.buf = (CBigDoubleLittle3::AtomicT*)realloc(q.buf, q.size);
	}
	std::cout << " 3: " << std::clock() - t << " ";
	return q;
}

std::istream &operator >> (std::istream &stream, CBigDoubleLittle3 &num) {
	std::string str;
	stream >> str;
	CBigDoubleLittle3 _int(str);
	num = _int;
	return stream;
}
std::ostream &operator<<(std::ostream &stream, const CBigDoubleLittle3 &num) {
	if (!num.sign)
		stream << '-';
	for (int i = (int)num.size - 1; i >= 0; i--) {
		if (i == num.size - 2)
			stream << '.';
		stream << int(num.buf[i]);
	}
	stream << "e";
	stream << num.order;
	return stream;
}
