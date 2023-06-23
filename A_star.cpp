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
	if ((a.x >= 0) && (a.x < rows) && (a.y >= 0) && (a.y < cols)) //pokud jsou souøadnice v poøádku, vrací true
		return true;
	else
		return false;
}

int graph::heuristic(const Cell& a, const Cell& end) 
{
	//Diagonal distance
	int dx = abs(a.x - end.x);
	int dy = abs(a.y - end.y);

	int h_straight = 10 * max(dx, dy);			//pohyb horizontální nebo vertikální
	int h_diag = (14 - 2 * 10) * min(dx, dy);	//diagonální pohyb, záporná hodnota -> je levnìjší, má pøednost

	return h_straight + h_diag;
}

void graph::search(Cell start, Cell end, int m, int n)
{
	vector<vector<bool>> on_border(m, vector<bool>(n, false));
	vector<vector<bool>> explored(m, vector<bool>(n, false));

	explored = std::vector<std::vector<bool>>(rows, std::vector<bool>(cols, false)); //matice bunìk, ty s hodnotou true už jsme prošli
	on_border = std::vector<std::vector<bool>>(rows, std::vector<bool>(cols, false)); //matice bunìk, ty s hodnotou true jsou na hranici

	has_solution = false;

	if (!is_valid(start) || !is_valid(end)) //kdyby nebyly souøadnice startu a cíle platné, nemá smysl hledat cestu
	{
		return;
	}

	//poèáteèní buòka bude mít parametry na nule
	grid_cells[start.x][start.y].f = 0;
	grid_cells[start.x][start.y].h = 0;
	grid_cells[start.x][start.y].g = 0;
	grid_cells[start.x][start.y].parentX = start.x;
	grid_cells[start.x][start.y].parentY = start.y;

	std::priority_queue<Cell> border;  //fronta bunìk na hranici, na vrchu je ta s nejmenší f hodnotou, tzv. "open list"
	border.push(grid_cells[start.x][start.y]);
	on_border[start.x][start.y] = true;

	while (!border.empty())
	{
		Cell curr = border.top(); //buòka s nejmenším f, souèástí cesty
		while (!border.empty()) //pokud je curr navštívené, pøiøadí curr další buòku ze fronty
		{
			if (explored[curr.x][curr.y] == true) //již navštívený prvek vyjmu z fronty
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

		//z prvku na hranici se stane navštívený prvek
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

				//nejdøív zjistím, jestli má smysl zkoumat souseda, je-li to cíl, pøekážka, explored, mimo bludištì

				if (next_x < 0 || next_x >= m || next_y < 0 || next_y >= n) // kontroluju souøadnice souseda
				{
					continue;
				}
				if ((i == 0 && j == 0) || (grid[next_x][next_y] == 1))   //pokud jsem na pozici curr nebo je tam pøekážka
				{
					continue;
				}
				if (explored[next_x][next_y] == true)    //pokud je už prozkoumaný
				{
					continue;
				}

				//spoèítám g a h pro buòku -> dostanu f
				int g_new = 0; //g souseda

				if (i == 0 || j == 0)    //sousední prvek je kolmo od buòky curr
				{
					g_new = grid_cells[curr.x][curr.y].g + 10;
				}
				else        //sousední prvek je na diagonále
				{
					g_new = grid_cells[curr.x][curr.y].g + 14;
				}

				if (on_border[next_x][next_y] == false)        //jestli není prvek už na hranici
				{
					//do fronty border pøidám next, jeho rodièe jsou souøadnice curr, spoètu f = g + h, bude to prvek na hranici
					grid_cells[next_x][next_y].parentX = curr.x;
					grid_cells[next_x][next_y].parentY = curr.y;
					grid_cells[next_x][next_y].g = g_new;
					grid_cells[next_x][next_y].h = heuristic(grid_cells[next_x][next_y], end);
					grid_cells[next_x][next_y].f = grid_cells[next_x][next_y].g + grid_cells[next_x][next_y].h;
					border.push(grid_cells[next_x][next_y]);
					on_border[next_x][next_y] = true;
				}
				else if (g_new < grid_cells[next_x][next_y].g)      //jestli je nové g lepší než prvku na hranici, pøidej potomka next jako výše
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
	for (size_t i = path.size(); i > 0; i--) //zpìtnì iteruje našlou cestu, tj. prošlé buòky
	{
		cout << "(" << path[i - 1].x << "," << path[i - 1].y << ")" << " ";
	}
	cout << endl;
}

void graph::make_path(Cell start, Cell end)
{
	int x = end.x;
	int y = end.y;

	while (x != start.x || y != start.y) //zaèíná v cíli a postupuje smìrem ke startu pomocí odkazu na rodièe
	{
		path.push_back(grid_cells[x][y]);	//pøidá aktuální buòku do vektoru path
		int parentX = grid_cells[x][y].parentX;
		int parentY = grid_cells[x][y].parentY;
		x = parentX;
		y = parentY;
	}
	path.push_back(grid_cells[start.x][start.y]); //pøidá startovní buòku nakonec
}

void graph::read_graph(const std::string& name)
{
	std::ifstream file;
	file.open(name);

	if (file.is_open())
	{
		file >> rows;
		file >> cols;
		vector<int> help;           //sloupec matice grid, tj. bludištì
		vector<Cell> cells_help;    //ukládá buòky jednoho sloupce do grid_cells
		int pom;
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				file >> pom;
				help.push_back(pom);

				Cell c{};     //nová buòka pro grid_cells, rodièe prozatím mimo, f,g,h na nule
				c.x = i;
				c.y = j;
				c.parentX = -1;
				c.parentY = -1;
				c.f = 0;
				c.g = 0;
				c.h = 0;

				cells_help.push_back(c); //buòku pøidá do cells_help, tak vznikne sloupec
			}
			grid.push_back(help); //pøidá sloupec do bludištì
			grid_cells.push_back(cells_help); //pøidá sloupec do matice grid_cells
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