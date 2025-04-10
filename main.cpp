#include <iostream>
#include <vector>
#include <utility>
#include <cstdlib>
#include <random>


struct punct {
    long long x;
    long long y;
    
    friend std::ostream& operator<<(std::ostream& os, const punct& p) {
        if (p.x == -1 && p.y == -1)
            os << "O (infinit)";
        else
            os << "(" << p.x << ", " << p.y << ")";
        return os;
    }
    
    bool operator==(const punct& other) const {
        return x == other.x && y == other.y;
    }
};

bool isPrime(long long n) {
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (long long i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    }
    return true;
}

long long modPow(long long base, long long exponent, long long modulus) {
    long long result = 1;
    base = base % modulus;
    while (exponent > 0) {
        if (exponent % 2 == 1)
            result = (result * base) % modulus;
        exponent = exponent >> 1;
        base = (base * base) % modulus;
    }
    
    return result;
}

bool rezid(long long z, long long p) {
    return modPow(z, (p - 1) / 2, p) == 1;
}

long long modSqrt(long long z, long long p) {
    return modPow(z, (p + 1) / 4, p);
}

std::vector<punct> gen_pct_curb(long long a, long long b, long long p) {
    std::vector<punct> puncte;
    
    if (!isPrime(p)) {
        std::cout << "p nu este un număr prim" << std::endl;
        return puncte;
    }
    
    long long discriminant = (4 * modPow(a, 3, p) + 27 * modPow(b, 2, p)) % p;
    if (discriminant == 0) {
        std::cout << "curba nu este valida " << std::endl;
        return puncte;
    }
    
    if (p % 4 != 3) {
        std::cout << "p invalid " << std::endl;
        return puncte;
    }
    
    puncte.push_back({-1, -1});
    
    for (long long x = 0; x < p; x++) {
        long long z = (modPow(x, 3, p) + a * x + b) % p;
        if (rezid(z, p)) {
            long long y1 = modSqrt(z, p);
            long long y2 = (p - y1) % p;
            
            puncte.push_back({x, y1});
            if (y1 != y2) {
                puncte.push_back({x, y2});
            }
        }
    }
    
    return puncte;
}

punct adunaPuncte(const punct& P, const punct& Q, long long a, long long p) {
    if (P.x == -1 && P.y == -1) return Q;
    if (Q.x == -1 && Q.y == -1) return P;
    
    long long lambda;
    
    if (P.x == Q.x && (P.y + Q.y) % p == 0) {
        return {-1, -1};
    }
    
    if (P.x == Q.x && P.y == Q.y) {
        long long numerator = (3 * modPow(P.x, 2, p) + a) % p;
        long long denominator = (2 * P.y) % p;
        
        long long inv_denominator = modPow(denominator, p - 2, p);
        lambda = (numerator * inv_denominator) % p;
    } else {
        long long numerator = (Q.y - P.y + p) % p;
        long long denominator = (Q.x - P.x + p) % p;
        
        long long inv_denominator = modPow(denominator, p - 2, p);
        lambda = (numerator * inv_denominator) % p;
    }
    
    long long x3 = (modPow(lambda, 2, p) - P.x - Q.x + 2 * p) % p;
    long long y3 = (lambda * (P.x - x3 + p) - P.y + p) % p;
    
    return {x3, y3};
}

punct inmultestePunct(const punct& P, long long k, long long a, long long p) {
    punct result = {-1, -1};
    punct temp = P;
    
    while (k > 0) {
        if (k % 2 == 1) {
            result = adunaPuncte(result, temp, a, p);
        }
        temp = adunaPuncte(temp, temp, a, p);
        k /= 2;
    }
    
    return result;
}

std::pair<punct, punct> cript(const punct& M, const punct& P1, const punct& P2, long long k, long long a, long long p) {
    punct P3 = inmultestePunct(P1, k, a, p);
    punct P4 = adunaPuncte(M, inmultestePunct(P2, k, a, p), a, p);
    return {P3, P4};
}

