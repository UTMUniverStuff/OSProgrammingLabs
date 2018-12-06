#include "../inc/print.inc"

define_cmd_beep MACRO
LOCAL skip_proc_declaration_cmd_beep

jmp skip_proc_declaration_cmd_beep

cmd_beep PROC NEAR
	beep
    ret
cmd_beep ENDP

skip_proc_declaration_cmd_beep:
define_cmd_beep ENDM