#include <cassert>
#include <string>
#include <iostream>
#include <limits>

using namespace std;

struct arrayHash{
    string dominio;
    string path;
    string titulo;
    int tiempo;
    string clave;
    string estado; //solo puede tomar "ocupado", "libre", "borrado"
    arrayHash(string _dominio, string _path, string _titulo, int _tiempo):dominio(_dominio), path(_path), titulo(_titulo), tiempo(_tiempo), clave(dominio + "|" +path), estado("libre"){}
    arrayHash():dominio(), path(), titulo(), tiempo(0), clave(), estado("libre"){}
};

struct listaPath{
    string path;
    int tiempo;
    string titulo;
    listaPath* sig;
    int generacion;
};
struct arrayHash2{
    string dominio;
    string estado;
    listaPath* lista;
    arrayHash2():dominio(), estado("libre"), lista(NULL){}

};


class cache{
    int generacion;
    arrayHash2* hash2;
    arrayHash* hash;
    int cantidad;

    int h1(string clave){
        int h=0;
        for(int i=0;i<clave.length(); i++){
            h=31*h+int(clave[i]);
        }
        return h;
    }

    int h2(string clave){
        int h=0;
        for(int i=0;i<clave.length();i++){
            h=h+clave[i]*(i+1);
        }
        return h;
    }

    int pos(string clave, int intento){
        int uno=h1(clave);
        int dos=h2(clave);
        return (uno+intento*dos)%10000019; //el siguiente número primo de 10^6 sacado de https://www.walter-fendt.de/html5/mes/primenumbers_es.htm
    }

    public:
    cache(){
        hash=new arrayHash[10000019]; //el siguiente número primero de 10^6 sacado de https://www.walter-fendt.de/html5/mes/primenumbers_es.htm
        hash2 = new arrayHash2[10000019];
        cantidad = 0;
        generacion = 1;
    }

    void put(string dominio, string path, string titulo, int tiempo){
        int intento=1;
        string clave = dominio + "|" + path;
        int posicion=pos(clave, intento);
        if(this->hash[posicion].dominio == dominio && this->hash[posicion].path == path){
            this->hash[posicion].dominio == dominio;
            this->hash[posicion].path == path;
            return;
        }
        while(this->hash[posicion].estado == "ocupado"){
            intento++;
            posicion=pos(clave, intento);
        }
        this->hash[posicion].dominio=dominio;
        this->hash[posicion].path=path;
        this->hash[posicion].titulo=titulo;
        this->hash[posicion].titulo=tiempo;
        this->hash[posicion].estado="ocupado";

    }

    void get(string dominio, string path){
        int intento=1;
        string clave = dominio + "|" + path;
        int posicion=pos(clave, intento);
        while(this->hash[posicion].estado == "ocupado" || this->hash[posicion].estado == "borrado"){
            if(this->hash[posicion].estado == "ocupado" && this->hash[posicion].dominio== dominio && this->hash[posicion].path == path){
                cout << "{" << this->hash[posicion].titulo << "}" << " " <<"{" << this->hash[posicion].tiempo << "}" <<endl;
                return;
            }
            intento++;
            posicion=pos(clave, intento);
        }
        cout << "recurso_no_encontrado" <<endl;
    }

    void remove(string dominio, string path){
        int intento=1;
        string clave = dominio + "|" + path;
        int posicion=pos(clave, intento);
        while(this->hash[posicion].estado == "ocupado" || this->hash[posicion].estado == "borrado"){
            if(this->hash[posicion].estado == "ocupado" && this->hash[posicion].dominio== dominio && this->hash[posicion].path == path){
                this->hash[posicion].estado = "borrado";
                cantidad--;
                return;
            }
            intento++;
            posicion=pos(clave, intento);
        }
    }

    void contains(string dominio, string path){
        int intento=1;
        string clave = dominio + "|" + path;
        int posicion=pos(clave, intento);
        while(this->hash[posicion].estado == "ocupado" || this->hash[posicion].estado == "borrado"){
            if(this->hash[posicion].estado == "ocupado" && this->hash[posicion].dominio== dominio && this->hash[posicion].path == path){
                cout << "true" << endl;
                return;
            }
            intento++;
            posicion=pos(clave, intento);
        }
        cout << "false" <<endl;
    }

    int size(){
        return cantidad;
    }

    void clear(){
        
    }

};

int main()
{
    // TODO
    return 0;
}