#include "A_star.h"
#include <iostream>
#include <fstream>

graph::graph(int rows, int cols)
{
    this->rows = rows;
    this->cols = cols;
    has_path = false;

    matrix = vector<vector<int>>(rows, vector<int>(cols, 0));
    cells = vector<vector<Cell>>(rows, vector<Cell>(cols));
    path = vector<Cell>();
}

graph::graph(const graph& src)
{
    this->rows = src.rows;
    this->cols = src.cols;
    this->has_path = src.has_path;
}

bool graph::is_valid(Cell a) const
{
    return ((a.x >= 0) && (a.x < rows) && (a.y >= 0) && (a.y < cols)); //pokud jsou souradnice v poradku, vraci true
}

int graph::heuristic(const Cell& a, const Cell& end)
{
    int dx = abs(a.x - end.x);
    int dy = abs(a.y - end.y);

    int h_straight = 10 * (dx + dy);			//pohyb horizontalni nebo vertikalni
    int h_diag = (14 - 2 * 10) * min(dx, dy);	//diagonalni pohyb, zaporna hodnota -> je levnejsi, ma prednost

    return h_straight + h_diag;
}

void graph::updateBorderCell(int x, int y, int g_new, int h_new, int parentX, int parentY)
{
    cells[x][y].parentX = parentX;
    cells[x][y].parentY = parentY;
    cells[x][y].g = g_new;
    cells[x][y].h = h_new;
    cells[x][y].f = g_new + h_new;
}

void graph::update(vector<vector<bool>> &on_border, priority_queue<Cell, vector<Cell>, greater<>> & frontier,
                          vector<vector<bool>> & visited, Cell curr, Cell end)
{
    //projdeme všechny možné sousedy, tj. 8 směrů
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            int next_x = curr.x + i;
            int next_y = curr.y + j;

            //nejprve zjistim, jestli ma smysl zkoumat souseda, je-li to prekazka, visited, mimo bludiste nebo curr

            if (next_x < 0 || next_x >= rows || next_y < 0 || next_y >= cols) // kontroluju souradnice souseda
            {
                continue;
            }
            if ((i == 0 && j == 0) || (matrix[next_x][next_y] == 1))   //pokud jsem na pozici curr nebo je tam prekazka
            {
                continue;
            }
            if (visited[next_x][next_y])    //pokud je uz prozkoumany
            {
                continue;
            }

            //spočítám g a h pro buňku -> dostanu f
            int g_new = 0; //g souseda, tj. dosavadni g od startu ke curr + vzdalenost od curr do next

            if (i == 0 || j == 0)    //sousední prvek je kolmo od buňky curr
            {
                g_new = cells[curr.x][curr.y].g + 10;
            }
            else        //sousední prvek je na diagonále
            {
                g_new = cells[curr.x][curr.y].g + 14;
            }

            int h_new = heuristic(cells[next_x][next_y], end);

            //jestli neni prvek uz na hranici nebo je-li nové g lepsi nez prvku na hranici, pridej potomka next jako vyse
            //do fronty border pridam next, jeho rodice budou souradnice curr, spoctu f = g + h, bude to prvek na hranici
            if (!on_border[next_x][next_y] || g_new < cells[next_x][next_y].g)
            {
                updateBorderCell(next_x, next_y, g_new, h_new, curr.x, curr.y);
                frontier.push(cells[next_x][next_y]);
                on_border[next_x][next_y] = true;
            }
        }
    }
}

void graph::search(Cell start, Cell end)
{
    vector<vector<bool>> on_border(rows, vector<bool>(cols, false)); //matice bunek, ty s hodnotou true jsme prosli
    vector<vector<bool>> visited(rows, vector<bool>(cols, false));   //matice bunek, ty s hodnotou true jsou na hranici

    has_path = false;

    if (!is_valid(start) || !is_valid(end)) //kdyby nebyly souradnice startu a cile platne, nema smysl hledat cestu
    {
        return;
    }

    //pocatecni bunka bude mít parametry na nule
    updateBorderCell(start.x, start.y, 0, 0, start.x, start.y);

    priority_queue<Cell, vector<Cell>, greater<>> frontier;  //fronta bunek na hranici, na vrchu je ta s nejmensi f hodnotou, tzv. "open list"
    frontier.push(cells[start.x][start.y]);
    on_border[start.x][start.y] = true;

    while (!frontier.empty())
    {
        Cell curr = frontier.top(); //bunka s nejmensi f hodnotou, soucasti cesty
        frontier.pop();
        if (visited[curr.x][curr.y])
        {
            continue;
        }

        //z prvku na hranici se stane navstiveny prvek
        on_border[curr.x][curr.y] = false;
        visited[curr.x][curr.y] = true;

        if (curr.x == end.x && curr.y == end.y) //pokud je curr cíl, existuje řešení
        {
            has_path = true;
            return;
        }

        update(on_border, frontier, visited, curr, end);
    }
}

void graph::print()
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
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
        path.push_back(cells[x][y]);	//prida aktualni bunku do vektoru path
        int parentX = cells[x][y].parentX;
        int parentY = cells[x][y].parentY;
        x = parentX;
        y = parentY;
    }
    path.push_back(cells[start.x][start.y]); //prida startovni bunku nakonec
}

void graph::read_graph(const string& name)
{
    ifstream file;
    file.open(name);

    if (file.is_open())
    {
        file >> rows >> cols;

        if (rows <= 0 || cols <= 0)
        {
            has_path = false;
            return;
        }
        matrix = vector<vector<int>>(rows, vector<int>(cols, 0)); //matice k uchování hodnot buněk
        cells = vector<vector<Cell>>(rows, vector<Cell>(cols)); //matice k uchování parametrů buněk

        int value;
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                file >> value;

                if (value != 0 && value != 1) //v matici 0 predstavuje volnou cestu, 1 prekazku
                {
                    has_path = false;
                    file.close();
                    return;
                }
                matrix[i][j] = value;

                Cell c{};     //nova bunka pro grid_cells, rodice prozatím mimo, f,g,h na nule
                c.x = i;
                c.y = j;
                c.parentX = -1;
                c.parentY = -1;
                c.f = 0;
                c.g = 0;
                c.h = 0;

                cells[i][j] = c;
            }
        }
        file.close();
    }
    else
    {
        has_path = false;
        return;
    }
}


