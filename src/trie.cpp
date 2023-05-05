#include "trie.hpp"

#define FAILED false
#define SUCCESS true

unsigned int Node::counter = 0;

Node::Node(char letter){
    this->val = letter;
    this->code = Node::counter;
    Node::counter++;
}

Node* Node::findNode(char letter){
    for(unsigned int i=0; i < this->sons.size(); i++){
        if(this->sons[i]->val == letter){
            return this->sons[i];
        }
    }
    return NULL;
}

bool Node::insertNode(char letter){
    Node* new_node = new Node(letter);
    this->sons.push_back(new_node);
    return (new_node == NULL) ? FAILED : SUCCESS;
}

Node::~Node(){
    for(unsigned int i=0; i < this->sons.size(); i++){
        delete this->sons[i];
    }
}