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
        /**
         * @brief Construct a new Node object
         * 
         * @param letter Key of the Node
         */
        Node(char letter);
        /**
         * @brief Verify if any of tge sons of this node has the desired key
         * 
         * @param letter Key of the Node
         * @return Node* Pointer to the Node
         */
        Node* findNode(char letter);
        /**
         * @brief Get the Code object
         * 
         * @return unsigned int Code of the Node
         */
        inline unsigned int getCode(){return this->code;}
        /**
         * @brief Get the Counter object
         * 
         * @return unsigned int Number of Nodes created
         */
        static inline unsigned int getCounter(){return counter;}
        /**
         * @brief Create a new node with the key passed and make it this node's son
         * 
         * @param letter Key of the new node
         * @return true Operation succesful
         * @return false Operation failed
         */
        bool insertNode(char letter);
        /**
         * @brief Destroy the Node object
         * 
         */
        ~Node();
};

#endif