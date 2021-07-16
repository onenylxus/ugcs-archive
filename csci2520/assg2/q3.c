/* CSCI2520
*
* Assignment: 2
* Student ID: 1155127101
* Name      : NG Wing Yin
* Email     : 1155127101@link.cuhk.edu.hk
*/

#include <stdio.h>
#include <stdlib.h>

void Read(char A[][5], int N) {
  printf("Input all strings: ");
  for (int i = 0; i < N; i++) {
    scanf("%s", A[i]);
    A[i][4] = '\0';
  }
}

void PrintA(char A[][5], int N) {
  for (int i = 0; i < N; i++) {
    printf("%s ", A[i]);
  }
  printf("\n");
}

void PrintA_in_sort(char A[][5], int N, int round) {
  printf("After Round-%d: ", round);
  for (int i = 0; i < N; i++) {
    printf("%s ", A[i]);
  }
  printf("\n");
}

void Selection_sort(char A[][5], int N);

int main(){
  int N;
  printf("Enter how many strings: ");
  scanf("%d", &N);
  char A[N][5]; // A[i][4] stores '\0' to be the end of one string.

  Read(A, N);
  printf("Initial Array: ");
  PrintA(A, N);

  Selection_sort(A, N);
  return 0;
}

// Please finish this function implementation.
void Selection_sort(char A[][5], int N) {
  int best, place;

  for (int start = 0; start < N - 1; start++) {
    best = start;
    for (int i = start; i < N; i++) {
      place = 0;
      while (place >= 0 && place < 5) {
        int a = A[i][place];
        int b = A[best][place];
        if (a < b) {
          best = i;
          place = -1;
          break;
        } else if (a > b) {
          place = -1;
        } else {
          place++;
        }
      }
    }

    for (int j = 0; j < 5; j++) {
      char temp = A[start][j];
      A[start][j] = A[best][j];
      A[best][j] = temp;
    }

    PrintA_in_sort(A, N, start);
  }
}
