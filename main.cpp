#include "bits/stdc++.h"
#include "Position.h"
using namespace std;

#pragma region Structs

#pragma region Piece
struct Piece
{
    bool isWhite;
    char type;
    Piece() : isWhite(false), type('x') {}
    Piece(bool _isWhite, char _type) : isWhite(_isWhite), type(_type) {}
};
#pragma endregion

#pragma region Move
struct Move
{
    Position from;
    Position to;
};
#pragma endregion

#pragma endregion

#pragma region Board

class Board
{
public:
    Piece pieces[8][8];
    Board(string fenCode)
    {
        int x = 0, y = 0;

        for (int i = 0; i < fenCode.length(); i++)
        {
            if (isdigit(fenCode[i]))
            {
                x += fenCode[i] - '0';
            }
            else if (fenCode[i] == '/')
            {
                x = 0;
                y++;
            }
            else
            {
                pieces[y][x].isWhite = isupper(fenCode[i]);
                pieces[y][x].type = tolower(fenCode[i]);
                x++;
            }
        }
    }
    vector<Position> possibleMoves(int x, int y)
    {
        Piece currentPiece = pieces[y][x];

        switch (currentPiece.type)
        {
        case 'p':
            return pawnMoves(x, y);
        case 'n':
            return knightMoves(x, y);
        case 'b':
            return bishopMoves(x, y);
        case 'r':
            return rookMoves(x, y);
        case 'q':
        {
            vector<Position> bishop = bishopMoves(x, y);
            vector<Position> rook = rookMoves(x, y);
            vector<Position> moves;
            moves.reserve(bishop.size() + rook.size());
            moves.insert(moves.end(), bishop.begin(), bishop.end());
            moves.insert(moves.end(), rook.begin(), rook.end());
            return moves;
            break;
        }
        case 'k':
            return kingMoves(x, y);
        default:
            return {};
        }
    }
    vector<Position> possibleMoves(Position position)
    {
        return possibleMoves(position.x, position.y);
    }
    void executeMove(int x1, int y1, int x2, int y2)
    {
        pieces[y2][x2] = pieces[x1][y1];
        pieces[y1][x1].type = 'x';
    }
    void executeMove(Position from, Position to)
    {
        pieces[to.y][to.x] = pieces[from.y][from.x];
        pieces[from.y][from.x].type = 'x';
    }
    void executeMove(Move move)
    {
        pieces[move.to.y][move.to.x] = pieces[move.from.y][move.from.x];
        pieces[move.from.y][move.from.x].type = 'x';
    }
    bool hasKing()
    {
        for (int y = 0; y < 8; y++)
            for (int x = 0; x < 8; x++)
                if (pieces[y][x].type == 'k')
                    return true;
        return false;
    }
    string fenCode()
    {
        string fenCode = "";

        int num = 0;

        for (int y = 0; y < 8; y++)
        {
            for (int x = 0; x < 8; x++)
            {
                Piece currentPiece = pieces[y][x];
                if (currentPiece.type == 'x')
                    num++;
                else
                {
                    if (num > 0)
                        fenCode += to_string(num);
                    num = 0;
                    fenCode += (char)(currentPiece.isWhite ? toupper(currentPiece.type) : currentPiece.type);
                }
            }
            if (num > 0)
                fenCode += to_string(num);
            num = 0;
            if (y < 7)
                fenCode += "/";
        }

        return fenCode;
    }

private:
    vector<Position> pawnMoves(int x, int y)
    {
        vector<Position> possibleMoves;
        Piece currentPawn = pieces[y][x];
        int multiplier = currentPawn.isWhite ? -1 : 1;

        Position topLeft = {x - 1, y + multiplier};
        Position topRight = {x + 1, y + multiplier};
        Position top = {x, y + multiplier};

        if (isInBounds(topLeft.x, topLeft.y))
        {
            Piece testPiece = pieces[topLeft.y][topLeft.x];
            if (testPiece.type != 'x' && currentPawn.isWhite != testPiece.isWhite)
                possibleMoves.push_back(topLeft);
        }
        if (isInBounds(topRight.x, topRight.y))
        {
            Piece testPiece = pieces[topRight.y][topRight.x];
            if (testPiece.type != 'x' && currentPawn.isWhite != testPiece.isWhite)
                possibleMoves.push_back(topRight);
        }
        if (isInBounds(top.x, top.y))
        {
            Piece testPiece = pieces[top.y][top.x];
            if (testPiece.type == 'x')
                possibleMoves.push_back(top);
        }

        return possibleMoves;
    }
    vector<Position> knightMoves(int x, int y)
    {
        vector<Position> possibleMoves;
        Piece currentKnight = pieces[y][x];
        int xOffset = 2, yOffset = 1;

        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                for (int k = 0; k < 2; k++)
                {
                    Position currentPosition = {x + xOffset, y + yOffset};
                    if (isInBounds(currentPosition))
                    {
                        Piece testPiece = pieces[currentPosition.y][currentPosition.x];
                        if (testPiece.type == 'x' || testPiece.isWhite != currentKnight.isWhite)
                            possibleMoves.push_back(currentPosition);
                    }
                    swap(xOffset, yOffset);
                }
                yOffset *= -1;
            }
            xOffset *= -1;
        }

        return possibleMoves;
    }
    vector<Position> bishopMoves(int x, int y)
    {
        vector<Position> possibleMoves;
        Piece currentBishop = pieces[y][x];

        int xOffset = 1, yOffset = 1;

        for (int i = 0; i < 4; i++)
        {
            swap(xOffset, yOffset);
            yOffset *= -1;
            Position currentPosition = {x + xOffset, y + yOffset};
            while (isInBounds(currentPosition))
            {
                Piece testPiece = pieces[currentPosition.y][currentPosition.x];
                if (testPiece.type == 'x')
                    possibleMoves.push_back(currentPosition);
                else if (testPiece.isWhite != currentBishop.isWhite)
                {
                    possibleMoves.push_back(currentPosition);
                    break;
                }
                else
                    break;

                currentPosition.x += xOffset;
                currentPosition.y += yOffset;
            }
        }

        return possibleMoves;
    }
    vector<Position> rookMoves(int x, int y)
    {
        vector<Position> possibleMoves;
        Piece currentRook = pieces[y][x];

        int xOffset = 1, yOffset = 0;

        for (int i = 0; i < 4; i++)
        {
            swap(xOffset, yOffset);
            yOffset *= -1;

            Position currentPosition = {x + xOffset, y + yOffset};
            while (isInBounds(currentPosition))
            {
                Piece testPiece = pieces[currentPosition.y][currentPosition.x];
                if (testPiece.type == 'x')
                    possibleMoves.push_back(currentPosition);
                else if (testPiece.isWhite != currentRook.isWhite)
                {
                    possibleMoves.push_back(currentPosition);
                    break;
                }
                else
                    break;

                currentPosition.x += xOffset;
                currentPosition.y += yOffset;
            }
        }

        return possibleMoves;
    }
    vector<Position> kingMoves(int x, int y)
    {
        vector<Position> possibleMoves;
        Piece currentKing = pieces[y][x];

        int xOffset = 1, yOffset = 0;

        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                swap(xOffset, yOffset);
                yOffset *= -1;

                Position currentPosition = {x + xOffset, y + yOffset};
                if (isInBounds(currentPosition))
                {
                    Piece testPiece = pieces[currentPosition.y][currentPosition.x];
                    if (testPiece.type == 'x' || testPiece.isWhite != currentKing.isWhite)
                        possibleMoves.push_back(currentPosition);
                }
            }
            yOffset = 1;
        }
        return possibleMoves;
    }
    bool isInBounds(int x, int y)
    {
        return (x >= 0 &&
                x < 8 &&
                y >= 0 &&
                y < 8);
    }
    bool isInBounds(Position position)
    {
        return (position.x >= 0 &&
                position.x < 8 &&
                position.y >= 0 &&
                position.y < 8);
    }
};

