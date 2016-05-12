	.file	"ctattack.c"
	.intel_syntax noprefix
	.text
.Ltext0:
	.globl	timestamp_weak
	.type	timestamp_weak, @function
timestamp_weak:
.LFB2:
	.file 1 "src/ctattack.c"
	.loc 1 8 0
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	.loc 1 11 0
#APP
# 11 "src/ctattack.c" 1
	.byte 15;.byte 49
# 0 "" 2
#NO_APP
	mov	DWORD PTR [rbp-8], eax
	mov	DWORD PTR [rbp-4], edx
	.loc 1 12 0
	mov	eax, DWORD PTR [rbp-8]
	.loc 1 13 0
	pop	rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	timestamp_weak, .-timestamp_weak
	.globl	timestamp
	.type	timestamp, @function
timestamp:
.LFB3:
	.loc 1 15 0
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	push	rbx
	.cfi_offset 3, -24
	.loc 1 21 0
#APP
# 21 "src/ctattack.c" 1
	CPUID
	RDTSC
	mov %edx, edi
	mov %eax, esi
	CPUID
	
# 0 "" 2
#NO_APP
	mov	DWORD PTR [rbp-16], edi
	mov	DWORD PTR [rbp-12], esi
	.loc 1 26 0
	mov	eax, DWORD PTR [rbp-12]
	.loc 1 27 0
	pop	rbx
	pop	rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	timestamp, .-timestamp
	.globl	timestamp_start
	.type	timestamp_start, @function
timestamp_start:
.LFB4:
	.loc 1 29 0
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	push	rbx
	.cfi_offset 3, -24
	.loc 1 35 0
#APP
# 35 "src/ctattack.c" 1
	.align 16
	CPUID
	CPUID
	CPUID
	RDTSCP
	mov %edx, edi
	mov %eax, esi
	
# 0 "" 2
#NO_APP
	mov	DWORD PTR [rbp-16], edi
	mov	DWORD PTR [rbp-12], esi
	.loc 1 42 0
	mov	eax, DWORD PTR [rbp-16]
	sal	rax, 32
	mov	rdx, rax
	mov	eax, DWORD PTR [rbp-12]
	or	rax, rdx
	.loc 1 43 0
	pop	rbx
	pop	rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE4:
	.size	timestamp_start, .-timestamp_start
	.globl	timestamp_stop
	.type	timestamp_stop, @function
timestamp_stop:
.LFB5:
	.loc 1 45 0
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	push	rbx
	.cfi_offset 3, -24
	.loc 1 51 0
#APP
# 51 "src/ctattack.c" 1
	RDTSCP
	mov %edx, edi
	mov %eax, esi
	CPUID
	
# 0 "" 2
#NO_APP
	mov	DWORD PTR [rbp-16], edi
	mov	DWORD PTR [rbp-12], esi
	.loc 1 55 0
	mov	eax, DWORD PTR [rbp-16]
	sal	rax, 32
	mov	rdx, rax
	mov	eax, DWORD PTR [rbp-12]
	or	rax, rdx
	.loc 1 56 0
	pop	rbx
	pop	rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5:
	.size	timestamp_stop, .-timestamp_stop
	.globl	ptr2bin
	.type	ptr2bin, @function
ptr2bin:
.LFB6:
	.loc 1 62 0
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	mov	QWORD PTR [rbp-24], rdi
	mov	QWORD PTR [rbp-32], rsi
	.loc 1 63 0
	mov	rax, QWORD PTR [rbp-24]
	mov	QWORD PTR [rbp-8], rax
.LBB2:
	.loc 1 64 0
	mov	DWORD PTR [rbp-12], 0
	jmp	.L10
.L13:
	.loc 1 65 0
	mov	rax, QWORD PTR [rbp-8]
	and	eax, 1
	test	rax, rax
	je	.L11
	.loc 1 65 0 is_stmt 0 discriminator 1
	mov	eax, DWORD PTR [rbp-12]
	movsx	rdx, eax
	mov	rax, QWORD PTR [rbp-32]
	add	rax, rdx
	mov	BYTE PTR [rax], 49
	jmp	.L12
