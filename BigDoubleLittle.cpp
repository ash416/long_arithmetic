#include "BigDoubleLittle.h"

CBigDoubleLittle::CBigDoubleLittle()
{
}

CBigDoubleLittle::CBigDoubleLittle(std::string str)
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
	this->buf = (int*)malloc(size * sizeof(int));
	int j = 0;
	this->point = -1;
	for (int i = leng - 1; i >= 0 + is_negative; i--) {
		if (str[i] == '.') {
			this->point = j - 1;
			continue;
		}
		*(buf + j++) = int(str[i] - '0');
	}
	if (this->point == -1) this->size++;
}

int & CBigDoubleLittle::operator[](const int i)
{
	return buf[i];
}

int CBigDoubleLittle::operator[](const int i) const
{
	return buf[i];
}

bool operator==(const CBigDoubleLittle & a, const CBigDoubleLittle & b)
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

bool operator<(const CBigDoubleLittle & a, const CBigDoubleLittle &b) {
	if (a.sign == b.sign) {
		if (a.size - a.point != b.size - b.point)
			return a.size - a.point > b.size - b.point? !a.sign : a.sign;
		else { 
			for (int i = a.size - 1, j = b.size - 1; i >= 0 && j>= 0; i--, j--) {
				if (a[i] == b[j])
					continue;
				else
					return a.sign ? (a[i] < b[j]) : !(a[i] < b[j]);
			}
			return a.size < b.size ? a.sign : !a.sign;
		}
	}
	else {
		return !a.sign;
	}
}

bool operator>(const CBigDoubleLittle & a, const CBigDoubleLittle &b) {
	if (a.sign == b.sign) {
		if (a.size  - a.point != b.size - b.point)
			return a.size - a.point > b.size - b.point ? a.sign : !a.sign;
		else { 
			for (int i = a.size - 1, j = b.size - 1; i >= 0 && j >= 0; i--, j--) {
				if (a[i] == b[j])
					continue;
				else
					return !a.sign ? (a[i] < b[j]) : !(a[i] < b[j]);
			}
			return a.size > b.size ? a.sign : !a.sign;
		}
	}
	else {
		return a.sign;
	}
}

bool operator >=(const CBigDoubleLittle &a, const CBigDoubleLittle &b) {
	return(!(a < b));
}

bool operator <=(const CBigDoubleLittle &a, const CBigDoubleLittle &b) {
	return (!(a > b));
}

bool operator !=(const CBigDoubleLittle &a, const CBigDoubleLittle &b) {
	return (!(a == b));
}

CBigDoubleLittle &CBigDoubleLittle::operator =(const CBigDoubleLittle &num) {
	free(this->buf);
	this->buf = (int*)malloc(num.size * sizeof(int));
	for (int i = 0; i < num.size; i++)
		this->buf[i] = num[i];
	this->point = num.point;
	this->sign = num.sign;
	this->size = num.size;
	return *this;
}


void CBigDoubleLittle::differences(CBigDoubleLittle & sum, const CBigDoubleLittle & big, const CBigDoubleLittle & small)
{
	int carry = 0;
	int fract_size = big.point > small.point ? big.point + 1 : small.point + 1;
	sum.point = fract_size - 1;
	sum.size = fract_size + (small.size - small.point > big.size - big.point ? small.size - small.point : big.size - big.point) - 1;
	sum.buf = (int*)malloc(sum.size * sizeof(int));
	int sum_index = 0;
	for (int i = small.point - fract_size + 1, j = big.point - fract_size + 1; i <= small.point && j <= big.point; i++, j++) {
		if (i < 0) {
			sum[sum_index++] = big[j];
		}
		else 
			if (j < 0) {
				sum[sum_index++] = (-small[i] + 10 - carry) % 10;
				carry = -small[i] - carry >= 0 ? 0 : 1;
			}
			else {
				int temp = big[j] - small[i] - carry + 10;
				sum[sum_index++] = temp % 10;
				carry = big[j] - carry >= small[i] ? 0 : 1;
			}
	}
	for (int i = small.point + 1, j = big.point + 1; sum_index < sum.size; i++, j++, sum_index++) {
		if (i >= small.size) {
			sum[sum_index] = (big[j] - carry + 10) % 10;
			carry = (big[j] - carry) >= 0 ? 0 : 1;
		}
		else {
			sum[sum_index] = (big[j] - carry - small[i] + 10) % 10;
			carry = (big[j] - carry >= small[i] ? 0 : 1);
		}
	}
}

