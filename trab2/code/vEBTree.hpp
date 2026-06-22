#ifndef VEBTREE_HPP
#define VEBTREE_HPP

#include <cstdint>
#include "HashTable.hpp"

// Valor sentinela que representa "vazio" para min/max
const uint64_t VEB_EMPTY = UINT64_MAX;

class VEBTree {
private:
    uint64_t u;        // tamanho do universo deste nivel
    uint64_t min;
    uint64_t max;
    VEBTree* summary;  // subarvore de resumo (tamanho sqrt(u))
    HashTable clusters; // mapa: indice do cluster -> VEBTree*

    // Funcoes auxiliares de particionamento de bits
    uint64_t high(uint64_t x) const;
    uint64_t low(uint64_t x) const;
    uint64_t indexOf(uint64_t high, uint64_t low) const; // reconstroi x = high*sqrt(u)+low
    uint64_t lowerSqrt() const; // sqrt inferior de u
    uint64_t upperSqrt() const; // sqrt superior de u

    // Obtem (criando se necessario) o cluster i
    VEBTree* getOrCreateCluster(uint64_t i);

public:
    explicit VEBTree(uint64_t universeSize);
    ~VEBTree();

    bool isEmpty() const;
    uint64_t getMin() const;
    uint64_t getMax() const;

    bool member(uint64_t x) const;
    void insert(uint64_t x);
    void remove(uint64_t x);

    // Retorna VEB_EMPTY se nao existir sucessor/predecessor
    uint64_t successor(uint64_t x) const;
    uint64_t predecessor(uint64_t x) const;

    // Para impressao: percorre clusters nao vazios em ordem,
    // chamando func(clusterIndex, subtree) para cada cluster nao vazio.
    template <typename Func>
    void forEachCluster(Func func) const {
        clusters.forEachSorted(func);
    }

    uint64_t getU() const { return u; }
};

#endif 
