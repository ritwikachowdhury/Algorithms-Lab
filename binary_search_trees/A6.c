#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MINUS_INFINITY -1000000000
#define PLUS_INFINITY 1000000000

typedef struct _node {
   int key;
   struct _node *L;
   struct _node *R;
} BSTnode;

typedef BSTnode *BST;

/*** The following few functions are not part of the assignment. These
     are implemented in order to generate valid inputs to buildBST.
 ***/

BST insert ( BST T, int x, int *size )
{
   BSTnode *p, *q;

   p = T; q = NULL;
   while (p) {
      if (p -> key == x) return T;
      q = p;
      if (x < p -> key) p = p -> L; else p = p -> R;
   }
   ++(*size);
   p = (BSTnode *)malloc(sizeof(BSTnode));
   p -> key = x; p -> L = p -> R = NULL;
   if (q == NULL) return p;
   if (x < q -> key) q -> L = p; else q -> R = p;
   return T;
}

BST buildOrigBST ( int n )
{
   int size = 0;
   BST T = NULL;

   while (size < n) T = insert(T, 100 + rand() % 900, &size);
   return T;
}

void storePreorder ( BST T, int *A, int *i )
{
   if (T == NULL) return;
   A[(*i)++] = T -> key;
   storePreorder(T -> L, A, i);
   storePreorder(T -> R, A, i);
}

void destroyTree ( BST T )
{
   if (T == NULL) return;
   destroyTree(T -> L);
   destroyTree(T -> R);
   free(T);
}

/*** End of the extra functions ***/

/*** This is the O(n log n)-time reconstruction of the tree from the
     preorder listing, as given in the assignment statement. The space
     requirement is O(h(T)). But h(T) can be Theta(n), so the worst-case
     space requirement is O(n).

     The function is commented, because a linear-time function is used
     for the purpose of reconstruction.
 ***/

/***
BST buildBST ( int A[], int n )
{
   BST T;
   int l, r, m;

   if (n == 0) return NULL;
   T = (BSTnode *)malloc(sizeof(BSTnode));
   T -> key = A[0];
   if (n == 1) {
      l = r = 0;
   } else {
      if (A[1] > A[0]) {
         l = 0; r = n - 1;
      } else if (A[n-1] < A[0]) {
         l = n - 1; r = 0;
      } else {
         l = 1; r = n - 1;
         while (r != l + 1) {
            m = (l + r) / 2;
            if (A[m] < A[0]) l = m; else r = m;
         }
         r = n - l - 1;
      }
   }
   T -> L = buildBST(A+1,l);
   T -> R = buildBST(A+l+1,r);
   return T;
}
 ***/

/*** This is an O(n)-time and O(n)-space reconstruction algorithm. It
     simulates the preorder traversal while creating the tree itself.

     The space requirement can be reduced to O(h(T)) by implementing
     the stacks as doubly linked lists. But since h(T) can be Theta(n),
     this leads to no theoretical improvements in the worst-case space
     complexity.
 ***/
BST buildBST ( int A[], int n )
{
   BST *S;    /* Pointer stack simulating the recursion stack during preorder traversal */
   BST *LL;   /* Another stack storing the last left turn (the parent) before reaching a node */
   int top;   /* Index of the top in the two stacks */
   BST T;     /* The root pointer to be returned */
   BST p, q;  /* Running pointers */
   int i;

   if (n == 0) return NULL;

   S = (BST *)malloc(n * sizeof(BST));
   LL = (BST *)malloc(n * sizeof(BST));

   /* Create the root node */
   T = (BST)malloc(sizeof(BSTnode));
   T -> key = A[0]; T -> L = T -> R = NULL;

   /* Initialize S and LL to one-element stacks */
   top = 0;
   S[top] = T;      /* the freshly created root */
   LL[top] = NULL;  /* no left turn made so far */

   /* A child of p is created in each iteration of the following loop */
   p = T;

   /* Loop for creating the nodes storing A[1], A[2], ..., A[n-1] */
   for (i=1; i<n; ++i) {
      if (A[i] < p -> key) {
         /* The key must be stored in the left child of p */
         p -> L = (BST)malloc(sizeof(BSTnode));    /* Allocate memory */
         ++top; LL[top] = p;                       /* Case of a left turn */
         p = S[top] = p -> L;                      /* Go to the created node */
         p -> key = A[i]; p -> L = p -> R = NULL;  /* Populate the new node */
      } else {
         /* The key is to be stored in the right child of a node, but of
            which node? The search for this appropriate node v is based
            upon the following observations.
               (a) the right child of v must be NULL at this point
               (b) the new key A[i] must be larger than v -> key
               (c) v is the most distant from the current node p, among all
                   the nodes on the root-to-p path, which satisfy (a) and (b) */
         while (1) {
            /* A right turn violates (a), so we keep on looking at left
               turns made so far. We keep on repeating until (b) fails
               or no previous left turn exists. */
            q = LL[top];
            if ((q == NULL) || (A[i] < q -> key)) break;
            while (S[top] != q) --top;
         }
         /* Now the appropriate node is found, whose right child will
            store A[i] */
         p = S[top];
         p -> R = (BST)malloc(sizeof(BSTnode));    /* Allocate memory */
         ++top; p = S[top] = p -> R;               /* Go to the new right child */
         LL[top] = LL[top-1];                      /* This is a case of right turn, so copy the previous left turn */
         p -> key = A[i]; p -> L = p -> R = NULL;  /* Populate the new node */
      }
   }

   free(S); free(LL);

   return T;
}

