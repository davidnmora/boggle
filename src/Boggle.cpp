/*
 * Complete methods for the boggle class, importing
 * data structures, constants, and libraries from the Boggle.h file.
 *
 * Boggle class lets the client instantiate the data representation of a
 * boggle board and game, and executes necessary actions
 * to load, record, store, and/or return data pertaining to words
 * found and existing in the baord. Other than handling cube highlighting
 * during the human word validation search, the boggle class does
 * not execute or manipulate the GUI.
 *
 * Note that, for all inputs, the Boggle class normalizes words
 * to UPPERCASE before working with them.
 */

#include "Boggle.h"

// ***DATA STRUCTURES***

//  letters on all 6 sides of every cube
static string CUBES[16] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

// ***FUNCTIONS***

// Boggle class constructor
Boggle::Boggle(Lexicon& dictionary, string boardText) {
    loadBoard(boardText);
    dictPtr = &dictionary;
}

/*
 * Populate boggle board based on user's decision; either
 * randomly from letter cubes or from user input string
 */
void Boggle::loadBoard(string &boardText) {
    // create a set to avoid repeating cubes
    Set<string> placedCubes;
    for(int r = 0; r < BOARD_DIM; r++){
        for(int c = 0; c < BOARD_DIM; c++) {
            char l;
            // RANDOM: get random CUBE, then random letter from it (don't reuse cubes)
            if(boardText == "") {
                string cube;
                do {
                   cube = CUBES[randomInteger(0, BOARD_DIM*BOARD_DIM - 1)];
                } while(placedCubes.contains(cube));
                placedCubes.add(cube);
                l = cube[randomInteger(0, FACE_COUNT - 1)];
            } else {
                // MANUAL INPUT: select next letter from string
                l = boardText[0];
                boardText.erase(0,1);
            }
            boardToString += l;
            board[r][c] = l;
        }
    }
}

// If in bounds, returns letter as specified row,col on board
char Boggle::getLetter(int row, int col) {
    if (!board.inBounds(row, col)) throw("GRID OBJECT ERROR: attempted to access a cell outside the board.");
    return board[row][col];
}

/*
 * Returns true if input is valid, such that search should be done
 * to see if word can be constructed from boggle board
 */
bool Boggle::isValid(string& word) {
    if(word.length() >= MIN_WORD_LENGTH &&
            dictPtr->contains(word) &&
            !humanWords.contains(word)) {
        return true;
    }
    return false;
}

/*
 * After checking if user input word is a valid word (long enough and in the dictionary),
 * searches recursively for the word through the board,
 * returning true and terminating search upone finding the word on the board,
 * returning false if the word was not found after completing the entire search.
 */
bool Boggle::humanWordSearch(string word) {
    unhighlightBoard();
    word = toUpperCase(word); // normalize all words to UPPERCASE
    if(isValid(word)) {
        // run recursive search, checking possibilties from every possible starting cube
        for(int r = 0; r < BOARD_DIM; r++){
            for(int c = 0; c < BOARD_DIM; c++) {
                Grid<bool> path = Grid<bool>(BOARD_DIM, BOARD_DIM); // stores recursive search path
                // recursively explore neighbors
                if(isOnPathToSoln(path, word, r, c)) {
                    humanWords.add(word);
                    return true;
                } else { //backtrack and unhighlight
                    BoggleGUI::setHighlighted(r, c, false);
                    path[r][c] = false;
                }
            }
        }
    }
    return false; // if word is not found legally in board
}


/*
 * Recursive function checks if next character of target word
 * can be formed using the character at the passed in board position.
 * If so, the function recursively explores all surrounding neighbors.
 * Returns true and terminates if every letter of the word is found.
 * Returns false if a a given recursive exploration doesn't lead to a solution.
 */
bool Boggle::isOnPathToSoln(Grid<bool>& path, string word, int row, int col) {
    // BASECASE: a legal path has been drawn matching every letter of the target word
    if(word == "") return true;
    if(charNotValid(path, word, row, col)) return false;
    // RECURSIVE STEP: add current grid position to path and explore neighbors
    path[row][col] = true;
    // explore neighors
    for(int r = -1; r <= 1; r++) {
        for(int c = -1; c <= 1; c++) {
            if (isOnPathToSoln(path, word.substr(1), row + r, col + c)) {
                return true; // passes true back to top of search tree
            }
        }
    }
    // RECURSIVELY BACKTRACK: if no solution possible from this character
    BoggleGUI::setHighlighted(row, col, false);
    path[row][col] = false;
    return false;
}

