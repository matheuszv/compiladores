import sys
import subprocess

# Modelo de código assembly
modelo_assembly = """\
.section .text
.globl _start

_start:
mov ${}, %rax  # Código gerado pelo compilador
call imprime_num
call sair

.include "runtime.s"

"""

def compilar(arquivo_entrada):
    try:
        # 1. Ler o número do arquivo de entrada
        with open(arquivo_entrada, "r") as f:
            numero = f.read().strip()
            if not numero.isdigit():
                raise ValueError("O arquivo deve conter apenas um número inteiro.")

        # 2. Criar o nome do arquivo de saída (.s)
        arquivo_saida = arquivo_entrada.replace(".ci", ".s")

        # 3. Gerar o código assembly com o número lido
        codigo_assembly = modelo_assembly.format(numero)

        # 4. Salvar no arquivo .s
        with open(arquivo_saida, "w") as f:
            f.write(codigo_assembly)
        print(f"Arquivo assembly gerado: {arquivo_saida}")

        # 5. Montar e Linkar
        objeto = arquivo_saida.replace(".s", ".o")
        executavel = arquivo_saida.replace(".s", "")

        subprocess.run(["as", "--64", "-o", objeto, arquivo_saida], check=True)
        subprocess.run(["ld", "-o", executavel, objeto], check=True)

        print(f"Compilação concluída! Execute `./{executavel}` para ver o resultado.")

    except FileNotFoundError:
        print(f"Erro: O arquivo {arquivo_entrada} não foi encontrado.")
        sys.exit(1)
    except ValueError as e:
        print(f"Erro: {e}")
        sys.exit(1)
    except subprocess.CalledProcessError:
        print("Erro ao montar e linkar o código assembly.")
        sys.exit(1)

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Uso: python compilador.py <arquivo.ci>")
        sys.exit(1)
    
    arquivo_entrada = sys.argv[1]
    compilar(arquivo_entrada)
