#include <iostream>
#include <vector>
#include <string>

using namespace std; //in this particular case, this is convenient

/** maximum allowable board size (don't want to go off the screen) */
const int maxSize = 20;
/** Arbirary minimum allowed board size */
const int minSize = 10;

/** Utility function, @returns if @param size is between maxSize and minSize */
bool legalSize(int size){
  if(size > maxSize || size < minSize){
      return false;
    } else {
      return true;
    }
}

/** the provided class */
class WordValidator {
public:
  virtual ~WordValidator() = default;
  virtual bool isValid(const string &){
    return true; //stub
  }
};

/** A class that can display and manipulate a Scrabble board */
class ScrabbleBoard {
  /** the game board */
  std::vector<std::vector<char> > board;
  /** board size X */
  int maxX;
  /** board size Y */
  int maxY;

public:
  /** Our trusty validation object. Public so main() can also validate */
  WordValidator dictionary;
  /** List of already played words */
  vector<string> words;
  ScrabbleBoard(int sizeX, int sizeY):
    maxX(sizeX),
    maxY(sizeY){
    for(int i=0; i<maxX; ++i){
        vector<char> newbie;
        board.push_back(newbie);
        for(int j=0; j<maxY; ++j){
            board[i].push_back(' '); //initialize the board to blank spaces
          }
      }
  }

  void printBoard(){
    cout << "  ";
    for(int i=0; i<maxX; ++i){
        cout << i; //insert  Y index on first line
        cout << " ";
      }
    cout << "\n";
    for(int j=0; j<maxY; ++j){
        if(j < 10){
            cout << " "; //so our words don't shift when we hit double digits
          }
        cout << j; //insert X index
        cout << " ";
        for(int k=0; k<maxX; ++k){
            cout << board[k][j]; //finally, print the actual board
            cout << " "; //for prettiness
          }
        cout << "\n";
      }
    cout << "\n";

  }
  int getMaxX() const{
    return maxX;
  }
  int getMaxY() const{
    return maxY;
  }

  /** @returns char at x, y */
  char getCell(int x, int y){
    return board[x][y];
  }

  /** @returns true if the cell is already occupied by a non-default character (' ')*/
  bool checkCell(int x, int y){
    if(getCell(x,y) == ' '){
        return false;
      }
    return true;
  }

  /** given an index that contains a letter, finds the (possibly vertical) word it is a part of and @returns that word */
  string getWord(int x, int y, bool vertical){
    string word = "";
    if(getCell(x,y)==' '){
        return " "; //bail, we've been called on an invalid location.
      } else {
        if(vertical){ //if vertical, iterate up until we find the start of the word...
            while(y-1 >= 0 && getCell(x,y-1) != ' '){ //while we're at a valid index and still have letters
                --y;
              }
            while(y <= maxY && getCell(x,y) != ' '){
                word += getCell(x,y);
                ++y;
              }
          } else { //...else iterate left until we find the start.
            while(x-1 >= 0 && getCell(x-1,y) != ' '){ //while we're at a valid index and still have letters
                --x;
              }
            while(x < maxX && getCell(x,y) != ' '){
                word += getCell(x,y);
                ++x;
              }
          }
      }
    return word;
  }

  bool checkAdjacent(string word, int x, int y, bool vertical){
    int length = word.length(); //caching for convenience
    string tempWord; //for holding our created word so we can check it
    if(vertical){
        for(int i=y; i < y+length; ++i){ //NB: we initialize to y here and want to be less than y+length
            if(x+1 <= maxX && board[x+1][i] != ' '){ //x+1: we are checking the cells above and below our word
                tempWord = getWord(x+1,i,false); //false: we're checking for colliding horizontal words because we are vertical
                if(!dictionary.isValid(tempWord)){
                    return false; //then it's an illegal move
                  }
              }
            if(x-1 >= 0 && board[x-1][i] != ' '){ //check both left and right
                tempWord = getWord(x-1,i,false);
                if(!dictionary.isValid(tempWord)){
                    return false;
                  }
              }
          }
      } else {
        for(int i=x; i < x+length; ++i){ //NB: we initialize to y here and want to be less than y+length
            if(y+1 <= maxY && board[i][y+1] != ' '){
                tempWord = getWord(i,y+1,true); //true: we are checking for colliding vertical words
                if(!dictionary.isValid(tempWord)){
                    return false; //then it's an illegal move
                  }
              }
            if(y-1 >= 0 && board[i][y-1] != ' '){
                tempWord = getWord(i,y-1,true); //true: we are checking for colliding vertical words
                if(!dictionary.isValid(tempWord)){
                    return false; //then it's an illegal move
                  }
              }
          }
      }
    return true; //we made it through and it's all legal
  }

