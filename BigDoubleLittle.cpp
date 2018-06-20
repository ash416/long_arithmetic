#include "BigDoubleLittle.h"
#include <ctime>

CBigDoubleLittle::CBigDoubleLittle()
{
}

CBigDoubleLittle::CBigDoubleLittle(std::string str)
{
	size_t leng = str.length();
	size_t is_negative = 0;
	if (str[0] == '-') {
		this->sign = false;
		is_negative = 1;
	}
	else {
		this->sign = true;
	}
	this->size = leng - 1 - is_negative;
	this->buf = (AtomicT*)malloc(size);
	int j = 0;
	this->point = -1;
	for (int i = (int)leng - 1; i >= 0 + (int)is_negative; i--) {
		if (str[i] == '.') {
			this->point = j - 1;
			continue;
		}
		buf[j++] = str[i] - '0';
	}
	if (this->point == -1) this->size++;
}

/*CBigDoubleLittle::AtomicT & CBigDoubleLittle::operator[](const size_t i)
{
	return buf[i];
}

CBigDoubleLittle::AtomicT CBigDoubleLittle::operator[](const size_t i) const
{
	return buf[i];
}*/

bool operator==(const CBigDoubleLittle & a, const CBigDoubleLittle & b)
{
	if (a.size != b.size || a.sign != b.sign || a.point != b.point)
		return false;
	else {
		for (int i = 0; i < (int)a.size; i++)
			if (a.buf[i] != b.buf[i])
				return false;
		return true;
	}
}

