#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct _tnode {
   struct _tnode *L;
   struct _tnode *R;
   struct _tnode *P;
} tnode;

typedef tnode *bintree;

/* I do not have a teacher to give me valid ternary or binary encodings.
   So I have this constructor to generate big examples. */
bintree gentree ( int n, bintree P )
{
   bintree T;
   int nl, nr;

   nl = rand() % n; nr = (n-1) - nl;
   T = (bintree)malloc(sizeof(tnode));
   T -> P = P;
   T -> L = (nl == 0) ? NULL: gentree(nl,T);
   T -> R = (nr == 0) ? NULL: gentree(nr,T);
   return T;
}

void printtree ( bintree T, int level )
{
   int i;

   if (T == NULL) { printf("    NULL\n"); return; }

   if (T -> P == NULL) printf("    X\n");

   for (i=0; i<=level; ++i) printf("    ");
   printf("+---");
   if (T -> L == NULL) printf("|\n");
   else {
      printf("X\n");
      printtree(T->L,level+1);
   }

   for (i=0; i<=level; ++i) printf("    ");
   printf("+---");
   if (T -> R == NULL) printf("|\n");
   else {
      printf("X\n");
      printtree(T->R,level+1);
   }
}

void destroytree ( bintree T )
{
   if (T == NULL) return;
   destroytree(T->L);
   destroytree(T->R);
   free(T);
}

char *genenc1 ( bintree T, int n )
{
   char *E, *F;
   int i, j;

   if (T == NULL) return NULL;

   E = (char *)malloc((2*n+1)*sizeof(char));
   i = 0;
   if (T -> L) {
      E[i++] = '0';                 /* Follow the left child pointer */
      F = genenc1(T->L,n);          /* Recursively get the encoding of the left subtree */
      while ((E[i] = F[i-1])) ++i;  /* Append the encoding */
      free(F);
      E[i++] = '2';                 /* Follow the parent pointer */
   }
   j = i+1;                         /* The appending position for the right substring */
   if (T -> R) {
      E[i++] = '1';                 /* Follow the right child pointer */
      F = genenc1(T->R,n);          /* Recursively get the encoding of the right subtree */
      while ((E[i] = F[i-j])) ++i;  /* Append the encoding */
      free(F);
      E[i++] = '2';                 /* Follow the parent pointer */
   }
   E[i] = '\0';

   return E;
}

/* This is almost the same as gennenc1 */
char *genenc2 ( bintree T, int n )
{
   char *E, *F;
   int i, j;

   if (T == NULL) return NULL;

   E = (char *)malloc((2*n+1)*sizeof(char));
   i = 0;
   if (T -> L) {
      E[i++] = '0';
      F = genenc2(T->L,n);
      while ((E[i] = F[i-1])) ++i;
      free(F);
      E[i++] = '0';  /* Following a link from left child to parent */
   }
   j = i+1;
   if (T -> R) {
      E[i++] = '1';
      F = genenc2(T->R,n);
      while ((E[i] = F[i-j])) ++i;
      free(F);
      E[i++] = '1';  /* Following a link from right child to parent */
   }
   E[i] = '\0';

   return E;
}

bintree buildtree1 ( char *E, int n )
{
   bintree T;
   tnode *p;
   int i;

   if (n == 0) return NULL;

   /* First create the root node */
   T = (bintree)malloc(sizeof(tnode));
   T -> P = T -> L = T -> R = NULL;
   p = T; i = 0;

   /* Iteratively build the tree guided by the ternary encoding */
   for (i=0; i<=2*n-3; ++i) {
      if (E[i] == '2') {         /* Go to the parent */
         p = p -> P;
      } else if (E[i] == '0') {  /* Create and move to the left child */
         p -> L = (bintree)malloc(sizeof(tnode));
         p -> L -> P = p;
         p -> L -> L = p -> L -> R = NULL;
         p = p -> L;
      } else if (E[i] == '1') {  /* Create and move to the right child */
         p -> R = (bintree)malloc(sizeof(tnode));
         p -> R -> P = p;
         p -> R -> L = p -> R -> R = NULL;
         p = p -> R;
      } else {
         printf("*** The input ternary encoding is invalid...\n");
      }
   }

   return T;
}

/* This is a helper function that converts a valid binary encoding to a
   valid ternary encoding */
void genmoves ( char *M, int **V, int i, int j )
{
   int l;

   if (i >= j) return;

   l = V[i][j]; /* l is the number of nodes in the left subtree */
   if (l < 0) {
      printf("*** The input binary encoding is invalid...\n");
   } else if (l == 0) {        /* The entire encoding comes from the right subtree */
      M[i] = '1'; M[j] = '2';
      genmoves(M,V,i+1,j-1);
   } else if (l == j-i+1) {    /* The entire encoding comes from the left subtree */
      M[i] = '0'; M[j] = '2';
      genmoves(M,V,i+1,j-1);
   } else {
      genmoves(M,V,i,i+l-1);   /* Convert the string from binary to ternary for the left subtree */
      genmoves(M,V,i+l,j);     /* Convert the string from binary to ternary for the right subtree */
   }
}

