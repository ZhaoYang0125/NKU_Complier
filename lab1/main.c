#include<stdio.h>
#define NUM 2//hongdingyi

int func1(int i,int n,int f){//function
    while (i<=n)//loop
    {
        f=f*i;
        i=i+1;
    }
    return f;    
}
int main(){
    int i,n,f;
    int inf;//dingyierweishiyongdebianliang
    float tem;//sidaima
    tem=3.14;
    scanf("%d",&n);
    i=NUM;
    f=1;
    f=func1(i,n,f);
    printf("%d\n",f);
}
