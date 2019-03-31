#include <stdlib.h>
#include<stdio.h>
#include <malloc.h>


typedef struct node{
    int i;
    int sigma;
    int flag;
    int flag2; 
} table;

int exhs(int *A, int N, int T, int i, int P){
    if(i==N)
        return P;
    int k1 = -1,k2 = -1;
    k2 = exhs(A,N,T,i+1,P);
    if(P+A[i] <= T){
        k1 = exhs(A,N,T,i+1,P+A[i]);
    }
    return (k1>=k2)?k1:k2;
    
}

table* htinit(int s){
    table* H = (table*)malloc(s*sizeof(table));
    for(int i=0;i<s;i++){
        H[i].flag2 = 0;
    }
    return H;
}

int htsearch(table* H,int i,int sigma,int s){
    int key;
    key = (100003*i + 103*sigma)%s;
    
    if(H[key].flag2 == 0)
        return 0;
    else{
        while(H[key].i != i && H[key].sigma != sigma && H[key].flag2 == 1){
            key++;
            key = key%s;
        }
        if(H[key].flag2 == 0){
            return 0;
        }
        else return ((key+1)%s);
    }
}

void htinsert(table* H, int i, int sigma, int flag, int s){
    int k;
    k = 100003*i + 103*sigma;
    k = k%s;
    while(H[k].flag2 == 1){
        k++;
        k = k%s;
    }
    H[k].flag2 = 1;
    H[k].i = i;
    H[k].sigma = sigma;
    H[k].flag = flag;
}

int hashs(int* A, int N, int T, int i, int sigma, table* H, int s ){
    if(i==N)
        return sigma;
    int k1 = -1, k2 = -1;
    if(htsearch(H,i,sigma,s) == 0){
        htinsert(H,i,sigma,0,s);
        k2 = hashs(A,N,T,i+1,sigma,H,s);
    }
    if(htsearch(H,i,sigma+A[i],s) == 0 && sigma+A[i] <=T){
        htinsert(H,i,sigma+A[i],1,s);
        k1 = hashs(A,N,T,i+1,sigma+A[i],H,s);
    }
    return (k1>=k2)?k1:k2;
}

void findsol(int* A, int N, table* H, int maxshare,int s){
    int index;
    for(int i=0;i<N;i++){
         index = htsearch(H,i,maxshare,s);
         if(index!=0)
             break;
    }
    index--;
    while(maxshare!=0){
        if(H[index].flag == 1){
            maxshare = maxshare - A[H[index].i];
            if(maxshare == 0){
                printf(" %d", A[H[index].i]);
                break;
            }
            printf(" %d + ", A[H[index].i]);
            for(int i =0;i<N;i++){
                index = htsearch(H,i,maxshare,s);
                if(index!=0){
                    index--;
                    break;
                }
            }
        }
    }
}

void main(){
    int N,S = 0;
    scanf("%d", &N);
    printf("N = %d\n", N);
    int* A = (int*)malloc(N*sizeof(int));
    for(int i =0;i<N;i++){
        scanf("%d", &A[i]);
    }
    for(int i = 0;i<N;i++){
        S = S + A[i];
    }
    int T = S/2;
    printf("S = %d \nT = %d\n", S,T);
    printf("+++Exhaustive search \n");
    int P = exhs(A,N,T,0,0);
    printf("   Maximum P =  %d\n",P);


    int s = N*T;
    table* Htable = htinit(s);

    printf("+++ Search with a hash table\n");
    int Phash = hashs(A,N,T,0,0,Htable,s);
    printf("   Maximum P =  %d\n",Phash);

    printf("   Solution: ");
    findsol(A,N,Htable,Phash,s);
    printf("\n");

}