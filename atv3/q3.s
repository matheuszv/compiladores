section .data
    fmt db "%ld", 10, 0   ; Formato para imprimir um número com sinal

section .text
    global main
    extern printf

main:
    ; Calcula 512 * 65
    mov rax, 512        
    imul rax, 65        ; rax = 512 * 65

    ; Guarda o resultado temporário em rbx
    mov rbx, rax        

    ; Calcula 5657 * 23
    mov rax, 5657    
    imul rax, 23        ; rax = 5657 * 23

    ; Subtrai os valores (resultado final fica em RAX)
    sub rbx, rax        

    ; Move o resultado final para RAX
    mov rax, rbx        

    ; Imprime o resultado
    mov rdi, fmt         
    mov rsi, rax        
    xor rax, rax        
    call printf          

    ; Finaliza o programa
    mov rax, 60         
    xor rdi, rdi        
    syscall             