.L11:
	.loc 1 65 0 discriminator 2
	mov	eax, DWORD PTR [rbp-12]
	movsx	rdx, eax
	mov	rax, QWORD PTR [rbp-32]
	add	rax, rdx
	mov	BYTE PTR [rax], 48
.L12:
	.loc 1 66 0 is_stmt 1 discriminator 2
	shr	QWORD PTR [rbp-8]
	.loc 1 64 0 discriminator 2
	add	DWORD PTR [rbp-12], 1
.L10:
	.loc 1 64 0 is_stmt 0 discriminator 1
	cmp	DWORD PTR [rbp-12], 63
	jle	.L13
.LBE2:
	.loc 1 68 0 is_stmt 1
	mov	rax, QWORD PTR [rbp-32]
	add	rax, 64
	mov	BYTE PTR [rax], 0
	.loc 1 69 0
	nop
	pop	rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	ptr2bin, .-ptr2bin
	.globl	printPtr2bin
	.type	printPtr2bin, @function
printPtr2bin:
.LFB7:
	.loc 1 71 0
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 112
	mov	QWORD PTR [rbp-104], rdi
	.loc 1 71 0
	mov	rax, QWORD PTR fs:40
	mov	QWORD PTR [rbp-8], rax
	xor	eax, eax
	.loc 1 73 0
	lea	rdx, [rbp-80]
	mov	rax, QWORD PTR [rbp-104]
	mov	rsi, rdx
	mov	rdi, rax
	call	ptr2bin
.LBB3:
	.loc 1 74 0
	mov	DWORD PTR [rbp-88], 0
	jmp	.L15
.L18:
.LBB4:
	.loc 1 75 0
	mov	DWORD PTR [rbp-84], 0
	jmp	.L16
.L17:
	.loc 1 76 0 discriminator 3
	mov	edx, DWORD PTR [rbp-88]
	mov	eax, 0
	sub	eax, edx
	sal	eax, 3
	sub	eax, DWORD PTR [rbp-84]
	add	eax, 63
	cdqe
	movzx	eax, BYTE PTR [rbp-80+rax]
	movsx	eax, al
	mov	edi, eax
	call	putchar
	.loc 1 75 0 discriminator 3
	add	DWORD PTR [rbp-84], 1
.L16:
	.loc 1 75 0 is_stmt 0 discriminator 1
	cmp	DWORD PTR [rbp-84], 7
	jle	.L17
.LBE4:
	.loc 1 78 0 is_stmt 1 discriminator 2
	mov	edi, 32
	call	putchar
	.loc 1 74 0 discriminator 2
	add	DWORD PTR [rbp-88], 1
.L15:
	.loc 1 74 0 is_stmt 0 discriminator 1
	cmp	DWORD PTR [rbp-88], 7
	jle	.L18
.LBE3:
	.loc 1 80 0 is_stmt 1
	mov	edi, 10
	call	putchar
	.loc 1 81 0
	nop
	mov	rax, QWORD PTR [rbp-8]
	xor	rax, QWORD PTR fs:40
	je	.L19
	call	__stack_chk_fail
.L19:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	printPtr2bin, .-printPtr2bin
	.type	rand_int, @function
rand_int:
.LFB8:
	.loc 1 94 0
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 32
	mov	DWORD PTR [rbp-20], edi
	.loc 1 95 0
	mov	eax, 2147483647
	cdq
	idiv	DWORD PTR [rbp-20]
	mov	eax, edx
	mov	edx, 2147483647
	sub	edx, eax
	mov	eax, edx
	mov	DWORD PTR [rbp-8], eax
.L21:
	.loc 1 98 0 discriminator 1
	call	rand
	mov	DWORD PTR [rbp-4], eax
	.loc 1 100 0 discriminator 1
	mov	eax, DWORD PTR [rbp-4]
	cmp	eax, DWORD PTR [rbp-8]
	jge	.L21
	.loc 1 101 0
	mov	eax, DWORD PTR [rbp-4]
	cdq
	idiv	DWORD PTR [rbp-20]
	mov	eax, edx
	.loc 1 102 0
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	rand_int, .-rand_int
	.globl	sattolo_shuffle
	.type	sattolo_shuffle, @function
