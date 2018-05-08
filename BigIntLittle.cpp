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
		bool num1_is_less;
		if (num1.size != num2.size)
			num1_is_less = num1.size > num2.size ? false : true;
		else { 
			int i;
			for (i = num1.size - 1; i >= 0; i--) {
				if (num1[i] == num2[i])
					continue;
				else {
					num1_is_less = (num1[i] < num2[i]);
					break;
				}
			}
			if (i < 0)
				num1_is_less = false;
		}
		if (num1_is_less) {
			sum.size = num2.size;
			sum.buf = (int*)malloc(sum.size * sizeof(int));
			for (int i = 0; i < num2.size; i++) {
				if (i >= num1.size) {
					sum.buf[i] = (num2[i] - carry + 10) % 10;
					carry = (num2[i] - carry) >= 0 ? 0 : 1;
					continue;
				}
				sum.buf[i] = (num2[i] - carry - num1[i] + 10) % 10;
				carry = (num2[i] - carry >= num1[i] ? 0 : 1);
			}
		}
		else {
			sum.size = num1.size;
			sum.buf = (int*)malloc(sum.size * sizeof(int));
			for (int i = 0; i < num1.size; i++) {
				if (i >= num2.size) {
					sum.buf[i] = (num1[i] - carry + 10) % 10;
					carry = (num1[i] - carry) >= 0 ? 0 : 1;
					continue;
				}
				sum.buf[i] = (num1[i] - carry - num2[i] + 10) % 10;
				carry = (num1[i] - carry >= num2[i] ? 0 : 1);
			}
		}
		sum.sign = num1_is_less ? num2.sign : num1.sign;
		bool flag = false;
		while (sum.size > 1 && sum.buf[sum.size - 1] == 0) {
			sum.size--;
			flag = true;
		}
		if (flag)
			sum.buf = (int*)realloc(sum.buf, sum.size * sizeof(int));
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
	std::chrono::duration<double, std::milli> time_span;
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
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
	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
	time_span = t2 - t1;
	std::cout << " 1: " << std::to_string(time_span.count()) << "; ";
	t1 = std::chrono::high_resolution_clock::now();
	bool flag = false;
	while (result.size > 1 && result[result.size - 1] == 0) {
		result.size--;
		flag = true;
	}
	t2 = std::chrono::high_resolution_clock::now();
	time_span = t2 - t1;
	std::cout << " 2: " << std::to_string(time_span.count()) << "; ";
	t1 = std::chrono::high_resolution_clock::now();
	if (flag) {
		result.buf = (int*)realloc(result.buf, result.size * sizeof(int));
	}
	result.sign = num1.sign == num2.sign ? true : false;
	t2 = std::chrono::high_resolution_clock::now();
	time_span = t2 - t1;
	std::cout << " 3: " << std::to_string(time_span.count()) << "; ";
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
	std::chrono::duration<double, std::milli> time_span1, time_span2, time_span3;
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
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
	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
	//time_span = t2 - t1;
	//std::cout << " 1: " << std::to_string(time_span.count()) << "; ";
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
	int *aSh = copyA.buf;
	int *bSh = copyB.buf;
	q.size = m + 1;
	q.buf = (int*) malloc(q.size * sizeof(int));
	int q_ind, a_ind;
	for (q_ind = m, a_ind = n + q_ind; q_ind >= 0; q_ind--, a_ind--) {
		t1 = std::chrono::high_resolution_clock::now();
		int temp = (aSh[a_ind] * 10 + aSh[a_ind - 1]);
		qGuess = temp / bSh[n - 1];
		r = temp % bSh[n - 1];
		while (r < 10) {
			if (bSh[n - 2] * qGuess > r * 10 + aSh[a_ind - 2] || qGuess == 10) {
				qGuess--;
				r += bSh[n - 1];
			}
			else break;
		}
		t2 = std::chrono::high_resolution_clock::now();
		if (q_ind == m)
			time_span1 = t2 - t1;
		else
			time_span1 += t2 - t1;
		t1 = std::chrono::high_resolution_clock::now();
		carry = 0; 
		borrow = 0;
		int *aShift = &aSh[0] + q_ind;

		int temp1, temp2;
		for (int i = 0; i < n; i++) {
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
		t2 = std::chrono::high_resolution_clock::now();
		if (q_ind == m)
			time_span2 = t2 - t1;
		else
			time_span2 += t2 - t1;
		t1 = std::chrono::high_resolution_clock::now();
		if (borrow == 0) {
			if (q_ind + shift == m && qGuess == 0) {
				shift++;
				t2 = std::chrono::high_resolution_clock::now();
				if (q_ind == m)
					time_span3 = t2 - t1;
				else
					time_span3 += t2 - t1;
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
		t2 = std::chrono::high_resolution_clock::now();
		if (q_ind == m)
			time_span3 = t2 - t1;
		else
			time_span3 += t2 - t1;
	}
	std::cout << " 1: " << std::to_string(time_span1.count()) << "; ";
	std::cout << " 2: " << std::to_string(time_span2.count()) << "; ";
	std::cout << " 3: " << std::to_string(time_span3.count()) << "; ";
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
