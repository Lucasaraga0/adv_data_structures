#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include <cstdint>
#include <vector>

class VEBTree; // forward declaration

// Tabela de dispersao dinamica (chave: uint64_t -> valor: VEBTree*)
// table doubling/halving.

class HashTable {
private:
    struct Node {
        uint64_t key;
        VEBTree* value;
        Node* next;
        Node(uint64_t k, VEBTree* v, Node* n) : key(k), value(v), next(n) {}
    };

    std::vector<Node*> table;
    size_t numElements;

    static const size_t INITIAL_SIZE = 2;

    size_t hashFunc(uint64_t key, size_t tableSize) const;
    void resize(size_t newSize);

public:
    HashTable();
    ~HashTable();

    // Insere ou atualiza o valor associado a key
    void insert(uint64_t key, VEBTree* value);

    // Retorna o valor associado a key, ou nullptr se nao existir
    VEBTree* get(uint64_t key) const;

    // Remove a entrada associada a key (se existir)
    void remove(uint64_t key);

    // Verifica se a chave existe
    bool contains(uint64_t key) const;

    size_t size() const;
    size_t capacity() const;

    // Itera sobre todas as chaves armazenadas, em ordem crescente,
    // chamando func(key, value) para cada uma.
    template <typename Func>
    void forEachSorted(Func func) const {
        // Coleta todas as chaves
        std::vector<uint64_t> keys;
        keys.reserve(numElements);
        std::vector<VEBTree*> values;
        values.reserve(numElements);

        for (size_t i = 0; i < table.size(); ++i) {
            Node* cur = table[i];
            while (cur != nullptr) {
                keys.push_back(cur->key);
                values.push_back(cur->value);
                cur = cur->next;
            }
        }

        // Ordenacao simples
        for (size_t i = 1; i < keys.size(); ++i) {
            uint64_t k = keys[i];
            VEBTree* v = values[i];
            size_t j = i;
            while (j > 0 && keys[j - 1] > k) {
                keys[j] = keys[j - 1];
                values[j] = values[j - 1];
                --j;
            }
            keys[j] = k;
            values[j] = v;
        }

        for (size_t i = 0; i < keys.size(); ++i) {
            func(keys[i], values[i]);
        }
    }
};

#endif 
