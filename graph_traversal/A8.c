#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define INFINITY 1000000000

typedef struct _node {
   int nbr;
   struct _node *next;
} node;

typedef node **graph;

typedef struct {
   int vertex;
   int level;
} qnode;

typedef qnode *queue;

graph gengraph ( int n, int m, int n1, int n2 )
{
   graph G;
   int u, v, e;
   node *p;

   G = (node **)malloc(n * sizeof(node *));
   for (u=0; u<n; ++u) G[u] = NULL;
   e = 0;
   while (e < m) {
      u = rand() % n; v = rand() % n;
      if (u == v) continue;
      if ((u < n1) && (v >= n-n2)) continue;
      if ((v < n1) && (u >= n-n2)) continue;
      p = G[u];
      while (p) {
         if (p -> nbr == v) break;
         p = p -> next;
      }
      if (p) continue;
      p = (node *)malloc(sizeof(node));
      p -> nbr = v;
      p -> next = G[u];
      G[u] = p;
      p = (node *)malloc(sizeof(node));
      p -> nbr = u;
      p -> next = G[v];
      G[v] = p;
      ++e;
      printf("%2d %-2d", u, v);
      if (e % 10) printf("\t"); else printf("\n");
   }
   if (e % 10) printf("\n");
   return G;
}

void prngraph ( graph G, int n )
{
   int u, first;
   node *p;

   for (u=0; u<n; ++u) {
      printf("    %-2d -> ", u);
      p = G[u]; first = 1;
      while (p) {
         if (!first) printf(", ");
         first = 0;
         printf("%2d", p -> nbr);
         p = p -> next;
      }
      printf("\n");
   }
}

void destroygraph ( graph G, int n )
{
   int u;
   node *p, *q;

   for (u=0; u<n; ++u) {
      p = G[u];
      while (p) {
         q = p -> next;
         free(p);
         p = q;
      }
   }
   free(G);
}

int BFS ( graph G, int n, int u, int n2 )
{
   queue Q;
   int F, B;
   int *visited;
   int v, w, l;
   node *p;

   Q = (qnode *)malloc(n * sizeof(qnode));
   Q[0] = (qnode){u,0}; F = B = 0;

   visited = (int *)malloc(n * sizeof(int));
   for (v=0; v<n; ++v) visited[v] = 0;
   visited[u] = 1;

   while (F <= B) {
      v = Q[F].vertex;
      l = Q[F].level;
      ++F;
      if (v >= n-n2) {
         free(Q); free(visited);
         return l;
      }
      p = G[v];
      while (p) {
         w = p -> nbr;
         if (!visited[w]) {
            ++B;
            Q[B].vertex = w;
            Q[B].level = l+1;
            visited[w] = 1;
         }
         p = p -> next;
      }
   }

   free(Q); free(visited);

   return INFINITY;
}

void getdist1 ( graph G, int n, int n1, int n2 )
{
   int u, d, l;

   d = INFINITY;
   for (u=0; u<n1; ++u) {
      l = BFS(G,n,u,n2);
      if (l == INFINITY) printf("    BFS(%d) returns INFINITY\n", u);
      else printf("    BFS(%d) returns %d\n", u, l);
      if (l < d) d = l;
   }
   printf("\n--- d(V1,V2) = ");
   if (d == INFINITY) printf("INFINITY\n"); else printf("%d\n", d);
}

