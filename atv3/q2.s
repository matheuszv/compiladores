section .data
    fmt db "%lu", 10, 0   ; Formato para imprimir um número sem sinal

section .text
    global main
    extern printf

main:
    ; Calcula 7374 * 657
    mov rax, 7374        
    imul rax, 657        ; rax = 7374 * 657

    ; Guarda o resultado temporário em rbx
    mov rbx, rax        

    ; Calcula 13121517 * 256
    mov rax, 13121517    
    imul rax, 256        ; rax = 13121517 * 256

    ; Soma com o valor de rbx
    add rax, rbx        

    ; Soma com 4294979641 (valor grande)
    add rax, 4294979641  

    ; Imprime o resultado
    mov rdi, fmt         
    mov rsi, rax        
    xor rax, rax        
    call printf          

    ; Finaliza o programa
    mov rax, 60         
    xor rdi, rdi        
    syscall             
