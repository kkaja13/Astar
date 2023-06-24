#include "A_star.h"
#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
using namespace std;

graph::graph(int rows, int cols)
{
	this->rows = rows;
	this->cols = cols;
	has_solution = false;

	grid = std::vector<std::vector<int>>(rows, std::vector<int>(cols, 0));
	grid_cells = std::vector<std::vector<Cell>>(rows, std::vector<Cell>(cols));
	path = std::vector<Cell>();
}

graph::graph(const graph& src)
{
	this->rows = src.rows;
	this->cols = src.cols;
	this->has_solution = src.has_solution;
}

bool graph::is_valid(Cell a)
{
	if ((a.x >= 0) && (a.x < rows) && (a.y >= 0) && (a.y < cols)) //pokud jsou souradnice v poradku, vraci true
		return true;
	else
		return false;
}

int graph::heuristic(const Cell& a, const Cell& end) 
{
	//Diagonal distance
	int dx = abs(a.x - end.x);
	int dy = abs(a.y - end.y);

	int h_straight = 10 * max(dx, dy);			//pohyb horizontalni nebo vertikalni
	int h_diag = (14 - 2 * 10) * min(dx, dy);	//diagonalni pohyb, zaporna hodnota -> je levnejsi, ma prednost

	return h_straight + h_diag;
}

void graph::search(Cell start, Cell end)
{
	vector<vector<bool>> on_border(rows, vector<bool>(cols, false));
	vector<vector<bool>> explored(rows, vector<bool>(cols, false));

	explored = std::vector<std::vector<bool>>(rows, std::vector<bool>(cols, false)); //matice bunek, ty s hodnotou true jsme prosli
	on_border = std::vector<std::vector<bool>>(rows, std::vector<bool>(cols, false)); //matice bunek, ty s hodnotou true jsou na hranici

	has_solution = false;

	if (!is_valid(start) || !is_valid(end)) //kdyby nebyly souradnice startu a cile platne, nema smysl hledat cestu
	{
		return;
	}

	//pocatecni bunka bude mít parametry na nule
	grid_cells[start.x][start.y].f = 0;
	grid_cells[start.x][start.y].h = 0;
	grid_cells[start.x][start.y].g = 0;
	grid_cells[start.x][start.y].parentX = start.x;
	grid_cells[start.x][start.y].parentY = start.y;

	std::priority_queue<Cell> border;  //fronta bunek na hranici, na vrchu je ta s nejmensi f hodnotou, tzv. "open list"
	border.push(grid_cells[start.x][start.y]);
	on_border[start.x][start.y] = true;

	while (!border.empty())
	{
		Cell curr = border.top(); //bunka s nejmensi f hodnotou, soucasti cesty
		while (!border.empty()) //pokud je curr navstivene, priradi curr dalsi bunku ze fronty
		{
			if (explored[curr.x][curr.y] == true) //již navstiveny prvek vyjmu z fronty
			{
				border.pop();
				curr = border.top();
			}
			else
			{
				break;
			}
		}
		border.pop();

		//z prvku na hranici se stane navstiveny prvek
		on_border[curr.x][curr.y] = false;
		explored[curr.x][curr.y] = true;

		if (curr.x == end.x && curr.y == end.y) //pokud je curr cíl, existuje øešení
		{
			has_solution = true;
			return;
		}

		//projdeme všechny možné sousedy, tj. 8 smìrù
		for (int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <= 1; j++)
			{
				int next_x = curr.x + i;
				int next_y = curr.y + j;

				//nejprve zjistim, jestli ma smysl zkoumat souseda, je-li to cil, prekazka, explored, mimo bludiste

				if (next_x < 0 || next_x >= rows || next_y < 0 || next_y >= cols) // kontroluju souradnice souseda
				{
					continue;
				}
				if ((i == 0 && j == 0) || (grid[next_x][next_y] == 1))   //pokud jsem na pozici curr nebo je tam prekazka
				{
					continue;
				}
				if (explored[next_x][next_y] == true)    //pokud je uz prozkoumany
				{
					continue;
				}

				//spoèítám g a h pro buòku -> dostanu f
				int g_new = 0; //g souseda, tj. dosavadni g od startu ke curr + vzdalenost od curr do next

				if (i == 0 || j == 0)    //sousední prvek je kolmo od buòky curr
				{
					g_new = grid_cells[curr.x][curr.y].g + 10;
				}
				else        //sousední prvek je na diagonále
				{
					g_new = grid_cells[curr.x][curr.y].g + 14;
				}

				if (on_border[next_x][next_y] == false)        //jestli neni prvek uz na hranici
				{
					//do fronty border pridam next, jeho rodice budou souradnice curr, spoctu f = g + h, bude to prvek na hranici
					grid_cells[next_x][next_y].parentX = curr.x;
					grid_cells[next_x][next_y].parentY = curr.y;
					grid_cells[next_x][next_y].g = g_new;
					grid_cells[next_x][next_y].h = heuristic(grid_cells[next_x][next_y], end);
					grid_cells[next_x][next_y].f = grid_cells[next_x][next_y].g + grid_cells[next_x][next_y].h;
					border.push(grid_cells[next_x][next_y]);
					on_border[next_x][next_y] = true;
				}
				else if (g_new < grid_cells[next_x][next_y].g)      //je-li nové g lepsi nez prvku na hranici, pridej potomka next jako vyse
				{
					grid_cells[next_x][next_y].parentX = curr.x;
					grid_cells[next_x][next_y].parentY = curr.y;
					grid_cells[next_x][next_y].g = g_new;
					grid_cells[next_x][next_y].h = heuristic(grid_cells[next_x][next_y], end);
					grid_cells[next_x][next_y].f = grid_cells[next_x][next_y].g + grid_cells[next_x][next_y].h;
					border.push(grid_cells[next_x][next_y]);
					on_border[next_x][next_y] = true;
				}
			}
		}
	}
}

