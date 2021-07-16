/* CSCI2520
*
* Assignment: 2
* Student ID: 1155127101
* Name      : NG Wing Yin
* Email     : 1155127101@link.cuhk.edu.hk
*/

#include <stdio.h>
#include <stdlib.h>

void Read(int* num, int N) {
  printf("Input all strings: ");
  for (int i = 0; i < N; i++) {
    scanf("%d", &num[i]);
  }
}

void PrintNum(int* num, int N, int cnt) {
    printf("Final result: ");
    for (int i = 0; i < N; i++) {
        printf("%d ", num[i]);
    }
    printf("\nCalled times: %d\n", cnt);
}

int quickSort(int* num, int start, int end);

int main() {
  int N;
  printf("Enter how many strings: ");
  scanf("%d", &N);
  int *num = (int*)malloc(sizeof(int) * N);

  Read(num, N);
  int call_cnt = quickSort(num, 0, N - 1);
  PrintNum(num, N, call_cnt);

  return 0;
}

// Please finish this function implementation.
int quickSort(int* num, int start, int end) {
  int pivot = num[start];
  int part = start;
  int temp;

  if (start < end) {
    for (int i = start + 1; i <= end; i++) {
      if (num[i] < pivot) {
        temp = num[++part];
        num[part] = num[i];
        num[i] = temp;
      }
    }

    temp = num[part];
    num[part] = num[start];
    num[start] = temp;

    int l = quickSort(num, start, part - 1);
    int r = quickSort(num, part + 1, end);
    return 1 + l + r;
  }

  return 1;
}
