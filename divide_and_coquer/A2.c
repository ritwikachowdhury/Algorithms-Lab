
#include<stdio.h>
#include<stdlib.h>



extern int registerme ( int );
extern void startsort ( );
extern int compareballs ( int, int );
extern void verifysort ( int*);
extern void startmatch ( int );
extern int fitsin ( int i, int j );
extern void verifymatch ( int*);



void mergeArray(int A[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;
 
   
	int *L;
	int *R;
	L=(int*) malloc(n1*sizeof(int));
	R=(int*) malloc(n2*sizeof(int));
 
    
    for (i = 0; i < n1; i++)
        L[i] = A[l + i];
    for (j = 0; j < n2; j++)
        R[j] = A[m + 1+ j];
 
    
    i = 0; 
    j = 0; 
    k = l; 
    while (i < n1 && j < n2)
    {
        if (compareballs(L[i],R[j])==-1)
        {
            A[k] = L[i];
            i++;
        }
        else
        {
            A[k] = R[j];
            j++;
        }
        k++;
    }
 
    
    while (i < n1)
    {
        A[k] = L[i];
        i++;
        k++;
    }
 
    
    while (j < n2)
    {
        A[k] = R[j];
        j++;
        k++;
    }
}
 

void mergeSort(int A[], int low, int high)
{

	int mid;
	mid = low+(high-low)/2;
    if (low < high)
    {
         
        mergeSort(A, low, mid);
        mergeSort(A, mid+1, high);
 
        mergeArray(A, low, mid, high);
    }
}

void swapElements(int* x, int* y)
{
    int t= *x;
    *x = *y;
    *y = t;
}
int partition (int *M, int *S, int l, int r)
{   
    int i = (l - 1);
    int mid = l+(r-l)/2;
    int k;
	int j=l;
    while( j < r) 
    {
        k = fitsin(S[mid],M[j]);
       
        if(k == 0)
        {
            swapElements(&M[j],&M[r]);
            j--;
        }
        else if(k== -1)
        {
            i++;
            swapElements(&M[i],&M[j]);
        }
	j=j+1;
    }
    swapElements(&M[i+1],&M[r]);
    i=i+1;
    return (i);
}

void quickSort(int *M, int *S, int l, int h)
{
    if (l< h)
    {
        int part = partition(M, S, l, h);

        quickSort(M, S, l, part - 1);
        quickSort(M, S, part + 1, h);
    }
}


int main()

{
	int n = 100000;
	registerme(n);
	printf("\n+++ Sorting the balls...\n");
	startsort();
	int i;

	int *S;
	S=(int *)malloc(n*sizeof(int));
	for (i = 0; i < n; i++)
        S[i] =i;
	mergeSort(S,0,n-1);
	verifysort(S);
	printf("\n+++ Finding the matching boxes...\n");
	int *M;
	M=(int *)malloc(n*sizeof(int));
	for (i = 0; i < n; i++)
        M[i] =i;

	startmatch(1);
	quickSort(M,S,0, n-1);
	 int *A;
    A = (int *)malloc((n)*sizeof(int));
    for (int j = 0; j<n ; j++)
        {
            A[S[j]] = M[j];
        }

	//;
	verifymatch(A);

}