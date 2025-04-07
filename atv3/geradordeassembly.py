import re

assembly_template = """section .data
    fmt db "%d", 10, 0

section .text
    global main
    extern printf

main:
    {operations}

    mov rdi, fmt
    mov rsi, rax
    xor rax, rax
    call printf

    mov rax, 60
    xor rdi, rdi
    syscall
"""

def infix_to_postfix(expression):
    """Converte a expressão infixa para pós-fixa (Notação Polonesa Reversa - RPN)"""
    precedence = {'+': 1, '-': 1, '*': 2, '/': 2, '(': 0}
    output = []
    operators = []
    
    tokens = re.findall(r'\d+|[\+\-\*/\(\)]', expression.replace(" ", ""))
    
    for token in tokens:
        if token.isdigit():
            output.append(token)
        elif token == '(':
            operators.append(token)
        elif token == ')':
            while operators and operators[-1] != '(':
                output.append(operators.pop())
            operators.pop()
        else:
            while operators and precedence[operators[-1]] >= precedence[token]:
                output.append(operators.pop())
            operators.append(token)
    
    while operators:
        output.append(operators.pop())
    
    return output

def generate_assembly(expression):
    """Gera código assembly a partir de uma expressão matemática"""
    postfix = infix_to_postfix(expression)
    assembly_code = []
    register_stack = []
    register_counter = 0
    registers = ["rax", "rbx", "rcx", "rdx", "rsi", "rdi", "r8", "r9", "r10", "r11"]  # Lista de registradores válidos


    for token in postfix:
        if token.isdigit():
            reg = registers[register_counter]  # Seleciona um registrador válido da lista
            assembly_code.append(f"    mov {reg}, {token}")
            register_stack.append(reg)
            register_counter += 1
        else:
            b = register_stack.pop()
            a = register_stack.pop()
            if token == '+':
                assembly_code.append(f"    add {a}, {b}")
            elif token == '-':
                assembly_code.append(f"    sub {a}, {b}")
            elif token == '*':
                assembly_code.append(f"    imul {a}, {b}")
            elif token == '/':
                assembly_code.append("    cqo")  # Estende RAX para RDX:RAX antes da divisão
                assembly_code.append(f"    idiv {b}")
            register_stack.append(a)

    assembly_code.append(f"    mov rax, {register_stack.pop()}")  # Resultado final em RAX
    return assembly_template.format(operations="\n".join(assembly_code))

def save_assembly_file(expression, filename="q1-3.s"):
    assembly_code = generate_assembly(expression)
    with open(filename, "w") as file:
        file.write(assembly_code)
    print(f"Arquivo {filename} gerado com sucesso!")

# Exemplo de uso
expression = input("Digite uma expressão matemática: ")
save_assembly_file(expression)
