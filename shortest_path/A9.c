#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define INFTY 1000000000

typedef struct _node {
   int vid;
   double ewt;
   struct _node *next;
} node;

typedef struct {
   int vwt;
   node *nbr;
} header;

typedef header *graph;

typedef struct {
   int vid;
   double wt;
} hnode;

typedef hnode *heap;

graph gengraph ( int n, int m )
{
   graph G;
   int i, u, v, e;
   node *p;

   G = (header *)malloc(n * sizeof(header));
   for (i=0; i<n; ++i) G[i].nbr = NULL;
   e = 0;
   while (e < m) {
      u = rand() % n; v = rand() % n;
      if (u == v) continue;
      p = G[u].nbr;
      while (p) {
         if (p -> vid == v) break;
         p = p -> next;
      }
      if (p) continue;
      p = (node *)malloc(sizeof(node));
      p -> vid = v;
      p -> ewt = 0;
      p -> next = G[u].nbr;
      G[u].nbr = p;
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
      p = G[u].nbr; first = 1;
      while (p) {
         if (!first) printf(", ");
         first = 0;
         printf("%2d", p -> vid);
         p = p -> next;
      }
      printf("\n");
   }
}

void readedgewts ( graph G, int n )
{
   int u;
   node *p;

   for (u=0; u<n; ++u) {
      p = G[u].nbr;
      while (p) {
         p -> ewt = (double)(1 + rand() % 10) / (double)10;
         printf("    wt(%2d,%-2d) = %3.1lf\n", u, p -> vid, p -> ewt);
         p = p -> next;
      }
   }
}

void readvertexwts ( graph G, int n )
{
   int u;

   for (u=0; u<n; ++u) {
      G[u].vwt = 5 + rand() % 16;
      printf("    wt(%d) = %d\n", u, G[u].vwt);
   }
}

void initheap ( heap Q, int *L, int *prev, graph G, int n, int s )
{
   int i, j;
   node *p;

   j = 1;
   for (i=0; i<n; ++i) {
      if (i == s) {
         L[i] = -1;
         prev[i] = -1;
         continue;
      }
      Q[j].vid = i;
      Q[j].wt = INFTY;
      L[i] = j;
      prev[i] = s;
      ++j;
   }
   p = G[s].nbr;
   while (p) {
      i = p -> vid;
      j = L[i];
      Q[j].wt = p -> ewt;
      p = p -> next;
   }
}

void heapify ( heap Q, int *L, int size, int i )
{
   int l, r, m;
   int u, v;
   hnode t;

   if (size == 0) return;
   while (1) {
      l = 2*i; r = 2*i + 1;
      if (l > size) break;
      if (r > size) m = size; else m = (Q[l].wt <= Q[r].wt) ? l : r;
      if (Q[i].wt <= Q[m].wt) break;
      u = Q[i].vid; v = Q[m].vid;
      t = Q[i]; Q[i] = Q[m]; Q[m] = t;
      L[u] = m; L[v] = i;
      i = m;
   }
}

void makeheap ( heap Q, int *L, int size )
{
   int i;

   for (i=size/2; i>=1; --i) heapify(Q,L,size,i);
}

void deletemin ( heap Q, int *L, int size )
{
   L[Q[size].vid] = 1;
   L[Q[1].vid] = -1;
   Q[1] = Q[size];
   heapify(Q,L,size-1,1);
}

void changepriority ( heap Q, int *L, int i, double newpriority )
{
   int p;
   hnode t;
   int u, v;

   i = L[i];
   Q[i].wt = newpriority;
   while (1) {
      if (i == 1) break;
      p = i / 2;
      if (Q[p].wt <= Q[i].wt) break;
      u = Q[i].vid; v = Q[p].vid;
      t = Q[i]; Q[i] = Q[p]; Q[p] = t;
      L[u] = p; L[v] = i;
      i = p;
   }
}

void printheap ( heap Q, int size, int s )
{
   int i;

   for (i=1; i<=size; ++i) {
      if (Q[i].wt == INFTY)
         printf("(%d,%d) INF ", s, Q[i].vid);
      else
         printf("(%d,%d) %lf ", s, Q[i].vid, Q[i].wt);
   }
   printf("\n");
}

