	.file	"cacheprof_v4.c"
	.intel_syntax noprefix
	.section	.rodata
.LC0:
	.string	"B\t%p\t\n"
	.align 8
.LC1:
	.string	"##############################################"
.LC2:
	.string	"S: RAM"
.LC3:
	.string	"S: L3"
.LC4:
	.string	"\t: %lu\n"
.LC5:
	.string	"%lu\n"
.LC6:
	.string	"TEST2"
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
	sub	rsp, 96
	mov	DWORD PTR [rbp-84], edi
	mov	QWORD PTR [rbp-96], rsi
	mov	rax, QWORD PTR fs:40
	mov	QWORD PTR [rbp-8], rax
	xor	eax, eax
	mov	QWORD PTR [rbp-56], 0
	mov	QWORD PTR [rbp-48], 0
	mov	BYTE PTR [rbp-77], 0
	mov	QWORD PTR [rbp-40], 134217728
	mov	rax, QWORD PTR [rbp-40]
	mov	r9d, 0
	mov	r8d, -1
	mov	ecx, 262178
	mov	edx, 3
	mov	rsi, rax
	mov	edi, 0
	call	mmap
	mov	QWORD PTR [rbp-32], rax
	mov	DWORD PTR [rbp-76], 0
	jmp	.L2
.L4:
	mov	eax, DWORD PTR [rbp-76]
	cdqe
	lea	rdx, [0+rax*8]
	mov	rax, QWORD PTR [rbp-32]
	add	rax, rdx
	mov	edx, DWORD PTR [rbp-76]
	movsx	rdx, edx
	add	rdx, 1
	lea	rcx, [0+rdx*8]
	mov	rdx, QWORD PTR [rbp-32]
	add	rdx, rcx
	mov	QWORD PTR [rax], rdx
	mov	eax, DWORD PTR [rbp-76]
	cdq
	shr	edx, 29
	add	eax, edx
	and	eax, 7
	sub	eax, edx
	cmp	eax, 7
	jne	.L3
	mov	eax, DWORD PTR [rbp-76]
	cdqe
	lea	rdx, [0+rax*8]
	mov	rax, QWORD PTR [rbp-32]
	add	rdx, rax
	lea	rax, [rbp-64]
	mov	QWORD PTR [rdx], rax
.L3:
	add	DWORD PTR [rbp-76], 1
.L2:
	cmp	DWORD PTR [rbp-76], 16777215
	jle	.L4
	mov	rax, QWORD PTR [rbp-32]
	mov	rsi, rax
	mov	edi, OFFSET FLAT:.LC0
	mov	eax, 0
	call	printf
	mov	rax, QWORD PTR [rbp-32]
	mov	rdi, rax
	call	printPtr2bin
	mov	edi, OFFSET FLAT:.LC1
	call	puts
	mov	QWORD PTR [rbp-56], 0
	mov	DWORD PTR [rbp-72], 0
	jmp	.L5
.L6:
	mov	eax, DWORD PTR [rbp-72]
	cdqe
	lea	rdx, [0+rax*8]
	mov	rax, QWORD PTR [rbp-32]
	add	rax, rdx
	mov	rax, QWORD PTR [rax]
	add	rax, 1
	movzx	eax, BYTE PTR [rax]
	movsx	rax, al
	add	QWORD PTR [rbp-48], rax
	add	DWORD PTR [rbp-72], 1
.L5:
	cmp	DWORD PTR [rbp-72], 63
	jle	.L6
	mov	DWORD PTR [rbp-68], 0
	jmp	.L7
.L10:
	call	timestamp_start
	mov	QWORD PTR [rbp-24], rax
	mov	rax, QWORD PTR [rbp-32]
	mov	rax, QWORD PTR [rax]
	mov	QWORD PTR [rbp-16], rax
	call	timestamp_stop
	mov	QWORD PTR [rbp-64], rax
	mov	rax, QWORD PTR [rbp-64]
	sub	rax, QWORD PTR [rbp-24]
	cmp	rax, 300
	jbe	.L8
	mov	edi, OFFSET FLAT:.LC2
	mov	eax, 0
	call	printf
	jmp	.L9
.L8:
	mov	edi, OFFSET FLAT:.LC3
	mov	eax, 0
	call	printf
.L9:
	mov	rax, QWORD PTR [rbp-64]
	sub	rax, QWORD PTR [rbp-24]
	mov	rsi, rax
	mov	edi, OFFSET FLAT:.LC4
	mov	eax, 0
	call	printf
	mov	rax, QWORD PTR [rbp-64]
	sub	rax, QWORD PTR [rbp-24]
	add	QWORD PTR [rbp-56], rax
	add	DWORD PTR [rbp-68], 32768
