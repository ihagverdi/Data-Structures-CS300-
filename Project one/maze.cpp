/*
The Maze program
Written by Hagverdi Ibrahimli
Date 29. March. 2023
The program generates mazes and writes them to files. Upon the request of the user,
program tries to find a solution path for the given maze and writes it into a file.
*/

#include <iostream>
#include <vector>
#include <random>
#include <sstream>
#include <fstream>
#include <string>
#include "Stack.cpp"
using namespace std;

/*random engine*/
random_device rd;
mt19937 gen(rd());


struct cell {
	bool is_visited;
	int walls; 
	cell() :
		is_visited(false), walls(15) {}; //setting walls parameter to 15 has a binary use (1111) to represent all walls

};
void getInputs(int& Nrows, int& Ncols, int& Nmazes) {
	cout << "Enter the number of mazes: ";
	cin >> Nmazes;
	cout << "Enter the number of rows and columns (M and N): ";
	cin >> Nrows >> Ncols;
}

int randomDirection() {
	// Create a uniform integer distribution over the range [start, end]
	uniform_int_distribution<> dis(0, 3);

	// Generate a random number
	int randomNumber = dis(gen);
	return randomNumber;
}

void writeMazeToFile(const vector<vector<cell>>& maze, const int& index) {
	string fullFileName = "maze_" + to_string(index) + ".txt";
	ofstream outputFile(fullFileName);

	if (!outputFile) {
		cerr << "Cannot open file: " << fullFileName << endl;
	}

	int rows = maze.size();
	int cols = maze[0].size();

	outputFile << cols << " " << rows << endl;

	for (int j = 0; j < cols; j++) {
		for (int i = 0; i < rows; i++) {
			outputFile << "x=" << i << " y=" << j << " ";

			if (maze[i][j].walls & 1) {
				outputFile << "l=1 ";
			}
			else {
				outputFile << "l=0 ";
			}

			if (maze[i][j].walls & 4) {
				outputFile << "r=1 ";
			}
			else {
				outputFile << "r=0 ";
			}

			if (maze[i][j].walls & 8) {
				outputFile << "u=1 ";
			}
			else {
				outputFile << "u=0 ";
			}

			if (maze[i][j].walls & 2) {
				outputFile << "d=1";
			}
			else {
				outputFile << "d=0";
			}

			outputFile << endl;
		}
	}

	outputFile.close();
}

void writePathToFile(const vector<pair<int,int>> & path, const int & index, const int& entryX, const int& entryY, const int& exitX, const int& exitY) {
	string fullFileName = "maze_" + to_string(index) + '_' + "path" + '_' + to_string(entryX) +
		'_' + to_string(entryY) + '_' + to_string(exitX) + '_' + to_string(exitY) + ".txt";
	ofstream outputFile(fullFileName);
	if (!outputFile) {
		cerr << "Cannot open file: " << fullFileName << endl;
	}
	else {
		for (int i = path.size() - 1; i >= 0; i--) {
			outputFile << path[i].first << " " << path[i].second;
			outputFile << endl;
		}
		outputFile.close();
	}
}

void generateMaze(vector<vector<cell>> &maze) {

	Stack<pair<int,int>> stack;

	int height = maze[0].size();
	int width = maze.size();

	stack.push(make_pair(0,0));
	int num_visited = 1;
	maze[0][0].is_visited = true;

	while (num_visited < width * height) {
		vector<int> neighbor_cells;
		// We shall represent up direction as 0, right as 1, down as 2, left as 3
		pair<int,int> current_cell = stack.top();
		int x_coord = current_cell.first;
		int y_coord = current_cell.second;
		
		//up direction
		if (y_coord < height - 1 && !maze[x_coord][y_coord + 1].is_visited) {
			neighbor_cells.push_back(0);
		}
		//right direction
		if (x_coord < width - 1 && !maze[x_coord + 1][y_coord].is_visited) {
			neighbor_cells.push_back(1);
		}
		//down direction
		if (y_coord > 0 && !maze[x_coord][y_coord - 1].is_visited) {
			neighbor_cells.push_back(2);
		}
		//left direction
		if (x_coord > 0 && !maze[x_coord - 1][y_coord].is_visited) {
			neighbor_cells.push_back(3);
		}

		if (!neighbor_cells.empty()) {
			//define a random neighbor to visit
			int random_direction = neighbor_cells[randomDirection() % neighbor_cells.size()];

			switch (random_direction) {
				case 0: // up
					stack.push(make_pair(x_coord, y_coord + 1));
					maze[x_coord][y_coord + 1].is_visited = true;
					maze[x_coord][y_coord + 1].walls &= ~2;  // break the down wall of the upper cell
					maze[x_coord][y_coord].walls &= ~8; //break the upper wall of the current cell
					break;
				case 1: // right
					stack.push(make_pair(x_coord + 1, y_coord));
					maze[x_coord + 1][y_coord].is_visited = true;
					maze[x_coord + 1][y_coord].walls &= ~1; // break the left wall of the new cell
					maze[x_coord][y_coord].walls &= ~4; //break the right wall of the current cell
					break;
				case 2: // down
					stack.push(make_pair(x_coord, y_coord - 1));
					maze[x_coord][y_coord - 1].is_visited = true;
					maze[x_coord][y_coord - 1].walls &= ~8; // break the upper wall of the new cell
					maze[x_coord][y_coord].walls &= ~2; //break the down wall of the current cell
					break;
				case 3: // left
					stack.push(make_pair(x_coord - 1, y_coord));
					maze[x_coord - 1][y_coord].is_visited = true;
					maze[x_coord - 1][y_coord].walls &= ~4; // break the right wall of the new cell
					maze[x_coord][y_coord].walls &= ~1; //break the left wall of the current cell
					break;
			}
			num_visited++;
		}
		else {
			stack.pop(); //backtracking
		}
	}
}

