#include "../inc/term_utils.inc"

; DEFINE_CLEAR_SCREEN

define_cmd_clear MACRO
LOCAL skip_proc_declaration

jmp skip_proc_declaration

cmd_clear PROC NEAR
    call clear_screen
    ret
cmd_clear ENDP

skip_proc_declaration:
define_cmd_clear ENDM