#include <cassert>
#include <string>
#include <iostream>
#include <limits>
using namespace std;

int max(int a, int b)
{
    if (a > b)
        return a;
    return b;
}

void mejorPuntaje(int capacidadMB, int capacidadLineas, int cantidadObjetos, int *pesos, int *puntajes, int *cantLineas)
{

    int ***tab = new int **[cantidadObjetos + 1]; // fila de 0 caso base
    for (int ejercicioActual = 0; ejercicioActual <= cantidadObjetos; ejercicioActual++)
    {
        tab[ejercicioActual] = new int *[capacidadMB + 1](); // columna de 0 caso base
        for (int capacidadActual = 0; capacidadActual <= capacidadMB; capacidadActual++)
        {
            tab[ejercicioActual][capacidadActual] = new int[capacidadLineas + 1]();
        }
    }

    for (int ejercicioActual = 1; ejercicioActual <= cantidadObjetos; ejercicioActual++)
    {
        int pesoEjercicioActual = pesos[ejercicioActual - 1];
        int puntajeEjercicioActual = puntajes[ejercicioActual - 1];
        int lineasEjercicioActual = cantLineas[ejercicioActual - 1];
        for (int capacidadMBActual = 0; capacidadMBActual <= capacidadMB; capacidadMBActual++)
        {

            for (int capacidadLineasActual = 0; capacidadLineasActual <= capacidadLineas; capacidadLineasActual++)
            {
                // El ejercicio actual no entra en la capacidad actual
                if (pesoEjercicioActual > capacidadMBActual || lineasEjercicioActual > capacidadLineasActual)
                {
                    // valor maximo sin usar el ejercicio
                    tab[ejercicioActual][capacidadMBActual][capacidadLineasActual] = tab[ejercicioActual - 1][capacidadMBActual][capacidadLineasActual];
                }
                else
                {
                    int valorDeUsarElObjeto = puntajeEjercicioActual + tab[ejercicioActual - 1][capacidadMBActual - pesoEjercicioActual][capacidadLineasActual - lineasEjercicioActual];
                    int valorDeNoUsarElObjeto = tab[ejercicioActual - 1][capacidadMBActual][capacidadLineasActual];
                    tab[ejercicioActual][capacidadMBActual][capacidadLineasActual] = max(valorDeUsarElObjeto, valorDeNoUsarElObjeto);
                }
            }
        }
    }
    cout << tab[cantidadObjetos][capacidadMB][capacidadLineas] << endl;
}

int main()
{
    int cantidadObjetos;
    cin >> cantidadObjetos;
    int capacidadMB;
    int capacidadLineas;
    cin >> capacidadMB;
    cin >> capacidadLineas;
    int *puntajes = new int[cantidadObjetos];
    int *pesos = new int[cantidadObjetos];
    int *lineas = new int[cantidadObjetos];
    for (int i = 0; i < cantidadObjetos; i++)
    {
        int peso;
        int linea;
        int puntaje;
        cin >> peso;
        cin >> linea;
        cin >> puntaje;
        puntajes[i] = puntaje;
        pesos[i] = peso;
        lineas[i] = linea;
    }
    mejorPuntaje(capacidadMB, capacidadLineas, cantidadObjetos, pesos, puntajes, lineas);
    return 0;
}