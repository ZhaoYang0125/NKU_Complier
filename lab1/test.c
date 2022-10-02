#include<stdio.h>
int sum=1;
char* str="Error input!";

int factorial(int n){
    while(n>0){
        sum=sum*n;
        n=n-1;
    }
    return sum ;
}

int main(){
    int n;
    scanf("%d",&n);
    if(n>0&&n<11){
        float tem;
        tem=factorial(n);
        printf("%f\n",tem);
    }
    else{
        printf("%s\n",str);
    }
}