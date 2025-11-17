# X++ Compiler 🖥️

> Um compilador (Front-end) para a linguagem X++, desenvolvido em C++ como parte da disciplina de Compiladores.

![C++](https://img.shields.io/badge/C++-17-blue.svg?style=flat&logo=c%2B%2B)
![Status](https://img.shields.io/badge/Status-Completed-green)
![Type](https://img.shields.io/badge/Type-Recursive%20Descent%20Parser-orange)

## 📖 Sobre o Projeto

Este projeto implementa as fases de **Análise Léxica (Scanner)**, **Análise Sintática (Parser)** e **Tabela de Símbolos** para a linguagem orientada a objetos **X++** (baseada na especificação do livro de *Márcio Eduardo Delamaro*).

O objetivo principal foi construir um **Analisador Descendente Preditivo (LL(1))** robusto, capaz de processar estruturas complexas como herança de classes, métodos, controle de fluxo e expressões matemáticas, reportando erros com precisão.

## ✨ Funcionalidades

- **Scanner (Lexer):**
  - Processamento de tokens, literais (strings/inteiros) e comentários (`//` e `/* */`).
  - Tratamento de palavras reservadas vs. identificadores.
- **Parser (Sintático):**
  - Implementação "na unha" de um Parser Descendente Recursivo.
  - Tratamento de estruturas aninhadas (blocos, loops, condicionais).
  - Detecção de erros sintáticos com reporte de linha.
- **Symbol Table:**
  - Gerenciamento de escopos (Global, Classe, Método).
  - Diferenciação semântica entre Keywords e IDs.

## 🔧 Refatoração da Gramática

Um dos maiores desafios técnicos deste projeto foi lidar com ambiguidades na gramática original da especificação, que impediam a implementação de um parser LL(1) puro.

**O Problema:**
A gramática original permitia que tanto declarações de variáveis quanto de métodos começassem com `Type ID` dentro do corpo da classe, gerando conflito de *First Set*.

**A Solução:**
Apliquei técnicas de **Fatoração à Esquerda (Left Factoring)** para transformar a gramática, postergando a decisão do parser até encontrar um token distintivo (como `(` ou `;`).

*Trecho da Gramática Fatorada:*
```antlr
// Gramática Original (Ambígua)
ClassBody -> { VarDeclListOpt ConstructDeclListOpt MethodDeclListOpt }

// Gramática Refatorada (Implementada)
ClassBody      -> { ClassContentListOpt }
ClassContent   -> ConstructDecl | Type ID VarOrMethod
VarOrMethod    -> VarDeclOpt ;  | MethodBody
```
## 🚀 Como Rodar

### Pré-requisitos
- Compilador `g++` (GCC) instalado.

### Compilação
Clone o repositório e compile os arquivos fonte no terminal:

```bash
g++ *.cpp -o xpp_compiler
```
### Execução
Execute o compilador passando o arquivo de código fonte `.xpp` como argumento:
```bash
./xpp_compiler teste1.xpp
```
## 📂 Estrutura do Projeto

* **`scanner.cpp/h`**: Autômato finito para reconhecimento de tokens e leitura de arquivo.
* **`parser.cpp/h`**: Lógica de derivação das regras gramaticais e tratamento de erros.
* **`symboltable.cpp/h`**: Estrutura de dados (Map) para armazenamento de símbolos e escopos.
* **`token.h`**: Definição dos Enums e estrutura dos tokens.
* **`principal.cpp`**: Ponto de entrada (main) que orquestra a execução.

## 📝 Exemplo de Código X++ Suportado

```cpp
class Exemplo {
    int x;
    
    constructor() {
        x = 10;
    }

    int calcular(int y) {
        if (y > 0) {
            return x + y;
        } else {
            return x;
        }
    }
}
```
##
Desenvolvido por Priscila Orejana - 2025.
