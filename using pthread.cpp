#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
#include <pthread.h>

using namespace std;
using namespace chrono;

const int MAX_SIZE = 10;
const int MAX_THREADS = 10;

// Global variables
int A[MAX_SIZE][MAX_SIZE], B[MAX_SIZE][MAX_SIZE], C[MAX_SIZE][MAX_SIZE];
int N;

// Mutex lock for matrix C
pthread_mutex_t mutexC;

// Struct for passing data to threads
struct ThreadData {
    int startRow;
    int endRow;
};

// Function to perform matrix multiplication for a subset of rows
void* matrixMultiplication(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    for (int i = data->startRow; i < data->endRow; ++i) {
        for (int j = 0; j < N; ++j) {
            int sum = 0;
            for (int k = 0; k < N; ++k) {
                sum += A[i][k] * B[k][j];
            }
            pthread_mutex_lock(&mutexC);
            C[i][j] = sum;
            pthread_mutex_unlock(&mutexC);
        }
    }
    pthread_exit(NULL);
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
    cout << "Enter the size of the matrix (up to " << MAX_SIZE << "): ";
    cin >> N;

    // Check if matrix size is within bounds
    if (N <= 0 || N > MAX_SIZE) {
        cout << "Invalid matrix size." << endl;
        return 1;
    }

    // Generate random values for matrices A and B
    generateRandomMatrix(A, N);
    generateRandomMatrix(B, N);

    // Initialize matrix C to zeros
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            C[i][j] = 0;
        }
    }

    // Initialize mutex lock
    pthread_mutex_init(&mutexC, NULL);

    // Start measuring execution time
    auto start = high_resolution_clock::now();

    // Create and start threads
    pthread_t threads[MAX_THREADS];
    ThreadData threadData[MAX_THREADS];
    int numThreads = min(N, MAX_THREADS);
    int rowsPerThread = N / numThreads;
    for (int i = 0; i < numThreads; ++i) {
        threadData[i].startRow = i * rowsPerThread;
        threadData[i].endRow = (i == numThreads - 1) ? N : (i + 1) * rowsPerThread;
        pthread_create(&threads[i], NULL, matrixMultiplication, (void*)&threadData[i]);
    }

    // Join threads
    for (int i = 0; i < numThreads; ++i) {
        pthread_join(threads[i], NULL);
    }

    // Stop measuring execution time
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    // Destroy mutex lock
    pthread_mutex_destroy(&mutexC);

    // Print execution time
    cout << "Execution time: " << duration.count() << " microseconds" << endl;

    // Write output to a file
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
