
main：     文件格式 elf64-x86-64


Disassembly of section .init:

0000000000001000 <_init>:
    1000:	f3 0f 1e fa          	endbr64 
    1004:	48 83 ec 08          	sub    $0x8,%rsp
    1008:	48 8b 05 d9 2f 00 00 	mov    0x2fd9(%rip),%rax        # 3fe8 <__gmon_start__@Base>
    100f:	48 85 c0             	test   %rax,%rax
    1012:	74 02                	je     1016 <_init+0x16>
    1014:	ff d0                	call   *%rax
    1016:	48 83 c4 08          	add    $0x8,%rsp
    101a:	c3                   	ret    

Disassembly of section .plt:

0000000000001020 <printf@plt-0x10>:
    1020:	ff 35 e2 2f 00 00    	push   0x2fe2(%rip)        # 4008 <_GLOBAL_OFFSET_TABLE_+0x8>
    1026:	ff 25 e4 2f 00 00    	jmp    *0x2fe4(%rip)        # 4010 <_GLOBAL_OFFSET_TABLE_+0x10>
    102c:	0f 1f 40 00          	nopl   0x0(%rax)

0000000000001030 <printf@plt>:
    1030:	ff 25 e2 2f 00 00    	jmp    *0x2fe2(%rip)        # 4018 <printf@GLIBC_2.2.5>
    1036:	68 00 00 00 00       	push   $0x0
    103b:	e9 e0 ff ff ff       	jmp    1020 <_init+0x20>

0000000000001040 <__isoc99_scanf@plt>:
    1040:	ff 25 da 2f 00 00    	jmp    *0x2fda(%rip)        # 4020 <__isoc99_scanf@GLIBC_2.7>
    1046:	68 01 00 00 00       	push   $0x1
    104b:	e9 d0 ff ff ff       	jmp    1020 <_init+0x20>

Disassembly of section .plt.got:

0000000000001050 <__cxa_finalize@plt>:
    1050:	ff 25 a2 2f 00 00    	jmp    *0x2fa2(%rip)        # 3ff8 <__cxa_finalize@GLIBC_2.2.5>
    1056:	66 90                	xchg   %ax,%ax

Disassembly of section .text:

0000000000001060 <_start>:
    1060:	f3 0f 1e fa          	endbr64 
    1064:	31 ed                	xor    %ebp,%ebp
    1066:	49 89 d1             	mov    %rdx,%r9
    1069:	5e                   	pop    %rsi
    106a:	48 89 e2             	mov    %rsp,%rdx
    106d:	48 83 e4 f0          	and    $0xfffffffffffffff0,%rsp
    1071:	50                   	push   %rax
    1072:	54                   	push   %rsp
    1073:	45 31 c0             	xor    %r8d,%r8d
    1076:	31 c9                	xor    %ecx,%ecx
    1078:	48 8d 3d 22 02 00 00 	lea    0x222(%rip),%rdi        # 12a1 <main>
    107f:	ff 15 53 2f 00 00    	call   *0x2f53(%rip)        # 3fd8 <__libc_start_main@GLIBC_2.34>
    1085:	f4                   	hlt    
    1086:	66 2e 0f 1f 84 00 00 	cs nopw 0x0(%rax,%rax,1)
    108d:	00 00 00 

0000000000001090 <deregister_tm_clones>:
    1090:	48 8d 3d a1 2f 00 00 	lea    0x2fa1(%rip),%rdi        # 4038 <__TMC_END__>
    1097:	48 8d 05 9a 2f 00 00 	lea    0x2f9a(%rip),%rax        # 4038 <__TMC_END__>
    109e:	48 39 f8             	cmp    %rdi,%rax
    10a1:	74 15                	je     10b8 <deregister_tm_clones+0x28>
    10a3:	48 8b 05 36 2f 00 00 	mov    0x2f36(%rip),%rax        # 3fe0 <_ITM_deregisterTMCloneTable@Base>
    10aa:	48 85 c0             	test   %rax,%rax
    10ad:	74 09                	je     10b8 <deregister_tm_clones+0x28>
    10af:	ff e0                	jmp    *%rax
    10b1:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
    10b8:	c3                   	ret    
    10b9:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

