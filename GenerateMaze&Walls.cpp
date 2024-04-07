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
    }
    void printMaze()
    {
        int c = cols + 2;
        int r = rows + 2;
        char **array = new char *[r];
        for (int i = 0; i < r; ++i)
        {
            array[i] = new char[c];
        }
        for (int i = 0; i < r; i++)
        {
            for (int j = 0; j < c; j++)
            {
                if (i == 0 || j == 0 || j == c - 1 || i == r - 1)
                    array[i][j] = '#';
                else
                    array[i][j] = grid[i - 1][j - 1];
            }
        }
        bool flag1 = true;
        bool flag2 = true;
        for (int i = 0; i < r; i++)
        {
            for (int j = 0; j < c; j++)
            {
                if (j == 0 && (array[i][1] == '.' || array[i][1] == '*') && flag1)
                {
                    array[i][0] = 'S';
                    saveiS = i;
                    flag1 = false;
                }
                else if (j == c - 2 && (array[r - 1 - i][j] == '.' || array[r - 1 - i][j] == '*') && flag2)
                {
                    array[r - 1 - i][j + 1] = 'F';
                    saveiF = r - 1 - i;
                    flag2 = false;
                }
                cout << array[i][j] << " ";
            }
            cout << "\n";
        }
        for (int i = 0; i < r; ++i)
        {
            delete[] array[i];
        }
        delete[] array;
    }

// A* search algorithm for pathfinding.
    vector<pair<int, int>> aStarSearch(int startX, int startY, int goalX, int goalY)
    {
        set<pair<int, pair<int, int>>> openSet;
        map<pair<int, int>, pair<int, int>> cameFrom;
        map<pair<int, int>, int> gScore, fScore;

        auto startNode = make_pair(startX, startY), goalNode = make_pair(goalX, goalY);
        gScore[startNode] = 0;
        fScore[startNode] = heuristic(startX, startY, goalX, goalY);
        openSet.insert({fScore[startNode], startNode});

        while (!openSet.empty())
        {
            auto current = openSet.begin()->second;
            if (current == goalNode)
            {
                vector<pair<int, int>> path = reconstructPath(cameFrom, current);
                path.insert(path.begin(), startNode); // Ensure the start node is included in the path.
                return path;
            }

            openSet.erase(openSet.begin());
            for (auto &dir : directions)
            {
                auto neighbor = make_pair(current.first + dir.first, current.second + dir.second);
                if (isValid(neighbor.first, neighbor.second))
                {
                    int tentative_gScore = gScore[current] + 1;
                    if (!gScore.count(neighbor) || tentative_gScore < gScore[neighbor])
                    {
                        cameFrom[neighbor] = current;
                        gScore[neighbor] = tentative_gScore;
                        fScore[neighbor] = tentative_gScore + heuristic(neighbor.first, neighbor.second, goalX, goalY);
                        openSet.insert({fScore[neighbor], neighbor});
                    }
                }
            }
        }

        return {};
    }
// Manhattan distance A*
    int heuristic(int x1, int y1, int x2, int y2) const
    {
        return abs(x1 - x2) + abs(y1 - y2);
    }

vector<pair<int, int>> reconstructPath(map<pair<int, int>, pair<int, int>> &cameFrom, pair<int, int> current) const
    {
        vector<pair<int, int>> path;
        path.push_back(current);
        while (cameFrom.find(current) != cameFrom.end())
        {
            current = cameFrom[current];
            path.push_back(current);
        }
        reverse(path.begin(), path.end());
        return path;
    }

    void markPath(const vector<pair<int, int>> &path)
    {
        if (path.empty())
            return;

        if (path.size() > 1 && grid[path[1].first][path[1].second] == '#')
        {
            cout << "\n No valid path found." << endl;
            exit(EXIT_FAILURE);
        }

        for (size_t i = 0; i < path.size(); i++)
        {
            const auto &cell = path[i];
            grid[cell.first][cell.second] = '*';
        }
    }
};
int main()
{
    Maze maze(10, 11);
    maze.generateMaze();
    cout << "Initial Maze:" << endl;
    maze.printMaze();

    vector<pair<int, int>> path = maze.aStarSearch(maze.saveiS - 1, 0, maze.saveiF - 1, 10); // Start and finish positions
    if (path.empty())
    {
        cout << "\nNo path found." << endl;
    }
    else
    {
        maze.markPath(path);
        cout << "\nMaze with Path:" << endl;
        maze.printMaze();
    }

    return 0;
}
