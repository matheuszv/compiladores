section .data
    fmt db "%d", 10, 0

section .text
    global main
    extern printf

main:
        mov rax, 8
    mov rbx, 11
    imul rax, rbx
    mov rcx, 12
    mov rdx, 9
    imul rcx, rdx
    sub rax, rcx
    mov rsi, 112
    mov rdi, 19
    sub rsi, rdi
    add rax, rsi
    mov rax, rax

    mov rdi, fmt
    mov rsi, rax
    xor rax, rax
    call printf

    mov rax, 60
    xor rdi, rdi
    syscall