int isBST ( BST T, int l, int r )
{
   int childIsBST;

   if (T == NULL) return 1;
   if ((T -> key < l) || (T -> key > r)) return 0;
   childIsBST = isBST(T -> L, l, T -> key);
   if (childIsBST == 0) return 0;
   childIsBST = isBST(T -> R, T -> key, r);
   return childIsBST;
}

void preorder ( BST T, int *i )
{
   if (T == NULL) return;
   if (*i == 0) printf("   ");
   printf(" %d", T -> key);
   ++(*i);
   if (*i == 19) {
      printf("\n");
      *i = 0;
   }
   preorder(T -> L, i);
   preorder(T -> R, i);
}

BSTnode *inorderSuccessor ( BST T, int x )
{
   BSTnode *p, *q;

   q = NULL;
   p = T;
   while (p) {
      if (p -> key == x) break;
      if (x < p -> key) {
         q = p;
         p = p -> L;
      } else {
         p = p -> R;
      }
   }
   if (p == NULL) return NULL;
   if (p -> R) {
      q = p -> R;
      while (q -> L != NULL) q = q -> L;
   }
   return q;
}

/* Let x be found at a node pointed to by p. If p has a left child, the
   successor is stored at the left child. Otherwise if p has a right child,
   the successor is stored at that right child. Finally, if p is a leaf
   node, the successor is stored in the node corresponding to the last
   left turn with a non-empty right subtree. */
BSTnode *preorderSuccessor ( BST T, int x )
{
   BSTnode *p, *q;

   q = NULL;
   p = T;
   while (p) {
      if (p -> key == x) break;
      if (x < p -> key) {
         if (p -> R) q = p -> R;
         p = p -> L;
      } else {
         p = p -> R;
      }
   }
   if (p == NULL) return NULL;
   if (p -> L) q = p -> L;
   else if (p -> R) q = p -> R;
   return q;
}

int main ( int argc, char *argv[] )
{
   int n, i, x;
   BST T, p;
   int *A;

   if (argc > 1) n = atoi(argv[1]); else scanf("%d", &n);
   printf("    n = %d\n\n", n);
   srand((unsigned int)time(NULL));

   T = buildOrigBST(n);
   A = (int *)malloc(n * sizeof(int));
   i = 0; storePreorder(T,A,&i);
   printf("+++ Array storing preorder listing of keys\n");
   for (i=0; i<n; ++i) {
      if (i % 19 == 0) printf("   ");
      printf(" %d", A[i]);
      if (i % 19 == 18) printf("\n");
   }
   if (i % 19) printf("\n");
   destroyTree(T);
   T = NULL;

   T = buildBST(A,n);

   printf("\n+++ Preorder listing of the keys in the constructed tree\n");
   i = 0; preorder(T, &i);
   if (i % 19) printf("\n");

   if (isBST(T, MINUS_INFINITY, PLUS_INFINITY))
      printf("\n+++ The constructed tree is a BST...\n\n");
   else
      printf("\n+++ The constructed tree is NOT a BST...\n\n");

   x = A[rand() % n]; p = inorderSuccessor(T, x);
   if (p == NULL)
      printf("    Inorder successor of %d does not exist\n", x);
   else
      printf("    Inorder successor of %d is %d\n", x, p -> key);

   p = T;
   while (p -> R) {
      if ((p -> L) && (rand() % 2)) p = p -> L;
      else p = p -> R;
   }
   x = p -> key; p = inorderSuccessor(T,x);
   if (p == NULL)
      printf("    Inorder successor of %d does not exist\n", x);
   else
      printf("    Inorder successor of %d is %d\n", x, p -> key);

   p = T;
   while (p -> R) p = p -> R;
   x = p -> key; p = inorderSuccessor(T,x);
   if (p == NULL)
      printf("    Inorder successor of %d does not exist\n\n", x);
   else
      printf("    Inorder successor of %d is %d\n\n", x, p -> key);

   x = A[rand() % n]; p = preorderSuccessor(T, x);
   if (p == NULL)
      printf("    Preorder successor of %d does not exist\n", x);
   else
      printf("    Preorder successor of %d is %d\n", x, p -> key);

   p = T;
   while ((p -> L) || (p -> R)) {
      if (p -> R == NULL) p = p -> L;
      else if (p -> L == NULL) p = p -> R;
      else p = (rand() % 2) ? p -> L : p -> R;
   }
   x = p -> key; p = preorderSuccessor(T, x);
   if (p == NULL)
      printf("    Preorder successor of %d does not exist\n", x);
   else
      printf("    Preorder successor of %d is %d\n", x, p -> key);

   x = A[n-1]; p = preorderSuccessor(T, x);
   if (p == NULL)
      printf("    Preorder successor of %d does not exist\n", x);
   else
      printf("    Preorder successor of %d is %d\n", x, p -> key);

   exit(0);
}