sattolo_shuffle:
.LFB9:
	.loc 1 106 0
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 32
	mov	QWORD PTR [rbp-24], rdi
	mov	DWORD PTR [rbp-28], esi
	.loc 1 109 0
	mov	eax, DWORD PTR [rbp-28]
	sub	eax, 1
	mov	DWORD PTR [rbp-16], eax
	jmp	.L24
.L25:
	.loc 1 110 0 discriminator 3
	mov	eax, DWORD PTR [rbp-16]
	add	eax, 1
	mov	edi, eax
	call	rand_int
	mov	DWORD PTR [rbp-12], eax
	.loc 1 111 0 discriminator 3
	mov	eax, DWORD PTR [rbp-12]
	cdqe
	lea	rdx, [0+rax*8]
	mov	rax, QWORD PTR [rbp-24]
	add	rax, rdx
	mov	rax, QWORD PTR [rax]
	mov	QWORD PTR [rbp-8], rax
	.loc 1 112 0 discriminator 3
	mov	eax, DWORD PTR [rbp-12]
	cdqe
	lea	rdx, [0+rax*8]
	mov	rax, QWORD PTR [rbp-24]
	add	rdx, rax
	mov	eax, DWORD PTR [rbp-16]
	cdqe
	lea	rcx, [0+rax*8]
	mov	rax, QWORD PTR [rbp-24]
	add	rax, rcx
	mov	rax, QWORD PTR [rax]
	mov	QWORD PTR [rdx], rax
	.loc 1 113 0 discriminator 3
	mov	eax, DWORD PTR [rbp-16]
	cdqe
	lea	rdx, [0+rax*8]
	mov	rax, QWORD PTR [rbp-24]
	add	rdx, rax
	mov	rax, QWORD PTR [rbp-8]
	mov	QWORD PTR [rdx], rax
	.loc 1 109 0 discriminator 3
	sub	DWORD PTR [rbp-16], 1
.L24:
	.loc 1 109 0 is_stmt 0 discriminator 1
	cmp	DWORD PTR [rbp-16], 0
	jg	.L25
	.loc 1 116 0 is_stmt 1
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE9:
	.size	sattolo_shuffle, .-sattolo_shuffle
