#include "structs.cpp"
#include <bits/stdc++.h>
#include "operations.cpp"
#include <iostream>
#include <vector>
using namespace std;

const int MAX_VERSIONS = 100;
const int MAX_MODS = 4; 


int main(int argc, char* argv[]){
    if (argc < 2){
        return 1;
    }

    ifstream file(argv[1]);
    string op;
    
    Node* raizes[MAX_VERSIONS];
    int versao_atual = 0;   
    
    while (file >> op){
        if (op == "INC"){
            int x; file >> x;
            versao_atual++;
            raizes[versao_atual] = insert(raizes[versao_atual-1], x, versao_atual);
        }
        else if (op == "REM"){
            int x; file >> x;
            versao_atual++;
            raizes[versao_atual] = remove_node(raizes[versao_atual-1], x, versao_atual);   
        }
        else if (op == "SUC"){
            int x,versao; file >> x>> versao;

            if (versao> versao_atual){
                versao = versao_atual;
            }

            cout << "SUC" << x << " " << versao << "\n";
            int suc = sucessor(raizes[versao], x, versao);
            if (suc == INT_MAX){
                cout << "inf" << "\n"; 
            }
            else{
                cout << suc << "\n"; 
            }
        }

        else if (op == "IMP"){
            int versao; file >> versao;
            
            if(versao > versao_atual){
                versao = versao_atual;
            }
            cout << "IMP" << versao << "\n";

            inorder(raizes[versao], versao, 0);
            cout << "\n";
        }
    }
    return 0;
}