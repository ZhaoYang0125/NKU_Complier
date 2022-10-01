
main_x86.o：     文件格式 elf64-x86-64


Disassembly of section .text:

0000000000000000 <func1>:
   0:	f3 0f 1e fa          	endbr64 
   4:	55                   	push   %rbp
   5:	48 89 e5             	mov    %rsp,%rbp
   8:	89 7d fc             	mov    %edi,-0x4(%rbp)
   b:	89 75 f8             	mov    %esi,-0x8(%rbp)
   e:	89 55 f4             	mov    %edx,-0xc(%rbp)
  11:	eb 0e                	jmp    21 <func1+0x21>
  13:	8b 45 f4             	mov    -0xc(%rbp),%eax
  16:	0f af 45 fc          	imul   -0x4(%rbp),%eax
  1a:	89 45 f4             	mov    %eax,-0xc(%rbp)
  1d:	83 45 fc 01          	addl   $0x1,-0x4(%rbp)
  21:	8b 45 fc             	mov    -0x4(%rbp),%eax
  24:	3b 45 f8             	cmp    -0x8(%rbp),%eax
  27:	7e ea                	jle    13 <func1+0x13>
  29:	8b 45 f4             	mov    -0xc(%rbp),%eax
  2c:	5d                   	pop    %rbp
  2d:	c3                   	ret    

000000000000002e <main>:
  2e:	f3 0f 1e fa          	endbr64 
  32:	55                   	push   %rbp
  33:	48 89 e5             	mov    %rsp,%rbp
  36:	48 83 ec 20          	sub    $0x20,%rsp
  3a:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
  41:	00 00 
  43:	48 89 45 f8          	mov    %rax,-0x8(%rbp)
  47:	31 c0                	xor    %eax,%eax
  49:	f3 0f 10 05 00 00 00 	movss  0x0(%rip),%xmm0        # 51 <main+0x23>
  50:	00 
  51:	f3 0f 11 45 ec       	movss  %xmm0,-0x14(%rbp)
  56:	48 8d 45 e8          	lea    -0x18(%rbp),%rax
  5a:	48 89 c6             	mov    %rax,%rsi
  5d:	48 8d 05 00 00 00 00 	lea    0x0(%rip),%rax        # 64 <main+0x36>
  64:	48 89 c7             	mov    %rax,%rdi
  67:	b8 00 00 00 00       	mov    $0x0,%eax
  6c:	e8 00 00 00 00       	call   71 <main+0x43>
  71:	c7 45 f0 02 00 00 00 	movl   $0x2,-0x10(%rbp)
  78:	c7 45 f4 01 00 00 00 	movl   $0x1,-0xc(%rbp)
  7f:	8b 4d e8             	mov    -0x18(%rbp),%ecx
  82:	8b 55 f4             	mov    -0xc(%rbp),%edx
  85:	8b 45 f0             	mov    -0x10(%rbp),%eax
  88:	89 ce                	mov    %ecx,%esi
  8a:	89 c7                	mov    %eax,%edi
  8c:	e8 00 00 00 00       	call   91 <main+0x63>
  91:	89 45 f4             	mov    %eax,-0xc(%rbp)
  94:	8b 45 f4             	mov    -0xc(%rbp),%eax
  97:	89 c6                	mov    %eax,%esi
  99:	48 8d 05 00 00 00 00 	lea    0x0(%rip),%rax        # a0 <main+0x72>
  a0:	48 89 c7             	mov    %rax,%rdi
  a3:	b8 00 00 00 00       	mov    $0x0,%eax
  a8:	e8 00 00 00 00       	call   ad <main+0x7f>
  ad:	b8 00 00 00 00       	mov    $0x0,%eax
  b2:	48 8b 55 f8          	mov    -0x8(%rbp),%rdx
  b6:	64 48 2b 14 25 28 00 	sub    %fs:0x28,%rdx
  bd:	00 00 
  bf:	74 05                	je     c6 <main+0x98>
  c1:	e8 00 00 00 00       	call   c6 <main+0x98>
  c6:	c9                   	leave  
  c7:	c3                   	ret    
