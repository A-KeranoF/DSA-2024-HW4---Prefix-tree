#pragma once

#include <stdexcept>
#include <unordered_set>
#include <iostream>

class incorrect_word_exception : public std::runtime_error {
public:
      incorrect_word_exception()
        : runtime_error("incorrect word")
      {
          // Nothing to do here
      }
};

class Dictionary {
public:
    void insert(const char* word);
    void erase(const char* word) noexcept;
    bool contains(const char* word) const noexcept;
    size_t size() const noexcept;
    static bool isCorrectWord(const char* word) noexcept;

public:
    Dictionary();
    ~Dictionary();
    Dictionary(const Dictionary& other);
    Dictionary& operator=(const Dictionary& other);

private:
    const static int ALPHABET_SIZE = 'Z' - 'A' + 1; 
    struct Node {
        bool isWord;
        Node* successors[ALPHABET_SIZE];
    };

    Node* prefixTree;
    size_t numberOfWords;

private:
    Node* eraseHelper(Node* root, const char* word);
    Node* createNode();
    Node* copy(Node* node);
    void clear(Node* root);
    bool hasSuccessors(Node* node);
    unsigned short letterIndex(const char letter) const;
};
