#include <cassert>
#include <string>
#include <iostream>
#include <limits>

using namespace std;

struct nodoAyuda
{
    int valor;
    int posicion;
    nodoAyuda()
    {
        valor = 0;
        posicion = 0;
    }
    nodoAyuda(int _valor, int _posicion)
    {
        valor = _valor;
        posicion = _posicion;
    }
};

struct nodoPozo
{
    int inicio;
    int largo;
    nodoPozo()
    {
        inicio = 0;
        largo = 0;
    }
    nodoPozo(int _inicio, int _largo)
    {
        inicio = _inicio;
        largo = _largo;
    }
};

class MaxHeap
{

    int *array;
    int proxLibre;
    int capacidad;
    // PRE:
    // POS: Intercambia los elementos array[pos1] y array[pos2]
    void swap(int pos1, int pos2)
    {
        int aux = array[pos2];
        array[pos2] = array[pos1];
        array[pos1] = aux;
    }
    // PRE:
    // POS: Ajusta el elemento en pos hacia abajo (LO HUNDE ) hasta que se cumpla la propiedad de orden del heap
    void hundir(int pos)
    {
        int posHijoIzq = pos * 2;
        int posHijoDer = (pos * 2) + 1;
        if (posHijoIzq < proxLibre)
        {
            int posHijoMayor = posHijoIzq;
            if (posHijoDer < proxLibre && array[posHijoDer] > array[posHijoIzq])
            {
                posHijoMayor = posHijoDer;
            }
            if (array[posHijoMayor] > array[pos])
            {
                swap(posHijoMayor, pos);
                hundir(posHijoMayor);
            }
        }
    }
    // PRE:
    // POS: Ajusta el elemento en pos hacia arriba (lo fLota) hasta que se cumpla la propiedad de orden del heap
    void flotar(int pos)
    {
        if (pos > 1)
        {
            int posPadre = pos / 2;
            if (array[pos] > array[posPadre])
            {
                swap(pos, posPadre);
                flotar(posPadre);
            }
        }
    }

public:
    // PRE:
    // POS: crea un heap
    MaxHeap(int _capacidad)
    {
        array = new int[_capacidad + 1];
        proxLibre = 1;
        capacidad = _capacidad;
    }
    // PRE:
    // POS: Devuelve true ssi el heap esta lleno
    bool estaLleno()
    {
        return proxLibre > capacidad;
    }
    // PRE:
    // POS: Devuelve true ssi el heap esta vacio
    bool esVacio()
    {
        return proxLibre == 1;
    }
    // PRE: !estaLleno()
    // POS: Inserta el elemento mantendiendo la propiedad de orden del heap
    void agregar(int elemento)
    {
        assert(!estaLleno());
        array[proxLibre] = elemento;
        flotar(proxLibre);
        proxLibre++;
    }
    // PRE: !esVacio()
    // POS: Elimina y retorna el menos elemento del minheap, manteniendo la propiedad de orden del mismo
    int eliminarTope()
    {
        assert(!esVacio());
        int ret = array[1];
        array[1] = array[--proxLibre];
        hundir(1);
        return ret;
    }
    // PRE:
    // POS: libera la memoria del array
    void destruir()
    {
        delete[] array;
    }
};

void proceso(nodoAyuda *ayudas, nodoPozo *pozos, int M, int N)
{
    MaxHeap *heap = new MaxHeap(M);
    bool llego = false;
    int posPozos = 0;
    int posAyudas = 0;
    int potencia = 1;
    int cantAyuda = 0;
    while (!llego)
    {
        int inicioPozo = pozos[posPozos].inicio;
        int largoPozo = pozos[posPozos].largo;
        while (posAyudas < M && ayudas[posAyudas].posicion < inicioPozo)
        {
            heap->agregar(ayudas[posAyudas].valor);
            posAyudas++;
        }
        while (!heap->esVacio() && potencia < largoPozo)
        {
            int ayuda = heap->eliminarTope();
            potencia += ayuda;
            cantAyuda++;
        }
        if (potencia < largoPozo)
        {
            cout << "Imposible" << endl;
            return;
        }
        posPozos++;
        if (posPozos == N)
        {
            llego = true;
        }
    }
    cout << cantAyuda << endl;
}

int main()
{
    int N;
    int M;
    int F;
    cin >> N;
    cin >> M;
    cin >> F;
    nodoAyuda *ayudas = new nodoAyuda[M]();
    nodoPozo *pozos = new nodoPozo[N]();
    for (int i = 0; i < N; i++)
    {
        int I; // posicion inicio pozo
        int D; // posicion final pozo
        cin >> I;
        cin >> D;
        pozos[i].inicio = I;
        pozos[i].largo = D - I + 1;
    }
    for (int i = 0; i < M; i++)
    {
        int x; // posicion casa vecino
        int p; // potencia que ofrece
        cin >> x;
        cin >> p;
        ayudas[i].posicion = x;
        ayudas[i].valor = p;
    }
    proceso(ayudas, pozos, M, N);
}