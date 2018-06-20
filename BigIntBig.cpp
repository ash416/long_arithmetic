#include "BigIntBig.h"

CBigIntBig::CBigIntBig()
{
}

CBigIntBig::CBigIntBig(std::string str)
{
	size_t j = 0;
	if (str[0] == '-') {
		this->sign = false;
		this->size = str.length() - 1;
		j = 1;
	}
	else {
		this->sign = true;
		this->size = str.length();
	}
	this->buf = (AtomicT*)malloc(size);
	for (size_t i = 0; i < size; i++)
		buf[i] = str[j++] - '0';
}

CBigIntBig::AtomicT & CBigIntBig::operator[](const size_t i)
{
	return buf[i];
}

CBigIntBig::AtomicT CBigIntBig::operator[](const size_t i) const
{
	return buf[i];
}

bool operator==(const CBigIntBig & a, const CBigIntBig & b)
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

bool operator<(const CBigIntBig & a, const CBigIntBig &b) {
	if (a.sign == b.sign) {
		if (a.size != b.size)
			return a.size > b.size ? !a.sign : a.sign;
		else { // a.size() == b.size()
			for (size_t i = 0; i < a.size; i++) {
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
			for (size_t i = 0; i < a.size; i++) {
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
	this->buf = (CBigIntBig::AtomicT*)malloc(num.size);
	for (size_t i = 0; i < num.size; i++)
		this->buf[i] = num.buf[i];
	this->sign = num.sign;
	this->size = num.size;
	return *this;
}

CBigIntBig operator+(const CBigIntBig & num1, const CBigIntBig & num2)
{
	CBigIntBig sum;
	clock_t t = std::clock();
	CBigIntBig::AtomicT carry = 0;
	if (num1.sign == num2.sign) {
		sum.size = num1.size > num2.size ? num1.size : num2.size;
		sum.buf = (CBigIntBig::AtomicT*) malloc(sum.size);
		for (int i = num1.size - 1, j = num2.size - 1, sum_index = sum.size - 1; sum_index >= 0; i--, j--, sum_index--){
			CBigIntBig::AtomicT temp;
			if (i < 0)
				temp = num2.buf[j] + carry;
			else
				if (j < 0)
					temp = num1.buf[i] + carry;
				else
					temp = num1.buf[i] + num2.buf[j] + carry;
			sum.buf[sum_index] = temp % 10;
			carry = temp / 10;
		}
		std::cout << " 1: " << std::clock() - t << " ";
		t = std::clock();
		if (carry > 0) {
			std::cout << " --- ";
			sum.buf = (CBigIntBig::AtomicT*)realloc(sum.buf, (sum.size + 1));
			for (size_t i = sum.size; i > 0; i--)
				sum.buf[i] = sum.buf[i - 1];
			sum.buf[0] = carry;
			sum.size++;
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
			for (i = 0; i < num1.size; i++) {
				if (num1.buf[i] == num2.buf[i])
					continue;
				else {
					num1_is_less = (num1.buf[i] < num2.buf[i]);
					break;
				}
			}
			if (i >= num1.size)
				num1_is_less = false;
		}
		if (num1_is_less) {
			sum.size = num2.size;
			sum.buf = (CBigIntBig::AtomicT*)malloc(sum.size);
			for (size_t i = num2.size - 1, j = num1.size - 1, sum_index = sum.size - 1; i >= 0; i--, j--, sum_index--) {
				if (j < 0) {
					sum.buf[sum_index] = (num2.buf[i] - carry + 10) % 10;
					carry = (num2.buf[i] - carry) >= 0 ? 0 : 1;
					continue;
				}
				sum.buf[sum_index] = (num2.buf[i] - carry - num1.buf[j] + 10) % 10;
				carry = (num2.buf[i] - carry >= num1.buf[j] ? 0 : 1);
			}
		}
		else {
			sum.size = num1.size;
			sum.buf = (CBigIntBig::AtomicT*)malloc(sum.size);
			for (size_t i = num1.size - 1, j = num2.size - 1, sum_index = sum.size - 1; i >= 0; i--, j--, sum_index--) {
				if (j < 0) {
					sum.buf[sum_index] = (num1.buf[i] - carry + 10) % 10;
					carry = (num1.buf[i] - carry) >= 0 ? 0 : 1;
					continue;
				}
				sum.buf[sum_index] = (num1.buf[i] - carry - num2.buf[j] + 10) % 10;
				carry = (num1.buf[i] - carry >= num2.buf[j] ? 0 : 1);
			}
		}
		sum.sign = num1_is_less ? num2.sign : num1.sign;
		size_t i = 0;
		while (sum.size > 1 && sum.buf[i] == 0) {
			sum.size--; i++;
		}
		if (i > 0) {
			for (size_t j = 0; j < sum.size; j++)
				sum.buf[j] = sum.buf[j + i];
			sum.buf = (CBigIntBig::AtomicT*)realloc(sum.buf, sum.size);
		}
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
	clock_t t = clock();
	result.size = num1.size + num2.size;
	result.buf = (CBigIntBig::AtomicT*)malloc(result.size);
	for (size_t i = 0; i < result.size; i++)
		result.buf[i] = 0;
	for (size_t i = num2.size - 1; i >= 0; i--) {
		if (num2.buf[i] != 0) {
			CBigIntBig::AtomicT carry = 0;
			for (size_t j = num1.size - 1; j >= 0; j--) {
				CBigIntBig::AtomicT temp = num1.buf[j] * num2.buf[i] + result.buf[i + j + 1] + carry;
				result.buf[i + j + 1] = temp % 10;
				carry = temp / 10;
			}
			result.buf[i] = carry;
		}
	}
	std::cout << " 1: " << std::to_string(double(clock() - t) / CLOCKS_PER_SEC * 1000) << "; ";
	t = clock();
	size_t i = 0;
	while (result.size > 1 && result[i] == 0) {
		result.size--;
		i++;
	}
	std::cout << " 2: " << std::to_string(double(clock() - t) / CLOCKS_PER_SEC * 1000) << "; ";
	t = clock();
	if (i > 0) {
		std::cout << " ___ ";
		for (size_t j = 0; j < result.size - i + 1; j++)
			result[j] = result[j + i];
		result.buf = (CBigIntBig::AtomicT*)realloc(result.buf, result.size);
	}
	result.sign = num1.sign == num2.sign ? true : false;
	std::cout << " 3: " << std::to_string(double(clock() - t) / CLOCKS_PER_SEC * 1000) << "; ";
	return result;
}

CBigIntBig operator*(const CBigIntBig & a, const int & c) {
	if (c == 0)
		return CBigIntBig("0");
	CBigIntBig result;
	int b = c;
	result.sign = (a.sign && b > 0) || (!a.sign && b < 0) ? true : false;
	if (b < 0) b *= (-1);
	CBigIntBig::AtomicT temp, carry = 0;
	CBigIntBig::AtomicT sizeB = 0;
	for (size_t i = b; i > 0; i = i / 10, sizeB++);
	result.size = a.size + sizeB;
	result.buf = (CBigIntBig::AtomicT*)malloc(result.size);
	for (size_t i = 0; i < result.size; i++)
		result[i] = 0;
	size_t i;
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


CBigIntBig operator/(const CBigIntBig & a, const CBigIntBig & b)
{
	clock_t t = clock();
	if (a.size < b.size)
		return CBigIntBig("0");
	if (a.size == b.size) {
		for (size_t i = 0; i < a.size; i++) {
			if (a[i] == b[i])
				continue;
			else
				if (a[i] < b[i])
					return CBigIntBig("0");
				else break;
		}
	}
	std::cout << " 1: " << std::to_string(clock() - t) << "; ";
	t = clock();
	CBigIntBig q;
	q.sign = a.sign == b.sign ? true : false;
	CBigIntBig copyA;
	copyA.size = a.size + 1;
	copyA.buf = (CBigIntBig::AtomicT*) malloc(copyA.size);
	copyA[0] = 0;
	for (size_t i = 1; i < copyA.size; i++)
		copyA[i] = a[i - 1];
	size_t shift = 0;
	CBigIntBig copyB;
	copyB = b;

	CBigIntBig::AtomicT scale;
	CBigIntBig::AtomicT qGuess, r;
	CBigIntBig::AtomicT borrow, carry;
	size_t n = b.size, m = a.size - b.size;
	scale = 10 / (b[0] + 1);
	std::cout << " 2: " << std::to_string(clock() - t) << "; ";
	t = clock();
	size_t i = 0, j = 0;
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
	std::cout << " 3: " << std::to_string(clock() - t) << "; ";
	CBigIntBig::AtomicT *aShift = copyA.buf + i;
	CBigIntBig::AtomicT *bShift = copyB.buf + j;
	q.size = m + 1;
	q.buf = (CBigIntBig::AtomicT*)malloc(q.size);
	size_t q_ind, a_ind;
	t = clock();
	for (q_ind = 0, a_ind = 0; q_ind <= m; q_ind++, a_ind++) {
		CBigIntBig::AtomicT temp = (aShift[a_ind] * 10 + aShift[a_ind + 1]);
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

		CBigIntBig::AtomicT temp1, temp2;
		for (size_t i = n - 1; i >= 0; i--) {
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
			for (size_t i = n - 1; i >= 0; i--) {
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
	std::cout << " 4: " << std::to_string(clock() - t) << "; ";
	t = clock();
	copyA.deleteNumber();
	copyB.deleteNumber();
	std::cout << " 5: " << std::to_string(clock() - t) << "; ";
	t = clock();
	m -= shift;
	q.size = m + 1;
	if (shift != 0)
		q.buf = (CBigIntBig::AtomicT*)realloc(q.buf, q.size);
	std::cout << " 6: " << std::to_string(clock() - t) << "; ";
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
	for (size_t i = 0; i < num.size; i++) {
		stream << num[i];
	}
	return stream;
}
