name "loader"

; this is a very basic example of a tiny operating system.

; directive to create boot file:
#make_boot#

; this is an os loader only!
;
; it can be loaded at the first sector of a floppy disk:

;   cylinder: 0
;   sector: 1
;   head: 0

;
; The code in this file is supposed to load
; the kernel and to pass control over it.

; boot record is loaded at 0000:7c00
org 7c00h

; jmp start

clear_screen macro
    mov al, 00h
    mov bh, 0000_1111b
    mov ah, 06h
    mov cl, 00h
    mov ch, 00h
    mov dl, 80
    mov dh, 25
    int 10h
endm

; start:

; initialize the stack:
mov     ax, 07c0h
mov     ss, ax
mov     sp, 03feh ; top of the stack.


; set data segment:
xor     ax, ax
mov     ds, ax

; set default video mode 80x25:
mov     ah, 00h
mov     al, 03h
int     10h

; print welcome message:
lea     si, welcome_msg
call    print_string

lea     si, msg_press_1
call    print_string

; read loop. Ends when the user presses '1'.
read_1_loop:
    ; read character.
    mov ah, 00h
    int 16h

    ; if '1' was pressed, break.
    cmp al, '1'
    je read_1_loop_end

    ; else

    lea si, try_again_msg
    call print_string
    jmp read_1_loop

read_1_loop_end:

;===================================
; load the kernel at 0800h:kernel_memory_address
; 10 sectors starting at:
;   cylinder: 0
;   sector: 2
;   head: 0

; BIOS passes drive number in dl,
; so it's not changed:

mov     ah, 02h ; read function.
mov     al, 10  ; sectors to read.
mov     ch, kernel_cylinder
mov     cl, calculated_kernel_sector
mov     dh, kernel_head
; dl not changed! - drive number.

; es:bx points to receiving
;  data buffer:
mov     bx, 0800h   
mov     es, bx
mov     bx, kernel_memory_address

; read!
int     13h
;===================================

; integrity check:
; check if the first byte of kernel must is 0x90 == NOP.
cmp     es:[kernel_memory_address], 0x90
je      integrity_check_ok

; integrity check error
lea     si, err
call    print_string
hlt

integrity_check_ok:
lea si, success_load_kernel
call print_string

lea si, msg_press_2
call print_string

; read loop. Ends when the user presses '2'.
read_2_loop:
    ; read character.
    mov ah, 00h
    int 16h

    ; if '2' was pressed, break.
    cmp al, '2'
    je read_2_loop_end

    ; else

    lea si, try_again_msg
    call print_string
    jmp read_2_loop

read_2_loop_end:

clear_screen

; pass control to kernel:
jmp     0800h:kernel_memory_address

;===========================================

print_string proc near
    push    ax      ; store registers...
    push    si      ;
    next_char:      
            mov     al, [si]
            cmp     al, 0
            jz      printed
            inc     si
            mov     ah, 0eh ; teletype function.
            int     10h
            jmp     next_char
    printed:
    pop     si      ; re-store registers...
    pop     ax      ;
    ret
print_string endp

;==== data section =====================
endl equ 0ah, 0dh

; Constants
    my_number_fromstudents_registry equ 22

    ; = 66
    kernel_sector equ (my_number_fromstudents_registry * 3)

    ; = ((kernel_sector - 1) / 36)
    kernel_cylinder equ 1

    ; if kernel_sector mod 18 == 0
    ;   return 18;
    ; else
    ;   return (kernel_sector mod 18);
    ; (22 * 3) mod 18 = 12
    calculated_kernel_sector equ 12

    ; From sector 1 -> 18   => head = 0
    ; From sector 19 -> 36  => head = 1
    ; From sector 37 -> 54  => head = 0
    ; From sector 55 -> 72  => head = 1
    kernel_head equ 1

    ; 0xbe00 = 7c00h + 3 * 256 * my_number_fromstudents_registry
    kernel_memory_address equ 0xbe00


welcome_msg  db "BootLoader developed by student Terman Emil FAF161", endl, 0 
msg_press_1  db "Press 1 to start loading the kernel to RAM", endl, 0 
msg_press_2  db "Press 2 to start loading the kernel to RAM", endl, 0 
try_again_msg db "Try again", endl, 0     

success_load_kernel db "Kernel was loaded into RAM to address 0xbe00!", endl, 0

err  db "Invalid data at the given sector-cylinder-head", endl, 0
;======================================