.Letext0:
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.long	0x323
	.value	0x4
	.long	.Ldebug_abbrev0
	.byte	0x8
	.uleb128 0x1
	.long	.LASF24
	.byte	0xc
	.long	.LASF25
	.long	.LASF26
	.quad	.Ltext0
	.quad	.Letext0-.Ltext0
	.long	.Ldebug_line0
	.uleb128 0x2
	.byte	0x8
	.byte	0x7
	.long	.LASF0
	.uleb128 0x2
	.byte	0x1
	.byte	0x8
	.long	.LASF1
	.uleb128 0x2
	.byte	0x2
	.byte	0x7
	.long	.LASF2
	.uleb128 0x2
	.byte	0x4
	.byte	0x7
	.long	.LASF3
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.long	.LASF4
	.uleb128 0x2
	.byte	0x2
	.byte	0x5
	.long	.LASF5
	.uleb128 0x3
	.byte	0x4
	.byte	0x5
	.string	"int"
	.uleb128 0x2
	.byte	0x8
	.byte	0x5
	.long	.LASF6
	.uleb128 0x2
	.byte	0x8
	.byte	0x7
	.long	.LASF7
	.uleb128 0x4
	.byte	0x8
	.uleb128 0x5
	.byte	0x8
	.long	0x74
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.long	.LASF8
	.uleb128 0x2
	.byte	0x8
	.byte	0x5
	.long	.LASF9
	.uleb128 0x2
	.byte	0x8
	.byte	0x7
	.long	.LASF10
	.uleb128 0x6
	.long	.LASF12
	.byte	0x1
	.byte	0x8
	.long	0x42
	.quad	.LFB2
	.quad	.LFE2-.LFB2
	.uleb128 0x1
	.byte	0x9c
	.long	0xc7
	.uleb128 0x7
	.long	.LASF11
	.byte	0x1
	.byte	0x9
	.long	0x42
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x8
	.string	"top"
	.byte	0x1
	.byte	0xa
	.long	0x42
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.byte	0
	.uleb128 0x6
	.long	.LASF13
	.byte	0x1
	.byte	0xf
	.long	0x42
	.quad	.LFB3
	.quad	.LFE3-.LFB3
	.uleb128 0x1
	.byte	0x9c
	.long	0x105
	.uleb128 0x7
	.long	.LASF14
	.byte	0x1
	.byte	0x12
	.long	0x42
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0x7
	.long	.LASF15
	.byte	0x1
	.byte	0x13
	.long	0x42
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.byte	0
	.uleb128 0x6
	.long	.LASF16
	.byte	0x1
	.byte	0x1d
	.long	0x2d
	.quad	.LFB4
	.quad	.LFE4-.LFB4
	.uleb128 0x1
	.byte	0x9c
	.long	0x143
	.uleb128 0x7
	.long	.LASF14
	.byte	0x1
	.byte	0x20
	.long	0x42
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0x7
	.long	.LASF15
	.byte	0x1
	.byte	0x21
	.long	0x42
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.byte	0
	.uleb128 0x6
	.long	.LASF17
	.byte	0x1
	.byte	0x2d
	.long	0x2d
	.quad	.LFB5
	.quad	.LFE5-.LFB5
	.uleb128 0x1
	.byte	0x9c
	.long	0x181
	.uleb128 0x7
	.long	.LASF14
	.byte	0x1
	.byte	0x30
	.long	0x42
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0x7
	.long	.LASF15
	.byte	0x1
	.byte	0x31
	.long	0x42
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.byte	0
	.uleb128 0x9
	.long	.LASF19
	.byte	0x1
	.byte	0x3e
	.quad	.LFB6
	.quad	.LFE6-.LFB6
	.uleb128 0x1
	.byte	0x9c
	.long	0x1e7
	.uleb128 0xa
	.string	"ptr"
	.byte	0x1
	.byte	0x3e
	.long	0x6c
	.uleb128 0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0xa
	.string	"bin"
	.byte	0x1
	.byte	0x3e
	.long	0x6e
	.uleb128 0x2
	.byte	0x91
	.sleb128 -48
	.uleb128 0x7
	.long	.LASF18
	.byte	0x1
	.byte	0x3f
	.long	0x2d
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0xb
	.quad	.LBB2
	.quad	.LBE2-.LBB2
	.uleb128 0x8
	.string	"i"
	.byte	0x1
	.byte	0x40
	.long	0x57
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.byte	0
	.byte	0
	.uleb128 0xc
	.long	.LASF20
	.byte	0x1
	.byte	0x47
	.quad	.LFB7
	.quad	.LFE7-.LFB7
	.uleb128 0x1
	.byte	0x9c
	.long	0x261
	.uleb128 0xa
	.string	"ptr"
	.byte	0x1
	.byte	0x47
	.long	0x6c
	.uleb128 0x3
	.byte	0x91
	.sleb128 -120
	.uleb128 0x8
	.string	"bin"
	.byte	0x1
	.byte	0x48
	.long	0x261
	.uleb128 0x3
	.byte	0x91
	.sleb128 -96
	.uleb128 0xb
	.quad	.LBB3
	.quad	.LBE3-.LBB3
	.uleb128 0x8
	.string	"i"
	.byte	0x1
	.byte	0x4a
	.long	0x57
	.uleb128 0x3
	.byte	0x91
	.sleb128 -104
	.uleb128 0xb
	.quad	.LBB4
	.quad	.LBE4-.LBB4
	.uleb128 0x8
	.string	"j"
	.byte	0x1
	.byte	0x4b
	.long	0x57
	.uleb128 0x3
	.byte	0x91
	.sleb128 -100
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0xd
	.long	0x74
	.long	0x271
	.uleb128 0xe
	.long	0x65
	.byte	0x40
	.byte	0
	.uleb128 0xf
	.long	.LASF27
	.byte	0x1
	.byte	0x5e
	.long	0x57
	.quad	.LFB8
	.quad	.LFE8-.LFB8
	.uleb128 0x1
	.byte	0x9c
	.long	0x2bb
	.uleb128 0xa
	.string	"n"
	.byte	0x1
	.byte	0x5e
	.long	0x57
	.uleb128 0x2
	.byte	0x91
	.sleb128 -36
	.uleb128 0x7
	.long	.LASF21
	.byte	0x1
	.byte	0x5f
	.long	0x57
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x8
	.string	"rnd"
	.byte	0x1
	.byte	0x60
	.long	0x57
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.byte	0
	.uleb128 0xc
	.long	.LASF22
	.byte	0x1
	.byte	0x6a
	.quad	.LFB9
	.quad	.LFE9-.LFB9
	.uleb128 0x1
	.byte	0x9c
	.long	0x319
	.uleb128 0x10
	.long	.LASF23
	.byte	0x1
	.byte	0x6a
	.long	0x319
	.uleb128 0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0xa
	.string	"n"
	.byte	0x1
	.byte	0x6a
	.long	0x57
	.uleb128 0x2
	.byte	0x91
	.sleb128 -44
	.uleb128 0x8
	.string	"i"
	.byte	0x1
	.byte	0x6b
	.long	0x57
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0x8
	.string	"j"
	.byte	0x1
	.byte	0x6b
	.long	0x57
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0x8
	.string	"tmp"
	.byte	0x1
	.byte	0x6c
	.long	0x319
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.byte	0
	.uleb128 0x5
	.byte	0x8
	.long	0x31f
	.uleb128 0x5
	.byte	0x8
	.long	0x325
	.uleb128 0x11
	.byte	0
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.uleb128 0x1
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1b
	.uleb128 0xe
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x10
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x2
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0
	.byte	0
	.uleb128 0x4
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x5
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2117
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x7
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x8
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x9
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2117
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xa
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.byte	0
	.byte	0
	.uleb128 0xc
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2116
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xd
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xe
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0xf
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2116
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x10
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x11
	.uleb128 0x35
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_aranges,"",@progbits
	.long	0x2c
	.value	0x2
	.long	.Ldebug_info0
	.byte	0x8
	.byte	0
	.value	0
	.value	0
	.quad	.Ltext0
	.quad	.Letext0-.Ltext0
	.quad	0
	.quad	0
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.section	.debug_str,"MS",@progbits,1
.LASF13:
	.string	"timestamp"
