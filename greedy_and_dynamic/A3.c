#include<stdio.h>
#include<stdlib.h>
#include<math.h>


void merge(int arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;
 
    int L[n1], R[n2];
 
   
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1+ j];
    i = 0; 
    j = 0;
    k = l;
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
 
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }
 
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}
 
void Sort(int arr[], int l, int r)
{
    if (l < r)
    {
       
        int m = l+(r-l)/2;
        Sort(arr, l, m);
        Sort(arr, m+1, r);
 		merge(arr, l, m, r);
    }
}



void solveGRD(int n,int M,int arr[]){

	int i,k;
	int sum=0;;
   Sort(arr,0,n-1);
   for(i=0;i<n;i++)
		{
			if((sum+arr[i])<=M)
			sum=sum+arr[i];
			else break;
			


		}

   printf("\n+++ Part1(Greedy)");
   printf("\n Minimum Effort= %d -",sum);

for(k=0;k<i-1;k++)
		{   
			printf("%d + ",arr[k]);


		}

		printf("%d ",arr[k]);

}
int minimum(int x,int y){

if (x<=y) return x;
return y;


}


void solveDP(int n,int M,int P,int m[],int p[])
{
    
    int i,j,M0,M1;
    int max_value=M+1;
    int **T = (int **)malloc((n+1) * sizeof(int *));
    for (i=0; i<n+1; i++)
    {
         T[i] = (int *)malloc((P+1) * sizeof(int));
         T[i][0] = 0;
    }
    for (j=1;j<P+1;j++)
        T[0][j] = max_value;
    for (i=1;i<n+1;i++)
    {
        for(j=1;j<P+1;j++)
        {
            if(j < p[i-1])
                T[i][j] = T[i-1][j];
            else
            {
                
                M0 = T[i-1][j];
                M1 = m[i-1] + T[i-1][j-p[i-1]];
                if(M1>M)
                    T[i][j] = M0;
                else
                    T[i][j] = minimum(M0,M1);

            }

        }
    }
    for (j = P; j >= 0 ; j--)
    {
        if (T[n][j] < max_value)
        {
            
             printf("\nMaximum point achieved is%d",j);
    		printf("\nminimum effort required is%d",T[n][j]);
            break;
        }
    }
   

}


void solveDPEXT(int n,int M,int P,int m[],int p[])
{

    int max_value=M+1;
    int i,j,M0,M1;
    int **T = (int **)malloc((n+1) * sizeof(int *));
    for (i=0; i<n+1; i++)
    {
         T[i] = (int *)malloc((P+1) * sizeof(int));
         T[i][0] = 0;
    }
    for (j=1;j<P+1;j++)
        T[0][j] = max_value;
    for (i=1;i<n+1;i++)
    {
        for(j=1;j<P+1;j++)
        {
            if(j < p[i-1])
                T[i][j] = T[i-1][j];
            else
            {
                M0 = T[i-1][j];
                M1 = m[i-1] + T[i-1][j-p[i-1]];
                if(M1>M)
                    T[i][j] = M0;
                else
                    T[i][j] = minimum(M0,M1);

            }

        }
    }
    int effort;
    int points;
    for (j = P; j >= 0 ; j--)
    {
        if (T[n][j] < max_value)
        {
            
            points=j;
            effort=T[n][j];
            break;
        }
    }
    int *result=(int*)malloc(n*sizeof(int));

    i=n;
    j=points;
    int  ind=0;
    while((i>0)&&(j>0))
    {

    	if(T[i][j]==T[i-1][j])
    	  {
             i--;
    	  }

    	  else
    	  {
    	  	result[ind]=i-1;
    	  	j=j-p[i-1];
    	  	i--;
    	  	ind++;
    	  }


    }
    printf("\nMaximum points=%d -- ",points);
    for (j=ind-1;j>=0;j--)
    {
        if(j)
        printf("%d +",p[result[j]]);
        else
        printf("%d",p[result[j]]);  
        }  
    printf("\nminimum effort = %d -- ",effort);
    for (j=ind-1;j>=0;j--){
        if(j)
        printf("%d +",m[result[j]]);
        else
        printf("%d\n",m[result[j]]);
}

 }







int main(){
int n,M;
printf("n=");
scanf("%d",&n);
printf("\n");
printf("M=");
scanf("%d",&M);
printf("\nEfforts :");
int j,*m,*p,P;
P=0;
m=(int *)malloc(n*sizeof(int));
 for(j=0;j<n;j++){
	scanf("%d",&m[j]);
	
	
}
printf("\n+++ Part 1 (Greedy)");
 solveGRD(n,M,m);
p=(int *)malloc(n*sizeof(int));
printf("\nPoints:");
for(j=0;j<n;j++){
	scanf("%d",&p[j]);
	
	
}

printf("\nEfforts :");

for(j=0;j<n;j++){
	scanf("%d",&m[j]);
	
	
}
for(j=0;j<n;j++){
	P=P+p[j];
	
	
}

printf("\n+++ Part 2 (DP)");
solveDP(n,M,P,m,p);
printf("\n+++ Part 3 (DP Extended)");
solveDPEXT(n,M,P,m,p);
return 0;

}