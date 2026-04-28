#include "structs.h"
#include "operations.h"
#include <iostream>

Node* ler_ptr(Node* node, std::string campo, int versao){
    Node* result = nullptr;
    int ult_versao = -1;

    for (auto &no : node->mods){
        if (no.campo == campo && no.versao <= versao){
            if (no.versao > ult_versao){
                ult_versao = no.versao;
                result = no.valor_ptr;
            }
        }
    }
    
    if (result) return result;

    if (campo == "esq") return node->esq;
    if (campo == "dir") return node->dir;

    return nullptr;
}

int ler_valor(Node* node, int versao){
    int val = node->valor;
    for (auto &no : node->mods){
        if (no.campo == "val" && no.versao <= versao){
            val = no.valor_int;
        }
    }
    return val;
}

Node* write(Node* node, std::string campo, Node* ptr_val, int int_val, int versao){
    // se o numero de mods nao estiver cheio 
    if ((int)node->mods.size() < MAX_MODS) {
        node->mods.emplace_back(versao, campo, ptr_val, int_val);
        return node;
    }
    
    //mod cheio, copiar para novo no 
    Node* new_node = new Node(ler_valor(node, versao));
    new_node->esq = ler_ptr(node, "esq", versao);
    new_node->dir = ler_ptr(node, "dir", versao);

    // atualizar os filhos do no 
    if (new_node->esq) new_node->esq->back_pointers.push_back(new_node);
    if (new_node->dir) new_node->dir->back_pointers.push_back(new_node);
    
    // 
if (campo == "esq") new_node->esq = ptr_val;
if (campo == "dir") new_node->dir = ptr_val;
if (campo == "val") new_node->valor = int_val;

for (Node* back : node->back_pointers){
    if (ler_ptr(back, "esq", versao) == node){
        write(back, "esq", new_node, 0, versao);
    }
    
    if (ler_ptr(back, "dir", versao) == node){
        write(back, "dir", new_node, 0, versao);
    }    
}
    return new_node;
}

Node* insert(Node* raiz, int x, int versao){
    if (!raiz) return new Node(x);

    int val = ler_valor(raiz, versao);

    if (x< val){
        Node* esq = ler_ptr(raiz, "esq", versao);
        Node* new_esq = insert(esq, x, versao);
        Node* new_raiz = write(raiz, "esq", new_esq, 0, versao);
        new_esq->back_pointers.push_back(new_raiz);
        return new_raiz;
    }

    else{
        Node* dir = ler_ptr(raiz, "dir", versao);
        Node* new_dir = insert(dir, x, versao);
        Node* new_raiz = write(raiz, "dir", new_dir, 0, versao);
        new_dir->back_pointers.push_back(new_raiz);
        return new_raiz;
    }
}

Node* find_min(Node* node, int versao){
    while(ler_ptr(node, "esq", versao)){
        node = ler_ptr(node, "esq", versao);
    }
    return node;
}

Node* remove_node(Node* raiz, int x, int versao){
    if (!raiz) return nullptr;

    int val = ler_valor(raiz, versao);

    if (x< val) {
        Node* esq = ler_ptr(raiz, "esq", versao);
        Node* new_esq = remove_node(esq, x, versao);
        return write(raiz, "esq", new_esq, 0, versao);
    }

    else if (x>val){        
        Node* dir = ler_ptr(raiz, "dir", versao);
        Node* new_dir = remove_node(dir, x, versao);
        return write(raiz, "dir", new_dir, 0, versao);
    }

    else{
        Node* esq = ler_ptr(raiz, "esq", versao);
        Node* dir = ler_ptr(raiz, "dir", versao);

        if (!esq) return dir;
        if (!dir) return esq;

        Node* succ = find_min(dir, versao);
        int succ_val = ler_valor(succ, versao);

        Node* new_right = remove_node(dir, succ_val, versao);
        Node* new_root = write(raiz, "val", nullptr, succ_val, versao);
        return write(new_root, "dir", new_right, 0, versao);
    }

}

int sucessor(Node* raiz, int x, int versao){
    int succ = __INT_MAX__;
    while(raiz){
        int val = ler_valor(raiz, versao);
        if (val> x){
            succ = val;
            raiz = ler_ptr(raiz, "esq", versao);
        }
        else{
            raiz = ler_ptr(raiz, "dir", versao);
        }
    }
    return succ;
}

void inorder(Node* node, int versao, int depth) {
    if (!node) return;
    inorder(ler_ptr(node, "esq", versao), versao, depth + 1);
    std::cout << ler_valor(node, versao) << "," << depth << " ";
    inorder(ler_ptr(node, "dir", versao), versao, depth + 1);
}