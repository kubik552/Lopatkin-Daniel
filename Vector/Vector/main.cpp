#include <iostream>
#include <cmath>
#include "vector.h"

using namespace std;

int main()
{
    Vector<int> vec = {0, 1, 2, 3, 4, 5};

    for (int i = 0; i < vec.size(); i++)
    {
        cout << vec[i] << " ";
    }
    cout << "\n";

    vec.push_back(10);

    for (int i = 0; i < vec.size(); i++)
    {
        cout << vec[i] << " ";
    }
    cout << "\n";

    vec.erase(2);

    for (int i = 0; i < vec.size(); i++)
    {
        cout << vec[i] << " ";
    }
    cout << "\n";
}