/* This is a DP solution that tries to "parse" the input binary string */
bintree buildtree2 ( char *E, int n )
{
   bintree T;
   char *M;
   int i, j, k;
   int **V;

   if (E[0] == '\0') return NULL;

   /* The input $E$ is a binary string of length 2n-2 */

   /* We build a 2-D DP table V. For 0 <= i <= j <= 2n-3, V[i][j] would
      store whether E[i...j] corresponds to a valid encoding of a binary
      tree. If E[i...j] is invalid, -1 is stored. Otherwise, E[i...j]
      stores the number of nodes in the left subtree. */

   V = (int **)malloc((2*n-2)*sizeof(int *));
   for (i=0; i<=2*n-3; ++i) {
      V[i] = (int *)malloc((2*n-2)*sizeof(int));
      /* Initialize every entry to invalid */
      for (j=0; j<=2*n-3; ++j) V[i][j] = -1;
   }

   /* Now, consider the case j=i+1, so the subtree contains onlt two nodes */
   for (i=0; i<=2*n-4; ++i) {
      if ((E[i] == '0') && (E[i+1] == '0'))
         /* This is a root with only one left child */
         V[i][i+1] = 2;
      else if ((E[i] == '1') && (E[i+1] == '1'))
         /* This is a root with only one right child */
         V[i][i+1] = 0;
   }

   /* Now build up the DP table for E[i...i+k] in the increasing sequence
      of k (subtree size - 1). Since any valid tree contains an even number
      of 0'a nd 1's in its binary encoding, we restrict only to odd values
      of k. Notice that the case k=1 is already handled above. */
   for (k=3; k<=2*n-3; k+=2) {
      for (i=0; i<=2*n-3-k; ++i) {
         /* If the root has only the left child, then the first and
            last entries are 0, and the intermediate symbols must
            correspond to the valid encoding of the left subtree. */
         if ((E[i] == '0') && (E[i+k] == '0') && (V[i+1][i+k-1] >= 0))
            V[i][i+k] = k+1;

         /* If the root has only the right child, then the first and
            last entries are 1, and the intermediate symbols must
            correspond to the valid encoding of the right subtree. */
         if ((E[i] == '1') && (E[i+k] == '1') && (V[i+1][i+k-1] >= 0))
            V[i][i+k] = 0;

         /* Both the subtrees are non-empty, so search for a valid
            combination of breaking the sequence into the encoding
            for the left subtree followed by the encoding of the
            right subtree. */
         for (j=1; j<=k-2; j+=2) {
            if ((V[i][i+j] >= 0) && (V[i+j+1][i+k] >= 0) &&  /* Encodings of both subtrees are valid */
                (E[i] == '0') && (E[i+j] == '0') &&          /* This encodes the left subtree */
                (E[i+j+1] == '1') && (E[i+k] == '1')) {      /* This encodes the right subtree */
               if (V[i][i+k] >= 0) {  /* One valid breakup is already discovered */
                  printf("*** Tree is not unique!!! V(%d,%d) = %d, %d\n", i, i+k, V[i][i+k], j+1);
               } else {
                  V[i][i+k] = j+1;
               }
            }
         }
      }
   }

   /* Now, call the helper function to generate the corresponding ternary
      encoding */
   M = (char *)malloc((2*n-1)*sizeof(char));
   M[2*n-2] = '2';
   genmoves(M,V,0,2*n-3);

   /* Once you got the ternary encoding, call the constructor of the tree
      from the ternary encoding */
   T = buildtree1(M,n);

   return T;
}

int main ( int argc, char *argv[] )
{
   int n;
   bintree T;
   char *E;

   if (argc > 1) n = atoi(argv[1]); else scanf("%d",&n);
   printf("n = %d\n", n);
   srand((unsigned int)time(NULL));

   printf("\n********** ENCODING 1 **********\n");
   T = gentree(n,NULL);
   printf("\n+++ The binary tree:\n"); printtree(T,0);

   E = genenc1(T,n);

   destroytree(T); T = NULL;
   printf("\n+++ The binary tree:\n"); printtree(T,0);

   printf("\n+++ Encoding 1 of tree: %s\n", E);

   T = buildtree1(E,n);
   printf("\n+++ The binary tree:\n"); printtree(T,0);
   printf("\n+++ Original encoding : %s\n", E);
   free(E); E = genenc1(T,n);
   printf("+++ Final encoding    : %s\n", E);

   destroytree(T); T = NULL;

   printf("\n********** ENCODING 2 **********\n");
   T = gentree(n,NULL);
   printf("\n+++ The binary tree:\n"); printtree(T,0);

   free(E); E = genenc2(T,n);
   printf("\n+++ Encoding 2 of tree: %s\n", E);

   T = buildtree2(E,n);

   printf("\n+++ The binary tree:\n"); printtree(T,0);
   printf("\n+++ Original encoding : %s\n", E);
   free(E); E = genenc2(T,n);
   printf("+++ Final encoding    : %s\n", E);

   destroytree(T); T = NULL;

   exit(0);
}