#include <vector>
#include <string>
struct Node;

struct Mod
{
    int versao;
    std::string campo; // indica qual foi o campo alterado
    Node* valor_ptr;
    int valor_int;

    Mod(int v, std::string f, Node* p = nullptr, int i = 0)
    : versao(v), campo(f), valor_ptr(p), valor_int(i) {}
};

struct Node
{
    int valor;
    Node* esq;
    Node* dir;
    
    std::vector<Node*> back_pointers;
    std::vector<Mod> mods;

    Node(int v): valor(v), esq(nullptr), dir(nullptr) {}
};
