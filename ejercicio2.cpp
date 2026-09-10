#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include <iomanip>

using namespace std;
using namespace std::chrono;

// Insertion sort sobre A[left..right]
void insertionSort(vector<int>& A, int left, int right) {
    for (int i = left + 1; i <= right; i++) {
        int key = A[i];
        int j = i - 1;
        while (j >= left && A[j] > key) {
            A[j + 1] = A[j];
            j--;
        }
        A[j + 1] = key;
    }
}

// Merge estandar de A[left..mid] y A[mid+1..right]
void merge(vector<int>& A, int left, int mid, int right) {
    vector<int> L(A.begin() + left,    A.begin() + mid + 1);
    vector<int> R(A.begin() + mid + 1, A.begin() + right + 1);

    int i = 0, j = 0, k = left;
    while (i < (int)L.size() && j < (int)R.size()) {
        if (L[i] <= R[j]) A[k++] = L[i++];
        else              A[k++] = R[j++];
    }
    while (i < (int)L.size()) A[k++] = L[i++];
    while (j < (int)R.size()) A[k++] = R[j++];
}

// Hibrido: si el segmento tiene tamaño <= k usa insertion sort,
// si no, divide y vence + merge. Con k=1 equivale a merge sort normal
void hybridMergeSort(vector<int>& A, int left, int right, int k) {
    if (right - left + 1 <= k) {
        insertionSort(A, left, right);
        return;
    }
    int mid = left + (right - left) / 2;
    hybridMergeSort(A, left, mid, k);
    hybridMergeSort(A, mid + 1, right, k);
    merge(A, left, mid, right);
}

int main() {
    const int n = 1000000;

    vector<int> base(n);
    mt19937 gen(42);
    uniform_int_distribution<int> dist(0, 1000000000);
    for (int i = 0; i < n; i++) base[i] = dist(gen);

    int valores_k[] = {1, 2, 4, 8, 16, 32, 64, 128};

    cout << "n = " << n << "\n\n";
    cout << "   k   |  Tiempo (ms)\n";
    cout << "-------+-------------\n";

    for (int k : valores_k) {
        vector<int> A = base; // misma data para cada k (comparacion justa)

        auto inicio = high_resolution_clock::now();
        hybridMergeSort(A, 0, n - 1, k);
        auto fin = high_resolution_clock::now();

        double ms = duration_cast<microseconds>(fin - inicio).count() / 1000.0;

        cout << setw(6) << k << " | "
             << setw(11) << fixed << setprecision(3) << ms << "\n";
    }
    return 0;
}