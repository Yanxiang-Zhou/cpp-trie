// Yanxiang Zhou, 903735170, yzhou791@gatech.edu
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdbool.h>
#include <sstream>
#include <cctype>
#include <algorithm>
#include "tutorial/tutorial.h"
const int ALPHANUM_SIZE = 37;

namespace buzzdb {
namespace tutorial {

#define UNUSED(p) ((void)(p))

struct TrieNode *getNode(void)
{
    struct TrieNode *pNode =  new TrieNode;
 
    pNode->isEndOfWord = false;
 
    for (int i = 0; i < ALPHANUM_SIZE; i++)
        pNode->children[i] = NULL;
 
    return pNode;
};
// TextProcessor: load text file and process / insert each word into trie
std::string Text::TextProcessor(std::string filename, struct TrieNode *root)
{
    std::ifstream input_file(filename);
    std::string output;
    if (input_file.is_open()){
        int i = 1;
        std::string word;
        while (input_file >> word) {
            std::string temp = "";
            int size = word.size();
            for (int j = 0; j < size; ++j) {
                char cc = word[j];
                if (std::isalpha(word[j])) {
                    char c = std::tolower(word[j]);
                    temp = temp + c;
                }else if (std::isdigit(word[j]) or cc== '\''){
                    temp = temp + word[j];
                }
                else{
                    if (!temp.empty()){
                        Insert(root, temp, i);
                        i++;
                    }
                    temp = "";           
                }
            }

            if (!temp.empty()){
                Insert(root, temp, i);
                
                i++;
            }
            
        }
        input_file.close();
    }
    else{
        output = "ERROR: Invalid command";
    } 

    return output;   
}

// CommandProcessor: process each word in command and insert into vector
std::vector<std::string> Text::CommandProcessor(std::string command){
    std::vector<std::string> parsed;
    std::stringstream ss(command);
    std::string w;

    while (ss >> w) {
    	std::transform(w.begin(), w.end(), w.begin(),[](unsigned char c){ return std::tolower(c); });
        parsed.push_back(w);
    }

    return parsed;
}

// If not present, inserts key into trie
// If the key is prefix of trie node, mark as leaf node
void Trie::Insert(struct TrieNode *root, std::string key, int pos)
{
    struct TrieNode *p = root;
    int len = key.length();
    for (int i = 0; i < len; i++)
    {
        int index;
        char cc = key[i];
        if (std::isdigit(key[i])){
            index = 26 + key[i] - '0';
        }
        else if (cc == '\''){
            index = 36;
        }
        else{
            index = key[i] - 'a';
        }

        if (!p->children[index])
            p->children[index] = getNode();
 
        p = p->children[index];
    }

    // mark last node as leaf
    p->isEndOfWord = true;
    p->index_list.push_back(pos);
}
 
// Returns location index if key presents in trie, else 0
int Trie::Search(struct TrieNode *root, std::string key, int idx)
{
    struct TrieNode *p = root;
    int len = key.length();
    for (int i = 0; i < len; i++)
    {
        int index;
        char cc = key[i];
        if (std::isdigit(key[i])){
            index = 26 + key[i] - '0';
        }
        else if (cc == '\''){
            index = 36;
        }
        else{
            index = key[i] - 'a';
        }

        if (!p->children[index])
            return 0;
 
        p = p->children[index];
    }
    
    if (p->isEndOfWord)
    {   
        int size = p->index_list.size();
        if (idx <= size)
        {    
            int output = p->index_list.at(idx-1);
            return output;
        }       
    }
    
    
    return 0;
    
}

//Delete the trie recursively
void Trie::Destroy(struct TrieNode *root){
    for (int i = 0; i < ALPHANUM_SIZE; i++)
    {
        if (root->children[i])
            Destroy(root->children[i]); 

        delete root->children[i];
    } 

}

/// Your implementation go here
CommandExecutor::CommandExecutor() {
    root = getNode();
}

CommandExecutor::~CommandExecutor() {
    if (root->children){
        tr.Destroy(root);
    }

    delete root;
}

/// Execute a command
/// Return the appropriate output as a string
std::string CommandExecutor::execute(std::string command) {
    // UNUSED(command);
    
    std::vector<std::string> parsed = te.CommandProcessor(command); 
    std::string output;
    
    if (parsed.size() == 1 && parsed[0] == "new") {
        struct TrieNode *newtrie = root;
        root = getNode();
        tr.Destroy(newtrie);
        delete newtrie;

    } else if ( parsed.size() == 1 && parsed[0] == "end") {
        tr.Destroy(root);

    } else if (parsed.size() == 2 && parsed[0] == "load") {
        std::string filename = parsed[1];
        output = te.TextProcessor(filename, root);

    } else if (parsed.size() == 3 && parsed[0] == "locate") {
        std::string key = parsed[1];
        int idx, flag;
        int syb1 = std::count_if(key.begin(),key.end(),[](char c){ return !(std::isalnum(c)); });
        int syb2 = std::count_if(parsed[2].begin(),parsed[2].end(),[](char c){ return !(std::isalnum(c)); });

        if (!syb2){
            idx = std::stoi(parsed[2]);
            flag = tr.Search(root, key, idx);
        }else{
            idx = -1;
            flag = 0;
        }

        if (flag){
            output = std::to_string(flag);
        }else if (idx < 0 or syb1 > 0){
            output = "ERROR: Invalid command";    
        }
        else {
            output = "No matching entry";    
        }
        
    } else {
        output = "ERROR: Invalid command";

    }

    return output;  
}

}  // namespace tutorial
}  // namespace buzzdb
