/*
 * Executes the play of a single game, handling all printing
 * to the console, updating GUI (highlighting is handled in
 * boggle class), instantiating GUi and boggle game.
 */

#include "lexicon.h"
#include "console.h"
#include "bogglegui.h"
#include "Boggle.h"
#include "simpio.h"
#include <string>

using namespace std;

//CONSTANTS
static const int MS_DELAY = 100;

//FUNCTION PROTOTYPES
string getBoardType();
bool invalidInput(string boardLetters);
void loadGUI(Boggle& b);
void humanPlays(Boggle &b);
void computerPlays(Boggle& b);
void winnerAnnounced(Boggle &b);

//FUNCTIONS
void playOneGame(Lexicon& dictionary) {
    BoggleGUI::initialize(BOARD_DIM, BOARD_DIM);
    Boggle b(dictionary, getBoardType()); //instantiate Boggle object
    loadGUI(b);
    humanPlays(b);
    computerPlays(b);
    winnerAnnounced(b);
}

/*
 * Handles human's turn, taking and checking input
 * words until user is finished finding words.
 */
void humanPlays(Boggle& b) {
    //reset GUI + console
    clearConsole();
    BoggleGUI::setStatusMessage("");
    BoggleGUI::setAnimationDelay(MS_DELAY);
    string word;
    while(true) {
        cout << b << endl; //cout board using operator override
        cout << "Your words: " << b.getWordsHuman() << endl;
        cout << "Your score: " << b.getScoreHuman() << endl;
        BoggleGUI::setScore(b.getScoreHuman(), BoggleGUI::HUMAN);
        word = toUpperCase(trim(getLine("Type a word (or Enter to stop): ")));
        if(word == "") break;
        clearConsole();
        //send to boggle instance
        if (b.humanWordSearch(word)) {
            string validWordMsg = "You found a new word! '" + word +  "'";
            cout << validWordMsg << endl;
            BoggleGUI::recordWord(word, BoggleGUI::HUMAN);
            BoggleGUI::setStatusMessage(validWordMsg);
        } else {
            string invalidWordMsg = "You must enter an unfound 4+ letter word from the dictionary.";
            BoggleGUI::setStatusMessage(invalidWordMsg);
            cout << invalidWordMsg << endl;
        }
    }
}

/*
 * Retrieves from the user whether they'd like a randomly
 * generated board, and if not, prompts them to enter
 * a board until it is valid.
 */
string getBoardType() {
    string boardLetters = "";
    if(!getYesOrNo("Do you want to generate a random board? ")) {
        string prompt = "Type the 16 letters to appear on the board: ";
        boardLetters = toUpperCase(trim(getLine(prompt)));
        while(invalidInput(boardLetters)) {
            prompt = "That is not a valid 16-letter board string. Try again. \nType the 16 letters to appear on the board: ";
            BoggleGUI::setStatusMessage(prompt);
            boardLetters = toUpperCase(trim(getLine(prompt)));
        }
    }
    return boardLetters;
}

//verifies if input board string is has exactly 16 letters, returns false if so
bool invalidInput(string boardLetters) {
    if(boardLetters.size() != BOARD_DIM*BOARD_DIM) return true;
    for(char c : boardLetters) {
        if (c < 'A' || c > 'Z') return true;
    }
    return false;
}

//loads GUI with the board's characters
void loadGUI(Boggle& b) {
    for(int r = 0; r < BOARD_DIM; r++){
        for(int c = 0; c < BOARD_DIM; c++) {
            BoggleGUI::labelCube(r, c, b.getLetter(r,c));
        }
    }
}

//Handles computers turn, finding and displaying all remaining words
void computerPlays(Boggle &b) {
    cout << endl << "It's my turn!" << endl;
    for(string word : b.computerWordSearch()) {
        BoggleGUI::recordWord(word, BoggleGUI::COMPUTER);
    }
    cout << b.getWordsComputer() << endl;
    cout << "My score: " <<  b.getScoreComputer() << endl;
    BoggleGUI::setScore(b.getScoreComputer(), BoggleGUI::COMPUTER);
}

//Winner announced based on who has found more words
void winnerAnnounced(Boggle &b) {
    if(b.getScoreComputer() > b.getScoreHuman()) {
        string msg = "Ha ha ha, I destroyed you. Better luck next time, puny human!";
        cout << msg << endl << endl;
        BoggleGUI::setStatusMessage(msg);
    } else {
        string msg = "WOW, you defeated me! Congratulations!";
        cout << msg << endl << endl;
        BoggleGUI::setStatusMessage(msg);
    }
}
