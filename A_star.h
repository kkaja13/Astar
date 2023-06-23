#pragma once
#include <vector>
#include <string>
#include <cmath>

struct Cell
{
	int x, y, parentX, parentY;		//souradnice bunky a rodice
	int f, g, h; //f = g + h, g je vzdalenost od startu k dane bunce, h je heuristicka vzdalenost od dane bunky k cili

	Cell(int x, int y, int parentX = 0, int parentY = 0, int f = 0, int g = 0, int h = 0)
		: x(x), y(y), parentX(parentX), parentY(parentY), f(f), g(g), h(h) {}

	bool operator<(const Cell& other) const //porovnani f hodnot, na vrchu prioritni fronty bude bunka s nejmensim f
	{
		return f > other.f;
	}
	Cell() = default;
};

class graph
{
private:
	std::vector<std::vector<int>> grid;         //dvourozmerne pole, bludiste
	std::vector<std::vector<Cell>> grid_cells;  //matice jednotlivych bunek v bludisti
	std::vector<Cell> path;		//vektor obsahujici bunky cesty od startu do cile
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

	bool is_valid(Cell a);	//jsou-li souradnice bunky v rozsahu bludište
	int heuristic(const Cell& a, const Cell& end);	//vypocet heurestiky, hodnoty h
	void search(Cell start_, Cell end_, int m, int n);	//nalezne nejkratsi cestu od startu do cíle

	void print();							//vypise graf (matici)
	void make_path(Cell start, Cell end);	//zpetne zkonstruuje cestu vedouci od cile ke startu
	void write_path();						//vypise souradnice proslych bunek

	void read_graph(const std::string& name); //nacte graf ze souboru txt
};