00000000000010c0 <register_tm_clones>:
    10c0:	48 8d 3d 71 2f 00 00 	lea    0x2f71(%rip),%rdi        # 4038 <__TMC_END__>
    10c7:	48 8d 35 6a 2f 00 00 	lea    0x2f6a(%rip),%rsi        # 4038 <__TMC_END__>
    10ce:	48 29 fe             	sub    %rdi,%rsi
    10d1:	48 89 f0             	mov    %rsi,%rax
    10d4:	48 c1 ee 3f          	shr    $0x3f,%rsi
    10d8:	48 c1 f8 03          	sar    $0x3,%rax
    10dc:	48 01 c6             	add    %rax,%rsi
    10df:	48 d1 fe             	sar    %rsi
    10e2:	74 14                	je     10f8 <register_tm_clones+0x38>
    10e4:	48 8b 05 05 2f 00 00 	mov    0x2f05(%rip),%rax        # 3ff0 <_ITM_registerTMCloneTable@Base>
    10eb:	48 85 c0             	test   %rax,%rax
    10ee:	74 08                	je     10f8 <register_tm_clones+0x38>
    10f0:	ff e0                	jmp    *%rax
    10f2:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)
    10f8:	c3                   	ret    
    10f9:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

0000000000001100 <__do_global_dtors_aux>:
    1100:	f3 0f 1e fa          	endbr64 
    1104:	80 3d 2d 2f 00 00 00 	cmpb   $0x0,0x2f2d(%rip)        # 4038 <__TMC_END__>
    110b:	75 2b                	jne    1138 <__do_global_dtors_aux+0x38>
    110d:	55                   	push   %rbp
    110e:	48 83 3d e2 2e 00 00 	cmpq   $0x0,0x2ee2(%rip)        # 3ff8 <__cxa_finalize@GLIBC_2.2.5>
    1115:	00 
    1116:	48 89 e5             	mov    %rsp,%rbp
    1119:	74 0c                	je     1127 <__do_global_dtors_aux+0x27>
    111b:	48 8b 3d 0e 2f 00 00 	mov    0x2f0e(%rip),%rdi        # 4030 <__dso_handle>
    1122:	e8 29 ff ff ff       	call   1050 <__cxa_finalize@plt>
    1127:	e8 64 ff ff ff       	call   1090 <deregister_tm_clones>
    112c:	c6 05 05 2f 00 00 01 	movb   $0x1,0x2f05(%rip)        # 4038 <__TMC_END__>
    1133:	5d                   	pop    %rbp
    1134:	c3                   	ret    
    1135:	0f 1f 00             	nopl   (%rax)
    1138:	c3                   	ret    
    1139:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

0000000000001140 <frame_dummy>:
    1140:	f3 0f 1e fa          	endbr64 
    1144:	e9 77 ff ff ff       	jmp    10c0 <register_tm_clones>
    1149:	0f 1f 00             	nopl   (%rax)

