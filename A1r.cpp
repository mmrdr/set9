#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

void radixSort(vector<string>& arr, int pos = 0) {
    if (arr.size() <= 1) {
        return;
    }
  
    vector<vector<string>> buckets(257);
    for (const auto& s : arr) {
        if (pos >= s.length()) {
        buckets[0].push_back(s);
        } else {
        unsigned char c = s[pos];
        buckets[c + 1].push_back(s);
        }
    }
  
    arr.clear();
    for (int i = 0; i < 257; ++i) {
        if (!buckets[i].empty()) {
            if (i == 0) {
                arr.insert(arr.end(), buckets[i].begin(), buckets[i].end());
            } else {
                radixSort(buckets[i], pos + 1);
                arr.insert(arr.end(), buckets[i].begin(), buckets[i].end());
            }
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    cin.ignore();
    vector<string> arr(n);
    for (int i = 0; i < n; i++) {
        getline(cin, arr[i]);
    }
    if (n == 0) {
        return 0;
    }
    radixSort(arr);
    for (const string& s : arr) {
        cout << s << '\n';
    }
}