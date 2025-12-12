#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>
#include <chrono>

using namespace std;

using Matrix = vector<vector<int>>;

void multiply_rows(
    const Matrix& A,
    const Matrix& B,
    Matrix& C,
    int start_row,
    int end_row
) {
    int M = B.size();
    int P = B[0].size();

    for (int i = start_row; i < end_row; i++) {
        for (int j = 0; j < P; j++) {
            int sum = 0;
            for (int k = 0; k < M; k++) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
}

int main() {
    int N, M, P;
    cin >> N >> M >> P;

    Matrix A(N, vector<int>(M));
    Matrix B(M, vector<int>(P));
    Matrix C(N, vector<int>(P, 0));

    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            cin >> A[i][j];

    for (int i = 0; i < M; i++)
        for (int j = 0; j < P; j++)
            cin >> B[i][j];

    /* ---------- SINGLE THREAD ---------- */
    auto start_single = chrono::high_resolution_clock::now();

    multiply_rows(A, B, C, 0, N);

    auto end_single = chrono::high_resolution_clock::now();
    double single_time =
        chrono::duration<double>(end_single - start_single).count();

    /* ---------- MULTI THREAD ---------- */
    fill(C.begin(), C.end(), vector<int>(P, 0));

    unsigned int num_threads = thread::hardware_concurrency();
    if (num_threads == 0) num_threads = 4;
    num_threads = min(num_threads, (unsigned int)N);

    vector<thread> threads;
    threads.reserve(num_threads);

    int rows_per_thread = N / num_threads;
    int extra = N % num_threads;

    auto start_multi = chrono::high_resolution_clock::now();

    int row = 0;
    for (unsigned int i = 0; i < num_threads; i++) {
        int start = row;
        int end = start + rows_per_thread + (i < extra ? 1 : 0);
        row = end;

        threads.emplace_back(
            multiply_rows,
            cref(A),
            cref(B),
            ref(C),
            start,
            end
        );
    }

    for (auto& t : threads) t.join();

    auto end_multi = chrono::high_resolution_clock::now();
    double multi_time =
        chrono::duration<double>(end_multi - start_multi).count();

    cout << "\nPerformance Results\n";
    cout << "-------------------\n";
    cout << "Matrix size: " << N << " x " << M << " * " << M << " x " << P << "\n";
    cout << "Single-thread time : " << single_time << " seconds\n";
    cout << "Multi-thread time  : " << multi_time << " seconds\n";
    cout << "Speedup            : " << single_time / multi_time << "x\n";

    return 0;
}
