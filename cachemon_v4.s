	.file	"cachemon_v4.c"
	.intel_syntax noprefix
	.section	.rodata
.LC0:
	.string	"%p\n"
.LC1:
	.string	"%d #############\n"
.LC2:
	.string	"S: RAM"
.LC3:
	.string	"S: L3"
.LC4:
	.string	"\t: %lu\n"
.LC5:
	.string	"R: RAM"
	.align 8
.LC6:
	.string	"**************************** R: L3"
.LC7:
	.string	"B\t%p\t\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB2:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 176
	mov	DWORD PTR [rbp-164], edi
	mov	QWORD PTR [rbp-176], rsi
	mov	QWORD PTR [rbp-88], 0
	mov	QWORD PTR [rbp-80], 0
	mov	BYTE PTR [rbp-157], 0
	mov	DWORD PTR [rbp-124], 1
	mov	QWORD PTR [rbp-72], 3145728
	mov	rax, QWORD PTR [rbp-72]
	mov	DWORD PTR [rbp-120], eax
	mov	rax, QWORD PTR [rbp-72]
	cdqe
	shr	rax, 3
	mov	DWORD PTR [rbp-116], eax
	mov	rax, QWORD PTR [rbp-72]
	mov	r9d, 0
	mov	r8d, -1
	mov	ecx, 262178
	mov	edx, 3
	mov	rsi, rax
	mov	edi, 0
	call	mmap
	mov	QWORD PTR [rbp-64], rax
	mov	rax, QWORD PTR [rbp-72]
	mov	r9d, 0
	mov	r8d, -1
	mov	ecx, 262178
	mov	edx, 3
	mov	rsi, rax
	mov	edi, 0
	call	mmap
	mov	QWORD PTR [rbp-56], rax
	mov	DWORD PTR [rbp-156], 0
	jmp	.L2
.L3:
	mov	eax, DWORD PTR [rbp-156]
	cdqe
	lea	rdx, [0+rax*8]
	mov	rax, QWORD PTR [rbp-64]
	lea	rcx, [rdx+rax]
	mov	eax, DWORD PTR [rbp-156]
	add	eax, 32768
	cdq
	idiv	DWORD PTR [rbp-116]
	mov	eax, edx
	cdqe
	lea	rdx, [0+rax*8]
	mov	rax, QWORD PTR [rbp-64]
	add	rax, rdx
	mov	QWORD PTR [rcx], rax
	add	DWORD PTR [rbp-156], 1
.L2:
	mov	eax, DWORD PTR [rbp-156]
	cmp	eax, DWORD PTR [rbp-116]
	jl	.L3
	mov	DWORD PTR [rbp-152], 0
	jmp	.L4
.L11:
	mov	DWORD PTR [rbp-148], 0
	jmp	.L5
.L6:
	mov	eax, DWORD PTR [rbp-124]
	sal	eax, 6
	mov	edx, eax
	mov	eax, DWORD PTR [rbp-148]
	add	eax, edx
	cdqe
	lea	rdx, [0+rax*8]
	mov	rax, QWORD PTR [rbp-56]
	add	rax, rdx
	mov	rax, QWORD PTR [rax]
	mov	QWORD PTR [rbp-112], rax
	add	DWORD PTR [rbp-148], 32768
.L5:
	mov	rax, QWORD PTR [rbp-72]
	lea	edx, [rax+7]
	test	eax, eax
	cmovs	eax, edx
	sar	eax, 3
	cmp	eax, DWORD PTR [rbp-148]
	jg	.L6
	mov	DWORD PTR [rbp-144], 0
	jmp	.L7
.L10:
	mov	eax, DWORD PTR [rbp-144]
	add	eax, 12
	cdqe
	sal	rax, 18
	mov	rdx, rax
	mov	rax, QWORD PTR [rbp-56]
	add	rax, rdx
	mov	rax, QWORD PTR [rax]
	mov	QWORD PTR [rbp-112], rax
	mov	DWORD PTR [rbp-140], 1
	jmp	.L8
.L9:
	mov	eax, DWORD PTR [rbp-124]
	sal	eax, 6
	mov	edx, eax
	mov	eax, DWORD PTR [rbp-140]
	add	eax, edx
	cdqe
	lea	rdx, [0+rax*8]
	mov	rax, QWORD PTR [rbp-56]
	add	rax, rdx
	mov	rax, QWORD PTR [rax]
	mov	QWORD PTR [rbp-112], rax
	add	DWORD PTR [rbp-140], 32768
