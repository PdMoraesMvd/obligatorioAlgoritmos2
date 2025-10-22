#include <cassert>
#include <string>
#include <iostream>
#include <limits>
#include "./tads/List.h"
#include "./tads/ListImp.cpp"
using namespace std;

class MFSet
{
private:
    int *padre;
    int *altura;
    int tamano;

public:
    //PRE:
    //POS: Crea un MFSET donde cada elemento es su propio representante
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
    //PRE:
    //POS: Devuelve el representante dle conjunto que contiene a x, aplicando path compression
    int find(int x)//busca el representante del conjunto de x
    {
        if (this->padre[x] == x) return x;
        this->padre[x] = find(this->padre[x]);
        return this->padre[x];
    }
    //PRE:
    //POS: Une los conjuntos que incliyen x e y, aplicnado union by rank. Si ya pertenecer al mismo conjunto no hace nada
    void merge(int x, int y)//une los conjuntos que incluyen a x e y
    {
        int padreX = find(x);
        int padreY = find(y);
        if (padreX == padreY)
            return;
        //evaluar alturas (union by rank)
        if (this->altura[padreY] < this->altura[padreX])
        {
            this->padre[padreY] = padreX;
        }
        else if (this->altura[padreY] > this->altura[padreX])
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


struct Arista{
    int origen;
    int peso;
    int destino;
    bool operator==(Arista a){ //nuestra forma para comparar la igualdad entre dos aristas
        return (origen==a.origen && destino == a.destino && a.peso == peso);
    }
};

class Grafo {
    int vertices;
    ListImp<Arista>* aristas;
    int cantAristas;
public:
    //PRE:
    //POS: Crea un grafo
    Grafo(int _vertices) {
        vertices = _vertices;
        aristas = new ListImp<Arista>;
        cantAristas = 0;
    }
    //PRE: 
    //POS: Devuelve la cantidad de vertices del grafo
    int getCantVertices(){
        return this->vertices;
    }
    //PRE:
    //POS: devuelve la cantidad de aristas del grafo
    int getCantAristas(){
        return this->cantAristas;
    }
    //PRE:
    //POS: Devuelve el puntero a la lista de aristas del grafo
    ListImp<Arista>* getAristas(){
        return this->aristas;
    }
    //PRE:
    //POS: Agrega una nueva arista al grafo con los valores dados e incrementa la cantidad de aristas
    void anadirArista(int _peso, int _origen, int _destino){
        Arista nuevo {_origen, _peso, _destino};
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
    //PRE:
    //POS: Devuelve true ssi el peso de a1 es menor estricto al de a2
    bool esMenor(Arista a1, Arista a2){
        return a1.peso < a2.peso;
    }
    //PRE: 
    //POS: Intercambia los elementos array[pos1] y array[pos2]
    void swap(int pos1, int pos2){
        Arista aux=array[pos2];
        array[pos2]=array[pos1];
        array[pos1]=aux;
    }
    //PRE:
    //POS: Ajusta el elemento en pos hacia abajo (LO HUNDE ) hasta que se cumpla la propiedad de orden del heap
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
    //PRE:
    //POS: Ajusta el elemento en pos hacia arriba (lo fLota) hasta que se cumpla la propiedad de orden del heap
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
    //PRE:
    //POS: Crea un heap
        MinHeap(int _capacidad){
            array = new Arista[_capacidad + 1];
            proxLibre=1;
            capacidad=_capacidad;
        }
        //PRE:
        //POS: Devuelve true ssi el heap esta lleno
        bool estaLleno(){
        return proxLibre > capacidad;
        }
        //PRE:
        //POS: Devuelve true ssi el heap esta vacio
        bool esVacio(){
            return proxLibre == 1;
        }
        //PRE: !estaLleno()
        //POS: Inserta el elemento mantendiendo la propiedad de orden del heap
        void agregar(Arista elemento){
            assert(!estaLleno());
            array[proxLibre] = elemento;
            flotar(proxLibre);
            proxLibre++;
        }
        //PRE:
        //POS: Elimina y retorna la arista con menos peso manteniendo la propiedad del minheap
        Arista eliminarTope(){
            assert(!esVacio());
            Arista ret=array[1];
            array[1]=array[--proxLibre];
            hundir(1);
        }
        void destruir(){
            delete[] array;
        }
};
//PRE:
//POS: Devuelve el peso del arbol o bosque de cubrimiento minimo del grafo g
int kruskal(Grafo* g){
    int ret = 0; //el peso total del MST que se va a devolver
    int aristasAgregadas = 0;
    MFSet* conj = new MFSet(g->getCantVertices());
    MinHeap* heap = new MinHeap(g->getCantAristas());
    ListImp<Arista>* aristas = g->getAristas(); //lista de aristas
    for(int i=0; i<g->getCantAristas(); i++){ //inserta todas las aristas en el minheap
        Arista a = aristas->get(i);
        heap->agregar(a);
    }
    while(!heap->esVacio() && aristasAgregadas < g->getCantVertices() - 1){ //se itera hasta formar el MST o cuando el minheap este vacio
        Arista a = heap->eliminarTope();
        int padreOrigen = conj->find(a.origen);
        int padreDestino = conj->find(a.destino);
        if(padreOrigen != padreDestino){ //si no forman ciclo
            conj->merge(padreOrigen, padreDestino);
            ret += a.peso;
            aristasAgregadas++;
        }
    }
    return ret;
}

int main(){
    int V;
    int E;
    cin >> V;
    cin >> E;
    assert( V >= 1);
    assert(V <= 100000);
    assert(E>=0);
    assert(E<=100000);
    Grafo* grafo = new Grafo(V); 
    for(int i=0; i<E; i++){
        int origen; //u
        int destino; //v
        int peso; //w
        cin >> origen; 
        assert(origen>=0);
        cin >> destino; 
        assert(destino<V);
        cin >> peso; 
        assert(peso<100000);
        grafo->anadirArista(peso, origen, destino);
    }
    cout<< kruskal(grafo) << endl;
    return 0;
}