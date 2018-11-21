#ifndef _PRINT_INC_
# define _PRINT_INC_

; this macro prints a char in AL and advances
; the current cursor position:
PUTC    MACRO   char
    PUSH    AX
    MOV     AL, char
    MOV     AH, 0Eh
    INT     10h     
    POP     AX
ENDM

PUT_NEWLINE	MACRO
	PUTC 0ah
	PUTC 0dh
ENDM


; this macro prints a string that is given as a parameter, example:
; PRINT 'hello world!'
; new line is NOT added.
PRINT   MACRO   sdat
LOCAL   next_char, s_dcl, printed, skip_dcl

PUSH    AX      ; store registers...
PUSH    SI      ;

JMP     skip_dcl    ; skip declaration.
    s_dcl DB sdat, 0

skip_dcl:
    LEA     SI, s_dcl
    
next_char:      
    MOV     AL, CS:[SI]
    CMP     AL, 0
    JZ      printed
    INC     SI
    MOV     AH, 0Eh ; teletype function.
    INT     10h
    JMP     next_char
printed:

POP     SI      ; re-store registers...
POP     AX      ;
ENDM


; this macro prints a string that is given as a parameter, example:
; PRINTN 'hello world!'
; the same as PRINT, but new line is automatically added.
PRINTN   MACRO   sdat
LOCAL   next_char, s_dcl, printed, skip_dcl

PUSH    AX      ; store registers...
PUSH    SI      ;

JMP     skip_dcl    ; skip declaration.
    s_dcl DB sdat, 13, 10, 0

skip_dcl:
    LEA     SI, s_dcl
    
next_char:      
    MOV     AL, CS:[SI]
    CMP     AL, 0
    JZ      printed
    INC     SI
    MOV     AH, 0Eh ; teletype function.
    INT     10h
    JMP     next_char
printed:

POP     SI      ; re-store registers...
POP     AX      ;
ENDM

;***************************************************************
; this macro defines a procedure to print a null terminated
; string at current cursor position, receives address of string in DS:SI
DEFINE_PRINT_STRING     MACRO
LOCAL   next_char, printed, skip_proc_print_string

; protect from wrong definition location:
JMP     skip_proc_print_string

PRINT_STRING PROC NEAR
PUSH    AX      ; store registers...
PUSH    SI      ;

next_char:      
    MOV     AL, [SI]
    CMP     AL, 0
    JZ      printed
    INC     SI
    MOV     AH, 0Eh ; teletype function.
    INT     10h
    JMP     next_char
printed:

POP     SI      ; re-store registers...
POP     AX      ;

RET
PRINT_STRING ENDP

skip_proc_print_string:

DEFINE_PRINT_STRING     ENDM
;***************************************************************

; This macro defines a procedure that prints number in AX,
; used with PRINT_NUM_UNS to print signed numbers:
; Requires DEFINE_PRINT_NUM_UNS !!!
DEFINE_PRINT_NUM    MACRO
LOCAL not_zero, positive, printed, skip_proc_print_num

; protect from wrong definition location:
JMP     skip_proc_print_num

PRINT_NUM       PROC    NEAR
    PUSH    DX
    PUSH    AX

    CMP     AX, 0
    JNZ     not_zero

    PUTC    '0'
    JMP     printed

not_zero:
    ; the check SIGN of AX,
    ; make absolute if it's negative:
    CMP     AX, 0
    JNS     positive
    NEG     AX

    PUTC    '-'

positive:
    CALL    PRINT_NUM_UNS
printed:
    POP     AX
    POP     DX
    RET
PRINT_NUM       ENDP

skip_proc_print_num:

DEFINE_PRINT_NUM    ENDM

;***************************************************************

; This macro defines a procedure that prints out an unsigned
; number in AX (not just a single digit)
; allowed values from 0 to 65535 (0FFFFh)
DEFINE_PRINT_NUM_UNS    MACRO
LOCAL begin_print, calc, skip, print_zero, end_print, ten
LOCAL skip_proc_print_num_uns

; protect from wrong definition location:
JMP     skip_proc_print_num_uns

PRINT_NUM_UNS   PROC    NEAR
    PUSH    AX
    PUSH    BX
    PUSH    CX
    PUSH    DX

    ; flag to prevent printing zeros before number:
    MOV     CX, 1

    ; (result of "/ 10000" is always less or equal to 9).
    MOV     BX, 10000       ; 2710h - divider.

    ; AX is zero?
    CMP     AX, 0
    JZ      print_zero

begin_print:

    ; check divider (if zero go to end_print):
    CMP     BX,0
    JZ      end_print

    ; avoid printing zeros before number:
    CMP     CX, 0
    JE      calc
    ; if AX<BX then result of DIV will be zero:
    CMP     AX, BX
    JB      skip
calc:
    MOV     CX, 0   ; set flag.

    MOV     DX, 0
    DIV     BX      ; AX = DX:AX / BX   (DX=remainder).

    ; print last digit
    ; AH is always ZERO, so it's ignored
    ADD     AL, 30h    ; convert to ASCII code.
    PUTC    AL


    MOV     AX, DX  ; get remainder from last div.

skip:
    ; calculate BX=BX/10
    PUSH    AX
    MOV     DX, 0
    MOV     AX, BX
    DIV     CS:ten  ; AX = DX:AX / 10   (DX=remainder).
    MOV     BX, AX
    POP     AX

    JMP     begin_print
    
print_zero:
    PUTC    '0'
    
end_print:

    POP     DX
    POP     CX
    POP     BX
    POP     AX
    RET
ten   DW      10      ; used as divider.      
PRINT_NUM_UNS   ENDP

skip_proc_print_num_uns:

DEFINE_PRINT_NUM_UNS    ENDM
;***************************************************************

#endif