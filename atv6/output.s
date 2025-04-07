section .data
    fmt db "%d", 10, 0

section .text
    global main
    extern printf

main:
    mov $32, %rax
    push %rax

    mov rdi, fmt
    mov rsi, rax
    xor eax, eax
    call printf

    mov eax, 0
    ret
