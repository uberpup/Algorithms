#include <iostream>
#include "biginteger.h"

int main() {
    while (true) {
        //int aa = rand() % 217 - 10;
        //int bb = rand() % 217 - 10;
        int aa = 55;
        int bb = 121;
        BigInteger a(aa);
        BigInteger b(bb);
        std::cout << aa <<  " " << a << std::endl;
        std::cout << bb << " " << b << std::endl;
        //BigInteger c = a * b;
        //int cc = aa * bb;
        if (!(a * b == aa * bb)) {
            std::cout << "aa = " << a << " | bb = " << b << std::endl;
            std::cout << "a = " << a << " | b = " << b << std::endl;
            std::cout << "a * b = " << a * b << std::endl;
            std::cout << "aa * bb = " << aa * bb << std::endl << std::endl << std::endl;
        }
        if (!(a + b == aa + bb)) {
            std::cout << "aa = " << a << " | bb = " << b << std::endl;
            std::cout << "a = " << a << " | b = " << b << std::endl;
            std::cout << "a + b = " << a + b << std::endl;
            std::cout << "aa + bb = " << aa + bb << std::endl << std::endl << std::endl;
        }
        if (!(a - b == aa - bb)) {
            std::cout << "aa = " << a << " | bb = " << b << std::endl;
            std::cout << "a = " << a << " | b = " << b << std::endl;
            std::cout << "a - b = " << a - b << std::endl;
            std::cout << "aa - bb = " << aa - bb << std::endl << std::endl << std::endl;
        }
        if (bb != 0 && !(a / b == aa / bb)) {
            std::cout << "aa = " << a << " | bb = " << b << std::endl;
            std::cout << "a = " << a << " | b = " << b << std::endl;
            std::cout << "a / b = " << a / b << std::endl;
            std::cout << "aa / bb = " << aa / bb << std::endl << std::endl << std::endl;
        }
        if (bb != 0 && !(a % b == aa % bb)) {
            std::cout << "aa = " << a << " | bb = " << b << std::endl;
            std::cout << "a = " << a << " | b = " << b << std::endl;
            std::cout << "a % b = " << a % b << std::endl;
            std::cout << "aa % bb = " << aa % bb << std::endl << std::endl << std::endl;
        }
        if (a++ != aa++) {
            std::cout << "aa = " << a << " | bb = " << b << std::endl;
            std::cout << "a = " << a << " | b = " << b << std::endl;
            std::cout << "a++ = " << a++ << std::endl;
            std::cout << "aa++ = " << aa++ << std::endl << std::endl << std::endl;
        }
        if (a-- != aa--) {
            std::cout << "aa = " << a << " | bb = " << b << std::endl;
            std::cout << "a = " << a << " | b = " << b << std::endl;
            std::cout << "a-- = " << a-- << std::endl;
            std::cout << "aa-- = " << aa-- << std::endl << std::endl << std::endl;
        }
        if (++a != ++aa) {
            std::cout << "aa = " << a << " | bb = " << b << std::endl;
            std::cout << "a = " << a << " | b = " << b << std::endl;
            std::cout << "++a = " << ++a << std::endl;
            std::cout << "++aa = " << ++aa << std::endl << std::endl << std::endl;
        }
        if (--a != --aa) {
            std::cout << "aa = " << a << " | bb = " << b << std::endl;
            std::cout << "a = " << a << " | b = " << b << std::endl;
            std::cout << "--a = " << --a << std::endl;
            std::cout << "--aa = " << --aa << std::endl << std::endl << std::endl;
        }
        if (------a != ------aa) {
            std::cout << "aa = " << a << " | bb = " << b << std::endl;
            std::cout << "a = " << a << " | b = " << b << std::endl;
            std::cout << "------a = " << ------a << std::endl;
            std::cout << "------aa = " << ------aa << std::endl << std::endl << std::endl;
        }
        if (++++++a != ++++++aa) {
            std::cout << "aa = " << a << " | bb = " << b << std::endl;
            std::cout << "a = " << a << " | b = " << b << std::endl;
            std::cout << "++++++a = " << ++++++a << std::endl;
            std::cout << "++++++aa = " << ++++++aa << std::endl << std::endl << std::endl;
        }
        if (bool(a) != bool(aa)) {
            std::cout << "aa = " << a << " | bb = " << b << std::endl;
            std::cout << "a = " << a << " | b = " << b << std::endl;
            std::cout << "bool(a) = " << bool(a) << std::endl;
            std::cout << "bool(aa) = " << bool(aa) << std::endl << std::endl << std::endl;
        }
        if (bool(a < b) != bool(aa < bb)) {
            std::cout << "aa = " << a << " | bb = " << b << std::endl;
            std::cout << "a = " << a << " | b = " << b << std::endl;
            std::cout << "bool(a < b) = " << bool(a < b) << std::endl;
            std::cout << "bool(aa < bb) = " << bool(aa < bb) << std::endl << std::endl << std::endl;
        }
        if (bool(a <= b) != bool(aa <= bb)) {
            std::cout << "aa = " << a << " | bb = " << b << std::endl;
            std::cout << "a = " << a << " | b = " << b << std::endl;
            std::cout << "bool(a <= b) = " << bool(a <= b) << std::endl;
            std::cout << "bool(aa <= bb) = " << bool(aa <= bb) << std::endl << std::endl << std::endl;
        }
        if (bool(a >= b) != bool(aa >= bb)) {
            std::cout << "aa = " << a << " | bb = " << b << std::endl;
            std::cout << "a = " << a << " | b = " << b << std::endl;
            std::cout << "bool(a >= b) = " << bool(a >= b) << std::endl;
            std::cout << "bool(aa >= bb) = " << bool(aa >= bb) << std::endl << std::endl << std::endl;
        }
        if (bool(a > b) != bool(aa > bb)) {
            std::cout << "aa = " << a << " | bb = " << b << std::endl;
            std::cout << "a = " << a << " | b = " << b << std::endl;
            std::cout << "bool(a > b) = " << bool(a > b) << std::endl;
            std::cout << "bool(aa > bb) = " << bool(aa > bb) << std::endl << std::endl << std::endl;
        }
        if (bool(a != b) != bool(aa != bb)) {
            std::cout << "aa = " << a << " | bb = " << b << std::endl;
            std::cout << "a = " << a << " | b = " << b << std::endl;
            std::cout << "bool(a != b) = " << bool(a != b) << std::endl;
            std::cout << "bool(aa != bb) = " << bool(aa != bb) << std::endl << std::endl << std::endl;
        }
        if (bool(a == b) != bool(aa == bb)) {
            std::cout << "aa = " << a << " | bb = " << b << std::endl;
            std::cout << "a = " << a << " | b = " << b << std::endl;
            std::cout << "bool(a == b) = " << bool(a == b) << std::endl;
            std::cout << "bool(aa == bb) = " << bool(aa == bb) << std::endl << std::endl << std::endl;
        }
        auto result = (++a %= a /= a *= a -=b++);
        //int res = (++aa %= aa /= aa *= aa -= bb++);
        if (!result) {
            std::cout << "aa = " << a << " | bb = " << b << std::endl;
            std::cout << "a = " << a << " | b = " << b << std::endl;
            ++a %= a /=a *= a -=b++;
            //++aa %= aa /=aa *= aa -=bb++;
            std::cout << "a++ = " << a << std::endl;
            std::cout << "aa++ = " << aa++ << std::endl << std::endl << std::endl;
        }
    }
    //BigInteger a(-604), b(-3);
    //std::cout << a / b;
    return 0;
} //*/
