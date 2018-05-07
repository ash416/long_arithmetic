#include "BigDoubleBig2.h"
#include <algorithm>

CBigDoubleBig2::CBigDoubleBig2()
{
}

CBigDoubleBig2::CBigDoubleBig2(std::string str)
{
	int leng = str.length();
	short is_negative = 0;
	if (str[0] == '-') {
		this->sign = false;
		is_negative = 1;
	}
	else {
		this->sign = true;
	}
	int point = str.find('.');
	this->int_size = point - is_negative;
	this->frac_size = str.length() - point - 1;
	this->int_part = (int*)malloc(this->int_size * sizeof(int));
	this->frac_part = (int*)malloc(this->frac_size * sizeof(int));
	int j = 0;
	for (int i = is_negative; i < point; i++)
		this->int_part[j++] = int(str[i] - '0');
	j = 0;
	for (size_t i = point + 1; i < str.length(); i++)
		this->frac_part[j++] = int(str[i] - '0');
}

int & CBigDoubleBig2::operator[](const int i)
{
	if (i >= this->int_size)
		return this->frac_part[i - this->int_size];
	else
		return this->int_part[i];
}

int CBigDoubleBig2::operator[](const int i) const
{
	if (i >= this->int_size)
		return this->frac_part[i - this->int_size];
	else
		return this->int_part[i];
}

bool operator==(const CBigDoubleBig2 & a, const CBigDoubleBig2 & b)
{
	if (a.int_size != b.int_size || a.sign != b.sign || a.frac_size != b.frac_size)
		return false;
	else {
		for (int i = 0; i < a.int_size; i++)
			if (a.int_part[i] != b.int_part[i])
				return false;
		for (int i = 0; i < a.frac_size; i++)
			if (a.frac_part[i] != b.frac_part[i])
				return false;
		return true;
	}
}

/*bool operator<(const CBigDoubleBig2 & a, const CBigDoubleBig2 &b) {
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

bool operator>(const CBigDoubleBig2 & a, const CBigDoubleBig2 &b) {
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

bool operator >=(const CBigDoubleBig2 &a, const CBigDoubleBig2 &b) {
	return(!(a < b));
}

bool operator <=(const CBigDoubleBig2 &a, const CBigDoubleBig2 &b) {
	return (!(a > b));
}

bool operator !=(const CBigDoubleBig2 &a, const CBigDoubleBig2 &b) {
	return (!(a == b));
}
*/
CBigDoubleBig2 &CBigDoubleBig2::operator =(const CBigDoubleBig2 &num) {
	free(this->int_part);
	free(this->frac_part);
	this->int_size = num.int_size;
	this->frac_size = num.frac_size;
	this->int_part = (int*)malloc(this->int_size * sizeof(int));
	this->frac_part = (int*)malloc(this->frac_size * sizeof(int));
	for (int i = 0; i < this->int_size; i++)
		this->int_part[i] = num.int_part[i];
	for (int i = 0; i < this->frac_size; i++)
		this->frac_part[i] = num.frac_part[i];
	this->sign = num.sign;
	return *this;
}

void CBigDoubleBig2::differences(CBigDoubleBig2 & sum, const CBigDoubleBig2 & big, const CBigDoubleBig2 & small)
{
	int carry = 0;
	sum.int_size = std::max(small.int_size, big.int_size);
	sum.frac_size = std::max(small.frac_size, big.frac_size);
	sum.int_part = (int*)malloc(sum.int_size * sizeof(int));
	sum.frac_part = (int*)malloc(sum.frac_size * sizeof(int));
	int sum_index = sum.frac_size - 1;
	for (int i = sum.frac_size - 1; i >= 0; i--) {
		if (i >= small.frac_size) sum.frac_part[sum_index--] = big.frac_part[i];
		else if (i >= big.frac_size) {
			sum.frac_part[sum_index--] = (-small.frac_part[i] + 10 - carry) % 10;
			carry = -small.frac_part[i] - carry >= 0 ? 0 : 1;
		}
		else {
			int temp = big.frac_part[i] - small.frac_part[i] - carry + 10;
			sum.frac_part[sum_index--] = temp % 10;
			carry = big.frac_part[i] - carry >= small.frac_part[i] ? 0 : 1;
		}
	}
	sum_index = sum.int_size - 1;
	for (int i = small.int_size - 1, j = big.int_size - 1; sum_index >= 0; i--, j--, sum_index--) {
		if (i < 0) {
			sum.int_part[sum_index] = (big.int_part[j] - carry + 10) % 10;
			carry = (big.int_part[j] - carry) >= 0 ? 0 : 1;
		}
		else {
			sum.int_part[sum_index] = (big.int_part[j] - carry - small.int_part[i] + 10) % 10;
			carry = (big.int_part[j] - carry >= small.int_part[i] ? 0 : 1);
		}
	}
}

