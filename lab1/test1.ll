;定义全局变量sum，类型为i32及k整型，初始值为1
@sum = global i32 1, align 4
;定义全局变量str，是一个长度为13的字符串
@str = global i8* getelementptr inbounds ([13 x i8], [13 x i8]* @.str, i32 0, i32 0), align 8
;定义全局字符串常量，分别是错误输入的字符串，%d  %f  %s输出格式控制字符
@.str = private unnamed_addr constant [13 x i8] c"Error input!\00", align 1
@.str.1 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@.str.2 = private unnamed_addr constant [5 x i8] c"%lf\0A\00", align 1
@.str.3 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1

;定义函数factorial，返回值类型为整形，有一个整型的参数
;每一个标签对应一个基本块,每个基本块以br或ret结尾
define i32 @factorial(i32 %0){
  ;传递参数，先通过0%，再赋值给其他寄存器(由于涉及到调用函数时的指针问题必须进行转换)
  %2 = alloca i32, align 4
  store i32 %0, i32* %2, align 4
  br label %3

3:                                         
  %4 = load i32, i32* %2, align 4
  ;循环条件判定，决定进入循环体还是直接返回
  %5 = icmp sgt i32 %4, 0
  br i1 %5, label %6, label %11

6:              
  ;调用全局变量sum
  %7 = load i32, i32* @sum, align 4
  ;引入n
  %8 = load i32, i32* %2, align 4
  ;相乘
  %9 = mul i32 %7, %8
  ;保存sum
  store i32 %9, i32* @sum, align 4
  ;n-=1
  %10 = sub i32 %8, 1
  ;存n
  store i32 %10, i32* %2, align 4
  ;循环
  br label %3

11:
  ;加载sum
  %12 = load i32, i32* @sum, align 4
  ;返回
  ret i32 %12
}

; Function main
define i32 @main() {
;为变量分配空间
  %1 = alloca i32, align 4
;调用scanf函数
  %2 = call i32 (i8*, ...) @__isoc99_scanf(i8*  getelementptr inbounds ([3 x i8], [3 x i8]* @.str.1, i64 0, i64 0), i32* %1)
; 调用load函数  
  %3 = load i32, i32* %1, align 4
; 比较 大于置位  
  %4 = icmp sgt i32 %3, 0
; 条件跳转  
  br i1 %4, label %5, label %12

5:         
; 比较 小于置位
  %6 = icmp slt i32 %3, 11
; 条件跳转  
  br i1 %6, label %7, label %12

7:                                               
; 调用load函数
  %8 = load i32, i32* %1, align 4
; 调用自定义函数factorial  
  %9 = call i32 @factorial(i32 %8)
; 强制类型转换 int to double  
  %10 = sitofp i32 %9 to double
; 调用printf函数输出  
  %11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.2, i64 0, i64 0), double %10)
; 无条件跳转
  br label %15

12:                                 
; 调用load函数，加载常量字符串
  %13 = load i8*, i8** @str, align 8
; 调用printf函数输出  
  %14 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.3, i64 0, i64 0), i8* %13)
; 无条件跳转
  br label %15

15:                                     
; main函数返回
  ret i32 0
}

;声明使用到的外部的函数scanf和printf
declare i32 @__isoc99_scanf(i8* , ...)
declare i32 @printf(i8* , ...)