#include <iostream>
#include <fstream>

using namespace std;

bool Solve(int in[9][9], int possible[9][9][10], int out[9][9], int start);
bool CheckOne(int in[9][9], int pos);
int ValidSolution(int in[9][9]);
bool GeneratePossibleNums(int sudokuBoard[9][9], int possibleNums[9][9][10]);

int main()
{
    ifstream inFile("Sudoku3.txt");
    int sudokuBoard[9][9];
    int solvedBoard[9][9];
    int possibleNums[9][9][10];
    for(int i = 0; i < 9; i++)
        for(int j = 0; j < 9; j++)
            inFile >> sudokuBoard[i][j];
    if(GeneratePossibleNums(sudokuBoard, possibleNums))
    {
        for(int i = 0; i < 9; i++)
            {
                for(int j = 0; j < 9; j++)
                    cout << sudokuBoard[i][j] << " ";
                cout << endl;
            }
        if(ValidSolution(sudokuBoard) == 0)
        {
            for(int i = 0; i < 9; i++)
            {
                for(int j = 0; j < 9; j++)
                    cout << sudokuBoard[i][j] << " ";
                cout << endl;
            }
            CheckOne(solvedBoard, 5);
        }
        else{
            double numPossible = 1;
            for(int i = 0; i < 9; i++)
            {
                for(int j = 0; j < 9; j++)
                {
                    cout << i << " " << j << ": ";
                    numPossible *= possibleNums[i][j][0];
                    for(int k = 1; k <= possibleNums[i][j][0]; k++)
                        cout << possibleNums[i][j][k] << " ";
                    cout << endl;
                }
            }
            cout << "Number of combinations:" << numPossible << endl;
            cout << "Non easy Board" << endl;
        }
    }
    else
        cout << "Impossible Board" << endl;
    Solve(sudokuBoard, possibleNums, solvedBoard, 0);
    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
            cout << solvedBoard[i][j] << " ";
        cout << endl;
    }
    return 0;
}

//-------------------------------------------------------------------------------------------------
// Solves the Sudoku board by recursion and brute force.
// Returns true if solved, false otherwise
// params(in,out,in)
// in - the 9x9 Sudoku Board to solve
// possible - a 9x9x10 array specifying which numbers are possible for each square
// out - the 9x9 Sudoku Board solved
// start - position to start from
//-------------------------------------------------------------------------------------------------
bool Solve(int in[9][9], int possible[9][9][10], int out[9][9], int start)
{
    while(in[start / 9][start % 9] != 0 && start < 81)
        start++;
    if(start >= 81)
    {
        if(ValidSolution(in) == 0)
        {
            for(int k = 0; k < 9; k++)
                for(int l = 0; l < 9; l++)
                    out[k][l] = in[k][l];
            return true;
        }
        return false;
    }
    for(int k = 0; k < 9; k++)
            for(int l = 0; l < 9; l++)
                out[k][l] = in[k][l];
    //Try the possibilities generated previously.
    for( int i = 1; i <= possible[start / 9][start % 9][0]; i++)
    {
        out[start / 9][start % 9] = possible[start / 9][start % 9][i];
        if(CheckOne(out, start))
        {
            int newBoard[9][9];
            if(Solve(out, possible, newBoard, start + 1))
            {
                for(int k = 0; k < 9; k++)
                    for(int l = 0; l < 9; l++)
                        out[k][l] = newBoard[k][l];
                return true;
            }
        }
    }
    return false;
}

//-------------------------------------------------------------------------------------------------
// Check if the square at pos is invalid.
// params(in, in)
// in - the 9x9 Sudoku board
// pos - position to check
//-------------------------------------------------------------------------------------------------
bool CheckOne(int in[9][9], int pos)
{
    int row = pos / 9;
    int col = pos % 9;
    for(int i = 0; i < 9; i++)
    {
        if(i != col && in[row][col] == in[row][i])
            return false;
    }
    for(int i = 0; i < 9; i++)
    {
        if(i != row && in[row][col] == in[i][col])
            return false;
    }
    int squareRow = (row / 3) * 3;
    int squareCol = (col / 3) * 3;
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
        {
            if( (i + squareRow) != row && (j + squareCol) != col && in[row][col] == in[i + squareRow][j + squareCol])
                return false;
        }
    return true;
}

//-------------------------------------------------------------------------------------------------
// Checks if the board is a valid solution.
// in - the 9x9 Sudoku board to check
//-------------------------------------------------------------------------------------------------
int ValidSolution(int in[9][9])
{
    int numErrors = 0;
    for(int i = 0; i < 81; i++)
        if(!CheckOne(in, i))
            numErrors++;
    return numErrors;
}


//-------------------------------------------------------------------------------------------------
// Takes in a 9x9 sudokuBoard with 0s in empty positions. This function will put any numbers in
// the board where there is only one choice. possibleNums will contain a list of numbers that work
// in each square. If the board has any squares with no choices, then it will return false.
// Otherwise true is returned
// params(inout, out)
// sudokuBoard - 9x9 array
// possibleNums - 9x9x10 array ([][][0] indicates number of possible nums, the rest is a list of
// nums that work)
//-------------------------------------------------------------------------------------------------
bool GeneratePossibleNums(int sudokuBoard[9][9], int possibleNums[9][9][10])
{
    for(int i = 0; i < 9; i++)
        for(int j = 0; j < 9; j++)
        {
            if(sudokuBoard[i][j] == 0)
            {
                possibleNums[i][j][0] = 0;
                for(int k = 1; k <= 9; k++)
                {
                    sudokuBoard[i][j] = k;//Try with k in the place
                    if(CheckOne(sudokuBoard, (i) * 9 + j )) //was it a valid number??
                    {
                        possibleNums[i][j][++possibleNums[i][j][0]] = k;//add to list of possibilities
                    }
                }
                if(possibleNums[i][j][0] == 0)
                    return false;
                else if(possibleNums[i][j][0] == 1) //only one choice, use it
                {
                    sudokuBoard[i][j] = possibleNums[i][j][1];
                    i = j = 0;//restart loop using new info
                }
                else
                    sudokuBoard[i][j] = 0; //can't replace it
            }
            else
            {
                possibleNums[i][j][0] = 1; //already known
                possibleNums[i][j][1] = sudokuBoard[i][j];
            }
        }
        return true;
}
