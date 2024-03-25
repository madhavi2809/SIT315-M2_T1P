#include <iostream>
#include <fstream>
#include <chrono>
#include <random>

using namespace std;
using namespace chrono;

const int MAX_SIZE = 10;

// Function to perform matrix multiplication
void Multiplication(int A[][MAX_SIZE], int B[][MAX_SIZE], int C[][MAX_SIZE], int N) {
    for (int i = 0; i < N; ++i) {   // Iteartion over rows of matrix C
        for (int j = 0; j < N; ++j) {   //Iteration over columns of matrix C
            C[i][j] = 0;
            // Iteration over row elements of matrix A and column elements of matrix B
            for (int k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// Function to generate random values for a matrix
void generateRandomMatrix(int matrix[][MAX_SIZE], int N) {
    random_device rnd;
    mt19937 gen(rnd());
    uniform_int_distribution<> dis(1, 10);  // Range of random number set from 1 to 10
    for (int i = 0; i < N; ++i) {    // Iteration over rows of matrix
        for (int j = 0; j < N; ++j) {  // Iteration over columns of matrix
            matrix[i][j] = dis(gen);    // Random value generated
        }
    }
}

int main() {
    // Size of matrix taken from user
    int N;
    cout << "Enter the size of the matrix (up to " << MAX_SIZE << "): ";
    cin >> N;

    // Check if matrix size is within bounds
    if (N <= 0 || N > MAX_SIZE) {
        cout << "Invalid matrix size." << endl;
        return 1;
    }

    // This declares three 2D arrays to store the matrices
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
    cout << "Execution time: " << duration.count() << " microseconds" << endl;

    // Write output to a txt file
    ofstream outputFile("output.txt");
    if (outputFile.is_open()) {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                outputFile << C[i][j] << " ";
            }
            outputFile << endl;
        }
        outputFile.close();
        cout << "Output written to output.txt" << endl;
    } else {
        cerr << "Unable to open file for writing." << endl;
    }

    return 0;
}
