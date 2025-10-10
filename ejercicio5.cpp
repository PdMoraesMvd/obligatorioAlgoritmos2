#include <cassert>
#include <string>
#include <iostream>
#include <limits>
#include <List.h>
using namespace std;
struct Arista{
    int peso;
    int destino;
};
class Grafo{
    int vertices;
    List<Arista>** adyacentes;
    public:
    Grafo(int _vertices){
        vertices=_vertices;
        adyacentes=new List<Arista>*[vertices];
    };
    void añadirArista(int _peso, int origen, int _destino){
        Arista* nuevo= new Arista;
        nuevo->destino=_destino;
        nuevo->peso=_peso;
       // adyacentes[origen]->insert(nuevo);
    }
};
int main()
{
    // TODO
    return 0;
}