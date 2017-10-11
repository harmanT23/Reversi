/* 
 * File:   Reversi2
 * Author: Harmanprit Tatla
 *
 * Created on November 1, 2015, 1:02 AM
 */
#include <sys/time.h>
#include <sys/resource.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int printBoard(char board[26][26], int size) {
    //Prints initial configuration of the board

    //Used to determine locations of White and Black pieces
    int tilePos1 = size / 2, tilePos2 = tilePos1 - 1;

    //Prints column indices with proper spacing
    printf("  ");
    for (int indices = 0; indices < size; indices++)
        printf("%c", 'a' + indices);
    printf("\n");

    //Prints board configuration
    for (int row = 0; row < size; row++) {
        printf("%c ", 'a' + row);

        for (int col = 0; col < size; col++) {

            if ((row == (tilePos2)) && (col == (tilePos2)))
                board[row][col] = 'W';

            else if ((row == (tilePos2)) && (col == tilePos1))
                board[row][col] = 'B';

            else if ((row == tilePos1) && (col == tilePos2))
                board[row][col] = 'B';


            else if ((row == tilePos1) && (col == tilePos1))
                board[row][col] = 'W';

            else
                board[row][col] = 'U';

            printf("%c", board[row][col]);
        }
        printf("\n");
    }

    return size;
}

void printCurrentBoard(char board[26][26], int dimension) {
    //Prints most recent version of game board

    //Prints indices
    printf("  "); //adds appropriate spacing
    for (int indices = 0; indices < dimension; indices++) //Prints indeces
        printf("%c", 'a' + indices);
    printf("\n");

    //Prints the current version of the board
    for (int i = 0; i < dimension; i++) {
        printf("%c ", 'a' + i);
        for (int j = 0; j < dimension; j++) {
            printf("%c", board[i][j]);
        }
        printf("\n");
    }
}


bool positionInBounds(int N, char row, char col) {
    //Checks whether a given position is within the board.
    char START_INDEX = 'a';
    return ((row >= START_INDEX) && (row <= ((N - 1) + START_INDEX)) &&
            (col >= START_INDEX) && (col <= ((N - 1) + START_INDEX)));
}


char oppositeColour(char colour) {
    //Returns opposite colour
    char W_PIECE = 'W', B_PIECE = 'B';

    if(colour == W_PIECE)
        return B_PIECE;

    else
        return W_PIECE; 
}


bool checkLegalInDirection(char board[26][26], int N, char row, char col,
        char colour, int deltaRow, int deltaCol) {
    
    //Checks 1 of 8 possible directions for validity.
    const char START_INDEX = 'a', EMPTY = 'U'; //starting row and col index
    int rowInt = row - START_INDEX, colInt = col - START_INDEX;

    //Checks if position on board and if it is empty
    if (positionInBounds(N, row, col) && board[rowInt][colInt] == EMPTY) {
        //Get next cell
        rowInt += deltaRow;
        colInt += deltaCol;

        //Check if next immediate cell on board and is opposite colour
        if(positionInBounds(N, rowInt + 'a', colInt + 'a') && 
                (board[rowInt][colInt] == oppositeColour(colour))) {

            while(positionInBounds(N, rowInt + 'a', colInt + 'a') && 
                    (board[rowInt][colInt] != EMPTY)) {
                
                if(board[rowInt][colInt] != oppositeColour(colour))
                    return true;
                
                rowInt += deltaRow;
                colInt += deltaCol;
            }

        }
    }
    
    return false; //At this point position is invalid

}

int scoreAndFlipInDirection(char board[26][26], int dimension, int row, int col, 
        char colour, int deltaRow, int deltaCol, bool doFlip) {

    int positionScore = 0;
    
    if (doFlip)  
        board[row][col] = colour; //replace empty tile with given colour
    
    // move to next cell
    row += deltaRow;
    col += deltaCol;
    
    
    while(positionInBounds(dimension, row + 'a', col + 'a') && 
            board[row][col] != 'U' && board[row][col] != colour) {
        
        if (board[row][col] == oppositeColour(colour)) {
            if(doFlip) board[row][col] = colour; 
            positionScore++;
        }
        
        row += deltaRow;
        col += deltaCol;
    }

    return positionScore;

}

