#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define SIZE 9

// Sudoku board. Modify this to test your checker!
// 0 == empty cell; 1-9 is the filled in digit.
int board[SIZE][SIZE] = {
    {1,0,0,0,0,0,0,0,0},
    {0,2,0,0,0,0,0,0,0},
    {0,0,3,0,0,0,0,0,0},
    {0,0,0,4,0,0,0,0,0},
    {0,0,0,0,5,0,0,0,0},
    {0,0,0,0,0,6,0,0,0},
    {0,0,0,0,0,0,7,0,0},
    {0,0,0,0,0,0,0,8,0},
    {0,0,0,0,0,0,0,0,9},
};

typedef struct {
    int index;
} parameters;

bool row_check[SIZE];
bool col_check[SIZE];
bool box_check[SIZE];

void printBoard(int board[SIZE][SIZE]) {
    for(int i = 0; i<SIZE; i++) {
        for(int j = 0; j<SIZE; j++) {
            printf("%5d", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Checks the given row for duplicate numbers, and updates the row_check
// value for that row appropriately. If no number is repeated in that row,
// row_check[row] will be set to true; otherwise, it will be false.
void* checkRow(void* args) {
    parameters* param = (parameters*)args;
    int row = param->index;
    bool nums[SIZE + 1] = {false};

    for (int j = 0; j < SIZE; j++) {
        int num = board[row][j];
        if (num != 0) {
            if (nums[num]) {
                row_check[row] = false;
                return NULL;
            }
            nums[num] = true;
        }
    }
    row_check[row] = true;
    return NULL;
}

// Checks the given col for duplicate numbers, and updates the col_check
// value for that col appropriately. If no number is repeated in that col,
// col_check[col] will be set to true; otherwise, it will be false.
void* checkCol(void* args) {
    parameters* param = (parameters*)args;
    int col = param->index;
    bool nums[SIZE + 1] = {false};

    for (int i = 0; i < SIZE; i++) {
        int num = board[i][col];
        if (num != 0) {
            if (nums[num]) {
                col_check[col] = false;
                return NULL;
            }
            nums[num] = true;
        }
    }
    col_check[col] = true;
    return NULL;
}

// Checks the given 3x3 box for duplicate numbers, and updates the box_check
// value for that box appropriately. If no number is repeated in that box,
// box_check[box] will be set to true; otherwise, it will be false.
void* checkBox(void* args) {
    parameters* param = (parameters*)args;
    int box = param->index;
    bool nums[SIZE + 1] = {false};

    int boxSize = (int)sqrt(SIZE);
    int startRow = (box / boxSize) * boxSize;
    int startCol = (box % boxSize) * boxSize;

    for (int i = startRow; i < startRow + boxSize; i++) {
        for (int j = startCol; j < startCol + boxSize; j++) {
            int num = board[i][j];
            if (num != 0) {
                if (nums[num]) {
                    box_check[box] = false;
                    return NULL;
                }
                nums[num] = true;
            }
        }
    }
    box_check[box] = true;
    return NULL;
}

// Spawn a thread to fill each cell in each result matrix.
// How many threads will you spawn?
int main() { 
    // 1. Print the board.
    printf("Board:\n");
    printBoard(board);
    
    // 2. Create pthread_t objects for our threads.
    pthread_t threads[SIZE * 3];
    parameters thread_data[SIZE * 3];
    
    // 3. Create a thread for each cell of each matrix operation.
    for (int i = 0; i < SIZE; i++) {
        thread_data[i].index = i;
        pthread_create(&threads[i], NULL, checkRow, (void*)&thread_data[i]);

        thread_data[i + SIZE].index = i;
        pthread_create(&threads[i + SIZE], NULL, checkCol, (void*)&thread_data[i + SIZE]);

        thread_data[i + SIZE * 2].index = i;
        pthread_create(&threads[i + SIZE * 2], NULL, checkBox, (void*)&thread_data[i + SIZE * 2]);
    }
    
    // 4. Wait for all threads to finish.
    for (int i = 0; i < SIZE * 3; i++) {
        pthread_join(threads[i], NULL);
    }
    
    // 5. Print the results.
    printf("Results:\n");
    bool all_rows_passed = true;
    printf("Rows:\n");
    for (int i = 0; i < SIZE; i++) {
        if (!row_check[i]) {
            printf("Row %i did not pass\n", i);
            all_rows_passed = false;
        }
    }
    if (all_rows_passed) {
        printf("All rows passed!\n");
    }
    
    bool all_cols_passed = true;
    printf("Cols:\n");
    for (int i = 0; i < SIZE; i++) {
        if (!col_check[i]) {
            printf("Col %i did not pass\n", i);
            all_cols_passed = false;
        }
    }
    if (all_cols_passed) {
        printf("All cols passed!\n");
    }
    
    bool all_boxes_passed = true;
    printf("Boxes:\n");
    for (int i = 0; i < SIZE; i++) {
        if (!box_check[i]) {
            printf("Box %i did not pass\n", i);
            all_boxes_passed = false;
        }
    }
    if (all_boxes_passed) {
        printf("All boxes passed!\n");
    }
    return 0;
}

