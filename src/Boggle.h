/*
 * Header file for Boggle class.
 *
 * Boggle class includes public methods which allow
 * the client to instantiate a boggle game with a
 * chosen board, handle and fullfill requests for user
 * data such as scores and words found, and print out
 * humanly readable representations of the boggle game.
 *
 * Internally, the Boggle class maintains private data
 * structures for storing data about the board and words
 * found by the user or computer (in order to avoid
 * representational exposure), has a Boggle specific
 * struct to handle grid coordinate storage and comparison,
 * as well as methods to implement recursive word searches.
 */

#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <string>
#include "strlib.h"
#include "lexicon.h"
#include "grid.h"
#include "random.h"
#include "vector.h"
#include "set.h"
#include "bogglegui.h"

using namespace std;

//CONSTANTS
static const int BOARD_DIM = 4;
static const int FACE_COUNT = 6;
static const int MIN_WORD_LENGTH = 4;

class Boggle {
public:
    Boggle(Lexicon& dictionary, string boardText = "");
    char getLetter(int row, int col);
    bool humanWordSearch(string word);
    Set<string> computerWordSearch();
    int getScoreHuman();
    string getWordsHuman();
    int getScoreComputer();
    string getWordsComputer();
    friend ostream& operator<<(ostream& out, Boggle& boggle);

private:
    Grid<char> board = Grid<char>(BOARD_DIM, BOARD_DIM);
    string boardToString = "";
    Lexicon *dictPtr;
    Set<string> humanWords;
    Set<string> computerWords;
    //METHODS
    void loadBoard(string& boardText);
    void unhighlightBoard();
    bool isOnPathToSoln(Grid<bool> &path, string word, int row, int col);
    bool isValid(string& word);
    bool charNotValid(Grid<bool> &path, string word, int row, int col);
    void exploreForWords(Grid<bool> path, string wordStub, int row, int col);
    bool computerCharValid(Grid<bool> &path, string wordStub, int& row, int& col);

};

#endif // _boggle_h
