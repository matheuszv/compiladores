section .data
    fmt db "%d", 10, 0

section .text
    global main
    extern printf

main:
        mov rax, 7
    mov rbx, 6
    imul rax, rbx
    mov rcx, 5
    imul rax, rcx
    mov rdx, 4
    mov rsi, 3
    imul rdx, rsi
    mov rdi, 2
    imul rdx, rdi
    mov r8, 1
    imul rdx, r8
    cqo
    idiv rdx
    mov rax, rax

    mov rdi, fmt
    mov rsi, rax
    xor rax, rax
    call printf

    mov rax, 60
    xor rdi, rdi
    syscall
