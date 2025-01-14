# Домашно 4:

Фокусът на това домашно е върху реализацията на префиксно дърво.

Програмата получава като аргументи от командния ред пътищата на три текстови файла:

* един съдържащ думи, които са част от речник;
* втори, съдържащ думи, които трябва да се премахнат от речника (т.нар. "филтър");
* трети, който съдържа текст, който трябва да се провери спрямо получения речник;

Програмата не прави разлика между големи и малки букви. Например между думите "ABC" и "abc" не се прави разлика.

Програмата:

* Зарежда думите от речника и да ги съхрани в префиксно дърво.
* Премахва от префиксното дърво всички думи съдържащи се във втория файл (филтъра).
* Проверява всички думи в текста спрямо получения речник и извежда на екрана онези, които НЕ СЕ съдържат в речника.
* Извежда на екрана кратка статистика:
  * брой коректни и брой некоректни записи в прочетения речник ;
  * същата статистика се извежда и за файла с думи за премахване;
  * брой думи, които реално са премахнати от речника;
  * брой на думите в получения след премахването речник;
  * брой на коректните и брой на сгрешените думи в текста (коректни са тези думи, които се срещат в получения речник).

За улеснение считаме, че думите в речника се състоят само от латинските букви (от `a` до `z`).

Ако в речника бъде открита некоректна дума (например съдържаща интервал, цифра и т.н.), извежда се съобщение за грешка, като се посочва на кой ред се намира некоректната дума. Програмата след това продължава работа, като некоректната дума се игнорира.

Всяка дума в речника се изписва на отделен ред. Редовете започващи със символа `#` са коментари. Те се прескачат при обработката на тези файлове. Игнорират се празни редове, както и редове, съдържащи само празни символи.

Няма ограничение върху дължината на един ред или обема на целия текст. Думите в него се разделят помежду си с празни (whitespace) символи.

## Примерно изпълнение

Съдържание на речника (`dictionary.txt`):

```text
# This is a comment. Its contents must be skipped.
XYZ
ab
ba

# The next four lines define the same word
aBcD
abcd
ABCD
abcd

# The following entries are incorrect
This is an incorrect entry, it has spaces.
 spaceOnTheLeft
spaceOnTheRight 
an0ther1ncorrectW0rd
```

Файл с думи, които да се премахнат от речника (`filter.txt`):

```text
# This line is a comment and must be ignored
xyz
abcabc

1234567890
```

Съдържание на текста (`text.txt`):

```text
abc ab abbcd abcd xyz
an0ther1ncorrectW0rd
```

Изпълнението може да изглежда например така:

```text
Loading dictionary from .\input\dictionary.txt...
ERROR: incorrect entry "This is an incorrect entry, it has spaces." on line 8
ERROR: incorrect entry " spaceOnTheLeft" on line 9
ERROR: incorrect entry "spaceOnTheRight " on line 10
ERROR: incorrect entry "an0ther1ncorrectW0rd" on line 11

Removing the words listed at .\input\filter.txt...
ERROR: incorrect entry "1234567890" on line 3

Verifying the contents of .\input\text.txt...
SPELLING ERROR: "abc" on line 1
SPELLING ERROR: "abbcd" on line 1
SPELLING ERROR: "xyz" on line 1
SPELLING ERROR: "an0ther1ncorrectW0rd" on line 2

Statistics:
    Dictionary entries: 7 correct, 4 incorrect
        Filter entries: 2 correct, 1 incorrect
  Resultant dictionary: 6
         Words in text: 2 correct, 4 incorrect
```

### Библиотека `dictionarylib`

Изгражда се от файловете `Dictionary.h` и `Dictionary.cpp`.

Съдържа клас `Dictionary`, който представя речник и има като минимум следния интерфейс:

```cpp
class Dictionary {
public:
  void insert(const char* word);
  void erase(const char* word) noexcept;
  bool contains(const char* word) const noexcept;
  size_t size() const noexcept;
  static bool isCorrectWord(const char* word) noexcept;
};
```

Ако `insert` не може да добави думата по някаква причина, методът хвърля изключение. В частност, ако думата не е коректна, хвърля се обект от следния клас:

```cpp
class incorrect_word_exception : public std::runtime_error {
public:
  incorrect_word_exception()
    : runtime_error("incorrect word")
  {
    // Nothing to do here
  }
};
```

Ако на `erase` бъде подадена дума, която не се съдържа в речника, тя не прави никакви промени в него. В частност, ако бъде подаден `nullptr`, не се прави нищо (последното важи и за `contains`).

Вътрешно `Dictionary` трябва да реализира префиксно дърво (trie). Всеки елемент в него може да има най-много 26 наследника (колкото е броят на буквите в азбуката). 

Решението първоначално е имплементирано по следната структура - дърво с 'Z' - 'A' наследника

```cpp
class Node {
    Node* successors['Z'-'A'];
}
```

Съответно, всяка позиция в масива съответства на преход със съответната буква.

## Алтернативно решение
Съществува и отделно решение (в branch "linear-version"), което използва свързан списък на мястото на дървовидната структура, който на всяка "кутия" съдържа число с достатъчен брой битове, по което изчисляваме наличие на букви/наследници чрез битови маски с цел спестяване на памет и евентуално скорост.

```cpp
struct Node {
    uint32_t wordPlacement;
    Node* next;
};
```
