#pragma once

#include <cstdint>
#include <stdexcept>
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
    [[nodiscard]] size_t size() const noexcept;
    static bool isCorrectWord(const char* word) noexcept;

public:
    Dictionary();
    ~Dictionary();
    Dictionary(const Dictionary& other);
    Dictionary& operator=(const Dictionary& other);

private:
    struct Node {
        uint32_t wordPlacement;
        Node* next;
        explicit Node(const uint32_t data = 0, Node* next = nullptr) : wordPlacement(data), next(next) {};
    };

    Node* bitList;
    size_t numberOfWords;

private:
    void eraseHelper(Node* root, const char* word);
    Node* copyNodes(const Node* node);
    void clear(Node* root);
    bool hasSuccessors(const Node* node);
    [[nodiscard]] unsigned short letterIndex(char letter) const;
};
