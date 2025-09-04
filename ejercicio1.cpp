#include <cassert>
#include <string>
#include <iostream>
#include <limits>

using namespace std;

struct NodoAvl{
    int puntaje;
    int id;
    int altura;
    string nombre;
    NodoAvl* izq;
    NodoAvl* der;
    NodoAvl(int _puntaje, int _id, string _nombre):puntaje(_puntaje), id(_id), altura(1), nombre(_nombre), izq(NULL), der(NULL){}
};

struct NodoAvl2{
    int puntaje;
    int cantidad;
    int altura;
    NodoAvl2* izq;
    NodoAvl2* der;
    NodoAvl2(int _puntaje):puntaje(_puntaje), altura(1), cantidad (1), izq(NULL), der(NULL){}
};

template <class T>
class Avl{
    NodoAvl* raiz;
    int cantidadTotal;
    NodoAvl* topUno;
    NodoAvl2* raizPuntaje;
    bool* estaId;
    int alturaSegura(T* nodo) {
        if(nodo == NULL) return 0;
        return nodo->altura;
    }

    void actualizarAltura(T* nodo) {
        if(nodo == NULL) return;
        nodo->altura = 1 + max(alturaSegura(nodo->izq), alturaSegura(nodo->der));
    }

    int getBalance(T* nodo) {
        if(nodo == NULL) return 0;
        return alturaSegura(nodo->izq) - alturaSegura(nodo->der);
    }

    void rotacionDerecha(T*& B){
        T* A=B->izq;
        T* T2=A->der;
        B->izq=T2;
        A->der=B;
        actualizarAltura(B);
        actualizarAltura(A);
        B=A;
    }

    void rotacionIzquierda(T*& A){
        T* B = A->der;
        T*T2=B->izq; 
        A->der = T2;
        B->izq = A;
        actualizarAltura(A);
        actualizarAltura(B);
        A = B;
    }

    void insertarRec(NodoAvl*& raiz, int id, string nombre, int puntaje){
        if (raiz==NULL){
            raiz=new NodoAvl(puntaje, id, nombre);
            estaId[id]=true;
            if(cantidadTotal==0){
                this->topUno=raiz;
            }else{
                if (this->topUno->puntaje<puntaje || (this->topUno->puntaje==puntaje && this->topUno->id>id)) this->topUno=raiz;
            }
            this->cantidadTotal++;
        }else if(raiz->id<id){
            insertarRec(raiz->der, id, nombre, puntaje);
        }else if(raiz->id>id){
            insertarRec(raiz->izq, id, nombre, puntaje);
        }
        actualizarAltura(raiz);
        int balance=getBalance(raiz);

        // caso izquierda izquierda
        if(balance > 1 && raiz->izq->id > id) {
            rotacionDerecha(raiz);
        }else if( balance > 1 && raiz->izq->id < id) { // caso izquierda derecha
            rotacionIzquierda(raiz->izq);
            rotacionDerecha(raiz);
        }else if(balance < -1 && raiz->der->id < id) { //caso derecha derecha
            rotacionIzquierda(raiz);
        }else if( balance > 1 && raiz->der->id > id) { //caso derecha izquierda
            rotacionDerecha(raiz->der);
            rotacionIzquierda(raiz);
        }
    }
    void insertarRec2(NodoAvl2*& raiz, int puntaje){
        if (raiz==NULL){
            raiz=new NodoAvl2(puntaje);
        }else if(raiz->puntaje<puntaje){
             insertarRec2(raiz->der, puntaje);
        }else if(raiz->puntaje>puntaje){
            insertarRec2(raiz->izq, puntaje);
        }else{
            raiz->cantidad++;
        }
        actualizarAltura(raiz);
        int balance=getBalance(raiz);
        // caso izquierda izquierda
        if(balance > 1 && raiz->izq->puntaje > puntaje) {
            rotacionDerecha(raiz);
        }else if( balance > 1 && raiz->izq->puntaje < puntaje) { // caso izquierda derecha
            rotacionIzquierda(raiz->izq);
            rotacionDerecha(raiz);
        }else if(balance < -1 && raiz->der->puntaje < puntaje) { //caso derecha derecha
            rotacionIzquierda(raiz);
        }else if( balance > 1 && raiz->der->puntaje > puntaje) { //caso derecha izquierda
            rotacionDerecha(raiz->der);
            rotacionIzquierda(raiz);
        }
    }
    void encontrarRec(NodoAvl* raiz, int id){
        if(raiz==NULL){
            cout << "jugador_no_encontrado" << endl;
        }else if(raiz->id > id){
            encontrarRec(raiz->izq, id);
        }else if(raiz->id > id){
            encontrarRec(raiz->der, id);
        }else{
            cout << raiz->nombre <<  " " << raiz->puntaje << endl;
        }
    }
    int rankRec(NodoAvl2* raiz, int puntaje){
        if(raiz==NULL) return 0;
        if(raiz->puntaje < puntaje) return rankRec(raiz->der, puntaje);
        if(raiz ->puntaje == puntaje) return raiz->cantidad + rankRec(raiz->der, puntaje);
        if(raiz->puntaje > puntaje) return raiz->cantidad + rankRec(raiz->izq, puntaje)+rankRec(raiz->der, puntaje);
    }
    public:
    Avl(){
        raiz=NULL;
        topUno=NULL;
        cantidadTotal=0;
        raizPuntaje=NULL;
        estaId=new bool[100000000](); //creamos un array booleano con todos los id posibles
    }

    void add(int id, string nombre, int puntaje){
        assert(1<= id <= 100000000);
        assert(0<= puntaje <= 1000000);
        if(!estaId[id]){
            insertarRec(this->raiz, id, nombre, puntaje);
            insertarRec2(this->raizPuntaje, puntaje);
        } 
    }
    void find(int id){
        assert(1<= id <= 100000000);
        encontrarRec(this->raiz, id);
    }
    void top1(){
        if(topUno == NULL){
            cout << "sin_jugadores" << endl;
        }else{
            cout << topUno->nombre << " " << topUno->puntaje << endl;
        }
    }
    void count(){
        cout << cantidadTotal << endl;
    }
    void rank(int puntaje){
        assert(0<=puntaje<=1000000);
        cout << rankRec(raizPuntaje, puntaje) << endl;
    }
};
int main()
{
    int n;
    cout << "ingrese la cantidad de comandos a ejecutar" << endl;
    cin >> n;
    for(int i=0;i<n;i++){
        
    }
    
}