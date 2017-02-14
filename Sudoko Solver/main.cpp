#include <iostream>
#include <fstream>

using namespace std;

bool Solve(int in[9][9], int out[9][9], int start);
bool CheckOne(int in[9][9], int pos);
int ValidSolution(int in[9][9]);

int main()
{
    ifstream inFile("Sudoku2.txt");
    int sudokuBoard[9][9];
    int solvedBoard[9][9];
    for(int i = 0; i < 9; i++)
        for(int j = 0; j < 9; j++)
            inFile >> sudokuBoard[i][j];
   Solve(sudokuBoard, solvedBoard, 0);
   cout << (ValidSolution(solvedBoard) == 0 ? "true" : "false") << endl;
    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
            cout << solvedBoard[i][j] << " ";
        cout << endl;
    }
    CheckOne(solvedBoard, 5);
    return 0;
}

bool Solve(int in[9][9], int out[9][9], int start)
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
    for( int i = 1; i <= 9; i++)
    {
        out[start / 9][start % 9] = i;
        if(CheckOne(out, start))
        {
            int newBoard[9][9];
            if(Solve(out, newBoard, start + 1))
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


int ValidSolution(int in[9][9])
{
    int numErrors = 0;
    for(int i = 0; i < 81; i++)
        if(!CheckOne(in, i))
            numErrors++;
    return numErrors;

}
