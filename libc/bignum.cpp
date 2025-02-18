#include "bignum.h"

#ifdef _WIN32
    #define EXPORT __declspec(dllexport)
#else
    #define EXPORT
#endif

BigNum::BigNum(){// 构造函数
    len = 1;
    memset(a, 0, sizeof(a));
}

BigNum::BigNum(const int b){// 将一个int类型的变量转化为大数
    int c, d = b;
    len = 0;
    memset(a, 0, sizeof(a));
    while(d > MAXN){
        c = d - (d / (MAXN + 1)) * (MAXN + 1);
        d = d / (MAXN + 1);
        a[len++] = c;
    }
    a[len++] = d;
}

BigNum::BigNum(const char* s, int base){// 将一个字符串类型的变量转化为大数
    int t, k, index, l, i;
    memset(a, 0, sizeof(a));
    l = strlen(s);
    len = l / DLEN;
    if(l % DLEN) len++;
    index = 0;
    for(i = l - 1; i >= 0; i -= DLEN){
        t = 0;
        k = i - DLEN + 1;
        if(k < 0) k = 0;
        for(int j = k; j <= i; j++){
            if(base <= 10){
                t = t * base + s[j] - '0';
            }
            if(base == 16){// 十六进制处理
                t = t * 16 + ((s[j] >= 'a') ? (s[j] - 'a') + 10 : (s[j] >= 'A') ? (s[j] - 'A') + 10 : (s[j] - '0'));
            }
        }
        a[index++] = t;
    }
}

BigNum::BigNum(const BigNum& T) : len(T.len){// 拷贝构造函数
    int i;
    memset(a, 0, sizeof(a));
    for(i = 0; i < len; i++) a[i] = T.a[i];
}

BigNum& BigNum::operator=(const BigNum& n){// 重载赋值运算符，大数之间进行赋值运算
    int i;
    len = n.len;
    memset(a, 0, sizeof(a));
    for(i = 0; i < len; i++) a[i] = n.a[i];
    return *this;
}

istream& operator>>(istream& in, BigNum& b){// 重载输入运算符
    char ch[MAXSIZE * 4];
    int i = -1;
    in >> ch;
    int l = strlen(ch);
    int count = 0, sum = 0;
    for(i = l - 1; i >= 0;){
        sum = 0;
        int t = 1;
        for(int j = 0; j < 4 && i >= 0; j++, i--, t *= 10){
            sum += (ch[i] - '0') * t;
        }
        b.a[count] = sum;
        count++;
    }
    b.len = count++;
    return in;
}

ostream& operator<<(ostream& out, BigNum& b){// 重载输出运算符
    int i;
    out << b.a[b.len - 1];
    for(i = b.len - 2; i >= 0; i--){
        out.width(DLEN);
        out.fill('0');
        out << b.a[i];
    }
    return out;
}

BigNum BigNum::operator+(const BigNum& T) const{// 两个大数之间的相加运算
    BigNum t(*this);
    int i, big;//位数
    big = T.len > len ? T.len : len;
    for(i = 0; i < big; i++){
        t.a[i] += T.a[i];
        if(t.a[i] > MAXN){
            t.a[i + 1]++;
            t.a[i] -= MAXN + 1;
        }
    }
    if(t.a[big] != 0) t.len = big + 1;
    else t.len = big;
    return t;
}

BigNum BigNum::operator-(const BigNum& T) const{// 两个大数之间的相减运算
    int i, j, big;
    bool flag;
    BigNum t1, t2;
    if(*this > T){
        t1 = *this;
        t2 = T;
        flag = 0;
    }else{
        t1 = T;
        t2 = *this;
        flag = 1;
    }
    big = t1.len;
    for(i = 0; i < big; i++){
        if(t1.a[i] < t2.a[i]){
            j = i + 1;
            while(t1.a[j] == 0) j++;
            t1.a[j--]--;
            while(j > i){
                t1.a[j--] += MAXN;
            }
            t1.a[i] += MAXN + 1 - t2.a[i];
        }else{
            t1.a[i] -= t2.a[i];
        }
    }
    t1.len = big;
    while(t1.a[len - 1] == 0 && t1.len > 1){
        t1.len--;
        big--;
    }
    if(flag){
        t1.a[big - 1] = 0 - t1.a[big - 1];
    }
    return t1;
}

BigNum BigNum::operator*(const BigNum& T) const{// 两个大数之间的相乘运算
    BigNum ret;
    int i, j, up;
    int temp, temp1;
    for(i = 0; i < len; i++){
        up = 0;
        for(j = 0; j < T.len; j++){
            temp = a[i] * T.a[j] + ret.a[i + j] + up;
            if (temp > MAXN){
                temp1 = temp - temp / (MAXN + 1) * (MAXN + 1);
                up = temp / (MAXN + 1);
                ret.a[i + j] = temp1;
            }else{
                up = 0;
                ret.a[i + j] = temp;
            }
        }
        if(up != 0){
            ret.a[i + j] = up;
        }
    }
    ret.len = i + j;
    while(ret.a[ret.len - 1] == 0 && ret.len > 1) ret.len--;
    return ret;
}

