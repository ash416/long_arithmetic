#include "BigDoubleBig.h"

CBigDoubleBig::CBigDoubleBig()
{
}

CBigDoubleBig::CBigDoubleBig(std::string str)
{
	int j = 0;
	if (str[0] == '-') {
		this->sign = false;
		this->size = str.length() - 2;
		j = 1;
	}
	else {
		this->sign = true;
		this->size = str.length() - 1;
	}
	this->buf = (int*)malloc(size * sizeof(int));
	for (int i = 0; i < size; i++) {
		if (str[j] == '.') {
			j++;
			i--;
			this->point = i + 1;
			continue;
		}
		*(buf + i) = int(str[j++] - '0');
	}
}

int & CBigDoubleBig::operator[](const int i)
{
	return buf[i];
}

int CBigDoubleBig::operator[](const int i) const
{
	return buf[i];
}

bool operator==(const CBigDoubleBig & a, const CBigDoubleBig & b)
{
	if (a.size != b.size || a.sign != b.sign || a.point != b.point)
		return false;
	else {
		for (int i = 0; i < a.size; i++)
			if (a[i] != b[i])
				return false;
		return true;
	}
}

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

CBigDoubleBig &CBigDoubleBig::operator =(const CBigDoubleBig &num) {
	free(this->buf);
	this->buf = (int*)malloc(num.size * sizeof(int));
	for (int i = 0; i < num.size; i++)
		this->buf[i] = num[i];
	this->point = num.point;
	this->sign = num.sign;
	this->size = num.size;
	return *this;
}

void CBigDoubleBig::differences(CBigDoubleBig & sum, const CBigDoubleBig & big, const CBigDoubleBig & small)
{
	int carry = 0;
	int frac1 = big.size - big.point, frac2 = small.size - small.point;
	int frac_size = frac1 > frac2 ? frac1 : frac2;
	sum.point = small.point > big.point ? small.point : big.point;
	sum.size = frac_size + (small.point > big.point ? small.point : big.point);
	sum.buf = (int*)malloc(sum.size * sizeof(int));
	sum.buf = (int*)malloc(sum.size * sizeof(int));
	int sum_index = sum.size - 1;
	for (int i = small.point + frac_size - 1, j = big.point + frac_size - 1; i >= small.point, j >= big.point; i--, j--) {
		if (i >= small.size) sum[sum_index--] = big[j];
		else if (j >= big.size) {
			sum[sum_index--] = (-small[i] + 10 - carry) % 10;
			carry = -small[i] - carry >= 0 ? 0 : 1;
		}
		else {
			int temp = big[j] - small[i] - carry + 10;
			sum[sum_index--] = temp % 10;
			carry = big[j] - carry >= small[i] ? 0 : 1;
		}
	}
	for (int i = small.point - 1, j = big.point - 1; sum_index >= 0; i--, j--, sum_index--) {
		if (i < 0) {
			sum[sum_index] = (big[j] - carry + 10) % 10;
			carry = (big[j] - carry) >= 0 ? 0 : 1;
		}
		else {
			sum[sum_index] = (big[j] - carry - small[i] + 10) % 10;
			carry = (big[j] - carry >= small[i] ? 0 : 1);
		}
	}
}

