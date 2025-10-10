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

int main(){
        int contador = 0;
        int K;
        cin >> K;
        Nodo** arr = new Nodo*[K];
        for(int i=0;i<K;i++){
            int Ni;
            cin >> Ni;
            arr[i] = NULL;
            for(int j=0 ; j<Ni ; j++){
                int ingresa;
                cin >> ingresa;
                Nodo* dato = new Nodo;
                dato->valor = ingresa;
                dato->sig = arr[i];
                arr[i]=dato;
                contador++;
            }
        }
        MinHeap* heap = new MinHeap(contador);
        for(int i=0; i<K; i++){
            while(arr[i] != NULL){
                heap->agregar(arr[i]->valor);
                Nodo* borro = arr[i];
                arr[i]=arr[i] -> sig;
                delete borro;
            }
        }
        while(!heap->esVacio()){
            cout<< heap->top() << endl;
            heap->eliminarTope();
        }
        heap->destruir();
        for(int i=0; i<K; i++){
            delete arr[i];
        }
        delete[] arr;
}