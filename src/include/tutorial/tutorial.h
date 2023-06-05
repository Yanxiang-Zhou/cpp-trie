#pragma once
// Yanxiang Zhou, 903735170, yzhou791@gatech.edu
#include <string>
#include <stdbool.h>
#include <vector>

namespace buzzdb {
namespace tutorial {

const int ALPHANUM_SIZE = 37;
// trie node
struct TrieNode
{
    struct TrieNode *children[ALPHANUM_SIZE];
 
    // isEndOfWord is true if the node represents
    // end of a word
    bool isEndOfWord;
    std::vector<int> index_list;
};
 
// Returns new trie node (initialized to NULLs)
struct TrieNode *getNode(void);

class Trie {
  public:
    
    void Insert(struct TrieNode *root, std::string key, int pos);
    int Search(struct TrieNode *root, std::string key, int idx);
    void Destroy(struct TrieNode *root);
};

class Text: public Trie {  
  public:
    std::string TextProcessor(std::string filename, struct TrieNode *root);
    std::vector<std::string> CommandProcessor(std::string command);
};

class CommandExecutor {
  public:
    CommandExecutor();
    struct TrieNode *root;
    /// Execute a command
    /// Return the appropriate output as a string
    std::string execute(std::string);
    ~CommandExecutor();
  private:
    Trie tr;
    Text te;  
};

}  // namespace tutorial
}  // namespace buzzdb
