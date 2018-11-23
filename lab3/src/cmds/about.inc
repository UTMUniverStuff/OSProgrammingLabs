#include "../inc/print.inc"

define_cmd_about MACRO
LOCAL skip_proc_declaration

jmp skip_proc_declaration

cmd_about PROC NEAR
    printn "This OS was made by Terman Emil FAF161"
    ret
cmd_about ENDP

skip_proc_declaration:
define_cmd_about ENDM