punct decript(const punct& P3, const punct& P4, long long q, long long a, long long p) {
    punct qP3 = inmultestePunct(P3, q, a, p);
    punct negQP3 = {qP3.x, (p - qP3.y) % p};
    return adunaPuncte(P4, negQP3, a, p);
}

bool valid_curba(const punct& P, long long a, long long b, long long p) {
    if (P.x == -1 && P.y == -1) return true;
    return (modPow(P.y, 2, p) == (modPow(P.x, 3, p) + a * P.x + b) % p);
}

punct citestePunct(const std::string& mesaj) {
    punct P;
    std::cout << mesaj << std::endl;
    std::cout << "x: ";
    std::cin >> P.x;
    std::cout << "y: ";
    std::cin >> P.y;
    return P;
}


int main() {
    long long a, b, p;
    std::cout << "a: ";
    std::cin >> a;
    std::cout << "b: ";
    std::cin >> b;
    std::cout << "p (nr. prim): ";
    std::cin >> p;
    
    std::vector<punct> puncte = gen_pct_curb(a, b, p);
    std::cout << "\nCurba eliptica: y^2 = x^3 + " << a << "x + " << b << " (mod " << p << ")" << std::endl;
    
    if (puncte.empty()) {
        std::cout << "nu s-au putut genera puncte" << std::endl;
    } else {
        std::cout << "punctele curbei eliptice: " << std::endl;
        
        for (const auto& punct : puncte) {
            std::cout << "(" << punct.x << ", " << punct.y << ") ";
        }
    }
    
    int optiune;
    std::cout << "\n\nAlegeti o optiune: " << std::endl;
    std::cout << "1. Criptare" << std::endl;
    std::cout << "2. Decriptare" << std::endl;
    std::cin >> optiune;
    
    if (optiune == 1) {
        std::cout << "\nCRIPTARE: " << std::endl;
        
        punct P1 = citestePunct("P1: ");
        
        if (!valid_curba(P1, a, b, p)) {
            std::cout << "Eroare: Punctul P1 nu se afla pe curba!" << std::endl;
            return 0;
        }
        
        long long q;
        std::cout << "Introduceti cheia secreta q: ";
        std::cin >> q;
        
        punct P2 = inmultestePunct(P1, q, a, p);
        std::cout << "Cheia publica P2 = q*P1 = " << P2 << std::endl;
        
        punct M = citestePunct("Introduceti mesajul M: ");
        
        if (!valid_curba(M, a, b, p)) {
            std::cout << "Punctul M nu se afla pe curba!" << std::endl;
            return 0;
        }
        
        long long k;
        std::cout << "k(valoare aleatoare): ";
        std::cin >> k;
        
        auto criptat = cript(M, P1, P2, k, a, p);
        punct P3 = criptat.first;
        punct P4 = criptat.second;
        
        std::cout << "Text criptat:\nP3=" << P3 << ", \nP4=" << P4 << std::endl;
        
        punct Mdecriptat = decript(P3, P4, q, a, p);
        
        
        if (Mdecriptat == M) {
            std::cout << "\n" << std::endl;
        } else {
            std::cout << "Eroare" << std::endl;
        }
    }
    else if (optiune == 2) {
        std::cout << "\nDECRIPTARE" << std::endl;
        
        punct P3 = citestePunct("P3: ");
        punct P4 = citestePunct("P4: ");
        
        long long q;
        std::cout << "cheia secreta q: ";
        std::cin >> q;
        
        punct Mdecriptat = decript(P3, P4, q, a, p);
        
        std::cout << "\nText decriptat: " << Mdecriptat << std::endl;
        
        if (valid_curba(Mdecriptat, a, b, p)) {
            std::cout << "\n" << std::endl;
        } else {
            std::cout << "Punctul decriptat nu se află pe curbă." << std::endl;
        }
    }
    else {
        std::cout << "Optiune invalida" << std::endl;
    }
    
    return 0;
}
