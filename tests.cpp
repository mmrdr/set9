#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <chrono>
#include <fstream>
#include <cassert>

using namespace std;
using namespace chrono;

class StringGenerator {
public:
    static const string ch; 
    
    StringGenerator() : rng(random_device{}()) {}
    
    string generateString(int length) {
        uniform_int_distribution<int> dist(0, ch.size() - 1);
        string result;
        for (int i = 0; i < length; ++i) {
            result += ch[dist(rng)];
        }
        return result;
    }
    
    vector<string> generateRandomStrings(int count) {
        uniform_int_distribution<int> dist(10, 200);
        vector<string> arr;
        for (int i = 0; i < count; ++i) {
            arr.push_back(generateString(dist(rng)));
        }
        return arr;
    }
    
    vector<string> generateReversedStrings(int count) {
        auto arr = generateRandomStrings(count);
        sort(arr.begin(), arr.end(), greater<string>());
        return arr;
    }
    
    vector<string> generateAlmostSortedStrings(int count, int swaps = 15) {
        auto arr = generateRandomStrings(count);
        sort(arr.begin(), arr.end());
        
        uniform_int_distribution<int> dist(0, count - 1);
        for (int i = 0; i < swaps; ++i) {
            int a = dist(rng);
            int b = dist(rng);
            swap(arr[a], arr[b]);
        }
        return arr;
    }
    
    vector<string> generateSamePrefixStrings(int count) {
        vector<string> result;
        string prefix = "test_prefix";
        result.reserve(count);
        for (size_t i = 0; i < count; ++i) {
            string suffix = generateString(count);
            result.push_back(prefix + suffix);
        }
        return result;
    }

private:
    mt19937 rng;
};

const string StringGenerator::ch = 
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#%:;^&*()-.";

class ComparisonCounter {
private:
    size_t count = 0;
    
public:
    ComparisonCounter() : count(0) {}
    
    void reset() {
        count = 0;
    }
    
    size_t getCount() const {
        return count;
    }
    
    int compareStrings(const string& a, const string& b) {
        count++;
        return a.compare(b);
    }
    
    int compareChars(const string& a, const string& b, int pos = 0) {
        if (pos >= a.length() && pos >= b.length()) return 0;
        if (pos >= a.length()) return -1;
        if (pos >= b.length()) return 1;
        
        count++;
        return a[pos] - b[pos];
    }
    
    int lcpCompare(const string& a, const string& b, int lcp = 0) {
        int len = min(a.length(), b.length());
        while (lcp < len && a[lcp] == b[lcp]) {
            count++;
            lcp++;
        }
        
        if (lcp < a.length() && lcp < b.length()) {
            count++;
            return a[lcp] < b[lcp] ? -1 : 1;
        }
        
        if (lcp == a.length() && lcp == b.length()) {
            return 0;
        }
        
        return lcp == a.length() ? -1 : 1;
    }
    
    int charAt(const string &s, int d) {
        return d < s.length() ? s[d] : -1;
    }
};

class StringSortTester {
private:
    ComparisonCounter counter;

    void standardQuickSort(vector<string>& arr, int left, int right) {
        if (left >= right) return;
        
        int pivotIndex = left + (right - left) / 2;
        string pivot = arr[pivotIndex];
        
        swap(arr[pivotIndex], arr[right]);
        int storeIndex = left;
        
        for (int i = left; i < right; ++i) {
            if (counter.compareStrings(arr[i], pivot) < 0) {
                swap(arr[i], arr[storeIndex]);
                storeIndex++;
            }
        }
        
        swap(arr[storeIndex], arr[right]);
        
        standardQuickSort(arr, left, storeIndex - 1);
        standardQuickSort(arr, storeIndex + 1, right);
    }
    
    void standardMergeSort(vector<string>& arr, int left, int right) {
        if (left >= right) return;
        
        int mid = left + (right - left) / 2;
        standardMergeSort(arr, left, mid);
        standardMergeSort(arr, mid + 1, right);
        
        vector<string> temp(right - left + 1);
        int i = left, j = mid + 1, k = 0;
        
        while (i <= mid && j <= right) {
            if (counter.compareStrings(arr[i], arr[j]) <= 0) {
                temp[k++] = arr[i++];
            } else {
                temp[k++] = arr[j++];
            }
        }
        
        while (i <= mid) {
            temp[k++] = arr[i++];
        }
        
        while (j <= right) {
            temp[k++] = arr[j++];
        }
        
        for (i = 0; i < k; ++i) {
            arr[left + i] = temp[i];
        }
    }

