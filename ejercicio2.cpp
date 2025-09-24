#include <cassert>
#include <string>
#include <iostream>
#include <limits>

using namespace std;

struct listaPath{ //representa un nodo, utilizamos una lista dobelemente enlazada para poder manejar los ordenes pedidos
    string path;
    int tiempo;
    string titulo;
    listaPath *sig; //nodo siguiente
    listaPath *prev; //nodo anterior
};

struct arrayHash{ //es la tabla de hash principal, que tiene como clave el dominio y el path
    string dominio;
    string path;
    string titulo;
    int tiempo;
    string estado;   // solo puede tomar "ocupado", "libre", "borrado"
    listaPath *nodo; // puntero al nodo correspondiente en la lista de paths
    arrayHash(string _dominio, string _path, string _titulo, int _tiempo) : dominio(_dominio), path(_path), titulo(_titulo), tiempo(_tiempo), estado("libre"), nodo(NULL) {}
    arrayHash() : dominio(), path(), titulo(), tiempo(0), estado("libre"), nodo(NULL) {}
};

struct arrayHash2{ //otra tabla de hash que tiene como clave el dominio
    string dominio;
    string estado; //solo puede tomar "ocupado", "libre", "borrado"
    listaPath *lista; //lista de paths de ese dominio
    int cantidad; //numero de recursos del dominio
    arrayHash2() : dominio(""), estado("libre"), lista(NULL), cantidad(0) {}
};

class cache{
    int capacidad; //tamaño de ambas tablas de hash
    arrayHash2 *hash2;
    arrayHash *hash;
    int cantidad; //cantidad total de recursos del cache

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

    //PRE:
    //POS: devuelve un entero
    int h2(string clave){ //segunda funcion de hash, una adaptacion de la primera, decidimos multiplicarla por un numero cualquiera que elegimos 
        int h = 0;
        for (int i = 0; i < clave.length(); i++)
        {
            h = (31 * h + int(clave[i]))*7;
        }
        return h;
    }

    //PRE: intento es un número entero positivo
    //POS: devuelve un índice válido en el rango 0,capacidad-1
    int pos(string clave, int intento){ //usamos nuestras dos funciones de hash para calcular la posicion
        unsigned int uno = h1(clave);
        unsigned int dos = h2(clave);
        unsigned int indice =  (uno + intento * dos) % capacidad;
        return indice; 
    }

    //PRE:
    //POS: borra todos los nodos de la lista
    void borrar(listaPath *&l){
        while (l != NULL)
        {
            listaPath *aBorrar = l;
            l = l->sig;
            delete aBorrar;
        }
    }

    //PRE: nodo no es vacío (en nuestro caso no lo chequeamos explicitamente cuando llamamos a esta funcion porque ya sabemos de antemano que no es vacia)
    //POS: nodo se inserta al principio de l
    void insertarAlPrincipio(listaPath*&l, listaPath*&nodo){
        nodo->sig = l;
        nodo->prev = NULL;
        if (l != NULL) {
            l->prev = nodo;
        }
        l = nodo;
    }

