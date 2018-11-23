#include "../inc/print.inc"

define_cmd_ascii MACRO
LOCAL skip_proc_declaration
LOCAL ascii_print_loop

jmp skip_proc_declaration

cmd_ascii PROC NEAR
	xor si, si

    ascii_print_loop:
    	mov ax, si
    	mov ah, 0Eh
        int 10h

    	inc si

    	cmp si, 256
    	jne ascii_print_loop
    ret
cmd_ascii ENDP

skip_proc_declaration:
define_cmd_ascii ENDM