#pragma endregion

#pragma region Utilities
void printOutBoard(Piece pieces[8][8])
{
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            if (pieces[y][x].type == 'x')
                cout << ". ";
            else
                cout << (char)(pieces[y][x].isWhite ? toupper(pieces[y][x].type) : pieces[y][x].type) << " ";
        }

        cout << endl;
    }
    cout << endl;
}
void printOutBoard(Board board)
{
    printOutBoard(board.pieces);
}
void printOutBoard(string fenCode)
{
    Piece pieces[8][8];

    int x = 0, y = 0;

    for (int i = 0; i < fenCode.length(); i++)
    {
        if (isdigit(fenCode[i]))
        {
            x += fenCode[i] - '0';
        }
        else if (fenCode[i] == '/')
        {
            x = 0;
            y++;
        }
        else
        {
            pieces[y][x].isWhite = isupper(fenCode[i]);
            pieces[y][x].type = tolower(fenCode[i]);
            x++;
        }
    }

    printOutBoard(pieces);
}
void printOutPossibleMoves(int x, int y, Board board)
{
    vector<Position> possibleMoves = board.possibleMoves(x, y);
    char test[8][8];
    fill(&test[0][0], &test[0][0] + sizeof(test), '.');

    test[y][x] = (char)(board.pieces[y][x].isWhite ? toupper(board.pieces[y][x].type) : board.pieces[y][x].type);

    for (Position pos : possibleMoves)
        test[pos.y][pos.x] = 'X';

    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
            cout << test[y][x] << " ";

        cout << endl;
    }
    cout << endl;
}
void printOutMove(Move move, Board board)
{
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            if (move.from.x == x && move.from.y == y)
                cout << "F ";
            else if (move.to.x == x && move.to.y == y)
                cout << "T ";
            else if (board.pieces[y][x].type == 'x')
                cout << ". ";
            else
                cout << (char)(board.pieces[y][x].isWhite ? toupper(board.pieces[y][x].type) : board.pieces[y][x].type) << " ";
        }
        cout << endl;
    }
    cout << endl;
}
vector<Position> sortPositions(vector<Position> positions)
{
    vector<Position> returnPositions;

    for (Position pos : positions)
    {
        bool hasBeenInserted = false;
        int size = returnPositions.size();

        for (int i = 0; i < size; i++)
        {
            if (pos.y < returnPositions[i].y)
            {
                returnPositions.insert(returnPositions.begin() + i, pos);
                hasBeenInserted = true;
                break;
            }
            else if (pos.y == returnPositions[i].y && pos.x < returnPositions[i].x)
            {
                returnPositions.insert(returnPositions.begin() + i, pos);
                hasBeenInserted = true;
                break;
            }
        }

        if (!hasBeenInserted)
            returnPositions.push_back(pos);
    }

    return returnPositions;
}
#pragma endregion

