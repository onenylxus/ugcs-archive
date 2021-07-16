/* CSCI2520
*
* Assignment: 3
* Student ID: 1155127101
* Name      : NG Wing Yin
* Email     : 1155127101@link.cuhk.edu.hk
*/

#include <stdio.h>

typedef struct bstCDT bst;
struct bstCDT {
  int val;
  bst* left;
  bst* right;
  int height;
};

// Display functions
// They do not affect the tree.
void Display(bst *tree) {
  printf("Preorder: ");
  Preorder(tree);
  printf("\nInorder: ");
  Inorder(tree);
  printf("\nPostorder: ");
  Postorder(tree);
  printf("\n\n");
}

void Preorder(bst *tree) {
  if (tree) {
    printf("%d ", tree->val);
    Preorder(tree->left);
    Preorder(tree->right);
  }
}

void Inorder(bst *tree) {
  if (tree) {
    Inorder(tree->left);
    printf("%d ", tree->val);
    Inorder(tree->right);
  }
}

void Postorder(bst *tree) {
  if (tree) {
    Postorder(tree->left);
    Postorder(tree->right);
    printf("%d ", tree->val);
  }
}

// This is just an example.
// The real test cases will be more complex.
// You also can change this function to test your code.
bst* TreeGenerator() {
  bst* tree = (bst*)malloc(sizeof(bst));
  tree->val = 3;
  tree->height = 2;
  tree->right = (bst*)malloc(sizeof(bst));
  tree->right->val = 4;
  tree->height = 0;
  tree->right->right = NULL;
  tree->right->left = NULL;
  tree->left = (bst*)malloc(sizeof(bst));
  tree->left->val = 2;
  tree->left->height = 1;
  tree->left->right = NULL;
  tree->left->left = (bst*)malloc(sizeof(bst));
  tree->left->left->val = 1;
  tree->left->left->height = 0;
  tree->left->left->right = NULL;
  tree->left->left->left = NULL;
  return tree;
}

bst* Rebuild(bst* tree);
int Size(bst* tree);
int Collect(bst* tree, int* arr, int i);
int* Sort(int* arr, int size);
bst* Create(int* arr, int start, int end);
int Height(bst* tree);

int main() {
  bst* tree = TreeGenerator();
  Display(tree);
  tree = Rebuild(tree);
  Display(tree);
  return 0;
}

// Please finish this function implementation.
bst* Rebuild(bst* tree) {
  int* arr = (int*)malloc(sizeof(int) * Size(tree));
  Collect(tree, arr, 0);
  arr = Sort(arr, Size(tree));
  bst *out = Create(arr, 0, Size(tree) - 1);
  return out;
}

int Size(bst* tree) {
  if (!tree) { return 0; }
  return 1 + Size(tree->left) + Size(tree->right);
}

int Collect(bst* tree, int* arr, int i) {
  arr[i++] = tree->val;
  if (tree->left) {
    i = Collect(tree->left, arr, i);
  }
  if (tree->right) {
    i = Collect(tree->right, arr, i);
  }
  return i;
}

int* Sort(int* arr, int size) {
  int temp;
  for (int i = 0; i < size; i++) {
    for (int j = i + 1; j < size; j++) {
      if (arr[j] < arr[i]) {
        temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
      }
    }
  }
  return arr;
}

bst* Create(int* arr, int start, int end) {
  if (start <= end) {
    int mid = (start + end) / 2;
    bst* tree = (bst*)malloc(sizeof(bst));

    tree->val = arr[mid];
    tree->left = Create(arr, start, mid - 1);
    tree->right = Create(arr, mid + 1, end);
    tree->height = Height(tree);

    return tree;
  }
  return NULL;
}

int Height(bst* tree) {
  if (!tree) { return 0; }
  int l = Height(tree->left);
  int r = Height(tree->right);
  return l > r ? ++l : ++r;
}