#include "../inc/print.inc"

; Prints the strings from w. [DI] until it meets a 0.
; argv[0] is ignored, since it should be "echo"
define_cmd_echo MACRO
LOCAL skip_proc_declaration

jmp skip_proc_declaration

cmd_echo PROC NEAR
	pusha
	; Ignore argv[0]
	add di, 2

	argv_loop:
		cmp w. [di], 0
		je argv_loop_end

		mov si, w. [di]
		call print_string
		putc ' '

		add di, 2
		jmp argv_loop

	argv_loop_end:

	put_newline
	popa
	ret
cmd_echo ENDP

skip_proc_declaration:
define_cmd_echo ENDM