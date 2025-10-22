#include <cassert>
#include <string>
#include <iostream>
#include <limits>

using namespace std;

struct nodoHash{ //es la tabla de hash principal, que tiene como clave el dominio y el path
    string dominio;
    string path;
    string titulo;
    nodoHash* sig;
    int tiempo;
};


struct listaPath{ //representa un nodo, utilizamos una lista dobelemente enlazada para poder manejar los ordenes pedidos
    string path;
    int tiempo;
    string titulo;
    listaPath *sig; //nodo siguiente
    listaPath *prev; //nodo anterior
};

struct nodoHash2{
    string dominio;
    listaPath *lista; //lista de paths de ese dominio
    int cantidad; //numero de recursos del dominio
    nodoHash2* sig;
};

class cache{
    int capacidad;
    nodoHash** hash; //ordenado por clave (dominio + path)
    nodoHash2** hash2; //ordenado por clave (dominio)
    int cantidad; //cantidad de recursos totales en el cache
    //PRE:
    //POS: devuelve un entero
    int h1(string clave){ //primera funcion de hash. Sacada del curso teórico (ver documentacion)
        int h = 0;
        for (int i = 0; i < clave.length(); i++)
        {
            h = 31 * h + int(clave[i]);
        }
        return h;
    }

    //PRE: intento es un número entero positivo
    //POS: devuelve un índice válido en el rango 0,capacidad-1
    int pos(string clave){ //usamos nuestras dos funciones de hash para calcular la posicion
        unsigned int uno = h1(clave);
        unsigned int indice =  uno % capacidad;
        return indice; 
    }

    //PRE:  parametro es de hasta 50 caracteres (sin espacios)
    //POS: devuelve true si parametro contiene espacios, false en otro caso
    bool tieneEspacio(string parametro){
        for(int i=0;i<parametro.length(); i++){
            if(parametro[i] == ' ') return true;
        }
        return false;
    }

    //PRE:
    //POS: elimina o no nodo de la lista l, liberando memoria
    void borrarNodo(listaPath*& l, listaPath* nodo){
        if (nodo == NULL) return; 

        if (nodo->prev != NULL) {
            nodo->prev->sig = nodo->sig;
        } else {
            l = nodo->sig;
        }

        if (nodo->sig != NULL) {
            nodo->sig->prev = nodo->prev;
        }

        delete nodo;
    }
    //PRE: l existe en la lista
    //POS: borra el nodo de la lista y la deja consistente
    void borrarNodo2(string dominio, string path, nodoHash* &l){ 
        nodoHash* aux = l;
        if (l->dominio == dominio && l->path == path) {
            nodoHash* aBorrar = l;
            l = l->sig;          
            delete aBorrar;
            return;
        }
       
        
        while(( aux -> sig != NULL) && (aux->sig->dominio != dominio || aux -> sig ->path != path)) aux = aux->sig;
        nodoHash* aBorrar = aux->sig;
        aux->sig = aux->sig->sig;
        delete aBorrar;
    }
    public:

