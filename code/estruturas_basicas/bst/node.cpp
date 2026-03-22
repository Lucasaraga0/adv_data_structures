#include <iostream>
#include <queue>
using namespace std;

template <typename T>
class Node {
    public:
    T data;
    Node* left;
    Node* right;

    // Construtor para inicializar o no com um valor
    Node(T value) : data(value), left(nullptr), right(nullptr) {}
};

template <typename T>
class BinaryTree{
private:
    Node <T>* root; // ponteiro para a raiz

    // deletar um no 

    Node<T>* deleteNode(Node<T>* current, T value){

        if (current == nullptr) return nullptr;

        if (current->data == value){
            
            // caso seja folha                
            if (current->left == nullptr && current->right == nullptr){
                delete current; 
                return nullptr;
            }
            // se somente o no da direita nao for nulo
            if (current->left == nullptr){
                Node<T>* temp = current->right;
                delete current;
                return temp;
            }
            // se somente o no da esquerda nao for nulo
            if (current->right == nullptr){
                Node<T>* temp = current->left;
                delete current;
                return temp;
            }
            // se nos direito e esquerdo nao forem nulo
            Node<T>* sucessor = findMin(current->min);
            current->data = sucessor->data;
            current->right = deleteNode(current->right, sucessor->data);
        }

        else{
            current->left = deleteNode(current->left, value);
            current->right = deleteNode(current->right, value)
        }
    }

    // retorna minimo da arvore
    Node<T>* findMin(Node <T>* node){
        while (node->left != nullptr) node = node->left;
        return node;
    }

    // procura um valor na arvore e retorna se ele esta la 
    bool searchRecursive(Node <T>* current, T value){
        if (current == nullptr) return false;
        if(current->data == value) return true;
        return searchRecursive(current->left, value) || searchRecursive(current->right, value);
    }

    // percurso em ordem
    void inOrder(Node<T>* node){
        if (node != nullptr){
            inorder(node->left);
            cout << node->data << " ";
            inorder(node->right);
        }
    }

    void preOrder(Node<T>* node){
        if (node != nullptr){
            cout << node->data << " ";
            preOrder(node->left);
            preOrder(node->right);
        }
    }

    void postOrder(Node<T>* node){
        if (node != nullptr){
            postOrder(node->left);
            postOrder(node->right);
            cout << node->data << " ";
        }
    }
public:
    BinaryTree() : root(nullptr){}

    void insertNode(T value){
    }
};