#include <cassert>
#include <string>
#include <iostream>
#include <limits>
using namespace std;

struct nodoHash
{
    string nombre;
    int posicion;
    nodoHash *sig;
};

class tablaHash
{
private:
    nodoHash **tabla;
    int capacidad;

    int h1(string clave)
    { // primera funcion de hash. Sacada del curso teórico (ver documentacion)
        int h = 0;
        for (int i = 0; i < clave.length(); i++)
        {
            h = 31 * h + int(clave[i]);
        }
        return abs(h);
    }

public:
    tablaHash(int cantidadElementos)
    {
        capacidad = cantidadElementos * 1.5;
        tabla = new nodoHash *[capacidad];
        for (int i = 0; i < capacidad; i++)
        {
            tabla[i] = NULL;
        }
    }
    void agregarElemento(string nombre, int posicion)
    {
        // ya sabemos que no se va a repetir nunca un nombre por letra, tambien que el factor de carga no va a pasar el 0.5
        int pos = h1(nombre) % capacidad;
        nodoHash *nodo = new nodoHash;
        nodo->nombre = nombre;
        nodo->posicion = posicion;
        if (tabla[pos] == NULL)
        {
            nodo->sig = NULL;
            tabla[pos] = nodo;
        }
        else
        {
            nodo->sig = tabla[pos];
            tabla[pos] = nodo;
        }
    }
    int buscarElemento(string nombre)
    {
        // nunca vamos a buscar un elemento que no exista asi que no hay que hacer ese chequeo
        int pos = h1(nombre) % capacidad;
        nodoHash *aux = tabla[pos];
        while (aux != NULL && aux->nombre != nombre)
            aux = aux->sig;
        return aux->posicion;
    }
};

long long contarInversionesMitad(int *a, int inicio, int mitad, int final)
{
    int *aux = new int[final - inicio + 1];
    long long ret = 0;
    int i = inicio;
    int j = mitad + 1;
    int k = 0;
    while (i <= mitad && j <= final)
    {
        if (a[i] <= a[j])
        {
            aux[k] = a[i];
            k++;
            i++;
        }
        else
        {
            aux[k] = a[j];
            k++;
            j++;
            ret += (mitad - i + 1); // todos los que quedan en la izquierda son mayores
        }
    }

    // copiar lo que quede en la mitad izquierda
    while (i <= mitad)
    {
        aux[k] = a[i];
        k++;
        i++;
    }

    // copiar lo que quede en la mitad derecha
    while (j <= final)
    {
        aux[k] = a[j];
        k++;
        j++;
    }

    // poner aux en el array original p que quede ordenado
    for (int t = 0; t < k; t++)
    {
        a[inicio + t] = aux[t];
    }

    delete[] aux;

    return ret;
}

long long contarInversiones(int *a, int inicio, int final)
{
    // caso base
    if (inicio >= final)
        return 0;

    // dividir
    int mitad = (inicio + final) / 2;
    long long cantInversionesIzq = contarInversiones(a, inicio, mitad);
    long long cantInversionesDer = contarInversiones(a, mitad + 1, final);
    long long cantInversionesInter = contarInversionesMitad(a, inicio, mitad, final);

    // merge
    return cantInversionesIzq + cantInversionesDer + cantInversionesInter;
}

int main()
{
    int N;
    cin >> N;
    int *rankingAyudante = new int[N];
    tablaHash tabla(N);
    for (int i = 0; i < N; i++)
    {
        string nom;
        cin >> nom;
        tabla.agregarElemento(nom, i);
    }

    for (int i = 0; i < N; i++)
    {
        string nom;
        cin >> nom;
        int pos = tabla.buscarElemento(nom);
        rankingAyudante[i] = pos;
    }
    long long total = contarInversiones(rankingAyudante, 0, N - 1);
    cout << total << endl;
    delete[] rankingAyudante;
    return 0;
}