    void stringQuickSort(vector<string>& arr, int left, int right, int d) {
        if (right <= left) return;
        
        int lt = left, gt = right;
        int pivot = counter.charAt(arr[left], d);
        int i = left + 1;
        
        while (i <= gt) {
            int current = counter.charAt(arr[i], d);
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
    
    void stringMergeSort(vector<string>& arr, int left, int right, vector<int>& lcps) {
        if (left >= right) return;
        
        int mid = left + (right - left) / 2;
        stringMergeSort(arr, left, mid, lcps);
        stringMergeSort(arr, mid + 1, right, lcps);

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
            int cmp = counter.lcpCompare(L[i], R[j], min(lcpsL[i], lcpsR[j]));
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
    
    void msdRadixSort(vector<string>& arr, int pos = 0) {
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
                    msdRadixSort(buckets[i], pos + 1);
                    arr.insert(arr.end(), buckets[i].begin(), buckets[i].end());
                }
            }
        }   
    }

    void hybridRadixSort(vector<string>& arr, int left, int right, int d) {
        if (right - left + 1 < 74) {
            stringQuickSort(arr, left, right, d);
            return;
        }
        
        vector<int> count(256 + 2, 0);
        vector<string> temp(right - left + 1);
        
        for (int i = left; i <= right; i++) {
            int c = counter.charAt(arr[i], d);
            count[c + 2]++;
        }
        
        for (int i = 1; i < 256 + 2; i++) {
            count[i] += count[i - 1];
        }
        
        for (int i = left; i <= right; i++) {
            int c = counter.charAt(arr[i], d);
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
    
public:
    struct TestResult {
        string algorithmName;
        string dataType;
        int dataSize;
        double executionTime;
        size_t comparisons;
        
        TestResult(const string& alg, const string& type, int size, double time, size_t comp)
            : algorithmName(alg), dataType(type), dataSize(size), executionTime(time), comparisons(comp) {}
    };
    
    TestResult testStandardQuickSort(vector<string> arr, const string& dataType) {
        counter.reset();
        auto start = high_resolution_clock::now();
        
        standardQuickSort(arr, 0, arr.size() - 1);
        
        auto end = high_resolution_clock::now();
        duration<double, milli> duration = end - start;
        
        return TestResult("Standard QuickSort", dataType, arr.size(), duration.count(), counter.getCount());
    }
    
    TestResult testStandardMergeSort(vector<string> arr, const string& dataType) {
        counter.reset();
        auto start = high_resolution_clock::now();
        
        standardMergeSort(arr, 0, arr.size() - 1);
        
        auto end = high_resolution_clock::now();
        duration<double, milli> duration = end - start;
        
        return TestResult("Standard MergeSort", dataType, arr.size(), duration.count(), counter.getCount());
    }
    
    TestResult testStringQuickSort(vector<string> arr, const string& dataType) {
        counter.reset();
        auto start = high_resolution_clock::now();
        
        stringQuickSort(arr, 0, arr.size() - 1, 0);
        
        auto end = high_resolution_clock::now();
        duration<double, milli> duration = end - start;
        
        return TestResult("String QuickSort", dataType, arr.size(), duration.count(), counter.getCount());
    }
    
    TestResult testStringMergeSort(vector<string> arr, const string& dataType) {
        counter.reset();
        auto start = high_resolution_clock::now();
        
        vector<int> lcps(arr.size(), 0);
        stringMergeSort(arr, 0, arr.size() - 1, lcps);
        
        auto end = high_resolution_clock::now();
        duration<double, milli> duration = end - start;
        
        return TestResult("String MergeSort with LCP", dataType, arr.size(), duration.count(), counter.getCount());
    }

    TestResult testMsdRadixSort(vector<string> arr, const string& dataType) {
        counter.reset();
        auto start = high_resolution_clock::now();
        
        msdRadixSort(arr);
        
        auto end = high_resolution_clock::now();
        duration<double, milli> duration = end - start;
        
        return TestResult("MSD Radix Sort", dataType, arr.size(), duration.count(), counter.getCount());
    }
    
    TestResult testHybridRadixSort(vector<string> arr, const string& dataType) {
        counter.reset();
        auto start = high_resolution_clock::now();
        
        hybridRadixSort(arr, 0, arr.size() - 1, 0);
        
        auto end = high_resolution_clock::now();
        duration<double, milli> duration = end - start;
        
        return TestResult("Hybrid MSD Radix Sort", dataType, arr.size(), duration.count(), counter.getCount());
    }
    
    void saveResultsToCSV(const vector<TestResult>& results, const string& filename) {
        ofstream file(filename);
        
        if (!file.is_open()) {
            cerr << "Failed to open file: " << filename << "\n";
            return;
        }
        
        file << "Algorithm,DataType,DataSize,ExecutionTime(ms),Comparisons\n";
        
        for (const auto& result : results) {
            file << result.algorithmName << ","
                 << result.dataType << ","
                 << result.dataSize << ","
                 << fixed << result.executionTime << ","
                 << result.comparisons << "\n";
        }
        
        file.close();
    }
};

int main() {
    StringGenerator generator;
    StringSortTester tester;
    
    vector<StringSortTester::TestResult> results;
    
    vector<int> sizes;

    for (int size = 100; size <= 3000; size += 100) {
      sizes.push_back(size);
    }
    
    vector<string> randomArray = generator.generateRandomStrings(3000);
    vector<string> reverseSortedArray = generator.generateReversedStrings(3000);
    vector<string> nearlySortedArray = generator.generateAlmostSortedStrings(3000);
    vector<string> commonPrefixArray = generator.generateSamePrefixStrings(3000);
    
    for (int size : sizes) {
        cout << "Testing arrays of size " << size << "..." << "\n";
        
        vector<string> randomSubarray(randomArray.begin(), randomArray.begin() + size);
        vector<string> reverseSortedSubarray(reverseSortedArray.begin(), reverseSortedArray.begin() + size);
        vector<string> nearlySortedSubarray(nearlySortedArray.begin(), nearlySortedArray.begin() + size);
        vector<string> commonPrefixSubarray(commonPrefixArray.begin(), commonPrefixArray.begin() + size);

        results.push_back(tester.testStandardQuickSort(randomSubarray, "Random"));
        results.push_back(tester.testStandardQuickSort(reverseSortedSubarray, "ReverseSorted"));
        results.push_back(tester.testStandardQuickSort(nearlySortedSubarray, "NearlySorted"));
        results.push_back(tester.testStandardQuickSort(commonPrefixSubarray, "CommonPrefix"));
        
        results.push_back(tester.testStandardMergeSort(randomSubarray, "Random"));
        results.push_back(tester.testStandardMergeSort(reverseSortedSubarray, "ReverseSorted"));
        results.push_back(tester.testStandardMergeSort(nearlySortedSubarray, "NearlySorted"));
        results.push_back(tester.testStandardMergeSort(commonPrefixSubarray, "CommonPrefix"));

        results.push_back(tester.testStringQuickSort(randomSubarray, "Random"));
        results.push_back(tester.testStringQuickSort(reverseSortedSubarray, "ReverseSorted"));
        results.push_back(tester.testStringQuickSort(nearlySortedSubarray, "NearlySorted"));
        results.push_back(tester.testStringQuickSort(commonPrefixSubarray, "CommonPrefix"));
        
        results.push_back(tester.testStringMergeSort(randomSubarray, "Random"));
        results.push_back(tester.testStringMergeSort(reverseSortedSubarray, "ReverseSorted"));
        results.push_back(tester.testStringMergeSort(nearlySortedSubarray, "NearlySorted"));
        results.push_back(tester.testStringMergeSort(commonPrefixSubarray, "CommonPrefix"));
        
        results.push_back(tester.testMsdRadixSort(randomSubarray, "Random"));
        results.push_back(tester.testMsdRadixSort(reverseSortedSubarray, "ReverseSorted"));
        results.push_back(tester.testMsdRadixSort(nearlySortedSubarray, "NearlySorted"));
        results.push_back(tester.testMsdRadixSort(commonPrefixSubarray, "CommonPrefix"));
        
        results.push_back(tester.testHybridRadixSort(randomSubarray, "Random"));
        results.push_back(tester.testHybridRadixSort(reverseSortedSubarray, "ReverseSorted"));
        results.push_back(tester.testHybridRadixSort(nearlySortedSubarray, "NearlySorted"));
        results.push_back(tester.testHybridRadixSort(commonPrefixSubarray, "CommonPrefix"));
    }
    
    tester.saveResultsToCSV(results, "sorting_results.csv");
    
    cout << "Testing completed. Results saved to sorting_results.csv" << "\n";
}