CBigDoubleBig operator+(const CBigDoubleBig & num1, const CBigDoubleBig & num2)
{
	CBigDoubleBig sum;
	int carry = 0;
	if (num1.sign == num2.sign) {
		int frac1 = num1.size - num1.point, frac2 = num2.size - num2.point;
		int frac_size = frac1 > frac2 ? frac1 : frac2;
		sum.point = num1.point > num2.point ? num1.point : num2.point;
		sum.size = frac_size + (num1.point > num2.point ? num1.point : num2.point);
		sum.buf = (int*)malloc(sum.size * sizeof(int));
		int sum_index = sum.size - 1;
		for (int i = num1.point + frac_size - 1, j = num2.point + frac_size - 1; i >= num1.point, j >= num2.point; i--, j--) {
			if (i >= num1.size) sum[sum_index--] = num2[j];
			else if (j >= num2.size) sum[sum_index--] = num1[i];
			else {
				int temp = num1[i] + num2[j] + carry;
				sum[sum_index--] = temp % 10;
				carry = temp / 10;
			}
		}
		for(int i = num1.point - 1, j = num2.point - 1; sum_index >= 0; i--, j--, sum_index--){
			int temp;
			if (i < 0)
				temp = num2[j] + carry;
			else
				if (j < 0)
					temp = num1[i] + carry;
				else
					temp = num1[i] + num2[j] + carry;
			sum[sum_index] = temp % 10;
			carry = temp / 10;
		}
		if (carry > 0) {
			sum.buf = (int*)realloc(sum.buf, (sum.size + 1) * sizeof(int));
			for (int i = sum.size; i > 0; i--)
				sum[i] = sum[i - 1];
			sum.point++;
			sum[0] = carry;
			sum.size++;
		}
		sum.sign = num1.sign;
	}
	else {
		bool num1_is_less;
		if (num1.size != num2.size)
			num1_is_less = num1.size > num2.size ? false : true;
		else {
			int i;
			for (i = 0; i < num1.size; i++) {
				if (num1[i] == num2[i])
					continue;
				else {
					num1_is_less = (num1[i] < num2[i]);
					break;
				}
			}
			if (i == num1.size)
				num1_is_less = false;
		}
		if (num1_is_less) sum.differences(sum, num2, num1);
		else sum.differences(sum, num1, num2);
		sum.sign = num1_is_less ? num2.sign : num1.sign;
		int i = 0;
		while (i < sum.point - 1 && sum[i] == 0) {
			sum.size--; i++;
		}
		if (i > 0) {
			for (int j = 0; j < sum.size - i + 1; j++)
				sum[j] = sum[j + i];
			sum.buf = (int*)realloc(sum.buf, sum.size * sizeof(int));
		}
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
	result.point = result.size - (num1.size + num2.size - num1.point - num2.point);
	int i = 0;
	while (i < result.point - 1 && result[i] == 0) {
		result.size--;
		i++;
	}
	if (i > 0) {
		for (int j = 0; j <= result.size - 1; j++) {
			result[j] = result[j + i];
		}
		result.buf = (int*)realloc(result.buf, result.size * sizeof(int));
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
	while (carry > 0) {
		result[i--] = carry % 10;
		carry /= 10;
	}
	return result;
}


CBigDoubleBig operator/(const CBigDoubleBig & a, const CBigDoubleBig & b)
{
	CBigDoubleBig q;
	q.sign = a.sign == b.sign ? true : false;
	CBigDoubleBig copyA;
	int accuracy = 10;
	int frac_diff = b.size - b.point - (a.size - a.point);
	copyA.size = a.size + 1 + (frac_diff > 0 ? frac_diff : 0) + accuracy;
	copyA.buf = (int*)malloc(copyA.size * sizeof(int));
	copyA[0] = 0;
	int i, j;
	for (i = 1; i <= a.size; i++)
		copyA[i] = a[i - 1];
	for (i = a.size + 1; i < copyA.size; i++)
		copyA[i] = 0;
	int a_size = a.size + frac_diff;
	
	int shift = 0;
	CBigDoubleBig copyB;
	copyB = b;

	short scale;
	short qGuess, r;
	short borrow, carry;
	int diff = a.point - b.point;
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
	if (copyA[0] != 0)
		diff++;
	else
		diff += (copyA[1] >= copyB[0] ? 1 : 0);
	int *aShift = copyA.buf + i;
	int *bShift = copyB.buf + j;
	q.size = m + 1;
	q.buf = (int*)malloc(q.size * sizeof(int));
	q.point = diff;
	
	int q_ind, a_ind;
	for (q_ind = 0, a_ind = 0; q_ind <= m; q_ind++, a_ind++) {
		if (q_ind - shift == q.point + accuracy) {
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
				if (copyA[1] >= copyB[0])
					q.point--;
				shift++;
				continue;
			}
			q[q_ind - shift] = qGuess;
		}
		else {
			if (q_ind - shift == 0 && qGuess - 1 == 0) {
				if (copyA[1] >= copyB[0])
					q.point--;
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

	m -= shift;
	q.size = m + 1;
	if (shift != 0) {
		q.buf = (int*)realloc(q.buf, q.size * sizeof(int));
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
	for (int i = 0; i < num.size; i++) {
		if (i == num.point)
			stream << ".";
		stream << num[i];
	}
	return stream;
}
