; This org is used for emulator
; ORG 100h

; This org is used for VM
ORG 7c00h

jmp _main

_main:
	xor si, si

    ascii_print_loop:
    	mov ax, si
    	mov ah, 0Eh
        int 10h

    	inc si

    	cmp si, 256
    	jne ascii_print_loop

hlt