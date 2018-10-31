; This org is used for emulator
; ORG 100h

; This org is used for VM
ORG 7c00h

jmp begin

set_curs_pos MACRO pos_x, pos_y
    mov dh, pos_y
    mov dl, pos_x
    mov bh, 0
    mov ah, 02h 
    int 10h    
ENDM

pretty_print MACRO char1, char2
	mov ah, 0Eh

	mov al, char1
    int 10h

    mov al, char2
    int 10h
    
    mov al, ':'
    int 10h
    
    mov al, ' '
    int 10h
ENDM

print_with_10h_1300h MACRO start_x, start_y, attribute
    ; Page number = 0.
    mov bh, 0

    ; Attributes.
    mov bl, attribute

    ; Message length.
    mov cx, text_10h_1300h_end - 1 - offset text_10h_1300h

    ; Pos x and y
    mov dl, start_x
    mov dh, start_y

    mov bp, offset text_10h_1300h

    mov al, 00h
    mov ah, 13h
    int 10h
ENDM

; Print hex value from dx
print_hex_value PROC
	mov cx, 4
	hex_loop:
		; Save current dx.
		mov bx, dx

		; Mask last 4 bits.
		and bx, 0xf000
		shr bx, 3 * 4

		; Add ascii value depeing if it's a number or letter.
			cmp bx, 09h
			jg its_a_letter

			; its_a_number:
				add bx, 0x30
				jmp letter_or_nb_endif
			its_a_letter:
				add bx, 0x37

			letter_or_nb_endif:

		mov ax, bx
		mov ah, 0Eh
	    int 10h
        shl dx, 4

        loop hex_loop

	mov ax, 'h'
	mov ah, 0Eh
    int 10h
    ret
ENDP

print_next_reg MACRO reg_name_char1, reg_name_char2
	mov ah, 0Eh

	mov al, reg_name_char1
    int 10h

    mov al, reg_name_char2
    int 10h
    
    mov al, ':'
    int 10h
    
    mov al, ' '
    int 10h    

    pop dx
	call print_hex_value
ENDM

print_regs PROC
	; Save register values on stack, to restore after procedure ends.
	pusha

	; Save registers for printing.
	pusha

	set_curs_pos 0, 7
	print_next_reg 'D', 'I'

	set_curs_pos 0, 6
	print_next_reg 'S', 'I'

	set_curs_pos 0, 5
	print_next_reg 'B', 'P'

	set_curs_pos 0, 4
	print_next_reg 'S', 'P'

	set_curs_pos 0, 3
	print_next_reg 'B', 'X'

	set_curs_pos 0, 2
	print_next_reg 'D', 'X'

	set_curs_pos 0, 1
	print_next_reg 'C', 'X'	

	set_curs_pos 0, 0
	print_next_reg 'A', 'X'

	popa
	ret
ENDP

begin:
	mov ax, 0x0420
	mov bx, 0xA42A

	call print_regs
hlt