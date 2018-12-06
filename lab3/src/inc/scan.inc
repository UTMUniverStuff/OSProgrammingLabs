#ifndef _SCAN_INC_
# define _SCAN_INC_

# include "term_utils.inc"
# include "print.inc"

; Read a keystroke
; ah - BIOS scan code
; al - ASCII char
getch MACRO
    mov ah, 00h
    int 16h
ENDM

;***************************************************************

; This macro defines a procedure that gets the multi-digit SIGNED number from the keyboard,
; and stores the result in CX register:
DEFINE_SCAN_NUM  MACRO
LOCAL make_minus, ten, next_digit, set_minus
LOCAL too_big, backspace_checked, too_big2
LOCAL stop_input, not_minus, skip_proc_scan_num
LOCAL remove_not_digit, ok_AE_0, ok_digit, not_cr

; protect from wrong definition location:
JMP     skip_proc_scan_num

SCAN_NUM    PROC    NEAR
    PUSH    DX
    PUSH    AX
    PUSH    SI
    
    MOV     CX, 0

    ; reset flag:
    MOV     CS:make_minus, 0

next_digit:

    ; get char from keyboard
    ; into AL:
    MOV     AH, 00h
    INT     16h
    ; and print it:
    MOV     AH, 0Eh
    INT     10h

    ; check for MINUS:
    CMP     AL, '-'
    JE      set_minus

    ; check for ENTER key:
    CMP     AL, 13  ; carriage return?
    JNE     not_cr
    JMP     stop_input
not_cr:


    CMP     AL, 8      ; 'BACKSPACE' pressed?
    JNE     backspace_checked
    MOV     DX, 0      ; remove last digit by
    MOV     AX, CX    ; division:
    DIV     CS:ten    ; AX = DX:AX / 10 (DX-rem).
    MOV     CX, AX
    PUTC    ' '      ; clear position.
    PUTC    8          ; backspace again.
    JMP     next_digit
backspace_checked:


    ; allow only digits:
    CMP     AL, '0'
    JAE     ok_AE_0
    JMP     remove_not_digit
ok_AE_0:    
    CMP     AL, '9'
    JBE     ok_digit
remove_not_digit:       
    PUTC    8       ; backspace.
    PUTC    ' '     ; clear last entered not digit.
    PUTC    8       ; backspace again.  
    JMP     next_digit ; wait for next input.       
ok_digit:


    ; multiply CX by 10 (first time the result is zero)
    PUSH    AX
    MOV     AX, CX
    MUL     CS:ten    ; DX:AX = AX*10
    MOV     CX, AX
    POP     AX

    ; check if the number is too big
    ; (result should be 16 bits)
    CMP     DX, 0
    JNE     too_big

    ; convert from ASCII code:
    SUB     AL, 30h

    ; add AL to CX:
    MOV     AH, 0
    MOV     DX, CX      ; backup, in case the result will be too big.
    ADD     CX, AX
    JC      too_big2    ; jump if the number is too big.

    JMP     next_digit

set_minus:
    MOV     CS:make_minus, 1
    JMP     next_digit

too_big2:
    MOV     CX, DX      ; restore the backuped value before add.
    MOV     DX, 0       ; DX was zero before backup!
too_big:
    MOV     AX, CX
    DIV     CS:ten  ; reverse last DX:AX = AX*10, make AX = DX:AX / 10
    MOV     CX, AX
    PUTC    8       ; backspace.
    PUTC    ' '     ; clear last entered digit.
    PUTC    8       ; backspace again.  
    JMP     next_digit ; wait for Enter/Backspace.
    
    
stop_input:
    ; check flag:
    CMP     CS:make_minus, 0
    JE      not_minus
    NEG     CX
not_minus:

    POP     SI
    POP     AX
    POP     DX
    RET
make_minus      DB      ?       ; used as a flag.
ten      DW      10      ; used as multiplier.
SCAN_NUM    ENDP

skip_proc_scan_num:

DEFINE_SCAN_NUM  ENDM
;***************************************************************


;***************************************************************
; This macro defines a procedure to get a null terminated
; string from user, the received string is written to buffer
; at DS:DI, buffer size should be in DX.
; Procedure stops the input when 'Enter' is pressed.
; Screen width must be given in BL
DEFINE_GET_STRING       MACRO
LOCAL   empty_buffer, wait_for_key, skip_proc_get_string
LOCAL   exit, add_to_buffer, screen_width
LOCAL   its_not_on_the_edge_of_the_screen, edge_of_the_screen_endif
LOCAL   _process_pos_at_x_0_exit, do_the_beep

; protect from wrong definition location:
JMP     skip_proc_get_string
    screen_width db 0

_process_pos_at_x_0 PROC NEAR
    pusha
        get_cursor_pos
        cmp dl, 0

        jne _process_pos_at_x_0_exit

        ; Goto row - 1, screen_width
        dec     DH ; decrease the row
        MOV     DL, screen_width
        MOV     AH, 02h
        INT     10h

    _process_pos_at_x_0_exit:
    popa
    ret
_process_pos_at_x_0 ENDP

GET_STRING      PROC    NEAR
pusha

; Get the screen_width from parameters
mov     screen_width, bl
MOV     CX, 0      ; char counter.

CMP     DX, 1      ; buffer too small?
JBE     empty_buffer        ;

DEC     DX        ; reserve space for last zero.


;============================
; loop to get and processes key presses:

wait_for_key:

MOV     AH, 0      ; get pressed key.
INT     16h

CMP     AL, 13    ; 'RETURN' pressed?
JZ      exit


CMP     AL, 8               ; 'BACKSPACE' pressed?
JNE     add_to_buffer
JCXZ    do_the_beep         ; nothing to remove!
DEC     CX
DEC     DI

; If it's at the edge of the screen (right)
    pusha
    
    ; Get cursor pos
    get_cursor_pos

    ; if x == screen_width
    cmp dl, screen_width

    popa
    jne its_not_on_the_edge_of_the_screen

    pusha
    get_cursor_pos

    ; Decrease x
    dec dl

    ; Set cursor pos to screen_width - 1
    mov ah, 02h
    int 10h

    ; clear position.
    PUTC    ' '
    PUTC    8

    ; Set cursor pos to screen_width - 1
    mov ah, 02h
    int 10h


    popa

    jmp edge_of_the_screen_endif

    its_not_on_the_edge_of_the_screen:
    PUTC 8         ; backspace.
    PUTC ' '         ; clear position.
    PUTC 8         ; backspace again.
    
    edge_of_the_screen_endif:

    call _process_pos_at_x_0

JMP     wait_for_key

add_to_buffer:

    CMP     CX, DX    ; buffer is full?

    JAE     do_the_beep    ; if so wait for 'BACKSPACE' or 'RETURN'...

    MOV     [DI], AL
    INC     DI
    INC     CX
    
    ; print the key:
    MOV     AH, 0Eh
    INT     10h

    call _process_pos_at_x_0
JMP     wait_for_key

do_the_beep:
    beep
    jmp wait_for_key

;============================

exit:

; terminate by null:
MOV     [DI], 0

empty_buffer:

popa
RET
GET_STRING      ENDP


skip_proc_get_string:

DEFINE_GET_STRING       ENDM
;***************************************************************

#endif