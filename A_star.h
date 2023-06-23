#pragma once
#include <vector>
#include <string>
#include <cmath>

struct Cell
{
	int x, y, parentX, parentY;		//souèadnice buòky a rodièe
	int f, g, h; //f = g + h, g je vzdálenost od startu k dané buòce, h je heurestická vzdálenost od dané buòky k cíli

	Cell(int x, int y, int parentX = 0, int parentY = 0, int f = 0, int g = 0, int h = 0)
		: x(x), y(y), parentX(parentX), parentY(parentY), f(f), g(g), h(h) {}

	bool operator==(const Cell& coordinates) const //jak pøistupuju k souøadnicím buòky
	{
		return (x == coordinates.x && y == coordinates.y);
	}
	bool operator<(const Cell& other) const //porovnání f hodnot, na vrchu prioritní fronty bude buòka s nejmenší f
	{
		return f > other.f;
	}
	Cell() = default;
};

class graph
{
private:
	std::vector<std::vector<int>> grid;         //dvourozmìrné pole, bludištì
	std::vector<std::vector<Cell>> grid_cells;  //matice jednotlivých bunìk v bludišti
	std::vector<Cell> path;		//vektor obsahující buòky cesty od startu k cíli
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

	bool is_valid(Cell a);	//jsou-li souøadnice buòky v rozsahu bludištì
	int heuristic(const Cell& a, const Cell& end);	//výpoèet heurestiky, hodnoty h
	void search(Cell start_, Cell end_, int m, int n);	//nalezne nejkratší cestu od startu do cíle

	void print();							//vypíše graf
	void make_path(Cell start, Cell end);	//zpìtnì zkonstruuje cestu vedoucí od cíle ke startu
	void write_path();						//vypíše souøadnice prošlých bunìk

	void read_graph(const std::string& name); //naète graf ze souboru txt
};

