#include <cassert>
#include <string>
#include <iostream>
#include <limits>

using namespace std;

struct nodo
{
    int valor;
    int largoPozo;
    nodo()
    {
        valor = 0;
        largoPozo = 0;
    }
    nodo(int _valor, int _largoPozo)
    {
        valor = _valor;
        largoPozo = _largoPozo;
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

void proceso(nodo *aProcesar, int m, int termina)
{
    MaxHeap *heap = new MaxHeap(m);
    int pos = 1;
    bool llego = false;
    int potencia = 1;
    int cantAyuda = 0;
    while (!llego)
    {
        int valor = aProcesar[pos].valor;
        if (valor == 0) // Avanzo sin hacer nada
        {
            pos++;
        }
        else if (valor > 0) // Vecino que ofrece ayuda
        {
            heap->agregar(valor);
            pos++;
        }
        else // Aca hay un pozo, hay que ver si podemos saltarlo
        {
            int largo = aProcesar[pos].largoPozo;
            while (!heap->esVacio() && potencia < largo + 1)
            {
                int tope = heap->eliminarTope();
                potencia += tope;
                cantAyuda++;
            }
            if (potencia < largo + 1) // Si todavia no le alcanza incluso tomando todas las ayudas no puede llegar
            {
                cout << "imposible" << endl;
                return;
            }
            else // Si llega, salto el pozo
            {
                pos += largo;
                potencia -= largo + 1;
            }
        }
        if (pos >= termina)
            llego = true;
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
    nodo *arr = new nodo[F + 1]();
    for (int i = 0; i < N; i++)
    {
        int I;
        int D;
        cin >> I;
        cin >> D;
        for (int j = I; j <= D; j++)
        {
            arr[j].largoPozo = D - I + 1;
            arr[j].valor = -1;
        }
    }
    for (int i = 0; i < M; i++)
    {
        int x;
        int p;
        cin >> x;
        cin >> p;
        arr[x].largoPozo = 0;
        arr[x].valor = p;
    }
    proceso(arr, M, F);
    delete[] arr;
}