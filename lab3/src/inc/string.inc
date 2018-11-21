#ifndef _STRING_INC_
# define _STRING_INC_

;***************************************************************
; Compare 2 null terminated strings from DI and SI. The result is written to AL:
;   0 - not equal
;   1 - equal
DEFINE_STREQU     MACRO
LOCAL skip_proc_strequ, str_loop, strs_are_not_equal, strs_are_equal, end_of_proc

; protect from wrong definition location:
JMP     skip_proc_strequ

strequ PROC NEAR
    push si
    push di

    str_loop:
        mov ax, [di]
        cmp [si], ax
        jne strs_are_not_equal

        cmp [si], 0
        je strs_are_equal

        inc si
        inc di
        jmp str_loop

    strs_are_not_equal:
        mov al, 0
        jmp end_of_proc

    strs_are_equal:
        mov al, 1

    end_of_proc:
    pop di
    pop si

    RET
strequ ENDP

skip_proc_strequ:
DEFINE_CLEAR_SCREEN     ENDM
;***************************************************************

#endif