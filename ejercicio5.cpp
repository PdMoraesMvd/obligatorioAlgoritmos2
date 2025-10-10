#include <cassert>
#include <string>
#include <iostream>
#include <limits>
#include <List.h>
#include <ListImp.cpp>

class MFSet
{
private:
    int *padre;
    int *altura;
    int tamano;

public:
    MFSet(int x)
    {
        this->padre = new int[x + 1];
        this->altura = new int[x + 1]();
        this->tamano = x;
        for (int i = 0; i <= x; i++)
        {
            this->padre[i] = i;
        }
    }

    int find(int x)
    {
        if (this->padre[x] == x)
            return x;
        //path compression
        this->padre[x] = find(this->padre[x]);
        return this->padre[x];
    }

    void merge(int x, int y)
    {
        int padreX = find(x);
        int padreY = find(y);
        if (padreX == padreY)
            return;
        // evaluar alturas (union by rank)
        if (this->altura[padreX] < this->altura[padreY])
        {
            this->padre[padreY] = padreX;
        }
        else if (this->altura[padreX] > this->altura[padreY])
        {
            this->padre[padreX] = padreY;
        }
        else
        {
            this->padre[padreX] = padreY;
            this->altura[padreY]++;
        }
    }
};

using namespace std;
struct Arista{
    int peso;
    int destino;
};

class Grafo {
    int vertices;
    List<Arista>** adyacentes;
public:
    Grafo(int _vertices) : vertices(_vertices) {
        adyacentes = new List<Arista>*[vertices+1];
        for (int i = 1; i < vertices+1; i++) {
            adyacentes[i] = new ListImp<Arista>();
        }
    }
    void añadirArista(int _peso, int origen, int _destino){
        Arista nuevo {_peso, _destino};
        adyacentes[origen]->insert(nuevo);
    }
};

class MinHeap
{ 

    int* array;
    int proxLibre;
    int capacidad;
    int cantElementosTotales;

    void swap(int pos1, int pos2){
        int aux=array[pos1];
        array[pos1]=array[pos2];
        array[pos2]=aux;
    }

    void hundir(int pos){
        int posHijoIzq = pos*2;
        int posHijoDer = (pos*2)+1;
        if(posHijoIzq < proxLibre){
            int posHijoMenor = posHijoIzq;
            if(posHijoDer < proxLibre && array[posHijoDer] < array[posHijoIzq]){
                posHijoMenor=posHijoDer;
            }
            if(array[posHijoMenor] < array[pos]){
                swap(posHijoMenor, pos);
                hundir(posHijoMenor);
            }
        }
    }
    
    void flotar(int pos){
        if(pos > 1){
            int posPadre = pos/2;
            if(array[pos]<array[posPadre]){
                swap(pos, posPadre);
                flotar(posPadre);
            }
        }
    }
    
    public:
        MinHeap(int _capacidad){
            array =new int[_capacidad + 1];
            proxLibre=1;
            capacidad=_capacidad;
        }
        bool estaLleno(){
        return proxLibre > capacidad;
        }
        bool esVacio(){
            return proxLibre == 1;
        }
        void agregar(int elemento){
            assert(!estaLleno());
            array[proxLibre] = elemento;
            flotar(proxLibre);
            proxLibre++;
        }
        void eliminarTope(){
            assert(!esVacio());
            array[1]=array[--proxLibre];
            hundir(1);
        }
        int top(){
            assert(!esVacio());
            return array[1];
        }
        void destruir(){
            delete[] array;
        }
};

Grafo kruskal(Grafo g){
    //TODO
    return NULL;
}

int main()
{
    // TODO
    return 0;
}