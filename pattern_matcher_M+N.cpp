#include <iostream>
#include <vector>
using namespace std;

const int base = 31;
const int mod = 1e9 + 7;

// Compute hash of pattern (ignoring wildcards)
long long computePatternHash(const string &pattern, const vector<long long> &power) {
    long long hash = 0;
    for (int i = 0; i < pattern.size(); ++i) {
        if (pattern[i] != '_') {
            hash = (hash + (pattern[i] - 'a' + 1) * power[i]) % mod;
        }
    }
    return hash;
}

// Precompute prefix hashes of text (for each position in pattern that is NOT wildcard)
void computeTextHashes(const string &text, const string &pattern,
                       vector<long long> &prefixHash, const vector<long long> &power,
                       const vector<bool> &mask) {
    int n = text.size(), m = pattern.size();
    prefixHash.resize(n + 1, 0);

    for (int i = 0; i < n; ++i) {
        int val = (text[i] - 'a' + 1);
        if (i < m && mask[i]) {
            prefixHash[i + 1] = (prefixHash[i] + val * power[i]) % mod;
        } else {
            prefixHash[i + 1] = prefixHash[i];
        }
    }
}

// Get rolling hash of a window [i...i+m) using the precomputed prefix hash
long long getWindowHash(int i, const vector<long long> &prefixHash,
                        const vector<long long> &power, const vector<bool> &mask) {
    long long hash = 0;
    int m = mask.size();
    for (int j = 0; j < m; ++j) {
        if (!mask[j]) continue;
        int pos = i + j;
        if (pos >= prefixHash.size() - 1) break;
        hash = (hash + (prefixHash[pos + 1] - prefixHash[pos] + mod) % mod * power[j]) % mod;
    }
    return hash;
}

void WildCard_Finder(const string &text, const string &pattern) {
    int n = text.size(), m = pattern.size();

    if (m > n) {
        cout << "Pattern longer than text\n";
        return;
    }

    // Precompute powers
    vector<long long> power(max(n, m) + 1);
    power[0] = 1;
    for (int i = 1; i < power.size(); i++) {
        power[i] = (power[i - 1] * base) % mod;
    }

    // Track which characters are not wildcards
    vector<bool> mask(m, false);
    for (int i = 0; i < m; ++i) {
        if (pattern[i] != '_') mask[i] = true;
    }

    // Compute pattern hash (only for non-wildcard positions)
    long long patHash = computePatternHash(pattern, power);

    // Precompute prefix hash of text
    vector<long long> prefixHash;
    computeTextHashes(text, pattern, prefixHash, power, mask);

    // Compare hashes in sliding window
    vector<int> matches;
    for (int i = 0; i <= n - m; ++i) {
        long long textHash = 0;

        // Compute hash of current window [i...i+m)
        for (int j = 0; j < m; ++j) {
            if (!mask[j]) continue;
            int idx = i + j;
            long long val = (text[idx] - 'a' + 1);
            textHash = (textHash + val * power[j]) % mod;
        }

        if (textHash == patHash) {
            matches.push_back(i);
        }
    }

    cout << "Pattern found at indices: ";
    for (int i : matches) cout << i << " ";
    cout << endl;
}