.L8:
	mov	rax, QWORD PTR [rbp-72]
	lea	edx, [rax+7]
	test	eax, eax
	cmovs	eax, edx
	sar	eax, 3
	cmp	eax, DWORD PTR [rbp-140]
	jg	.L9
	add	DWORD PTR [rbp-144], 1
.L7:
	cmp	DWORD PTR [rbp-144], 11
	jle	.L10
	add	DWORD PTR [rbp-152], 1
.L4:
	cmp	DWORD PTR [rbp-152], 99999
	jle	.L11
	mov	rax, QWORD PTR [rbp-56]
	mov	rdx, QWORD PTR [rbp-56]
	mov	QWORD PTR [rax], rdx
	mov	rax, QWORD PTR [rbp-56]
	add	rax, 15360
	mov	rdx, QWORD PTR [rbp-56]
	add	rdx, 15360
	mov	QWORD PTR [rax], rdx
	mov	rax, QWORD PTR [rbp-64]
	add	rax, 1024
	mov	rax, QWORD PTR [rax]
	mov	rsi, rax
	mov	edi, OFFSET FLAT:.LC0
	mov	eax, 0
	call	printf
	mov	rax, QWORD PTR [rbp-64]
	add	rax, 1032
	mov	rsi, rax
	mov	edi, OFFSET FLAT:.LC0
	mov	eax, 0
	call	printf
	mov	rax, QWORD PTR [rbp-64]
	mov	rax, QWORD PTR [rax]
	mov	QWORD PTR [rbp-112], rax
	mov	rax, QWORD PTR [rbp-64]
	mov	rax, QWORD PTR [rax]
	mov	QWORD PTR [rbp-104], rax
	mov	rax, QWORD PTR [rbp-64]
	mov	rax, QWORD PTR [rax]
	mov	QWORD PTR [rbp-96], rax
	mov	rax, QWORD PTR [rbp-112]
	mov	rsi, rax
	mov	edi, OFFSET FLAT:.LC0
	mov	eax, 0
	call	printf
	mov	DWORD PTR [rbp-136], 0
	jmp	.L12
.L17:
	mov	eax, DWORD PTR [rbp-136]
	lea	edx, [rax+32767]
	test	eax, eax
	cmovs	eax, edx
	sar	eax, 15
	mov	esi, eax
	mov	edi, OFFSET FLAT:.LC1
	mov	eax, 0
	call	printf
	call	timestamp_start
	mov	QWORD PTR [rbp-48], rax
	mov	rax, QWORD PTR [rbp-112]
	mov	rax, QWORD PTR [rax]
	mov	QWORD PTR [rbp-112], rax
	call	timestamp_stop
	mov	QWORD PTR [rbp-40], rax
	mov	rax, QWORD PTR [rbp-112]
	mov	rsi, rax
	mov	edi, OFFSET FLAT:.LC0
	mov	eax, 0
	call	printf
	call	timestamp_start
	mov	QWORD PTR [rbp-32], rax
	mov	rax, QWORD PTR [rbp-104]
	mov	rax, QWORD PTR [rax]
	mov	QWORD PTR [rbp-104], rax
	call	timestamp_stop
	mov	QWORD PTR [rbp-24], rax
	call	timestamp_start
	mov	QWORD PTR [rbp-16], rax
	call	timestamp_stop
	mov	QWORD PTR [rbp-8], rax
	mov	rax, QWORD PTR [rbp-40]
	sub	rax, QWORD PTR [rbp-48]
	cmp	rax, 299
	jbe	.L13
	mov	edi, OFFSET FLAT:.LC2
	mov	eax, 0
	call	printf
	jmp	.L14
.L13:
	mov	edi, OFFSET FLAT:.LC3
	mov	eax, 0
	call	printf
.L14:
	mov	rax, QWORD PTR [rbp-40]
	sub	rax, QWORD PTR [rbp-48]
	mov	rdx, rax
	mov	rax, QWORD PTR [rbp-16]
	sub	rax, QWORD PTR [rbp-8]
	add	rax, rdx
	mov	rsi, rax
	mov	edi, OFFSET FLAT:.LC4
	mov	eax, 0
	call	printf
	mov	rax, QWORD PTR [rbp-24]
	sub	rax, QWORD PTR [rbp-32]
	cmp	rax, 299
	jbe	.L15
	mov	edi, OFFSET FLAT:.LC2
	mov	eax, 0
	call	printf
	jmp	.L16
.L15:
	mov	edi, OFFSET FLAT:.LC3
	mov	eax, 0
	call	printf
