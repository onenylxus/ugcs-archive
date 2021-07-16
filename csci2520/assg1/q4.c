/* CSCI2520
*
* Assignment: 1
* Student ID: 1155127101
* Name      : NG Wing Yin
* Email     : 1155127101@link.cuhk.edu.hk
*/

#include <stdio.h>
#include <stdlib.h>

typedef int ElementType;
typedef struct Node *PtrToNode;
typedef PtrToNode List;
typedef PtrToNode Position;
struct Node {
  ElementType Element;
  Position Next;
};

List Read() {
  List L = (List)malloc(sizeof(struct Node));
  List ptr = L;
  int num = 0;
  scanf("%d", &num);
  ElementType value;
  for (int i = 0; i < num; i++) {
    scanf("%d", &value);
    List tmp = (List)malloc(sizeof(struct Node));
    tmp->Element = value;
    ptr->Next = tmp;
    ptr = ptr->Next;
  }
  ptr->Next = NULL;
  return L;
}

void Print(List L) {
  List ptr = L;
  while (ptr->Next != NULL) {
    printf("%d ", ptr->Next->Element);
    ptr = ptr->Next;
  }
  printf("\n");
}

List Reverse(List L);

int main() {
  List L1, L2;
  L1 = Read();
  L2 = Reverse(L1);
  Print(L1);
  Print(L2);
  return 0;
}

/* Your function will be put here */
List Reverse(List L) {
  List L1 = L;
  List C = (List)malloc(sizeof(struct Node));
  List tmp = NULL;

  while (L1 != NULL) {
    if (tmp == NULL) {
      C->Element = L1->Element;
      C->Next = NULL;
      tmp = C;
    } else {
      tmp->Next = (List)malloc(sizeof(struct Node));
      tmp = tmp->Next;
      tmp->Element = L1->Element;
      tmp->Next = NULL;
    }
    L1 = L1->Next;
  }

  List L2 = (List)malloc(sizeof(struct Node));
  List ptr = C->Next;
  List prev = NULL;

  while (ptr != NULL) {
    List next = ptr->Next;
    ptr->Next = prev;
    if (next == NULL) {
      L2->Next = ptr;
    }
    prev = ptr;
    ptr = next;
  }

  return L2;
}
