/* CSCI2520
*
* Assignment: 3
* Student ID: 1155127101
* Name      : NG Wing Yin
* Email     : 1155127101@link.cuhk.edu.hk
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct btCDT bt;
struct btCDT {
  int val;
  bt* left;
  bt* right;
};

bt* Mirror(bt* tree);

// Display functions
// They do not affect the tree.
void Display(bt* tree) {
  printf("Preorder: ");
  Preorder(tree);
  printf("\nInorder: ");
  Inorder(tree);
  printf("\nPostorder: ");
  Postorder(tree);
  printf("\n\n");
}

void Preorder(bt* tree) {
  if (tree) {
    printf("%d ", tree->val);
    Preorder(tree->left);
    Preorder(tree->right);
  }
}

void Inorder(bt* tree) {
  if (tree) {
    Inorder(tree->left);
    printf("%d ", tree->val);
    Inorder(tree->right);
  }
}

void Postorder(bt* tree) {
  if (tree) {
    Postorder(tree->left);
    Postorder(tree->right);
    printf("%d ", tree->val);
  }
}

// This is just an example.
// The real test cases will be more complex.
// You also can change this function to test your code.
bt* TreeGenerator() {
  bt *tree = (bt *)malloc(sizeof(bt));
  tree->val = 3;
  tree->right = (bt *)malloc(sizeof(bt));
  tree->right->val = 4;
  tree->right->right = NULL;
  tree->right->left = NULL;
  tree->left = (bt *)malloc(sizeof(bt));
  tree->left->val = 2;
  tree->left->right = NULL;
  tree->left->left = (bt *)malloc(sizeof(bt));
  tree->left->left->val = 1;
  tree->left->left->right = NULL;
  tree->left->left->left = NULL;
  return tree;
}

int main() {
  bt* tree = TreeGenerator();
  Display(tree);
  tree = Mirror(tree);
  Display(tree);
  return 0;
}

// Please finish this function implementation.
bt* Mirror(bt* tree) {
  bt *out = (bt *)malloc(sizeof(bt));
  out->val = tree->val;

  if (tree->left) {
    out->right = Mirror(tree->left);
  } else {
    out->right = NULL;
  }

  if (tree->right) {
    out->left = Mirror(tree->right);
  } else {
    out->left = NULL;
  }

  return out;
}