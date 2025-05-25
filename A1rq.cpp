#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

size_t getMaxLength(const vector<string>& arr) {
    size_t maxLen = 0;
    for (const string& s : arr) {
        maxLen = max(maxLen, s.length());
    }
    return maxLen;
}

int charAt(const string& s, int d) {
    return d < s.length() ? (unsigned char)s[d] : -1;
}

void stringQuickSort(vector<string>& arr, int left, int right, int d) {
    if (left >= right) return;

    int lt = left, gt = right;
    int pivot = charAt(arr[left], d);
    int i = left + 1;

    while (i <= gt) {
        int current = charAt(arr[i], d);
        if (current < pivot) {
            swap(arr[lt++], arr[i++]);
        } else if (current > pivot) {
            swap(arr[i], arr[gt--]);
        } else {
            i++;
        }
    }

    stringQuickSort(arr, left, lt - 1, d);
    if (pivot >= 0) {
        stringQuickSort(arr, lt, gt, d + 1);
    }
    stringQuickSort(arr, gt + 1, right, d);
}

void hybridRadixSort(vector<string>& arr, int left, int right, int d) {
    if (right - left + 1 < 70) {
        stringQuickSort(arr, left, right, d);
        return;
    }

    vector<int> count(256 + 2, 0);
    vector<string> temp(right - left + 1);

    for (int i = left; i <= right; i++) {
        int c = charAt(arr[i], d);
        count[c + 2]++;
    }

    for (int i = 1; i < 256 + 2; i++) {
        count[i] += count[i - 1];
    }

    for (int i = left; i <= right; i++) {
        int c = charAt(arr[i], d);
        temp[count[c + 1]++] = arr[i];
    }

    for (int i = left; i <= right; i++) {
        arr[i] = temp[i - left];
    }

    for (int c = 0; c < 256; c++) {
        int start = left + count[c];
        int end = left + count[c + 1] - 1;
        if (start < end) {
            hybridRadixSort(arr, start, end, d + 1);
        }
    }
}

void sort(vector<string>& arr) {
    if (arr.size() <= 1) return;
    hybridRadixSort(arr, 0, arr.size() - 1, 0);
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
    sort(arr);
    for (const string& s : arr) {
        cout << s << '\n';
    }
}