/*
 * Checks if selected char can validly help construct the target word.
 * Highlights char if it's inbounds, regardless if it passes the
 * other tests, to mark it as searched temporarily.
 */
bool Boggle::charNotValid(Grid<bool>& path, string word, int row, int col) {
    // CHECK 1: isn't inbounds (impossible to highlight)
    if(!board.inBounds(row,col)) return true;
    // CHECK 2: given char should not have already been used in the word
    if (path[row][col] == true) {
        return true;
    }
    BoggleGUI::setHighlighted(row, col, true); // highlight char being explored
    // CHECK 3: selected char should form next needed letter of word
    if(board[row][col] != word[0]) {
        BoggleGUI::setHighlighted(row, col, false);
        return true;
    }
    return false; // meaning: it IS a valid char
}

// Returns human player's score based on quantity and lenght of words found
int Boggle::getScoreHuman() {
    int score = 0;
    for(string word : humanWords) {
        // 1 point for meeting minimum, 1 for every extra letter
        score += word.length() - MIN_WORD_LENGTH + 1;
    }
    return score;
}

// Returns a string representation of the set containing words found by human player
string Boggle::getWordsHuman() {
    return humanWords.toString();
}

// Returns a set of all possible words on boggle board not already found by human
Set<string> Boggle::computerWordSearch() {
    unhighlightBoard();
    // check every possible cube as a starting letter
    for(int r = 0; r < BOARD_DIM; r++){
        for(int c = 0; c < BOARD_DIM; c++) {
            // recursively explore neighbors
            Grid<bool> path = Grid<bool>(BOARD_DIM, BOARD_DIM);
            string wordStub = "";
            exploreForWords(path, wordStub, r, c);
         }
    }
    return computerWords;
}
/*
 * Populates computerWords with every possible, legal word on board.
 * To simplify recursive backtracking, the path is passed by value
 * rather than reference.
 */
void Boggle::exploreForWords(Grid<bool> path, string wordStub, int row, int col) {
    // Check: have we found a new word of sufficient length? If so, store and continue.
    if(wordStub.length() >= MIN_WORD_LENGTH &&
       dictPtr->contains(wordStub) &&
       !humanWords.contains(wordStub)) {
        computerWords.add(wordStub);
    }
    if(computerCharValid(path, wordStub, row, col)) {
        // RECURSIVE CASE: cube passed all checks, so explore neighors
        wordStub += board[row][col]; // add letter to word
        path[row][col] = true;
        for(int r = -1; r <= 1; r++) {
            for(int c = -1; c <= 1; c++) {
                exploreForWords(path, wordStub, r+row, c+col);
            }
        }
    }
}

// Determines if char at passed in position can be used toward a valid word
bool Boggle::computerCharValid(Grid<bool>& path, string wordStub, int& row, int& col) {
    // CHECK 1: is it within the bounds of the board?
    if(!board.inBounds(row, col)) return false;
    // CHECK 2: has this letter currently unused in our wordStub?
    if (path[row][col] == true) return false;
    wordStub += board[row][col];
    // CHECK 3: does the updated wordStub form a prefix from the dictionary?
    if(!dictPtr->containsPrefix(wordStub)) return false;

    return true;
}

// Returns computers score based on quantity and lenght of words found
int Boggle::getScoreComputer() {
    int score = 0;
    for(string word : computerWords) {
        // 1 point for meeting minimum, 1 for every extra letter
        score += word.length() - MIN_WORD_LENGTH + 1;
    }
    return score;
}

// Returns a printable string of the number and a list of words found
string Boggle::getWordsComputer() {
    return "My words (" +integerToString(computerWords.size()) + "): " + computerWords.toString();
}

void Boggle::unhighlightBoard() {
   BoggleGUI::labelAllCubes(boardToString);
}

// Override prints out a human-readable board given a cout-ed boggle instance
ostream& operator<<(ostream& out, Boggle& boggle) {
    for(int r = 0; r < BOARD_DIM; r++){
        for(int c = 0; c < BOARD_DIM; c++) {
            c == BOARD_DIM- 1 ? out << boggle.getLetter(r,c) << endl : out << boggle.getLetter(r,c);
        }
    }
    return out;
}
