#include <cassert>
#include <string>
#include <iostream>
#include <climits>
using namespace std;

int max(int a, int b)
{
    return a > b ? a : b;
}

int memo_rec(int *arr, int i, int f, int k, int ***cubo)
{
    if (i > f)
        cubo[i][f][k] = 0;
    else if (i == f)
        cubo[i][f][k] = (k + 1) * (k + 1);
    else if (cubo[i][f][k] == -1)
    {
        int valorUsarlo = ((k + 1) * (k + 1)) + memo_rec(arr, i + 1, f, 0, cubo);
        int numero = arr[i];
        int valorNoUsarlo = INT_MIN;
        for (int pos = i + 1; pos <= f; pos++) // buscar si hay mas valores de numero
        {
            if (arr[pos] == numero)
            {
                int a = memo_rec(arr, i + 1, pos - 1, 0, cubo) + memo_rec(arr, pos, f, k + 1, cubo); // para cada uno calculo el memo_rec
                if (a > valorNoUsarlo)
                    valorNoUsarlo = a;
            }
        }
        int maxValor = max(valorUsarlo, valorNoUsarlo);
        cubo[i][f][k] = maxValor;
    }
    return cubo[i][f][k];
}

void memo(int *arr, int largo)
{
    int ***cubo = new int **[largo]();
    for (int i = 0; i < largo; i++)
    {
        cubo[i] = new int *[largo]();
        // cout << largo << endl;
        for (int j = 0; j < largo; j++)
        {
            cubo[i][j] = new int[largo]();
            for (int k = 0; k < largo; k++)
            {
                cubo[i][j][k] = -1;
                if (i > j)
                    cubo[i][j][k] = 0;
            }
        }
    }

    // cout << "test" << endl;
    int var = memo_rec(arr, 0, largo - 1, 0, cubo);
    cout << var << endl;
}

int main()
{
    int largo;
    cin >> largo;
    int *arr = new int[largo];
    for (int i = 0; i < largo; i++)
    {
        int n;
        cin >> n;
        arr[i] = n;
    }
    memo(arr, largo);
}