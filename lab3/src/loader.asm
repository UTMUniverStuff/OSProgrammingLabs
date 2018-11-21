include "loader_consts.inc"
include "inc/term_utils.inc"
include "inc/print.inc"
include "inc/scan.inc"

; directive to create boot file:
#make_boot#

; this is an os loader
;
; The code in this file is supposed to load
; the kernel and to pass control over it.

; boot record is loaded at 0000:7c00
org 7c00h

; Proc declarations
    DEFINE_CLEAR_SCREEN

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

printn "BootLoader by Terman Emil FAF161"
printn "Loading the kernel"

;===================================
; load the kernel at 0800h:kernel_memory_address

; BIOS passes drive number in dl,
; so it's not changed:

mov     ah, 02h ; read function.
mov     al, kernel_size_in_sectors
mov     ch, kernel_cylinder
mov     cl, calculated_kernel_sector
mov     dh, kernel_head
; dl not changed! - drive number.

; es:bx points to receiving data buffer:
mov     bx, 0800h   
mov     es, bx
mov     bx, kernel_memory_address

; read!
int     13h
;===================================

; integrity check:
; check if the first byte of kernel is the right one
cmp     es:[kernel_memory_address], first_instruction_of_the_kernel_value
je      integrity_check_ok

; integrity check error
print "Integrity check ERROR: Invalid data at the given sector-cylinder-head"
hlt
read_key_loop:
    getch
    jmp read_key_loop

integrity_check_ok:
printn "Kernel was loaded" endl "Starting the kernel"

call clear_screen

; pass control to kernel:
jmp     0800h:kernel_memory_address
