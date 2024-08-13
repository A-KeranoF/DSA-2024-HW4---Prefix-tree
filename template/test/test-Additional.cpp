#include "catch2/catch_all.hpp"
#include "Dictionary.h"

TEST_CASE("Dictionary::size() tracks the number of words after insert and erase")
{
    Dictionary dict;
    dict.insert("hello");
    dict.insert("world");
	dict.erase("hello");
    REQUIRE(dict.size() == 1);
}

TEST_CASE("Dictionary copy constructor copies words from the original to a new dictionary")
{
    Dictionary dict;
    dict.insert("hello");
    dict.insert("world");
	Dictionary newDict(dict);
	REQUIRE(newDict.contains("hello"));
	REQUIRE(newDict.contains("world"));
    REQUIRE(newDict.size() == 2);
}

TEST_CASE("Dictionary::operator= copies words from the original to a empty dictionary")
{
    Dictionary dict;
    dict.insert("hello");
    dict.insert("world");
	Dictionary newDict = dict;
	REQUIRE(newDict.contains("hello"));
	REQUIRE(newDict.contains("world"));
    REQUIRE(newDict.size() == 2);
    REQUIRE(newDict.size() == dict.size());
}

TEST_CASE("Dictionary::operator= assignes words to a non-empty dictionary")
{
    Dictionary dict1;
    Dictionary dict2;
    dict1.insert("hello");
    dict2.insert("STAR");
    dict2.insert("PLATINUM");
    dict1 = dict2;
    REQUIRE_FALSE(dict1.contains("hello"));
    REQUIRE_FALSE(dict1.size() == 1);
    REQUIRE(dict1.size() == 2);
    REQUIRE(dict1.contains("STAR"));
    REQUIRE(dict1.contains("PLATINUM"));
	REQUIRE(dict1.size() == dict2.size());
}

TEST_CASE("Dictionary::operator= copies words from the original to a new dictionary")
{
    Dictionary dict1;
    dict1.insert("hello");
    dict1.insert("world");
	Dictionary dict2 = dict1;
	REQUIRE(dict2.contains("hello"));
	REQUIRE(dict2.contains("world"));
    REQUIRE(dict2.size() == 2);
	REQUIRE(dict2.size() == dict1.size());
}

TEST_CASE("Dictionary::operator= handles self-assignment")
{
	Dictionary dict;
    dict.insert("hello");
    dict.insert("world");
    dict = dict;
    REQUIRE(dict.size() == 2);
    REQUIRE(dict.contains("hello"));
    REQUIRE(dict.contains("world"));
}

TEST_CASE("Dictionary::operator= handles chain assignment") 
{
	Dictionary dict1;
    dict1.insert("hello");
    dict1.insert("world");
    Dictionary dict2;
    Dictionary dict3;
    dict3 = dict2 = dict1;
    REQUIRE(dict1.size() == 2);
    REQUIRE(dict2.size() == 2);
    REQUIRE(dict3.size() == 2);
    REQUIRE(dict1.contains("hello"));
    REQUIRE(dict1.contains("world"));
    REQUIRE(dict2.contains("hello"));
    REQUIRE(dict2.contains("world"));
    REQUIRE(dict3.contains("hello"));
    REQUIRE(dict3.contains("world"));
}

TEST_CASE("Dictionary::insert() inserts a word with different cases only once")
{
	Dictionary dictionary;
	dictionary.insert("abcde");
	dictionary.insert("Abcde");
	dictionary.insert("AbCdE");
	dictionary.insert("aBcDe");
	dictionary.insert("abcde");
	dictionary.insert("ABCDE");
	REQUIRE(dictionary.size() == 1);
}

TEST_CASE("Dictionary::insert() inserts a word with duplicates only once")
{
	Dictionary dictionary;
	dictionary.insert("hello");
	dictionary.insert("hello");
	dictionary.insert("hello");
	dictionary.insert("world");
	dictionary.insert("world");
	dictionary.erase("hello");
	REQUIRE_FALSE(dictionary.contains("hello"));
	REQUIRE(dictionary.size() == 1);
}

TEST_CASE("Dictionary operations handles relatively long words")
{
	Dictionary dictionary;
	dictionary.insert("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
	dictionary.insert("AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz");
	dictionary.insert("ZYXWVUTSRQPONMLKJIHGFEDCBAZYXWVUTSRQPONMLKJIHGFEDCBA");
	REQUIRE(dictionary.contains("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"));
	REQUIRE(dictionary.contains("AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz"));
	REQUIRE(dictionary.contains("ZYXWVUTSRQPONMLKJIHGFEDCBAZYXWVUTSRQPONMLKJIHGFEDCBA"));
	REQUIRE(dictionary.size() == 3);
	
	dictionary.erase("AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz");
	REQUIRE_FALSE(dictionary.contains("AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz"));
	REQUIRE_FALSE(dictionary.size() == 3);
	
	dictionary.insert("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZAaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz");
	REQUIRE(dictionary.contains("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZAaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz"));
	REQUIRE(dictionary.size() == 3);
	
	dictionary.erase("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
	dictionary.erase("ZYXWVUTSRQPONMLKJIHGFEDCBAZYXWVUTSRQPONMLKJIHGFEDCBA");
	dictionary.erase("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZAaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz");
	REQUIRE(dictionary.size() == 0);
	REQUIRE_FALSE(dictionary.contains("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"));
	REQUIRE_FALSE(dictionary.contains("ZYXWVUTSRQPONMLKJIHGFEDCBAZYXWVUTSRQPONMLKJIHGFEDCBA"));
	REQUIRE_FALSE(dictionary.contains("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZAaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz"));
}

TEST_CASE("Dictionary::isCorrectWord() returns false for non-latin/non-ASCII characters in word")
{
	REQUIRE_FALSE(Dictionary::isCorrectWord("непротивоконституционствувателствувайте"));
	REQUIRE_FALSE(Dictionary::isCorrectWord("このジョルノ・ジョバーナには夢がある"));
}

TEST_CASE("Dictionary::contains() returns false for non-latin/non-ASCII characters in word")
{
	Dictionary dict;
	REQUIRE_FALSE(dict.contains("непротивоконституционствувателствувайте"));
	REQUIRE_FALSE(dict.contains("このジョルノ・ジョバーナには夢がある"));
}

TEST_CASE("Dictionary::insert() throws an exception for non-latin/non-ASCII characters in word")
{
	Dictionary dict;
	CHECK_THROWS_AS(dict.insert("непротивоконституционствувателствувайте"), incorrect_word_exception);
	CHECK_THROWS_AS(dict.insert("このジョルノ・ジョバーナには夢がある"), incorrect_word_exception);
	
	REQUIRE_FALSE(dict.contains("непротивоконституционствувателствувайте"));
	REQUIRE_FALSE(dict.contains("このジョルノ・ジョバーナには夢がある"));
	REQUIRE(dict.size() == 0);
}