    //PRE:
    //POS: devuelve indice del dominio del hash2 o -1 si no esta
    int indiceHash2(arrayHash2 *H2, string dominio){
        int intento = 1;
        int posicion = pos(dominio, intento);
        while (H2->estado != "libre")
        {
            if (H2->estado == "ocupado" && H2->dominio == dominio)
            {
                return posicion;
            }
            intento++;
            posicion = pos(dominio, intento);
        }
        return -1;
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

//PRE:  parametro es de hasta 50 caracteres (sin espacios)
//POS: devuelve true si parametro contiene espacios, false en otro caso
bool tieneEspacio(string parametro){
    for(int i=0;i<parametro.length(); i++){
        if(parametro[i] == ' ') return true;
    }
    return false;
}

public:
    //PRE: 1<= capacidad <=1000000
    //POS: crea un cache correctamente inicializado
    cache(int capacidad){
        assert(1<= capacidad <=1000000);
        this->capacidad=capacidad;
        hash = new arrayHash[capacidad](); 
        hash2 = new arrayHash2[capacidad]();
        cantidad = 0;
    }

    //PRE: dominio es de hasta 50 caracteres (sin espacios), path es de hasta 50 caracteres (sin espacios), titulo es de hasta 50 caracteres (sin espacios), 1 <= tiempo <= 10^9
    //POS: inserta un recurso nuevo o actualiza su título y tiempo si ya esta
    void put(string dominio, string path, string titulo, int tiempo){
        assert(dominio.length() <= 50 && !tieneEspacio(dominio));
        assert(path.length() <= 50 && !tieneEspacio(path));
        assert(titulo.length() <= 50 && !tieneEspacio(titulo));
        assert(1 <= tiempo && tiempo <= 1000000000);

        string clave = dominio + "|" + path;
        int intento =1;
        int posicion=pos(clave, intento);

        while(hash[posicion].estado == "ocupado" && !(hash[posicion].dominio==dominio && hash[posicion].path==path)){
            intento++;
            posicion=pos(clave, intento);
        }
        if(hash[posicion].estado == "ocupado" && hash[posicion].dominio==dominio && hash[posicion].path==path){ 
            hash[posicion].tiempo=tiempo;
            hash[posicion].titulo=titulo;
            if(hash[posicion].nodo != NULL){ 
                listaPath* lp = hash[posicion].nodo;
                lp->tiempo = tiempo;
                lp->titulo = titulo;

                //mover al frente de la lista si no esta, busco dominio en el segundo hash
                int intento2 = 1;
                int posicion2 = pos(dominio, intento2);
                while((hash2[posicion2].estado == "ocupado" || hash2[posicion2].estado == "borrado") &&
                    hash2[posicion2].dominio != dominio && intento2 < capacidad){
                    intento2++;
                    posicion2 = pos(dominio, intento2);
                }
                if(hash2[posicion2].estado == "ocupado" && hash2[posicion2].dominio == dominio){
                    if(lp != hash2[posicion2].lista){
                        if(lp->prev) lp->prev->sig = lp->sig;
                        if(lp->sig)  lp->sig->prev = lp->prev;
                        lp->sig = hash2[posicion2].lista;
                        if(hash2[posicion2].lista) hash2[posicion2].lista->prev = lp;
                        lp->prev = NULL;
                        hash2[posicion2].lista = lp;
                    }
                }
            }
            return;
        }

        if(hash[posicion].estado == "libre" || hash[posicion].estado == "borrado"){
            hash[posicion].estado = "ocupado";
            hash[posicion].dominio=dominio;
            hash[posicion].tiempo=tiempo;
            hash[posicion].path=path;
            hash[posicion].titulo=titulo;
            cantidad++;
        }

        clave = dominio;
        intento=1;
        int posicion2 = pos(clave,intento);
        while(hash2[posicion2].estado == "ocupado" && hash2[posicion2].dominio!=dominio && intento < capacidad){
            intento++;
            posicion2=pos(clave, intento);
        }

        listaPath* nodo = new listaPath;
        nodo->path = path;
        nodo->prev=NULL;
        nodo->sig=NULL;
        nodo->tiempo=tiempo;
        nodo->titulo=titulo;

        if(hash2[posicion2].estado == "libre" || hash2[posicion2].estado == "borrado"){
            hash2[posicion2].dominio=dominio;
            insertarAlPrincipio(hash2[posicion2].lista, nodo);
            hash[posicion].nodo=nodo;
            hash2[posicion2].estado="ocupado";
            hash2[posicion2].cantidad++;
            return;
        }

        if(hash2[posicion2].estado == "ocupado"){
            insertarAlPrincipio(hash2[posicion2].lista, nodo);
            hash[posicion].nodo=nodo;
            hash2[posicion2].cantidad++;
        }
    }
    //PRE:dominio es de hasta 50 caracteres (sin espacios), path es de hasta 50 caracteres (sin espacios)
    //POS: imprime {titulo} {tiempo} si el recurso existe, o "recurso_no_encontrado" si no existe
    void get(string dominio, string path){
        assert(dominio.length() <= 50 && !tieneEspacio(dominio));
        assert(path.length() <= 50 && !tieneEspacio(path));
        int intento = 1;
        string clave = dominio + "|" + path;
        int posicion = pos(clave, intento);
        while (this->hash[posicion].estado == "ocupado" || this->hash[posicion].estado == "borrado")//busca mientras la celde este ocupada o borrada
        {
            if (this->hash[posicion].estado == "ocupado" && this->hash[posicion].dominio == dominio && this->hash[posicion].path == path)
            {
                cout << this->hash[posicion].titulo << " "  << this->hash[posicion].tiempo << endl;
                return;
            }
            intento++;
            posicion = pos(clave, intento);
        }
        cout << "recurso_no_encontrado" << endl;
    }
    //PRE:dominio es de hasta 50 caracteres (sin espacios), path es de hasta 50 caracteres (sin espacios)
    //POS: si existe el recurso, lo elimina, si no, no hace nada
    void remove(string dominio, string path){
      assert(dominio.length() <= 50 && !tieneEspacio(dominio));
      assert(path.length() <= 50 && !tieneEspacio(path));
      int intento=1;
      string clave = dominio + "|" + path;
      int posicion = pos(clave, intento);
      while(hash[posicion].estado == "ocupado" && hash[posicion].dominio!=dominio && hash[posicion].path!=path){
        intento++;
        posicion=pos(clave,intento);
      }
      if(hash[posicion].estado == "libre" || hash[posicion].estado == "borrado") return; //no esta
      if(hash[posicion].estado == "ocupado" && hash[posicion].dominio==dominio && hash[posicion].path==path){
        listaPath* aBorrar = hash[posicion].nodo;
        intento=1;
        int posicion2=pos(dominio, intento);
        while (hash2[posicion2].estado != "libre" &&(hash2[posicion2].estado != "ocupado" || hash2[posicion2].dominio != dominio)){
            intento++;
            posicion2 = pos(dominio, intento);
        }
        if (hash2[posicion2].estado == "ocupado" && hash2[posicion2].dominio == dominio) {
            borrarNodo(hash2[posicion2].lista, aBorrar);
            hash2[posicion2].cantidad--;
        }
        hash[posicion].estado="borrado";
        cantidad--;
      }
    }

    //PRE:dominio es de hasta 50 caracteres (sin espacios), path es de hasta 50 caracteres (sin espacios)
    //POS: imprime "true" si existe, "false" E.O.C (en otro caso)
    void contains(string dominio, string path){
        assert(dominio.length() <= 50 && !tieneEspacio(dominio));
        assert(path.length() <= 50 && !tieneEspacio(path));
        int intento = 1;
        string clave = dominio + "|" + path;
        int posicion = pos(clave, intento);
        while (this->hash[posicion].estado == "ocupado" || this->hash[posicion].estado == "borrado"){
            if (this->hash[posicion].estado == "ocupado" && this->hash[posicion].dominio == dominio && this->hash[posicion].path == path){
                cout << "true" << endl;
                return;
            }
            intento++;
            posicion = pos(clave, intento);
        }
        cout << "false" << endl;
    }

    //PRE:dominio es de hasta 50 caracteres (sin espacios)
    //POS: imprime todos los recursos del dominio o vacio si no hay ni 1
    void list_domain(string dominio){
        assert(dominio.length() <= 50 && !tieneEspacio(dominio));
        int intento = 1;
        string clave = dominio;
        int posicion = pos(clave, intento);

        while ((hash2[posicion].estado == "ocupado" || hash2[posicion].estado == "borrado") && hash2[posicion].dominio != dominio && intento < capacidad) {
            intento++;
            posicion = pos(clave, intento);
        }

        if (intento == capacidad || hash2[posicion].estado != "ocupado" || hash2[posicion].dominio != dominio){
            cout << endl;
            return;
        }

        listaPath* aux = hash2[posicion].lista;
        if (aux == NULL){
            cout << endl;
            return;
        }

        while (aux != NULL){
            cout << aux->path;
            aux = aux->sig;
            if (aux != NULL) cout << " ";
        }
        cout << endl;
        
    }

    //PRE:
    //POS: imprime la cantidad total de recursos del cache
    int size(){
        cout << cantidad << endl;
    }

    //PRE:
    //POS: elimina todos los recursos del dominio y "resetea" todos los atributos correspondientes
    void clear(){
        for(int i=0; i<capacidad ; i++){
            hash[i].nodo = NULL;
            hash[i].estado = "libre";
            hash2[i].estado = "libre";
            while(hash2[i].lista!=NULL){
                listaPath* aBorrar = hash2[i].lista;
                hash2[i].lista = hash2[i].lista->sig;
                delete aBorrar;
            }
            hash2[i].cantidad=0;
        }
        cantidad=0;
    }

    //PRE:dominio es de hasta 50 caracteres (sin espacios)
    //POS: imprime la cantidad de recursos de un dominio especifico
    void count_domain(string dominio){
        assert(dominio.length() <= 50 && !tieneEspacio(dominio));
        int intento = 1;
        string clave=dominio;
        int posicion = pos(clave, intento);
        while( (hash2[posicion].estado == "ocupado" || hash2[posicion].estado == "borrado") && hash2[posicion].dominio != dominio){
            intento++;
            posicion= pos(clave, intento);
        }
        if(hash2[posicion].estado=="ocupado" && hash2[posicion].dominio == dominio){
            cout << hash2[posicion].cantidad << endl;
        }else{
            cout << "0" << endl;
        }
    }

    //PRE:dominio es de hasta 50 caracteres (sin espacios)
    //POS: elimina todos los recursos de dominio o nada si no existe o no tiene
    void clear_domain(string dominio){
        assert(dominio.length() <= 50 && !tieneEspacio(dominio));
        int intento = 1;
        string clave=dominio;
        int posicion = pos(clave, intento);
        while( (hash2[posicion].estado == "ocupado" || hash2[posicion].estado == "borrado") && hash2[posicion].dominio != dominio && intento < capacidad){
            intento++;
            posicion= pos(clave, intento);
        }
        if (intento == capacidad || hash2[posicion].estado != "ocupado" || hash2[posicion].dominio != dominio){
            return; 
        }
        if(hash2[posicion].estado=="ocupado" && hash2[posicion].dominio == dominio){
            while(hash2[posicion].lista != NULL){
                listaPath* aBorrar = hash2[posicion].lista;
                string clave2 = dominio + "|" + aBorrar->path;
                int intento2 = 1;
                int posicion2 = pos(clave2, intento2);
                while( (hash[posicion2].estado == "ocupado" || hash[posicion2].estado == "borrado") && hash[posicion2].dominio!=dominio){
                    intento2++;
                    posicion2 = pos(clave2,intento2);
                }
                hash[posicion2].estado = "borrado";
                hash[posicion2].nodo = NULL;
                hash2[posicion].lista = hash2[posicion].lista->sig;
                delete aBorrar;
                cantidad--;
                hash2[posicion].cantidad--;
                
            }
            hash2[posicion].estado="borrado";
        }
    }
    //PRE:
    //POS: libera toda la memoria del cache
    void destruir(){
        for(int i=0; i<capacidad;i++){
            listaPath* aux = hash2[i].lista;
            while(aux!=NULL){
                listaPath* aBorrar = aux;
                aux=aux->sig;
                delete aBorrar;
                hash2[i].lista = NULL;
            }
        }
        delete[] hash;
        delete[] hash2;
        cantidad=0;
        capacidad=0;
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
