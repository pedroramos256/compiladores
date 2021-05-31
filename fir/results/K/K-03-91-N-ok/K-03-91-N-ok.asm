; BSS
segment	.bss
; ALIGN
align	4
; LABEL f
f:
; SALLOC 4
	resb	4
; TEXT
segment	.text
; ALIGN
align	4
; GLOBAL _main, :function
global	_main:function
; LABEL _main
_main:
; ENTER 28
	push	ebp
	mov	ebp, esp
	sub	esp, 28
; INT 0
	push	dword 0
; LOCAL -4
	lea	eax, [ebp+-4]
	push	eax
; STINT
	pop	ecx
	pop	eax
	mov	[ecx], eax
        ;; before function PBE 
        ;; before body 
; INT 4
	push	dword 4
; LOCAL -8
	lea	eax, [ebp+-8]
	push	eax
; STINT
	pop	ecx
	pop	eax
	mov	[ecx], eax
; LOCAL -8
	lea	eax, [ebp+-8]
	push	eax
; LDINT
	pop	eax
	push	dword [eax]
; INT 3
	push	dword 3
; SHTL
	pop	ecx
	sal	dword [esp], cl
; ALLOC
	pop	eax
	sub	esp, eax
; SP
	push	esp
; LOCAL -12
	lea	eax, [ebp+-12]
	push	eax
; STINT
	pop	ecx
	pop	eax
	mov	[ecx], eax
; INT 0
	push	dword 0
; LOCAL -16
	lea	eax, [ebp+-16]
	push	eax
; STINT
	pop	ecx
	pop	eax
	mov	[ecx], eax
        ;; WHILE condition
; ALIGN
align	4
; LABEL _L2
_L2:
; LOCAL -16
	lea	eax, [ebp+-16]
	push	eax
; LDINT
	pop	eax
	push	dword [eax]
; LOCAL -8
	lea	eax, [ebp+-8]
	push	eax
; LDINT
	pop	eax
	push	dword [eax]
; LT
	pop	eax
	xor	ecx, ecx
	cmp	[esp], eax
	setl	cl
	mov	[esp], ecx
; JZ _L3
	pop	eax
	cmp	eax, byte 0
	je	near _L3
        ;; WHILE block
; INT 2
	push	dword 2
; I2D
	fild	dword [esp]
	sub	esp, byte 4
	fstp	qword [esp]
; DUP64
; SP
	push	esp
; LDDOUBLE
	pop	eax
	push	dword [eax+4]
	push	dword [eax]
; LOCAL -12
	lea	eax, [ebp+-12]
	push	eax
; LDINT
	pop	eax
	push	dword [eax]
; LOCAL -16
	lea	eax, [ebp+-16]
	push	eax
; LDINT
	pop	eax
	push	dword [eax]
; INT 8
	push	dword 8
; MUL
	pop	eax
	imul	dword eax, [esp]
	mov	[esp], eax
; ADD
	pop	eax
	add	dword [esp], eax
; DSTORE
	pop	ecx
	pop	eax
	mov	[ecx], eax
	pop	eax
	mov	[ecx+4], eax
; TRASH 8
	add	esp, 8
; LOCAL -16
	lea	eax, [ebp+-16]
	push	eax
; LDINT
	pop	eax
	push	dword [eax]
; INT 1
	push	dword 1
; ADD
	pop	eax
	add	dword [esp], eax
; DUP32
	push	dword [esp]
; LOCAL -16
	lea	eax, [ebp+-16]
	push	eax
; STINT
	pop	ecx
	pop	eax
	mov	[ecx], eax
; TRASH 4
	add	esp, 4
; JMP _L2
	jmp	dword _L2
; LABEL _L3
_L3:
; INT 0
	push	dword 0
; LOCAL -20
	lea	eax, [ebp+-20]
	push	eax
; STINT
	pop	ecx
	pop	eax
	mov	[ecx], eax
        ;; WHILE condition
; ALIGN
align	4
; LABEL _L4
_L4:
; LOCAL -20
	lea	eax, [ebp+-20]
	push	eax
; LDINT
	pop	eax
	push	dword [eax]
; LOCAL -8
	lea	eax, [ebp+-8]
	push	eax
; LDINT
	pop	eax
	push	dword [eax]
; LT
	pop	eax
	xor	ecx, ecx
	cmp	[esp], eax
	setl	cl
	mov	[esp], ecx
; JZ _L5
	pop	eax
	cmp	eax, byte 0
	je	near _L5
        ;; WHILE block
; LOCAL -12
	lea	eax, [ebp+-12]
	push	eax
; LDINT
	pop	eax
	push	dword [eax]
; LOCAL -20
	lea	eax, [ebp+-20]
	push	eax
; LDINT
	pop	eax
	push	dword [eax]
; INT 8
	push	dword 8
; MUL
	pop	eax
	imul	dword eax, [esp]
	mov	[esp], eax
; ADD
	pop	eax
	add	dword [esp], eax
; LDDOUBLE
	pop	eax
	push	dword [eax+4]
	push	dword [eax]
; CALL printd
	call	printd
; TRASH 8
	add	esp, 8
; LOCAL -20
	lea	eax, [ebp+-20]
	push	eax
; LDINT
	pop	eax
	push	dword [eax]
; INT 1
	push	dword 1
; ADD
	pop	eax
	add	dword [esp], eax
; DUP32
	push	dword [esp]
; LOCAL -20
	lea	eax, [ebp+-20]
	push	eax
; STINT
	pop	ecx
	pop	eax
	mov	[ecx], eax
; TRASH 4
	add	esp, 4
; JMP _L4
	jmp	dword _L4
