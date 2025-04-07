#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <stack>
#include <cctype>


using namespace std;

// Definindo a estrutura de um nó da árvore
enum NodeType {
    NUMBER,
    OPERATOR
};

struct Node {
    string value;  // valor do nó (pode ser operador ou operando)
    Node* left;    // filho esquerdo
    Node* right;   // filho direito
    NodeType type;

    // Construtor do nó
    Node(string val, NodeType t) : value(val), left(nullptr), right(nullptr), type(t) {}
};

// Função para verificar se um caractere é um operador
bool isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/');
}


void showError(int Erro){
    printf("Erro no caractere: %d \n", Erro+1);
}


// Função para construir a árvore binária a partir da expressão
Node* buildExpressionTree(const string& expr) {
    stack<Node*> nodes;
    stack<char> ops;
    
    for (size_t i = 0; i < expr.size(); ++i) {
        char c = expr[i];
        
        // Se for um número, adiciona à pilha
        if (isdigit(c)) {
            string num = "";
            while (i < expr.size() && isdigit(expr[i])) {
                num += expr[i];
                i++;
            }
            i--; // Ajusta o índice após o número completo
            nodes.push(new Node(num, NUMBER));;
        }
        // Se for um operador, empilha
        else if (isOperator(c)) {
            while (!ops.empty() && (ops.top() == '*' || ops.top() == '/') && (c == '+' || c == '-')) {
                // Se o operador atual tem menor prioridade, realiza a operação
                char op = ops.top();
                ops.pop();
                Node* right = nodes.top(); nodes.pop();
                Node* left = nodes.top(); nodes.pop();
                Node* node = new Node(string(1, op), OPERATOR);
                node->left = left;
                node->right = right;
                nodes.push(node);
            }
            ops.push(c);
        }
        // Se for um parêntese de abertura, apenas empilha
        else if (c == '(') {
            ops.push(c);
        }
        // Se for um parêntese de fechamento, resolve a expressão interna
        else if (c == ')') {
            while (ops.top() != '(') {
                char op = ops.top();
                ops.pop();
                Node* right = nodes.top(); nodes.pop();
                Node* left = nodes.top(); nodes.pop();
                Node* node = new Node(string(1, op), OPERATOR);
                node->left = left;
                node->right = right;
                nodes.push(node);
            }
            ops.pop(); // Remove o '(' da pilha
        }
        else if(isspace(c)){

        } else {
            showError((int)i);
        }
    }

    // Finaliza qualquer operação restante
    while (!ops.empty()) {
        char op = ops.top();
        ops.pop();
        Node* right = nodes.top(); nodes.pop();
        Node* left = nodes.top(); nodes.pop();
        Node* node = new Node(string(1, op), OPERATOR);
        node->left = left;
        node->right = right;
        nodes.push(node);
    }
    
    return nodes.top(); // O topo da pilha terá a árvore construída
}

// Função para imprimir a árvore binária em ordem (in-order traversal)
void printInOrder(Node* node) {
    if (node != nullptr) {
        
        cout << node->value << " ";  // Imprime o valor do nó
        printInOrder(node->left);  // Imprime o subárvore esquerda
        printInOrder(node->right);  // Imprime o subárvore direita
    }
}

int evaluateExpressionTree(Node* node) {
    // Se o nó for um número, retorna o valor numérico
    if (node->type == NUMBER) {
        return atoi(node->value.c_str());
    }
    
    // Se for um operador, resolve as subárvores esquerda e direita
    int leftValue = evaluateExpressionTree(node->left);
    int rightValue = evaluateExpressionTree(node->right);
    
    // Aplica o operador
    if (node->value == "+") {
        return leftValue + rightValue;
    } else if (node->value == "-") {
        return leftValue - rightValue;
    } else if (node->value == "*") {
        return leftValue * rightValue;
    } else if (node->value == "/") {
        return leftValue / rightValue;
    }
    
    return 0; // Caso não reconheça o operador (não deveria acontecer)
}


void generateAssembler(Node* node) {
    if (!node) return;
    
    // Percorre a esquerda e a direita primeiro (pós-ordem)
    generateAssembler(node->left);
    generateAssembler(node->right);
    
    if (node->type == NUMBER) {
        cout << "mov $" << node->value << ", %rax" << endl;
        cout << "push %rax" << endl;
    } else if (node->type == OPERATOR) {
        cout << "pop %rbx" << endl;
        cout << "pop %rax" << endl;
        
        if (node->value == "+") {
            cout << "add %rbx, %rax" << endl;
        } else if (node->value == "-") {
            cout << "sub %rbx, %rax" << endl;
        } else if (node->value == "*") {
            cout << "imul %rbx, %rax" << endl;
        } else if (node->value == "/") {
            cout << "cqo" << endl; // Sinaliza que %rax é 64 bits
            cout << "idiv %rbx" << endl;
        }
        
        cout << "push %rax" << endl;
    }
}


std::string generateAssembly(Node* node) {
    if (!node) return "";

    std::string asmCode = "";
    
    // Percorre a esquerda e a direita primeiro (pós-ordem)
    asmCode += generateAssembly(node->left);
    asmCode += generateAssembly(node->right);
    
    if (node->type == NUMBER) {
        asmCode += "    mov $" + node->value + ", %rax\n";
        asmCode += "    push %rax\n";
    } else if (node->type == OPERATOR) {

        asmCode += "    pop %rbx\n";
        asmCode += "    pop %rax\n";
        
        if (node->value == "+") {
            asmCode += "    add %rbx, %rax\n";
        } else if (node->value == "-") {
            asmCode += "    sub %rbx, %rax\n";
        } else if (node->value == "*") {
            asmCode += "    imul %rbx, %rax\n";
        } else if (node->value == "/") {
            asmCode += "    cqo\n";
            asmCode += "    idiv %rbx\n";
        }
        
        asmCode += "    push %rax\n";
    }
    
    return asmCode;
}

void generate_asm(const std::string& operations) {
    std::ofstream out("output.s");
    
    out << "section .data\n";
    out << "    fmt db \"%d\", 10, 0\n\n";
    out << "section .text\n";
    out << "    global main\n";
    out << "    extern printf\n\n";
    out << "main:\n";
    out << operations << "\n";
    out << "    mov rdi, fmt\n";
    out << "    mov rsi, rax\n";
    out << "    xor eax, eax\n";
    out << "    call printf\n\n";
    out << "    mov eax, 0\n";
    out << "    ret\n";
}


int main() {
    ifstream inputFile("entrada.txt");
    
    if (!inputFile) {
        cerr << "Erro ao abrir o arquivo!" << endl;
        return 1;
    }
    
    // Lendo o conteúdo do arquivo
    string expression;
    getline(inputFile, expression);  // Lê a linha inteira do arquivo
    
    inputFile.close();
    
    // Construindo a árvore
    Node* tree = buildExpressionTree(expression);
    
    // Imprimindo a árvore em ordem
    cout << "Árvore binária da expressão: ";
    printInOrder(tree);
    int result = evaluateExpressionTree(tree);
    printf("\nResultado da expressão: %d", result);
    cout << endl;

    cout << "\n# Código Assembly gerado" << endl;
    
    generateAssembler(tree);
    std::string operations = generateAssembly(tree);
    generate_asm(operations);

    return 0;
}
