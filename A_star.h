#pragma once
#include <vector>
#include <string>
#include <cmath>

struct Cell
{
	int x, y, parentX, parentY;		//sou�adnice bu�ky a rodi�e
	int f, g, h; //f = g + h, g je vzd�lenost od startu k dan� bu�ce, h je heurestick� vzd�lenost od dan� bu�ky k c�li

	Cell(int x, int y, int parentX = 0, int parentY = 0, int f = 0, int g = 0, int h = 0)
		: x(x), y(y), parentX(parentX), parentY(parentY), f(f), g(g), h(h) {}

	bool operator==(const Cell& coordinates) const //jak p�istupuju k sou�adnic�m bu�ky
	{
		return (x == coordinates.x && y == coordinates.y);
	}
	bool operator<(const Cell& other) const //porovn�n� f hodnot, na vrchu prioritn� fronty bude bu�ka s nejmen�� f
	{
		return f > other.f;
	}
	Cell() = default;
};

class graph
{
private:
	std::vector<std::vector<int>> grid;         //dvourozm�rn� pole, bludi�t�
	std::vector<std::vector<Cell>> grid_cells;  //matice jednotliv�ch bun�k v bludi�ti
	std::vector<Cell> path;		//vektor obsahuj�c� bu�ky cesty od startu k c�li
	int rows, cols;

public:
	graph(int rows, int cols);
	graph(int rows, int cols, bool has_solution, const std::vector<std::vector<int>>& grid, const std::vector<std::vector<Cell>>& grid_cells)
		: rows(rows), cols(cols), has_solution(has_solution), grid(grid), grid_cells(grid_cells) {}
	graph(const graph& src);
	~graph() = default;

	bool has_solution;

	int getRows() { return this->rows; }
	int getCols() { return this->cols; }

	bool is_valid(Cell a);	//jsou-li sou�adnice bu�ky v rozsahu bludi�t�
	int heuristic(const Cell& a, const Cell& end);	//v�po�et heurestiky, hodnoty h
	void search(Cell start_, Cell end_, int m, int n);	//nalezne nejkrat�� cestu od startu do c�le

	void print();							//vyp�e graf
	void make_path(Cell start, Cell end);	//zp�tn� zkonstruuje cestu vedouc� od c�le ke startu
	void write_path();						//vyp�e sou�adnice pro�l�ch bun�k

	void read_graph(const std::string& name); //na�te graf ze souboru txt
};

