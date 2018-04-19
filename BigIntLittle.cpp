#include "BigIntLittle.h"

CBigIntLittle::CBigIntLittle()
{
}

CBigIntLittle::CBigIntLittle(std::string str)
{
	int leng = str.length();
	if (str[0] == '-') {
		this->sign = false;
		this->size = leng - 1;
	}
	else {
		this->sign = true;
		this->size = leng;
	}
	this->buf =(int*) malloc(size * sizeof(int));
	for (int i = 0; i < size; i++)
		*(buf + i) = int(str[leng - i - 1] - '0');
}

int & CBigIntLittle::operator[](const int i)
{
	return buf[i];
}

int CBigIntLittle::operator[](const int i) const
{
	return buf[i];
}

bool operator==(const CBigIntLittle & a, const CBigIntLittle & b)
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

bool operator<(const CBigIntLittle & a, const CBigIntLittle &b) {
	if (a.sign == b.sign) {
		if (a.size != b.size)
			return a.size > b.size ? !a.sign : a.sign;
		else { // a.size() == b.size()
			for (int i = a.size - 1; i >= 0; i--) {
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

bool operator>(const CBigIntLittle & a, const CBigIntLittle &b) {
	if (a.sign == b.sign) {
		if (a.size != b.size)
			return a.size > b.size ? a.sign : !a.sign;
		else { // a.size() == b.size()
			for (int i = a.size - 1; i >= 0; i--) {
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

bool operator >=(const CBigIntLittle &a, const CBigIntLittle &b) {
	return(!(a < b));
}

bool operator <=(const CBigIntLittle &a, const CBigIntLittle &b) {
	return (!(a > b));
}

bool operator !=(const CBigIntLittle &a, const CBigIntLittle &b) {
	return (!(a == b));
}

CBigIntLittle &CBigIntLittle::operator =(const CBigIntLittle &num) {
	free(this->buf);
	this->buf = (int*)malloc(num.size * sizeof(int));
	for (int i = 0; i < num.size; i++)
		this->buf[i] = num[i];
	this->sign = num.sign;
	this->size = num.size;
	return *this;
}

CBigIntLittle operator+(const CBigIntLittle & num1, const CBigIntLittle & num2)
{	
	CBigIntLittle sum;
	int carry = 0;
	if (num1.sign == num2.sign) {
		sum.size = num1.size > num2.size ? num1.size: num2.size;
		sum.buf = (int*) malloc(sum.size * sizeof(int));
		for (int i = 0; i < sum.size; i++) {
			int temp;
			if (i >= num1.size) 
				temp = num2[i] + carry;
			else 
				if (i >= num2.size) 
					temp = num1[i] + carry;
				else
					temp = num1[i] + num2[i] + carry;
			sum.buf[i] = temp % 10;
			carry = temp / 10;
		}
		if (carry > 0) {
			sum.size++;
			sum.buf = (int*)realloc(sum.buf, sum.size * sizeof(int));
			sum.buf[sum.size - 1] = carry;
		}
		sum.sign = num1.sign;
	}
	else {
		CBigIntLittle _num1, _num2;
		_num1 = num1, _num2 = num2;
		_num1.sign = true;
		_num2.sign = true;
		bool num1_is_less = (_num1 < _num2);
		if (num1_is_less) {
			CBigIntLittle temp;
			temp = _num1;
			_num1 = _num2;
			_num2 = temp;
			temp.deleteNumber();
		}
		sum.size = _num1.size;
		sum.buf = (int*) malloc(sum.size * sizeof(int));
		for (int i = 0; i < _num1.size; i++) {
			if (i >= _num2.size) {
				sum.buf[i] = (_num1[i] - carry + 10) % 10;
				carry = (_num1[i] - carry) >= 0 ? 0 : 1;
				continue;
			}
			sum.buf[i] = (_num1[i] - carry - _num2[i] + 10) % 10;
			carry = (_num1[i] - carry >= _num2[i] ? 0 : 1);
		}
		sum.sign = num1_is_less ? num2.sign : num1.sign;
		bool flag = false;
		while (sum.size > 1 && sum.buf[sum.size - 1] == 0) {
			sum.size--;
			flag = true;
		}
		if (flag)
			sum.buf = (int*)realloc(sum.buf, sum.size * sizeof(int));
		_num1.deleteNumber(); _num2.deleteNumber();
	}
	return sum;
}

CBigIntLittle operator-(const CBigIntLittle & num1, CBigIntLittle & num2)
{
	num2.sign = false;
	CBigIntLittle res;
	res = num1 + num2;
	return res;
}

CBigIntLittle operator*(const CBigIntLittle &num1, const CBigIntLittle &num2) {
	CBigIntLittle result;
	result.size = num1.size + num2.size;
	result.buf = (int*) malloc(result.size * sizeof(int));
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
	while (result.size > 1 && result[result.size - 1] == 0) {
		result.size--;
		flag = true;
	}
	if (flag) {
		result.buf = (int*)realloc(result.buf, result.size * sizeof(int));
	}
	result.sign = num1.sign == num2.sign ? true : false;
	return result;
}

CBigIntLittle operator*(const CBigIntLittle & a, const int & c) {
	if (c == 0)
		return CBigIntLittle("0");
	CBigIntLittle result;
	int b = c;
	result.sign = (a.sign && b > 0) || (!a.sign && b < 0) ? true : false;
	if (b < 0) b *= (-1);
	int temp, carry = 0;
	int sizeB = 0;
	for (int i = b; i > 0; i = i / 10, sizeB++);
	result.size = a.size + sizeB;
	result.buf =(int*) malloc(result.size * sizeof(int));
	for (int i = 0; i < result.size; i++)
		result[i] = 0;
	int i;
	for (i = 0; i < a.size; i++) {
		temp = a[i] * b + carry;
		carry = temp / 10;
		result[i] = temp - carry * 10;
	}
	while (carry > 0) {
		result[i++] = carry % 10;
		carry /= 10;
	}
	/*bool flag = false;
	if (result[result.size - 1] == 0) {
		result.size--;
		flag = true;
	}
	if (flag) {
		result.buf = (int*)realloc(result.buf, result.size * sizeof(int));
	}	*/
	return result;
}

CBigIntLittle operator/(const CBigIntLittle & a, const CBigIntLittle & b)
{
	if (a.size < b.size)
		return CBigIntLittle("0");
	if (a.size == b.size) {
		for (int i = a.size - 1; i >= 0; i--) {
			if (a[i] == b[i])
				continue;
			else
				if (a[i] < b[i])
					return CBigIntLittle("0");
				else break;
		}
	}
	CBigIntLittle q;
	q.sign = a.sign == b.sign ? true : false;
	CBigIntLittle copyA;
	copyA.size = a.size + 1;
	copyA.buf = (int*) malloc (copyA.size * sizeof(int));
	for (int i = 0; i < copyA.size- 1; i++) 
		copyA[i] = a[i];
	copyA[copyA.size - 1] = 0;
	int shift = 0;
	CBigIntLittle copyB;
	copyB = b;

	short scale;
	short qGuess, r;
	short borrow, carry;
	int n = b.size, m = a.size - b.size;
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
	q.size = m + 1;
	q.buf = (int*) malloc(q.size * sizeof(int));
	int q_ind, a_ind;
	for (q_ind = m, a_ind = n + q_ind; q_ind >= 0; q_ind--, a_ind--) {
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
				shift++;
				continue;
			}
			q[q_ind + shift] = qGuess;
		}
		else {
			if (q_ind + shift == m && qGuess - 1 == 0) {
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
	/*while (copyA.size > 1 && (copyA[copyA.size - 1] == 0))
		copyA.size--;*/
	copyA.deleteNumber();
	copyB.deleteNumber();
	if (shift != 0) {
		for (int i = 0; i <= m - shift; i++)
			q[i] = q[i + shift];
		m -= shift;
		q.size = m + 1;
		q.buf = (int*)realloc(q.buf, q.size * sizeof(int));
	}
	else	
		q.size = m + 1;
		
	return q;
}

std::istream &operator >> (std::istream &stream, CBigIntLittle &num) {
	std::string str;
	stream >> str;
	CBigIntLittle _int(str);
	num = _int;
	return stream;
}
std::ostream &operator<<(std::ostream &stream, const CBigIntLittle &num) {
	if (!num.sign)
		stream << '-';
	for (int i = num.size - 1; i >= 0; i--) {
		stream << num[i];
	}
	return stream;
}
