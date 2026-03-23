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

    Node<T>* deleteNodeRec(Node<T>* current, T value){

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
            Node<T>* sucessor = findMinRec(current->min);
            current->data = sucessor->data;
            current->right = deleteNodeRec(current->right, sucessor->data);
        }

        else{
            current->left = deleteNodeRec(current->left, value);
            current->right = deleteNodeRec(current->right, value)
        }
    }

    // retorna minimo da arvore
    Node<T>* findMinRec(Node <T>* node){
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
    void inOrderRec(Node<T>* node){
        if (node != nullptr){
            inorder(node->left);
            cout << node->data << " ";
            inorder(node->right);
        }
    }

    void preOrderRec(Node<T>* node){
        if (node != nullptr){
            cout << node->data << " ";
            preOrderRec(node->left);
            preOrderRec(node->right);
        }
    }

    void postOrderRec(Node<T>* node){
        if (node != nullptr){
            postOrderRec(node->left);
            postOrderRec(node->right);
            cout << node->data << " ";
        }
    }
public:
    BinaryTree() : root(nullptr){}

    void insertNode(T value){
        Node<T>* newNode = new Node<T>(value);
        
        // se a arvore ta vazia 
        if (root == nullptr){
            root =newNode;
            return
        }

        queue<Node<T>*> q;
        q.push(root);

        while (!q.empty()){
            Node<T>* current = q.front();
            q.pop();

            if (current->left == nullptr){
                current->left = newNode;
                return
            }
            else{
                q.push(current->left);
            }

            if(current->right == nullptr){
                current->right = newNode;
                return
            }
            else{
                q.push(current->right);
            }
        }
    }

    void deleteNode(T value){
        root = deleteNodeRec(root, value);
    }

    void search(T value){
        return searchRecursive(root, value);
    }

    void preOder(){
        preOrderRec(root);
        cout << endl;
    }

    void postOder(){
        postOrderRec(root);
        cout << endl;
    }
};