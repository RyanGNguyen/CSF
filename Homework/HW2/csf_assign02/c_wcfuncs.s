	.file	"c_wcfuncs.c"
	.text
	.globl	wc_hash
	.type	wc_hash, @function
wc_hash:
.LFB6:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movl	$5381, -4(%rbp)
	jmp	.L2
.L3:
	movl	-4(%rbp), %eax
	movl	%eax, %edx
	sall	$5, %edx
	addl	%eax, %edx
	movq	-24(%rbp), %rax
	movzbl	(%rax), %eax
	movzbl	%al, %eax
	addl	%edx, %eax
	movl	%eax, -4(%rbp)
	addq	$1, -24(%rbp)
.L2:
	movq	-24(%rbp), %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L3
	movl	-4(%rbp), %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	wc_hash, .-wc_hash
	.globl	wc_str_compare
	.type	wc_str_compare, @function
wc_str_compare:
.LFB7:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	jmp	.L6
.L8:
	addq	$1, -8(%rbp)
	addq	$1, -16(%rbp)
.L6:
	movq	-8(%rbp), %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	je	.L7
	movq	-8(%rbp), %rax
	movzbl	(%rax), %edx
	movq	-16(%rbp), %rax
	movzbl	(%rax), %eax
	cmpb	%al, %dl
	je	.L8
.L7:
	movq	-8(%rbp), %rax
	movzbl	(%rax), %eax
	movzbl	%al, %edx
	movq	-16(%rbp), %rax
	movzbl	(%rax), %eax
	movzbl	%al, %ecx
	movl	%edx, %eax
	subl	%ecx, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	wc_str_compare, .-wc_str_compare
	.globl	wc_str_copy
	.type	wc_str_copy, @function
wc_str_copy:
.LFB8:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	jmp	.L11
.L12:
	movq	-16(%rbp), %rax
	movzbl	(%rax), %edx
	movq	-8(%rbp), %rax
	movb	%dl, (%rax)
	addq	$1, -8(%rbp)
	addq	$1, -16(%rbp)
.L11:
	movq	-16(%rbp), %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L12
	movq	-8(%rbp), %rax
	movb	$0, (%rax)
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	wc_str_copy, .-wc_str_copy
	.globl	wc_isspace
	.type	wc_isspace, @function
wc_isspace:
.LFB9:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, %eax
	movb	%al, -4(%rbp)
	movzbl	-4(%rbp), %eax
	cmpl	$13, %eax
	jg	.L14
	cmpl	$9, %eax
	jge	.L15
	jmp	.L16
.L14:
	cmpl	$32, %eax
	jne	.L16
.L15:
	movl	$1, %eax
	jmp	.L17
.L16:
	movl	$0, %eax
.L17:
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE9:
	.size	wc_isspace, .-wc_isspace
	.globl	wc_isalpha
	.type	wc_isalpha, @function
wc_isalpha:
.LFB10:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, %eax
	movb	%al, -4(%rbp)
	cmpb	$64, -4(%rbp)
	jbe	.L19
	cmpb	$90, -4(%rbp)
	jbe	.L20
.L19:
	cmpb	$96, -4(%rbp)
	jbe	.L21
	cmpb	$122, -4(%rbp)
	ja	.L21
.L20:
	movl	$1, %eax
	jmp	.L22
.L21:
	movl	$0, %eax
.L22:
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE10:
	.size	wc_isalpha, .-wc_isalpha
	.globl	wc_readnext
	.type	wc_readnext, @function
