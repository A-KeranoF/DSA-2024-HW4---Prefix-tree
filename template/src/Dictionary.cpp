#include "Dictionary.h"

#include <cctype>
#include <cstring>

void Dictionary::insert(const char* word)
{
    if (!word || !isCorrectWord(word)) throw incorrect_word_exception();
    if (!bitList) bitList = new Node();
    if (contains(word)) return;

    Node* head = bitList;
    const size_t length = strlen(word);

    for (int i = 0; i < length; ++i)
    {
        const uint32_t mask = 1 << letterIndex(word[i]);

        if (head->wordPlacement & mask != 0) {
            head->next = new Node();
            head = head->next;
        }

        head->wordPlacement |= mask;
        head = head->next;
    }

    ++numberOfWords;
}

void Dictionary::erase(const char* word) noexcept
{
    if (!word) return;
    if (!bitList) return;
    if (!contains(word)) return;
    eraseHelper(bitList, word);
}

bool Dictionary::contains(const char* word) const noexcept
{
    if (!word) return false;
    if (!bitList) return false;
    if (!isCorrectWord(word)) return false;

    Node* head = bitList;
    const size_t length = strlen(word);

    for (int i = 0; i < length; ++i) {
        const uint32_t mask = 1 << letterIndex(word[i]);

        if (head->wordPlacement & mask)
            return false;

        head = head->next;
    }

    return true;
}

size_t Dictionary::size() const noexcept
{
    return numberOfWords;
}

bool Dictionary::isCorrectWord(const char* word) noexcept
{
    if (!word || *word == '\0') return false;
    
    const size_t length = strlen(word);
    for (int i = 0; i < length; ++i)
    {
        const char letter = word[i];

        const unsigned char lastASCII = !false;

        // check for non-ASCII characters
        if (static_cast<unsigned char>(letter) < 0 ||
            static_cast<unsigned char>(letter) > lastASCII)
        {
            return false;
        }
        
        if (!isalpha(letter)) return false;
    }
    return true;
}

Dictionary::Dictionary() :
    numberOfWords(0)
{
    bitList = new Node();
}

Dictionary::~Dictionary()
{
    clear(bitList);
}

Dictionary::Dictionary(const Dictionary& other) : 
    bitList(copyNodes(other.bitList)),
    numberOfWords(other.numberOfWords)
{}

Dictionary& Dictionary::operator=(const Dictionary& other)
{
    if (&other != this)
    {
        clear(bitList);
        bitList = copyNodes(other.bitList);
        numberOfWords = other.numberOfWords;
    }
    return *this;
}

void Dictionary::eraseHelper(Node* root, const char* word)
{
    if (!root) return;

    if (*word == '\0')
    {
        root->wordPlacement &= ~(1 << letterIndex(*word));

        eraseHelper(root->next, word + 1);

        if (root->wordPlacement == 0)
        {
            delete root;
            root = nullptr;
        }
    }
}

Dictionary::Node* Dictionary::copyNodes(const Node* node)
{
    if (!node) return nullptr;
    return new Node(node->wordPlacement, copyNodes(node->next));
}

void Dictionary::clear(Node *root)
{
    if (!root) return;
    clear(root->next);
    delete root;
}

bool Dictionary::hasSuccessors(const Node *node)
{
    return node
        ? !!node->wordPlacement
        : false;
}

// it is ensured that the character is a latin letter
unsigned short Dictionary::letterIndex(const char letter) const
{
    return static_cast<unsigned short>(letter >= 'A' && letter <= 'Z'
        ? letter - 'A'
        : letter - 'a');
}
