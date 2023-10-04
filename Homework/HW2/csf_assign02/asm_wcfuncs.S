/*
 * Assembly language function implementations
 */

#define MAX_WORDLEN 63

/*
 * Offsets for the fields of the struct WordEntry data type
 * (Note that there is 4 bytes of padding between the count and next
 * fields.)
 */
#define WORDENTRY_WORD_OFFSET   (0)
#define WORDENTRY_COUNT_OFFSET  (MAX_WORDLEN+1)
#define WORDENTRY_NEXT_OFFSET   (MAX_WORDLEN+1+4+4)
#define WORDENTRY_SIZE          (WORDENTRY_NEXT_OFFSET+8)

	.section .rodata
/* Define any string constants or read-only data here */

	.section .text

/*
 * Compute a hash code for the given NUL-terminated
 * character string.
 *
 * The hash algorithm should be implemented as follows:
 *
 * uint32_t hash_code = 5381
 * for each character c of w in order {
 *   hash_code = hash_code * 33 + c
 * }
 *
 * Note that the character values should be treated as
 * being unsigned (in the range 0..255)
 *
 * C function prototype:
 *    uint32_t wc_hash(const unsigned char *w);
 */
	.globl wc_hash
wc_hash:
	movl $5381, %r8d          /* Initialize register to hold final hashcode */
    movl $5381, %r9d          /* Preserve a copy of hashcode for future calculations  */  
	jmp .HashCond            /* Check condition */ 
.HashLoop:
	shll $5, %r8d             /* Bit shift left by 5 is equal to multiplying by 32 */  
	addl %r9d, %r8d            /* Add initial hashcode to left shifted version to equal multiplying by 33 */ 
	movzbl (%rdi), %r10d      /* Copy over char byte and zero extend it for calculation */
    addl %r10d, %r8d           /* Add char value to hashcode */ 
	movl %r8d, %r9d            /* Copy new value of hashcode to r9d for future calculation */
	inc %rdi                 /* Advance to next char */ 
.HashCond:
	cmpb $0, (%rdi)          /* Check if reached null terminator */ 
	jne .HashLoop            /* If not, jump back into loop */ 
	movl %r8d, %eax           /* Otherwise, return final hashcode to %eax */ 
	ret

/*
 * Compare two strings lexicographically. Return
 *
 * - a negative value if lhs string is less than rhs string
 * - 0 if lhs string is identical to rhs string
 * - a positive value if lhs string is greater than rhs string
 *
 * Lexicographical comparison is a generalization of alphabetical
 * order, but using character codes. If one string is a prefix
 * of the other, it is considered as "less than". E.g.,
 * "hi" would compare as less than "high".
 *
 * C function prototype:
 *    int wc_str_compare(const unsigned char *lhs, const unsigned char *rhs);
 */
	.globl wc_str_compare //need to complete this
wc_str_compare:
.StrCmpLoop:
	movzbl (%rdi), %r8d        /* Copies char from lhs and converts into integer in r8d */
	movzbl (%rsi), %r9d        /* Copies char from rhs and converts into integer in r9d */
	cmpl %r9d, %r8d            /* compare the two bytes */
	jne .NotEqual              /* if not equal, determine order */
	testl %r8d, %r8d           /* check if reached the null terminator of lhs */
	je .AreEqual               /* if yes, strings are equal or lhs is a prefix */
	inc %rdi                   /* move to next character of lhs */
	inc %rsi                   /* move to next character of rhs */
	jmp .StrCmpLoop
.NotEqual:
	subl %r9d, %r8d            /* calculate difference */
	movl %r8d, %eax            /* store difference in eax for return */
	ret
.AreEqual:
	movl $0, %eax              /* strings are identical, return 0 */
	ret 

/*
 * Copy NUL-terminated source string to the destination buffer.
 *
 * C function prototype:
 *    void wc_str_copy(unsigned char *dest, const unsigned char *source);
 */
	.globl wc_str_copy
wc_str_copy:
.StrCpyLoop:
	movb (%rsi), %r8b           /* Copies char from source and adds it to buffer register */
	movb %r8b, (%rdi)           /* Copies char from buffer register to destination */ 
	inc %rsi                   /* Increment to next character in source */ 
	inc %rdi                   /* Increment to next memory location in destination */
	cmpb $0, (%rsi)            /* Check if source has reached null terminator */ 
	jne .StrCpyLoop            /* If not, loop again */ 