.L7:
	cmp	DWORD PTR [rbp-68], 16777215
	jle	.L10
	mov	rax, QWORD PTR [rbp-56]
	shr	rax, 6
	mov	rsi, rax
	mov	edi, OFFSET FLAT:.LC5
	mov	eax, 0
	call	printf
	mov	edi, OFFSET FLAT:.LC6
	call	puts
	call	timestamp_start
	mov	QWORD PTR [rbp-24], rax
	call	timestamp_stop
	mov	QWORD PTR [rbp-64], rax
	mov	rax, QWORD PTR [rbp-64]
	sub	rax, QWORD PTR [rbp-24]
	mov	rsi, rax
	mov	edi, OFFSET FLAT:.LC5
	mov	eax, 0
	call	printf
	mov	eax, 0
	mov	rcx, QWORD PTR [rbp-8]
	xor	rcx, QWORD PTR fs:40
	je	.L12
	call	__stack_chk_fail
.L12:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	main, .-main
	.section	.rodata
.LC7:
	.string	"rb"
.LC8:
	.string	"/proc/self/pagemap"
	.align 8
.LC9:
	.string	"Failed to seek pagemap to proper location\n"
	.text
	.globl	get_page_frame_number_of_address
	.type	get_page_frame_number_of_address, @function
get_page_frame_number_of_address:
.LFB3:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	push	rbx
	sub	rsp, 56
	.cfi_offset 3, -24
	mov	QWORD PTR [rbp-56], rdi
	mov	rax, QWORD PTR fs:40
	mov	QWORD PTR [rbp-24], rax
	xor	eax, eax
	mov	esi, OFFSET FLAT:.LC7
	mov	edi, OFFSET FLAT:.LC8
	call	fopen
	mov	QWORD PTR [rbp-40], rax
	mov	rbx, QWORD PTR [rbp-56]
	call	getpagesize
	movsx	rcx, eax
	mov	rax, rbx
	mov	edx, 0
	div	rcx
	sal	rax, 21
	mov	QWORD PTR [rbp-32], rax
	mov	rcx, QWORD PTR [rbp-32]
	mov	rax, QWORD PTR [rbp-40]
	mov	edx, 0
	mov	rsi, rcx
	mov	rdi, rax
	call	fseek
	test	eax, eax
	je	.L14
	mov	rax, QWORD PTR stderr[rip]
	mov	rcx, rax
	mov	edx, 42
	mov	esi, 1
	mov	edi, OFFSET FLAT:.LC9
	call	fwrite
	mov	edi, 1
	call	exit
.L14:
	mov	QWORD PTR [rbp-48], 0
	mov	rdx, QWORD PTR [rbp-40]
	lea	rax, [rbp-48]
	mov	rcx, rdx
	mov	edx, 2097151
	mov	esi, 1
	mov	rdi, rax
	call	fread
	mov	rdx, QWORD PTR [rbp-48]
	movabs	rax, 36028797018963967
	and	rax, rdx
	mov	QWORD PTR [rbp-48], rax
	mov	rax, QWORD PTR [rbp-40]
	mov	rdi, rax
	call	fclose
	mov	rax, QWORD PTR [rbp-48]
	mov	rbx, QWORD PTR [rbp-24]
	xor	rbx, QWORD PTR fs:40
	je	.L16
	call	__stack_chk_fail
.L16:
	add	rsp, 56
	pop	rbx
	pop	rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	get_page_frame_number_of_address, .-get_page_frame_number_of_address
	.section	.rodata
.LC10:
	.string	"r"
.LC11:
	.string	"First if"
.LC12:
	.string	"Second if"
.LC13:
	.string	"Third if"
.LC14:
	.string	"Fourth if"
	.text
	.globl	vtop
	.type	vtop, @function
vtop:
.LFB4:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	push	rbx
	sub	rsp, 56
	.cfi_offset 3, -24
	mov	QWORD PTR [rbp-56], rdi
	mov	QWORD PTR [rbp-40], 0
	mov	rax, QWORD PTR [rbp-56]
	shr	rax, 12
	sal	eax, 3
	mov	DWORD PTR [rbp-44], eax
	mov	esi, OFFSET FLAT:.LC10
	mov	edi, OFFSET FLAT:.LC8
	call	fopen
	mov	QWORD PTR [rbp-32], rax
	cmp	QWORD PTR [rbp-32], 0
	je	.L18
	mov	edi, OFFSET FLAT:.LC11
	call	puts
	mov	eax, DWORD PTR [rbp-44]
	movsx	rbx, eax
	mov	rax, QWORD PTR [rbp-32]
	mov	rdi, rax
	call	fileno
	mov	edx, 0
	mov	rsi, rbx
	mov	edi, eax
	call	lseek
	mov	rdx, rax
	mov	eax, DWORD PTR [rbp-44]
	cdqe
	cmp	rdx, rax
	jne	.L19
	mov	edi, OFFSET FLAT:.LC12
	call	puts
	mov	rax, QWORD PTR [rbp-32]
	mov	rdi, rax
	call	_IO_getc
	cdqe
	mov	QWORD PTR [rbp-24], rax
	cmp	QWORD PTR [rbp-24], 0
	je	.L19
	mov	edi, OFFSET FLAT:.LC13
	call	puts
	mov	rax, QWORD PTR [rbp-24]
	test	rax, rax
	jns	.L19
	mov	edi, OFFSET FLAT:.LC14
	call	puts
	mov	rdx, QWORD PTR [rbp-24]
	movabs	rax, 18014398509481983
	and	rax, rdx
	mov	QWORD PTR [rbp-40], rax
	sal	QWORD PTR [rbp-40], 21
	mov	rax, QWORD PTR [rbp-56]
	and	eax, 2097151
	mov	rdx, rax
	mov	rax, QWORD PTR [rbp-40]
	or	rax, rdx
	mov	QWORD PTR [rbp-40], rax
