	.file	"ctattack.c"
	.intel_syntax noprefix
	.text
	.globl	timestamp_weak
	.type	timestamp_weak, @function
timestamp_weak:
.LFB2:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
#APP
# 11 "src/ctattack.c" 1
	.byte 15;.byte 49
# 0 "" 2
#NO_APP
	mov	DWORD PTR [rbp-8], eax
	mov	DWORD PTR [rbp-4], edx
	mov	eax, DWORD PTR [rbp-8]
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
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	push	rbx
	.cfi_offset 3, -24
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
	mov	eax, DWORD PTR [rbp-12]
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
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	push	rbx
	.cfi_offset 3, -24
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
	mov	eax, DWORD PTR [rbp-16]
	sal	rax, 32
	mov	rdx, rax
	mov	eax, DWORD PTR [rbp-12]
	or	rax, rdx
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
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	push	rbx
	.cfi_offset 3, -24
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
	mov	eax, DWORD PTR [rbp-16]
	sal	rax, 32
	mov	rdx, rax
	mov	eax, DWORD PTR [rbp-12]
	or	rax, rdx
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
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	mov	QWORD PTR [rbp-24], rdi
	mov	QWORD PTR [rbp-32], rsi
	mov	rax, QWORD PTR [rbp-24]
	mov	QWORD PTR [rbp-8], rax
	mov	DWORD PTR [rbp-12], 0
	jmp	.L10
.L13:
	mov	rax, QWORD PTR [rbp-8]
	and	eax, 1
	test	rax, rax
	je	.L11
	mov	eax, DWORD PTR [rbp-12]
	movsx	rdx, eax
	mov	rax, QWORD PTR [rbp-32]
	add	rax, rdx
	mov	BYTE PTR [rax], 49
	jmp	.L12
.L11:
	mov	eax, DWORD PTR [rbp-12]
	movsx	rdx, eax
	mov	rax, QWORD PTR [rbp-32]
	add	rax, rdx
	mov	BYTE PTR [rax], 48
.L12:
	shr	QWORD PTR [rbp-8]
	add	DWORD PTR [rbp-12], 1
.L10:
	cmp	DWORD PTR [rbp-12], 63
	jle	.L13
	mov	rax, QWORD PTR [rbp-32]
	add	rax, 64
	mov	BYTE PTR [rax], 0
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
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 112
	mov	QWORD PTR [rbp-104], rdi
	mov	rax, QWORD PTR fs:40
	mov	QWORD PTR [rbp-8], rax
	xor	eax, eax
	lea	rdx, [rbp-80]
	mov	rax, QWORD PTR [rbp-104]
	mov	rsi, rdx
	mov	rdi, rax
	call	ptr2bin
	mov	DWORD PTR [rbp-88], 0
	jmp	.L15
.L18:
	mov	DWORD PTR [rbp-84], 0
	jmp	.L16
.L17:
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
	add	DWORD PTR [rbp-84], 1
.L16:
	cmp	DWORD PTR [rbp-84], 7
	jle	.L17
	mov	edi, 32
	call	putchar
	add	DWORD PTR [rbp-88], 1
.L15:
	cmp	DWORD PTR [rbp-88], 7
	jle	.L18
	mov	edi, 10
	call	putchar
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
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 32
	mov	DWORD PTR [rbp-20], edi
	mov	eax, 2147483647
	cdq
	idiv	DWORD PTR [rbp-20]
	mov	eax, edx
	mov	edx, 2147483647
	sub	edx, eax
	mov	eax, edx
	mov	DWORD PTR [rbp-8], eax
.L21:
	call	rand
	mov	DWORD PTR [rbp-4], eax
	mov	eax, DWORD PTR [rbp-4]
	cmp	eax, DWORD PTR [rbp-8]
	jge	.L21
	mov	eax, DWORD PTR [rbp-4]
	cdq
	idiv	DWORD PTR [rbp-20]
	mov	eax, edx
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
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 32
	mov	QWORD PTR [rbp-24], rdi
	mov	DWORD PTR [rbp-28], esi
	mov	eax, DWORD PTR [rbp-28]
	sub	eax, 1
	mov	DWORD PTR [rbp-16], eax
	jmp	.L24
.L25:
	mov	eax, DWORD PTR [rbp-16]
	add	eax, 1
	mov	edi, eax
	call	rand_int
	mov	DWORD PTR [rbp-12], eax
	mov	eax, DWORD PTR [rbp-12]
	cdqe
	lea	rdx, [0+rax*8]
	mov	rax, QWORD PTR [rbp-24]
	add	rax, rdx
	mov	rax, QWORD PTR [rax]
	mov	QWORD PTR [rbp-8], rax
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
	mov	eax, DWORD PTR [rbp-16]
	cdqe
	lea	rdx, [0+rax*8]
	mov	rax, QWORD PTR [rbp-24]
	add	rdx, rax
	mov	rax, QWORD PTR [rbp-8]
	mov	QWORD PTR [rdx], rax
	sub	DWORD PTR [rbp-16], 1
.L24:
	cmp	DWORD PTR [rbp-16], 0
	jg	.L25
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE9:
	.size	sattolo_shuffle, .-sattolo_shuffle
	.ident	"GCC: (Ubuntu 5.2.1-22ubuntu2) 5.2.1 20151010"
	.section	.note.GNU-stack,"",@progbits