ret

/*
 * Return 1 if the character code in c is a whitespace character,
 * false otherwise.
 *
 * For the purposes of this function, a whitespace character is one of
 *
 *   ' '
 *   '\t'
 *   '\r'
 *   '\n'
 *   '\f'
 *   '\v'
 *
 * C function prototype:
 *    int wc_isspace(unsigned char c);
 */
	.globl wc_isspace
wc_isspace:
	cmpl $9, %edi  /* Compare with horizontal tab */
	je .IsSpace    /* Jump to Is Space */
	cmpl $10, %edi /* Compare with new line */
	je .IsSpace    /* Jump to Is Space */
	cmpl $11, %edi /* Compare with vertical tab */
	je .IsSpace    /* Jump to Is Space */
	cmpl $12, %edi /* Compare to end of page */
	je .IsSpace    /* Jump to Is Space */
	cmpl $13, %edi /* Compare with carriage return */
	je .IsSpace    /* Jump to Is Space */
	cmpl $32, %edi /* Compare with space character */
	je .IsSpace    /* Jump to Is Space */
	movl $0, %eax  /* Default for non-spaces to return 0 */
	ret             
.IsSpace:
	movl $1, %eax  /* Return 1 for space */
	ret

/*
 * Return 1 if the character code in c is an alphabetic character
 * ('A' through 'Z' or 'a' through 'z'), 0 otherwise.
 *
 * C function prototype:
 *    int wc_isalpha(unsigned char c);
 */


	.globl wc_isalpha
wc_isalpha:
	cmpl $65, %edi   /* Compare with 'A' */
	jb .NotAlpha      /* Not alpha if below 'A' */
	cmpl $90, %edi   /* Compare with 'Z' */
	jbe .IsAlpha      /* Is alpha since above 'A' and below or equal to 'Z' */
	cmpl $97, %edi   /* Compare with 'a' */
	jb .NotAlpha      /* Not alpha if above 'Z' and below 'a' */
	cmpl $122, %edi   /* Compare with 'z' */
	jbe .IsAlpha      /* Is alpha if below or equal to 'z' and above/equal to 'a'  */
	jmp .NotAlpha     /* Is not alpha if above 'z' */
.IsAlpha:
	movl $1, %eax     /* Set return value to 1 */ 
	ret
.NotAlpha:
	movl $0, %eax     /* Set return value to 0 */
	ret

/*
 * Read the next word from given input stream, storing
 * the word in the array pointed-to by w. (This array should be
 * assumed to be MAX_WORDLEN+1 elements in size.) Return 1
 * if a word is read successfully, 0 otherwise.
 *
 * For the purposes of this function, a word is a sequence of
 * 1 or more non-whitespace characters.
 *
 * If a sequence of non-whitespace characters has more than
 * MAX_WORDLEN characters, then only the first MAX_WORDLEN
 * characters in the sequence should be stored in the array.
 *
 * C function prototype:
 *    int wc_readnext(FILE *in, unsigned char *w);
 */
	.globl wc_readnext
wc_readnext:
pushq   %rbp
    movq    %rsp, %rbp
    pushq   %r12
    pushq   %rbx
    pushq   %r13
    subq    $32, %rsp

    movq    %rdi, -24(%rbp)  /* FILE* in */
    movq    %rsi, -32(%rbp)  /* char* w */
    movl    $0, -4(%rbp)     /* int i */

    jmp     .LcheckEOF

.Lloop:
    /* Check if character is a whitespace */
    movzbl  -5(%rbp), %edi   /* Load current char into %edi */
    call    wc_isspace
    testl   %eax, %eax
    jne     .Lwhitespace
    /* If not whitespace, process character */
    movl    -4(%rbp), %eax   /* Load i */
    cmpq    $63, %rax        /* Compare with MAX_WORDLEN */
    jae     .LskipChar       /* If >= MAX_WORDLEN, skip char */
    movl    -4(%rbp), %edx
    movq    -32(%rbp), %rax
    addq    %rdx, %rax
    movzbl  -5(%rbp), %edx
    movb    %dl, (%rax)
    addl    $1, -4(%rbp)

