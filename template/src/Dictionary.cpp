#include "Dictionary.h"

#include <cctype> // Contains std::isalpha()
#include <string.h>

void Dictionary::insert(const char* word) {
    if (!word || !isCorrectWord(word)) throw incorrect_word_exception();
    if (!prefixTree) prefixTree = createNode();
    if (contains(word)) return;
    Node* head = prefixTree;
    const size_t length = strlen(word);
    for (int i = 0; i < length; ++i) {
        const unsigned short index = letterIndex(word[i]);
        if (head->successors[index] == nullptr) {
            head->successors[index] = createNode();
        }
        head = head->successors[index];
    }
    head->isWord = true;
    ++numberOfWords;
}

void Dictionary::erase(const char* word) noexcept
{
    if (!word) return;
    if (!prefixTree) return;
    if (!contains(word)) return;
    prefixTree = eraseHelper(prefixTree, word);
}

bool Dictionary::contains(const char* word) const noexcept
{
    if (!word) return false;
    if (!prefixTree) return false;
    Node* head = prefixTree;
    const size_t length = strlen(word);
    for (int i = 0; i < length; ++i) {
        const unsigned short index = letterIndex(word[i]);
        if (head->successors[index] == nullptr) {
            return false;
        }
        head = head->successors[index];
    }
    return head->isWord;
}

size_t Dictionary::size() const noexcept
{
    return numberOfWords;
}

bool Dictionary::isCorrectWord(const char* word) noexcept
{
    if (!word || *word == '\0') return false;
    const size_t length = strlen(word);
    for (int i = 0; i < length; ++i) {
        if (!isalpha(word[i])) {
            return false;
        }
    }
    return true;
}

Dictionary::Dictionary() : numberOfWords(0)
{
    prefixTree = createNode();
}

Dictionary::~Dictionary() {
    clear(prefixTree);
}

Dictionary::Dictionary(const Dictionary& other) : 
    prefixTree(copy(other.prefixTree)), 
    numberOfWords(other.numberOfWords)
{
}

Dictionary& Dictionary::operator=(const Dictionary& other) {
    if (&other != this) {
        clear(prefixTree);
        prefixTree = copy(other.prefixTree);
        numberOfWords = other.numberOfWords;
    }
    return *this;
}

Dictionary::Node* Dictionary::eraseHelper(Node *root, const char* word)
{
    if (!root) return root;
    if (*word == '\0') {
        if (root->isWord) {
            root->isWord = false;
            --numberOfWords;
            if (!hasSuccessors(root)) {
                delete root;
                root = nullptr;
            }
        }
        return root;
    }
    root->successors[letterIndex(*word)] = eraseHelper(root->successors[letterIndex(*word)], word + 1);
    if (!hasSuccessors(root) && !root->isWord) {
        delete root;
        root = nullptr;
    }
    return root;
}

Dictionary::Node* Dictionary::createNode() {
    Node* newNode = new Node();
    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        newNode->successors[i] = nullptr;
    }
    newNode->isWord = false;
    return newNode;
}

Dictionary::Node* Dictionary::copy(Node* node)
{
    Node* newNode = createNode();
    newNode->isWord = node->isWord;
    for (size_t i = 0; i < ALPHABET_SIZE; i++)
    {
        if (!node) return node;
        newNode->successors[i] = copy(node->successors[i]);
    }
    return newNode;
}

void Dictionary::clear(Node *root) {
    if (!root) return;
    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        clear(root->successors[i]);
    }
    delete root;
}

bool Dictionary::hasSuccessors(Node *node) {
    if (!node) return false;
    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        if (node->successors[i]) return true;
    }
    return false;
}

// it is ensured that the character is a latin letter
unsigned short Dictionary::letterIndex(const char letter) const {
    return (unsigned short)(letter >= 'A' && letter <= 'Z' ? letter - 'A': letter - 'a');
}
