#include "BigDoubleLittle3.h"

CBigDoubleLittle3::CBigDoubleLittle3()
{
}

CBigDoubleLittle3::CBigDoubleLittle3(std::string str)
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
	int j = this->size - 1;
	for (; i < leng; i++) {
		if (str[i] == '.') {
			this->order = i - is_negative - 1;
			continue;
		}
		this->buf[j--] = int(str[i] - '0');
	}
}

int & CBigDoubleLittle3::operator[](const int i)
{
	return buf[i];
}

int CBigDoubleLittle3::operator[](const int i) const
{
	return buf[i];
}

bool operator==(const CBigDoubleLittle3 & a, const CBigDoubleLittle3 & b)
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


CBigDoubleLittle3 &CBigDoubleLittle3::operator =(const CBigDoubleLittle3 &num) {
	free(this->buf);
	this->buf = (int*)malloc(num.size * sizeof(int));
	for (int i = 0; i < num.size; i++)
		this->buf[i] = num[i];
	this->order = num.order;
	this->sign = num.sign;
	this->size = num.size;
	return *this;
}


void CBigDoubleLittle3::differences(CBigDoubleLittle3 & sum, const CBigDoubleLittle3 & big, const CBigDoubleLittle3 & small)
{
	int carry = 0;
	sum.order = big.order;
	int small_size = small.size + sum.order - small.order;
	sum.size = big.size > small_size ? big.size : small_size;
	sum.buf = (int*)malloc(sum.size * sizeof(int));
	int sum_index = 0;
	for (int i = small_size - sum.size, j = big.size - sum.size; sum_index < sum.size - 1; i++, j++, sum_index++) {
		if (i < 0) {
			sum[sum_index] = big[j];
		}
		else
			if (j < 0) {
				if (i >= small.size) {
					sum[sum_index] = (10 - carry) % 10;
					carry = -carry >= 0 ? 0 : 1;
				}
				else {
					sum[sum_index] = (-small[i] + 10 - carry) % 10;
					carry = -small[i] - carry >= 0 ? 0 : 1;
				}
			}
			else if (i >= small.size) {
				int temp = big[j] - carry + 10;
				sum[sum_index] = temp % 10;
				carry = big[j] - carry >= 0 ? 0 : 1;
			}
			else {
				int temp = big[j] - small[i] - carry + 10;
				sum[sum_index] = temp % 10;
				carry = big[j] - carry >= small[i] ? 0 : 1;
			}
	}
	if (small_size > small.size) {
		sum[sum_index] = (big[big.size - 1] - carry + 10) % 10;
		carry = (big[big.size - 1] - carry) >= 0 ? 0 : 1;
	}
	else {
		sum[sum_index] = (big[big.size - 1] - carry - small[small.size - 1] + 10) % 10;
		carry = (big[big.size - 1] - carry >= small[small.size - 1] ? 0 : 1);
	}
}

