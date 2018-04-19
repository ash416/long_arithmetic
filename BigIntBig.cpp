#include "BigIntBig.h"

CBigIntBig::CBigIntBig()
{
}

CBigIntBig::CBigIntBig(std::string str)
{
	int j = 0;
	if (str[0] == '-') {
		this->sign = false;
		this->size = str.length() - 1;
		j = 1;
	}
	else {
		this->sign = true;
		this->size = str.length();
	}
	this->buf = (int*)malloc(size * sizeof(int));
	for (int i = 0; i < size; i++)
		*(buf + i) = int(str[j++] - '0');
}

int & CBigIntBig::operator[](const int i)
{
	return buf[i];
}

int CBigIntBig::operator[](const int i) const
{
	return buf[i];
}

bool operator==(const CBigIntBig & a, const CBigIntBig & b)
{
	if (a.size != b.size || a.sign != b.sign)
		return false;
	else {
		for (int i = 0; i < a.size; i++)
			if (a[i] != b[i])
				return false;
		return true;
	}
}

bool operator<(const CBigIntBig & a, const CBigIntBig &b) {
	if (a.sign == b.sign) {
		if (a.size != b.size)
			return a.size > b.size ? !a.sign : a.sign;
		else { // a.size() == b.size()
			for (int i = 0; i < a.size; i++) {
				if (a[i] == b[i])
					continue;
				else
					return a.sign ? (a[i] < b[i]) : !(a[i] < b[i]);
			}
			return false;
		}
	}
	else {
		return !a.sign;
	}
}

bool operator>(const CBigIntBig & a, const CBigIntBig &b) {
	if (a.sign == b.sign) {
		if (a.size != b.size)
			return a.size > b.size ? a.sign : !a.sign;
		else { // a.size() == b.size()
			for (int i = 0; i < a.size; i++) {
				if (a[i] == b[i])
					continue;
				else
					return !a.sign ? (a[i] < b[i]) : !(a[i] < b[i]);
			}
			return false;
		}
	}
	else {
		return a.sign;
	}
}

bool operator >=(const CBigIntBig &a, const CBigIntBig &b) {
	return(!(a < b));
}

bool operator <=(const CBigIntBig &a, const CBigIntBig &b) {
	return (!(a > b));
}

bool operator !=(const CBigIntBig &a, const CBigIntBig &b) {
	return (!(a == b));
}

CBigIntBig &CBigIntBig::operator =(const CBigIntBig &num) {
	free(this->buf);
	this->buf = (int*)malloc(num.size * sizeof(int));
	for (int i = 0; i < num.size; i++)
		this->buf[i] = num[i];
	this->sign = num.sign;
	this->size = num.size;
	return *this;
}

CBigIntBig operator+(const CBigIntBig & num1, const CBigIntBig & num2)
{
	CBigIntBig sum;
	int carry = 0;
	if (num1.sign == num2.sign) {
		sum.size = num1.size > num2.size ? num1.size : num2.size;
		sum.buf = (int*) malloc(sum.size * sizeof(int));
		int i = num1.size - 1, j = num2.size - 1, sum_index = sum.size - 1;
		while (i >= 0 || j >= 0) {
			int temp;
			if (i < 0)
				temp = num2[j] + carry;
			else
				if (j < 0)
					temp = num1[i] + carry;
				else
					temp = num1[i] + num2[j] + carry;
			sum[sum_index--] = temp % 10;
			carry = temp / 10;
			i--; j--;
		}
		if (carry > 0) {
			sum.buf = (int*)realloc(sum.buf, (sum.size + 1) * sizeof(int));
			for (int i = sum.size; i > 0; i--)
				sum[i] = sum[i - 1];
			sum[0] = carry;
			sum.size++;
		}
		sum.sign = num1.sign;
	}
	else {
		CBigIntBig _num1, _num2;
		_num1 = num1; _num2 = num2;
		_num1.sign = true;
		_num2.sign = true;
		bool num1_is_less = (_num1 < _num2);
		if (num1_is_less) {
			CBigIntBig temp;
			temp = _num1;
			_num1 = _num2;
			_num2 = temp;
			temp.deleteNumber();
		}
		sum.size = _num1.size;
		sum.buf = (int*) malloc(sum.size * sizeof(int));
		int sum_index = sum.size - 1, i = _num1.size - 1, j =_num2.size - 1;
		while (i >= 0) {
			if (j < 0) {
				sum[sum_index--] = (_num1[i] - carry + 10) % 10;
				carry = (_num1[i--] - carry) >= 0 ? 0 : 1;
				continue;
			}
			sum[sum_index--] = (_num1[i] - carry - _num2[j] + 10) % 10;
			carry = (_num1[i] - carry >= _num2[j] ? 0 : 1);
			i--; j--;
		}
		sum.sign = num1_is_less ? num2.sign : num1.sign;
		i = 0;
		while (sum.size > 1 && sum.buf[i] == 0) {
			sum.size--; i++;
		}
		if (i > 0) {
			for (int j = 0; j < sum.size - i + 1; j++)
				sum[j] = sum[j + i];
			sum.buf = (int*)realloc(sum.buf, sum.size * sizeof(int));
		}
		_num1.deleteNumber();
		_num2.deleteNumber();
	}
	return sum;
}