bool operator<(const CBigDoubleLittle & a, const CBigDoubleLittle &b) {
	if (a.sign == b.sign) {
		if (a.size - a.point != b.size - b.point)
			return a.size - a.point > b.size - b.point? !a.sign : a.sign;
		else { 
			for (int i = (int)a.size - 1, j = (int)b.size - 1; i >= 0 && j>= 0; i--, j--) {
				if (a.buf[i] == b.buf[j])
					continue;
				else
					return a.sign ? (a.buf[i] < b.buf[j]) : !(a.buf[i] < b.buf[j]);
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
			for (int i = (int)a.size - 1, j = (int)b.size - 1; i >= 0 && j >= 0; i--, j--) {
				if (a.buf[i] == b.buf[j])
					continue;
				else
					return !a.sign ? (a.buf[i] < b.buf[j]) : !(a.buf[i] < b.buf[j]);
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
	this->buf = (AtomicT*)malloc(num.size);
	for (size_t i = 0; i < num.size; i++)
		this->buf[i] = num.buf[i];
	this->point = num.point;
	this->sign = num.sign;
	this->size = num.size;
	return *this;
}


void CBigDoubleLittle::differences(CBigDoubleLittle & sum, const CBigDoubleLittle & big, const CBigDoubleLittle & small)
{
	//clock_t t = std::clock();
	CBigDoubleLittle::AtomicT carry = 0;
	size_t fract_size = big.point > small.point ? big.point + 1 : small.point + 1;
	sum.point = fract_size - 1;
	sum.size = fract_size + (small.size - small.point > big.size - big.point ? small.size - small.point : big.size - big.point) - 1;
	sum.buf = (CBigDoubleLittle::AtomicT*)malloc(sum.size);
	int sum_index = 0;
	//std::cout << "1: " << std::clock() - t << " ";
	//t = std::clock();
	for (int i = (int)small.point - (int)fract_size + 1, j = (int)big.point - (int)fract_size + 1; i <= (int)small.point && j <= (int)big.point; i++, j++) {
		if (i < 0) {
			sum.buf[sum_index++] = big.buf[j];
		}
		else 
			if (j < 0) {
				sum.buf[sum_index++] = (-small.buf[i] + 10 - carry) % 10;
				carry = -small.buf[i] - carry >= 0 ? 0 : 1;
			}
			else {
				CBigDoubleLittle::AtomicT temp = big.buf[j] - small.buf[i] - carry + 10;
				sum.buf[sum_index++] = temp % 10;
				carry = big.buf[j] - carry >= small.buf[i] ? 0 : 1;
			}
	}
	//std::cout << "2: " << std::clock() - t << " ";
	//t = std::clock();
	for (int i = (int)small.point + 1, j = (int)big.point + 1; sum_index < (int)sum.size; i++, j++, sum_index++) {
		if (i >= (int)small.size) {
			sum.buf[sum_index] = (big.buf[j] - carry + 10) % 10;
			carry = (big.buf[j] - carry) >= 0 ? 0 : 1;
		}
		else {
			sum.buf[sum_index] = (big.buf[j] - carry - small.buf[i] + 10) % 10;
			carry = (big.buf[j] - carry >= small.buf[i] ? 0 : 1);
		}
	}
	//std::cout << "3: " << std::clock() - t << " ";
}

CBigDoubleLittle operator+(const CBigDoubleLittle & num1, const CBigDoubleLittle & num2)
{
	CBigDoubleLittle sum;
	CBigDoubleLittle::AtomicT carry = 0;
	if (num1.sign == num2.sign) {
		size_t fract_size = num1.point > num2.point ? num1.point + 1 : num2.point + 1;
		sum.point = fract_size - 1;
		sum.size = fract_size + (num1.size - num1.point > num2.size - num2.point ? num1.size - num1.point : num2.size - num2.point) - 1;
		sum.buf = (CBigDoubleLittle::AtomicT*)malloc(sum.size);
		size_t sum_index = 0;
		for (int i = (int)num1.point - (int)fract_size + 1, j = (int)num2.point - (int)fract_size + 1; i <= (int)num1.point && j <= (int)num2.point; i++, j++) {
			if (i < 0) 	sum.buf[sum_index++] = num2.buf[j];
			else if (j < 0) sum.buf[sum_index++] = num1.buf[i];
			else {
				CBigDoubleLittle::AtomicT temp = num1.buf[i] + num2.buf[j] + carry;
				sum.buf[sum_index++] = temp % 10;
				carry = temp / 10;
			}
		}
		for (size_t i = num1.point + 1, j = num2.point + 1; sum_index < sum.size; i++, j++, sum_index++) {
			CBigDoubleLittle::AtomicT temp;
			if (i >= num1.size)
				temp = num2.buf[j] + carry;
			else
				if (j >= num2.size)
					temp = num1.buf[i] + carry;
				else
					temp = num1.buf[i] + num2.buf[j] + carry;
			sum.buf[sum_index] = temp % 10;
			carry = temp / 10;
		}
		if (carry > 0) {
			sum.size++;
			sum.buf = (CBigDoubleLittle::AtomicT*)realloc(sum.buf, sum.size);
			sum.buf[sum.size - 1] = carry;
		}
		sum.sign = num1.sign;
	}
	else {
		//clock_t t = std::clock();
		bool num1_is_less;
		if (num1.size - num1.point != num2.size - num2.point)
			num1_is_less = num1.size - num1.point >= num2.size - num2.point ? false : true;
		else {
			int i, j;
			for (i = (int)num1.size - 1, j = (int)num2.size - 1; i >= 0 && j >= 0; i--, j--) {
				if (num1.buf[i] == num2.buf[j])
					continue;
				else {
					num1_is_less = num1.buf[i] < num2.buf[j];
					break;
				}
			}
			if (i < 0 || j < 0)
				num1_is_less = num1.size < num2.size;
		}
		//std::cout << "1: " << std::clock() - t << " ";
		//t = std::clock();
		if (num1_is_less) sum.differences(sum, num2, num1);
		else sum.differences(sum, num1, num2);
		//std::cout << "2: " << std::clock() - t << " ";
		//t = std::clock();
		sum.sign = num1_is_less ? num2.sign : num1.sign;
		bool flag = false;
		int i = (int)sum.size - 1;
		while (i > (int)sum.point + 1 && sum.buf[i--] == 0) {
			sum.size--;
			flag = true;
		}
		if (flag)
			sum.buf = (CBigDoubleLittle::AtomicT*)realloc(sum.buf, sum.size);
		//std::cout << "3: " << std::clock() - t << " ";
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
	result.buf = (CBigDoubleLittle::AtomicT*)malloc(result.size);
	for (size_t i = 0; i < result.size; i++)
		result.buf[i] = 0;
	for (size_t i = 0; i < num2.size; ++i) {
		if (num2.buf[i] != 0) {
			CBigDoubleLittle::AtomicT carry = 0;
			for (size_t j = 0; j < num1.size; ++j) {
				CBigDoubleLittle::AtomicT temp = num1.buf[j] * num2.buf[i] + result.buf[i + j] + carry;
				result.buf[i + j] = temp % 10;
				carry = temp / 10;
			}
			result.buf[i + num1.size] = carry;
		}
	}
	result.point = num1.point + num2.point + 1;
	bool flag = false;
	int i = (int)result.size - 1;
	while (i > (int)result.point + 1 && result.buf[i] == 0) {
		result.size--; flag = true;
		i--;
	}
	if (flag) {
		result.buf = (CBigDoubleLittle::AtomicT*)realloc(result.buf, result.size);
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
	CBigDoubleLittle::AtomicT temp, carry = 0;
	size_t sizeB = 0;
	for (int i = b; i > 0; i = i / 10, sizeB++);
	result.size = a.size + sizeB;
	result.buf = (CBigDoubleLittle::AtomicT*)malloc(result.size);
	for (size_t i = 0; i < result.size; i++)
		result.buf[i] = 0;
	size_t i;
	for (i = 0; i < a.size; i++) {
		temp = a.buf[i] * b + carry;
		carry = temp / 10;
		result.buf[i] = temp - carry * 10;
	}
	while (carry > 0) {
		result.buf[i++] = carry % 10;
		carry /= 10;
	}
	return result;
}

CBigDoubleLittle operator/(const CBigDoubleLittle & a, const CBigDoubleLittle & b)
{
	CBigDoubleLittle q;
	q.sign = a.sign == b.sign ? true : false;
	size_t a_z_num = 0, b_z_num = 0;
	for (int i = (int)a.size - 1; i >= 0; i--)
		if (a.buf[i] == 0)
			a_z_num++;
		else
			break;
	for (int i = b.size - 1; i >= 0; i--)
		if (b.buf[i] == 0)
			b_z_num++;
		else
			break;
	CBigDoubleLittle copyA;
	size_t accuracy = 10;
	copyA.size = a.size + 1 + (b.point - a.point > 0 ? b.point - a.point : 0) + accuracy;
	copyA.buf = (CBigDoubleLittle::AtomicT*)malloc(copyA.size);
	size_t j; 
	for (j = 0; j < ((int)b.point - (int)a.point > 0 ? (int)b.point - (int)a.point : 0) + accuracy; j++)
		copyA.buf[j] = 0;
	for (size_t i = 0; i < a.size; i++)
		copyA.buf[j++] = a.buf[i];
	size_t a_size = a.size + ((int)b.point - (int)a.point > 0 ? (int)b.point - (int)a.point : 0);
	if (copyA.buf[j - 1] == 0) {
		a_size--;
		j--;
		copyA.size--;
	}
	copyA.buf[j] = 0;

	size_t shift = 0;
	CBigDoubleLittle copyB;
	copyB = b;

	CBigDoubleLittle::AtomicT scale;
	CBigDoubleLittle::AtomicT qGuess, r;
	CBigDoubleLittle::AtomicT borrow, carry;
	int diff = (int)a.size - (int)b.size + (int)b.point - (int)a.point;
	size_t n = b.size, m = a_size - b.size + accuracy;
	scale = 10 / (b.buf[n - 1] + 1);
	if (scale > 1) {
		copyA = copyA * scale;
		if (copyA.buf[copyA.size - 1] == 0) {
			copyA.size--;
		}
		copyB = copyB * scale;
		if (copyB.buf[copyB.size - 1] == 0) {
			copyB.size--;
		}
	}

	bool a_is_less_b;
	for (int i = (int)copyA.size - 1 - (int)(copyA.buf[copyA.size - 1] == 0 ? 1 : 0) - (int)a_z_num, j = (int)copyB.size - (int)b_z_num - 1; i >= 0 || j >= 0; i--, j--) {
		if (i < 0) {
			a_is_less_b = true;
			break;
		}
		else if (j < 0) {
			a_is_less_b = false;
			break;
		}
		else if (copyA.buf[i] == copyB.buf[j])
			continue;
		else {
			a_is_less_b = (copyA.buf[i] < copyB.buf[j]);
 			break;
		}
	}
	if (a_z_num == 0 && b_z_num == 0) {
		if (diff < 0) {
			q.point = 1;
			size_t q_zero = copyB.size - b.point - copyA.size + a.point + (copyA.buf[copyA.size - 1] == 0 ? 1 : 0) + (a_is_less_b ? 0 : 1);
			q.size = m + q_zero + 1;
			q.buf = (CBigDoubleLittle::AtomicT*)malloc(q.size);
			for (size_t i = q.size - 1; i >= q.size - 1 - q_zero; i--)
				q.buf[i] = 0;
		}
		else {
			q.size = m + 1;
			q.point = q.size - (diff + (copyA.buf[copyA.size - 1] != 0 ? 1 : (a_is_less_b ? 0 : 1))) - 1;
			q.buf = (CBigDoubleLittle::AtomicT*)malloc(q.size);
		}
	}
	else {
		if (a_z_num > b_z_num) {
			q.point = 1;
			size_t q_zero = copyB.size - b.point - copyA.size + a.point + (copyA.buf[copyA.size - 1] == 0 ? 1 : 0) + (a_is_less_b ? 0 : 1) + a_z_num - b_z_num;
			q.size = m + q_zero + 1;
			q.buf = (CBigDoubleLittle::AtomicT*)malloc(q.size);
			for (size_t i = q.size - 1; i > m; i--)
				q.buf[i] = 0;
		}
		else {
			q.size = m + 1;
			q.point = q.size - 1 - (b_z_num - a_z_num + (a_is_less_b ? 0 : 1));
			q.buf = (CBigDoubleLittle::AtomicT*)malloc(q.size);
		}
	}

	int q_ind, a_ind;
	for (q_ind = (int)m, a_ind = (int)a_size + (int)accuracy; q_ind >= 0; q_ind--, a_ind--) {
		if (q_ind + shift == q.point - accuracy) {
			shift++;
			break;
		}
		CBigDoubleLittle::AtomicT temp = (copyA.buf[a_ind] * 10 + copyA.buf[a_ind - 1]);
		qGuess = temp / copyB.buf[n - 1];
		r = temp % copyB.buf[n - 1];
		while (r < 10) {
			if (copyB.buf[n - 2] * qGuess > r * 10 + copyA.buf[a_ind - 2] || qGuess == 10) {
				qGuess--;
				r += copyB.buf[n - 1];
			}
			else break;
		}
		carry = 0;
		borrow = 0;
		CBigDoubleLittle::AtomicT *aShift = copyA.buf + q_ind;

		CBigDoubleLittle::AtomicT temp1, temp2;
		for (size_t i = 0; i < n; i++) {
			temp1 = copyB.buf[i] * qGuess + carry;
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
			if (q_ind + shift == m && qGuess == 0 ) {
				shift++;
				continue;
			}
			q.buf[q_ind + shift] = qGuess;
		}
		else {
			if (q_ind + shift == m && qGuess - 1 == 0 ) {
				shift++;
			}
			else {
				q.buf[q_ind + shift] = qGuess - 1;
			}
			carry = 0;
			for (size_t i = 0; i < n; i++) {
				temp = aShift[i] + copyB.buf[i] + carry;
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
		size_t sh = q.point - accuracy + 1;
		for (size_t i = 0; i <= m - sh; i++)
			q.buf[i] = q.buf[i + sh];
		m -= sh;
		q.point -= sh;
		q.size = m + 1;
		q.buf = (CBigDoubleLittle::AtomicT*)realloc(q.buf, q.size);
	}
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
		stream << (int)num.buf[i];
	}
	return stream;
}
