#include "../inc/print.inc"

define_cmd_help MACRO
LOCAL skip_proc_declaration_cmd_help, help_content

jmp skip_proc_declaration_cmd_help
	help_content db	"Available commands: ", endl
				 db	"about", endl
				 db	"ascii - print the ascii table", endl
				 db	"clear - clear the screen", endl
				 db	"beep - should produce a sound", endl
				 db	"reboot - restart the system", endl
				 db	"echo - print the arguments", endl
				 db	0

cmd_help PROC NEAR
	lea si, help_content
	call print_string
    ret
cmd_help ENDP

skip_proc_declaration_cmd_help:
define_cmd_help ENDM