.LASF18:
	.string	"ptr_int"
.LASF2:
	.string	"short unsigned int"
.LASF27:
	.string	"rand_int"
.LASF20:
	.string	"printPtr2bin"
.LASF22:
	.string	"sattolo_shuffle"
.LASF24:
	.string	"GNU C11 5.2.1 20151010 -masm=intel -mtune=generic -march=x86-64 -g -O0 -fstack-protector-strong"
.LASF1:
	.string	"unsigned char"
.LASF0:
	.string	"long unsigned int"
.LASF19:
	.string	"ptr2bin"
.LASF23:
	.string	"array"
.LASF11:
	.string	"bottom"
.LASF14:
	.string	"cycles_low"
.LASF17:
	.string	"timestamp_stop"
.LASF3:
	.string	"unsigned int"
.LASF10:
	.string	"long long unsigned int"
.LASF16:
	.string	"timestamp_start"
.LASF21:
	.string	"limit"
.LASF26:
	.string	"/home/user/Documents/github/ctattack"
.LASF12:
	.string	"timestamp_weak"
.LASF7:
	.string	"sizetype"
.LASF9:
	.string	"long long int"
.LASF8:
	.string	"char"
.LASF25:
	.string	"src/ctattack.c"
.LASF5:
	.string	"short int"
.LASF6:
	.string	"long int"
.LASF4:
	.string	"signed char"
.LASF15:
	.string	"cycles_high"
	.ident	"GCC: (Ubuntu 5.2.1-22ubuntu2) 5.2.1 20151010"
	.section	.note.GNU-stack,"",@progbits
