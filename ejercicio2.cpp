#include <cassert>
#include <string>
#include <iostream>
#include <limits>

using namespace std;

struct listaPath
{
    string path;
    int tiempo;
    string titulo;
    listaPath *sig;
    listaPath *prev;
};

struct arrayHash
{
    string dominio;
    string path;
    string titulo;
    int tiempo;
    string estado;   // solo puede tomar "ocupado", "libre", "borrado"
    listaPath *nodo; // puntero al nodo correspondiente en la lista de paths
    arrayHash(string _dominio, string _path, string _titulo, int _tiempo) : dominio(_dominio), path(_path), titulo(_titulo), tiempo(_tiempo), estado("libre"), nodo(NULL) {}
    arrayHash() : dominio(), path(), titulo(), tiempo(0), estado("libre"), nodo(NULL) {}
};

struct arrayHash2
{
    string dominio;
    string estado;
    listaPath *lista;
    arrayHash2() : dominio(), estado("libre"), lista(NULL) {}
};

class cache
{
    int generacion;
    arrayHash2 *hash2;
    arrayHash *hash;
    int cantidad;

    int h1(string clave)
    {
        int h = 0;
        for (int i = 0; i < clave.length(); i++)
        {
            h = 31 * h + int(clave[i]);
        }
        return h;
    }

    int h2(string clave)
    {
        int h = 0;
        for (int i = 0; i < clave.length(); i++)
        {
            h = h + clave[i] * (i + 1);
        }
        return h;
    }

    int pos(string clave, int intento)
    {
        int uno = h1(clave);
        int dos = h2(clave);
        return (uno + intento * dos) % 10000019; // el siguiente número primo de 10^6 sacado de https://www.walter-fendt.de/html5/mes/primenumbers_es.htm
    }

    void borrar(listaPath *&l)
    {
        while (l != NULL)
        {
            listaPath *aBorrar = l;
            l = l->sig;
            delete aBorrar;
        }
    }

void insertarOrdenado(listaPath*& l, listaPath* nodo){
    if (l == NULL){
        l = nodo;
        nodo->prev = NULL;
        nodo->sig = NULL;
        return;
    }
    if (nodo->tiempo >= l->tiempo){
        nodo->sig = l;
        nodo->prev = NULL;
        l->prev = nodo;
        l = nodo;
        return;
    }

    listaPath* cur = l;
    while (cur->sig != NULL && cur->sig->tiempo > nodo->tiempo){
        cur = cur->sig;
    }
    // insertar DESPUÉS de cur
    nodo->sig  = cur->sig;
    nodo->prev = cur;
    if (cur->sig != NULL) cur->sig->prev = nodo;
    cur->sig = nodo;
}

    // Busca el bucket de un dominio en arrayHash2 (devuelve índice o -1 si no existe)
    int indiceHash2(arrayHash2 *H2, string dominio)
    {
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

public:
    cache()
    {
        hash = new arrayHash[10000019](); // el siguiente número primero de 10^6 sacado de https://www.walter-fendt.de/html5/mes/primenumbers_es.htm
        hash2 = new arrayHash2[10000019]();
        cantidad = 0;
        generacion = 1;
    }

    void put(string dominio, string path, string titulo, int tiempo)
    {
        // agregar en arrayHash
        int intento = 1;
        string clave = dominio + "|" + path;
        int posicion = pos(clave, intento);
        if (this->hash[posicion].dominio == dominio && this->hash[posicion].path == path)
        {
            this->hash[posicion].dominio == dominio;
            this->hash[posicion].path == path;
            return;
        }
        while (this->hash[posicion].estado == "ocupado")
        {
            intento++;
            posicion = pos(clave, intento);
        }
        int index = posicion;
        this->hash[posicion].dominio = dominio;
        this->hash[posicion].path = path;
        this->hash[posicion].titulo = titulo;
        this->hash[posicion].tiempo = tiempo;
        this->hash[posicion].estado = "ocupado";
        this->hash[posicion].nodo = NULL;

        // agregar en arrayHash2 (ya sabemos que se agrego en la primera)
        intento = 1;
        int posicion = pos(dominio, intento);
        while (this->hash2[posicion].estado == "ocupado" && this->hash2[posicion].dominio != dominio)
        {
            intento++;
            posicion = pos(dominio, intento);
        }
        listaPath *nodo = new listaPath;
        nodo->sig = NULL;
        nodo->path = path;
        nodo->tiempo = tiempo;
        nodo->titulo = titulo;
        // caso dominio nuevo
        if (this->hash2[posicion].estado == "libre" || this->hash2[posicion].estado == "borrado")
        {
            this->hash2[posicion].estado = "ocupado";
            this->hash2[posicion].dominio = dominio;
            insertarOrdenado(this->hash2[posicion].lista, nodo);
        }
        // caso dominio ya existente
        else
        {
            insertarOrdenado(this->hash2[posicion].lista, nodo);
        }
        cantidad++;
    }

    void get(string dominio, string path)
    {
        int intento = 1;
        string clave = dominio + "|" + path;
        int posicion = pos(clave, intento);
        while (this->hash[posicion].estado == "ocupado" || this->hash[posicion].estado == "borrado")
        {
            if (this->hash[posicion].estado == "ocupado" && this->hash[posicion].dominio == dominio && this->hash[posicion].path == path)
            {
                cout << "{" << this->hash[posicion].titulo << "}" << " " << "{" << this->hash[posicion].tiempo << "}" << endl;
                return;
            }
            intento++;
            posicion = pos(clave, intento);
        }
        cout << "recurso_no_encontrado" << endl;
    }

    void remove(string dominio, string path)
    {
        int intento = 1;
        string clave = dominio + "|" + path;
        int posicion = pos(clave, intento);
        // remove del arrayHash
        while ((hash[posicion].estado == "ocupado" || hash[posicion].estado == "borrado") && (hash[posicion].dominio != dominio || hash[posicion].path != path))
        {
            intento++;
            posicion = pos(clave, intento);
        }
        if (hash[posicion].estado == "libre")
            return; // no existe ese elemento
        if (hash[posicion].estado == "ocupado" && hash[posicion].dominio == dominio && hash[posicion].path == path)
        {
            if (hash[posicion].nodo != NULL)
            {
                int indice = indiceHash2(hash2, dominio);
                listaPath *aBorrar = hash[posicion].nodo;
                if (indice != -1 && hash2[indice].lista == aBorrar)
                {
                    hash2[indice].lista = aBorrar->sig;
                }
                if (aBorrar->prev != NULL)
                {
                    aBorrar->prev->sig = aBorrar->sig;
                }
                if (aBorrar->sig != NULL)
                {
                    aBorrar->sig->prev = aBorrar->prev;
                }
                delete aBorrar;
                hash[posicion].nodo = NULL;
            }
            hash[posicion].estado = "borrado";
        }
    }

    void contains(string dominio, string path)
    {
        int intento = 1;
        string clave = dominio + "|" + path;
        int posicion = pos(clave, intento);
        while (this->hash[posicion].estado == "ocupado" || this->hash[posicion].estado == "borrado")
        {
            if (this->hash[posicion].estado == "ocupado" && this->hash[posicion].dominio == dominio && this->hash[posicion].path == path)
            {
                cout << "true" << endl;
                return;
            }
            intento++;
            posicion = pos(clave, intento);
        }
        cout << "false" << endl;
    }

    void list_domain(string dominio)
    {
        int intento = 1;
    }

    int size()
    {
        return cantidad;
    }

    void clear()
    {
    }
};

int main()
{
    // TODO
    return 0;
}