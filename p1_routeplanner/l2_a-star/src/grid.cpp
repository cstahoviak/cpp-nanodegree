
// pre-compiler instructions
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

using std::cout;
using std::vector;

// why are the data types of kEmpty and kObstacle not declared??
enum class State {kEmpty, kObstacle};

// to build and run:
// $ cd obj/
// $ g++ ../src/grid.cpp -o ./grid.o && ./grid.o

// NOTE: must be declared BEFORE readBoardFile()
vector<State> ParseLine( std::string line ) {
    std::istringstream sline(line);
    int n;
    char c;
    vector<State> row;
    
    while (sline >> n >> c && c == ',') {
        // TODO: Modify the line below to push_back
        // a State::kEmpty if n is 0, and push_back
        // a State::kObstacle otherwise.
        if(n == 0) {
            row.push_back(State::kEmpty);
        }
        else {
            row.push_back(State::kObstacle);
        }
    }

    return row;
}

vector<vector<State>> ReadBoardFile( std::string path ) {
    // read board from file
    std::ifstream board_file( path );
    vector<vector<State>> board{};

    if (board_file) {
    //   cout << "The file stream has been created!" << "\n";
      std::string line;
        while (getline(board_file, line)) {
            vector<State> row = ParseLine( line );
            board.push_back( row );
        }
    } 

    return board;
}

// TODO: Create the CellString function here,
// using the following return strings:
// "⛰️   "
// "0   "
std::string CellString( State state ) {

    if(state == State::kObstacle) {
        return "⛰️   ";
    }
    else {
        return "0   ";
    }

    // switch(state) {
    //     case State::kObstacle: return "⛰️   ";
    //     default: return "0   "; 
    // }
}

void PrintBoard( const vector<vector<State>> board) {
    // range-based for loops
    for(auto v : board) {       // could "strongly-type" State instead of auto
        for(auto i : v) {       // could "strongly-type" State instead of auto
            cout << CellString(i);
        }
        cout << "\n";
    }

    // for (int i = 0; i < board.size(); i++) {
    //     for (int j = 0; j < board[i].size(); j++) {
    //         cout << cellString(board[i][j]);
    //     }
    //     cout << "\n";
    // }

    return;
}

int addFcn( vector<int> v ) {
    int sum = 0;

    for(auto elem : v) {
        sum += elem;
    }

    return sum;
}

// #include "test_parse_line.cpp"

int main() {
    // create board manually
    vector<vector<int>> board{{0, 1, 0, 0, 0, 0},
                              {0, 1, 0, 0, 0, 0},
                              {0, 1, 0, 0, 0, 0},
                              {0, 1, 0, 0, 0, 0},
                              {0, 0, 0, 0, 1, 0}};

    // print board properties
    cout << board.size() << "\n";
    cout << board[1].size() << "\n";

    // call user-defined function addFcn()
    cout << "\naddFcn( board[0] = " << addFcn( board[0] ) << "\n\n";

    // test parsing function - NOT working - would need to overload ParseLine()
    // for type std::vector<int>
    // TestParseLine();

    // load board from file and save as 2D 'State' vector
    std::string path = "../data/1.board";
    auto board1 = ReadBoardFile( path );
    PrintBoard( board1 );
}