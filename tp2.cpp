#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

const int ALPHABET_SIZE = 256;

class TrieNode {
    public:
        int index;
        int parentIndex;
        char parentChar;
        TrieNode* children[ALPHABET_SIZE];

        TrieNode(int idx = -1, int pIndex = -1, char pChar = '\0') {
            index = idx;
            parentIndex = pIndex;
            parentChar = pChar;
            for (int i = 0; i < ALPHABET_SIZE; i++) {
                children[i] = nullptr;
            }
        }
};


class DictionaryTrie {
    public:
        TrieNode* root;
        int currentIndex;
        //vector<pair<int, char>> lz78_compress(const string& text);
        //string lz78_decompress(const vector<pair<int, char>>& compressed);

        DictionaryTrie() {
            root = new TrieNode();
            currentIndex = 1;
        }

        void insert(int parentIndex, char parentChar) {
            TrieNode* parentNode = search(root, parentIndex);
            if (parentNode) {
                TrieNode* newNode = new TrieNode(currentIndex, parentIndex, parentChar);
                parentNode->children[parentChar] = newNode;
                currentIndex++;
            }
        }


       vector<pair<int, char>> lz78_compress(const string& text) {
            vector<pair<int, char>> compressed;
            TrieNode* current = root;
            int parentIndex = 0;

            for (char c : text) {
                if (current->children[c] == nullptr) {
                    compressed.push_back(make_pair(parentIndex, c));
                    insert(parentIndex, c);
                    current = root;
                    parentIndex = 0;
                } else {
                    current = current->children[c];
                    parentIndex = current->index;
                }
            }

            if (parentIndex != 0) {
                compressed.push_back(make_pair(parentIndex, '\0'));
            }

            return compressed;
        }

        string lz78_decompress(const vector<pair<int, char>>& compressed) {
            string decompressed;

            for (const auto& entry : compressed) {
                int parentIndex = entry.first;
                char parentChar = entry.second;
                string word;

                while (parentIndex != 0) {
                    word = root->children[parentChar]->parentChar + word;
                    parentIndex = root->children[parentChar]->parentIndex;
                    parentChar = root->children[parentChar]->parentChar;
                }

                decompressed += word;
            }

            return decompressed;
        }

    private:
        TrieNode* search(TrieNode* node, int index) {
            if (node == nullptr || node->index == index) {
                return node;
            }

            TrieNode* foundNode = nullptr;
            for (int i = 0; i < ALPHABET_SIZE && foundNode == nullptr; i++) {
                foundNode = search(node->children[i], index);
            }

            return foundNode;
        }

};


void write_file(const string &file_name, const string &content) {
    ofstream file(file_name);
    if (!file.is_open()) {
        cerr << "Error while opening file: " << file_name << endl;
        exit(1);
    }
    file << content;
    file.close();
} // end of write_file method


string read_file(const string &file_name) {
    ifstream file(file_name);
    if (!file.is_open()) {
        cerr << "Erro ao abrir arquivo: " << file_name << endl;
        exit(1);
    }
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
} // fim do metodo read_file


int main(int argc, char* argv[]) {

    if (argc < 3) {
        
        cerr << "Modo correto: " << argv[0] << " [-c|-x] <arquivoEntrada> [-o <arquivoSaida>]" << endl;
    
    } else {

        string operation_flag = argv[1];
        string input_file = argv[2];
        string output_file;

        
                        // Set the default output file name based on the operation
                        if (operation_flag == "-c") {
                            output_file = input_file + ".z78";
                        } else if (operation_flag == "-x") {
                            size_t last_dot = input_file.find_last_of('.');
                            if (last_dot != string::npos) {
                                output_file = input_file.substr(0, last_dot) + ".txt";
                            } else {
                                output_file = input_file + ".txt";
                            }
                        }

                        string file_content = read_file(input_file);
                        cout<<file_content<<endl;

                        DictionaryTrie dicTrie;
                        vector<pair<int, char>> compressed = dicTrie.lz78_compress(file_content);
                        
                        string decompressed = dicTrie.lz78_decompress(compressed);


                        cout<<decompressed<<endl;
                        // Do the compression or decompression here, using file_content and output_file.

        if (operation_flag != "-c" && operation_flag != "-x") {
            
            cerr << "Flag invalida. Utilize -c para comprimir ou -x para descomprimir." << endl;
        
        } else {

                if (argc > 3) {
        
                    string output_flag = argv[3];
    

                    if (output_flag == "-o" && argc > 4) {
    
                        output_file = argv[4];

                    } else {
                
                        cerr << "Flag de output invalida. Utiliza -o seguido do nome do arquivo de saida." << endl;
        
                    }

                } 

        }

    }

    return 0;

} // fim da main
