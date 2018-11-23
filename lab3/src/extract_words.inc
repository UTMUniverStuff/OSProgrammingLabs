; Extract the words from [DI] to [SI]
define_extract_words MACRO
    LOCAL last_char_was_sep
    LOCAL cmd_loop, its_separator, its_not_separator, end_of_sep_control
    LOCAL cmd_loop_end

    last_char_was_sep db 0

extract_words PROC
    pusha

    mov last_char_was_sep, 1
    mov w. [si], 0
    cmd_loop:
        cmp b. [di], 0
        je cmd_loop_end

        ; Check if it's a separator
        ; - Check for space
        cmp b. [di], ' '
        je its_separator

        ; - Check for tab
        cmp b. [di], 9
        jne its_not_separator

        its_separator:
            ; Put 0 instead of separators
            mov b. [di], 0 
            
            mov last_char_was_sep, 1
            jmp end_of_sep_control

        its_not_separator:
            cmp last_char_was_sep, 1
            jne end_of_sep_control

            mov last_char_was_sep, 0
            
            mov ax, di
            mov [si], ax
            
            add si, 2
            mov w. [si], 0

        end_of_sep_control:

        inc di
        jmp cmd_loop

    cmd_loop_end:

    popa
    ret
extract_words ENDP
define_extract_words ENDM
