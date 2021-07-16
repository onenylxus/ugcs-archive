/* CSCI2520
*
* Assignment: 3
* Student ID: 1155127101
* Name      : NG Wing Yin
* Email     : 1155127101@link.cuhk.edu.hk
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct bstCDT bst;
struct bstCDT {
  int val;
  bst* left;
  bst* right;
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
  tree->right = (bst*)malloc(sizeof(bst));
  tree->right->val = 5;
  tree->right->right = NULL;
  tree->right->left = NULL;
  tree->left = (bst*)malloc(sizeof(bst));
  tree->left->val = 2;
  tree->left->right = NULL;
  tree->left->left = (bst*)malloc(sizeof(bst));
  tree->left->left->val = 1;
  tree->left->left->right = NULL;
  tree->left->left->left = NULL;
  return tree;
}

// This is just an example.
int NumGenerator() {
  return 4;
}

int Find_closest(bst* tree, int num);

int main() {
  bst* tree = TreeGenerator();
  int num = NumGenerator();
  int ans = Find_closest(tree, num);
  // For this example, the ans is 3;
  Display(tree);
  printf("%d\n", ans);
  return 0;
}

// Please finish this function implementation.
int Find_closest(bst* tree, int num)
{
  if (tree->val == num) {
    return tree->val;
  } else if (tree->val > num) {
    if (tree->left && tree->left->val - num <= tree->val - num) {
      return Find_closest(tree->left, num);
    } else {
      return tree->val;
    }
  } else if (tree->val < num) {
    if (tree->right && num - tree->right->val < tree->val - num) {
      return Find_closest(tree->right, num);
    } else {
      return tree->val;
    }
  }
}