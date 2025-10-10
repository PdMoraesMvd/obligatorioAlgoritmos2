#include <cassert>
#include <string>
#include <iostream>
#include <limits>

using namespace std;
struct Nodo{
    int valor;
    Nodo* sig;
};
class MinHeap
{ 

    int* array;
    int proxLibre;
    int capacidad;
    //PRE: 
    //POS: Intercambia los elementos array[pos1] y array[pos2]
    void swap(int pos1, int pos2){
        int aux=array[pos2];
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
            if(posHijoDer < proxLibre && array[posHijoDer] < array[posHijoIzq]){
                posHijoMenor=posHijoDer;
            }
            if(array[posHijoMenor] < array[pos]){
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
            if(array[pos]<array[posPadre]){
                swap(pos, posPadre);
                flotar(posPadre);
            }
        }
    }
    
    public:
        //PRE:
        //POS: crea un heap
        MinHeap(int _capacidad){
            array =new int[_capacidad + 1];
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
        void agregar(int elemento){
            assert(!estaLleno());
            array[proxLibre] = elemento;
            flotar(proxLibre);
            proxLibre++;
        }
        //PRE: !esVacio()
        //POS: Elimina y retorna el menos elemento del minheap, manteniendo la propiedad de orden del mismo
        int eliminarTope(){
            assert(!esVacio());
            int ret=array[1];
            array[1]=array[--proxLibre];
            hundir(1);
            return ret;
        }
        //PRE:
        //POS: libera la memoria del array
        void destruir(){
            delete[] array;
        }
};

int main(){
        int contador = 0; //total de elementos
        int K; //cantidad de listas a leer
        cin >> K;
        assert(K>=1);
        assert(K<=1000);
        Nodo** arr = new Nodo*[K]; //un array de punteros a listas
        for(int i=0;i<K;i++){
            int Ni; //cantidad de elementos en la lista i
            cin >> Ni;
            assert(Ni>=0);
            assert(Ni<=10000);
            arr[i] = NULL;
            for(int j=0 ; j<Ni ; j++){
                int ingresa;
                cin >> ingresa;
                assert(ingresa>= -1000000);
                assert(ingresa<=1000000);
                Nodo* dato = new Nodo;
                dato->valor = ingresa;
                dato->sig = arr[i];
                arr[i]=dato;
                contador++;
            }
        }
        MinHeap* heap = new MinHeap(contador);
        //inserta todos los elementos
        for(int i=0; i<K; i++){
            while(arr[i] != NULL){
                heap->agregar(arr[i]->valor);
                Nodo* borro = arr[i];
                arr[i]=arr[i] -> sig;
                delete borro;
            }
        }
        //se imprime todos los elementos del heap
        while(!heap->esVacio()){
            cout<< heap->eliminarTope() << endl;
        }
        heap->destruir();
        for(int i=0; i<K; i++){
            delete arr[i];
        }
        delete[] arr;
}