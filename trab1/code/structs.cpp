#include "structs.h"

Mod::Mod(int v, std::string f, Node* p, int i)
    : versao(v), campo(f), valor_ptr(p), valor_int(i) {}

Node::Node(int v)
    : valor(v), esq(nullptr), dir(nullptr) {}