.L16:
	mov	rax, QWORD PTR [rbp-24]
	sub	rax, QWORD PTR [rbp-32]
	mov	rdx, rax
	mov	rax, QWORD PTR [rbp-16]
	sub	rax, QWORD PTR [rbp-8]
	add	rax, rdx
	mov	rsi, rax
	mov	edi, OFFSET FLAT:.LC4
	mov	eax, 0
	call	printf
	add	DWORD PTR [rbp-136], 32768
.L12:
	mov	eax, DWORD PTR [rbp-120]
	lea	edx, [rax+7]
	test	eax, eax
	cmovs	eax, edx
	sar	eax, 3
	cmp	eax, DWORD PTR [rbp-136]
	jg	.L17
	mov	rax, QWORD PTR [rbp-56]
	mov	rax, QWORD PTR [rax]
	mov	QWORD PTR [rbp-112], rax
	mov	DWORD PTR [rbp-132], 0
	jmp	.L18
.L19:
	mov	rax, QWORD PTR [rbp-112]
	mov	rax, QWORD PTR [rax]
	mov	QWORD PTR [rbp-112], rax
	add	DWORD PTR [rbp-132], 1
.L18:
	cmp	DWORD PTR [rbp-132], 999999
	jle	.L19
	mov	DWORD PTR [rbp-128], 0
	jmp	.L20
.L23:
	call	timestamp_start
	mov	QWORD PTR [rbp-48], rax
	mov	rax, QWORD PTR [rbp-96]
	mov	rax, QWORD PTR [rax]
	mov	QWORD PTR [rbp-96], rax
	call	timestamp_stop
	mov	QWORD PTR [rbp-40], rax
	call	timestamp_start
	mov	QWORD PTR [rbp-16], rax
	call	timestamp_stop
	mov	QWORD PTR [rbp-8], rax
	mov	rax, QWORD PTR [rbp-40]
	sub	rax, QWORD PTR [rbp-48]
	cmp	rax, 299
	jbe	.L21
	mov	edi, OFFSET FLAT:.LC5
	mov	eax, 0
	call	printf
	jmp	.L22
.L21:
	mov	edi, OFFSET FLAT:.LC6
	mov	eax, 0
	call	printf
.L22:
	mov	rax, QWORD PTR [rbp-40]
	sub	rax, QWORD PTR [rbp-48]
	mov	rdx, rax
	mov	rax, QWORD PTR [rbp-16]
	sub	rax, QWORD PTR [rbp-8]
	add	rax, rdx
	mov	rsi, rax
	mov	edi, OFFSET FLAT:.LC4
	mov	eax, 0
	call	printf
	add	DWORD PTR [rbp-128], 32768
.L20:
	mov	eax, DWORD PTR [rbp-120]
	lea	edx, [rax+7]
	test	eax, eax
	cmovs	eax, edx
	sar	eax, 3
	cmp	eax, DWORD PTR [rbp-128]
	jg	.L23
	mov	rax, QWORD PTR [rbp-64]
	mov	rsi, rax
	mov	edi, OFFSET FLAT:.LC7
	mov	eax, 0
	call	printf
	mov	rax, QWORD PTR [rbp-64]
	mov	rdi, rax
	call	printPtr2bin
	mov	rax, QWORD PTR [rbp-56]
	mov	rsi, rax
	mov	edi, OFFSET FLAT:.LC7
	mov	eax, 0
	call	printf
	mov	rax, QWORD PTR [rbp-56]
	mov	rdi, rax
	call	printPtr2bin
	call	timestamp_start
	mov	QWORD PTR [rbp-48], rax
	call	timestamp_stop
	mov	QWORD PTR [rbp-40], rax
	mov	rax, QWORD PTR [rbp-40]
	sub	rax, QWORD PTR [rbp-48]
	mov	rsi, rax
	mov	edi, OFFSET FLAT:.LC4
	mov	eax, 0
	call	printf
	mov	eax, 0
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	main, .-main
	.type	rand_int, @function
rand_int:
.LFB3:
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
.L26:
	call	rand
	mov	DWORD PTR [rbp-4], eax
	mov	eax, DWORD PTR [rbp-4]
	cmp	eax, DWORD PTR [rbp-8]
	jge	.L26
	mov	eax, DWORD PTR [rbp-4]
	cdq
	idiv	DWORD PTR [rbp-20]
	mov	eax, edx
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	rand_int, .-rand_int
	.globl	sattolo
	.type	sattolo, @function
