#ifndef TRIE_H_
#define TRIE_H_

#include <map>
#include <vector>
#include <string>

class Node{
    private:
        static unsigned int counter;
        unsigned int code;
        char val;
        std::vector<Node*> sons;
    public:
        Node(char letter);
        Node* findNode(char letter);
        inline unsigned int getCode(){return this->code;}
        static inline unsigned int getCounter(){return counter;}
        bool insertNode(char letter);
        ~Node();
};

#endif