void printpath ( int *prev, int s, int v, int flag )
{
   if (v == s) {
      printf("%d", s); fflush(stdout);
      return;
   }
   if (!flag) {
      printpath(prev,s,prev[v],flag);
      printf(" - %d", v); fflush(stdout);
   } else {
      printpath(prev,s,prev[v],flag);
      if (v % 2 == 0) printf(" - %d", v/2);
      fflush(stdout);
   }
}

void Dijkstra ( graph G, int n, int s, int t, int flag )
{
   hnode *Q;
   int *L, *prev;
   int size, w, v;
   double d, newd;
   node *p;

   Q = (hnode *)malloc(n * sizeof(hnode));
   L = (int *)malloc(n * sizeof(int));
   prev = (int *)malloc(n * sizeof(int));
   size = n-1;
   initheap(Q,L,prev,G,n,s);
   makeheap(Q,L,n-1);
   while (1) {
      // printheap(Q,size,s);
      if (Q[1].wt == INFTY) {
         printf("    Node %d cannot be reached from Node %d\n", t, s);
         break;
      }
      w = Q[1].vid;
      d = Q[1].wt;
      deletemin(Q,L,size);
      --size;
      if (w == t) {
         if (!flag) {
            printf("--- Shortest (%d,%d) distance is %lf\n", s, t, d);
            printf("--- Shortest (%d,%d) path: ", s, t); fflush(stdout);
         } else {
            printf("--- Shortest (%d,%d) distance is %lf\n", s/2, t/2, d);
            printf("--- Shortest (%d,%d) path: ", s/2, t/2); fflush(stdout);
         }
         printpath(prev,s,t,flag); printf("\n");
         break;
      } else {
         p = G[w].nbr;
         while (p) {
            v = p -> vid;
            if (L[v] != -1) {
               newd = d + p -> ewt;
               if (newd < Q[L[v]].wt) {
                  changepriority(Q,L,v,newd);
                  prev[v] = w;
               }
            }
            p = p -> next;
         }
      }
   }
}

void changeedgecosts ( graph G, int n )
{
   node *p;
   int u;

   for (u=0; u<n; ++u) {
      p = G[u].nbr;
      while (p) {
         printf("    Edge weight (%d,%d) changes from %3.1lf to ", u, p -> vid, p -> ewt);
         p -> ewt = (p -> ewt == 1.0) ? 0.0 : -log(p -> ewt);
         printf("%lf\n", p -> ewt);
         p = p -> next;
      }
   }
}

graph convertgraph ( graph G, int n )
{
   graph H;
   int u, v;
   node *p, *q;

   H = (header *)malloc(2 * n * sizeof(header));
   for (u=0; u<n; ++u) {
      p = (node *)malloc(sizeof(node));
      p -> ewt = (double)(G[u].vwt);
      p -> vid = 2*u + 1;
      H[2*u].nbr = p;
      H[2*u+1].nbr = NULL;

      q = G[u].nbr;
      while (q) {
         v = q -> vid;
         p = (node *)malloc(sizeof(node));
         p -> ewt = 0;
         p -> vid = 2 * v;
         p -> next = H[2*u+1].nbr;
         H[2*u+1].nbr = p;
         q = q -> next;
      }
   }
   return H;
}

int main ( int argc, char *argv[] )
{
   int n, m, s, t;
   graph G, H;

   if (argc >= 3) {
      n = atoi(argv[1]);
      m = atoi(argv[2]);
   } else {
      scanf("%d%d", &n, &m);
   }
   s = 0; t = n-1;
   printf("n = %d\nm = %d\ns = %d\nt = %d\n", n, m, s, t);

   srand((unsigned int)time(NULL));

   printf("\n+++ Reading edges\n");
   G = gengraph(n,m);

   printf("\n+++ The generated graph\n");
   prngraph(G,n);

   printf("\n+++ Reading edge weights\n");
   readedgewts(G,n);

   printf("\n+++ Reading vertex weights\n");
   readvertexwts(G,n);

   printf("\n+++ Running Dijkstra on the original graph\n");
   Dijkstra(G,n,s,t,0);

   printf("\n+++ Changing the edge weights\n");
   changeedgecosts(G,n);

   printf("\n+++ Running Dijkstra on the log-converted graph\n");
   Dijkstra(G,n,s,t,0);

   printf("\n+++ Converting vertex weights to edge weights\n");
   H = convertgraph(G,n);
   prngraph(H,2*n);

   printf("\n+++ Running Dijkstra on the vertex-weight graph\n");
   Dijkstra(H,2*n,2*s,2*t+1,1);

   exit(0);
}