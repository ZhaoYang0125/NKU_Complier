
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

0000000000001020 <.plt>:
    1020:	ff 35 8a 2f 00 00    	push   0x2f8a(%rip)        # 3fb0 <_GLOBAL_OFFSET_TABLE_+0x8>
    1026:	f2 ff 25 8b 2f 00 00 	bnd jmp *0x2f8b(%rip)        # 3fb8 <_GLOBAL_OFFSET_TABLE_+0x10>
    102d:	0f 1f 00             	nopl   (%rax)
    1030:	f3 0f 1e fa          	endbr64 
    1034:	68 00 00 00 00       	push   $0x0
    1039:	f2 e9 e1 ff ff ff    	bnd jmp 1020 <_init+0x20>
    103f:	90                   	nop
    1040:	f3 0f 1e fa          	endbr64 
    1044:	68 01 00 00 00       	push   $0x1
    1049:	f2 e9 d1 ff ff ff    	bnd jmp 1020 <_init+0x20>
    104f:	90                   	nop
    1050:	f3 0f 1e fa          	endbr64 
    1054:	68 02 00 00 00       	push   $0x2
    1059:	f2 e9 c1 ff ff ff    	bnd jmp 1020 <_init+0x20>
    105f:	90                   	nop

Disassembly of section .plt.got:

0000000000001060 <__cxa_finalize@plt>:
    1060:	f3 0f 1e fa          	endbr64 
    1064:	f2 ff 25 8d 2f 00 00 	bnd jmp *0x2f8d(%rip)        # 3ff8 <__cxa_finalize@GLIBC_2.2.5>
    106b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

Disassembly of section .plt.sec:

0000000000001070 <__stack_chk_fail@plt>:
    1070:	f3 0f 1e fa          	endbr64 
    1074:	f2 ff 25 45 2f 00 00 	bnd jmp *0x2f45(%rip)        # 3fc0 <__stack_chk_fail@GLIBC_2.4>
    107b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

0000000000001080 <printf@plt>:
    1080:	f3 0f 1e fa          	endbr64 
    1084:	f2 ff 25 3d 2f 00 00 	bnd jmp *0x2f3d(%rip)        # 3fc8 <printf@GLIBC_2.2.5>
    108b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

0000000000001090 <__isoc99_scanf@plt>:
    1090:	f3 0f 1e fa          	endbr64 
    1094:	f2 ff 25 35 2f 00 00 	bnd jmp *0x2f35(%rip)        # 3fd0 <__isoc99_scanf@GLIBC_2.7>
    109b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

Disassembly of section .text:

00000000000010a0 <_start>:
    10a0:	f3 0f 1e fa          	endbr64 
    10a4:	31 ed                	xor    %ebp,%ebp
    10a6:	49 89 d1             	mov    %rdx,%r9
    10a9:	5e                   	pop    %rsi
    10aa:	48 89 e2             	mov    %rsp,%rdx
    10ad:	48 83 e4 f0          	and    $0xfffffffffffffff0,%rsp
    10b1:	50                   	push   %rax
    10b2:	54                   	push   %rsp
    10b3:	45 31 c0             	xor    %r8d,%r8d
    10b6:	31 c9                	xor    %ecx,%ecx
    10b8:	48 8d 3d f8 00 00 00 	lea    0xf8(%rip),%rdi        # 11b7 <main>
    10bf:	ff 15 13 2f 00 00    	call   *0x2f13(%rip)        # 3fd8 <__libc_start_main@GLIBC_2.34>
    10c5:	f4                   	hlt    
    10c6:	66 2e 0f 1f 84 00 00 	cs nopw 0x0(%rax,%rax,1)
    10cd:	00 00 00 