; LABEL _L5
_L5:
; RODATA
segment	.rodata
; ALIGN
align	4
; LABEL _L6
_L6:
; SSTRING (value omitted -- see below)
	db	10, 0
; TEXT
segment	.text
; ALIGN
align	4
; ADDR _L6
	push	dword $_L6
; CALL prints
	call	prints
; TRASH 4
	add	esp, 4
; INT 0
	push	dword 0
; LOCAL -24
	lea	eax, [ebp+-24]
	push	eax
; STINT
	pop	ecx
	pop	eax
	mov	[ecx], eax
        ;; WHILE condition
; ALIGN
align	4
; LABEL _L7
_L7:
; LOCAL -24
	lea	eax, [ebp+-24]
	push	eax
; LDINT
	pop	eax
	push	dword [eax]
; LOCAL -8
	lea	eax, [ebp+-8]
	push	eax
; LDINT
	pop	eax
	push	dword [eax]
; LT
	pop	eax
	xor	ecx, ecx
	cmp	[esp], eax
	setl	cl
	mov	[esp], ecx
; JZ _L8
	pop	eax
	cmp	eax, byte 0
	je	near _L8
        ;; WHILE block
; INT 6
	push	dword 6
; I2D
	fild	dword [esp]
	sub	esp, byte 4
	fstp	qword [esp]
; LOCAL -12
	lea	eax, [ebp+-12]
	push	eax
; LDINT
	pop	eax
	push	dword [eax]
; LOCAL -24
	lea	eax, [ebp+-24]
	push	eax
; LDINT
	pop	eax
	push	dword [eax]
; INT 8
	push	dword 8
; MUL
	pop	eax
	imul	dword eax, [esp]
	mov	[esp], eax
; ADD
	pop	eax
	add	dword [esp], eax
; LDDOUBLE
	pop	eax
	push	dword [eax+4]
	push	dword [eax]
; DDIV
	fld	qword [esp]
	add	esp, byte 8
	fld	qword [esp]
	fdivrp	st1
	fstp	qword [esp]
; DUP64
; SP
	push	esp
; LDDOUBLE
	pop	eax
	push	dword [eax+4]
	push	dword [eax]
; LOCAL -12
	lea	eax, [ebp+-12]
	push	eax
; LDINT
	pop	eax
	push	dword [eax]
; LOCAL -24
	lea	eax, [ebp+-24]
	push	eax
; LDINT
	pop	eax
	push	dword [eax]
; INT 8
	push	dword 8
; MUL
	pop	eax
	imul	dword eax, [esp]
	mov	[esp], eax
; ADD
	pop	eax
	add	dword [esp], eax
; DSTORE
	pop	ecx
	pop	eax
	mov	[ecx], eax
	pop	eax
	mov	[ecx+4], eax
; TRASH 8
	add	esp, 8
; LOCAL -24
	lea	eax, [ebp+-24]
	push	eax
; LDINT
	pop	eax
	push	dword [eax]
; INT 1
	push	dword 1
; ADD
	pop	eax
	add	dword [esp], eax
; DUP32
	push	dword [esp]
; LOCAL -24
	lea	eax, [ebp+-24]
	push	eax
; STINT
	pop	ecx
	pop	eax
	mov	[ecx], eax
; TRASH 4
	add	esp, 4
; JMP _L7
	jmp	dword _L7
; LABEL _L8
_L8:
; INT 0
	push	dword 0
; LOCAL -28
	lea	eax, [ebp+-28]
	push	eax
; STINT
	pop	ecx
	pop	eax
	mov	[ecx], eax
        ;; WHILE condition
; ALIGN
align	4
; LABEL _L9
_L9:
; LOCAL -28
	lea	eax, [ebp+-28]
	push	eax
; LDINT
	pop	eax
	push	dword [eax]
; LOCAL -8
	lea	eax, [ebp+-8]
	push	eax
; LDINT
	pop	eax
	push	dword [eax]
; LT
	pop	eax
	xor	ecx, ecx
	cmp	[esp], eax
	setl	cl
	mov	[esp], ecx
; JZ _L10
	pop	eax
	cmp	eax, byte 0
	je	near _L10
        ;; WHILE block
; LOCAL -12
	lea	eax, [ebp+-12]
	push	eax
; LDINT
	pop	eax
	push	dword [eax]
; LOCAL -28
	lea	eax, [ebp+-28]
	push	eax
; LDINT
	pop	eax
	push	dword [eax]
; INT 8
	push	dword 8
; MUL
	pop	eax
	imul	dword eax, [esp]
	mov	[esp], eax
; ADD
	pop	eax
	add	dword [esp], eax
; LDDOUBLE
	pop	eax
	push	dword [eax+4]
	push	dword [eax]
; CALL printd
	call	printd
; TRASH 8
	add	esp, 8
; LOCAL -28
	lea	eax, [ebp+-28]
	push	eax
; LDINT
	pop	eax
	push	dword [eax]
; INT 1
	push	dword 1
; ADD
	pop	eax
	add	dword [esp], eax
; DUP32
	push	dword [esp]
; LOCAL -28
	lea	eax, [ebp+-28]
	push	eax
; STINT
	pop	ecx
	pop	eax
	mov	[ecx], eax
; TRASH 4
	add	esp, 4
; JMP _L9
	jmp	dword _L9
; LABEL _L10
_L10:
        ;; after body 
; LABEL _L1
_L1:
        ;; after function PBE 
; LOCAL -4
	lea	eax, [ebp+-4]
	push	eax
; LDINT
	pop	eax
	push	dword [eax]
; STFVAL32
	pop	eax
; LEAVE
	leave
; RET
	ret
; EXTERN printd
extern	printd
; EXTERN prints
extern	prints
