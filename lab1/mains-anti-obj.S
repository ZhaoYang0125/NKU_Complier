
mains.o：     文件格式 elf64-x86-64


Disassembly of section .text:

0000000000000000 <func1>:
   0:	89 d0                	mov    %edx,%eax
   2:	39 f7                	cmp    %esi,%edi
   4:	0f 8f c6 00 00 00    	jg     d0 <func1+0xd0>
   a:	29 fe                	sub    %edi,%esi
   c:	8d 4e 04             	lea    0x4(%rsi),%ecx
   f:	83 e1 fc             	and    $0xfffffffc,%ecx
  12:	66 0f 6e c6          	movd   %esi,%xmm0
  16:	66 0f 70 c0 00       	pshufd $0x0,%xmm0,%xmm0
  1b:	66 0f 6e d0          	movd   %eax,%xmm2
  1f:	0f 28 0d 00 00 00 00 	movaps 0x0(%rip),%xmm1        # 26 <func1+0x26>
  26:	f3 0f 10 ca          	movss  %xmm2,%xmm1
  2a:	66 0f 6e d7          	movd   %edi,%xmm2
  2e:	66 0f 70 da 00       	pshufd $0x0,%xmm2,%xmm3
  33:	66 0f fe 1d 00 00 00 	paddd  0x0(%rip),%xmm3        # 3b <func1+0x3b>
  3a:	00 
  3b:	b8 04 00 00 00       	mov    $0x4,%eax
  40:	66 0f 6f 25 00 00 00 	movdqa 0x0(%rip),%xmm4        # 48 <func1+0x48>
  47:	00 
  48:	0f 28 d1             	movaps %xmm1,%xmm2
  4b:	66 0f f4 cb          	pmuludq %xmm3,%xmm1
  4f:	66 0f 70 c9 e8       	pshufd $0xe8,%xmm1,%xmm1
  54:	66 0f 70 eb f5       	pshufd $0xf5,%xmm3,%xmm5
  59:	66 0f 70 f2 f5       	pshufd $0xf5,%xmm2,%xmm6
  5e:	66 0f f4 f5          	pmuludq %xmm5,%xmm6
  62:	66 0f 70 ee e8       	pshufd $0xe8,%xmm6,%xmm5
  67:	66 0f 62 cd          	punpckldq %xmm5,%xmm1
  6b:	66 0f fe dc          	paddd  %xmm4,%xmm3
  6f:	89 c2                	mov    %eax,%edx
  71:	83 c0 fc             	add    $0xfffffffc,%eax
  74:	01 ca                	add    %ecx,%edx
  76:	83 c2 fc             	add    $0xfffffffc,%edx
  79:	83 fa 04             	cmp    $0x4,%edx
  7c:	75 ca                	jne    48 <func1+0x48>
  7e:	f7 d8                	neg    %eax
  80:	66 0f 6e d8          	movd   %eax,%xmm3
  84:	66 0f 70 db 00       	pshufd $0x0,%xmm3,%xmm3
  89:	66 0f eb 1d 00 00 00 	por    0x0(%rip),%xmm3        # 91 <func1+0x91>
  90:	00 
  91:	66 0f 6f 25 00 00 00 	movdqa 0x0(%rip),%xmm4        # 99 <func1+0x99>
  98:	00 
  99:	66 0f ef c4          	pxor   %xmm4,%xmm0
  9d:	66 0f ef dc          	pxor   %xmm4,%xmm3
  a1:	66 0f 66 d8          	pcmpgtd %xmm0,%xmm3
  a5:	66 0f db d3          	pand   %xmm3,%xmm2
  a9:	66 0f df d9          	pandn  %xmm1,%xmm3
  ad:	66 0f eb da          	por    %xmm2,%xmm3
  b1:	66 0f 70 c3 ee       	pshufd $0xee,%xmm3,%xmm0
  b6:	66 0f 70 cb ff       	pshufd $0xff,%xmm3,%xmm1
  bb:	66 0f 70 d3 f5       	pshufd $0xf5,%xmm3,%xmm2
  c0:	66 0f f4 d1          	pmuludq %xmm1,%xmm2
  c4:	66 0f f4 c3          	pmuludq %xmm3,%xmm0
  c8:	66 0f f4 c2          	pmuludq %xmm2,%xmm0
  cc:	66 0f 7e c0          	movd   %xmm0,%eax
  d0:	c3                   	ret    