CBigDoubleBig2 operator+(const CBigDoubleBig2 & num1, const CBigDoubleBig2 & num2)
{
	CBigDoubleBig2 sum;
	int carry = 0;
	if (num1.sign == num2.sign) {
		sum.int_size = std::max(num1.int_size, num2.int_size);
		sum.frac_size = std::max(num1.frac_size, num2.frac_size);
		sum.int_part = (int*)malloc(sum.int_size * sizeof(int));
		sum.frac_part = (int*)malloc(sum.frac_size * sizeof(int));
		int sum_index = sum.frac_size - 1;
		for (int i = sum.frac_size - 1; i >= 0; i--) {
			if (i >= num1.frac_size) sum.frac_part[sum_index--] = num2.frac_part[i];
			else if (i >= num2.frac_size) sum.frac_part[sum_index--] = num1.frac_part[i];
			else {
				int temp = num1.frac_part[i] + num2.frac_part[i] + carry;
				sum.frac_part[sum_index--] = temp % 10;
				carry = temp / 10;
			}
		}
		sum_index = sum.int_size - 1;
		for (int i = num1.int_size - 1, j = num2.int_size - 1; sum_index >= 0; i--, j--, sum_index--) {
			int temp;
			if (i < 0)
				temp = num2.int_part[j] + carry;
			else
				if (j < 0)
					temp = num1.int_part[i] + carry;
				else
					temp = num1.int_part[i] + num2.int_part[j] + carry;
			sum.int_part[sum_index] = temp % 10;
			carry = temp / 10;
		}
		if (carry > 0) {
			sum.int_part = (int*)realloc(sum.int_part, (sum.int_size + 1) * sizeof(int));
			for (int i = sum.int_size; i > 0; i--)
				sum.int_part[i] = sum.int_part[i - 1];
			sum.int_part[0] = carry;
			sum.int_size++;
		}
		sum.sign = num1.sign;
	}
	else {
		bool num1_is_less;
		bool flag = false;
		if (num1.int_size != num2.int_size)
			num1_is_less = num1.int_size < num2.int_size;
		else {
			int i;
			for (i = 0; i < num1.int_size; i++) {
				if (num1.int_part[i] == num2.int_part[i])
					continue;
				else {
					num1_is_less = num1.int_part[i] < num2.int_part[i];
					flag = true;
					break;
				}
			}
			if (!flag) {
				for (i = 0; i < num1.frac_size; i++) {
					if (num1.frac_part[i] == num2.frac_part[i])
						continue;
					else {
						num1_is_less = num1.int_part[i] < num2.int_part[i];
						break;
					}
				}
			}
			if (i == num1.frac_size)
				num1_is_less = num1.frac_size < num2.frac_size;
		}
		if (num1_is_less) sum.differences(sum, num2, num1);
		else sum.differences(sum, num1, num2);
		sum.sign = num1_is_less ? num2.sign : num1.sign;
		int i = 0;
		while (i < sum.int_size - 1 && sum.int_part[i] == 0) {
			sum.int_size--; i++;
		}
		if (i > 0) {
			for (int j = 0; j < sum.int_size - i + 1; j++)
				sum.int_part[j] = sum.int_part[j + i];
			sum.int_part = (int*)realloc(sum.int_part, sum.int_size * sizeof(int));
		}
	}
	return sum;
}

