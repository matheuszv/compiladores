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

    return 0;
}