void graph::print()
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			std::cout << grid[i][j] << " ";
		}
		std::cout << std::endl;
	}
	cout << endl;
}

void graph::write_path()
{
	for (size_t i = path.size(); i > 0; i--) //zpetne iteruje proslou cestu
	{
		cout << "(" << path[i - 1].x << "," << path[i - 1].y << ")" << " ";
	}
	cout << endl;
}

void graph::make_path(Cell start, Cell end)
{
	int x = end.x;
	int y = end.y;

	while (x != start.x || y != start.y) //zacina v cili a postupuje smerem ke startu pomoci odkazu na rodice
	{
		path.push_back(grid_cells[x][y]);	//prida aktualni bunku do vektoru path
		int parentX = grid_cells[x][y].parentX;
		int parentY = grid_cells[x][y].parentY;
		x = parentX;
		y = parentY;
	}
	path.push_back(grid_cells[start.x][start.y]); //prida startovni bunku nakonec
}

void graph::read_graph(const std::string& name)
{
	std::ifstream file;
	file.open(name);

	if (file.is_open())
	{
		file >> rows;
		file >> cols;
		vector<int> help;           //sloupec matice grid, tj. bludiste
		vector<Cell> cells_help;    //uklada bunky jednoho sloupce do grid_cells
		int pom;
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				file >> pom;
				help.push_back(pom);

				Cell c{};     //nova bunka pro grid_cells, rodice prozatím mimo, f,g,h na nule
				c.x = i;
				c.y = j;
				c.parentX = -1;
				c.parentY = -1;
				c.f = 0;
				c.g = 0;
				c.h = 0;

				cells_help.push_back(c); //bunku prida do cells_help, tak vznikne sloupec
			}
			grid.push_back(help); //prida sloupec do bludiste
			grid_cells.push_back(cells_help); //prida sloupec do matice grid_cells
			help.clear();
			cells_help.clear();
		}
	}
	else
	{
		has_solution = false;
		return;
	}
}