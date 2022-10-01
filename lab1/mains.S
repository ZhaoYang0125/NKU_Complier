	.text
	.file	"main.c"
	.section	.rodata.cst16,"aM",@progbits,16
	.p2align	4                               # -- Begin function func1
.LCPI0_0:
	.zero	4
	.long	1                               # 0x1
	.long	1                               # 0x1
	.long	1                               # 0x1
.LCPI0_1:
	.long	0                               # 0x0
	.long	1                               # 0x1
	.long	2                               # 0x2
	.long	3                               # 0x3
.LCPI0_2:
	.long	4                               # 0x4
	.long	4                               # 0x4
	.long	4                               # 0x4
	.long	4                               # 0x4
.LCPI0_3:
	.long	2147483648                      # 0x80000000
	.long	2147483648                      # 0x80000000
	.long	2147483648                      # 0x80000000
	.long	2147483648                      # 0x80000000
	.text
	.globl	func1
	.type	func1,@function
func1:                                  # @func1
	.cfi_startproc
# %bb.0:
	movl	%edx, %eax
                                        # kill: def $esi killed $esi def $rsi
	cmpl	%esi, %edi
	jg	.LBB0_4
# %bb.1:
	subl	%edi, %esi
	leal	4(%rsi), %ecx
	andl	$-4, %ecx
	movd	%esi, %xmm0
	pshufd	$0, %xmm0, %xmm0                # xmm0 = xmm0[0,0,0,0]
	movd	%eax, %xmm2
	movaps	.LCPI0_0(%rip), %xmm1           # xmm1 = <u,1,1,1>
	movss	%xmm2, %xmm1                    # xmm1 = xmm2[0],xmm1[1,2,3]
	movd	%edi, %xmm2
	pshufd	$0, %xmm2, %xmm3                # xmm3 = xmm2[0,0,0,0]
	paddd	.LCPI0_1(%rip), %xmm3
	movl	$4, %eax
	movdqa	.LCPI0_2(%rip), %xmm4           # xmm4 = [4,4,4,4]
.LBB0_2:                                # =>This Inner Loop Header: Depth=1
	movaps	%xmm1, %xmm2
	pmuludq	%xmm3, %xmm1
	pshufd	$232, %xmm1, %xmm1              # xmm1 = xmm1[0,2,2,3]
	pshufd	$245, %xmm3, %xmm5              # xmm5 = xmm3[1,1,3,3]
	pshufd	$245, %xmm2, %xmm6              # xmm6 = xmm2[1,1,3,3]
	pmuludq	%xmm5, %xmm6
	pshufd	$232, %xmm6, %xmm5              # xmm5 = xmm6[0,2,2,3]
	punpckldq	%xmm5, %xmm1            # xmm1 = xmm1[0],xmm5[0],xmm1[1],xmm5[1]
	paddd	%xmm4, %xmm3
	movl	%eax, %edx
	addl	$-4, %eax
	addl	%ecx, %edx
	addl	$-4, %edx
	cmpl	$4, %edx
	jne	.LBB0_2
# %bb.3:
	negl	%eax
	movd	%eax, %xmm3
	pshufd	$0, %xmm3, %xmm3                # xmm3 = xmm3[0,0,0,0]
	por	.LCPI0_1(%rip), %xmm3
	movdqa	.LCPI0_3(%rip), %xmm4           # xmm4 = [2147483648,2147483648,2147483648,2147483648]
	pxor	%xmm4, %xmm0
	pxor	%xmm4, %xmm3
	pcmpgtd	%xmm0, %xmm3
	pand	%xmm3, %xmm2
	pandn	%xmm1, %xmm3
	por	%xmm2, %xmm3
	pshufd	$238, %xmm3, %xmm0              # xmm0 = xmm3[2,3,2,3]
	pshufd	$255, %xmm3, %xmm1              # xmm1 = xmm3[3,3,3,3]
	pshufd	$245, %xmm3, %xmm2              # xmm2 = xmm3[1,1,3,3]
	pmuludq	%xmm1, %xmm2
	pmuludq	%xmm3, %xmm0
	pmuludq	%xmm2, %xmm0
	movd	%xmm0, %eax
.LBB0_4:
	retq
.Lfunc_end0:
	.size	func1, .Lfunc_end0-func1
	.cfi_endproc
                                        # -- End function
	.section	.rodata.cst16,"aM",@progbits,16
	.p2align	4                               # -- Begin function main
.LCPI1_0:
	.long	2                               # 0x2
	.long	3                               # 0x3
	.long	4                               # 0x4
	.long	5                               # 0x5
