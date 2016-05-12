	.file	"cachemon_v6.c"
	.intel_syntax noprefix
	.text
.Ltext0:
	.section	.rodata
.LC0:
	.string	"%p\n"
.LC1:
	.string	"%d\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB2:
	.file 1 "src/ssa/cachemon_v6.c"
	.loc 1 15 0
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 144
	mov	DWORD PTR [rbp-132], edi
	mov	QWORD PTR [rbp-144], rsi
	.loc 1 18 0
	mov	QWORD PTR [rbp-104], 0
	mov	QWORD PTR [rbp-96], 0
	mov	QWORD PTR [rbp-88], 0
	mov	QWORD PTR [rbp-80], 0
	mov	QWORD PTR [rbp-72], 0
	.loc 1 24 0
	mov	QWORD PTR [rbp-64], 0
	.loc 1 30 0
	mov	BYTE PTR [rbp-113], 0
	.loc 1 33 0
	mov	QWORD PTR [rbp-56], 6291456
	.loc 1 34 0
	mov	rax, QWORD PTR [rbp-56]
	mov	DWORD PTR [rbp-112], eax
	.loc 1 35 0
	mov	rax, QWORD PTR [rbp-56]
	cdqe
	shr	rax, 3
	mov	DWORD PTR [rbp-108], eax
	.loc 1 38 0
	mov	rax, QWORD PTR [rbp-56]
	mov	r9d, 0
	mov	r8d, -1
	mov	ecx, 262178
	mov	edx, 3
	mov	rsi, rax
	mov	edi, 0
	call	mmap
	mov	QWORD PTR [rbp-48], rax
	.loc 1 39 0
	mov	rax, QWORD PTR [rbp-56]
	mov	r9d, 0
	mov	r8d, -1
	mov	ecx, 262178
	mov	edx, 3
	mov	rsi, rax
	mov	edi, 0
	call	mmap
	mov	QWORD PTR [rbp-40], rax
	.loc 1 40 0
	mov	rax, QWORD PTR [rbp-56]
	mov	r9d, 0
	mov	r8d, -1
	mov	ecx, 262178
	mov	edx, 3
	mov	rsi, rax
	mov	edi, 0
	call	mmap
	mov	QWORD PTR [rbp-32], rax
	.loc 1 44 0
	mov	rax, QWORD PTR [rbp-48]
	add	rax, 1572864
	mov	rax, QWORD PTR [rax]
	mov	QWORD PTR [rbp-64], rax
	.loc 1 45 0
	mov	rax, QWORD PTR [rbp-48]
	add	rax, 3670016
	mov	rax, QWORD PTR [rax]
	mov	QWORD PTR [rbp-64], rax
	.loc 1 46 0
	mov	rax, QWORD PTR [rbp-48]
	add	rax, 5767168
	mov	rax, QWORD PTR [rax]
	mov	QWORD PTR [rbp-64], rax
	.loc 1 61 0
	mov	rax, QWORD PTR [rbp-48]
	mov	QWORD PTR [rbp-24], rax
	.loc 1 63 0
	mov	rax, QWORD PTR [rbp-48]
	lea	rdx, [rax+524288]
	mov	rax, QWORD PTR [rbp-48]
	mov	QWORD PTR [rax], rdx
	.loc 1 64 0
	mov	rax, QWORD PTR [rbp-48]
	add	rax, 524288
	mov	rdx, QWORD PTR [rbp-48]
	add	rdx, 1048576
	mov	QWORD PTR [rax], rdx
	.loc 1 65 0
	mov	rax, QWORD PTR [rbp-48]
	add	rax, 1048576
	mov	rdx, QWORD PTR [rbp-48]
	add	rdx, 1572864
	mov	QWORD PTR [rax], rdx
	.loc 1 66 0
	mov	rax, QWORD PTR [rbp-48]
	add	rax, 1572864
	mov	rdx, QWORD PTR [rbp-48]
	add	rdx, 2097152
	mov	QWORD PTR [rax], rdx
	.loc 1 67 0
	mov	rax, QWORD PTR [rbp-48]
	add	rax, 2097152
	mov	rdx, QWORD PTR [rbp-48]
	add	rdx, 2621440
	mov	QWORD PTR [rax], rdx
	.loc 1 68 0
	mov	rax, QWORD PTR [rbp-48]
	add	rax, 2621440
	mov	rdx, QWORD PTR [rbp-48]
	add	rdx, 3145728
	mov	QWORD PTR [rax], rdx
	.loc 1 69 0
	mov	rax, QWORD PTR [rbp-48]
	add	rax, 3145728
	mov	rdx, QWORD PTR [rbp-48]
	add	rdx, 3670016
	mov	QWORD PTR [rax], rdx
	.loc 1 70 0
	mov	rax, QWORD PTR [rbp-48]
	add	rax, 3670016
	mov	rdx, QWORD PTR [rbp-48]
	add	rdx, 4194304
	mov	QWORD PTR [rax], rdx
	.loc 1 71 0
	mov	rax, QWORD PTR [rbp-48]
	add	rax, 4194304
	mov	rdx, QWORD PTR [rbp-48]
	add	rdx, 4718592
	mov	QWORD PTR [rax], rdx
	.loc 1 72 0
	mov	rax, QWORD PTR [rbp-48]
	add	rax, 4718592
	mov	rdx, QWORD PTR [rbp-48]
	add	rdx, 5242880
	mov	QWORD PTR [rax], rdx
	.loc 1 73 0
	mov	rax, QWORD PTR [rbp-48]
	add	rax, 5242880
	mov	rdx, QWORD PTR [rbp-48]
	add	rdx, 5767168
	mov	QWORD PTR [rax], rdx
	.loc 1 77 0
	call	timestamp_start
	mov	QWORD PTR [rbp-16], rax
	.loc 1 78 0
	mov	rax, QWORD PTR [rbp-24]
	mov	rsi, rax
	mov	edi, OFFSET FLAT:.LC0
	mov	eax, 0
	call	printf
	.loc 1 79 0
	mov	rax, QWORD PTR [rbp-24]
	mov	rax, QWORD PTR [rax]
	mov	QWORD PTR [rbp-24], rax
	.loc 1 80 0
	mov	rax, QWORD PTR [rbp-24]
	mov	rax, QWORD PTR [rax]
	mov	QWORD PTR [rbp-24], rax
	.loc 1 81 0
	mov	rax, QWORD PTR [rbp-24]
	mov	rax, QWORD PTR [rax]
	mov	QWORD PTR [rbp-24], rax
	.loc 1 82 0
	mov	rax, QWORD PTR [rbp-24]
	mov	rax, QWORD PTR [rax]
	mov	QWORD PTR [rbp-24], rax
	.loc 1 83 0
	mov	rax, QWORD PTR [rbp-24]
	mov	rax, QWORD PTR [rax]
	mov	QWORD PTR [rbp-24], rax
	.loc 1 84 0
	mov	rax, QWORD PTR [rbp-24]
	mov	rax, QWORD PTR [rax]
	mov	QWORD PTR [rbp-24], rax
	.loc 1 85 0
	mov	rax, QWORD PTR [rbp-24]
	mov	rax, QWORD PTR [rax]
	mov	QWORD PTR [rbp-24], rax
	.loc 1 86 0
	mov	rax, QWORD PTR [rbp-24]
	mov	rax, QWORD PTR [rax]
	mov	QWORD PTR [rbp-24], rax
	.loc 1 87 0
	mov	rax, QWORD PTR [rbp-24]
	mov	rax, QWORD PTR [rax]
	mov	QWORD PTR [rbp-24], rax
	.loc 1 88 0
	mov	rax, QWORD PTR [rbp-24]
	mov	rax, QWORD PTR [rax]
	mov	QWORD PTR [rbp-24], rax
	.loc 1 89 0
	call	timestamp_stop
	mov	QWORD PTR [rbp-8], rax
	.loc 1 93 0
	mov	rax, QWORD PTR [rbp-8]
	sub	rax, QWORD PTR [rbp-16]
	mov	QWORD PTR [rbp-104], rax
	.loc 1 95 0
	mov	rax, QWORD PTR [rbp-104]
	mov	rsi, rax
	mov	edi, OFFSET FLAT:.LC1
	mov	eax, 0
	call	printf
	.loc 1 99 0
	mov	eax, 0
	.loc 1 100 0
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	main, .-main
.Letext0:
	.file 2 "/usr/lib/gcc/x86_64-linux-gnu/5/include/stddef.h"
	.file 3 "/usr/include/x86_64-linux-gnu/bits/types.h"
	.file 4 "/usr/include/stdio.h"
	.file 5 "/usr/include/libio.h"
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.long	0x45a
	.value	0x4
	.long	.Ldebug_abbrev0
	.byte	0x8
	.uleb128 0x1
	.long	.LASF66
	.byte	0xc
	.long	.LASF67
	.long	.LASF68
	.quad	.Ltext0
	.quad	.Letext0-.Ltext0
	.long	.Ldebug_line0
	.uleb128 0x2
	.long	.LASF7
	.byte	0x2
	.byte	0xd8
	.long	0x38
	.uleb128 0x3
	.byte	0x8
	.byte	0x7
	.long	.LASF0
	.uleb128 0x3
	.byte	0x1
	.byte	0x8
	.long	.LASF1
	.uleb128 0x3
	.byte	0x2
	.byte	0x7
	.long	.LASF2
	.uleb128 0x3
	.byte	0x4
	.byte	0x7
	.long	.LASF3
	.uleb128 0x3
	.byte	0x1
	.byte	0x6
	.long	.LASF4
	.uleb128 0x3
	.byte	0x2
	.byte	0x5
	.long	.LASF5
	.uleb128 0x4
	.byte	0x4
	.byte	0x5
	.string	"int"
	.uleb128 0x3
	.byte	0x8
	.byte	0x5
	.long	.LASF6
	.uleb128 0x2
	.long	.LASF8
	.byte	0x3
	.byte	0x83
	.long	0x69
	.uleb128 0x2
	.long	.LASF9
	.byte	0x3
	.byte	0x84
	.long	0x69
	.uleb128 0x3
	.byte	0x8
	.byte	0x7
	.long	.LASF10
	.uleb128 0x5
	.byte	0x8
	.uleb128 0x6
	.byte	0x8
	.long	0x95
	.uleb128 0x3
	.byte	0x1
	.byte	0x6
	.long	.LASF11
	.uleb128 0x2
	.long	.LASF12
	.byte	0x4
	.byte	0x30
	.long	0xa7
	.uleb128 0x7
	.long	.LASF42
	.byte	0xd8
	.byte	0x5
	.byte	0xf5
	.long	0x227
	.uleb128 0x8
	.long	.LASF13
	.byte	0x5
	.byte	0xf6
	.long	0x62
	.byte	0
	.uleb128 0x8
	.long	.LASF14
	.byte	0x5
	.byte	0xfb
	.long	0x8f
	.byte	0x8
	.uleb128 0x8
	.long	.LASF15
	.byte	0x5
	.byte	0xfc
	.long	0x8f
	.byte	0x10
	.uleb128 0x8
	.long	.LASF16
	.byte	0x5
	.byte	0xfd
	.long	0x8f
	.byte	0x18
	.uleb128 0x8
	.long	.LASF17
	.byte	0x5
	.byte	0xfe
	.long	0x8f
	.byte	0x20
	.uleb128 0x8
	.long	.LASF18
	.byte	0x5
	.byte	0xff
	.long	0x8f
	.byte	0x28
	.uleb128 0x9
	.long	.LASF19
	.byte	0x5
	.value	0x100
	.long	0x8f
	.byte	0x30
	.uleb128 0x9
	.long	.LASF20
	.byte	0x5
	.value	0x101
	.long	0x8f
	.byte	0x38
	.uleb128 0x9
	.long	.LASF21
	.byte	0x5
	.value	0x102
	.long	0x8f
	.byte	0x40
	.uleb128 0x9
	.long	.LASF22
	.byte	0x5
	.value	0x104
	.long	0x8f
	.byte	0x48
	.uleb128 0x9
	.long	.LASF23
	.byte	0x5
	.value	0x105
	.long	0x8f
	.byte	0x50
	.uleb128 0x9
	.long	.LASF24
	.byte	0x5
	.value	0x106
	.long	0x8f
	.byte	0x58
	.uleb128 0x9
	.long	.LASF25
	.byte	0x5
	.value	0x108
	.long	0x25f
	.byte	0x60
	.uleb128 0x9
	.long	.LASF26
	.byte	0x5
	.value	0x10a
	.long	0x265
	.byte	0x68
	.uleb128 0x9
	.long	.LASF27
	.byte	0x5
	.value	0x10c
	.long	0x62
	.byte	0x70
	.uleb128 0x9
	.long	.LASF28
	.byte	0x5
	.value	0x110
	.long	0x62
	.byte	0x74
	.uleb128 0x9
	.long	.LASF29
	.byte	0x5
	.value	0x112
	.long	0x70
	.byte	0x78
	.uleb128 0x9
	.long	.LASF30
	.byte	0x5
	.value	0x116
	.long	0x46
	.byte	0x80
	.uleb128 0x9
	.long	.LASF31
	.byte	0x5
	.value	0x117
	.long	0x54
	.byte	0x82
	.uleb128 0x9
	.long	.LASF32
	.byte	0x5
	.value	0x118
	.long	0x26b
	.byte	0x83
	.uleb128 0x9
	.long	.LASF33
	.byte	0x5
	.value	0x11c
	.long	0x27b
	.byte	0x88
	.uleb128 0x9
	.long	.LASF34
	.byte	0x5
	.value	0x125
	.long	0x7b
	.byte	0x90
	.uleb128 0x9
	.long	.LASF35
	.byte	0x5
	.value	0x12e
	.long	0x8d
	.byte	0x98
	.uleb128 0x9
	.long	.LASF36
	.byte	0x5
	.value	0x12f
	.long	0x8d
	.byte	0xa0
	.uleb128 0x9
	.long	.LASF37
	.byte	0x5
	.value	0x130
	.long	0x8d
	.byte	0xa8
	.uleb128 0x9
	.long	.LASF38
	.byte	0x5
	.value	0x131
	.long	0x8d
	.byte	0xb0
	.uleb128 0x9
	.long	.LASF39
	.byte	0x5
	.value	0x132
	.long	0x2d
	.byte	0xb8
	.uleb128 0x9
	.long	.LASF40
	.byte	0x5
	.value	0x134
	.long	0x62
	.byte	0xc0
	.uleb128 0x9
	.long	.LASF41
	.byte	0x5
	.value	0x136
	.long	0x281
	.byte	0xc4
	.byte	0
	.uleb128 0xa
	.long	.LASF69
	.byte	0x5
	.byte	0x9a
	.uleb128 0x7
	.long	.LASF43
	.byte	0x18
	.byte	0x5
	.byte	0xa0
	.long	0x25f
	.uleb128 0x8
	.long	.LASF44
	.byte	0x5
	.byte	0xa1
	.long	0x25f
	.byte	0
	.uleb128 0x8
	.long	.LASF45
	.byte	0x5
	.byte	0xa2
	.long	0x265
	.byte	0x8
	.uleb128 0x8
	.long	.LASF46
	.byte	0x5
	.byte	0xa6
	.long	0x62
	.byte	0x10
	.byte	0
	.uleb128 0x6
	.byte	0x8
	.long	0x22e
	.uleb128 0x6
	.byte	0x8
	.long	0xa7
	.uleb128 0xb
	.long	0x95
	.long	0x27b
	.uleb128 0xc
	.long	0x86
	.byte	0
	.byte	0
	.uleb128 0x6
	.byte	0x8
	.long	0x227
	.uleb128 0xb
	.long	0x95
	.long	0x291
	.uleb128 0xc
	.long	0x86
	.byte	0x13
	.byte	0
	.uleb128 0x3
	.byte	0x8
	.byte	0x5
	.long	.LASF47
	.uleb128 0x3
	.byte	0x8
	.byte	0x7
	.long	.LASF48
	.uleb128 0xd
	.long	.LASF70
	.byte	0x1
	.byte	0xe
	.long	0x62
	.quad	.LFB2
	.quad	.LFE2-.LFB2
	.uleb128 0x1
	.byte	0x9c
	.long	0x440
	.uleb128 0xe
	.long	.LASF49
	.byte	0x1
	.byte	0xe
	.long	0x62
	.uleb128 0x3
	.byte	0x91
	.sleb128 -148
	.uleb128 0xe
	.long	.LASF50
	.byte	0x1
	.byte	0xe
	.long	0x440
	.uleb128 0x3
	.byte	0x91
	.sleb128 -160
	.uleb128 0xf
	.string	"tt"
	.byte	0x1
	.byte	0x12
	.long	0x38
	.uleb128 0x3
	.byte	0x91
	.sleb128 -120
	.uleb128 0xf
	.string	"tt2"
	.byte	0x1
	.byte	0x12
	.long	0x38
	.uleb128 0x3
	.byte	0x91
	.sleb128 -112
	.uleb128 0xf
	.string	"tt3"
	.byte	0x1
	.byte	0x12
	.long	0x38
	.uleb128 0x3
	.byte	0x91
	.sleb128 -104
	.uleb128 0xf
	.string	"tt4"
	.byte	0x1
	.byte	0x12
	.long	0x38
	.uleb128 0x3
	.byte	0x91
	.sleb128 -96
	.uleb128 0xf
	.string	"tt5"
	.byte	0x1
	.byte	0x12
	.long	0x38
	.uleb128 0x3
	.byte	0x91
	.sleb128 -88
	.uleb128 0x10
	.long	.LASF51
	.byte	0x1
	.byte	0x13
	.long	0x38
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0xf
	.string	"end"
	.byte	0x1
	.byte	0x13
	.long	0x38
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x11
	.long	.LASF52
	.byte	0x1
	.byte	0x14
	.long	0x38
	.uleb128 0x11
	.long	.LASF53
	.byte	0x1
	.byte	0x14
	.long	0x38
	.uleb128 0x11
	.long	.LASF54
	.byte	0x1
	.byte	0x15
	.long	0x38
	.uleb128 0x11
	.long	.LASF55
	.byte	0x1
	.byte	0x15
	.long	0x38
	.uleb128 0x11
	.long	.LASF56
	.byte	0x1
	.byte	0x16
	.long	0x38
	.uleb128 0x11
	.long	.LASF57
	.byte	0x1
	.byte	0x16
	.long	0x38
	.uleb128 0x11
	.long	.LASF58
	.byte	0x1
	.byte	0x17
	.long	0x38
	.uleb128 0x11
	.long	.LASF59
	.byte	0x1
	.byte	0x17
	.long	0x38
	.uleb128 0xf
	.string	"x"
	.byte	0x1
	.byte	0x18
	.long	0x38
	.uleb128 0x3
	.byte	0x91
	.sleb128 -80
	.uleb128 0x11
	.long	.LASF60
	.byte	0x1
	.byte	0x1a
	.long	0x446
	.uleb128 0x11
	.long	.LASF61
	.byte	0x1
	.byte	0x1b
	.long	0x446
	.uleb128 0x11
	.long	.LASF62
	.byte	0x1
	.byte	0x1c
	.long	0x446
	.uleb128 0xf
	.string	"ch"
	.byte	0x1
	.byte	0x1e
	.long	0x95
	.uleb128 0x3
	.byte	0x91
	.sleb128 -129
	.uleb128 0x12
	.string	"fp"
	.byte	0x1
	.byte	0x1f
	.long	0x457
	.uleb128 0x10
	.long	.LASF63
	.byte	0x1
	.byte	0x21
	.long	0x2d
	.uleb128 0x3
	.byte	0x91
	.sleb128 -72
	.uleb128 0x10
	.long	.LASF64
	.byte	0x1
	.byte	0x22
	.long	0x62
	.uleb128 0x3
	.byte	0x91
	.sleb128 -128
	.uleb128 0x10
	.long	.LASF65
	.byte	0x1
	.byte	0x23
	.long	0x62
	.uleb128 0x3
	.byte	0x91
	.sleb128 -124
	.uleb128 0xf
	.string	"B"
	.byte	0x1
	.byte	0x26
	.long	0x446
	.uleb128 0x2
	.byte	0x91
	.sleb128 -64
	.uleb128 0xf
	.string	"C"
	.byte	0x1
	.byte	0x27
	.long	0x44c
	.uleb128 0x2
	.byte	0x91
	.sleb128 -56
	.uleb128 0xf
	.string	"D"
	.byte	0x1
	.byte	0x28
	.long	0x44c
	.uleb128 0x2
	.byte	0x91
	.sleb128 -48
	.uleb128 0xf
	.string	"E"
	.byte	0x1
	.byte	0x3d
	.long	0x446
	.uleb128 0x2
	.byte	0x91
	.sleb128 -40
	.byte	0
	.uleb128 0x6
	.byte	0x8
	.long	0x8f
	.uleb128 0x6
	.byte	0x8
	.long	0x44c
	.uleb128 0x6
	.byte	0x8
	.long	0x452
	.uleb128 0x13
	.long	0x95
	.uleb128 0x6
	.byte	0x8
	.long	0x9c
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
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
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
	.uleb128 0xe
	.byte	0
	.byte	0
	.uleb128 0x4
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
	.uleb128 0x5
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x7
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x8
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x9
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0xa
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0xb
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xc
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0xd
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
	.uleb128 0x2116
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xe
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
	.uleb128 0xf
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
	.uleb128 0x10
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
	.uleb128 0x11
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
	.byte	0
	.byte	0
	.uleb128 0x12
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
	.byte	0
	.byte	0
	.uleb128 0x13
	.uleb128 0x35
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
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
.LASF53:
	.string	"end2"
