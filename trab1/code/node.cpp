#include "mods.cpp"
#include <vector>

struct Node
{
    int valor;
    Node* esq;
    Node* dir;
    
    std::vector<Node*> back_pointers;
    std::vector<Mods> mods;
};

