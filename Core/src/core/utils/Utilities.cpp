#include "Utilities.h"

long long Utilities::ComputeStringHash(std::string const& s) {
    const int p = 31;
    const int m = static_cast<int>(1e9 + 9);
    long long hash_value = 0;
    long long p_pow = 1;
    for (char c : s) {
        hash_value = (hash_value + (c - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    return hash_value;
}

std::string Utilities::GetRootFolder() {
    return "";
}