00000000000000d1 <main>:
  d1:	53                   	push   %rbx
  d2:	48 83 ec 10          	sub    $0x10,%rsp
  d6:	48 8d 5c 24 0c       	lea    0xc(%rsp),%rbx
  db:	bf 00 00 00 00       	mov    $0x0,%edi
  e0:	48 89 de             	mov    %rbx,%rsi
  e3:	31 c0                	xor    %eax,%eax
  e5:	e8 00 00 00 00       	call   ea <main+0x19>
  ea:	8b 0b                	mov    (%rbx),%ecx
  ec:	be 01 00 00 00       	mov    $0x1,%esi
  f1:	83 f9 02             	cmp    $0x2,%ecx
  f4:	0f 8c bc 00 00 00    	jl     1b6 <main+0xe5>
  fa:	8d 41 02             	lea    0x2(%rcx),%eax
  fd:	83 e0 fc             	and    $0xfffffffc,%eax
 100:	83 c1 fe             	add    $0xfffffffe,%ecx
 103:	66 0f 6e c1          	movd   %ecx,%xmm0
 107:	66 0f 70 c0 00       	pshufd $0x0,%xmm0,%xmm0
 10c:	66 0f 6f 1d 00 00 00 	movdqa 0x0(%rip),%xmm3        # 114 <main+0x43>
 113:	00 
 114:	66 0f 6f 0d 00 00 00 	movdqa 0x0(%rip),%xmm1        # 11c <main+0x4b>
 11b:	00 
 11c:	b9 04 00 00 00       	mov    $0x4,%ecx
 121:	66 0f 6f 25 00 00 00 	movdqa 0x0(%rip),%xmm4        # 129 <main+0x58>
 128:	00 
 129:	66 0f 6f d1          	movdqa %xmm1,%xmm2
 12d:	66 0f 6f cb          	movdqa %xmm3,%xmm1
 131:	66 0f f4 ca          	pmuludq %xmm2,%xmm1
 135:	66 0f 70 c9 e8       	pshufd $0xe8,%xmm1,%xmm1
 13a:	66 0f 70 ea f5       	pshufd $0xf5,%xmm2,%xmm5
 13f:	66 0f 70 f3 f5       	pshufd $0xf5,%xmm3,%xmm6
 144:	66 0f f4 f5          	pmuludq %xmm5,%xmm6
 148:	66 0f 70 ee e8       	pshufd $0xe8,%xmm6,%xmm5
 14d:	66 0f 62 cd          	punpckldq %xmm5,%xmm1
 151:	66 0f fe dc          	paddd  %xmm4,%xmm3
 155:	89 ca                	mov    %ecx,%edx
 157:	83 c1 fc             	add    $0xfffffffc,%ecx
 15a:	01 c2                	add    %eax,%edx
 15c:	83 c2 fc             	add    $0xfffffffc,%edx
 15f:	83 fa 04             	cmp    $0x4,%edx
 162:	75 c5                	jne    129 <main+0x58>
 164:	f7 d9                	neg    %ecx
 166:	66 0f 6e d9          	movd   %ecx,%xmm3
 16a:	66 0f 70 db 00       	pshufd $0x0,%xmm3,%xmm3
 16f:	66 0f eb 1d 00 00 00 	por    0x0(%rip),%xmm3        # 177 <main+0xa6>
 176:	00 
 177:	66 0f 6f 25 00 00 00 	movdqa 0x0(%rip),%xmm4        # 17f <main+0xae>
 17e:	00 
 17f:	66 0f ef c4          	pxor   %xmm4,%xmm0
 183:	66 0f ef dc          	pxor   %xmm4,%xmm3
 187:	66 0f 66 d8          	pcmpgtd %xmm0,%xmm3
 18b:	66 0f db d3          	pand   %xmm3,%xmm2
 18f:	66 0f df d9          	pandn  %xmm1,%xmm3
 193:	66 0f eb da          	por    %xmm2,%xmm3
 197:	66 0f 70 c3 ee       	pshufd $0xee,%xmm3,%xmm0
 19c:	66 0f 70 cb ff       	pshufd $0xff,%xmm3,%xmm1
 1a1:	66 0f 70 d3 f5       	pshufd $0xf5,%xmm3,%xmm2
 1a6:	66 0f f4 d1          	pmuludq %xmm1,%xmm2
 1aa:	66 0f f4 c3          	pmuludq %xmm3,%xmm0
 1ae:	66 0f f4 c2          	pmuludq %xmm2,%xmm0
 1b2:	66 0f 7e c6          	movd   %xmm0,%esi
 1b6:	bf 00 00 00 00       	mov    $0x0,%edi
 1bb:	31 c0                	xor    %eax,%eax
 1bd:	e8 00 00 00 00       	call   1c2 <main+0xf1>
 1c2:	31 c0                	xor    %eax,%eax
 1c4:	48 83 c4 10          	add    $0x10,%rsp
 1c8:	5b                   	pop    %rbx
 1c9:	c3                   	ret    
