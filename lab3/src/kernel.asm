include "inc/term_utils.inc"
include "inc/print.inc"
include "inc/scan.inc"

kernel_memory_address equ 0x0000

ORG kernel_memory_address

; The first byte of this instruction is 0x90.
; It is used by the loader to determine if we had a sucessful launch or not.
; This is known as the Integrity Check.
; The loader prints out an error message if kernel was not found.
nop

; mov ds, cs
push cs
pop ds

; Directive to create bin file:
#make_bin#

; Skip the data and function declaration section
jmp start
    
start:
    read_key_loop:
        getch


; Data
endl equ 0ah, 0dh
welcome_msg  db "BootLoader developed by student Terman Emil FAF161", endl, 0 

buffer db 512 dup (0)