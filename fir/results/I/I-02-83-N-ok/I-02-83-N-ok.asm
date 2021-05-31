; TEXT
segment	.text
; ALIGN
align	4
; LABEL f
f:
; ENTER 8
	push	ebp
	mov	ebp, esp
	sub	esp, 8
        ;; before function PBE 
        ;; before body 
; LOCAL 8
	lea	eax, [ebp+8]
	push	eax
; LDDOUBLE
	pop	eax
	push	dword [eax+4]
	push	dword [eax]
; DUP64
; SP
	push	esp
; LDDOUBLE
	pop	eax
	push	dword [eax+4]
	push	dword [eax]
; LOCAL -8
	lea	eax, [ebp+-8]
	push	eax
; DSTORE
	pop	ecx
	pop	eax
	mov	[ecx], eax
	pop	eax
	mov	[ecx+4], eax
; TRASH 8
	add	esp, 8
        ;; after body 
; LABEL _L1
_L1:
        ;; after function PBE 
; LOCAL -8
	lea	eax, [ebp+-8]
	push	eax
; LDDOUBLE
	pop	eax
	push	dword [eax+4]
	push	dword [eax]
; STFVAL64
	fld	qword [esp]
	add	esp, byte 8
; LEAVE
	leave
; RET
	ret
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
; INT 10
	push	dword 10
; I2D
	fild	dword [esp]
	sub	esp, byte 4
	fstp	qword [esp]
; CALL f
	call	f
; TRASH 8
	add	esp, 8
; LDFVAL64
	sub	esp, byte 8
	fstp	qword [esp]
; CALL printd
	call	printd
; TRASH 8
	add	esp, 8
; INT 2
	push	dword 2
; I2D
	fild	dword [esp]
	sub	esp, byte 4
	fstp	qword [esp]
; CALL f
	call	f
; TRASH 8
	add	esp, 8
; LDFVAL64
	sub	esp, byte 8
	fstp	qword [esp]
; CALL printd
	call	printd
; TRASH 8
	add	esp, 8
; CALL println
	call	println
        ;; after body 
; LABEL _L2
_L2:
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
; EXTERN println
extern	println
