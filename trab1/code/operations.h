#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "structs.h"
#include <string>

const int MAX_VERSIONS = 100;
const int MAX_MODS = 4; 

// leitura
Node* ler_ptr(Node* node, std::string campo, int versao);
int ler_valor(Node* node, int versao);

// escrita
Node* write(Node* node, std::string campo, Node* ptr_val, int int_val, int versao);

// bst
Node* insert(Node* raiz, int x, int versao);
Node* remove_node(Node* raiz, int x, int versao);
Node* find_min(Node* node, int versao);

// consultas
int sucessor(Node* raiz, int x, int versao);
void inorder(Node* node, int versao, int depth);

#endif