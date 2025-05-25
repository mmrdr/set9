#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

int lcpCompare(const string &a, const string &b, int lcp = 0) {
    int len = min(a.length(), b.length());
    while (lcp < len && a[lcp] == b[lcp]) {
        lcp++;
    }
    if (lcp < a.length() && lcp < b.length()) {
        return a[lcp] < b[lcp] ? -1 : 1;
    }
    if (lcp == a.length() && lcp == b.length()) {
        return 0;
    }
    return lcp == a.length() ? -1 : 1;
}

void merge(vector<string> &arr, int left, int mid, int right, vector<int> &lcps) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    vector<string> L(n1), R(n2);
    vector<int> lcpsL(n1), lcpsR(n2);
    
    for (int i = 0; i < n1; i++) {
        L[i] = arr[left + i];
        lcpsL[i] = lcps[left + i];
    }
    for (int j = 0; j < n2; j++) {
        R[j] = arr[mid + 1 + j];
        lcpsR[j] = lcps[mid + 1 + j];
    }
    
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        int cmp = lcpCompare(L[i], R[j], min(lcpsL[i], lcpsR[j]));
        if (cmp <= 0) {
            arr[k] = L[i];
            lcps[k] = lcpsL[i];
            i++;
        } else {
            arr[k] = R[j];
            lcps[k] = lcpsR[j];
            j++;
        }
        k++;
    }
    
    while (i < n1) {
        arr[k] = L[i];
        lcps[k] = lcpsL[i];
        i++;
        k++;
    }
    
    while (j < n2) {
        arr[k] = R[j];
        lcps[k] = lcpsR[j];
        j++;
        k++;
    }
}

void mergeSort(vector<string> &arr, int left, int right, vector<int> &lcps) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid, lcps);
        mergeSort(arr, mid + 1, right, lcps);
        merge(arr, left, mid, right, lcps);
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
    vector<int> lcps(n, 0);
    
    mergeSort(arr, 0, n - 1, lcps);
    for (const string &s : arr) {
        cout << s << '\n';
    }
}