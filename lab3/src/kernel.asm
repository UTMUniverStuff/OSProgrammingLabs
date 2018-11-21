include "loader_consts.inc"
include "consts.inc"
include "inc/term_utils.inc"
include "inc/print.inc"
include "inc/scan.inc"
include "inc/string.inc"

ORG kernel_memory_address

; It is used by the loader to determine if we had a sucessful launch or not.
; This is known as the Integrity Check.
first_instruction_of_the_kernel

; mov ds, cs
push cs
pop ds

; Directive to create bin file:
#make_bin#

; Skip the data and function declaration section
jmp start
    ; Proc declarations
        DEFINE_GET_STRING
        DEFINE_PRINT_STRING
        DEFINE_CLEAR_SCREEN
        DEFINE_STREQU



start:
    call clear_screen

    ; Set video mode 40x25
    mov ah, 0
    mov al, 00h
    int 10h

    read_str_loop:
        print "$> "

        mov bl, term_width
        mov dx, cmd_buffer_size
        lea di, cmd_buffer
        call get_string
        put_newline

        no_such_cmd:
            put_newline
            print "No such command: "
            lea si, cmd_buffer
            call print_string

        end_check_cmp:


        ; lea si, cmd_buffer
        ; call print_string
        put_newline

        jmp read_str_loop

    read_key_loop:
        getch
        jmp read_key_loop

; Data
    cmd_buffer_size equ 512
    cmd_buffer db 512 dup (0)

    ; Cmds
    cmd_help db "help", 0
