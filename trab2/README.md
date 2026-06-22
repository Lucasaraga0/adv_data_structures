# Árvore van Emde Boas (vEB)
- Linguagem utilizada: C++
- Versão:
    - C++: C++17
    - g++: 13.3.0

- Como rodar. No diretório dos arquivos:
    - Para compilar basta executar `make build`, que criará o arquivo executável `programa.exe`.
    - Para rodar, execute `make run INPUT=arquivo.txt`, onde `arquivo.txt` é o arquivo de entrada com as operações. As saídas de `SUC`, `PRE` e `IMP` são impressas no terminal (pode-se usar `> saida.txt` para salvar em arquivo).
    - O arquivo `main.cpp` contém apenas a lógica de leitura do arquivo de entrada e a chamada das operações; os demais arquivos possuem as lógicas das estruturas.

## Estruturas
- Duas estruturas são criadas: `HashTable` (em `HashTable.hpp`/`HashTable.cpp`) e `VEBTree` (em `vEBTree.hpp`/`vEBTree.cpp`).

- HashTable:
    - Implementação própria de uma tabela de dispersão dinâmica, usada para mapear o índice de um cluster para o ponteiro da subárvore vEB correspondente.
    - `table`: vetor de listas encadeadas (`Node*`), começando com tamanho 2.
    - `numElements`: quantidade de pares chave/valor armazenados.
    - `hashFunc`: função de hash multiplicativa (constante de Knuth).
    - Colisões são tratadas por encadeamento (chaining).
    - A tabela dobra de tamanho (table doubling) quando o fator de carga ultrapassa 0.75, e reduz à metade (table halving) quando o fator de carga cai abaixo de 0.25, nunca ficando menor que o tamanho inicial (2). Isso garante espaço Θ(n) em relação ao número de elementos armazenados.

- VEBTree:
    - `u`: tamanho do universo daquele nó (cai de 2³² até 2 nas chamadas recursivas).
    - `min` / `max`: menor e maior elemento armazenado na (sub)árvore.
    - `summary`: ponteiro para uma subárvore vEB de tamanho `upperSqrt(u)`, que indica quais clusters estão não vazios.
    - `clusters`: instância de `HashTable` que mapeia o índice de cada cluster não vazio para a subárvore vEB correspondente (de tamanho `lowerSqrt(u)`).
    - Seguindo a otimização clássica de van Emde Boas, o `min` de cada nó **não** é inserido recursivamente nos clusters/summary; o `max` é apenas uma cópia do maior valor armazenado.

## Operações
- Além das operações `INC`, `REM`, `SUC`, `PRE` e `IMP` definidas na especificação do trabalho, são usadas as funções auxiliares `high`, `low`, `indexOf`, `lowerSqrt`, `upperSqrt` e `getOrCreateCluster`. O arquivo `vEBTree.cpp` possui a implementação de todas as operações citadas, enquanto `vEBTree.hpp` contém as declarações.

- ```c++
    uint64_t high(uint64_t x) const
    ```
    - Calcula o índice do cluster ao qual `x` pertence, dividindo `x` por `lowerSqrt(u)`.

- ```c++
    uint64_t low(uint64_t x) const
    ```
    - Calcula a posição de `x` dentro do seu cluster, pelo resto da divisão de `x` por `lowerSqrt(u)`.

- ```c++
    uint64_t indexOf(uint64_t high, uint64_t low) const
    ```
    - Operação inversa de `high`/`low`: reconstrói o valor original `x = high * lowerSqrt(u) + low`.

- ```c++
    uint64_t lowerSqrt() const / uint64_t upperSqrt() const
    ```
    - Calculam, respectivamente, 2^⌊lg(u)/2⌋ e 2^⌈lg(u)/2⌉, usados para determinar o tamanho dos clusters e do summary em cada nível.

- ```c++
    VEBTree* getOrCreateCluster(uint64_t i)
    ```
    - Busca o cluster `i` na `HashTable`. Se não existir, cria uma nova subárvore vEB de tamanho `lowerSqrt(u)` e a insere na tabela.

- ```c++
    void insert(uint64_t x)
    ```
    - Insere `x` na (sub)árvore. Se a árvore estiver vazia, `x` se torna `min` e `max`. Se `x` for menor que `min`, eles são trocados antes da recursão (garantindo que `min` nunca seja propagado para os clusters). A inserção recursiva só ocorre se `u > 2`, atualizando `summary` quando o cluster de destino estava vazio.

- ```c++
    void remove(uint64_t x)
    ```
    - Remove `x` da estrutura. Caso `x` seja o `min`, o novo mínimo é obtido a partir do cluster indicado pelo `summary`. Após a remoção recursiva, se o cluster correspondente ficar vazio, ele é removido da `HashTable` (com `delete` da subárvore) e também removido do `summary`, liberando memória. O caso base (`u == 2`) trata diretamente os valores 0 e 1.

- ```c++
    uint64_t successor(uint64_t x) const
    ```
    - Retorna o sucessor estritamente maior que `x`, ou `VEB_EMPTY` se não existir. Usa o `summary` para localizar o próximo cluster não vazio quando necessário.

- ```c++
    uint64_t predecessor(uint64_t x) const
    ```
    - Análoga à `successor`, retornando o predecessor estritamente menor que `x`, ou `VEB_EMPTY` se não existir.

- ```c++
    void printIMP(const VEBTree* root)
    ```
    - (em `main.cpp`) Imprime o `min` da raiz e, para cada cluster de primeiro nível não vazio, imprime `C:` seguido de todos os seus elementos (obtidos recursivamente via `collectClusterElements`), no formato `IMP Min: 10, C: 1, 2, C: 100`.

## Observações
- O universo utilizado é de 32 bits (u = 2³²), e todos os valores são tratados com `uint64_t` para evitar problemas de sinal e overflow.
- A recursão da vEB termina no caso base `u = 2`.
- A memória é liberada corretamente: o destrutor de `VEBTree` deleta sua `summary`, e o destrutor de `HashTable` deleta todas as subárvores referenciadas pelos clusters, evitando vazamentos de memória.