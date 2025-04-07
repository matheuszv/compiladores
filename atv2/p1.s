.section .text
.globl _start

_start:
mov $42, %rax  # Código gerado pelo compilador
call imprime_num
call sair

.include "runtime.s"
