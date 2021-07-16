/* CSCI2520
*
* Assignment: 4
* Student ID: 1155127101
* Name      : NG Wing Yin
* Email     : 1155127101@link.cuhk.edu.hk
*/

#include <stdio.h>

int main() {
  int M, N;
  printf("Input:\n");
  scanf("%d %d", &N, &M);
  int weight[N][N];

  for (int a1 = 0; a1 < N; a1++) {
    for (int b1 = 0; b1 < N; b1++) {
      weight[a1][b1] = -1;
    }
  }

  int W = 0;
  for (int m = 0; m < M; m++) {
    int x, y, w;
    scanf("%d %d %d", &x, &y, &w);
    weight[x - 1][y - 1] = w;
    W = w > W ? w : W;
  }
  W++;

  for (int a2 = 0; a2 < N; a2++) {
    for (int b2 = 0; b2 < N; b2++) {
      if (a2 == b2) {
        weight[a2][b2] = 0;
      } else if (weight[a2][b2] < 0) {
        weight[a2][b2] = W;
      }
    }
  }

  for (int t = 0; t < N; t++) {
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        if (weight[i][t] + weight[t][j] < weight[i][j]) {
          weight[i][j] = weight[i][t] + weight[t][j];
        }
      }
    }
  }

  int d = 0;
  for (int u = 0; u < N; u++) {
    for (int v = u + 1; v < N; v++) {
      d = weight[u][v] > d ? weight[u][v] : d;
    }
  }
  printf("Output:\n%d", d);

  return 0;
}