00000000000010d0 <deregister_tm_clones>:
    10d0:	48 8d 3d 39 2f 00 00 	lea    0x2f39(%rip),%rdi        # 4010 <__TMC_END__>
    10d7:	48 8d 05 32 2f 00 00 	lea    0x2f32(%rip),%rax        # 4010 <__TMC_END__>
    10de:	48 39 f8             	cmp    %rdi,%rax
    10e1:	74 15                	je     10f8 <deregister_tm_clones+0x28>
    10e3:	48 8b 05 f6 2e 00 00 	mov    0x2ef6(%rip),%rax        # 3fe0 <_ITM_deregisterTMCloneTable@Base>
    10ea:	48 85 c0             	test   %rax,%rax
    10ed:	74 09                	je     10f8 <deregister_tm_clones+0x28>
    10ef:	ff e0                	jmp    *%rax
    10f1:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
    10f8:	c3                   	ret    
    10f9:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

0000000000001100 <register_tm_clones>:
    1100:	48 8d 3d 09 2f 00 00 	lea    0x2f09(%rip),%rdi        # 4010 <__TMC_END__>
    1107:	48 8d 35 02 2f 00 00 	lea    0x2f02(%rip),%rsi        # 4010 <__TMC_END__>
    110e:	48 29 fe             	sub    %rdi,%rsi
    1111:	48 89 f0             	mov    %rsi,%rax
    1114:	48 c1 ee 3f          	shr    $0x3f,%rsi
    1118:	48 c1 f8 03          	sar    $0x3,%rax
    111c:	48 01 c6             	add    %rax,%rsi
    111f:	48 d1 fe             	sar    %rsi
    1122:	74 14                	je     1138 <register_tm_clones+0x38>
    1124:	48 8b 05 c5 2e 00 00 	mov    0x2ec5(%rip),%rax        # 3ff0 <_ITM_registerTMCloneTable@Base>
    112b:	48 85 c0             	test   %rax,%rax
    112e:	74 08                	je     1138 <register_tm_clones+0x38>
    1130:	ff e0                	jmp    *%rax
    1132:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)
    1138:	c3                   	ret    
    1139:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

0000000000001140 <__do_global_dtors_aux>:
    1140:	f3 0f 1e fa          	endbr64 
    1144:	80 3d c5 2e 00 00 00 	cmpb   $0x0,0x2ec5(%rip)        # 4010 <__TMC_END__>
    114b:	75 2b                	jne    1178 <__do_global_dtors_aux+0x38>
    114d:	55                   	push   %rbp
    114e:	48 83 3d a2 2e 00 00 	cmpq   $0x0,0x2ea2(%rip)        # 3ff8 <__cxa_finalize@GLIBC_2.2.5>
    1155:	00 
    1156:	48 89 e5             	mov    %rsp,%rbp
    1159:	74 0c                	je     1167 <__do_global_dtors_aux+0x27>
    115b:	48 8b 3d a6 2e 00 00 	mov    0x2ea6(%rip),%rdi        # 4008 <__dso_handle>
    1162:	e8 f9 fe ff ff       	call   1060 <__cxa_finalize@plt>
    1167:	e8 64 ff ff ff       	call   10d0 <deregister_tm_clones>
    116c:	c6 05 9d 2e 00 00 01 	movb   $0x1,0x2e9d(%rip)        # 4010 <__TMC_END__>
    1173:	5d                   	pop    %rbp
    1174:	c3                   	ret    
    1175:	0f 1f 00             	nopl   (%rax)
    1178:	c3                   	ret    
    1179:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

0000000000001180 <frame_dummy>:
    1180:	f3 0f 1e fa          	endbr64 
    1184:	e9 77 ff ff ff       	jmp    1100 <register_tm_clones>

