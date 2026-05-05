# Árvore Binária de Busca Persistente
- Linguagem utilizada: C++ 
- Versão: 
    - C++: C++17
    - g++: 13.3.0

- Como rodar. No diretório dos arquivos:
    - O arquivo MakeFile possui a variável INPUT que deve ter o nome do arquivo .txt de entrada.
    - Para compilar basta executar `make build`, que criará o arquivo executável `programa.exe`.
    - Então basta rodar `make run` para rodá-lo normalmente e, `make run > saida.txt` para salvar as saídas em um arquivo .txt.
    - O arquivo ```main.cpp``` contém apenas a lógica da leitura do arquivo .txt, os demais arquivos possuem as lógicas das estruturas e das operações.

## Estruturas:
- Apenas duas estruturas são criadas, `Mod` e `Node`. Ambas são definidas no arquivo `struct.h`. O arquivo `struct.cpp` possui os métodos construtores das estruturas.

- Mod: 
    - Possui inteiro indicando a versão.
    - String indicando qual o campo foi alterado (esquerda, direita, valor).
    - Ponteiro para o nó.
    - Inteiro para o valor. 

- Node:
    - Valor do Nó.
    - Nó esquerdo.
    - Nó Direito.

    - Vetor de ponteiros de `Node` para retorno (*back_pointers*)
    - Vetor de `Mods`.

## Operações
- Além das 4 operações definidas na especificação do trabalho: `Inserir`, `Remover`, `Sucessor` e `Imprimir`, são usadas as funções auxiliares de `ler_ptr`, `ler_valor`, `write` e  `find_min`. O arquivo `operations.cpp` possui a implementação de todas as operações citadas, enquanto o `operations.h` contém as declarações dessas operações.

- ```c++ 
    Node* ler_prt(Node* node, string campo, int versão)
    ```
    - Percorre o vetor de Mods de um nó e retorna o campo (esquerdo ou direito) solicitado na versão menor ou igual a solicitada. 

- ```c++ 
    int ler_valor(Node* node, int versão)
    ```
    - Basicamente a mesma função anterior mas retornando o campo de valor em vez de esquerda ou direita.

- ```c++ 
    int find_min(Node* node, int versão)
    ```
    - Encontra o menor valor de uma subárvore em determinada versao.

- ```c++ 
    Node* write(Node* node, string campo, Node* ptr_val, int int_val, int versao)
    ```
    - Escreve uma nova modificação no vetor de mods. Caso o vetor de mods não esteja cheio, a modificação é apenas colocada ao final do vetor e a função retorna o nó. 
    - Caso esteja cheio, um novo no deve ser criado para que a mudança seja registrada. Os valores atuais do no são copiados para o novo no, os filhos são atualizados e o campo da modificação é registrado.
    - Após isso, os nós do vetor de retorno devem ser "notificados" da mudança. Por isso, percorre-se o vetor de *back_pointers* e a função de `write` é chamada recursivamente para notificar a mudança para o novo nó. Depois disso o novo nó criado é retornado.

- ```c++
    Node* insert(Node* raiz, int x, int versao)
    ```
    - Insere um novo valor na arvore.
    - A inserção segue o padrão de uma árvore binária de busca comum, busca-se o local correto para o novo nó de forma recursiva. Quando o local é encontrado, a função `write` é chamada para fazer as alterações necessárias na estrutura do nó (mods e *back_pointers*).


- ```c++
    Node* remove_node(Node* raiz, int x, int versao)
    ```
    - Remove um valor da árvore. 
    - Os dois primeiros casos, assim como na inserção, são similares a uma árvore binária comum. Encontra-se o nó, e então é chamado o ```write``` para fazer as alterações necessárias.
    - Usam-se chamadas recursivas para encontrar o valor correto, buscando na árvore. Quando o valor é encontrado, 

- ```c++
    int sucessor(Node* raiz, int x, int versao)
    ```
    - Encontra o sucessor de forma recursiva. 
    - Caso não exista sucessor, retorna-se infinito.

- ```c++
    void inorder(Node* node, int versao, int depth)
    ```
    - Faz o percurso em ordem da árvore e imprime os valores dos nós. 