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
; INT 0
	push	dword 0
; JZ _L2
	pop	eax
	cmp	eax, byte 0
	je	near _L2
; RODATA
segment	.rodata
; ALIGN
align	4
; LABEL _L3
_L3:
; SSTRING (value omitted -- see below)
	db	"KO", 0
; TEXT
segment	.text
; ALIGN
align	4
; ADDR _L3
	push	dword $_L3
; CALL prints
	call	prints
; TRASH 4
	add	esp, 4
; CALL println
	call	println
; JMP _L4
	jmp	dword _L4
; LABEL _L2
_L2:
; INT 0
	push	dword 0
; JZ _L5
	pop	eax
	cmp	eax, byte 0
	je	near _L5
; RODATA
segment	.rodata
; ALIGN
align	4
; LABEL _L6
_L6:
; SSTRING (value omitted -- see below)
	db	"elif1", 0
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
; CALL println
	call	println
; JMP _L7
	jmp	dword _L7
; LABEL _L5
_L5:
; INT 0
	push	dword 0
; JZ _L8
	pop	eax
	cmp	eax, byte 0
	je	near _L8
; RODATA
segment	.rodata
; ALIGN
align	4
; LABEL _L9
_L9:
; SSTRING (value omitted -- see below)
	db	"elif2", 0
; TEXT
segment	.text
; ALIGN
align	4
; ADDR _L9
	push	dword $_L9
; CALL prints
	call	prints
; TRASH 4
	add	esp, 4
; CALL println
	call	println
; JMP _L10
	jmp	dword _L10
; LABEL _L8
_L8:
; INT 0
	push	dword 0
; JZ _L11
	pop	eax
	cmp	eax, byte 0
	je	near _L11
; RODATA
segment	.rodata
; ALIGN
align	4
; LABEL _L12
_L12:
; SSTRING (value omitted -- see below)
	db	"elif3", 0
; TEXT
segment	.text
; ALIGN
align	4
; ADDR _L12
	push	dword $_L12
; CALL prints
	call	prints
; TRASH 4
	add	esp, 4
; CALL println
	call	println
; JMP _L13
	jmp	dword _L13
; LABEL _L11
_L11:
; INT 0
	push	dword 0
; JZ _L14
	pop	eax
	cmp	eax, byte 0
	je	near _L14
; RODATA
segment	.rodata
; ALIGN
align	4
; LABEL _L15
_L15:
; SSTRING (value omitted -- see below)
	db	"elif4", 0
; TEXT
segment	.text
; ALIGN
align	4
; ADDR _L15
	push	dword $_L15
; CALL prints
	call	prints
; TRASH 4
	add	esp, 4
; CALL println
	call	println
; LABEL _L14
_L14:
; LABEL _L13
_L13:
; LABEL _L10
_L10:
; LABEL _L7
_L7:
; LABEL _L4
_L4:
; RODATA
segment	.rodata
; ALIGN
align	4
; LABEL _L16
_L16:
; SSTRING (value omitted -- see below)
	db	"OK", 0
; TEXT
segment	.text
; ALIGN
align	4
; ADDR _L16
	push	dword $_L16
; CALL prints
	call	prints
; TRASH 4
	add	esp, 4
; CALL println
	call	println
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
; EXTERN println
extern	println
; EXTERN prints
extern	prints