CBigDoubleLittle operator+(const CBigDoubleLittle & num1, const CBigDoubleLittle & num2)
{
	CBigDoubleLittle sum;
	int carry = 0;
	if (num1.sign == num2.sign) {
		int fract_size = num1.point > num2.point ? num1.point + 1 : num2.point + 1;
		sum.point = fract_size - 1;
		sum.size = fract_size + (num1.size - num1.point > num2.size - num2.point ? num1.size - num1.point : num2.size - num2.point) - 1;
		sum.buf = (int*)malloc(sum.size * sizeof(int));
		int sum_index = 0;
		for (int i = num1.point - fract_size + 1, j = num2.point - fract_size + 1; i <= num1.point && j <= num2.point; i++, j++) {
			if (i < 0) 	sum[sum_index++] = num2[j];
			else if (j < 0) sum[sum_index++] = num1[i];
			else {
				int temp = num1[i] + num2[j] + carry;
				sum[sum_index++] = temp % 10;
				carry = temp / 10;
			}
		}
		for (int i = num1.point + 1, j = num2.point + 1; sum_index < sum.size; i++, j++, sum_index++) {
			int temp;
			if (i >= num1.size)
				temp = num2[j] + carry;
			else
				if (j >= num2.size)
					temp = num1[i] + carry;
				else
					temp = num1[i] + num2[j] + carry;
			sum[sum_index] = temp % 10;
			carry = temp / 10;
		}
		if (carry > 0) {
			sum.size++;
			sum.buf = (int*)realloc(sum.buf, sum.size * sizeof(int));
			sum[sum.size - 1] = carry;
		}
		sum.sign = num1.sign;
	}
	else {
		bool num1_is_less;
		if (num1.size - num1.point != num2.size - num2.point)
			num1_is_less = num1.size - num1.point >= num2.size - num2.point ? false : true;
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
		while (i > sum.point + 1 && sum.buf[i--] == 0) {
			sum.size--;
			flag = true;
		}
		if (flag)
			sum.buf = (int*)realloc(sum.buf, sum.size * sizeof(int));
	}
	return sum;
}


CBigDoubleLittle operator-(const CBigDoubleLittle & num1, CBigDoubleLittle & num2)
{
	num2.sign = false;
	CBigDoubleLittle res;
	res = num1 + num2;
	return res;
}

CBigDoubleLittle operator*(const CBigDoubleLittle &num1, const CBigDoubleLittle &num2) {
	CBigDoubleLittle result;
	result.size = num1.size + num2.size;
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
	result.point = num1.point + num2.point + 1;
	bool flag = false;
	int i = result.size - 1;
	while (i > result.point + 1 && result[i] == 0) {
		result.size--; flag = true;
		i--;
	}
	if (flag) {
		result.buf = (int*)realloc(result.buf, result.size * sizeof(int));
	}
	
	result.sign = num1.sign == num2.sign ? true : false;
	return result;
}

CBigDoubleLittle operator*(const CBigDoubleLittle & a, const int & c) {
	if (c == 0)
		return CBigDoubleLittle("0");
	CBigDoubleLittle result;
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

CBigDoubleLittle operator/(const CBigDoubleLittle & a, const CBigDoubleLittle & b)
{
	/*if (a.size < b.size)
		return CBigDoubleLittle("0");
	if (a.size == b.size) {
		for (int i = a.size - 1; i >= 0; i--) {
			if (a[i] == b[i])
				continue;
			else
				if (a[i] < b[i])
					return CBigDoubleLittle("0");
				else break;
		}
	}*/

	CBigDoubleLittle q;
	q.sign = a.sign == b.sign ? true : false;
	CBigDoubleLittle copyA;
	int accuracy = 2;
	copyA.size = a.size + 1 + (b.point - a.point > 0 ? b.point - a.point : 0) + accuracy;
	copyA.buf = (int*)malloc(copyA.size * sizeof(int));
	int j; 
	for (j = 0; j < (b.point - a.point > 0 ? b.point - a.point : 0) + accuracy; j++)
		copyA[j] = 0;
	for (int i = 0; i < a.size; i++)
		copyA[j++] = a[i];
	int a_size = a.size;
	if (copyA[j - 1] == 0) {
		a_size--;
		j--;
		copyA.size--;
	}
	copyA[j] = 0;
	std::cout << copyA << std::endl;
	int shift = 0;
	CBigDoubleLittle copyB;
	copyB = b;

	short scale;
	short qGuess, r;
	short borrow, carry;
	int diff = a.size - b.size + b.point - a.point;
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
	
	diff += (a[a.size - 1] >= b[b.size - 1] ? 1 : 0);
	q.size = m + 1;
	q.buf = (int*)malloc(q.size * sizeof(int));
	int new_m = m;
	if (diff > 0)
		q.point = q.size - diff - 1;
	else {
		while (diff <= 0) {
			diff++;
			q[new_m] = 0;
			new_m--;
		}
		q.point = q.size - 2;
	}
	
	int q_ind, a_ind;
	for (q_ind = new_m, a_ind = a_size + accuracy; q_ind >= 0; q_ind--, a_ind--) {
		if (q_ind + shift == q.point - accuracy) {
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
		int *aShift = copyA.buf + q_ind + (m - new_m);

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
			if (q_ind + shift == new_m && qGuess == 0 ) {
				q.point++;
				shift++;
				continue;
			}
			q[q_ind + shift] = qGuess;
		}
		else {
			if (q_ind + shift == new_m && qGuess - 1 == 0 ) {
				q.point++;
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
	if (shift != 0) {
		for (int i = 0; i <= m - shift; i++)
			q[i] = q[i + shift];
		m -= shift;
		q.point -= shift;
		q.size = m + 1;
		q.buf = (int*)realloc(q.buf, q.size * sizeof(int));
	}
	//else
		//q.size = m + 1;
	return q;
}

std::istream &operator >> (std::istream &stream, CBigDoubleLittle &num) {
	std::string str;
	stream >> str;
	CBigDoubleLittle _int(str);
	num = _int;
	return stream;
}
std::ostream &operator<<(std::ostream &stream, const CBigDoubleLittle &num) {
	if (!num.sign)
		stream << '-';
	for (int i = num.size - 1; i >= 0; i--) {
		if (i == num.point)
			stream << '.';
		stream << num[i];
	}
	return stream;
}
