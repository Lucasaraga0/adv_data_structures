#ifndef STRUCTS_H
#define STRUCTS_H

#include <vector>
#include <string>

struct Node;

struct Mod
{
    int versao;
    std::string campo;
    Node* valor_ptr;
    int valor_int;

    Mod(int v, std::string f, Node* p = nullptr, int i = 0);
};

struct Node
{
    int valor;
    Node* esq;
    Node* dir;

    std::vector<Node*> back_pointers;
    std::vector<Mod> mods;

    Node(int v);
};

#endif