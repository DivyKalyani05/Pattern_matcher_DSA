#include <iostream>
#include <vector>
#include <string>
using namespace std;

const int base = 31;
const int mod = 1e9 + 7;

// Compute hash for a string, ignoring wildcards
long long computeHash(const string &s) {
    long long hash = 0;
    long long p = 1;

    for (char ch : s) {
        if (ch != '_') { // ignore wildcards
            hash = (hash + (ch - 'a' + 1) * p) % mod;
        }
        p = (p * base) % mod;
    }
    return hash;
}

// Finds all occurrences of pattern p in text t
void WildCard_Finder(const string &text, const string &pattern) {
    int n = text.size();
    int m = pattern.size();
    vector<int> result;

    long long patHash = computeHash(pattern);
    long long textHash = 0;
    long long p = 1;

    // Precompute powers
    vector<long long> power(m);
    power[0] = 1;
    for (int i = 1; i < m; i++)
        power[i] = (power[i - 1] * base) % mod;

    // Slide a window of length m over text
    for (int i = 0; i <= n - m; i++) {
        textHash = 0;

        // Compute hash of text window matching non-wildcard pattern
        for (int j = 0; j < m; j++) {
            if (pattern[j] != '_') {
                textHash = (textHash + (text[i + j] - 'a' + 1) * power[j]) % mod;
            }
        }

        // If hash matches, it's a potential match
        if (textHash == patHash) {
            result.push_back(i);  // store starting index
        }
    }

    // Output all matched positions
    cout << "Pattern found at indices: ";
    for (int i : result) cout << i << " ";
    cout << endl;
}