int checkIsValidAndFlip(char board[26][26], int dimension, int row, int col, 
        char colour, bool doFlip) {
    //Checks all 8 directions to determine if a position is a valid move
    int score = 0; //score for the given position
    
    for (int deltaRow = -1; deltaRow <= 1; deltaRow++) {
        for (int deltaCol = -1; deltaCol <= 1; deltaCol++) {

            //Don't execute if deltaRow=deltaCol=0, not valid direction
            if (!((deltaRow == 0) && (deltaCol == 0))) {
                
                //If given direction valid for position, then update score.
                if(checkLegalInDirection(board, dimension, row + 'a', col + 'a', 
                        colour, deltaRow, deltaCol)) {
                    score += scoreAndFlipInDirection(board, dimension, row, col, 
                            colour, deltaRow, deltaCol, false);
                    
                    //Flip tiles in the valid direction.
                    if(doFlip)
                        scoreAndFlipInDirection(board, dimension, row + deltaRow
                                , col + deltaCol, colour, deltaRow, deltaCol, 
                                true);
                }

            }
        }
    }
    
    if (doFlip) board[row][col] = colour;
    
    return score;
}


int possibleMoves(char board[26][26], int dimension, char colour) {
    //Finds, prints moves for white and black pieces based on current board conf
    const char START_INDEX = 'a'; //starting char index of row and col
    bool validPos = false; 
    int numMoves = 0;
    
    //Iterates through all pos's in board and finds avail moves for colour
    for (int row = 0; row < dimension; row++) {
        for (int col = 0; col < dimension; col++) {
            
            validPos = checkIsValidAndFlip(board, dimension, row, col,
                    colour, false);
            
            //Increment num moves if position is valid for colour
            if (validPos) {
                numMoves++;
            }
        }
    }
    
                
            
    return numMoves;
}


int tileCounter (char board[26][26], int dimension, char colour) {
    //Counts number of colour tiles on board
    
    int numTiles = 0; //holds number of tiles
    //Loops through entire board checking number of colour tiles
    for(int rows = 0; rows < dimension; rows++) {
        for(int cols = 0; cols < dimension; cols++) {
            if (board[rows][cols] == colour)
                numTiles++;
        }
    }
    
    return numTiles;
}

bool isGameDone(char playerColour, char compColour, char board[26][26], 
        int size) {
    return possibleMoves(board, size, playerColour) || possibleMoves(
            board, size, compColour);
}

void whoWon(char board[26][26], int dimension, char playerColour, 
        char compColour) {
    //Determines winner and prints who won.
    int playerTiles = tileCounter(board, dimension, playerColour);
    int compTiles = tileCounter(board, dimension, compColour);
    
    if (playerTiles > compTiles)
        printf("%c player wins.", playerColour);
    
    else if (compTiles > playerTiles)
        printf("%c player wins.", compColour);
    
    else
        printf("Draw, no player wins.");
    
}


bool playerMoves(char board[26][26], int dimension, char colour) {
    //Gets player move, and determines validity
    const int MAX = 2; //Max number of entries user can input in single move
    char userMove[MAX]; //array to hold user input
    
    bool validMove;

    //Prompts user and gets input
    printf("Enter move for colour %c (RowCol): ", colour);
    for (int i = 0; i < MAX; i++) 
        scanf(" %c", &userMove[i]); //assigns player move to array
    

    //Checks if valid move, and does flip
    if (checkIsValidAndFlip(board, dimension, userMove[0] - 'a', 
            userMove[1] - 'a', colour, true)) {
        return true;
    }
    
    else {
        printf("Invalid move.\n");
        return false;
    }

}

