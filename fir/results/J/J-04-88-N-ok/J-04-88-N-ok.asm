; TEXT
segment	.text
; ALIGN
align	4
; LABEL f
f:
; ENTER 4
	push	ebp
	mov	ebp, esp
	sub	esp, 4
        ;; before function PBE 
        ;; before prologue 
; LOCAL 8
	lea	eax, [ebp+8]
	push	eax
; LDINT
	pop	eax
	push	dword [eax]
; DUP32
	push	dword [esp]
; LOCAL -4
	lea	eax, [ebp+-4]
	push	eax
; STINT
	pop	ecx
	pop	eax
	mov	[ecx], eax
; TRASH 4
	add	esp, 4
        ;; after prologue 
        ;; before body 
; INT 5
	push	dword 5
; DUP32
	push	dword [esp]
; LOCAL -4
	lea	eax, [ebp+-4]
	push	eax
; STINT
	pop	ecx
	pop	eax
	mov	[ecx], eax
; TRASH 4
	add	esp, 4
; JMP _L1
	jmp	dword _L1
        ;; after body 
; LABEL _L1
_L1:
        ;; before epilogue 
; LOCAL -4
	lea	eax, [ebp+-4]
	push	eax
; LDINT
	pop	eax
	push	dword [eax]
; CALL printi
	call	printi
; TRASH 4
	add	esp, 4
        ;; after epilogue 
        ;; after function PBE 
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
; INT 3
	push	dword 3
; CALL f
	call	f
; TRASH 4
	add	esp, 4
; TRASH 0
	add	esp, 0
; RODATA
segment	.rodata
; ALIGN
align	4
; LABEL _L3
_L3:
; SSTRING (value omitted -- see below)
	db	"a", 0
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
; EXTERN printi
extern	printi
; EXTERN prints
extern	prints
