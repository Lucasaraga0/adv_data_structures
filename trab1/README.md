# Árvore Binária de Busca Persistente
- Linguagem utilizada: C++ 
- Versão: 
    - C++: C++17
    - g++: 13.3.0

- Como rodar. No diretório dos arquivos:
    - O arquivo MakeFile possui a variável INPUT que deve ter o nome do arquivo .txt de entrada.
    - Para compilar basta executar `make build`, que criará o arquivo executável `programa.exe`.
    - Então basta rodar `make run` para rodá-lo normalmente e, `make run > saida.txt` para salvar as saídas em um arquivo .txt.
    - O arquivo `main.cpp` contém apenas a lógica da leitura do arquivo .txt, os demais arquivos possuem as lógicas das estruturas e das operações.

## Estruturas:
- Apenas duas estruturas são criadas, `Mod` e `Node`. Ambas são definidas no arquivo `struct.h`. O arquivo `struct.cpp` possui os métodos construtores das estruturas.

- Mod: 
    - `versao`: inteiro que a versão em que a alteração ocorreu.
    - `campo`: String indicando qual o campo alterado (esquerda, direita, valor).
    - `valor_ptr`: ponteiro usado para quando a modicação altera um ponteiro do nó.
    - `valor_int`: inteiro usado para quando a modificação altera o valor do nó. 

- Node:
    - `valor`: valor do Nó.
    - `esq`: ponteiro para nó esquerdo.
    - `dir`: ponteiro para nó Direito.

    - `back pointers`: vetor de ponteiros de `Node` para retorno, ou seja, nós que referenciam o nó atual.
    - `mods`: vetor de `Mods` associada ao nó.

## Operações
- Além das 4 operações definidas na especificação do trabalho: `Inserir`, `Remover`, `Sucessor` e `Imprimir`, são usadas as funções auxiliares de `ler_ptr`, `ler_valor`, `write` e  `find_min`. O arquivo `operations.cpp` possui a implementação de todas as operações citadas, enquanto o `operations.h` contém as declarações dessas operações.

- ```c++ 
    Node* ler_ptr(Node* node, string campo, int versao)
    ```
    - Percorre o vetor de Mods de um nó e retorna o campo (esquerdo ou direito) da versão mais recente que seja menor ou igual a versão solicitada. 
    - Se existir a modificação que corresponda as solicitações (campo e versão) o ponteiro correspondente é retornado. Se não, retorna o valor do nó. 

- ```c++ 
    int ler_valor(Node* node, int versao)
    ```
    - Basicamente a mesma função anterior mas retornando o campo de valor em vez de esquerda ou direita. Se não existir modificação válida com as solicitações, a função retorna o valor original do nó.

- ```c++ 
    Node* find_min(Node* node, int versao)
    ```
    - Encontra o menor valor de uma subárvore em determinada versao.

- ```c++ 
    Node* write(Node* node, string campo, Node* ptr_val, int int_val, int versao)
    ```
    - Escreve uma nova modificação no vetor de mods. Caso o vetor de mods não esteja cheio, a modificação é apenas colocada ao final do vetor e a função retorna o nó. 
    - Caso esteja cheio, um novo nó deve ser criado para que a mudança seja registrada. Os valores atuais do nó são copiados para o novo nó, os filhos são atualizados e o campo da modificação é registrado.
    - Após isso, os nós do vetor de retorno devem ser "notificados" da mudança. Por isso, percorre-se o vetor de *back_pointers* e a função de `write` é chamada recursivamente para notificar a mudança para o novo nó. Depois disso o novo nó criado é retornado.

- ```c++
    Node* insert(Node* raiz, int x, int versao)
    ```
    - Insere um novo elemento na arvore.
    - A inserção segue o padrão de uma árvore binária de busca comum, busca-se o local correto para o novo nó de forma recursiva.
    - Quando o local é encontrado, um novo nó é criado e a função `write` é chamada para fazer as alterações necessárias na estrutura persistente (mods e *back_pointers*).

- ```c++
    Node* remove_node(Node* raiz, int x, int versao)
    ```
    - Remove um elemento da árvore. Assim como na inserção, a busca pelo nó correto é como em uma árvore binária de busca comum.
    - Depois de encontrar o nó, é analisado a quantidade de filhos do nó. Se ele possuir apenas um, o ponteiro é simplesmente ajustado para seu filho. Caso possua os dois filhos, a função de `find_min` é usada para encontrar o sucessor da subárvore direita.
    - Após isso, `write` é chamado para garantir a propriedade de persistência .

- ```c++
    int sucessor(Node* raiz, int x, int versao)
    ```
    - Busca o sucessor de um valor x, na versão especificada.
    - Percorre iterativamente a árvore até encontrar o menor sucessor de x.
    - Caso não exista sucessor, retorna-se infinito.

- ```c++
    void inorder(Node* node, int versao, int profundidade)
    ```
    - Faz o percurso em ordem da árvore e imprime os valores dos nós. 