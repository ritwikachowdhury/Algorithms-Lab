#include<stdio.h>
#include<stdlib.h>
#include<math.h>

typedef struct node{

	int l;
	int r;
	int c;

}heapnode;

int leftchild(int i){

    return (2*i+1);
}

int rightchild(int i){

    return (2*i+2);
}


int parent(int i){
    return (i/2);
}

void swap(heapnode *n1, heapnode *n2) {
    heapnode temp = *n1 ;
    *n1 = *n2 ;
    *n2 = temp ;
}


void heapify(heapnode* H, int j, int n){
	// printf("%d %d \n",j,n);
 int l=leftchild(j);
 int r=rightchild(j);
 int smallest;
 heapnode temp;

 if(l>=n) return;
else
	{
	if(H[l].c<H[j].c) smallest=l;

 	else smallest=j;

 	if(r>=n) return;
 	else
 	{
 	if(H[r].c<H[smallest].c) smallest=r;

 		if(smallest!=j) 
 			{
 			swap(&(H[j]), &(H[smallest])) ;
    // temp=H[j];
    // H[j]=H[smallest];
    // H[smallest]=temp;

     		heapify(H,smallest,n);
			}
		}
	}
}



void makeheap(heapnode *H,int n)
	{
	int heapsize=n;
	int i=0;
	for(i=n/2;i>=0;i--)
		{

    		heapify(H,i,heapsize);
		}

	
	}

void insert(heapnode *H,int index,int heapcapacity ,heapnode key)
{
if(index>heapcapacity-1)
	{
		printf("\nThere is no more space in the heap");
		return;
	}

	H[index]=key;
	int i=index;
	while(i>=0&&H[parent(i)].c>H[i].c)
		{

   			swap(&H[i],&H[parent(i)]);
    		i=parent(i);
		}
}


void extractmin(heapnode *H, int heapsize)
	{
		// printf("\nWe are inside extractmin");
		// printf("\n%d",heapsize);
		int i=heapsize-1;
		H[0]=H[i];
		heapsize--;
       // assert(heapsize!=0);
		heapify(H,0,heapsize);

	}


void greedy(int N,int K,int *D)
	{
		int i;
		int rc,rl,rr;
		int *arr=(int*)malloc((N)*sizeof(int));
		heapnode *H=(heapnode*)malloc((N-1)*sizeof(heapnode));
		for(i=0;i<N-1;i++)
			{
				H[i].l=i;
				H[i].r=i+1;
				H[i].c=D[i];
				arr[i]=0;

			}
		arr[N-1]=0;
		int cost=0;
		int heapsize=N-1;
		makeheap(H,N-1);

		while(K>0)
		{
			rl=H[0].l;
			rr=H[0].r;
			rc=H[0].c;

			if((!arr[rl])&&(!arr[rr]))
			{
				
				printf("(%d,%d): %d, ",H[0].l,H[0].r,H[0].c);
				cost=cost+rc;
				arr[rl]=1;
				arr[rr]=1;
				K--;

			}
			//K--;
			extractmin(H,heapsize);
			heapsize--;
		}

		printf("\n\nTotal cost = %d",cost);

		

	}


// void greedyec2(int N,int K,int *D)
// 	{
// 		int i;
// 		int rc,rl,rr;
// 		int *arr=(int*)malloc((N)*sizeof(int));
// 		heapnode *H=(heapnode*)malloc((N-1)*sizeof(heapnode));
// 		heapnode *realconn=(heapnode*)malloc((K)*sizeof(heapnode));
// 		heapnode *virtualconn=(heapnode*)malloc((K)*sizeof(heapnode));
// 		for(i=0;i<N-1;i++)
// 			{
// 				H[i].l=i;
// 				H[i].r=i+1;
// 				H[i].c=D[i];
// 				arr[i]=0;

// 			}
// 		arr[N-1]=0;
// 		int cost=0;
// 		int heapsize=N-1;
// 		makeheap(H,N-1);
// 		int virtualcost=0;
// 		i=0;
// 		int j=0;
// 		int k=0;
// 		int flag=0;
// 		while(K>0)
// 		{
// 			rl=H[0].l;
// 			rr=H[0].r;
// 			rc=H[0].c;

// 			if((!arr[rl])&&(!arr[rr]))
// 			{

// 				if(rr==rl+1)
// 				{
// 				realconn[i]=H[0];
// 				cost=cost+rc;
// 				i++;
// 				}
// 				else{
// 					flag=1;
// 					if(rl>=0&&rr<N-1)

// 					virtualconn[j].l=rl-1;
// 					virtualconn[j].r=rr+1;
// 					for(k=rl-1;k<rr+1;k++)
// 					{
// 					virtualcost=virtualcost+D[k];
// 					}
// 					virtualconn[j].c=virtualcost;
// 					cost=cost+virtualconn[j].c;
// 				    j++;
// 				  }
				
// 				arr[rl]=1;
// 				arr[rr]=1;
// 				K--;
				

// 			}
// 			extractmin(H,heapsize);
// 			heapsize--;
// 			if(flag==1)
// 			{
// 			insert(H,heapsize-1,N-1,virtualconn[j-1]);
// 			}
			
// 		}

// 		printf("\n\nTotal cost = %d",cost);

		

// 	}

void greedyec(int N, int K, int* D){
    heapnode* H = (heapnode*)malloc((N - 1)*sizeof(heapnode));
    int* used_nodes = (int*)malloc(N*sizeof(int));
    for(int i = 0;i<N;i++){
        used_nodes[i] = 0;// 0 represents unconnected node
    }
    for(int i = 0;i<(N-1);i++){
        H[i].l = i;
        H[i].r = i+1;
        H[i].c = D[i];
    }
    makeheap(H,N-1);
    int n = N-1;
    int cost = 0;
    int left;
    int right;
    int cost_ind;
    heapnode* con = (heapnode*)malloc(K*sizeof(heapnode));
    for(int i = 1;i<=K;){
        left = H[0].l;
        right = H[0].r;
        cost_ind = H[0].c;
        if(used_nodes[left] == 0 && used_nodes[right] == 0){
            cost = cost + cost_ind;
            i++;
            used_nodes[left] = 1;
            used_nodes[right] = 1;
            con[i - 1] = H[0];
            if(left - 1 >= 0 && right+1 <= N - 1 && used_nodes[left - 1] ==0 && used_nodes[right + 1] == 0){
                heapnode new;
                new.l = left - 1;
                new.r = right + 1;
                new.c = D[left - 1] + D[right] - cost_ind;
                extractmin(H,n);
                n--;
                insert(H,n,N-1,new);
                n++;
            }
        }
        else{
            extractmin(H,n);
            n--;
        }
    }

    
    printf("The total cost is %d \n", cost);
}



int main(){

int N,K;

heapnode *H1=(heapnode*)malloc((N-1)*sizeof(heapnode));
printf("N = ");
scanf("%d",&N);
printf("\nK =");
scanf("%d",&K);
int i;
printf("\nPlease enter the distances\n");
int* D=(int *)malloc((N-1)*sizeof(int));
for(i=0;i<N-1;i++)
	{
		scanf("%d",&D[i]);
	}

printf("\n***Part 1: Greedy algorithm");
printf("\n--- Adding connections\n");
//heapnode *H=greedy(N,K,D);
// for(i=0;i<N-1;i++)
// 	{
		
// 	}
greedy(N,K,D);
printf("\n***Part 2: Greedy algorithm with error correction");
printf("\n--- Adding connections\n");
greedyec( N,K,D);
return 0;

}