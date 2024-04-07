#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <random>
#include <algorithm>
#include <unordered_set>
#include <cmath>
#include <limits>
#include <set>
#include <map>
#include <cstdlib>

using namespace std;

class Maze
{private:
    int rows, cols;
    vector<vector<char>> grid;
    unordered_set<int> visited;
    vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    bool isValid(int row, int col) const
    { // ensures the validity of maze
        return row >= 0 && row < rows && col >= 0 && col < cols && grid[row][col] == '.';
    }

    int countVisitedNeighbors(int x, int y) const
    {
        int count = 0;
        for (auto &dir : directions)
        {
            int nxCor = x + dir.first;
            int nyCor = y + dir.second;
            if (isValid(nxCor, nyCor) && visited.find(nxCor * cols + nyCor) != visited.end())
            {
                ++count;
            }
        }
        return count;
    }

    void addWalls(int x, int y, vector<pair<int, int>> &walls)
    {
        for (auto &dir : directions)
        {
            int nxCor = x + dir.first;
            int nyCor = y + dir.second;
            if (nxCor >= 0 && nxCor < rows && nyCor >= 0 && nyCor < cols && visited.find(nxCor * cols + nyCor) == visited.end() && grid[nxCor][nyCor] == '#')
            {
                walls.push_back({nxCor, nyCor});
            }
        }
    }public:
    int saveiS;
    int saveiF;
    Maze(int _rows, int _cols) : rows(_rows), cols(_cols), grid(_rows, vector<char>(_cols, '#')) {}

    void generateMaze()
    {
        srand(static_cast<unsigned int>(time(nullptr)));
        int startX = rand() % rows;
        int startY = rand() % cols;
        visited.insert(startX * cols + startY);
        grid[startX][startY] = '.';

        vector<pair<int, int>> walls;
        addWalls(startX, startY, walls);

        default_random_engine rng(random_device{}());

        while (!walls.empty())
        {
            shuffle(walls.begin(), walls.end(), rng);
            pair<int, int> wall = walls.back();
            walls.pop_back();

            if (countVisitedNeighbors(wall.first, wall.second) == 1)
            {
                visited.insert(wall.first * cols + wall.second);
                grid[wall.first][wall.second] = '.';
                addWalls(wall.first, wall.second, walls);
            }
        }
    }}