void generateMazesAndWriteToFile(const int &numberOfMazes,const int& rows,const int& cols) {
	for (int i = 0; i < numberOfMazes; i++) {
		vector<vector<cell>> maze(cols, vector<cell>(rows)); //initalize the maze matrix
		generateMaze(maze);
		writeMazeToFile(maze, i+1);
	}
	cout << "All mazes are generated.";
}

void getInputs(const int& Nmazes,  int& mazeID,  int& entryX,   int& entryY,  int& exitX,   int& exitY) {
	cout << "Enter a maze ID between " << 1 << " to " << Nmazes << " inclusive to find a path : ";
	cin >> mazeID;
	cout << "Enter x and y coordinates of the entry points (x,y) or (column,row): ";
	cin >> entryX >> entryY;
	cout << "Enter x and y coordinates of the exit points (x,y) or (column,row): ";
	cin >> exitX >> exitY;
}


void getMazeFromFile(const int& index, vector<vector<cell>> & maze) {
	string fullFileName = "maze_" + to_string(index) + ".txt";
	ifstream inputFile(fullFileName); 

	int Nrows, Ncols;
	inputFile >> Nrows >> Ncols;

	maze = vector<vector<cell>>(Ncols, vector<cell>(Nrows));

	string line;
	while (getline(inputFile, line)) {
		if (!line.empty()) {
			int x_coord, y_coord;
			int up, right, down, left;
			stringstream ss(line);
			string info1, info2, info3, info4, info5, info6;
			ss >> info1 >> info2 >> info3 >> info4 >> info5 >> info6;
			x_coord = stoi(info1.substr(2));
			y_coord = stoi(info2.substr(2));
			//converting bits to our standard (up-right-down-left = 1-1-1-1)
			left = 0 + (info3[2] - '0') == 1 ? 1:0;
			right = 0 + (info4[2] - '0') == 1 ? 4:0;
			up = 0 + (info5[2] - '0') == 1 ? 8:0;
			down = 0 + (info6[2] - '0') == 1? 2:0;

			int result_walls = up | right | down | left; 
			maze[x_coord][y_coord].walls = result_walls;
		}
		}
	inputFile.close();
}

void findMazePath(vector<vector<cell>> & maze, const int & mazeID, const int& entryX, const int& entryY, const int& exitX, const int& exitY) {
	Stack<pair<int,int>> stack;
	int height = maze[0].size();
	int width = maze.size();

	stack.push(make_pair(entryX, entryY));
	maze[entryX][entryY].is_visited = true;
	
	while (!stack.isEmpty()) {

		pair<int,int> current_cell = stack.top();
		int x_coord = current_cell.first;
		int y_coord = current_cell.second;
		bool has_neighbors = false;
		//found the path
		if (x_coord == exitX and y_coord == exitY) {
			vector<pair<int,int>> path;
			while (!stack.isEmpty()) {
				path.push_back(make_pair(stack.top().first, stack.top().second));
				stack.pop();
			}
			//write to the file
			writePathToFile(path, mazeID, entryX, entryY, exitX, exitY); 
		}
		else {
			//up
			if (y_coord < height - 1 && !maze[x_coord][y_coord + 1].is_visited && !(maze[x_coord][y_coord].walls & 8)) {
				maze[x_coord][y_coord + 1].is_visited = true;
				has_neighbors = true;
				stack.push(make_pair(x_coord, y_coord + 1));
			}
			//right
			else if (x_coord < width - 1 && !maze[x_coord + 1][y_coord].is_visited && !(maze[x_coord][y_coord].walls & 4)) {
				maze[x_coord + 1][y_coord].is_visited = true;
				has_neighbors = true;
				stack.push(make_pair(x_coord+1, y_coord));
			}
			//down 
			else if (y_coord > 0 && !maze[x_coord][y_coord - 1].is_visited && !(maze[x_coord][y_coord].walls & 2)) {
				maze[x_coord][y_coord - 1].is_visited = true;
				has_neighbors = true;
				stack.push(make_pair(x_coord, y_coord-1));
			}
			//left
			else if (x_coord > 0 && !maze[x_coord - 1][y_coord].is_visited && !(maze[x_coord][y_coord].walls & 1)) {
				maze[x_coord - 1][y_coord].is_visited = true;
				has_neighbors = true;
				stack.push(make_pair(x_coord-1, y_coord));
			}

			if (!has_neighbors) {
				stack.pop();
			}
		}

	}
}

void runProgram() {
	int Nrows, Ncols, Nmazes;
	int mazeID, entryX, entryY, exitX, exitY;
	getInputs(Nrows, Ncols, Nmazes);
	generateMazesAndWriteToFile(Nmazes, Nrows, Ncols);
	getInputs(Nmazes, mazeID, entryX, entryY, exitX, exitY);
	vector<vector<cell>> maze; // empty maze to store the user-selected maze from the chose .txt file
	getMazeFromFile(mazeID, maze);
	findMazePath(maze, mazeID, entryX, entryY, exitX, exitY);
}

int main() {
	runProgram();
	return 0;
}