	.text
	.global	zero		@ gloabl constant variable zero
	.section .rodata
	.align	2
	.type	zero, %object	
	.size	zero, 4
	.text
	.align	2
zero:
	.space	4
	.align	2
_str0:
	.ascii	"%d"
	.align	2
_str1:
	.ascii	"%d\n"
	.align	2

factorial:	@ function int factorial(int n)
	str lr, [sp,#-4]! 	@ sp = sp -4, Push lr 
        str r0, [sp,#-4]!  	@ Push r0 ，int n
	mov	r1, #1		    @ int sum=1
	b	loop	
loop:		                @ while
	cmp r0,	#0	        @ compare n ? 0
	ble	end		        @ n <= 0
	mul	r1,	r0	        @ sum=sum*n
	sub	r0,	r0,	#1	    @n=n-1
	b	loop
end:
	add	sp,	sp, #4		@ drop n
	ldr	lr,	[sp], #4	@ recover lr
	bx	lr				@ return
	
	.global main
main:
        str lr, [sp,#-4]!   @ push lr
        sub sp, sp, #4    	@ open space for int n
	mov r1, sp			@ &n
	ldr	r0,	_bridge+4	@ r0 = "%d"
	bl	__isoc99_scanf	@ scanf("%d", &n)

	ldr	r0, [sp]		@ load para
	bl	factorial		@ call factorial

	mov r2, r1			@ r2 = r1 = factorial(n)
	ldr r0, _bridge+8	@ *r0 = "%d"
	bl	printf			@ printf("%d\n", &r2)

	add	sp,	sp,	#4		@ release space for n
	ldr	lr,	[sp], #4	@ recover lr
	bx	lr				@ quit

_bridge:
	.word	zero
	.word	_str0
	.word	_str1