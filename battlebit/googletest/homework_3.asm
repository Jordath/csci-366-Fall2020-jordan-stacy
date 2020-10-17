        global    find_max
        section   .text
find_max:
        ;; rdi has the array in it
        ;; rsi has the length in it
        ;;mov rax, [rdi] ;; move the first value in the array into rax
        ;; you will need to loop through the array and
        ;; compare each value with rax to determine if it is greater
        ;; after the comparison, decrement the count, bump the
        ;; array pointer by 8 (long long = 64 bits = 8 bytes)
        ;; and if the counter is greater than zero, loop
        push    rbp
        mov     rbp, rsp
        mov     QWORD PTR [rbp-24], rdi
        mov     DWORD PTR [rbp-28], esi
        mov     rax, QWORD PTR [rbp-24]
        mov     rax, QWORD PTR [rax]
        mov     QWORD PTR [rbp-8], rax
.L4:
        cmp     DWORD PTR [rbp-28], 0
        jle     .L2
        add     QWORD PTR [rbp-24], 8
        sub     DWORD PTR [rbp-28], 1
        mov     rax, QWORD PTR [rbp-24]
        mov     rax, QWORD PTR [rax]
        mov     QWORD PTR [rbp-16], rax
        mov     rax, QWORD PTR [rbp-8]
        cmp     rax, QWORD PTR [rbp-16]
        jge     .L4
        mov     rax, QWORD PTR [rbp-16]
        mov     QWORD PTR [rbp-8], rax
        jmp     .L4
.L2:
        mov     rax, QWORD PTR [rbp-8]
        pop     rbp
        ret