000000000000114c <func1>:
    114c:	48 83 ec 38          	sub    $0x38,%rsp
    1150:	89 7c 24 28          	mov    %edi,0x28(%rsp)
    1154:	89 74 24 2c          	mov    %esi,0x2c(%rsp)
    1158:	89 54 24 30          	mov    %edx,0x30(%rsp)
    115c:	39 f7                	cmp    %esi,%edi
    115e:	89 54 24 34          	mov    %edx,0x34(%rsp)
    1162:	0f 8f 30 01 00 00    	jg     1298 <func1+0x14c>
    1168:	8b 44 24 28          	mov    0x28(%rsp),%eax
    116c:	8b 4c 24 30          	mov    0x30(%rsp),%ecx
    1170:	8b 54 24 2c          	mov    0x2c(%rsp),%edx
    1174:	29 c2                	sub    %eax,%edx
    1176:	89 d6                	mov    %edx,%esi
    1178:	83 c6 04             	add    $0x4,%esi
    117b:	83 e6 fc             	and    $0xfffffffc,%esi
    117e:	89 74 24 dc          	mov    %esi,-0x24(%rsp)
    1182:	66 0f 6e c2          	movd   %edx,%xmm0
    1186:	66 0f 70 c0 00       	pshufd $0x0,%xmm0,%xmm0
    118b:	0f 29 44 24 e0       	movaps %xmm0,-0x20(%rsp)
    1190:	66 0f 6e c1          	movd   %ecx,%xmm0
    1194:	0f 28 0d 75 0e 00 00 	movaps 0xe75(%rip),%xmm1        # 2010 <_IO_stdin_used+0x10>
    119b:	f3 0f 10 c8          	movss  %xmm0,%xmm1
    119f:	66 0f 6e c0          	movd   %eax,%xmm0
    11a3:	66 0f 70 c0 00       	pshufd $0x0,%xmm0,%xmm0
    11a8:	0f 28 15 71 0e 00 00 	movaps 0xe71(%rip),%xmm2        # 2020 <_IO_stdin_used+0x20>
    11af:	66 0f fe c2          	paddd  %xmm2,%xmm0
    11b3:	31 c0                	xor    %eax,%eax
    11b5:	89 44 24 fc          	mov    %eax,-0x4(%rsp)
    11b9:	0f 29 0c 24          	movaps %xmm1,(%rsp)
    11bd:	0f 29 44 24 10       	movaps %xmm0,0x10(%rsp)
    11c2:	0f 28 44 24 10       	movaps 0x10(%rsp),%xmm0
    11c7:	0f 28 14 24          	movaps (%rsp),%xmm2
    11cb:	8b 44 24 fc          	mov    -0x4(%rsp),%eax
    11cf:	8b 4c 24 dc          	mov    -0x24(%rsp),%ecx
    11d3:	0f 29 54 24 80       	movaps %xmm2,-0x80(%rsp)
    11d8:	89 44 24 9c          	mov    %eax,-0x64(%rsp)
    11dc:	0f 28 ca             	movaps %xmm2,%xmm1
    11df:	66 0f f4 c8          	pmuludq %xmm0,%xmm1
    11e3:	66 0f 70 c9 e8       	pshufd $0xe8,%xmm1,%xmm1
    11e8:	66 0f 70 d8 f5       	pshufd $0xf5,%xmm0,%xmm3
    11ed:	66 0f 70 d2 f5       	pshufd $0xf5,%xmm2,%xmm2
    11f2:	66 0f f4 d3          	pmuludq %xmm3,%xmm2
    11f6:	66 0f 70 d2 e8       	pshufd $0xe8,%xmm2,%xmm2
    11fb:	66 0f 62 ca          	punpckldq %xmm2,%xmm1
    11ff:	0f 29 4c 24 a0       	movaps %xmm1,-0x60(%rsp)
    1204:	83 c0 04             	add    $0x4,%eax
    1207:	89 44 24 bc          	mov    %eax,-0x44(%rsp)
    120b:	0f 28 15 1e 0e 00 00 	movaps 0xe1e(%rip),%xmm2        # 2030 <_IO_stdin_used+0x30>
    1212:	66 0f fe c2          	paddd  %xmm2,%xmm0
    1216:	0f 29 44 24 c0       	movaps %xmm0,-0x40(%rsp)
    121b:	39 c8                	cmp    %ecx,%eax
    121d:	89 44 24 fc          	mov    %eax,-0x4(%rsp)
    1221:	0f 29 0c 24          	movaps %xmm1,(%rsp)
    1225:	0f 29 44 24 10       	movaps %xmm0,0x10(%rsp)
    122a:	75 96                	jne    11c2 <func1+0x76>
    122c:	0f 28 44 24 80       	movaps -0x80(%rsp),%xmm0
    1231:	0f 28 5c 24 a0       	movaps -0x60(%rsp),%xmm3
    1236:	0f 28 4c 24 e0       	movaps -0x20(%rsp),%xmm1
    123b:	8b 44 24 9c          	mov    -0x64(%rsp),%eax
    123f:	66 0f 6e d0          	movd   %eax,%xmm2
    1243:	66 0f 70 d2 00       	pshufd $0x0,%xmm2,%xmm2
    1248:	0f 28 25 d1 0d 00 00 	movaps 0xdd1(%rip),%xmm4        # 2020 <_IO_stdin_used+0x20>
    124f:	66 0f eb d4          	por    %xmm4,%xmm2
    1253:	0f 28 25 e6 0d 00 00 	movaps 0xde6(%rip),%xmm4        # 2040 <_IO_stdin_used+0x40>
    125a:	66 0f ef cc          	pxor   %xmm4,%xmm1
    125e:	66 0f ef d4          	pxor   %xmm4,%xmm2
    1262:	66 0f 66 d1          	pcmpgtd %xmm1,%xmm2
    1266:	0f 28 ca             	movaps %xmm2,%xmm1
    1269:	66 0f df cb          	pandn  %xmm3,%xmm1
    126d:	66 0f db c2          	pand   %xmm2,%xmm0
    1271:	66 0f eb c1          	por    %xmm1,%xmm0
    1275:	66 0f 70 d0 ee       	pshufd $0xee,%xmm0,%xmm2
    127a:	66 0f 70 d8 ff       	pshufd $0xff,%xmm0,%xmm3
    127f:	66 0f 70 c8 f5       	pshufd $0xf5,%xmm0,%xmm1
    1284:	66 0f f4 cb          	pmuludq %xmm3,%xmm1
    1288:	66 0f f4 c2          	pmuludq %xmm2,%xmm0
    128c:	66 0f f4 c1          	pmuludq %xmm1,%xmm0
    1290:	66 0f 7e c0          	movd   %xmm0,%eax
    1294:	89 44 24 34          	mov    %eax,0x34(%rsp)
    1298:	8b 44 24 34          	mov    0x34(%rsp),%eax
    129c:	48 83 c4 38          	add    $0x38,%rsp
    12a0:	c3                   	ret    