.LASF55:
	.string	"end3"
.LASF57:
	.string	"end4"
.LASF29:
	.string	"_old_offset"
.LASF66:
	.string	"GNU C11 5.2.1 20151010 -masm=intel -mtune=generic -march=x86-64 -g -O0 -fstack-protector-strong"
.LASF24:
	.string	"_IO_save_end"
.LASF5:
	.string	"short int"
.LASF7:
	.string	"size_t"
.LASF10:
	.string	"sizetype"
.LASF34:
	.string	"_offset"
.LASF58:
	.string	"begin5"
.LASF51:
	.string	"begin"
.LASF18:
	.string	"_IO_write_ptr"
.LASF70:
	.string	"main"
.LASF65:
	.string	"mem_length_ptr"
.LASF52:
	.string	"begin2"
.LASF54:
	.string	"begin3"
.LASF56:
	.string	"begin4"
.LASF64:
	.string	"mem_length_char"
.LASF25:
	.string	"_markers"
.LASF59:
	.string	"end5"
.LASF68:
	.string	"/home/user/Documents/github/ctattack"
.LASF47:
	.string	"long long int"
.LASF33:
	.string	"_lock"
.LASF67:
	.string	"src/ssa/cachemon_v6.c"
.LASF6:
	.string	"long int"