sattolo:
.LFB4:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	push	rbx
	sub	rsp, 72
	.cfi_offset 3, -24
	mov	QWORD PTR [rbp-72], rdi
	mov	DWORD PTR [rbp-76], esi
	mov	rax, QWORD PTR fs:40
	mov	QWORD PTR [rbp-24], rax
	xor	eax, eax
	mov	rax, rsp
	mov	rbx, rax
	mov	DWORD PTR [rbp-48], 32768
	mov	eax, DWORD PTR [rbp-48]
	movsx	rdx, eax
	sub	rdx, 1
	mov	QWORD PTR [rbp-40], rdx
	movsx	rdx, eax
	mov	r10, rdx
	mov	r11d, 0
	movsx	rdx, eax
	mov	r8, rdx
	mov	r9d, 0
	cdqe
	sal	rax, 3
	lea	rdx, [rax+7]
	mov	eax, 16
	sub	rax, 1
	add	rax, rdx
	mov	esi, 16
	mov	edx, 0
	div	rsi
	imul	rax, rax, 16
	sub	rsp, rax
	mov	rax, rsp
	add	rax, 7
	shr	rax, 3
	sal	rax, 3
	mov	QWORD PTR [rbp-32], rax
	mov	eax, DWORD PTR [rbp-76]
	cdq
	idiv	DWORD PTR [rbp-48]
	mov	DWORD PTR [rbp-76], eax
	mov	eax, DWORD PTR [rbp-76]
	sub	eax, 1
	mov	DWORD PTR [rbp-56], eax
	jmp	.L29
.L32:
	mov	eax, DWORD PTR [rbp-56]
	add	eax, 1
	mov	edi, eax
	call	rand_int
	mov	DWORD PTR [rbp-44], eax
	mov	DWORD PTR [rbp-52], 0
	jmp	.L30
.L31:
	mov	eax, DWORD PTR [rbp-44]
	imul	eax, DWORD PTR [rbp-48]
	mov	edx, eax
	mov	eax, DWORD PTR [rbp-52]
	add	eax, edx
	cdqe
	lea	rdx, [0+rax*8]
	mov	rax, QWORD PTR [rbp-72]
	add	rax, rdx
	mov	rcx, QWORD PTR [rax]
	mov	rax, QWORD PTR [rbp-32]
	mov	edx, DWORD PTR [rbp-52]
	movsx	rdx, edx
	mov	QWORD PTR [rax+rdx*8], rcx
	mov	eax, DWORD PTR [rbp-44]
	imul	eax, DWORD PTR [rbp-48]
	mov	edx, eax
	mov	eax, DWORD PTR [rbp-52]
	add	eax, edx
	cdqe
	lea	rdx, [0+rax*8]
	mov	rax, QWORD PTR [rbp-72]
	add	rdx, rax
	mov	eax, DWORD PTR [rbp-56]
	imul	eax, DWORD PTR [rbp-48]
	mov	ecx, eax
	mov	eax, DWORD PTR [rbp-52]
	add	eax, ecx
	cdqe
	lea	rcx, [0+rax*8]
	mov	rax, QWORD PTR [rbp-72]
	add	rax, rcx
	mov	rax, QWORD PTR [rax]
	mov	QWORD PTR [rdx], rax
	mov	eax, DWORD PTR [rbp-56]
	imul	eax, DWORD PTR [rbp-48]
	mov	edx, eax
	mov	eax, DWORD PTR [rbp-52]
	add	eax, edx
	cdqe
	lea	rdx, [0+rax*8]
	mov	rax, QWORD PTR [rbp-72]
	lea	rcx, [rdx+rax]
	mov	rax, QWORD PTR [rbp-32]
	mov	edx, DWORD PTR [rbp-52]
	movsx	rdx, edx
	mov	rax, QWORD PTR [rax+rdx*8]
	mov	QWORD PTR [rcx], rax
	add	DWORD PTR [rbp-52], 1
.L30:
	mov	eax, DWORD PTR [rbp-52]
	cmp	eax, DWORD PTR [rbp-48]
	jl	.L31
	sub	DWORD PTR [rbp-56], 1
.L29:
	cmp	DWORD PTR [rbp-56], 0
	jg	.L32
	mov	rsp, rbx
	nop
	mov	rax, QWORD PTR [rbp-24]
	xor	rax, QWORD PTR fs:40
	je	.L33
	call	__stack_chk_fail
.L33:
	mov	rbx, QWORD PTR [rbp-8]
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE4:
	.size	sattolo, .-sattolo
	.ident	"GCC: (Ubuntu 5.2.1-22ubuntu2) 5.2.1 20151010"
	.section	.note.GNU-stack,"",@progbits
