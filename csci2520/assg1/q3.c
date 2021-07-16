/* CSCI2520
*
* Assignment: 1
* Student ID: 1155127101
* Name      : NG Wing Yin
* Email     : 1155127101@link.cuhk.edu.hk
*/

#include <stdio.h>
#include <stdlib.h>

typedef char Element;
typedef struct StackStruct *Stack;
struct StackStruct {
  Element elements[80];
  int count;
};

Stack NewStack() {
  Stack stack = (Stack)malloc(sizeof(struct StackStruct));
  stack->count = 0;
  return stack;
}

void Push(Stack stack, Element element) {
  stack->elements[(stack->count)++] = element;
}

Element Pop(Stack stack) {
  return stack->elements[--(stack->count)];
}

int Count(Stack stack) {
  return stack->count;
}

int main() {
  Stack CharStack = NewStack();
  char string[80];

  printf("input: ");
  scanf("%s", &string);
  while (string[Count(CharStack)] == 'T' || string[Count(CharStack)] == '(' || string[Count(CharStack)] == ')') {
    Push(CharStack, string[Count(CharStack)]);
  }

  int t = 0;
  int v = 0;
  while (Count(CharStack) > 0) {
    char p = Pop(CharStack);
    if (p == 'T') {
      t++;
    } else if (p == '(' && t > 0) {
      v++;
    } else if (p == ')' && t > 0) {
      v--;
    }
  }
  printf("output: %d", v);

  return 0;
}
