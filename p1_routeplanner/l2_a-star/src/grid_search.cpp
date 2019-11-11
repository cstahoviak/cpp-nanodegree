// pre-compiler instructions
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

// #include "grid.cpp"

using std::cout;
using std::string;
using std::vector;

/* to build and run:
 * $ cd obj/
 * $ g++ ../src/grid_search.cpp -o ./grid_search.o && ./grid_search.o
 * (w/ dbg sym) $ g++ -g ../src/grid_search.cpp -o ./grid_search.o && ./grid_search.o
 */

// why are the data types of kEmpty and kObstacle not declared??
enum class State {kEmpty, kObstacle, kClosed, kPath, kStart, kFinish};

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

std::string CellString( State state ) {

    // if(state == State::kObstacle) {
    //     return "⛰️   ";
    // }
    // else {
    //     return "0   ";
    // }

    switch(state) {
        case State::kObstacle: return "⛰️   ";
        case State::kPath: return "🚗   ";
        case State::kStart: return "🚦   ";
        case State::kFinish: return "🏁   ";
        default: return "0   "; 
    }
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
    //         cout << CellString(board[i][j]);
    //     }
    //     cout << "\n";
    // }

    return;
}

bool CheckValidCell( int x, int y, vector<vector<State>> &grid ) {
    // check that the (x,y) coordinate pair is a valid grid location
    // if( x < 0 || x > grid.size() || y < 0 || y > grid[0].size() ) {
    //     return false;
    // // check that the grid cell is not kClosed or kObstacle
    // } else if( grid[x][y] == State::kClosed || grid[x][y] == State::kObstacle ) {
    //     return false;
    // } else {
    //     return true;
    // }

    bool on_grid_x = (x >= 0 && x < grid.size());
    bool on_grid_y = (y >= 0 && y < grid[0].size());
    if (on_grid_x && on_grid_y)
        return grid[x][y] == State::kEmpty;
    return false;
}

bool Compare( const vector<int> node1, const vector<int> node2) {
    if (node1[2] + node1[3] > node2[2] + node2[3]) {
        return true;
    } else {
        return false;
    }

    // int f1 = a[2] + a[3]; // f1 = g1 + h1
    // int f2 = b[2] + b[3]; // f2 = g2 + h2
    // return f1 > f2; 
}

void CellSort( vector<vector<int>> *v ) {
    // sort the two-dimensional vector of ints by f-value in descending order
    std::sort(v->begin(), v->end(), Compare);
}

int Heuristic( int x1, int y1, int x2, int y2 ) {
    // comuptes the Manhattan distance to goal
    return std::abs(x2-x1) + std::abs(y2-y1);

}

void AddToOpen( int x, int y, int g, int h, vector<vector<int>> &open_nodes, 
                                            vector<vector<State>> &grid ) {
    // adds the node to the open list and marks the grid cell as closed
    open_nodes.push_back( vector<int> {x, y, g, h} );
    grid[x][y] = State::kClosed;
}

/* "A common usage of const is to guard against accidentally changing a variable,
 * especially when it is passed-by-reference as a function argument."
 */
void ExpandNeighbors( const vector<int> &current_node,
                      int goal[2],
                      vector<vector<int>> &open_nodes,
                      vector<vector<State>> &grid ) {
    // Loops through the current node's neighbors and calls appropriate functions
    // to add neighbors to the open list

    // NOTE: An array is a C++ container much like a vector, although without the
    // ability to change size after initialization. Arrays can be accessed and
    // iterated over just as vectors.
    const int delta[4][2]{{-1, 0}, {0, -1}, {1, 0}, {0, 1}}; // directional deltas

    // get current node's data
    const int current_x = current_node[0];
    const int current_y = current_node[1];
    const int current_g = current_node[2];

    // loop through current node's potential neighbors
    for( auto row : delta) {
        int potential_x = current_x + row[0];
        int potential_y = current_y + row[1];
        // check that the potential neighbors is a valid grid cell & not closed
        if( CheckValidCell(potential_x, potential_y, grid) ) {
            // increment g-val, comoute h-val, add neighbor to open list
            AddToOpen(potential_x,
                      potential_y,
                      current_g + 1, 
                      Heuristic(potential_x, potential_y, goal[0], goal[1]),
                      open_nodes,
                      grid);
        }
    }
}

/** 
 * Implementation of A* search algorithm
 */
vector<vector<State>> Search( vector<vector<State>> grid,
                              int init[2],
                              int goal[2] ) {
    /*
    1. maintain a list of open nodes
    2. while there are still nodes to explore and goal not reached, expand node
    with lowest f-value
    */

    // initialize the vector of open nodes
    vector<vector<int>> open_nodes {};

    // initialize the starting node
    int h_val = Heuristic(init[0], init[1], goal[0], goal[1]);
    AddToOpen( init[0], init[1], 0, h_val, open_nodes, grid);

    int iter = 0;
    while( open_nodes.size() > 0 ) {

        // // TODO: Sort the open list using `CellSort`, and get the current node
        CellSort( &open_nodes );
        // vector<int> current_node = open_nodes[0];    // my method - incorrect!

        // returns a reference to the last element in the vector.
        vector<int> current_node = open_nodes.back();

        // removes the last element in the vector, effectively reducing the container size by one.
        open_nodes.pop_back();

        // TODO: Get the x and y values from the current node,
        // and set grid[x][y] to kPath.
        grid[current_node[0]][current_node[1]] = State::kPath;

        // check to see if current node is goal node
        if (current_node[0] == goal[0] && current_node[1] == goal[1]) {
            grid[init[0]][init[1]] = State::kStart;
            grid[goal[0]][goal[1]] = State::kFinish;
            return grid;
        } else {
            // if not goal, expand search to current node's neighbors
            ExpandNeighbors( current_node, goal, open_nodes, grid );
        }
        iter++;
    }
    // We've run out of new nodes to explore and haven't found a path.
    cout << "No path found!" << "\n";
    return std::vector<vector<State>>{};
}

#include "test.cpp"     // unit tests

int main() {

    int init[2]{0, 0};
    int goal[2]{4, 5};

    // read board data from file
    string path = "../data/1.board";
    auto board = ReadBoardFile( path );

    // search the the board for a solution path - A* Search
    auto solution = Search( board, init, goal );

    // print solution
    PrintBoard( solution );

    // Unit Tests
    TestHeuristic();
    TestAddToOpen();
    TestCompare();
    TestCheckValidCell();
    TestExpandNeighbors();
    // TestSearch();   // not passing for some reason..?
}