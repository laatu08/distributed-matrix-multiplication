#include <fstream>
#include <cstdlib>

int main() {
    int N = 300;
    std::ofstream out("input.txt");

    out << N << " " << N << " " << N << "\n";

    // Matrix A
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            out << (rand() % 10) << " ";
        }
        out << "\n";
    }

    // Matrix B
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            out << (rand() % 10) << " ";
        }
        out << "\n";
    }

    return 0;
}