int shortestSequence = INT32_MAX;
int positionsTested = 0;
const int maxPositions = 1000000;
int numberOfSolutions = 0;
string solution;

#pragma region Functions
vector<Board> allPossiblePositionsForWhite(Board board)
{
    vector<Board> positions;

    for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++)
        {
            if (board.pieces[y][x].type != 'x' && board.pieces[y][x].isWhite)
            {
                vector<Position> possibleMoves = board.possibleMoves(x, y);

                for (Position pos : possibleMoves)
                {
                    Board newBoard = board;
                    newBoard.executeMove({x, y}, pos);
                    Piece testPiece = newBoard.pieces[pos.y][pos.x];
                    if (testPiece.type == 'p')
                    {
                        if (testPiece.isWhite && pos.y == 0)
                        {
                            newBoard.pieces[pos.y][pos.x].type = 'n';
                            positions.push_back(newBoard);
                            newBoard.pieces[pos.y][pos.x].type = 'r';
                            positions.push_back(newBoard);
                            newBoard.pieces[pos.y][pos.x].type = 'q';
                            positions.push_back(newBoard);
                            newBoard.pieces[pos.y][pos.x].type = 'b';
                            positions.push_back(newBoard);
                        }
                        else if (pos.y == 7)
                        {
                            newBoard.pieces[pos.y][pos.x].type = 'q';
                            positions.push_back(newBoard);
                        }
                        else
                            positions.push_back(newBoard);
                    }
                    else
                        positions.push_back(newBoard);
                }
            }
        }

    return positions;
}
Board blacksTurn(Board board)
{
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            Piece testPiece = board.pieces[y][x];

            if (testPiece.type == 'x' || testPiece.isWhite)
                continue;

            vector<Position> moves = sortPositions(board.possibleMoves(x, y));

            for (Position position : moves)
            {
                Piece pieceToTake = board.pieces[position.y][position.x];
                if (pieceToTake.type != 'x' && pieceToTake.isWhite)
                {
                    board.executeMove({x, y}, position);
                    return blacksTurn(board);
                }
            }
        }
    }

    return board;
}
void shortestSolution(Board board, int numMoves, int maxMoves, string positions)
{
    if (numMoves > maxMoves || numMoves > shortestSequence)
        return;

    if (positionsTested >= maxPositions)
        return;

    positionsTested++;

    if (!board.hasKing())
    {
        shortestSequence = numMoves;
        solution = positions + board.fenCode();
        numberOfSolutions++;
        return;
    }

    board = blacksTurn(board);
    vector<Board> possiblePositions = allPossiblePositionsForWhite(board);

    for (Board position : possiblePositions)
        shortestSolution(position, numMoves + 1, maxMoves, positions + board.fenCode() + " ");
}
#pragma endregion

int main()
{
#pragma region Actual stuff
    string fenCode;
    bool useSameCode = false;

    while (true)
    {
        positionsTested = 0;
        shortestSequence = INT32_MAX;
        numberOfSolutions = 0;

        if (!useSameCode)
        {
            cout << "Enter the FEN Notation: ";
            cin >> fenCode;
        }

        Board board(fenCode);
        cout << "Enter the max amount of moves: ";
        int maxMoves;
        cin >> maxMoves;
        time_t start, end;
        time(&start);
        ios_base::sync_with_stdio(false);
        shortestSolution(board, 0, maxMoves, "");
        time(&end);

        cout << positionsTested << " positions were tested" << endl;

        if (shortestSequence == INT32_MAX)
            cout << "There is no solution for this board in " << maxMoves << " moves" << endl;
        else
        {
            cout << "There are " << numberOfSolutions << " solutions that take " << shortestSequence << " moves" << endl;
            string temp = "";
            for (char c : solution)
                if (c == ' ')
                {
                    printOutBoard(temp);
                    temp = "";
                }
                else
                    temp += c;
            printOutBoard(temp);
        }

        double time_taken = double(end - start);
        cout << "Execution time: " << fixed
             << time_taken << setprecision(5);
        cout << " sec " << endl;

        string nextAction = "";
        while (nextAction != "repeat" && nextAction != "new" && nextAction != "exit" && nextAction != "r" && nextAction != "n" && nextAction != "e")
        {
            cout << "Next action: ";
            cin >> nextAction;
        }

        if (nextAction == "repeat" || nextAction == "r")
            useSameCode = true;
        else if (nextAction == "new" || nextAction == "n")
            useSameCode = false;
        else
            break;
    }
#pragma endregion
}