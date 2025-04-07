section .data
    fmt db "%d", 10, 0

section .text
    global main
    extern printf

main:
        mov rax, 72
    mov rbx, 101
    sub rax, rbx
    mov rcx, 4
    imul rax, rcx
    mov rdx, 14
    mov rsi, 77
    imul rdx, rsi
    add rax, rdx
    mov rax, rax

    mov rdi, fmt
    mov rsi, rax
    xor rax, rax
    call printf

    mov rax, 60
    xor rdi, rdi
    syscall
