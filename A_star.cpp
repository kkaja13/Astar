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
	if ((a.x >= 0) && (a.x < rows) && (a.y >= 0) && (a.y < cols)) //pokud jsou sou�adnice v po��dku, vrac� true
		return true;
	else
		return false;
}

int graph::heuristic(const Cell& a, const Cell& end) 
{
	//Diagonal distance
	int dx = abs(a.x - end.x);
	int dy = abs(a.y - end.y);

	int h_straight = 10 * max(dx, dy);			//pohyb horizont�ln� nebo vertik�ln�
	int h_diag = (14 - 2 * 10) * min(dx, dy);	//diagon�ln� pohyb, z�porn� hodnota -> je levn�j��, m� p�ednost

	return h_straight + h_diag;
}

void graph::search(Cell start, Cell end, int m, int n)
{
	vector<vector<bool>> on_border(m, vector<bool>(n, false));
	vector<vector<bool>> explored(m, vector<bool>(n, false));

	explored = std::vector<std::vector<bool>>(rows, std::vector<bool>(cols, false)); //matice bun�k, ty s hodnotou true u� jsme pro�li
	on_border = std::vector<std::vector<bool>>(rows, std::vector<bool>(cols, false)); //matice bun�k, ty s hodnotou true jsou na hranici

	has_solution = false;

	if (!is_valid(start) || !is_valid(end)) //kdyby nebyly sou�adnice startu a c�le platn�, nem� smysl hledat cestu
	{
		return;
	}

	//po��te�n� bu�ka bude m�t parametry na nule
	grid_cells[start.x][start.y].f = 0;
	grid_cells[start.x][start.y].h = 0;
	grid_cells[start.x][start.y].g = 0;
	grid_cells[start.x][start.y].parentX = start.x;
	grid_cells[start.x][start.y].parentY = start.y;

	std::priority_queue<Cell> border;  //fronta bun�k na hranici, na vrchu je ta s nejmen�� f hodnotou, tzv. "open list"
	border.push(grid_cells[start.x][start.y]);
	on_border[start.x][start.y] = true;

	while (!border.empty())
	{
		Cell curr = border.top(); //bu�ka s nejmen��m f, sou��st� cesty
		while (!border.empty()) //pokud je curr nav�t�ven�, p�i�ad� curr dal�� bu�ku ze fronty
		{
			if (explored[curr.x][curr.y] == true) //ji� nav�t�ven� prvek vyjmu z fronty
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

		//z prvku na hranici se stane nav�t�ven� prvek
		on_border[curr.x][curr.y] = false;
		explored[curr.x][curr.y] = true;

		if (curr.x == end.x && curr.y == end.y) //pokud je curr c�l, existuje �e�en�
		{
			has_solution = true;
			return;
		}

		//projdeme v�echny mo�n� sousedy, tj. 8 sm�r�
		for (int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <= 1; j++)
			{
				int next_x = curr.x + i;
				int next_y = curr.y + j;

				//nejd��v zjist�m, jestli m� smysl zkoumat souseda, je-li to c�l, p�ek�ka, explored, mimo bludi�t�

				if (next_x < 0 || next_x >= m || next_y < 0 || next_y >= n) // kontroluju sou�adnice souseda
				{
					continue;
				}
				if ((i == 0 && j == 0) || (grid[next_x][next_y] == 1))   //pokud jsem na pozici curr nebo je tam p�ek�ka
				{
					continue;
				}
				if (explored[next_x][next_y] == true)    //pokud je u� prozkouman�
				{
					continue;
				}

				//spo��t�m g a h pro bu�ku -> dostanu f
				int g_new = 0; //g souseda

				if (i == 0 || j == 0)    //sousedn� prvek je kolmo od bu�ky curr
				{
					g_new = grid_cells[curr.x][curr.y].g + 10;
				}
				else        //sousedn� prvek je na diagon�le
				{
					g_new = grid_cells[curr.x][curr.y].g + 14;
				}

				if (on_border[next_x][next_y] == false)        //jestli nen� prvek u� na hranici
				{
					//do fronty border p�id�m next, jeho rodi�e jsou sou�adnice curr, spo�tu f = g + h, bude to prvek na hranici
					grid_cells[next_x][next_y].parentX = curr.x;
					grid_cells[next_x][next_y].parentY = curr.y;
					grid_cells[next_x][next_y].g = g_new;
					grid_cells[next_x][next_y].h = heuristic(grid_cells[next_x][next_y], end);
					grid_cells[next_x][next_y].f = grid_cells[next_x][next_y].g + grid_cells[next_x][next_y].h;
					border.push(grid_cells[next_x][next_y]);
					on_border[next_x][next_y] = true;
				}
				else if (g_new < grid_cells[next_x][next_y].g)      //jestli je nov� g lep�� ne� prvku na hranici, p�idej potomka next jako v��e
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
	for (size_t i = path.size(); i > 0; i--) //zp�tn� iteruje na�lou cestu, tj. pro�l� bu�ky
	{
		cout << "(" << path[i - 1].x << "," << path[i - 1].y << ")" << " ";
	}
	cout << endl;
}

void graph::make_path(Cell start, Cell end)
{
	int x = end.x;
	int y = end.y;

	while (x != start.x || y != start.y) //za��n� v c�li a postupuje sm�rem ke startu pomoc� odkazu na rodi�e
	{
		path.push_back(grid_cells[x][y]);	//p�id� aktu�ln� bu�ku do vektoru path
		int parentX = grid_cells[x][y].parentX;
		int parentY = grid_cells[x][y].parentY;
		x = parentX;
		y = parentY;
	}
	path.push_back(grid_cells[start.x][start.y]); //p�id� startovn� bu�ku nakonec
}

void graph::read_graph(const std::string& name)
{
	std::ifstream file;
	file.open(name);

	if (file.is_open())
	{
		file >> rows;
		file >> cols;
		vector<int> help;           //sloupec matice grid, tj. bludi�t�
		vector<Cell> cells_help;    //ukl�d� bu�ky jednoho sloupce do grid_cells
		int pom;
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				file >> pom;
				help.push_back(pom);

				Cell c{};     //nov� bu�ka pro grid_cells, rodi�e prozat�m mimo, f,g,h na nule
				c.x = i;
				c.y = j;
				c.parentX = -1;
				c.parentY = -1;
				c.f = 0;
				c.g = 0;
				c.h = 0;

				cells_help.push_back(c); //bu�ku p�id� do cells_help, tak vznikne sloupec
			}
			grid.push_back(help); //p�id� sloupec do bludi�t�
			grid_cells.push_back(cells_help); //p�id� sloupec do matice grid_cells
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