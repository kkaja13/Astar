#include <iostream>
#include "A_star.h"
using namespace std;

int main()
{
    graph g(0, 0);
    g.read_graph("graf.txt");
    Cell start(0, 0);
    Cell end(4, 2);
    g.print();
    g.search(start, end);

    if (g.has_solution) //pokud m� �e�en�, vypi� cestu k c�li
    {
        cout << "Cesta nalezena" << endl;
        g.make_path(start, end);
        g.write_path();
    }
    else //pokud nem� �e�en�
    {
        cout << "Cesta nenalezena" << endl;
    }

    return 0;

}