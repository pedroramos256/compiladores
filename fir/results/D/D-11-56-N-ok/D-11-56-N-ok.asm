; BSS
segment	.bss
; ALIGN
align	4
; LABEL y
y:
; SALLOC 4
	resb	4
; BSS
segment	.bss
; ALIGN
align	4
; LABEL x
x:
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
; ENTER 4
	push	ebp
	mov	ebp, esp
	sub	esp, 4
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
; INT 1
	push	dword 1
; DUP32
	push	dword [esp]
; ADDR y
	push	dword $y
; STINT
	pop	ecx
	pop	eax
	mov	[ecx], eax
; TRASH 4
	add	esp, 4
        ;; WHILE condition
; ALIGN
align	4
; LABEL _L2
_L2:
; ADDR y
	push	dword $y
; LDINT
	pop	eax
	push	dword [eax]
; INT 3
	push	dword 3
; LE
	pop	eax
	xor	ecx, ecx
	cmp	[esp], eax
	setle	cl
	mov	[esp], ecx
; JZ _L3
	pop	eax
	cmp	eax, byte 0
	je	near _L3
        ;; WHILE block
; ADDR y
	push	dword $y
; LDINT
	pop	eax
	push	dword [eax]
; DUP32
	push	dword [esp]
; ADDR x
	push	dword $x
; STINT
	pop	ecx
	pop	eax
	mov	[ecx], eax
; TRASH 4
	add	esp, 4
        ;; WHILE condition
; ALIGN
align	4
; LABEL _L4
_L4:
; ADDR x
	push	dword $x
; LDINT
	pop	eax
	push	dword [eax]
; INT 50
	push	dword 50
; LE
	pop	eax
	xor	ecx, ecx
	cmp	[esp], eax
	setle	cl
	mov	[esp], ecx
; JZ _L5
	pop	eax
	cmp	eax, byte 0
	je	near _L5
        ;; WHILE block
; ADDR x
	push	dword $x
; LDINT
	pop	eax
	push	dword [eax]
; INT 10
	push	dword 10
; EQ
	pop	eax
	xor	ecx, ecx
	cmp	[esp], eax
	sete	cl
	mov	[esp], ecx
; JZ _L6
	pop	eax
	cmp	eax, byte 0
	je	near _L6
; JMP _L5
	jmp	dword _L5
; LABEL _L6
_L6:
; ADDR x
	push	dword $x
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
; ADDR x
	push	dword $x
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
; ADDR x
	push	dword $x
; LDINT
	pop	eax
	push	dword [eax]
; CALL printi
	call	printi
; TRASH 4
	add	esp, 4
; CALL println
	call	println
; ADDR y
	push	dword $y
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
; ADDR y
	push	dword $y
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
; EXTERN printi
extern	printi
; EXTERN println
extern	println
