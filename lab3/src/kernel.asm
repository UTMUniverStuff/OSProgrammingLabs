include "loader_consts.inc"
include "consts.inc"
include "extract_words.inc"
include "inc/term_utils.inc"
include "inc/print.inc"
include "inc/scan.inc"
include "inc/string.inc"

include "cmds/help.inc"
include "cmds/about.inc"
include "cmds/ascii.inc"
include "cmds/clear.inc"
include "cmds/beep.inc"
include "cmds/echo.inc"

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

    ; Cmds
    DEFINE_EXTRACT_WORDS
    DEFINE_CMD_HELP
    DEFINE_CMD_ABOUT
    DEFINE_CMD_ASCII
    DEFINE_CMD_CLEAR
    DEFINE_CMD_BEEP
    DEFINE_CMD_ECHO

execute_argv MACRO stat
LOCAL cmd_executed, no_such_cmd, execute_argv_end
LOCAL not_help_cmd
    push di
    push si

    mov si, argv[0]

    ; Cmd - help
    lea di, cmd_help_str
    call strequ

    cmp al, 0
    je not_help_cmd
        call cmd_help
        jmp cmd_executed
    
    not_help_cmd:

    ; Cmd - about
    lea di, cmd_about_str
    call strequ

    cmp al, 0
    je not_about_cmd
        call cmd_about
        jmp cmd_executed

    not_about_cmd:

    ; Cmd - ascii
    lea di, cmd_ascii_str
    call strequ

    cmp al, 0
    je not_ascii_cmd
        call cmd_ascii
        jmp cmd_executed    

    not_ascii_cmd:

    ; Cmd - clear
    lea di, cmd_clear_str
    call strequ

    cmp al, 0
    je not_clear_cmd
        call cmd_clear
        jmp cmd_executed    

    not_clear_cmd:

    ; Cmd - beep
    lea di, cmd_beep_str
    call strequ

    cmp al, 0
    je not_beep_cmd
        call cmd_beep
        jmp cmd_executed    

    not_beep_cmd:

    ; Cmd - reboot
    lea di, cmd_reboot_str
    call strequ

    cmp al, 0
    je not_reboot_cmd
        int 19h
        jmp cmd_executed

    not_reboot_cmd:

    ; Cmd - echo
    lea di, cmd_echo_str
    call strequ

    cmp al, 0
    je not_echo_cmd
        lea di, argv
        call cmd_echo
        jmp cmd_executed

    not_echo_cmd:

    ; End of cmds
    no_such_cmd:
        mov al, 0
        jmp execute_argv_end
    cmd_executed:
        mov al, 1
        jmp execute_argv_end

    execute_argv_end:
    pop si
    pop di
ENDM

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

        lea di, cmd_buffer
        lea si, argv
        call extract_words

        ; If nothing in argv - nothing happens.
        cmp w. argv[0], 0
        je read_str_loop

        execute_argv

        ; If a command was executed - all good
        cmp al, 1
        je end_of_cmd_processing

        ; else, print. "No such cmd: <argv[0]>"
        print "No such command: "
        mov si, argv[0]
        call print_string
        put_newline

        end_of_cmd_processing:
        put_newline
        jmp read_str_loop

    read_key_loop:
        getch
        jmp read_key_loop

; Data
    cmd_buffer_size equ 512
    cmd_buffer db 512 dup(0)

    argv dw 128 dup(0)

    ; Cmds
    cmd_help_str    db "help", 0
    cmd_about_str   db "about", 0
    cmd_ascii_str   db "ascii", 0
    cmd_clear_str   db "clear", 0
    cmd_beep_str    db "beep", 0
    cmd_reboot_str  db "reboot", 0
    cmd_echo_str    db "echo", 0
