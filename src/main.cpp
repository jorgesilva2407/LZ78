#include <iostream>
#include <fstream>
#include "trie.hpp"
#include <vector>
#include <utility>
#include <string>

#define BYTESIZE 8

struct FAILED_TO_OPEN_FILE{
    std::string file;
    FAILED_TO_OPEN_FILE(std::string file): file(file){}
};

struct FAILED_TO_PARSE_PARAMETERS
{
    std::string error;
    FAILED_TO_PARSE_PARAMETERS(std::string error): error(error){}
};

/**
 * @brief Return the string associated with a code
 * 
 * @param vals vector of strings
 * @param number code to be searched
 * @return std::string 
 */
std::string get_string(std::vector< std::pair<unsigned int, std::string> > vals, int number){
    if(number == 0){
        return "";
    } else {
        return get_string(vals, vals[number].first) + vals[number].second;
    }
}

/**
 * @brief Get the number of bytes necessary to respresent a certain integer
 * 
 * @return char 1, 2, 3 or 4 bytes
 */
char get_num_bytes(){
    if (Node::getCounter() > (1 << 3*BYTESIZE)) return 4;
    else if (Node::getCounter() > (1 << 2*BYTESIZE)) return 3;
    else if (Node::getCounter() > (1 << BYTESIZE)) return 2;
    else return 1;
}

/**
 * @brief Write the 'num_bytes' least signifficant bytes of an integer into the given file
 * 
 * @param num The number to be written
 * @param num_bytes The number of bytes to be used
 * @param ofile The file where it will be written
 */
void write_bytes(const unsigned int num, const char num_bytes, std::ofstream& ofile){
    char bytes[4];
    bytes[3] = (num >> 3*BYTESIZE) & 0xFF;
    bytes[2] = (num >> 2*BYTESIZE) & 0xFF;
    bytes[1] = (num >> 1*BYTESIZE) & 0xFF;
    bytes[0] = num & 0xFF;

    for(int i=0; i < num_bytes; i++){
        ofile.write(&bytes[i],sizeof(bytes[i]));
    }
}

/**
 * @brief Read a certain number of bytes and convert it into an unsigned integer
 * 
 * @param num_bytes Number of bytes to be read
 * @param ifile Where to read from
 * @return unsigned int The integer generated
 */
unsigned int read_bytes(const char num_bytes, std::ifstream& ifile){
    char bytes[4] = {0};
    unsigned int number = 0;
    for(int i=0; i < num_bytes; i++){
        ifile.get(bytes[i]);
        number |= (bytes[i] & 0xFF) << i*BYTESIZE;
    }
    return number;
}

/**
 * @brief Encode the given file a compressed one
 * 
 * @param in_file Name of the file that will be encoded
 * @param out_file Name of the file where the compression will be stored
 * @return std::string 
 */
std::string encode(std::string in_file, std::string out_file){
    std::ifstream ifile(in_file, std::ios::binary);
    std::vector< std::pair<unsigned int, char> > encoded_text;
    Node* root = new Node('\0');
    char c;
    Node* current = root;
    if(ifile.is_open()){
        while(ifile.get(c)){
            Node* next = current->findNode(c);
            if(next == NULL){
                encoded_text.push_back(std::pair<unsigned int, char>(current->getCode(), c));
                current->insertNode(c);
                current = root;
            } else {
                current = next;
            }
        }
        ifile.close();
    } else {
        throw FAILED_TO_OPEN_FILE(in_file);
    }
    std::ofstream ofile(out_file, std::ios::out | std::ios::binary);

    if(ofile.is_open()){
        char num_bytes = get_num_bytes();
        ofile.write(&num_bytes, sizeof(num_bytes));
        for(unsigned int i=0; i < encoded_text.size(); i++){
            write_bytes(encoded_text[i].first, num_bytes, ofile);
            ofile.write(&encoded_text[i].second, sizeof(encoded_text[i].second));
        }
        unsigned int last = current->getCode();
        write_bytes(last, num_bytes, ofile);
        ofile.close();
    } else {
        throw FAILED_TO_OPEN_FILE(out_file);
    }
    delete root;
    return out_file;
}

/**
 * @brief Decompress the given file
 * 
 * @param in_file Name of the compressed file
 * @param out_file Name of the file where the content will be decompressed to
 * @return std::string 
 */
std::string decode(std::string in_file, std::string out_file){
    std::ifstream ifile(in_file, std::ios::binary);
    std::ofstream ofile(out_file, std::ios::out | std::ios::binary);
    if(ifile.is_open() && ofile.is_open()){
        std::vector<std::string> vals;
        vals.push_back("");
        char c;
        char num_bytes;
        ifile.get(num_bytes);
        while(true){
            unsigned int number = read_bytes(num_bytes, ifile);

            std::string prefix = vals[number];
            ofile.write(prefix.c_str(), prefix.size());

            if(!ifile.get(c)){
                break;
            } else {
                vals.push_back(vals[number]+c);
                ofile.write(&c, sizeof(c));
            }
        }
    } else if (!ifile.is_open()){
        throw FAILED_TO_OPEN_FILE(in_file);
    } else {
        throw FAILED_TO_OPEN_FILE(out_file);
    }
    return out_file;
}

int main(int argc, char** argv){
    try{
        bool to_encode = false;
        bool to_decode = false;
        std::string in_file;
        std::string out_file = "";
        for(int i=1; i < argc; i++){
            if(std::string(argv[i]) == "-c"){
                i++;
                to_encode = true;
                in_file = argv[i];
            } else if (std::string(argv[i]) == "-x"){
                i++;
                to_decode = true;
                in_file = argv[i];
            } else if (std::string(argv[i]) == "-o"){
                i++;
                out_file = argv[i];
            }
        }

        if(to_encode && to_decode){
            throw FAILED_TO_PARSE_PARAMETERS("Too many parameters");
        } else if (!to_decode && !to_encode) {
            throw FAILED_TO_PARSE_PARAMETERS("No operation to be done");
        }

        if(out_file == ""){
            out_file = in_file.substr(0, in_file.length()-3);
            out_file += to_encode ? ("z78") : ("txt");
        }

        if(to_encode){
            std::cout << encode(in_file, out_file) << std::endl;
        } else if(to_decode){
            std::cout << decode(in_file, out_file) << std::endl;
        }
        
    } catch(const FAILED_TO_OPEN_FILE& error){
        std::cout << "Failed to open file: " << error.file << std::endl;
        exit(1);
    } catch(const FAILED_TO_PARSE_PARAMETERS& error){
        std::cout << "Failed to parse parameteres: " << error.error << std::endl;
        exit(1);
    }

    return 0;
}