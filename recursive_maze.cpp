#include <iostream>
#include <stack>
#include <windows.h>

#define SIZE 15
#define clearScreen() std::cout << "\033[1;1H\033[2J";
#define HEAD_COLOUR "\033[48;5;49m"
#define START_COLOUR "\033[48;5;196m"
#define RESET "\033[m"

struct Node {
    int y, x;
    bool checked = false;
};

bool inRange(int y, int x) {
    if (y < SIZE && y > -1) {
        if (x < SIZE && x > -1) {
            return true;
        }
    }
    return false;
}

struct Maze {
    private:
        char **grid = new char*[SIZE];
    public:
        int initial_y, initial_x;
        std::stack<Node> stack;
        bool show_back_tracking;
        // default constructor
        Maze() {
            for (int i = 0; i < SIZE; i++) {
                grid[i] = new char;
            }
            initial_y = rand() % SIZE;
            initial_x = rand() % SIZE;
            stack.push({initial_y, initial_x, true});
            show_back_tracking = false;
        }
        // get grid off the heap
        ~Maze() {
            delete[] grid;
        }

    void initGrid();
    void printGrid();
    
    void backTrack();
    bool finished();
    void createMaze();
    
};

void Maze::initGrid() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            grid[i][j] = '.';
        }
    }   
}

void Maze::printGrid() {
    clearScreen();
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (i == initial_y && j == initial_x) {
                std::cout << START_COLOUR << grid[i][j] << RESET << "  ";
            } else if (i == stack.top().y && j == stack.top().x) {
                std::cout << HEAD_COLOUR << grid[i][j] << RESET << "  ";
            } else {
                std::cout << grid[i][j] << "  ";
            }
            // (grid[i][j] & 15) == 0 ? std::cout << "+---" : std::cout << "+   ";
        }
        std::cout << std::endl;
        // std::cout << "+ " << std::endl;
        // for (int j = 0; j < SIZE; j++) {
        //     (grid[i][j] & 1) == 0 ? std::cout << "|   " : std::cout << "    ";
        // }			
        // std::cout << "| " << std::endl;
    }
    // for (int j = 0; j < SIZE; j++) {
    //     std::cout << "+---";
    // }			
    // std::cout << "+ " << std::endl;
}

void Maze::backTrack() {
    bool down  = false;
    bool up    = false;
    bool right = false;
    bool left  = false;
    if (!inRange(stack.top().y + 1, stack.top().x)) {
        down = true;
    } else if (grid[stack.top().y + 1][stack.top().x] == '#') {
        down = true;
    }

    if (!inRange(stack.top().y - 1, stack.top().x)) {
        up = true;
    } else if (grid[stack.top().y - 1][stack.top().x] == '#') {
        up = true;
    }

    if (!inRange(stack.top().y, stack.top().x + 1)) {
        right = true;
    } else if (grid[stack.top().y][stack.top().x + 1] == '#') {
        right = true;
    }

    if (!inRange(stack.top().y, stack.top().x - 1)) {
        left = true;
    } else if (grid[stack.top().y][stack.top().x - 1] == '#') {
        left = true;
    }

    if (down && up && right && left) {
        stack.pop();
        if (show_back_tracking) {printGrid();} // visualises the back-tracking
        backTrack();
    }
}

bool Maze::finished() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (grid[i][j] != '#') {
                return false;
            }
        }
    }
    return true;
}

void Maze::createMaze() {
    int direction   = (rand() % 3) - 1;   // random nums of set {-1, 0, 1}
    int rand_choice =  rand() % 2;        // gives a 50/50 chance to do either
    switch(rand_choice) {
        case 0:
            if (inRange(stack.top().y + direction, stack.top().x)) {
                if (grid[stack.top().y + direction][stack.top().x] != '#') {        // check if the North and South node is not checked

                    grid[stack.top().y + direction][stack.top().x] = '#';           // if so, print to the grid
                    stack.push({stack.top().y + direction, stack.top().x, true});   // and push onto the stack
                    printGrid();
                }
            }
            break;
        case 1: 
            if (inRange(stack.top().y, stack.top().x + direction)) {
                if (grid[stack.top().y][stack.top().x + direction] != '#') {        // check if the East and West node is not checked

                    grid[stack.top().y][stack.top().x + direction] = '#';           // if so, print to the grid
                    stack.push({stack.top().y, stack.top().x + direction, true});   // and push onto the stack
                    printGrid();
                }
            }
            break;
    }
    if (finished()) {return;}
    backTrack();
    createMaze();
    
};

int main() {
    srand(time(NULL));

    while (1) {
        Maze maze;
        maze.initGrid();    

        clock_t timer;
        timer = clock();

        // maze.show_back_tracking = true; // visualises the back-tracking
        maze.createMaze();

        timer = clock() - timer;

        std::cout << "Nodes: " << maze.stack.size() << std::endl;
        std::cout << "Timer: " << (float) timer / CLOCKS_PER_SEC << " seconds";
        // std::cin.get();
    }


}
