#include "HashTable.hpp"
#include "vEBTree.hpp"

HashTable::HashTable() : numElements(0) {
    table.assign(INITIAL_SIZE, nullptr);
}

HashTable::~HashTable() {
    for (size_t i = 0; i < table.size(); ++i) {
        Node* cur = table[i];
        while (cur != nullptr) {
            Node* tmp = cur;
            cur = cur->next;
            // Deleta a subarvore vEB associada
            delete tmp->value;
            delete tmp;
        }
    }
}

size_t HashTable::hashFunc(uint64_t key, size_t tableSize) const {
    // Multiplicative hashing (Knuth) para boa dispersao
    const uint64_t A = 2654435769ULL; // constante de Knuth (32-bit)
    uint64_t h = (key * A);
    return static_cast<size_t>(h % tableSize);
}

void HashTable::resize(size_t newSize) {
    if (newSize < INITIAL_SIZE) newSize = INITIAL_SIZE;

    std::vector<Node*> newTable(newSize, nullptr);

    for (size_t i = 0; i < table.size(); ++i) {
        Node* cur = table[i];
        while (cur != nullptr) {
            Node* next = cur->next;
            size_t idx = hashFunc(cur->key, newSize);
            cur->next = newTable[idx];
            newTable[idx] = cur;
            cur = next;
        }
    }

    table.swap(newTable);
}

void HashTable::insert(uint64_t key, VEBTree* value) {
    size_t idx = hashFunc(key, table.size());
    Node* cur = table[idx];
    while (cur != nullptr) {
        if (cur->key == key) {
            cur->value = value;
            return;
        }
        cur = cur->next;
    }

    // Nao existe ainda: insere novo no inicio da lista
    table[idx] = new Node(key, value, table[idx]);
    numElements++;

    // Table doubling: fator de carga > 0.75
    if (numElements > (table.size() * 3) / 4) {
        resize(table.size() * 2);
    }
}

VEBTree* HashTable::get(uint64_t key) const {
    size_t idx = hashFunc(key, table.size());
    Node* cur = table[idx];
    while (cur != nullptr) {
        if (cur->key == key) return cur->value;
        cur = cur->next;
    }
    return nullptr;
}

bool HashTable::contains(uint64_t key) const {
    return get(key) != nullptr;
}

void HashTable::remove(uint64_t key) {
    size_t idx = hashFunc(key, table.size());
    Node* cur = table[idx];
    Node* prev = nullptr;

    while (cur != nullptr) {
        if (cur->key == key) {
            if (prev == nullptr) {
                table[idx] = cur->next;
            } else {
                prev->next = cur->next;
            }
            delete cur; // NAO deleta o value aqui; quem chama controla a subarvore
            numElements--;

            // Table halving: fator de carga < 0.25, mas nunca abaixo do tamanho inicial
            if (table.size() > INITIAL_SIZE && numElements < table.size() / 4) {
                size_t newSize = table.size() / 2;
                if (newSize < INITIAL_SIZE) newSize = INITIAL_SIZE;
                resize(newSize);
            }
            return;
        }
        prev = cur;
        cur = cur->next;
    }
}

size_t HashTable::size() const {
    return numElements;
}

size_t HashTable::capacity() const {
    return table.size();
}
