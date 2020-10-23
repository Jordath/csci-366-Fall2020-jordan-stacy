        global    find_max
        section   .text
find_max:
        ;; rdi has the array in it
        ;; rsi has the length in it
        mov rax, [rdi] ;; move the first value in the array into rax
        ;; you will need to loop through the array and
        ;; compare each value with rax to determine if it is greater
        ;; after the comparison, decrement the count, bump the
        ;; array pointer by 8 (long long = 64 bits = 8 bytes)
        ;; and if the counter is greater than zero, loop

; I designed a find_max function in C and used godbolt.org to help with the conversion
; I have no idea what to do beyond this point.
        push    rbp
        mov     rbp, rsp
        mov     [rbp-24], rdi
        mov     [rbp-28], esi
        mov     rax, [rbp-24]
        mov     rax, [rax]
        mov     [rbp-8], rax
.L4:
        cmp     DWORD [rbp-28], 0
        jle     .L2
        add     QWORD [rbp-24], 8
        sub     DWORD [rbp-28], 1
        mov     rax, [rbp-24]
        mov     rax, [rax]
        mov     [rbp-16], rax
        mov     rax, [rbp-8]
        cmp     rax, [rbp-16]
        jge     .L4
        mov     rax, [rbp-16]
        mov     [rbp-8], rax
        jmp     .L4
.L2:
        mov     rax, [rbp-8]
        pop     rbp
        ret