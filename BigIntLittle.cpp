#include "BigIntLittle.h"

CBigIntLittle::CBigIntLittle()
{
}

CBigIntLittle::CBigIntLittle(std::string str)
{
	size_t leng = str.length();
	if (str[0] == '-') {
		this->sign = false;
		this->size = leng - 1;
	}
	else {
		this->sign = true;
		this->size = leng;
	}
	this->buf = (AtomicT*) malloc(size);
	for (size_t i = 0; i < size; i++)
		buf[i] = str[leng - i - 1] - '0';
}

CBigIntLittle::AtomicT & CBigIntLittle::operator[](const size_t i)
{
	return buf[i];
}

CBigIntLittle::AtomicT CBigIntLittle::operator[](const size_t i) const
{
	return buf[i];
}

bool operator==(const CBigIntLittle & a, const CBigIntLittle & b)
{
	if (a.size != b.size || a.sign != b.sign)
		return false;
	else {
		for (size_t i = 0; i < a.size; i++) 
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
			for (size_t i = a.size - 1; i >= 0; i--) {
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
			for (size_t i = a.size - 1; i >= 0; i--) {
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
	this->buf = (AtomicT*)malloc(num.size);
	for (size_t i = 0; i < num.size; i++)
		this->buf[i] = num.buf[i];
	this->sign = num.sign;
	this->size = num.size;
	return *this;
}

CBigIntLittle operator+(const CBigIntLittle & num1, const CBigIntLittle & num2) {	
	CBigIntLittle sum;
	clock_t t = std::clock();
	CBigIntLittle::AtomicT carry = 0;
	if (num1.sign == num2.sign) {
		sum.size = num1.size > num2.size ? num1.size: num2.size;
		sum.buf = (CBigIntLittle::AtomicT*) malloc(sum.size);
		for (size_t i = 0; i < sum.size; i++) {
			CBigIntLittle::AtomicT temp;
			if (i >= num1.size) 
				temp = num2.buf[i] + carry;
			else
				if (i >= num2.size) 
					temp = num1.buf[i] + carry;
				else
					temp = num1.buf[i] + num2.buf[i] + carry;
			sum.buf[i] = temp % 10;
			carry = temp / 10;
		}
		std::cout << " 1: " << std::clock() - t << " ";
		t = std::clock();
		if (carry > 0) {
			sum.size++;
			sum.buf = (CBigIntLittle::AtomicT*)realloc(sum.buf, sum.size);
			sum.buf[sum.size - 1] = carry;
		}
		std::cout << " 2: " << std::clock() - t << " ";
		sum.sign = num1.sign;
	}
	else {
		bool num1_is_less;
		if (num1.size != num2.size)
			num1_is_less = num1.size > num2.size ? false : true;
		else { 
			size_t i;
			for (i = num1.size - 1; i >= 0; i--) {
				if (num1.buf[i] == num2.buf[i])
					continue;
				else {
					num1_is_less = (num1.buf[i] < num2.buf[i]);
					break;
				}
			}
			if (i < 0)
				num1_is_less = false;
		}
		if (num1_is_less) {
			sum.size = num2.size;
			sum.buf = (CBigIntLittle::AtomicT*)malloc(sum.size);
			for (size_t i = 0; i < num2.size; i++) {
				if (i >= num1.size) {
					sum.buf[i] = (num2.buf[i] - carry + 10) % 10;
					carry = (num2.buf[i] - carry) >= 0 ? 0 : 1;
					continue;
				}
				sum.buf[i] = (num2.buf[i] - carry - num1.buf[i] + 10) % 10;
				carry = (num2.buf[i] - carry >= num1.buf[i] ? 0 : 1);
			}
		}
		else {
			sum.size = num1.size;
			sum.buf = (CBigIntLittle::AtomicT*)malloc(sum.size);
			for (size_t i = 0; i < num1.size; i++) {
				if (i >= num2.size) {
					sum.buf[i] = (num1.buf[i] - carry + 10) % 10;
					carry = (num1.buf[i] - carry) >= 0 ? 0 : 1;
					continue;
				}
				sum.buf[i] = (num1.buf[i] - carry - num2.buf[i] + 10) % 10;
				carry = (num1.buf[i] - carry >= num2.buf[i] ? 0 : 1);
			}
		}


		sum.sign = num1_is_less ? num2.sign : num1.sign;
		bool flag = false;
		while (sum.size > 1 && sum.buf[sum.size - 1] == 0) {
			sum.size--;
			flag = true;
		}
		if (flag)
			sum.buf = (CBigIntLittle::AtomicT*)realloc(sum.buf, sum.size);
		
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
	clock_t t = clock();
	result.size = num1.size + num2.size;
	result.buf = (CBigIntLittle::AtomicT*) malloc(result.size);
	for (size_t i = 0; i < result.size; i++)
		result.buf[i] = 0;
	for (size_t i = 0; i < num2.size; ++i) {	
		if (num2.buf[i] != 0) {
			CBigIntLittle::AtomicT carry = 0;
			for (size_t j = 0; j < num1.size; ++j) {
				CBigIntLittle::AtomicT temp = num1.buf[j] * num2.buf[i] + result.buf[i + j] + carry;
				result.buf[i + j] = temp % 10;
				carry = temp / 10;
			}
			result.buf[i + num1.size] = carry;
		}
	}
	std::cout << " 1: " << std::to_string(double(clock() - t) / CLOCKS_PER_SEC * 1000) << "; ";
	t = clock();
	bool flag = false;
	while (result.size > 1 && result[result.size - 1] == 0) {
		result.size--;
		flag = true;
	}
	std::cout << " 2: " << std::to_string(double(clock() - t) / CLOCKS_PER_SEC * 1000) << "; ";
	t = clock();
	if (flag) {
		result.buf = (CBigIntLittle::AtomicT*)realloc(result.buf, result.size);
	}
	result.sign = num1.sign == num2.sign ? true : false;
	std::cout << " 3: " << std::to_string(double(clock() - t) / CLOCKS_PER_SEC * 1000) << "; ";
	return result;
}

CBigIntLittle operator*(const CBigIntLittle & a, const int & c) {
	if (c == 0)
		return CBigIntLittle("0");
	CBigIntLittle result;
	int b = c;
	result.sign = (a.sign && b > 0) || (!a.sign && b < 0) ? true : false;
	if (b < 0) b *= (-1);
	CBigIntLittle::AtomicT temp, carry = 0;
	size_t sizeB = 0;
	for (size_t i = b; i > 0; i = i / 10, sizeB++);
	result.size = a.size + sizeB;
	result.buf =(CBigIntLittle::AtomicT*) malloc(result.size);
	for (size_t i = 0; i < result.size; i++)
		result[i] = 0;
	size_t i;
	for (i = 0; i < a.size; i++) {
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

CBigIntLittle operator/(const CBigIntLittle & a, const CBigIntLittle & b)
{
	clock_t t = clock();
	if (a.size < b.size)
		return CBigIntLittle("0");
	if (a.size == b.size) {
		for (size_t i = a.size - 1; i >= 0; i--) {
			if (a[i] == b[i])
				continue;
			else
				if (a[i] < b[i])
					return CBigIntLittle("0");
				else break;
		}
	}
	std::cout << " 1: " << std::to_string(clock() - t) << "; ";
	t = clock();
	CBigIntLittle q;
	q.sign = a.sign == b.sign ? true : false;
	CBigIntLittle copyA;
	copyA.size = a.size + 1;
	copyA.buf = (CBigIntLittle::AtomicT*) malloc (copyA.size);
	for (size_t i = 0; i < copyA.size- 1; i++) 
		copyA[i] = a[i];
	copyA[copyA.size - 1] = 0;
	size_t shift = 0;
	CBigIntLittle copyB;
	copyB = b;

	CBigIntLittle::AtomicT scale;
	CBigIntLittle::AtomicT qGuess, r;
	CBigIntLittle::AtomicT borrow, carry;
	size_t n = b.size, m = a.size - b.size;
	std::cout << " 2: " << std::to_string(clock() - t) << "; ";
	t = clock();
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
	std::cout << " 3: " << std::to_string(clock() - t) << "; ";
	CBigIntLittle::AtomicT *aSh = copyA.buf;
	CBigIntLittle::AtomicT *bSh = copyB.buf;
	q.size = m + 1;
	q.buf = (CBigIntLittle::AtomicT*) malloc(q.size);
	size_t q_ind, a_ind;
	t = clock();
	for (q_ind = m, a_ind = n + q_ind; q_ind >= 0; q_ind--, a_ind--) {
		CBigIntLittle::AtomicT temp = (aSh[a_ind] * 10 + aSh[a_ind - 1]);
		qGuess = temp / bSh[n - 1];
		r = temp % bSh[n - 1];
		while (r < 10) {
			if (bSh[n - 2] * qGuess > r * 10 + aSh[a_ind - 2] || qGuess == 10) {
				qGuess--;
				r += bSh[n - 1];
			}
			else break;
		}
		carry = 0; 
		borrow = 0;
		CBigIntLittle::AtomicT *aShift = &aSh[0] + q_ind;

		CBigIntLittle::AtomicT temp1, temp2;
		for (size_t i = 0; i < n; i++) {
			temp1 = bSh[i] * qGuess + carry;
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
			for (size_t i = 0; i < n; i++) {
				temp = aShift[i] + bSh[i] + carry;
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
	std::cout << " 4: " << std::to_string(clock() - t) << "; ";
	t = clock();
	copyA.deleteNumber();
	copyB.deleteNumber();
	std::cout << " 5: " << std::to_string(clock() - t) << "; ";
	t = clock();
	if (shift != 0) {
		std::cout << " shift point - >";
		for (size_t i = 0; i <= m - shift; i++)
			q[i] = q[i + shift];
		m -= shift;
		q.size = m + 1;
		q.buf = (CBigIntLittle::AtomicT*)realloc(q.buf, q.size);
	}
	else	
		q.size = m + 1;
	std::cout << " 6: " << std::to_string(clock() - t) << "; ";
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
	for (size_t i = num.size - 1; i >= 0; i--) {
		stream << num[i];
	}
	return stream;
}
