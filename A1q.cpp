#include <iostream>
#include <vector>
#include <string>

using namespace std;

int charAt(const string &s, int d) {
    return d < s.length() ? s[d] : -1;
}

void stringQuickSort(vector<string> &arr, int low, int high, int d) {
    if (high <= low) return;

    int lt = low, gt = high;
    int pivot = charAt(arr[low], d);
    int i = low + 1;

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

    stringQuickSort(arr, low, lt - 1, d);
    if (pivot >= 0) {
        stringQuickSort(arr, lt, gt, d + 1);
    }
    stringQuickSort(arr, gt + 1, high, d);
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
    stringQuickSort(arr, 0, n - 1, 0);
    for (const string &s : arr) {
        cout << s << '\n';
    }
}