00000000000012a1 <main>:
    12a1:	48 81 ec b8 00 00 00 	sub    $0xb8,%rsp
    12a8:	48 8d 84 24 b4 00 00 	lea    0xb4(%rsp),%rax
    12af:	00 
    12b0:	48 8d 3d b9 0d 00 00 	lea    0xdb9(%rip),%rdi        # 2070 <_IO_stdin_used+0x70>
    12b7:	48 8d b4 24 b4 00 00 	lea    0xb4(%rsp),%rsi
    12be:	00 
    12bf:	b0 00                	mov    $0x0,%al
    12c1:	e8 7a fd ff ff       	call   1040 <__isoc99_scanf@plt>
    12c6:	8b 8c 24 b4 00 00 00 	mov    0xb4(%rsp),%ecx
    12cd:	89 8c 24 ac 00 00 00 	mov    %ecx,0xac(%rsp)
    12d4:	b8 01 00 00 00       	mov    $0x1,%eax
    12d9:	83 f9 02             	cmp    $0x2,%ecx
    12dc:	89 84 24 b0 00 00 00 	mov    %eax,0xb0(%rsp)
    12e3:	0f 8c 2f 01 00 00    	jl     1418 <main+0x177>
    12e9:	8b 84 24 ac 00 00 00 	mov    0xac(%rsp),%eax
    12f0:	89 c1                	mov    %eax,%ecx
    12f2:	83 c1 02             	add    $0x2,%ecx
    12f5:	83 e1 fc             	and    $0xfffffffc,%ecx
    12f8:	89 4c 24 5c          	mov    %ecx,0x5c(%rsp)
    12fc:	83 c0 fe             	add    $0xfffffffe,%eax
    12ff:	66 0f 6e c0          	movd   %eax,%xmm0
    1303:	66 0f 70 c0 00       	pshufd $0x0,%xmm0,%xmm0
    1308:	0f 29 44 24 60       	movaps %xmm0,0x60(%rsp)
    130d:	0f 28 05 3c 0d 00 00 	movaps 0xd3c(%rip),%xmm0        # 2050 <_IO_stdin_used+0x50>
    1314:	0f 28 0d 45 0d 00 00 	movaps 0xd45(%rip),%xmm1        # 2060 <_IO_stdin_used+0x60>
    131b:	31 c0                	xor    %eax,%eax
    131d:	89 44 24 7c          	mov    %eax,0x7c(%rsp)
    1321:	0f 29 8c 24 80 00 00 	movaps %xmm1,0x80(%rsp)
    1328:	00 
    1329:	0f 29 84 24 90 00 00 	movaps %xmm0,0x90(%rsp)
    1330:	00 
    1331:	eb 00                	jmp    1333 <main+0x92>
    1333:	0f 28 84 24 90 00 00 	movaps 0x90(%rsp),%xmm0
    133a:	00 
    133b:	0f 28 94 24 80 00 00 	movaps 0x80(%rsp),%xmm2
    1342:	00 
    1343:	8b 44 24 7c          	mov    0x7c(%rsp),%eax
    1347:	8b 4c 24 5c          	mov    0x5c(%rsp),%ecx
    134b:	0f 29 14 24          	movaps %xmm2,(%rsp)
    134f:	89 44 24 1c          	mov    %eax,0x1c(%rsp)
    1353:	0f 28 c8             	movaps %xmm0,%xmm1
    1356:	66 0f f4 ca          	pmuludq %xmm2,%xmm1
    135a:	66 0f 70 c9 e8       	pshufd $0xe8,%xmm1,%xmm1
    135f:	66 0f 70 da f5       	pshufd $0xf5,%xmm2,%xmm3
    1364:	66 0f 70 d0 f5       	pshufd $0xf5,%xmm0,%xmm2
    1369:	66 0f f4 d3          	pmuludq %xmm3,%xmm2
    136d:	66 0f 70 d2 e8       	pshufd $0xe8,%xmm2,%xmm2
    1372:	66 0f 62 ca          	punpckldq %xmm2,%xmm1
    1376:	0f 29 4c 24 20       	movaps %xmm1,0x20(%rsp)
    137b:	83 c0 04             	add    $0x4,%eax
    137e:	89 44 24 3c          	mov    %eax,0x3c(%rsp)
    1382:	0f 28 15 a7 0c 00 00 	movaps 0xca7(%rip),%xmm2        # 2030 <_IO_stdin_used+0x30>
    1389:	66 0f fe c2          	paddd  %xmm2,%xmm0
    138d:	0f 29 44 24 40       	movaps %xmm0,0x40(%rsp)
    1392:	39 c8                	cmp    %ecx,%eax
    1394:	89 44 24 7c          	mov    %eax,0x7c(%rsp)
    1398:	0f 29 8c 24 80 00 00 	movaps %xmm1,0x80(%rsp)
    139f:	00 
    13a0:	0f 29 84 24 90 00 00 	movaps %xmm0,0x90(%rsp)
    13a7:	00 
    13a8:	75 89                	jne    1333 <main+0x92>
    13aa:	0f 28 04 24          	movaps (%rsp),%xmm0
    13ae:	0f 28 5c 24 20       	movaps 0x20(%rsp),%xmm3
    13b3:	0f 28 4c 24 60       	movaps 0x60(%rsp),%xmm1
    13b8:	8b 44 24 1c          	mov    0x1c(%rsp),%eax
    13bc:	66 0f 6e d0          	movd   %eax,%xmm2
    13c0:	66 0f 70 d2 00       	pshufd $0x0,%xmm2,%xmm2
    13c5:	0f 28 25 54 0c 00 00 	movaps 0xc54(%rip),%xmm4        # 2020 <_IO_stdin_used+0x20>
    13cc:	66 0f eb d4          	por    %xmm4,%xmm2
    13d0:	0f 28 25 69 0c 00 00 	movaps 0xc69(%rip),%xmm4        # 2040 <_IO_stdin_used+0x40>
    13d7:	66 0f ef cc          	pxor   %xmm4,%xmm1
    13db:	66 0f ef d4          	pxor   %xmm4,%xmm2
    13df:	66 0f 66 d1          	pcmpgtd %xmm1,%xmm2
    13e3:	0f 28 ca             	movaps %xmm2,%xmm1
    13e6:	66 0f df cb          	pandn  %xmm3,%xmm1
    13ea:	66 0f db c2          	pand   %xmm2,%xmm0
    13ee:	66 0f eb c1          	por    %xmm1,%xmm0
    13f2:	66 0f 70 d0 ee       	pshufd $0xee,%xmm0,%xmm2
    13f7:	66 0f 70 d8 ff       	pshufd $0xff,%xmm0,%xmm3
    13fc:	66 0f 70 c8 f5       	pshufd $0xf5,%xmm0,%xmm1
    1401:	66 0f f4 cb          	pmuludq %xmm3,%xmm1
    1405:	66 0f f4 c2          	pmuludq %xmm2,%xmm0
    1409:	66 0f f4 c1          	pmuludq %xmm1,%xmm0
    140d:	66 0f 7e c0          	movd   %xmm0,%eax
    1411:	89 84 24 b0 00 00 00 	mov    %eax,0xb0(%rsp)
    1418:	8b b4 24 b0 00 00 00 	mov    0xb0(%rsp),%esi
    141f:	48 8d 3d 4d 0c 00 00 	lea    0xc4d(%rip),%rdi        # 2073 <_IO_stdin_used+0x73>
    1426:	b0 00                	mov    $0x0,%al
    1428:	e8 03 fc ff ff       	call   1030 <printf@plt>
    142d:	31 c0                	xor    %eax,%eax
    142f:	48 81 c4 b8 00 00 00 	add    $0xb8,%rsp
    1436:	c3                   	ret    

Disassembly of section .fini:

0000000000001438 <_fini>:
    1438:	f3 0f 1e fa          	endbr64 
    143c:	48 83 ec 08          	sub    $0x8,%rsp
    1440:	48 83 c4 08          	add    $0x8,%rsp
    1444:	c3                   	ret    
