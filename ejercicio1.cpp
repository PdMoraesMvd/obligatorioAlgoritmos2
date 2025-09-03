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

class Avl{
    NodoAvl* raiz;
    int cantidadTotal;
    NodoAvl* top1;
    int alturaSegura(NodoAvl* nodo) {
        if(nodo == NULL) return 0;
        return nodo->altura;
    }

    void actualizarAltura(NodoAvl* nodo) {
        if(nodo == NULL) return;
        nodo->altura = 1 + max(alturaSegura(nodo->izq), alturaSegura(nodo->der));
    }

    int getBalance(NodoAvl * nodo) {
        if(nodo == NULL) return 0;
        return alturaSegura(nodo->izq) - alturaSegura(nodo->der);
    }

    void rotacionDerecha(NodoAvl*& B){
        NodoAvl* A=B->izq;
        NodoAvl* T2=A->der;
        B->izq=T2;
        A->der=B;
        actualizarAltura(B);
        actualizarAltura(A);
        B=A;
    }

    void rotacionIzquierda(NodoAvl*& A){
        NodoAvl* B = A->der;
        NodoAvl*T2=B->izq; 
        A->der = T2;
        B->izq = A;
        actualizarAltura(A);
        actualizarAltura(B);
        A = B;
    }

    void insertarRec(NodoAvl*& raiz, int id, string nombre, int puntaje){
        if (raiz==NULL){
            raiz=new NodoAvl(puntaje, id, nombre);
            if(cantidadTotal==0){
                this->top1=raiz;
            }else{
                if (this->top1->puntaje<puntaje || (this->top1->puntaje==puntaje && this->top1->id>id)) this->top1=raiz;
            }
            this->cantidadTotal++;
        }else if(raiz->id<id){
            insertarRec(raiz->der, id, nombre, puntaje);
        }else if(raiz->id>id){
            insertarRec(raiz->izq, id, nombre, puntaje);
        }else return;
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
    public:

    void insertar(int id, string nombre, int puntaje){
        insertarRec(this->raiz, id, nombre, puntaje);
    }
};
int main()
{
    
}