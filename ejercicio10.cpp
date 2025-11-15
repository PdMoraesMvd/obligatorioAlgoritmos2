#include <cassert>
#include <string>
#include <iostream>
#include <climits>

using namespace std;

// variable global para guardar la mejor solucion
int cantMinPasos = INT_MAX;
int mejorFC;

bool mejorSolucion(int paso)
{
    return paso < cantMinPasos;
}

bool esSolucion(string actual, string productoBuscado)
{
    return actual == productoBuscado;
}

bool esPasoValido(int fila, int columna, int cantF, int cantC, string producto, string **tabla)
{
    return (fila >= 0 && columna >= 0 && fila < cantF && columna < cantC && (tabla[fila][columna] == "C" || tabla[fila][columna] == producto));
}

void aplicarMovimiento(string **tabla, int filaAc, int colAc)
{
    tabla[filaAc][colAc] = "*";
}

void deshacerMovimiento(string **tabla, int filaAc, int colAc)
{
    tabla[filaAc][colAc] = "C";
}

bool poda(int pasosActuales)
{
    return pasosActuales > cantMinPasos;
}

void backtracking(string **tabla, int filaAct, int colAct, int FC, string producto, int cantF, int cantC, int paso)
{
    // poda
    if (poda(paso))
        return;
    if (esSolucion(tabla[filaAct][colAct], producto))
    {
        if (mejorSolucion(paso))
        {
            cantMinPasos = paso;
            mejorFC = FC;
        }
    }
    else
    {
        // for para los pasos posibles
        int df[4] = {-1, 0, 0, 1};
        int dc[4] = {0, -1, 1, 0};
        for (int i = 0; i < 4; i++)
            // si es paso valido
            if (esPasoValido(filaAct + df[i], colAct + dc[i], cantF, cantC, producto, tabla))
            {
                // lo hago
                aplicarMovimiento(tabla, filaAct, colAct);
                // recursiva
                backtracking(tabla, filaAct + df[i], colAct + dc[i], FC, producto, cantF, cantC, paso + 1);
                // deshago
                deshacerMovimiento(tabla, filaAct, colAct);
            }
    }
}

int main()
{
    // TODO
    string productoABuscar;
    cin >> productoABuscar;
    int P;
    cin >> P;
    for (int i = 0; i < P; i++)
    {
        int m;
        int n;
        cin >> m;
        cin >> n;
        string **tabla = new string *[m]();
        for (int j = 0; j < m; j++)
        {
            tabla[j] = new string[n]();
            for (int k = 0; k < n; k++)
            {
                string carac;
                cin >> carac;
                tabla[j][k] = carac;
            }
        }
        tabla[0][0] = "*";
        backtracking(tabla, 0, 0, i + 1, productoABuscar, m, n, 0);
    }
    cout << mejorFC << " " << cantMinPasos;
    return 0;
}