.LskipChar:
    call    fgetc@PLT
    movb    %al, -5(%rbp)

.LcheckEOF:
    cmpb    $-1, -5(%rbp)
    jne     .Lloop
    jmp     .Lend

.Lwhitespace:
    cmpl    $0, -4(%rbp)
    jne     .LwordEnd
    /* If whitespace and i == 0, fetch next character */
    call    fgetc@PLT
    movb    %al, -5(%rbp)
    jmp     .LcheckEOF

.LwordEnd:
    movl    -4(%rbp), %edx
    movq    -32(%rbp), %rax
    addq    %rdx, %rax
    movb    $0, (%rax)
    movl    $1, %eax
    jmp     .Lexit

.Lend:
    /* End of file reached */
    movl    $0, %eax

.Lexit:
    addq    $32, %rsp
    popq    %r13
    popq    %rbx
    popq    %r12
    leave
	ret

/*
 * Convert the NUL-terminated character string in the array
 * pointed-to by w so that every letter is lower-case.
 *
 * C function prototype:
 *    void wc_tolower(unsigned char *w);
 */
	.globl wc_tolower
wc_tolower:
	subq $8, %rsp          /* Align stack */
.LowLoop:
	movb (%rdi), %r10b     /* Copy over char byte from word */  
	cmpb $0, %r10b         /* Detect if reached null character */
	je .LowDone            /* End loop if detect null character */

    cmpb $65, %r10b   /* Compare with 'A' */
	jb .LowNext       /* Skip if not within uppercase range */
	cmpb $90, %r10b   /* Compare with 'Z' */
	ja .LowNext       /* Skip if not within uppercase range */   

	addb $32, %r10b     /* Convert uppercase to lowercase */ 
	movb %r10b, (%rdi)  /* Assign converted character to previous memory location */ 
.LowNext:
	incq %rdi           /* Advance pointer to next character */
	jmp .LowLoop        /* Re enter loop */ 
.LowDone:
	addq $8, %rsp          /* Dealign stack */ 
	ret 


/*
 * Remove any non-alphaabetic characters from the end of the
 * NUL-terminated character string pointed-to by w.
 *
 * C function prototype:
 *    void wc_trim_non_alpha(unsigned char *w);
 */
	.globl wc_trim_non_alpha
wc_trim_non_alpha:
	/* TODO: implement */
	ret

/*
 * Search the specified linked list of WordEntry objects for an object
 * containing the specified string.
 *
 * If a matching object is found, set the int variable pointed-to by
 * inserted to 0 and return a pointer to the matching object.
 *
 * If a matching object is not found, allocate a new WordEntry object,
 * set its next pointer to point to what head points to (i.e., so the
 * new object is at the head of the list), set the variable pointed-to
 * by inserted to 1, and return a pointer to the new node. Note that
 * the new node should have its count value set to 0. (It is the caller's
 * job to update the count.)
 *
 * C function prototype:
 *    struct WordEntry *wc_find_or_insert(struct WordEntry *head, const unsigned char *s, int *inserted);
 */
	.globl wc_find_or_insert
wc_find_or_insert:
	/* TODO: implement */
	ret

/*
 * Find or insert the WordEntry object for the given string (s), returning
 * a pointer to it. The head of the linked list which contains (or should
 * contain) the entry for s is the element of buckets whose index is the
 * hash code of s mod num_buckets.
 *
 * Returns a pointer to the WordEntry object in the appropriate linked list
 * which represents s.
 *
 * C function prototype:
 *    struct WordEntry *wc_dict_find_or_insert(struct WordEntry *buckets[], unsigned num_buckets, const unsigned char *s);
 */
	.globl wc_dict_find_or_insert
wc_dict_find_or_insert:
	/* TODO: implement */
	ret

/*
 * Free all of the nodes in given linked list of WordEntry objects.
 *
 * C function prototype:
 *    void wc_free_chain(struct WordEntry *p);
 */
	.globl wc_free_chain
wc_free_chain:
	/* TODO: implement */
	ret

/*
vim:ft=gas:
*/