  /** given a VALID word, prints the word on the board and validates any created words */
  bool printWord(string word, int xx, int yy, bool vertical, bool first){
    bool collision = false; //to make sure we get a word that intersects
    bool newLetter = false; //just in case someone plays the same word twice
    int xindex = 0;
    int yindex = 0;
    for(unsigned i=0; i<word.size(); ++i){
        xindex = vertical?xx:xx+i; //caching these values for convenience
        yindex = vertical?yy+i:yy;
        if(yindex>=maxY || xindex>=maxX){
            return false;
          }
        if(checkCell(xindex,yindex)){ //our "check legal" loop
            if(word.at(i)==getCell(xindex,yindex)){ //if our letter matches the conflicting one
                collision = true;
                continue; //we don't need to replace an already existing letter!
              } else {
                return false; //then it's a different letter and the move is illegal.
              }
          } else {
            newLetter = true;
          }

      }
    if(checkAdjacent(word,xx,yy,vertical) && (collision || first) && newLetter){ //check adjacent words and check collision
        xindex=0;
        yindex=0;
        for(unsigned i=0; i<word.size(); ++i){ //our "print it" loop
            xindex = vertical?xx:xx+i;
            yindex = vertical?yy+i:yy;
            if(xindex >= 0 && xindex <= maxX && yindex >= 0 && yindex <= maxY){
                board[xindex][yindex] = word.at(i);
              } else {
                return false;
              }
          }
        return true;  //we made it; the move is legal and it's printed!
      }
    return false;
  }

  bool alreadyPlayed(string word){
    for(unsigned i=0; i<words.size(); ++i){
        if(word == words.at(i)){
            return true;
          }
      }
    return false;
  }
};

int main() {
  int x = 0, y = 0;
  bool first = true;
  while(x == 0){
      cout << "Enter Board Size X: ";
      cin >> x;
      if(!legalSize(x)){
          x=0;
        }
    }
  while(y == 0){
      cout << "Enter Board Size Y: ";
      cin >> y;
      if(!legalSize(y)){
          y=0;
        }
    }
  ScrabbleBoard game(x,y);

  int xcoor=-1, ycoor=-1; //initialize to absurd value
  bool vertical = false;
  string word="";
  while(true){ //main game loop, we'll restart if we want to test again
      game.printBoard(); //show the board
      while(xcoor == -1){
          cout << "Enter X coordinate: ";
          cin >> xcoor;
          if(xcoor > game.getMaxX() || xcoor < 0){
              xcoor = -1;
            }
        }
      while(ycoor == -1){
          cout << "Enter Y coordinate: ";
          cin >> ycoor;
          if(ycoor > game.getMaxY() || ycoor < 0){
              ycoor = -1;
            }
        }
      while(word == ""){
          cout << "Enter a word to place there: ";
          cin >> word;
          if(!game.dictionary.isValid(word)){ //validate word before attempting to place it
              cout << "Unrecognized word.\n";
              word = "";
            } else if(game.alreadyPlayed(word)){
              cout << "Duplicate word.\n";
              word = "";
            }
        }
      char input='N';
      cout << "Vertical?(Y/N): ";
      cin >> input;
      if(input == 'Y'){
          vertical = true;
        } else {
          vertical = false;
        }
      bool legal = game.printWord(word, xcoor, ycoor, vertical, first);
      if(legal){
          cout << "Word accepted.\n";
          game.words.push_back(word);
          first=false;
        } else {
          cout << "Illegal move.\n";
        }
      xcoor=-1; //reset variables and move again
      ycoor=-1;
      word="";
    }
}


