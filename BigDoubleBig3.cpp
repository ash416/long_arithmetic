#include "BigDoubleBig3.h"

CBigDoubleBig3::CBigDoubleBig3()
{
}

CBigDoubleBig3::CBigDoubleBig3(std::string str)
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
	this->size = leng - 1 - is_negative;
	int i = is_negative;
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
	this->buf = (int*)malloc(this->size * sizeof(int));
	int j = 0;
	for (; i < leng; i++) {
		if (str[i] == '.') {
			this->order = i - is_negative - 1;
			continue;
		}
		this->buf[j++] = int(str[i] - '0');
	}
}

int & CBigDoubleBig3::operator[](const int i)
{
	return buf[i];
}

int CBigDoubleBig3::operator[](const int i) const
{
	return buf[i];
}

bool operator==(const CBigDoubleBig3 & a, const CBigDoubleBig3 & b)
{
	if (a.size != b.size || a.sign != b.sign || a.order != b.order)
		return false;
	else {
		for (int i = 0; i < a.size; i++)
			if (a[i] != b[i])
				return false;
		return true;
	}
}



CBigDoubleBig3 &CBigDoubleBig3::operator =(const CBigDoubleBig3 &num) {
	free(this->buf);
	this->buf = (int*)malloc(num.size * sizeof(int));
	for (int i = 0; i < num.size; i++)
		this->buf[i] = num[i];
	this->order = num.order;
	this->sign = num.sign;
	this->size = num.size;
	return *this;
}

void CBigDoubleBig3::differences(CBigDoubleBig3 & sum, const CBigDoubleBig3 & big, const CBigDoubleBig3 & small)
{
	int carry = 0;
	sum.order = big.order;
	int small_size = small.size + sum.order - small.order;
	sum.size = big.size > small_size ? big.size : small_size;
	sum.buf = (int*)malloc(sum.size * sizeof(int));
	int sum_index = sum.size - 1;
	for (int i = sum.size - 1 - (sum.order - small.order), j = sum.size - 1 - (sum.order - big.order); sum_index > 0; i--, j--, sum_index--) {
		if (i >= small.size) sum[sum_index] = big[j];
		else if (j >= big.size) {
			if (i < 0) {
				sum[sum_index] = (10 - carry) % 10;
				carry = -carry >= 0 ? 0 : 1;
			}
			else {
				sum[sum_index] = (-small[i] + 10 - carry) % 10;
				carry = -small[i] - carry >= 0 ? 0 : 1;
			}
		}
		else if (i < 0) {
			int temp = big[j]  - carry + 10;
			sum[sum_index] = temp % 10;
			carry = big[j] - carry >= 0 ? 0 : 1;
		}
		else {
			int temp = big[j] - small[i] - carry + 10;
			sum[sum_index] = temp % 10;
			carry = big[j] - carry >= small[i] ? 0 : 1;
		}
	}
	if (small.size != small_size) {
		sum[sum_index] = (big[0] - carry + 10) % 10;
		carry = (big[0] - carry) >= 0 ? 0 : 1;
	}
	else {
		sum[sum_index] = (big[0] - carry - small[0] + 10) % 10;
		carry = (big[0] - carry >= small[0] ? 0 : 1);
	}
}