bool goodnessValue(char board[26][26], int dimension, char colour, int row, 
        int col) {
    //Computes score of a move based on a strategy
    int baseScore = 1, cornerWeight = 1000, topOrBottom = 500, 
            minFlips = 3, tilesFlipped;
    
    tilesFlipped = checkIsValidAndFlip(board, dimension, row, col, colour, 
            false);
    
    if (row == col)
        return baseScore*cornerWeight; //Corner move preferred
    
    else if ((row == 1 && col < dimension) || 
            (row == (dimension - 2) && col < dimension))
        return topOrBottom*cornerWeight; //2nd top or bottom row preferred
    
    else if(minFlips <= tilesFlipped)
        return baseScore * tilesFlipped; //If flips at least 3 tiles is good. 
    
    else
        return baseScore;
    
}


bool computerMoves(char board[26][26], int dimension, char colour) {
    int biggestScore = 0; //later holds highest score of a position
    int rowBest = 0, colBest = 0; //col and row position of high score position
    bool done = false;
    
    for(int row = 0; row < dimension && !done; row++) {
        for(int col = 0; col < dimension && !done; col++) {
            
            if(checkIsValidAndFlip(board, dimension, row, col,colour, false)) {
                
                if (biggestScore < goodnessValue(board, dimension, colour, row, 
                        col))
                //assigns new biggest score
                biggestScore = checkIsValidAndFlip(board, dimension, row, col, 
                    colour, false);
                rowBest = row;
                colBest = col;
                
                if(biggestScore > dimension)
                    done = true;
                    
            }
                
        }
            
    }
    

    printf("Computer places %c at %c%c.\n", colour, rowBest + 'a', 
            colBest + 'a');
    checkIsValidAndFlip(board, dimension, rowBest, colBest, colour, true);
      
}




void gameTurns(char board[26][26], int dimension) {
    //Iterates through turns until game ends
    const char W_PIECE = 'W', B_PIECE = 'B'; //colour of pieces
    char playerColour, compColour; //holds player and comp colour
    bool turn, validEntry = true; //turn checker, and validity checker
    
    //Gets computer colour
    printf("Computer plays (B/W): ");
    scanf(" %c", &compColour);
    playerColour = oppositeColour(compColour); //Gives player opposite colour
    
    printBoard(board, dimension); //prints board
    
    
    //Black player goes first.
    if(compColour == B_PIECE) {
        computerMoves(board, dimension, compColour);
        turn = false; //Player will go next
    }
    
    else {
        validEntry = playerMoves(board, dimension, playerColour);
        turn = true; //computer will go next
    }
    
    if(validEntry)
        printCurrentBoard(board, dimension); //prints after first turn
    
    //Loops until there is winner or invalid entry entered. 
    while(isGameDone(playerColour, compColour, board, dimension) && validEntry) 
    {
        
        if(turn) {
            
            if(possibleMoves(board, dimension, compColour)) {
                computerMoves(board, dimension, compColour);
                printCurrentBoard(board, dimension);
            }
            
            else
                printf("%c player has no valid move.\n", compColour);
            
            turn = false;
            
        }
        
        else {
            if(possibleMoves(board, dimension, playerColour)) {
               validEntry = playerMoves(board, dimension, playerColour);
               
               if(validEntry) //prints only if user entered valid value
                   printCurrentBoard(board, dimension);
            }
            
            else
                printf("%c player has no valid move.\n", playerColour);
            
            turn = true;        
        }
        
    }
    
    if(validEntry) {
        //finds winner and prints.
        whoWon(board, dimension, playerColour, compColour);
    }
    
    else 
        printf("%c player wins.", compColour);
          
}

int main(void) {
     int size = 0; //holds size of board

    //Gets board size & computer colour 
    printf("Enter the board dimension: ");
    scanf("%d", &size);
    char reversiBoard[size][size];
    
    gameTurns(reversiBoard, size); //gets computer colour, then iterates turns.
    return 0;
}



