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
    int origen;
    int peso;
    int destino;
};

class Grafo {
    int vertices;
    ListImp<Arista>* aristas;
    int cantAristas;
public:
    Grafo(int _vertices) {
        vertices = _vertices;
        aristas = new ListImp<Arista>;
        cantAristas = 0;
    }

    int getCantVertices(){
        return this->vertices;
    }

    int getCantAristas(){
        return this->cantAristas;
    }

    ListImp<Arista>* getAristas(){
        return this->aristas;
    }

    void añadirArista(int _peso, int _origen, int _destino){
        Arista nuevo {_peso, _origen, _destino};
        aristas->insert(nuevo);
        cantAristas++;
    }
};

class MinHeap
{ 
    Arista* array;
    int proxLibre;
    int capacidad;
    int cantElementosTotales;

    bool esMenor(Arista a1, Arista a2){
        return a1.peso < a2.peso;
    }

    void swap(int pos1, int pos2){
        Arista aux=array[pos1];
        array[pos1]=array[pos2];
        array[pos2]=aux;
    }

    void hundir(int pos){
        int posHijoIzq = pos*2;
        int posHijoDer = (pos*2)+1;
        if(posHijoIzq < proxLibre){
            int posHijoMenor = posHijoIzq;
            if(posHijoDer < proxLibre && esMenor(array[posHijoDer], array[posHijoIzq])){
                posHijoMenor=posHijoDer;
            }
            if(esMenor(array[posHijoMenor], array[pos])){
                swap(posHijoMenor, pos);
                hundir(posHijoMenor);
            }
        }
    }
    
    void flotar(int pos){
        if(pos > 1){
            int posPadre = pos/2;
            if(esMenor(array[pos],array[posPadre])){
                swap(pos, posPadre);
                flotar(posPadre);
            }
        }
    }
    
    public:
        MinHeap(int _capacidad){
            array = new Arista[_capacidad + 1];
            proxLibre=1;
            capacidad=_capacidad;
        }
        bool estaLleno(){
        return proxLibre > capacidad;
        }
        bool esVacio(){
            return proxLibre == 1;
        }
        void agregar(Arista elemento){
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
        Arista top(){
            assert(!esVacio());
            return array[1];
        }
        void destruir(){
            delete[] array;
        }
};

int kruskal(Grafo* g){
    int ret = 0;
    int aristasAgregadas = 0;
    MFSet* conj = new MFSet(g->getCantVertices());
    MinHeap* heap = new MinHeap(g->getCantAristas());
    ListImp<Arista>* aristas = g->getAristas();
    for(int i=0; i<g->getCantAristas(); i++){
        Arista a = aristas->get(i);
        heap->agregar(a);
    }
    while(!heap->esVacio() || aristasAgregadas == g->getCantVertices() - 1){
        Arista a = heap->top();
        heap->eliminarTope();
        int padreOrigen = conj->find(a.origen);
        int padreDestino = conj->find(a.destino);
        if(padreOrigen != padreDestino){ //si no forman ciclo
            conj->merge(padreOrigen, padreDestino);
            ret += a.peso;
            aristasAgregadas++;
        }
    }

    //ir sacando la arista de menor peso y evaluar padres de ambos con MFSeta hecho
    //Si es el mismo padre entonces no agrego arista hecho
    //Si no tienen mismo padre agrego al mfset y agrego arista al grafo.
    return NULL;
}

int main()
{
    // TODO
    return 0;
}