CBigDoubleBig2 operator-(const CBigDoubleBig2 & num1, CBigDoubleBig2 & num2)
{
	num2.sign = false;
	CBigDoubleBig2 res;
	res = num1 + num2;
	return res;
}

CBigDoubleBig2 operator*(const CBigDoubleBig2 &num1, const CBigDoubleBig2 &num2) {
	CBigDoubleBig2 result;
	result.int_size = num1.int_size + num2.int_size;
	result.frac_size = num1.frac_size + num2.frac_size;
	result.int_part = (int*)malloc(result.int_size * sizeof(int));
	result.frac_part = (int*)malloc(result.frac_size * sizeof(int));
	for (int i = 0; i < std::max(result.int_size, result.frac_size); i++) {
		if (i < result.int_size)
			result.int_part[i] = 0;
		if (i < result.frac_size)
			result.frac_part[i] = 0;
	}
	for (int i = num2.int_size + num2.frac_size - 1; i >= 0; i--) {
		if (num2[i] != 0) {
			int carry = 0;
			for (int j = num1.int_size + num1.frac_size - 1; j >= 0; j--) {
				int temp = num1[j] * num2[i] + result[i + j + 1] + carry;
				result[i + j + 1] = temp % 10;
				carry = temp / 10;
			}
			result[i] = carry;
		}
	}
	int i = 0;
	while (i < result.int_size && result[i] == 0) {
		result.int_size--;
		i++;
	}
	if (i > 0) {
		for (int j = 0; j <= result.int_size - 1; j++) {
			result.int_part[j] = result.int_part[j + i];
		}
		result.int_part = (int*)realloc(result.int_part, result.int_size * sizeof(int));
	}
	result.sign = num1.sign == num2.sign ? true : false;
	return result;
}

CBigDoubleBig2 operator*(const CBigDoubleBig2 & a, const int & c) {
	if (c == 0)
		return CBigDoubleBig2("0");
	CBigDoubleBig2 result;
	int b = c;
	result.sign = (a.sign && b > 0) || (!a.sign && b < 0) ? true : false;
	if (b < 0) b *= (-1);
	int temp, carry = 0;
	int sizeB = 0;
	for (int i = b; i > 0; i = i / 10, sizeB++);
	result.int_size = a.int_size + sizeB;
	result.frac_size = a.frac_size;
	result.int_part = (int*)malloc(result.int_size * sizeof(int));
	result.frac_part = (int*)malloc(result.frac_size * sizeof(int));
	for (int i = 0; i < std::max(result.int_size, result.frac_size); i++) {
		if (i < result.int_size)
			result.int_part[i] = 0;
		if (i < result.frac_size)
			result.frac_part[i] = 0;
	}
	int i;
	for (i = result.int_size + result.frac_size - 1; i >= sizeB; i--) {
		temp = a[i - sizeB] * b + carry;
		carry = temp / 10;
		result[i] = temp - carry * 10;
	}
	while (carry > 0) {
		result[i--] = carry % 10;
		carry /= 10;
	}
	return result;
}