CBigDoubleBig3 operator+(const CBigDoubleBig3 & num1, const CBigDoubleBig3 & num2)
{
	CBigDoubleBig3 sum;
	int carry = 0;
	if (num1.sign == num2.sign) {
		sum.order = num1.order > num2.order ? num1.order : num2.order;
		int num1_size = num1.size + sum.order - num1.order;
		int num2_size = num2.size + sum.order - num2.order;
		sum.size = num1_size > num2_size ? num1_size : num2_size;
		sum.buf = (int*)malloc(sum.size * sizeof(int));
		int sum_index = sum.size - 1;
		for (int i = sum.size - 1 - (sum.order - num1.order), j = sum.size - 1 - (sum.order - num2.order); sum_index > 0; i--, j--, sum_index--) {
			if (i >= num1.size) {
				if (j < 0)
					sum[sum_index] = 0;
				else
					sum[sum_index] = num2[j];
			}
			else if (j >= num2.size) {
				if (i < 0)
					sum[sum_index] = 0;
				else
					sum[sum_index] = num1[i];
			}
			else {
				if (i < 0) {
					int temp = num2[i] + carry;
					sum[sum_index] = temp % 10;
					carry = temp / 10;
				}
				else if (j < 0) {
					int temp = num1[i] + carry;
					sum[sum_index] = temp % 10;
					carry = temp / 10;
				}
				else {
					int temp = num1[i] + num2[j] + carry;
					sum[sum_index] = temp % 10;
					carry = temp / 10;
				}
			}
		}
		int temp;
		if (num1.size != num1_size)
			temp = num2[0] + carry;
		else
			if (num2.size != num2_size)
				temp = num1[0] + carry;
			else
				temp = num1[0] + num2[0] + carry;
		sum[sum_index] = temp % 10;
		carry = temp / 10;

		if (carry > 0) {
			sum.buf = (int*)realloc(sum.buf, (sum.size + 1) * sizeof(int));
			for (int i = sum.size; i > 0; i--)
				sum[i] = sum[i - 1];
			sum.order++;
			sum[0] = carry;
			sum.size++;
		}
		sum.sign = num1.sign;
	}
	else {
		bool num1_is_less;
		if (num1.order != num2.order)
			num1_is_less = num1.order < num2.order;
		else {
			int i, j;
			for (i = 0, j = 0; i < num1.size && j < num2.size; i++, j++) {
				if (num1[i] == num2[i])
					continue;
				else {
					num1_is_less = (num1[i] < num2[i]);
					break;
				}
			}
			if (i == num1.size || j == num2.size)
				num1_is_less = num1.size < num2.size;
		}
		if (num1_is_less) sum.differences(sum, num2, num1);
		else sum.differences(sum, num1, num2);
		sum.sign = num1_is_less ? num2.sign : num1.sign;
		int i = 0;
		while (sum.size > 1 && sum[i] == 0) {
			sum.size--; i++;
		}
		if (i > 0) {
			for (int j = 0; j < sum.size - i + 1; j++)
				sum[j] = sum[j + i];
			sum.buf = (int*)realloc(sum.buf, sum.size * sizeof(int));
			sum.order--;
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
	result.buf = (int*)malloc(result.size * sizeof(int));
	for (int i = 0; i < result.size; i++)
		result[i] = 0;
	for (int i = num2.size - 1; i >= 0; i--) {
		if (num2[i] != 0) {
			int carry = 0;
			for (int j = num1.size - 1; j >= 0; j--) {
				int temp = num1[j] * num2[i] + result[i + j + 1] + carry;
				result[i + j + 1] = temp % 10;
				carry = temp / 10;
			}
			result[i] = carry;
		}
	}
	int i = 0;
	while (result.size > 1 && result[i] == 0) {
		result.size--;
		i++;
	}
	if (i > 0) {
		for (int j = 0; j <= result.size - 1; j++) {
			result[j] = result[j + i];
		}
		result.buf = (int*)realloc(result.buf, result.size * sizeof(int));
	}
	else
		result.order++;
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
	int temp, carry = 0;
	int sizeB = 0;
	for (int i = b; i > 0; i = i / 10, sizeB++);
	result.size = a.size + sizeB;
	result.buf = (int*)malloc(result.size * sizeof(int));
	for (int i = 0; i < result.size; i++)
		result[i] = 0;
	int i;
	for (i = result.size - 1; i >= sizeB; i--) {
		temp = a[i - sizeB] * b + carry;
		carry = temp / 10;
		result[i] = temp - carry * 10;
	}
	result[i] = carry;
	/*while (carry > 0) {
		result[i--] = carry % 10;
		carry /= 10;
	}*/
	return result;
}


CBigDoubleBig3 operator/(const CBigDoubleBig3 & a, const CBigDoubleBig3 & b)
{
	CBigDoubleBig3 q;
	q.sign = a.sign == b.sign ? true : false;
	CBigDoubleBig3 copyA;
	int accuracy = 10;
	int frac_diff = b.size - b.order - (a.size - a.order);
	copyA.size = a.size + 1 + (frac_diff > 0 ? frac_diff : 0) + accuracy;
	copyA.buf = (int*)malloc(copyA.size * sizeof(int));
	copyA[0] = 0;
	int i, j;
	for (i = 1; i <= a.size; i++)
		copyA[i] = a[i - 1];
	for (i = a.size + 1; i < copyA.size; i++)
		copyA[i] = 0;
	int a_size = a.size + (frac_diff > 0 ? frac_diff : 0);

	int shift = 0;
	CBigDoubleBig3 copyB;
	copyB = b;
	q.order = a.order - b.order - 1;
	short scale;
	short qGuess, r;
	short borrow, carry;

	int n = b.size, m = a_size - b.size + accuracy;
	scale = 10 / (b[0] + 1);
	i = 0; j = 0;
	//std::cout << copyA << std::endl;
	//std::cout << copyB << std::endl;
	if (scale > 1) {
		copyA = copyA * scale;
		if (copyA[i] == 0 && copyA.size > 1) {
			copyA.size--;
			i++;
		}
		copyB = copyB * scale;
		if (copyB[j] == 0 && copyB.size > 1) {
			copyB.size--;
			j++;
		}
	}
	//std::cout << copyA << std::endl;
	//std::cout << copyB << std::endl;
	
	int *aShift = copyA.buf + i;
	int *bShift = copyB.buf + j;
	if (aShift[0] != 0)
		q.order++;
	else
		q.order += (aShift[1] >= bShift[0] ? 1 : 0);
	q.size = m + 1;
	q.buf = (int*)malloc(q.size * sizeof(int));
	

	int q_ind, a_ind;
	for (q_ind = 0, a_ind = 0; q_ind <= m; q_ind++, a_ind++) {
		if (q_ind - shift == q.order + 1 + accuracy) {
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
				if (aShift[1] >= bShift[0])
					q.order--;
				shift++;
				continue;
			}
			q[q_ind - shift] = qGuess;
		}
		else {
			if (q_ind - shift == 0 && qGuess - 1 == 0) {
				if (aShift[1] >= bShift[0])
					q.order--;
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

	m = q.order + accuracy;
	q.size = m + 1;
	if (shift != 0) {
		q.buf = (int*)realloc(q.buf, q.size * sizeof(int));
	}

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
	for (int i = 0; i < num.size; i++) {
		if (i == 1)
			stream << ".";
		stream << num[i];
	}
	stream << "e";
	stream << num.order;
	return stream;
}