.LASF30:
	.string	"_cur_column"
.LASF46:
	.string	"_pos"
.LASF50:
	.string	"argv"
.LASF45:
	.string	"_sbuf"
.LASF42:
	.string	"_IO_FILE"
.LASF1:
	.string	"unsigned char"
.LASF49:
	.string	"argc"
.LASF4:
	.string	"signed char"
.LASF48:
	.string	"long long unsigned int"
.LASF43:
	.string	"_IO_marker"
.LASF32:
	.string	"_shortbuf"
.LASF9:
	.string	"__off64_t"
.LASF17:
	.string	"_IO_write_base"
.LASF41:
	.string	"_unused2"
.LASF14:
	.string	"_IO_read_ptr"
.LASF21:
	.string	"_IO_buf_end"
.LASF11:
	.string	"char"
.LASF44:
	.string	"_next"
.LASF35:
	.string	"__pad1"
.LASF36:
	.string	"__pad2"
.LASF37:
	.string	"__pad3"
.LASF38:
	.string	"__pad4"
.LASF39:
	.string	"__pad5"
.LASF61:
	.string	"tmp2"
.LASF62:
	.string	"tmp3"
.LASF2:
	.string	"short unsigned int"
.LASF60:
	.string	"tmp1"
.LASF0:
	.string	"long unsigned int"
.LASF19:
	.string	"_IO_write_end"
.LASF8:
	.string	"__off_t"
.LASF26:
	.string	"_chain"
.LASF23:
	.string	"_IO_backup_base"
.LASF20:
	.string	"_IO_buf_base"
.LASF28:
	.string	"_flags2"
.LASF40:
	.string	"_mode"
.LASF16:
	.string	"_IO_read_base"
.LASF31:
	.string	"_vtable_offset"
.LASF22:
	.string	"_IO_save_base"
.LASF27:
	.string	"_fileno"
.LASF12:
	.string	"FILE"
.LASF3:
	.string	"unsigned int"
.LASF15:
	.string	"_IO_read_end"
.LASF13:
	.string	"_flags"
.LASF63:
	.string	"mem_length"
.LASF69:
	.string	"_IO_lock_t"
	.ident	"GCC: (Ubuntu 5.2.1-22ubuntu2) 5.2.1 20151010"
	.section	.note.GNU-stack,"",@progbits
