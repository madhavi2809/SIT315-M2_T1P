#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
#include <omp.h>

using namespace std;
using namespace chrono;

const int MAX_SIZE = 10;

// Function to perform matrix multiplication
void Multiplication(int A[][MAX_SIZE], int B[][MAX_SIZE], int C[][MAX_SIZE], int N) {
    #pragma omp parallel for
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// Function to generate random values for a matrix
void generateRandomMatrix(int matrix[][MAX_SIZE], int N) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 10);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            matrix[i][j] = dis(gen);
        }
    }
}

int main() {
    int N;
    cout << "Enter the size of the matrix (up to " << MAX_SIZE << "): ";
    cin >> N;

    // Check if matrix size is within bounds
    if (N <= 0 || N > MAX_SIZE) {
        cout << "Invalid matrix size." << endl;
        return 1;
    }

    int A[MAX_SIZE][MAX_SIZE], B[MAX_SIZE][MAX_SIZE], C[MAX_SIZE][MAX_SIZE];

    // Generate random values for matrices A and B
    generateRandomMatrix(A, N);
    generateRandomMatrix(B, N);

    // Perform matrix multiplication and measure execution time
    auto start = high_resolution_clock::now();
    Multiplication(A, B, C, N);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    // Print execution time
    cout << "Execution time using OpenMP: " << duration.count() << " microseconds" << endl;

    // Write output to a file
    ofstream outputFile("output_omp.txt");
    if (outputFile.is_open()) {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                outputFile << C[i][j] << " ";
            }
            outputFile << endl;
        }
        outputFile.close();
        cout << "Output written to output_omp.txt" << endl;
    } else {
        cerr << "Unable to open file for writing." << endl;
    }

    return 0;
}