.L19:
	mov	rax, QWORD PTR [rbp-32]
	mov	rdi, rax
	call	fclose
.L18:
	mov	rax, QWORD PTR [rbp-40]
	add	rsp, 56
	pop	rbx
	pop	rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE4:
	.size	vtop, .-vtop
	.type	rand_int, @function
rand_int:
.LFB5:
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
.L22:
	call	rand
	mov	DWORD PTR [rbp-4], eax
	mov	eax, DWORD PTR [rbp-4]
	cmp	eax, DWORD PTR [rbp-8]
	jge	.L22
	mov	eax, DWORD PTR [rbp-4]
	cdq
	idiv	DWORD PTR [rbp-20]
	mov	eax, edx
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5:
	.size	rand_int, .-rand_int
	.section	.rodata
.LC15:
	.string	"i: %d\n"
.LC16:
	.string	"j: %d\n"
.LC17:
	.string	"%d\n"
	.text
	.globl	shuffle
	.type	shuffle, @function
shuffle:
.LFB6:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 112
	mov	QWORD PTR [rbp-104], rdi
	mov	DWORD PTR [rbp-108], esi
	mov	rax, QWORD PTR fs:40
	mov	QWORD PTR [rbp-8], rax
	xor	eax, eax
	mov	eax, DWORD PTR [rbp-108]
	cdqe
	shr	rax, 3
	sub	eax, 1
	mov	DWORD PTR [rbp-92], eax
	jmp	.L25
.L28:
	mov	eax, DWORD PTR [rbp-92]
	mov	esi, eax
	mov	edi, OFFSET FLAT:.LC15
	mov	eax, 0
	call	printf
	mov	eax, DWORD PTR [rbp-92]
	add	eax, 1
	mov	edi, eax
	call	rand_int
	mov	DWORD PTR [rbp-84], eax
	mov	eax, DWORD PTR [rbp-84]
	mov	esi, eax
	mov	edi, OFFSET FLAT:.LC16
	mov	eax, 0
	call	printf
	mov	DWORD PTR [rbp-88], 0
	jmp	.L26
.L27:
	mov	eax, DWORD PTR [rbp-88]
	mov	esi, eax
	mov	edi, OFFSET FLAT:.LC17
	mov	eax, 0
	call	printf
	mov	eax, DWORD PTR [rbp-84]
	cdqe
	lea	rdx, [0+rax*8]
	mov	eax, DWORD PTR [rbp-88]
	cdqe
	add	rax, rdx
	lea	rdx, [0+rax*8]
	mov	rax, QWORD PTR [rbp-104]
	add	rax, rdx
	mov	rdx, QWORD PTR [rax]
	mov	eax, DWORD PTR [rbp-88]
	cdqe
	mov	QWORD PTR [rbp-80+rax*8], rdx
	mov	eax, DWORD PTR [rbp-84]
	cdqe
	lea	rdx, [0+rax*8]
	mov	eax, DWORD PTR [rbp-88]
	cdqe
	add	rax, rdx
	lea	rdx, [0+rax*8]
	mov	rax, QWORD PTR [rbp-104]
	add	rdx, rax
	mov	eax, DWORD PTR [rbp-92]
	cdqe
	lea	rcx, [0+rax*8]
	mov	eax, DWORD PTR [rbp-88]
	cdqe
	add	rax, rcx
	lea	rcx, [0+rax*8]
	mov	rax, QWORD PTR [rbp-104]
	add	rax, rcx
	mov	rax, QWORD PTR [rax]
	mov	QWORD PTR [rdx], rax
	mov	eax, DWORD PTR [rbp-92]
	cdqe
	lea	rdx, [0+rax*8]
	mov	eax, DWORD PTR [rbp-88]
	cdqe
	add	rax, rdx
	lea	rdx, [0+rax*8]
	mov	rax, QWORD PTR [rbp-104]
	add	rdx, rax
	mov	eax, DWORD PTR [rbp-88]
	cdqe
	mov	rax, QWORD PTR [rbp-80+rax*8]
	mov	QWORD PTR [rdx], rax
	add	DWORD PTR [rbp-88], 1
.L26:
	mov	eax, DWORD PTR [rbp-88]
	cmp	eax, 7
	jbe	.L27
	mov	eax, DWORD PTR [rbp-92]
	sub	eax, 8
	mov	DWORD PTR [rbp-92], eax
.L25:
	cmp	DWORD PTR [rbp-92], 0
	jg	.L28
	nop
	mov	rax, QWORD PTR [rbp-8]
	xor	rax, QWORD PTR fs:40
	je	.L29
	call	__stack_chk_fail
.L29:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	shuffle, .-shuffle
	.ident	"GCC: (Ubuntu 5.2.1-22ubuntu2) 5.2.1 20151010"
	.section	.note.GNU-stack,"",@progbits