CBigIntBig operator-(const CBigIntBig & num1, CBigIntBig & num2)
{
	num2.sign = false;
	CBigIntBig res;
	res = num1 + num2;
	return res;
}

CBigIntBig operator*(const CBigIntBig &num1, const CBigIntBig &num2) {
	CBigIntBig result;
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
	int i = 0;
	while (result.size > 1 && result[i] == 0) {
		result.size--;
		i++;
	}
	if (i > 0) {
		for (int j = 0; j < result.size - i + 1; j++)
			result[j] = result[j + i];
		result.buf = (int*)realloc(result.buf, result.size * sizeof(int));
	}
	result.sign = num1.sign == num2.sign ? true : false;
	return result;
}

CBigIntBig operator*(const CBigIntBig & a, const int & c) {
	if (c == 0)
		return CBigIntBig("0");
	CBigIntBig result;
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
	/*i = 0;
	if (result[i] == 0 && result.size > 1) {
		result.size--;
		i++;
	}
	if (i > 0) {
		for (int j = 0; j < result.size - i + 1; j++)
			result[j] = result[j + i];
		result.buf = (int*)realloc(result.buf, result.size * sizeof(int));
	}*/

	return result;
}


CBigIntBig operator/(const CBigIntBig & a, const CBigIntBig & b)
{
	if (a.size < b.size)
		return CBigIntBig("0");
	if (a.size == b.size) {
		for (int i = 0; i < a.size; i++) {
			if (a[i] == b[i])
				continue;
			else
				if (a[i] < b[i])
					return CBigIntBig("0");
				else break;
		}
	}
	CBigIntBig q;
	q.sign = a.sign == b.sign ? true : false;
	CBigIntBig copyA;
	copyA.size = a.size + 1;
	copyA.buf = (int*) malloc(copyA.size * sizeof(int));
	copyA[0] = 0;
	for (int i = 1; i < copyA.size; i++)
		copyA[i] = a[i - 1];
	int shift = 0;
	CBigIntBig copyB;
	copyB = b;

	short scale;
	short qGuess, r;
	short borrow, carry;
	int n = b.size, m = a.size - b.size;
	scale = 10 / (b[0] + 1);
	int i = 0, j = 0;
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
	int *aShift = copyA.buf + i;
	int *bShift = copyB.buf + j;
	q.size = m + 1;
	q.buf = (int*)malloc(q.size * sizeof(int));
	int q_ind, a_ind;
	for (q_ind = 0, a_ind = 0; q_ind <= m; q_ind++, a_ind++) {
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
				shift++;
				continue;
			}
			q[q_ind - shift] = qGuess;
		}
		else {
			if (q_ind - shift == 0 && qGuess - 1 == 0) {
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
	if (shift != 0)
		q.buf = (int*)realloc(q.buf, q.size * sizeof(int));
	
	return q;
}

std::istream &operator >> (std::istream &stream, CBigIntBig &num) {
	std::string str;
	stream >> str;
	CBigIntBig _int(str);
	num = _int;
	return stream;
}
std::ostream &operator<<(std::ostream &stream, const CBigIntBig &num) {
	if (!num.sign)
		stream << '-';
	for (int i = 0; i < num.size; i++) {
		stream << num[i];
	}
	return stream;
}