    //PRE: 1<= capacidad <=1000000
    //POS: crea un cache correctamente inicializado
    cache(int capacidad){
        assert((1<= capacidad) && (capacidad <=1000000));
        this->capacidad=capacidad;
        hash = new nodoHash *[capacidad]; 
        hash2 = new nodoHash2*[capacidad];
        for(int i=0; i < capacidad; i++){
            hash[i]=NULL;
            hash2[i]=NULL;
        }
        cantidad = 0;
    }
    //PRE: dominio es de hasta 50 caracteres (sin espacios), path es de hasta 50 caracteres (sin espacios), titulo es de hasta 50 caracteres (sin espacios), 1 <= tiempo <= 10^9
    //POS: inserta un recurso nuevo o actualiza su título y tiempo si ya esta
    void put(string dominio, string path, string titulo, int tiempo){
        assert(dominio.length() <= 50 && !tieneEspacio(dominio));
        assert(path.length() <= 50 && !tieneEspacio(path));
        assert(titulo.length() <= 50 && !tieneEspacio(titulo));
        assert(1 <= tiempo && tiempo <= 1000000000);

        string clave1 = dominio + "|" + path;
        string clave2 = dominio;
        int posicion1 = pos(clave1);
        int posicion2 = pos(clave2);

        nodoHash* aux = hash[posicion1];
        nodoHash2* aux2 = hash2[posicion2];

        // buscar si ya existe en hash principal
        while(aux != NULL){
            if(aux->dominio == dominio && aux->path == path){
                // actualizar en hash
                aux->tiempo = tiempo;
                aux->titulo = titulo;

                // buscar en lista del dominio
                while(aux2 != NULL && aux2->dominio != dominio) aux2 = aux2->sig;
                if(aux2 != NULL){
                    listaPath* nodo = aux2->lista;
                    while(nodo != NULL && nodo->path != path) nodo = nodo->sig;
                    if(nodo != NULL){
                        nodo->tiempo = tiempo;
                        nodo->titulo = titulo;
                        if(nodo != aux2->lista){
                            if(nodo->prev) nodo->prev->sig = nodo->sig;
                            if(nodo->sig)  nodo->sig->prev  = nodo->prev;
                            nodo->sig = aux2->lista;
                            aux2->lista->prev = nodo;
                            nodo->prev = NULL;
                            aux2->lista = nodo;
                        }
                    }
                }
                return;
            }
            aux = aux->sig;
        }

       
        nodoHash* nodo = new nodoHash;  //si no existia: crear nodo en hash principal
        nodo->dominio = dominio;
        nodo->titulo  = titulo;
        nodo->tiempo  = tiempo;
        nodo->path    = path;
        nodo->sig     = hash[posicion1];
        hash[posicion1] = nodo;

        
        aux2 = hash2[posicion2];
        while(aux2 != NULL && aux2->dominio != dominio) aux2 = aux2->sig;

        if(aux2 == NULL){
            nodoHash2* nuevo = new nodoHash2;//dominio no existente
            nuevo->dominio = dominio;
            nuevo->cantidad = 1;
            nuevo->sig = hash2[posicion2];

            listaPath* nodo = new listaPath; //crear lista con un único path
            nodo->path = path;
            nodo->titulo = titulo;
            nodo->tiempo = tiempo;
            nodo->sig = NULL;
            nodo->prev = NULL;
            nuevo->lista = nodo;
            hash2[posicion2] = nuevo;
        } else {
            listaPath* nodo = new listaPath;//dominio ya existe: insertar al inicio
            nodo->path = path;
            nodo->titulo = titulo;
            nodo->tiempo = tiempo;
            nodo->prev = NULL;
            nodo->sig = aux2->lista;

            if(aux2->lista != NULL) aux2->lista->prev = nodo;
            aux2->lista = nodo;
            aux2->cantidad++;
        }

        cantidad++;
    }
    //PRE:dominio es de hasta 50 caracteres (sin espacios), path es de hasta 50 caracteres (sin espacios)
    //POS: imprime {titulo} {tiempo} si el recurso existe, o "recurso_no_encontrado" si no existe
    void get(string dominio, string path){
        assert(dominio.length() <= 50 && !tieneEspacio(dominio));
        assert(path.length() <= 50 && !tieneEspacio(path));
        string clave = dominio + "|" + path;
        int posicion = pos(clave);
        nodoHash* aux = hash[posicion];
        while(aux != NULL){
            if(aux ->dominio == dominio && aux->path == path){
                cout << aux->titulo << " " << aux->tiempo << endl;
                return;
            }else{
                aux = aux ->sig;
            }
        }
        cout << "recurso_no_encontrado" << endl;
    }
    //PRE:dominio es de hasta 50 caracteres (sin espacios), path es de hasta 50 caracteres (sin espacios)
    //POS: si existe el recurso, lo elimina, si no, no hace nada
    void remove(string dominio, string path){
        assert(dominio.length() <= 50 && !tieneEspacio(dominio));
        assert(path.length() <= 50 && !tieneEspacio(path));
        string clave=dominio;
        int posicion = pos(clave);
        nodoHash2* aux = hash2[posicion];
        nodoHash2* anterior=NULL;
        while(aux!=NULL && aux->dominio != dominio){
            anterior=aux;
            aux=aux->sig;
        } 
        if(aux != NULL){
            listaPath* aux2 = aux->lista;
            while(aux2 != NULL && aux2 ->path!=path) aux2 = aux2 ->sig;
            if(aux2 != NULL){
                borrarNodo(aux->lista, aux2);
                aux->cantidad--;
                if(aux->cantidad==0){
                    if(anterior==NULL){
                        hash2[posicion]=hash2[posicion]->sig;
                    }else{
                        anterior->sig=aux->sig;
                    }
                    delete aux;
                }
                cantidad--;
                string clave2 = dominio + "|" + path;
                posicion=pos(clave2);
                borrarNodo2(dominio, path, hash[posicion]);
            }
        }
        
    }
    //PRE:dominio es de hasta 50 caracteres (sin espacios), path es de hasta 50 caracteres (sin espacios)
    //POS: imprime "true" si existe, "false" E.O.C (en otro caso)
    void contains(string dominio, string path){
        string clave = dominio + "|" + path;
        int posicion = pos(clave);
        nodoHash* aux = hash[posicion];
        while(aux!=NULL && (aux->dominio != dominio || aux->path != path)) aux=aux->sig;
        if(aux!=NULL){
            cout << "true" <<endl;
        }else{
            cout << "false" << endl;
        }
    }
    //PRE:dominio es de hasta 50 caracteres (sin espacios)
    //POS: imprime todos los recursos del dominio o vacio si no hay ni 1
    void list_domain(string dominio){
        string clave = dominio;
        int posicion = pos(clave);
        nodoHash2* aux = hash2[posicion];
        while(aux!=NULL && aux->dominio != dominio) aux=aux->sig;
        if(aux == NULL || (aux!= NULL && aux->lista == NULL)){
            cout << endl;
        }else{
            listaPath* aux2 = aux->lista;
            while(aux2->sig != NULL){
                cout << aux2->path << " ";
                aux2= aux2->sig;
            }
            cout << aux2->path << endl;
        }
    }
    //PRE:
    //POS: imprime la cantidad total de recursos del cache
    void size(){
        cout << cantidad << endl;
    }
    //PRE:
    //POS: elimina todos los recursos del dominio y "resetea" todos los atributos correspondientes
    void clear(){
        for(int i=0;i<capacidad;i++){
            while(hash[i]!=NULL){
                nodoHash* aBorrar=hash[i];
                hash[i]=hash[i]->sig;
                delete aBorrar;
            }
            while(hash2[i]!=NULL){
                nodoHash2* aBorrar=hash2[i];
                hash2[i]=hash2[i]->sig;
                while(aBorrar->lista != NULL){
                    listaPath* aBorrar2 = aBorrar->lista;
                    aBorrar->lista=aBorrar->lista->sig;
                    delete aBorrar2;
                }
                delete aBorrar;
            }
        }
        cantidad=0;
    }
    //PRE:dominio es de hasta 50 caracteres (sin espacios)
    //POS: imprime la cantidad de recursos de un dominio especifico
    void count_domain(string dominio){
        int posicion=pos(dominio);
        nodoHash2* aux=hash2[posicion];
        while(aux!=NULL&&aux->dominio!=dominio){
            aux=aux->sig;
        }
        if(aux==NULL ){
            cout << 0 << endl;
        }else{
            cout << aux->cantidad << endl;
        }
    }
    //PRE:dominio es de hasta 50 caracteres (sin espacios)
    //POS: elimina todos los recursos de dominio o nada si no existe o no tiene
    void clear_domain(string dominio){
        assert(dominio.length() <= 50 && !tieneEspacio(dominio));
        int posicion = pos(dominio);
        nodoHash2* actual = hash2[posicion];
        nodoHash2* anterior = NULL;

        while (actual != NULL && actual->dominio != dominio) {
            anterior = actual;
            actual = actual->sig;
        }
        if (actual == NULL) return; 

        listaPath* aux = actual->lista; //busco el recurso en la tabla 1 y lo elimino
            while (aux != NULL) {
                string clave1 = dominio + "|" + aux->path;
                int posicion2 = pos(clave1);
                borrarNodo2(dominio, aux->path, hash[posicion2]);  //sabemos que existe
                listaPath* aBorrar = aux;
                aux = aux->sig;
                delete aBorrar;
                cantidad--;
                actual->cantidad--;
            }
            actual->lista = NULL;

            if (anterior == NULL) {
                hash2[posicion] = actual->sig;   //era el primero
            } else {
                anterior->sig = actual->sig;       // saltear el actual
            }
    }
    //PRE: 
    //POS: Libera toda la memoria del cache
    void destruir(){
        for(int i=0; i<capacidad; i++){
            while(hash[i] != NULL){
                nodoHash* aBorrar = hash[i];
                hash[i] = hash[i]->sig;
                delete aBorrar;
            }
        }
        for(int i=0; i<capacidad; i++){
            while(hash2[i] != NULL){
                nodoHash2* aBorrar = hash2[i];
                hash2[i] = hash2[i]->sig;
                while(aBorrar->lista != NULL){
                    listaPath* aBorrar2 = aBorrar->lista;
                    aBorrar->lista = aBorrar->lista->sig;
                    delete aBorrar2;
                }
                delete aBorrar;
            }
        }
        delete[] hash;
        delete[] hash2;
        hash2 = NULL;
        hash = NULL;
        capacidad = 0;
        cantidad = 0;
    }
};
int main(){
    int n;
    cin >> n;
    cache* cachee = new cache(n*2);
    for(int i=0;i<n;i++){
        string dominio;
        string path;
        string titulo;
        int tiempo;
        string operacion;
        cin >> operacion;
        if(operacion == "put" || operacion == "PUT"){
            cin >> dominio;
            cin >> path;
            cin >> titulo;
            cin >> tiempo;
            cachee->put(dominio, path, titulo, tiempo);
        }else if(operacion == "get" || operacion == "GET"){
            cin >> dominio;
            cin >> path;
            cachee->get(dominio, path);
        }else if(operacion == "remove" || operacion == "REMOVE"){
            cin >> dominio;
            cin >> path;
            cachee->remove(dominio, path);
        }else if(operacion == "contains" || operacion == "CONTAINS"){
            cin >> dominio;
            cin >> path;
            cachee->contains(dominio,path);
        }else if(operacion == "count_domain" || operacion == "COUNT_DOMAIN"){
            cin >> dominio;
            cachee->count_domain(dominio);
        }else if(operacion == "list_domain" || operacion == "LIST_DOMAIN"){
            cin >> dominio;
            cachee->list_domain(dominio);
        }else if(operacion == "clear_domain" || operacion == "CLEAR_DOMAIN"){
            cin >> dominio;
            cachee->clear_domain(dominio);
        }else if(operacion == "size" || operacion == "SIZE"){
            cachee->size();            
        }else if(operacion == "clear" || operacion == "CLEAR"){
            cachee->clear();
        }else{
            cout << "no ingresaste bien la operacion, perdiste un intento" << endl; 
        }
    }
    cachee->destruir();
    delete cachee;
}