void getdist2 ( graph G, int n, int n1, int n2 )
{
   int u, v, d;
   node *p, *q;
   graph H;
   int *V1nbr, *V2nbr;

   /* Contract G to a graph H as follows.

      All vertices in V1 are contracted to the Vertex 0, and other vertices
      in V1 will stay as isolated vertices. Likewise, all vertices in V2 are
      contracted to the Vertex n-1, and other vertices in V2 will stay as
      isolated vertices. These isolated vertices will play no role in the
      BFS of Method 2. We still keep these to avoid renumbering of vertices.
      We can straightaway call the BFS function on the contracted graph H.

      Let us now see what happens to an edge (u,v) in the original graph G.
      We need to consider several cases.

      Case 1: u,v in V1. This edge is internal to V1. It does not matter
      whether we start the BFS from u or v, so this edge is not added to H.

      Case 2: u,v in V2: It does not matter where the BFS ends in V2, so
      there is no need to add this edge to H.

      Case 3: u,v in V-V1-V2: This edge is not affected by the contraction.
      So this will go as it is to H.

      Case 4: u in V1, v in V-V1-V2: We replace this edge by the representative
      edge (0,v). Multiple vertices in V1 may share edges with the same v. So
      we remember which v share edges with vertices of V1. After all the edges
      of G are scanned, we add the edges (0,v) for all remembered values v.

      Case 5: u in V-V1-V2, v in V2: Now the representative edge is
      (u,n-1). Like Case 4, we add these edges after a complete scan of all
      the edges in G, in order to avoid multiple addition of edges of the
      form (u,n-1) for the same u.

      Case 6: u in V1, v in V2: Add the edge (0,n-1), but after the scan
      of all edges in G is complete.
   */
   H = (node **)malloc(n * sizeof(node *));
   V1nbr = (int *)malloc(n * sizeof(int));    /* Neighbors of V1 */
   V2nbr = (int *)malloc(n * sizeof(int));    /* Neighbors of V2 */
   for (u=0; u<n; ++u) {
      H[u] = NULL;
      V1nbr[u] = V2nbr[u] = 0;
   }

   for (u=0; u<n; ++u) {
      p = G[u];
      while (p) {
         v = p -> nbr;
         p = p -> next;
         if (u > v) continue;
         if (u < n1) {
            if (v >= n-n2) V1nbr[n-1] = 1;               /* Case 6 */
            else if (v >= n1) V1nbr[v] = 1;              /* Case 4 */
         } else if (v >= n-n2) {
            if ((u >= n1) && (u < n-n2)) V2nbr[u] = 1;   /* Case 5 */
         } else {                                        /* Case 3 */
            q = (node *)malloc(sizeof(node));
            q -> nbr = v; q -> next = H[u]; H[u] = q;
            q = (node *)malloc(sizeof(node));
            q -> nbr = u; q -> next = H[v]; H[v] = q;
         }
      }
   }

   /* Add the representative edges from V1 */
   for (u=n1; u<n; ++u) {
      if (V1nbr[u]) {
         q = (node *)malloc(sizeof(node));
         q -> nbr = u; q -> next = H[0]; H[0] = q;
         q = (node *)malloc(sizeof(node));
         q -> nbr = 0; q -> next = H[u]; H[u] = q;
      }
   }
   free(V1nbr);

   /* Add the representative edges from V2 */
   for (u=n1; u<n-n2; ++u) {
      if (V2nbr[u]) {
         q = (node *)malloc(sizeof(node));
         q -> nbr = u; q -> next = H[n-1]; H[n-1] = q;
         q = (node *)malloc(sizeof(node));
         q -> nbr = n-1; q -> next = H[u]; H[u] = q;
      }
   }
   free(V2nbr);

   printf("\n--- The contracted graph\n"); prngraph(H,n);

   d = BFS(H,n,0,n2);
   printf("\n--- d(V1,V2) = %d\n", d);

   destroygraph(H,n);
}

int main ( int argc, char *argv[] )
{
   int n, m, n1, n2;
   graph G;

   if (argc >= 5) {
      n = atoi(argv[1]);
      m = atoi(argv[2]);
      n1 = atoi(argv[3]);
      n2 = atoi(argv[4]);
   } else {
      scanf("%d%d%d%d", &n, &m, &n1, &n2);
   }
   printf("n = %d\nm = %d\nn1 = %d\nn2 = %d\n", n, m, n1, n2);
   srand((unsigned int)time(NULL));

   G = gengraph(n,m,n1,n2);
   printf("\n+++ The constructed graph\n"); prngraph(G,n);

   printf("\n+++ Method 1\n\n");
   getdist1(G,n,n1,n2);

   printf("\n+++ Method 2\n");
   getdist2(G,n,n1,n2);

   destroygraph(G,n);

   exit(0);
}