.LCPI1_1:
	.long	1                               # 0x1
	.long	1                               # 0x1
	.long	1                               # 0x1
	.long	1                               # 0x1
.LCPI1_2:
	.long	4                               # 0x4
	.long	4                               # 0x4
	.long	4                               # 0x4
	.long	4                               # 0x4
.LCPI1_3:
	.long	0                               # 0x0
	.long	1                               # 0x1
	.long	2                               # 0x2
	.long	3                               # 0x3
.LCPI1_4:
	.long	2147483648                      # 0x80000000
	.long	2147483648                      # 0x80000000
	.long	2147483648                      # 0x80000000
	.long	2147483648                      # 0x80000000
	.text
	.globl	main
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:
	pushq	%rbx
	.cfi_def_cfa_offset 16
	subq	$16, %rsp
	.cfi_def_cfa_offset 32
	.cfi_offset %rbx, -16
	leaq	12(%rsp), %rbx
	movl	$.L.str, %edi
	movq	%rbx, %rsi
	xorl	%eax, %eax
	callq	__isoc99_scanf@PLT
	movl	(%rbx), %ecx
	movl	$1, %esi
	cmpl	$2, %ecx
	jl	.LBB1_4
# %bb.1:
	leal	2(%rcx), %eax
	andl	$-4, %eax
	addl	$-2, %ecx
	movd	%ecx, %xmm0
	pshufd	$0, %xmm0, %xmm0                # xmm0 = xmm0[0,0,0,0]
	movdqa	.LCPI1_0(%rip), %xmm3           # xmm3 = [2,3,4,5]
	movdqa	.LCPI1_1(%rip), %xmm1           # xmm1 = [1,1,1,1]
	movl	$4, %ecx
	movdqa	.LCPI1_2(%rip), %xmm4           # xmm4 = [4,4,4,4]
.LBB1_2:                                # =>This Inner Loop Header: Depth=1
	movdqa	%xmm1, %xmm2
	movdqa	%xmm3, %xmm1
	pmuludq	%xmm2, %xmm1
	pshufd	$232, %xmm1, %xmm1              # xmm1 = xmm1[0,2,2,3]
	pshufd	$245, %xmm2, %xmm5              # xmm5 = xmm2[1,1,3,3]
	pshufd	$245, %xmm3, %xmm6              # xmm6 = xmm3[1,1,3,3]
	pmuludq	%xmm5, %xmm6
	pshufd	$232, %xmm6, %xmm5              # xmm5 = xmm6[0,2,2,3]
	punpckldq	%xmm5, %xmm1            # xmm1 = xmm1[0],xmm5[0],xmm1[1],xmm5[1]
	paddd	%xmm4, %xmm3
	movl	%ecx, %edx
	addl	$-4, %ecx
	addl	%eax, %edx
	addl	$-4, %edx
	cmpl	$4, %edx
	jne	.LBB1_2
# %bb.3:
	negl	%ecx
	movd	%ecx, %xmm3
	pshufd	$0, %xmm3, %xmm3                # xmm3 = xmm3[0,0,0,0]
	por	.LCPI1_3(%rip), %xmm3
	movdqa	.LCPI1_4(%rip), %xmm4           # xmm4 = [2147483648,2147483648,2147483648,2147483648]
	pxor	%xmm4, %xmm0
	pxor	%xmm4, %xmm3
	pcmpgtd	%xmm0, %xmm3
	pand	%xmm3, %xmm2
	pandn	%xmm1, %xmm3
	por	%xmm2, %xmm3
	pshufd	$238, %xmm3, %xmm0              # xmm0 = xmm3[2,3,2,3]
	pshufd	$255, %xmm3, %xmm1              # xmm1 = xmm3[3,3,3,3]
	pshufd	$245, %xmm3, %xmm2              # xmm2 = xmm3[1,1,3,3]
	pmuludq	%xmm1, %xmm2
	pmuludq	%xmm3, %xmm0
	pmuludq	%xmm2, %xmm0
	movd	%xmm0, %esi
.LBB1_4:
	movl	$.L.str.1, %edi
	xorl	%eax, %eax
	callq	printf@PLT
	xorl	%eax, %eax
	addq	$16, %rsp
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc
                                        # -- End function
	.type	.L.str,@object                  # @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"%d"
	.size	.L.str, 3

	.type	.L.str.1,@object                # @.str.1
.L.str.1:
	.asciz	"%d\n"
	.size	.L.str.1, 4

	.ident	"Ubuntu clang version 14.0.0-1ubuntu1"
	.section	".note.GNU-stack","",@progbits