CBigDoubleBig2 operator/(const CBigDoubleBig2 & a, const CBigDoubleBig2 & b)
{
	CBigDoubleBig2 q;
	q.sign = a.sign == b.sign ? true : false;
	CBigDoubleBig2 copyA;
	int accuracy = 3;
	int frac_diff = b.frac_size - a.frac_size;
	copyA.int_size = a.int_size + a.frac_size + 1 + (frac_diff > 0 ? frac_diff : 0) + accuracy;
	copyA.int_part = (int*)malloc(copyA.int_size * sizeof(int));
	copyA[0] = 0;
	int i, j;
	for (i = 1; i <= a.int_size + a.frac_size; i++)
		copyA[i] = a[i - 1];
	for (i = a.int_size + a.frac_size + 1; i < copyA.int_size; i++)
		copyA[i] = 0;
	int a_size = a.int_size + a.frac_size + frac_diff;

	int shift = 0;
	CBigDoubleBig2 copyB;
	copyB.int_size = b.int_size + b.frac_size;
	copyB.int_part = (int*)malloc(copyB.int_size * sizeof(int));
	for (int i = 0; i < copyB.int_size; i++) {
		copyB.int_part[i] = b[i];
	}

	short scale;
	short qGuess, r;
	short borrow, carry;
	int diff = a.int_size - b.int_size;
	int n = b.int_size + b.frac_size, m = a_size - b.int_size - b.frac_size + accuracy;
	scale = 10 / (b[0] + 1);
	i = 0; j = 0;
	//std::cout << copyA << std::endl;
	//std::cout << copyB << std::endl;
	if (scale > 1) {
		copyA = copyA * scale;
		if (copyA.int_part[i] == 0 && copyA.int_size > 1) {
			copyA.int_size--;
			i++;
		}
		copyB = copyB * scale;
		if (copyB.int_part[j] == 0 && copyB.int_size > 1) {
			copyB.int_size--;
			j++;
		}
	}
	//std::cout << copyA << std::endl;
	//std::cout << copyB << std::endl;
	int *aShift = copyA.int_part + i;
	int *bShift = copyB.int_part + j;
	if (aShift[0] != 0)
		diff++;
	else
		diff += (aShift[1] >= bShift[0] ? 1 : 0);
	q.int_size = diff;
	q.frac_size = m + 1 - diff;
	q.int_part = (int*)malloc(q.int_size * sizeof(int));
	q.frac_part = (int*)malloc(q.frac_size * sizeof(int));

	int q_ind, a_ind;
	for (q_ind = 0, a_ind = 0; q_ind <= m; q_ind++, a_ind++) {
		if (q_ind - shift == q.int_size + accuracy) {
			shift++;
			break;
		}
		int temp = (aShift[a_ind] * 10 + aShift[a_ind + 1]);
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

		int temp1, temp2;
		for (int i = n - 1; i >= 0; i--) {
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
				if (aShift[1] >= bShift[0]) {
					q.frac_size++;
					q.int_size--;
					q.frac_part = (int *)realloc(q.frac_part, q.frac_size * sizeof(int));
					q.int_part = (int *)realloc(q.int_part, q.int_size * sizeof(int));
				}
				shift++;
				continue;
			}
			q[q_ind - shift] = qGuess;
		}
		else {
			if (q_ind - shift == 0 && qGuess - 1 == 0) {
				if (aShift[1] >= bShift[0]) {
					q.frac_size++;
					q.int_size--;
					q.frac_part = (int *)realloc(q.frac_part, q.frac_size * sizeof(int));
					q.int_part = (int *)realloc(q.int_part, q.int_size * sizeof(int));
				}
				shift++;
			}
			else {
				q[q_ind - shift] = qGuess - 1;
			}
			carry = 0;
			for (int i = n - 1; i >= 0; i--) {
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
	/*while (copyA.size > 1 && (copyA[0] == 0))
	copyA.size--;*/
	copyA.deleteNumber();
	copyB.deleteNumber();

	q.frac_size -= shift;
	if (shift != 0) {
		q.frac_part = (int*)realloc(q.frac_part, q.frac_size * sizeof(int));
	}

	return q;
}

std::istream &operator >> (std::istream &stream, CBigDoubleBig2 &num) {
	std::string str;
	stream >> str;
	CBigDoubleBig2 _int(str);
	num = _int;
	return stream;
}
std::ostream &operator<<(std::ostream &stream, const CBigDoubleBig2 &num) {
	if (!num.sign)
		stream << '-';
	for (int i = 0; i < num.int_size; i++) {
		stream << num.int_part[i];
	}
	stream << '.';
	for (int i = 0; i < num.frac_size; i++) {
		stream << num.frac_part[i];
	}
	return stream;
}
