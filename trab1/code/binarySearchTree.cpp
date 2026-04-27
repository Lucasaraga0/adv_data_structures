#include <iostream>
using namespace std;

struct Node{
    int value;
    Node* left;
    Node* right;
};

Node* createNode(int value){
    Node* newNode = new Node();
    newNode->value = value;
    newNode->left = newNode->right = nullptr;
    return newNode;
};

Node* insertNode(Node* root, int value){
    //  se a arvore esta vazia
    if (root == nullptr){
        return createNode(value);
    }

    if (value < root->value){
        root->left = insertNode(root->left, value);
    }

    else if (value> root->value){
        root->right = insertNode(root->right, value);
    }

    return root;
}


void inOrder(Node* root){
    if (root != nullptr){
        inOrder(root->left);
        cout << root->value << " ";
        inOrder(root->right);
    }
}

Node* searchNode(Node* root, int key){
    if (root == nullptr|| root->value == key){
        return root;
    }

    if (root->value < key){
        return searchNode(root->right, key);
    }

    return searchNode(root->left, key);
}

Node* minValueNode(Node* node){
    Node* current = node;
    while (current && current->left != nullptr){
        current = current->left;
    }
    return current;
}
Node* getSucessor(Node* root, int value){
    Node* current = root;
    
    
    // se o valor passado eh maior que o no atual, procura na arvore direita
    if (current->value <= value && current->right != nullptr){
        current = getSucessor(current->right, value);
    }
    
    // se o valor passado for menor que o no atual, procura na chave esquerda
    else if (current->value > value && current->left != nullptr){
        current = getSucessor(current->left, value);
    }
    
    
    if (current-> value < root->value && current->value > value){
        return current;
    }
    else if (current -> value> root->value && root->value > value){
        return root;
    }

    return root;
}

Node* deleteNode(Node* root, int value){
    if (root == nullptr)
        return root;
    
    if (value< root->value){
        root->left = deleteNode(root->left, value);
    }

    else if (value> root->value){
        root->right = deleteNode(root->right, value);
    }

    else{
        if (root->left == nullptr){
            Node* temp = root->right;
            delete root;
            return temp;
        }
        else if (root->right == nullptr){
            Node* temp = root->left;
            delete root;
            return temp;
        }
    Node* temp = minValueNode(root->right);
    root->value = temp->value;
    root->right = deleteNode(root->right, temp->value);
    }
    return root;
}