#ifndef _TERM_UTILS_INC_
# define _TERM_UTILS_INC_

; turns off the cursor:
CURSOROFF       MACRO
    PUSH    AX
    PUSH    CX

    MOV     AH, 1
    MOV     CH, 28h
    MOV     CL, 09h
    INT     10h
    
    POP     CX
    POP     AX
ENDM

get_cursor_pos MACRO
    mov ah, 03h
    mov bh, 0
    int 10h
ENDM

; turns on the cursor:
CURSORON    MACRO
    PUSH    AX
    PUSH    CX
    MOV     AH, 1
    MOV     CH, 08h
    MOV     CL, 09h
    INT     10h
    POP     CX
    POP     AX
ENDM

; sets current cursor
; position:
GOTOXY  MACRO   col, row
    PUSH    AX
    PUSH    BX
    PUSH    DX
    MOV     AH, 02h
    MOV     DH, row
    MOV     DL, col
    MOV     BH, 0
    INT     10h
    POP     DX
    POP     BX
    POP     AX
ENDM


;***************************************************************
; this macro defines procedure to clear the screen,
; (done by scrolling entire screen window),
; and set cursor position to top of it:
DEFINE_CLEAR_SCREEN     MACRO
LOCAL skip_proc_clear_screen

; protect from wrong definition location:
JMP     skip_proc_clear_screen

CLEAR_SCREEN PROC NEAR
    PUSH    AX      ; store registers...
    PUSH    DS      ;
    PUSH    BX      ;
    PUSH    CX      ;
    PUSH    DI      ;

    MOV     AX, 40h
    MOV     DS, AX  ; for getting screen parameters.
    MOV     AH, 06h ; scroll up function id.
    MOV     AL, 0   ; scroll all lines!
    MOV     BH, 07  ; attribute for new lines.
    MOV     CH, 0   ; upper row.
    MOV     CL, 0   ; upper col.
    MOV     DI, 84h ; rows on screen -1,
    MOV     DH, [DI] ; lower row (byte).
    MOV     DI, 4Ah ; columns on screen,
    MOV     DL, [DI]
    DEC     DL      ; lower col.
    INT     10h

    ; set cursor position to top
    ; of the screen:
    MOV     BH, 0   ; current page.
    MOV     DL, 0   ; col.
    MOV     DH, 0   ; row.
    MOV     AH, 02
    INT     10h

    POP     DI      ; re-store registers...
    POP     CX      ;
    POP     BX      ;
    POP     DS      ;
    POP     AX      ;

    RET
CLEAR_SCREEN ENDP

skip_proc_clear_screen:

DEFINE_CLEAR_SCREEN     ENDM
;***************************************************************

#endif