CBigDoubleLittle3 operator+(const CBigDoubleLittle3 & num1, const CBigDoubleLittle3 & num2)
{
	CBigDoubleLittle3 sum;
	int carry = 0;
	if (num1.sign == num2.sign) {
		sum.order = num1.order > num2.order ? num1.order : num2.order;
		int num1_size = num1.size + sum.order - num1.order;
		int num2_size = num2.size + sum.order - num2.order;
		sum.size = num1_size > num2_size ? num1_size : num2_size;
		sum.buf = (int*)malloc(sum.size * sizeof(int));
		int sum_index = 0;
		for (int i = num1_size - sum.size, j = num2_size - sum.size; sum_index < sum.size - 1; i++, j++, sum_index++) {
			if (i < 0) {
				if (j >= num2.size)
					sum[sum_index] = 0;
				else
					sum[sum_index] = num2[j];
			}
			else if (j < 0) {
				if (i >= num1.size)
					sum[sum_index] = 0;
				else
					sum[sum_index] = num1[i];
			}
			else {
				if (j >= num2.size) {
					int temp = num1[i] + carry;
					sum[sum_index] = temp % 10;
					carry = temp / 10;
				}
				else if (i >= num1.size) {
					int temp = num2[i] + carry;
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
		if (num1_size > num1.size)
			temp = num2[num2.size - 1] + carry;
		else if (num2_size > num2.size)
			temp = num1[num1.size - 1] + carry;
		else
			temp = num1[num1.size - 1] + num2[num2.size - 1] + carry;
		sum[sum_index] = temp % 10;
		carry = temp / 10;
		if (carry > 0) {
			sum.size++;
			sum.buf = (int*)realloc(sum.buf, sum.size * sizeof(int));
			sum[sum.size - 1] = carry;
			sum.order++;
		}
		sum.sign = num1.sign;
	}
	else {
		bool num1_is_less;
		if (num1.order != num2.order)
			num1_is_less = num1.order < num2.order;
		else {
			int i, j;
			for (i = num1.size - 1, j = num2.size - 1; i >= 0 && j >= 0; i--, j--) {
				if (num1[i] == num2[j])
					continue;
				else {
					num1_is_less = num1[i] < num2[j];
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
		int i = sum.size - 1;
		while (i > 0 && sum.buf[i--] == 0) {
			sum.size--;
			sum.order--;
			flag = true;
		}
		if (flag)
			sum.buf = (int*)realloc(sum.buf, sum.size * sizeof(int));
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
	result.buf = (int*)malloc(result.size * sizeof(int));
	for (int i = 0; i < result.size; i++)
		result[i] = 0;
	for (int i = 0; i < num2.size; ++i) {
		if (num2[i] != 0) {
			int carry = 0;
			for (int j = 0; j < num1.size; ++j) {
				int temp = num1[j] * num2[i] + result[i + j] + carry;
				result[i + j] = temp % 10;
				carry = temp / 10;
			}
			result[i + num1.size] = carry;
		}
	}
	bool flag = false;
	int i = result.size - 1;
	while (i > 1 && result[i] == 0) {
		result.size--; flag = true;
		i--;
	}
	if (flag) {
		result.buf = (int*)realloc(result.buf, result.size * sizeof(int));
	}
	else {
		result.order++;
	}

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
	int temp, carry = 0;
	int sizeB = 0;
	for (int i = b; i > 0; i = i / 10, sizeB++);
	result.size = a.size + sizeB;
	result.buf = (int*)malloc(result.size * sizeof(int));
	for (int i = 0; i < result.size; i++)
		result[i] = 0;
	int i;
	for (i = 0; i < a.size; i++) {
		temp = a[i] * b + carry;
		carry = temp / 10;
		result[i] = temp - carry * 10;
	}
	result[i] = carry;
	/*bool flag = false;
	i = result.size - 1;
	while (i > 1 && result[i] == 0) {
		result.size--; flag = true;
		i--;
	}
	if (!flag)
		result.order++;*/

	return result;
}

CBigDoubleLittle3 operator/(const CBigDoubleLittle3 & a, const CBigDoubleLittle3 & b)
{
	CBigDoubleLittle3 q;
	q.sign = a.sign == b.sign ? true : false;
	CBigDoubleLittle3 copyA;
	int accuracy = 10;
	int frac_diff = b.size - b.order - (a.size - a.order);
	copyA.size = a.size + 1 + (frac_diff > 0 ? frac_diff : 0) + accuracy;
	copyA.buf = (int*)malloc(copyA.size * sizeof(int));
	int j;
	for (j = 0; j < (frac_diff > 0 ? frac_diff : 0) + accuracy; j++)
		copyA[j] = 0;
	for (int i = 0; i < a.size; i++)
		copyA[j++] = a[i];
	int a_size = a.size + (frac_diff > 0 ? frac_diff  : 0);
	if (copyA[j - 1] == 0) {
		a_size--;
		j--;
		copyA.size--;
	}
	copyA[j] = 0;

	int shift = 0;
	CBigDoubleLittle3 copyB;
	copyB = b;
	q.order = a.order - b.order - 1;
	short scale;
	short qGuess, r;
	short borrow, carry;
	int n = b.size, m = a_size - b.size + accuracy;
	scale = 10 / (b[n - 1] + 1);
	if (scale > 1) {
		copyA = copyA * scale;
		if (copyA[copyA.size - 1] == 0) {
			copyA.size--;
		}
		copyB = copyB * scale;
		if (copyB[copyB.size - 1] == 0) {
			copyB.size--;
		}
	}
	std::cout << copyA << std::endl;
	std::cout << copyB << std::endl;
	if (copyA[copyA.size - 1] != 0)
		q.order++;
	else
		q.order += (copyA[copyA.size - 2] >= copyB[copyB.size - 1] ? 1 : 0);
	q.size = m + 1;
	q.buf = (int*)malloc(q.size * sizeof(int));
	

	int q_ind, a_ind;
	for (q_ind = m, a_ind = a_size + accuracy; q_ind >= 0; q_ind--, a_ind--) {
		if (q_ind + shift == m - q.order - 1 - accuracy) {
			shift++;
			break;
		}
		int temp = (copyA[a_ind] * 10 + copyA[a_ind - 1]);
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
		int *aShift = copyA.buf + q_ind;

		int temp1, temp2;
		for (int i = 0; i < n; i++) {
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
			if (q_ind + shift == m && qGuess == 0) {
				if (copyA[copyA.size - 2] >= copyB[copyB.size - 1])
					q.order--;
				shift++;
				continue;
			}
			q[q_ind + shift] = qGuess;
		}
		else {
			if (q_ind + shift == m && qGuess - 1 == 0) {
				if (copyA[copyA.size - 2] >= copyB[copyB.size - 1])
					q.order--;
				shift++;
			}
			else {
				q[q_ind + shift] = qGuess - 1;
			}
			carry = 0;
			for (int i = 0; i < n; i++) {
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
	std::cout << q << std::endl;
	
	if (shift != 0) {
		int sh = m - q.order - accuracy;
		for (int i = 0; i <= m - sh; i++)
			q[i] = q[i + sh];
		m -= sh;
		q.size = m + 1;
		q.buf = (int*)realloc(q.buf, q.size * sizeof(int));
	}
	/*if (shift != 0) {
		int sh = q.point - accuracy + 1;
		for (int i = 0; i <= m - sh; i++)
			q[i] = q[i + sh];
		m -= sh;
		q.point -= sh;
		q.size = m + 1;
		q.buf = (int*)realloc(q.buf, q.size * sizeof(int));
	}*/
	//else
	//q.size = m + 1;
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
	for (int i = num.size - 1; i >= 0; i--) {
		if (i == num.size - 2)
			stream << '.';
		stream << num[i];
	}
	stream << "e";
	stream << num.order;
	return stream;
}
