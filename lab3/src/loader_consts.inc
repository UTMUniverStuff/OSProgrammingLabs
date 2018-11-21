#ifndef _LOADER_CONSTS_INC_
# define _LOADER_CONSTS_INC_

kernel_cylinder equ 0
calculated_kernel_sector equ 2
kernel_head equ 0

kernel_memory_address equ 0x0000

kernel_size_in_sectors equ 20

first_instruction_of_the_kernel equ nop
first_instruction_of_the_kernel_value equ 0x90

#endif