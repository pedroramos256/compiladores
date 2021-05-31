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
; LABEL _L2
_L2:
; RODATA
segment	.rodata
; ALIGN
align	4
; LABEL _L4
_L4:
; SSTRING (value omitted -- see below)
	db	"OK", 0
; TEXT
segment	.text
; ALIGN
align	4
; ADDR _L4
	push	dword $_L4
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