BigNum BigNum::operator/(const BigNum& T) const{// 重载除法运算符，两个大数之间的相除运算
    BigNum ret(0);
    BigNum t1(*this);
    BigNum t2(T);
    int times = t1.len - t2.len;
    if(times < 0) return ret;
    BigNum base = 10;
    t2 = T * (base ^ (times * DLEN));
    if(t1 < t2){
        if(times == 0) return ret;
        else t2 = T * (base ^ (--times * DLEN));
    }
    while(!(T > t1)){// t1 >= T
        while(t1.a[t1.len - 1] > t2.a[t1.len - 1] + 1){
            int divided = t1.a[t1.len - 1];
            int divisor = t2.a[t1.len - 1] + 1;
            int tempans = divided / divisor;
            t1 = t1 - t2 * tempans;
            ret = ret + BigNum(tempans) * (base ^ (times * DLEN));
        }
        if(t1.a[t1.len - 1] == 1){
            t1.a[t1.len - 1] = 0;
            t1.a[t1.len - 2] += MAXN + 1;
            t1.len--;
        }
        if(times == 0){
            while(!(t2 > t1)){
                t1 = t1 - t2;
                ret = ret + (base ^ (times * DLEN));
            }
            break;
        }
        t2 = T * (base ^ (--times * DLEN));
    }
    return ret;
}

BigNum BigNum::operator%(const BigNum& T) const{// 重载取模运算符，两个大数之间的取模运算
    BigNum ret(*this);
    BigNum temp = (ret / T) * T;
    ret = ret - temp;
    return ret;
}

BigNum BigNum::operator/(const int& b) const{// 大数对一个整数进行相除运算
    BigNum ret;
    int i, down = 0;
    for(i = len - 1; i >= 0; i--){
        ret.a[i] = (a[i] + down * (MAXN + 1)) / b;
        down = a[i] + down * (MAXN + 1) - ret.a[i] * b;
    }
    ret.len = len;
    while(ret.a[ret.len - 1] == 0 && ret.len > 1) ret.len--;
    return ret;
}

BigNum BigNum::operator^(const int& n) const{// 大数的n次方运算
    BigNum t, ret(1);
    int i;
    if(n < 0) exit(-1);
    if(n == 0) return 1;
    if(n == 1) return *this;
    int m = n;
    while(m > 1) {
        t = *this;
        for(i = 1; i << 1 <= m; i <<= 1) {
            t = t * t;
        }
        m -= i;
        ret = ret * t;
        if(m == 1) ret = ret * (*this);
    }
    return ret;
}

int BigNum::operator%(const int& b) const{// 大数对一个int类型的变量进行取模运算
    int i, d = 0;
    for(i = len - 1; i >= 0; i--){
        d = ((d * (MAXN + 1)) % b + a[i]) % b;
    }
    return d;
}

bool BigNum::operator>(const BigNum& T) const{// 大数和另一个大数的大小比较
    int ln;
    if (len > T.len)
        return true;
    else if (len == T.len)
    {
        ln = len - 1;
        while (a[ln] == T.a[ln] && ln >= 0)
            ln--;
        if (ln >= 0 && a[ln] > T.a[ln])
            return true;
        else
            return false;
    }
    else
        return false;
}

bool BigNum::operator<(const BigNum& T) const{
    return T > *this;
}

bool BigNum::operator >(const int& t) const{// 大数和一个int类型的变量的大小比较
    BigNum b(t);
    return *this > b;
}

bool BigNum::operator<(const int& t) const{
    return BigNum(t) > *this;
}

bool BigNum::operator==(const BigNum& T) const{// 两个大数之间的相等比较
    return !(*this > T) && !(T > *this);
}

bool BigNum::operator==(const int& t) const{// 大数和一个int类型的变量的相等比较
    return *this == BigNum(t);
}

BigNum BigNum::random(int bytes){// 生成BYTES位的大随机数
    srand(time(0));
    BigNum base(1);
    BigNum ret(0);
    for(int i = 0; i < bytes; i++){
        if(rand() % 2) ret = ret + base;
        base = base + base;
    }
    return ret;
}

void BigNum::print(){// 输出大数
    int i;
    cout << a[len - 1];
    for(i = len - 2; i >= 0; i--){
        cout.width(DLEN);
        cout.fill('0');
        cout << a[i];
    }
    cout << endl;
}

string BigNum::toString(){// 大数转换字符串
    string ret;
    stringstream ss;
    ss << (*this);
    ss >> ret;
    return ret;
}
