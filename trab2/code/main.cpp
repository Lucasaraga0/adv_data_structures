#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdint>
#include <vector>
#include "vEBTree.hpp"

// Universo de 32 bits: u = 2^32
static const uint64_t UNIVERSE_SIZE = (uint64_t)1 << 32;

// Imprime recursivamente os elementos de todos os clusters nao vazios,
// na ordem em que aparecem (ordem dos indices de cluster), no formato:
// "C: e1, e2, ..."  -- cada cluster nao vazio gera um bloco "C: ..."
// contendo TODOS os elementos daquele cluster (incluindo subclusters), achatados.
static void collectClusterElements(const VEBTree* node, uint64_t base, std::vector<uint64_t>& out) {
    if (node->isEmpty()) return;

    // min nunca e inserido recursivamente nos clusters, entao e adicionado aqui
    out.push_back(base + node->getMin());

    if (node->getU() == 2) {
        // caso base: se min != max, max = 1 e min = 0; ambos ja foram ou serao coletados
        if (node->getMin() != node->getMax()) {
            out.push_back(base + node->getMax());
        }
    } else {
        node->forEachCluster([&](uint64_t idx, VEBTree* sub) {
            uint64_t lowerSqrtVal = sub->getU();
            uint64_t subBase = base + idx * lowerSqrtVal;
            collectClusterElements(sub, subBase, out);
        });
    }
}

static void printIMP(const VEBTree* root) {
    std::cout << "IMP" << std::endl << "Min: ";
    if (root->isEmpty()) {
        std::cout << "-INF";
    } else {
        std::cout << root->getMin();
    }

    // Para cada cluster de primeiro nivel nao vazio
    root->forEachCluster([&](uint64_t idx, VEBTree* sub) {
        if (sub->isEmpty()) return;
        uint64_t base = idx * sub->getU();
        std::vector<uint64_t> elems;
        collectClusterElements(sub, base, elems);
        std::cout << ", C[" << idx << "]: ";
        for (size_t i = 0; i < elems.size(); ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << elems[i];
        }
    });

    std::cout << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <arquivo_de_entrada.txt>" << std::endl;
        return 1;
    }

    std::ifstream infile(argv[1]);
    if (!infile.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << argv[1] << std::endl;
        return 1;
    }

    VEBTree* tree = new VEBTree(UNIVERSE_SIZE);

    std::string line;
    while (std::getline(infile, line)) {
        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string op;
        iss >> op;

        if (op == "INC") {
            uint64_t x;
            iss >> x;
            tree->insert(x);
        } else if (op == "REM") {
            uint64_t x;
            iss >> x;
            tree->remove(x);
        } else if (op == "SUC") {
            uint64_t x;
            iss >> x;
            std::cout<< "SUC " << x << std::endl;
            uint64_t result = tree->successor(x);
            if (result == VEB_EMPTY) {
                std::cout << "+INF" << std::endl;
            } else {
                std::cout << result << std::endl;
            }
        } else if (op == "PRE") {
            uint64_t x;
            iss >> x;
            std::cout<< "PRE " << x << std::endl;
            uint64_t result = tree->predecessor(x);
            if (result == VEB_EMPTY) {
                std::cout << "-INF" << std::endl;
            } else {
                std::cout << result << std::endl;
            }
        } else if (op == "IMP") {
            printIMP(tree);
        }
    }

    delete tree;
    return 0;
}