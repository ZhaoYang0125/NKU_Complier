	@程序始开始前的准备
	.text
	.section	.rodata
	.align	2
_str0:
	@格式控制符%d设置为字符串
	.ascii	"%d\000"
	.align  2
	@main函数声明
	.text
	.global	main
	.syntax unified
	.thumb
	.thumb_func
	.type	main, %function
main:
	push	{fp, lr}      @将fp和lr依次入栈
	sub	sp, sp, #32   @将栈帧下移，开辟空间
	add	fp, sp, #0    @fp保存新的栈底
	ldr	r2, _bridge
.LPIC3:
	add	r2, pc
	ldr	r3, _bridge+4
	ldr	r3, [r2, r3]
	ldr	r3, [r3]
	str	r3, [fp, #28] @为变量开辟空间
	movs	r3, #0        @arr1[0]=0
	str	r3, [fp, #20]
	movs	r3, #1        @arr1[1]=1
	str	r3, [fp, #24]
	movs	r3, #15       @num1=15
	str	r3, [fp, #8]
	movs	r3, #5        @num2=5
	str	r3, [fp, #12]
	adds	r3, fp, #4
	mov	r1, r3
	ldr	r3, _bridge+8
.LPIC0:
	add	r3, pc
	mov	r0, r3
	bl	__isoc99_scanf@调用输入函数
	ldr	r3, [fp, #4]  @获取输入值
	cmp	r3, #0        @输入值和0相比
	bne	.L2           @不等则跳转
	ldr	r3, [fp, #24] @arr1[1]
	mov	r1, r3
	ldr	r3, _bridge+12
.LPIC1:
	add	r3, pc
	mov	r0, r3
	bl	printf      @输出
	b	.L3
.L2:
	ldr	r2, [fp, #8] @取num1
	ldr	r3, [fp, #12]@取num2
	ands	r3, r3, r2   @做按位与
	str	r3, [fp, #16]@结果给num3
	ldr	r1, [fp, #16]
	ldr	r3, _bridge+16 @调用%d
.LPIC2:
	add	r3, pc
	mov	r0, r3
	bl	printf       @输出
.L3:
	movs	r3, #0   @堆栈保护机制，本段代码为编译器生成
	ldr	r1, _bridge+20
.LPIC4:
	add	r1, pc
	ldr	r2, _bridge+4 
	ldr	r2, [r1, r2]
	ldr	r1, [r2]
	ldr	r2, [fp, #28]
	eors	r1, r2, r1
	mov	r2, #0
	beq	.L5
	bl	__stack_chk_fail  @检测到堆栈溢出
.L5:
	mov	r0, r3
	adds	fp, fp, #32
	mov	sp, fp
	@ sp needed
	pop	{fp, pc}@寄存器出栈，main函数结束
.L7:
	.align	2
_bridge:
    @某常量在全局各段的偏移量汇总，通过动态链接的方式分段
	.word	_GLOBAL_OFFSET_TABLE_-(.LPIC3+4)
	.word	__stack_chk_guard(GOT)   @canary word
	.word	_str0-(.LPIC0+4)
	.word	_str0-(.LPIC1+4)
	.word	_str0-(.LPIC2+4)
	.word	_GLOBAL_OFFSET_TABLE_-(.LPIC4+4)
	@告知堆栈的映射权限
	.section	.note.GNU-stack,"",%progbits