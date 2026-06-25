#include "vEBTree.hpp"
#include <cmath>

// Funcoes auxiliares

// upperSqrt(u) = 2^(ceil(lgu/2))
uint64_t VEBTree::upperSqrt() const {
    uint64_t lgu = 0;
    uint64_t tmp = u;
    while (tmp > 1) { tmp >>= 1; lgu++; }
    uint64_t half = (lgu + 1) / 2; // ceil(lgu/2)
    return (uint64_t)1 << half;
}

// lowerSqrt(u) = 2^(floor(lgu/2)) = u / upperSqrt(u)
uint64_t VEBTree::lowerSqrt() const {
    return u / upperSqrt();
}

uint64_t VEBTree::high(uint64_t x) const {
    return x / lowerSqrt();
}

uint64_t VEBTree::low(uint64_t x) const {
    return x % lowerSqrt();
}

uint64_t VEBTree::indexOf(uint64_t h, uint64_t l) const {
    return h * lowerSqrt() + l;
}

// Construtor / Destrutor 

VEBTree::VEBTree(uint64_t universeSize)
    : u(universeSize), min(VEB_EMPTY), max(VEB_EMPTY), summary(nullptr) {
}

VEBTree::~VEBTree() {
    // Os clusters (e seus VEBTree*) sao deletados pelo destrutor da HashTable
    delete summary;
}

//  Consultas 

bool VEBTree::isEmpty() const {
    return min == VEB_EMPTY;
}

uint64_t VEBTree::getMin() const {
    return min;
}

uint64_t VEBTree::getMax() const {
    return max;
}

VEBTree* VEBTree::getOrCreateCluster(uint64_t i) {
    VEBTree* c = clusters.get(i);
    if (c == nullptr) {
        c = new VEBTree(lowerSqrt());
        clusters.insert(i, c);
    }
    return c;
}

//  member 

bool VEBTree::member(uint64_t x) const {
    if (x == min || x == max) return true;
    if (u == 2) return false; // caso base: so min/max existem

    VEBTree* c = clusters.get(high(x));
    if (c == nullptr) return false;
    return c->member(low(x));
}

//  insert 

void VEBTree::insert(uint64_t x) {
    if (isEmpty()) {
        min = x;
        max = x;
        return;
    }

    if (x == min || x == max) return; // ja existe

    if (x < min) {
        // troca x com min: o antigo min sera inserido recursivamente
        uint64_t tmp = min;
        min = x;
        x = tmp;
    }

    if (u > 2) {
        uint64_t h = high(x);
        uint64_t l = low(x);

        VEBTree* c = getOrCreateCluster(h);

        if (c->isEmpty()) {
            // insere h no summary
            if (summary == nullptr) summary = new VEBTree(upperSqrt());
            summary->insert(h);
            c->min = l;
            c->max = l;
        } else {
            c->insert(l);
        }
    }

    if (x > max) max = x;
}

// remove 

void VEBTree::remove(uint64_t x) {
    if (isEmpty()) return;
    if (x == min && x == max) {
        // unico elemento
        min = VEB_EMPTY;
        max = VEB_EMPTY;
        return;
    }

    if (u == 2) {
        // caso base: u=2 -> elementos possiveis sao 0 e 1
        if (x == 0) {
            min = 1;
        } else {
            min = 0;
        }
        max = min;
        return;
    }

    // se x == min, precisamos encontrar o novo minimo:
    // o segundo menor elemento eh o min do menor cluster nao vazio (via summary)
    if (x == min) {
        if (summary == nullptr || summary->isEmpty()) {
            // nao deveria acontecer se max != min, mas por seguranca:
            min = max;
            return;
        }
        uint64_t firstCluster = summary->getMin();
        VEBTree* c = clusters.get(firstCluster);
        x = indexOf(firstCluster, c->getMin());
        min = x;
    }

    uint64_t h = high(x);
    uint64_t l = low(x);

    VEBTree* c = clusters.get(h);
    if (c != nullptr) {
        c->remove(l);

        if (c->isEmpty()) {
            // remove cluster vazio da hash table para liberar memoria
            clusters.remove(h);
            delete c;

            if (summary != nullptr) {
                summary->remove(h);
            }

            if (x == max) {
                if (summary == nullptr || summary->isEmpty()) {
                    max = min;
                } else {
                    uint64_t summaryMax = summary->getMax();
                    VEBTree* cMax = clusters.get(summaryMax);
                    max = indexOf(summaryMax, cMax->getMax());
                }
            }
        } else if (x == max) {
            max = indexOf(h, c->getMax());
        }
    } else {
        // cluster nao existia (so podia ser pq x era max == min de algum cluster... )
        if (x == max) {
            if (summary == nullptr || summary->isEmpty()) {
                max = min;
            } else {
                uint64_t summaryMax = summary->getMax();
                VEBTree* cMax = clusters.get(summaryMax);
                max = indexOf(summaryMax, cMax->getMax());
            }
        }
    }
}

// successor 

uint64_t VEBTree::successor(uint64_t x) const {
    if (u == 2) {
        if (x == 0 && max == 1) return 1;
        return VEB_EMPTY;
    }

    if (!isEmpty() && x < min) {
        return min;
    }

    uint64_t h = high(x);
    uint64_t l = low(x);

    VEBTree* c = clusters.get(h);

    if (c != nullptr && l < c->getMax()) {
        uint64_t offset = c->successor(l);
        if (offset == VEB_EMPTY) return VEB_EMPTY;
        return indexOf(h, offset);
    }

    // procura no summary o proximo cluster que contenha algo
    if (summary == nullptr) return VEB_EMPTY;

    uint64_t succCluster = summary->successor(h);
    if (succCluster == VEB_EMPTY) return VEB_EMPTY;

    VEBTree* c2 = clusters.get(succCluster);
    uint64_t offset = c2->getMin();
    return indexOf(succCluster, offset);
}

// predecessor 

uint64_t VEBTree::predecessor(uint64_t x) const {
    if (u == 2) {
        if (x == 1 && min == 0) return 0;
        return VEB_EMPTY;
    }

    if (!isEmpty() && x > max) {
        return max;
    }

    uint64_t h = high(x);
    uint64_t l = low(x);

    VEBTree* c = clusters.get(h);

    if (c != nullptr && l > c->getMin()) {
        uint64_t offset = c->predecessor(l);
        if (offset == VEB_EMPTY) return VEB_EMPTY;
        return indexOf(h, offset);
    }

    if (summary == nullptr) {
        // sem summary: o predecessor pode ser o min, se x > min
        if (!isEmpty() && min != VEB_EMPTY && x > min) return min;
        return VEB_EMPTY;
    }

    uint64_t predCluster = summary->predecessor(h);
    if (predCluster == VEB_EMPTY) {
        // nao ha cluster anterior; verifica se min eh menor que x
        if (!isEmpty() && min != VEB_EMPTY && x > min) return min;
        return VEB_EMPTY;
    }

    VEBTree* c2 = clusters.get(predCluster);
    uint64_t offset = c2->getMax();
    return indexOf(predCluster, offset);
}
