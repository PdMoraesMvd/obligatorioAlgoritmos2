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


class Avl{
    NodoAvl* raiz;
    int cantidadTotal;
    NodoAvl* topUno;
    NodoAvl2* raizPuntaje;
    bool* estaId;

    //PRE:
    //POS: devuelve la altura del nodo, o 0 si no hay nodo
    template <class T>
    int alturaSegura(T* nodo) {
        if(nodo == NULL) return 0;
        return nodo->altura;
    }

    //PRE:
    //POS: actualiza la altura del nodo
    template <class T>
    void actualizarAltura(T* nodo) {
        if(nodo == NULL) return;
        nodo->altura = 1 + max(alturaSegura(nodo->izq), alturaSegura(nodo->der));
    }
    //PRE:
    //POS: devuelve el balance
    template <class T>
    int getBalance(T* nodo) {
        if(nodo == NULL) return 0;
        return alturaSegura(nodo->izq) - alturaSegura(nodo->der);
    }

    //PRE: recibe un puntero por referencia a un nodo NO VACÍO con hijo izquierdo
    //POS: rota a la derecha el subárbol y actualiza alturas
    template <class T>
    void rotacionDerecha(T*& B){
        T* A=B->izq;
        T* T2=A->der;
        B->izq=T2;
        A->der=B;
        actualizarAltura(B);
        actualizarAltura(A);
        B=A;
    }
    //PRE: recibe un puntero por referencia a un nodo NO VACÍO con hijo derecho
    //POS: rota a la izquierda el subarbol y actualiza alturas
    template <class T>
    void rotacionIzquierda(T*& A){
        T* B = A->der;
        T*T2=B->izq; 
        A->der = T2;
        B->izq = A;
        actualizarAltura(A);
        actualizarAltura(B);
        A = B;
    }

    //PRE: 
    //POS: inserta un nuevo jugador en el arbol por ID, actualiza topUno, cantidadTotal y balancea
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

        if(balance > 1 && raiz->izq->id > id) { //caso izquierda izquierda
            rotacionDerecha(raiz);
        }else if( balance > 1 && raiz->izq->id < id) { // casoizquierda derecha
            rotacionIzquierda(raiz->izq);
            rotacionDerecha(raiz);
        }else if(balance < -1 && raiz->der->id < id) { //caso derecha derecha
            rotacionIzquierda(raiz);
        }else if( balance > 1 && raiz->der->id > id) { //caso derecha izquierda
            rotacionDerecha(raiz->der);
            rotacionIzquierda(raiz);
        }
    }
    //PRE: 
    //POS: inserta el puntaje en el árbol o incrementa cantidad si ya existe.Balancea
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
        if(balance > 1 && raiz->izq->puntaje > puntaje) { //caso izquierda izquierda
            rotacionDerecha(raiz);
        }else if( balance > 1 && raiz->izq->puntaje < puntaje) { //caso izquierda derecha
            rotacionIzquierda(raiz->izq);
            rotacionDerecha(raiz);
        }else if(balance < -1 && raiz->der->puntaje < puntaje) { //caso derecha derecha
            rotacionIzquierda(raiz);
        }else if( balance > 1 && raiz->der->puntaje > puntaje) { //caso derecha izquierda
            rotacionDerecha(raiz->der);
            rotacionIzquierda(raiz);
        }
    }
    //PRE: 
    //POS: imprime nombre y puntaje si encuentra el jugador;. Si no existe imprime "jugador_no_encontrado"
    void encontrarRec(NodoAvl* raiz, int id){
        if(raiz==NULL){
            cout << "jugador_no_encontrado" << endl;
        }else if(raiz->id > id){
            encontrarRec(raiz->izq, id);
        }else if(raiz->id < id){
            encontrarRec(raiz->der, id);
        }else{
            cout << raiz->nombre <<  " " << raiz->puntaje << endl;
        }
    }
    //PRE: 
    //POS: devuelve cantidad de jugadores con puntaje >= al que le pasamos en la funcion
    int rankRec(NodoAvl2* raiz, int puntaje){
        if(raiz==NULL) return 0;
        if(raiz->puntaje < puntaje) return rankRec(raiz->der, puntaje);
        if(raiz ->puntaje == puntaje) return raiz->cantidad + rankRec(raiz->der, puntaje);
        if(raiz->puntaje > puntaje) return raiz->cantidad + rankRec(raiz->izq, puntaje)+rankRec(raiz->der, puntaje);
    }
    public:
    //PRE
    //POS: inicializamos un Avl vacío con estructuras auxiliares
    Avl(){
        raiz=NULL;
        topUno=NULL;
        cantidadTotal=0;
        raizPuntaje=NULL;
        estaId=new bool[100000000](); //creamos un array booleano con todos los id posibles
    }
    //PRE: 1 <= id <= 100000000, 0 <= puntaje <= 1000000.
    //POS: inserta un nuevo jugador con id unico, nombre y puntaje. Actualiza lo necesario para dejar consistencia
    void add(int id, string nombre, int puntaje){
        assert(1<= id <= 100000000);
        assert(0<= puntaje <= 1000000);
        if(!estaId[id]){
            insertarRec(this->raiz, id, nombre, puntaje);
            insertarRec2(this->raizPuntaje, puntaje);
        } 
    }
    //PRE: 1 <= id <= 100000000
    //POS: imprime información del jugador con ese id o "sin_jugadores" si no existe
    void find(int id){
        assert(1<= id <= 100000000);
        encontrarRec(this->raiz, id);
    }
    //PRE: 
    //POS: imprime al jugador con mayor puntaje (menor id si empatan), o "sin_jugadores" si no hay jugadores
    void top1(){
        if(topUno == NULL){
            cout << "sin_jugadores" << endl;
        }else{
            cout << topUno->nombre << " " << topUno->puntaje << endl;
        }
    }
    //PRE:
    //POS: imprime la cantidad total de jugadores registrados
    void count(){
        cout << cantidadTotal << endl;
    }
    //PRE: 0 <= puntaje <= 1000000
    //POS: imprime la cantidad de jugadores con puntaje >= al del pasado por parametro
    void rank(int puntaje){
        assert(0<=puntaje<=1000000);
        cout << rankRec(raizPuntaje, puntaje) << endl;
    }
};
int main()
{
    Avl* arbol = new Avl();
    int n;
    cin >> n;

    for(int i=0;i<n;i++){
        int id;
        string nombre;
        int puntaje;
        string operacion;
        cin >> operacion;
        if(operacion == "add" || operacion == "ADD"){
            cin >> id;
            cin >> nombre;
            cin >> puntaje;
            arbol->add(id,nombre, puntaje);
        }else if(operacion == "top1" || operacion == "TOP1"){
            arbol->top1();
        }else if(operacion == "count" || operacion == "COUNT"){
            arbol->count();
        }else if(operacion == "find" || operacion == "FIND"){
            cin >> id;
            arbol->find(id);
        }else if(operacion == "rank" || operacion == "RANK"){
            cin >> puntaje;
            arbol->rank(puntaje);
        }else{
            cout << "no ingresaste bien la operacion, perdiste un intento" << endl; 
        }
    }
    delete arbol;
    
}