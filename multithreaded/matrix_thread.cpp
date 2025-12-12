#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>

using namespace std;

using Matrix = vector<vector<int>>;

void multiply_rows(
    const Matrix& A,
    const Matrix& B,
    Matrix& C,
    int start_row,
    int end_row
) {
    std::cout << "Thread ID: " << std::this_thread::get_id()
          << " handling rows " << start_row << " to " << end_row << "\n";
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
    cout << "Enter N M P: ";
    cin >> N >> M >> P;

    Matrix A(N, vector<int>(M));
    Matrix B(M, vector<int>(P));
    Matrix C(N, vector<int>(P, 0));

    cout << "Enter matrix A:\n";
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            cin >> A[i][j];

    cout << "Enter matrix B:\n";
    for (int i = 0; i < M; i++)
        for (int j = 0; j < P; j++)
            cin >> B[i][j];

    unsigned int num_threads = thread::hardware_concurrency();
    if (num_threads == 0) num_threads = 4;
    num_threads = min(num_threads, (unsigned int)N);

    vector<thread> threads;
    threads.reserve(num_threads);

    int rows_per_thread = N / num_threads;
    int extra = N % num_threads;

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

    cout << "Result matrix C:\n";
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < P; j++)
            cout << C[i][j] << " ";
        cout << "\n";
    }

    return 0;
}
