/* CSCI2520
*
* Assignment: 4
* Student ID: 1155127101
* Name      : NG Wing Yin
* Email     : 1155127101@link.cuhk.edu.hk
*/

#include <stdio.h>

int main() {
  int K, N;
  printf("Input:\n");
  scanf("%d %d", &N, &K);
  int strong[N], weak[N];

  for (int n1 = 0; n1 < N; n1++) {
    scanf("%d", &strong[n1]);
  };
  for (int n2 = 0; n2 < N; n2++) {
    scanf("%d", &weak[n2]);
  }

  int order[N];
  for (int i = 0; i < N; i++) {
    int p = i;
    order[i] = i;
    while (p > 0) {
      if (weak[order[p]] > weak[order[p - 1]] || (weak[order[p]] == weak[order[p - 1]] && strong[order[p]] > strong[order[p - 1]])) {
        int temp = order[p];
        order[p] = order[p - 1];
        order[p - 1] = temp;
        p--;
      } else {
        break;
      }
    }
  }

  int power = 0;
  for (int j = 0; j <= N - K; j++) {
    int sum = 0, min = weak[order[j]];
    for (int k = 0; k < K; k++) {
      int o = order[j + k];
      sum += strong[o];
      min = weak[o] < min ? weak[o] : min;
    }
    power = sum * min > power ? sum * min : power;
  }
  printf("Output:\n%d", power);

  return 0;
}