0000000000001189 <func1>:
    1189:	f3 0f 1e fa          	endbr64 
    118d:	55                   	push   %rbp
    118e:	48 89 e5             	mov    %rsp,%rbp
    1191:	89 7d fc             	mov    %edi,-0x4(%rbp)
    1194:	89 75 f8             	mov    %esi,-0x8(%rbp)
    1197:	89 55 f4             	mov    %edx,-0xc(%rbp)
    119a:	eb 0e                	jmp    11aa <func1+0x21>
    119c:	8b 45 f4             	mov    -0xc(%rbp),%eax
    119f:	0f af 45 fc          	imul   -0x4(%rbp),%eax
    11a3:	89 45 f4             	mov    %eax,-0xc(%rbp)
    11a6:	83 45 fc 01          	addl   $0x1,-0x4(%rbp)
    11aa:	8b 45 fc             	mov    -0x4(%rbp),%eax
    11ad:	3b 45 f8             	cmp    -0x8(%rbp),%eax
    11b0:	7e ea                	jle    119c <func1+0x13>
    11b2:	8b 45 f4             	mov    -0xc(%rbp),%eax
    11b5:	5d                   	pop    %rbp
    11b6:	c3                   	ret    

00000000000011b7 <main>:
    11b7:	f3 0f 1e fa          	endbr64 
    11bb:	55                   	push   %rbp
    11bc:	48 89 e5             	mov    %rsp,%rbp
    11bf:	48 83 ec 20          	sub    $0x20,%rsp
    11c3:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
    11ca:	00 00 
    11cc:	48 89 45 f8          	mov    %rax,-0x8(%rbp)
    11d0:	31 c0                	xor    %eax,%eax
    11d2:	f3 0f 10 05 32 0e 00 	movss  0xe32(%rip),%xmm0        # 200c <_IO_stdin_used+0xc>
    11d9:	00 
    11da:	f3 0f 11 45 ec       	movss  %xmm0,-0x14(%rbp)
    11df:	48 8d 45 e8          	lea    -0x18(%rbp),%rax
    11e3:	48 89 c6             	mov    %rax,%rsi
    11e6:	48 8d 05 17 0e 00 00 	lea    0xe17(%rip),%rax        # 2004 <_IO_stdin_used+0x4>
    11ed:	48 89 c7             	mov    %rax,%rdi
    11f0:	b8 00 00 00 00       	mov    $0x0,%eax
    11f5:	e8 96 fe ff ff       	call   1090 <__isoc99_scanf@plt>
    11fa:	c7 45 f0 02 00 00 00 	movl   $0x2,-0x10(%rbp)
    1201:	c7 45 f4 01 00 00 00 	movl   $0x1,-0xc(%rbp)
    1208:	8b 4d e8             	mov    -0x18(%rbp),%ecx
    120b:	8b 55 f4             	mov    -0xc(%rbp),%edx
    120e:	8b 45 f0             	mov    -0x10(%rbp),%eax
    1211:	89 ce                	mov    %ecx,%esi
    1213:	89 c7                	mov    %eax,%edi
    1215:	e8 6f ff ff ff       	call   1189 <func1>
    121a:	89 45 f4             	mov    %eax,-0xc(%rbp)
    121d:	8b 45 f4             	mov    -0xc(%rbp),%eax
    1220:	89 c6                	mov    %eax,%esi
    1222:	48 8d 05 de 0d 00 00 	lea    0xdde(%rip),%rax        # 2007 <_IO_stdin_used+0x7>
    1229:	48 89 c7             	mov    %rax,%rdi
    122c:	b8 00 00 00 00       	mov    $0x0,%eax
    1231:	e8 4a fe ff ff       	call   1080 <printf@plt>
    1236:	b8 00 00 00 00       	mov    $0x0,%eax
    123b:	48 8b 55 f8          	mov    -0x8(%rbp),%rdx
    123f:	64 48 2b 14 25 28 00 	sub    %fs:0x28,%rdx
    1246:	00 00 
    1248:	74 05                	je     124f <main+0x98>
    124a:	e8 21 fe ff ff       	call   1070 <__stack_chk_fail@plt>
    124f:	c9                   	leave  
    1250:	c3                   	ret    

Disassembly of section .fini:

0000000000001254 <_fini>:
    1254:	f3 0f 1e fa          	endbr64 
    1258:	48 83 ec 08          	sub    $0x8,%rsp
    125c:	48 83 c4 08          	add    $0x8,%rsp
    1260:	c3                   	ret    
