#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

//Collab with Ivan Stewart
#define MAX 10 // Define maximum size of the matrix

// matA and matB are matrices with MAX rows and columns
int matA[MAX][MAX]; 
int matB[MAX][MAX]; 

// matSumResult, matDiffResult and matProductResult are matrices with MAX rows and columns
// that will be used to store the results of the matrix operations
int matSumResult[MAX][MAX];
int matDiffResult[MAX][MAX]; 
int matProductResult[MAX][MAX]; 

// A struct that represents a matrix cordinate 
typedef struct{
  int row;
  int column;
}Cordinate;


// fillMatrix() fills the given matrix with random values between 1 and 10
void fillMatrix(int matrix[MAX][MAX]) {
    for(int i = 0; i<MAX; i++) {
        for(int j = 0; j<MAX; j++) {
            matrix[i][j] = rand()%10+1; // Generate a random value between 1 and 10 and assign it to the current element of the matrix
        }
    }
}


// Function to print a matrix
void printMatrix(int matrix[MAX][MAX]) {
// Loop through each row and column of the matrix
for(int i = 0; i<MAX; i++) {
for(int j = 0; j<MAX; j++) {
// Print each element of the matrix with 5 spaces before it
printf("%5d", matrix[i][j]);
}
// After printing each row, add a newline
printf("\n");
}
// Add another newline after printing the entire matrix
printf("\n");
}

// Function to compute the sum of two matrices
void* computeSum(void* args) {
    // Cast the void pointer to a Cordinate pointer
    // pass in the number of the ith thread
    Cordinate *entry = (Cordinate *)args;
    int row = entry->row;
    int column = entry->column;

    // Compute the sum of the values at the given coordinates in matA and matB
    // and store the result in matSumResult
    matSumResult[row][column] = matA[row][column]+matB[row][column];
}

// Function to compute the difference of two matrices
void* computeDiff(void* args) {
    // pass in the number of the ith thread
    // Cast the void pointer to a Cordinate pointer
    Cordinate *entry = (Cordinate *)args;
    int row = entry->row;
    int column = entry->column;

    // Compute the difference of the values at the given coordinates in matA and matB
    // and store the result in matDiffResult
    matDiffResult[row][column] = matA[row][column]-matB[row][column];
}

// Function to compute the inner product of two matrices
void* computeProduct(void* args) {
    // pass in the number of the ith thread
    // Cast the void pointer to a Cordinate pointer
    Cordinate *entry = (Cordinate *)args;
    int row = entry->row;
    int column = entry->column;

    // Compute the inner product of the values at the given coordinates in matA and matB
    // and store the result in matProductResult
    matProductResult[row][column] = matA[row][column]*matB[row][column];

}

// Spawn a thread to fill each cell in each result matrix.
// How many threads will you spawn?
int main(int argc, char *argv[]) {
    srand(time(0));  // Do Not Remove. Just ignore and continue below.

    // 0. Get the matrix size from the command line and assign it to MAX
    int matrixSize;
    if (argc != 2) {
        printf("Usage: %s <matrix_size>\n", argv[0]);
        return 1;
    } else {
        matrixSize = atoi(argv[1]);
        if (matrixSize > MAX) {
            printf("Matrix size exceeds the maximum allowed size (%d). Using %d instead.\n", MAX, MAX);
            matrixSize = MAX;
        }
    }

    //int matrixSize = atoi(argv[1]);
    // int m = calloc (use calloc or malloc double star pointer)
  

    // 1. Fill the matrices (matA and matB) with random values.
    fillMatrix(matA);
    fillMatrix(matB);
    // 2. Print the initial matrices.
    printf("Matrix A:\n");
    printMatrix(matA);
    printf("Matrix B:\n");
    printMatrix(matB);
    
    // 3. Create pthread_t objects for our threads.
    int threadCount = matrixSize * matrixSize;
    pthread_t threadsSum[threadCount];
    pthread_t threadsDiff[threadCount];
    pthread_t threadsProduct[threadCount];

    // 4. Create a thread for each cell of each matrix operation.
    // You'll need to pass in the coordinates of the cell you want the thread
    // to compute.
    // One way to do this is to malloc memory for the thread number i, populate the coordinates
    // into that space, and pass that address to the thread. The thread will use that number to calculate
    // its portion of the matrix. The thread will then have to free that space when it's done with what's in that memory.

    int i;

    // Loop through each thread
    for (i = 0; i < threadCount; i++) {
        // Allocate memory for the Cordinate struct
        Cordinate *entry = malloc(sizeof(Cordinate));

        // Set the row and column coordinates in the Cordinate struct
        entry->row = i / matrixSize;
        entry->column = i % matrixSize;

        // Create a thread to compute the sum of the matrices using the Cordinate struct as the argument
        pthread_create(&threadsSum[i], NULL, &computeSum, (void *)(entry));

        // Create a thread to compute the difference of the matrices using the Cordinate struct as the argument
        pthread_create(&threadsDiff[i], NULL, &computeDiff, (void *)(entry));

        // Create a thread to compute the inner product of the matrices using the Cordinate struct as the argument
        pthread_create(&threadsProduct[i], NULL, &computeProduct, (void *)(entry));
    }

    // Wait for all threads to finish for each operation
    for (i = 0; i < threadCount; i++) {
        pthread_join(threadsSum[i], NULL);
        pthread_join(threadsDiff[i], NULL);
        pthread_join(threadsProduct[i], NULL);
    }

    
    // 6. Print the results.
    printf("Results:\n");
    printf("Sum:\n");
    printMatrix(matSumResult);
    printf("Difference:\n");
    printMatrix(matDiffResult);
    printf("Product:\n");
    printMatrix(matProductResult);
    return 0;
  
}