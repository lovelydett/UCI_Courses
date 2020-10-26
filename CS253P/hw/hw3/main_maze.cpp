/*************************************************
Author: Yuting Xie
Date:2020-10-23
Description: for problem1 in 253P HW3: maze problem
**************************************************/

#include <cstdio>
#include <vector>
#include <stack>

using std::vector;
using std::stack;

enum Direction {
    UP = 3,
    DOWN = 2,
    LEFT = 1,
    RIGHT = 0
};

bool canPass(vector<vector<int>>& mazeMap, int x, int y, Direction d) {
    //mazeMap stores the reachability of each of the 4 directions, in the order of up-down-left-right
    //e.g: if mazeMap[i][j] == 1001 (binary), it means at the point (i,j), I could go up or right, but not down and left.
    return (mazeMap[x][y] >> d) % 2 == 1;
};

bool walk_maze(vector<vector<int>>& mazeMap, vector<vector<bool>>& visited, int x, int y, stack<int>& res) {

    res.push(x * mazeMap[0].size() + y);// put current point in stack
    visited[x][y] = true;

    if (x == mazeMap.size() - 1 && y == mazeMap[0].size() - 1)
        return true;

    //x for UP/DOWN, y for LEFT/RIGHT, corresponding to row and col.
    if (x-1 >= 0 && !visited[x-1][y] && canPass(mazeMap, x, y, UP)) {
        if (walk_maze(mazeMap, visited, x-1, y, res))
            return true; // we found the result, just return!
    }

    if (x+1 < mazeMap.size() && !visited[x+1][y] && canPass(mazeMap, x, y, DOWN)) {
        if (walk_maze(mazeMap, visited, x+1, y, res))
            return true;
    }

    if (y-1>=0 && !visited[x][y-1] && canPass(mazeMap, x, y, LEFT)) {
        if (walk_maze(mazeMap, visited, x, y-1, res))
            return true;
    }

    if (y+1 < mazeMap[0].size() && !visited[x][y+1] && canPass(mazeMap, x, y, RIGHT)) {
        if (walk_maze(mazeMap, visited, x, y+1, res))
            return true;
    }

    res.pop(); // current point not in final result, pop it
    visited[x][y] = false;

    return false;

}

//init a row*col maze with only outer-rim surrounding boundary
void initMaze(vector<vector<int>>& mazeMap, vector<vector<bool>>& visited, int row, int col) {
    mazeMap.resize(row);
    visited.resize(row);
    for (int i = 0; i < row; i++) {
        mazeMap[i].resize(col);
        visited[i].resize(col);
    }

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            mazeMap[i][j] = 15;   //cuz 1111(2) = 15(10)
            visited[i][j] = false;
        }
    }

    for (int i = 0; i < row; i++) {
        mazeMap[i][0] -= 1 << 1;
        mazeMap[i][col - 1] -= 1;
    }

    for (int i = 0; i < col; i++) {
        mazeMap[0][i] -= 1 << 3;
        mazeMap[row - 1][i] -= 1 << 2;
    }
}

//init and read a maze map from sample_maze.txt
void readMaze(vector<vector<int>>& mazeMap, vector<vector<bool>>& visited){
    //open file
    FILE* fp = fopen("sample_maze.txt","r");
    if(!fp){
        printf("enable to open file\n");
        return;
    }

    //read row and col
    int row, col, _;
    fscanf(fp, "%d x %d Maze!", &row, &col);
    //init maze map
    initMaze(mazeMap, visited, row, col);

    //read inner obstacles
    auto readTilStart = [&](){
        while(true)
            if ('|' == fgetc(fp))
                break;
    };
    for(int i=0; i<row; i++){

        readTilStart(); //now at the start of a new row
        for(int j=0; j<col; j++){

            //printf("processing: %d, %d \n", i, j);
            //'_' only exists at odd positions, and have to skip the last row
            if ('_' == fgetc(fp)  && i!=row-1){
                mazeMap[i][j] -= 1<<2; //indicate this pos cannot go DOWN
                mazeMap[i+1][j] -= 1<<3; //also the pos below cannot go UP
            }

            //'|' only exists at even positions, and have to skip the last col
            if ('|' == fgetc(fp)  && j!=col-1  ){
                mazeMap[i][j] -= 1; //indicate this pos cannot go RIGHT
                mazeMap[i][j+1] -= 1<<1; //also the right pos cannot go LEFT
            }
        }

    }
    
    fclose(fp);
}

void outputSln(stack<int> res, int row, int col){
    
    vector<vector<bool>> path;
    path.resize(row);
    for(int i=0; i<row; i++)
        path[i].resize(col);
    
    stack<int> sln;
    while (!res.empty()) {
        sln.push(res.top());
        path[res.top()/col][res.top()%col] = true;
        res.pop();
    }

    //print solution:
    printf("Solution Path: (start)->");
    while(!sln.empty()){
        printf("%d->",sln.top());
        sln.pop();
    }
    printf("(end)\n");

    //print path map:
    printf("\n~~~~~~~~~~~~~~~~~~~~~\n");
    printf("(start)\n");
    for(auto it_row: path){
        printf("\t");
        for(auto it_col : it_row){
            printf(it_col ? "X " : "  ");
        }
        printf("\n");
    }

    printf("\t");
    for(int i=0;i<col;i++)
        printf("  ");
    printf("(end)\n");

}

int main() {
    vector<vector<int>> maze;
    vector<vector<bool>> visited;

    readMaze(maze, visited);

    stack<int> res;
    if (!walk_maze(maze, visited, 0, 0, res))
        printf("no solution for this maze!\n");
    else
        outputSln(res, maze.size(), maze[0].size());

    return 0;
}