wc_readnext:
.LFB11:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movl	$0, -4(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	fgetc@PLT
	movb	%al, -5(%rbp)
	jmp	.L24
.L28:
	movzbl	-5(%rbp), %eax
	movzbl	%al, %eax
	movl	%eax, %edi
	call	wc_isspace
	testl	%eax, %eax
	je	.L25
	cmpl	$0, -4(%rbp)
	je	.L26
	movl	-4(%rbp), %edx
	movq	-32(%rbp), %rax
	addq	%rdx, %rax
	movb	$0, (%rax)
	movl	$1, %eax
	jmp	.L27
.L25:
	movl	-4(%rbp), %edx
	movq	-32(%rbp), %rax
	addq	%rax, %rdx
	movzbl	-5(%rbp), %eax
	movb	%al, (%rdx)
	addl	$1, -4(%rbp)
.L26:
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	fgetc@PLT
	movb	%al, -5(%rbp)
.L24:
	cmpb	$-1, -5(%rbp)
	jne	.L28
	cmpl	$0, -4(%rbp)
	je	.L29
	movl	-4(%rbp), %edx
	movq	-32(%rbp), %rax
	addq	%rdx, %rax
	movb	$0, (%rax)
	movl	$1, %eax
	jmp	.L27
.L29:
	movl	$0, %eax
.L27:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE11:
	.size	wc_readnext, .-wc_readnext
	.globl	wc_tolower
	.type	wc_tolower, @function
wc_tolower:
.LFB12:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	jmp	.L31
.L33:
	movq	-8(%rbp), %rax
	movzbl	(%rax), %eax
	cmpb	$64, %al
	jbe	.L32
	movq	-8(%rbp), %rax
	movzbl	(%rax), %eax
	cmpb	$90, %al
	ja	.L32
	movq	-8(%rbp), %rax
	movzbl	(%rax), %eax
	leal	32(%rax), %edx
	movq	-8(%rbp), %rax
	movb	%dl, (%rax)
.L32:
	addq	$1, -8(%rbp)
.L31:
	movq	-8(%rbp), %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L33
	nop
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE12:
	.size	wc_tolower, .-wc_tolower
	.globl	wc_trim_non_alpha
	.type	wc_trim_non_alpha, @function
wc_trim_non_alpha:
.LFB13:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$8, %rsp
	movq	%rdi, -8(%rbp)
	jmp	.L35
.L36:
	addq	$1, -8(%rbp)
.L35:
	movq	-8(%rbp), %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L36
.L37:
	movq	-8(%rbp), %rax
	movb	$0, (%rax)
	subq	$1, -8(%rbp)
	movq	-8(%rbp), %rax
	movzbl	(%rax), %eax
	movzbl	%al, %eax
	movl	%eax, %edi
	call	wc_isalpha
	testl	%eax, %eax
	je	.L37
	nop
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE13:
	.size	wc_trim_non_alpha, .-wc_trim_non_alpha
	.globl	wc_find_or_insert
	.type	wc_find_or_insert, @function
wc_find_or_insert:
.LFB14:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	%rdx, -40(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, -16(%rbp)
	jmp	.L39
.L42:
	movq	-16(%rbp), %rax
	movq	-32(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	wc_str_compare
	testl	%eax, %eax
	jne	.L40
	movq	-40(%rbp), %rax
	movl	$0, (%rax)
	movq	-16(%rbp), %rax
	jmp	.L41
.L40:
	movq	-16(%rbp), %rax
	movq	72(%rax), %rax
	movq	%rax, -16(%rbp)
.L39:
	cmpq	$0, -16(%rbp)
	jne	.L42
	movl	$80, %edi
	call	malloc@PLT
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	-32(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	wc_str_copy
	movq	-8(%rbp), %rax
	movl	$0, 64(%rax)
	movq	-8(%rbp), %rax
	movq	-24(%rbp), %rdx
	movq	%rdx, 72(%rax)
	movq	-40(%rbp), %rax
	movl	$1, (%rax)
	movq	-8(%rbp), %rax
.L41:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE14:
	.size	wc_find_or_insert, .-wc_find_or_insert
	.globl	wc_dict_find_or_insert
	.type	wc_dict_find_or_insert, @function
wc_dict_find_or_insert:
.LFB15:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$64, %rsp
	movq	%rdi, -40(%rbp)
	movl	%esi, -44(%rbp)
	movq	%rdx, -56(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movq	-56(%rbp), %rax
	movq	%rax, %rdi
	call	wc_hash
	movl	$0, %edx
	divl	-44(%rbp)
	movl	%edx, -20(%rbp)
	movl	-20(%rbp), %eax
	leaq	0(,%rax,8), %rdx
	movq	-40(%rbp), %rax
	addq	%rdx, %rax
	movq	(%rax), %rax
	movq	%rax, -16(%rbp)
	leaq	-24(%rbp), %rdx
	movq	-56(%rbp), %rcx
	movq	-16(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	wc_find_or_insert
	movq	%rax, -16(%rbp)
	movl	-24(%rbp), %eax
	testl	%eax, %eax
	je	.L44
	movl	-20(%rbp), %eax
	leaq	0(,%rax,8), %rdx
	movq	-40(%rbp), %rax
	addq	%rax, %rdx
	movq	-16(%rbp), %rax
	movq	%rax, (%rdx)
.L44:
	movq	-16(%rbp), %rax
	movq	-8(%rbp), %rdx
	subq	%fs:40, %rdx
	je	.L46
	call	__stack_chk_fail@PLT
.L46:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE15:
	.size	wc_dict_find_or_insert, .-wc_dict_find_or_insert
	.globl	wc_free_chain
	.type	wc_free_chain, @function
wc_free_chain:
.LFB16:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	jmp	.L48
.L49:
	movq	-24(%rbp), %rax
	movq	%rax, -8(%rbp)
	movq	-24(%rbp), %rax
	movq	72(%rax), %rax
	movq	%rax, -24(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	free@PLT
.L48:
	cmpq	$0, -24(%rbp)
	jne	.L49
	nop
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE16:
	.size	wc_free_chain, .-wc_free_chain
	.ident	"GCC: (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
