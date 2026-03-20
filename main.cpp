#include <iostream>
#include <fstream>
#include <bits/stdc++.h>

using namespace std;


vector<vector<int>> wczytajDane(const string& path)
{
    cout << "Wczytywanie danych z pliku: " << path << endl;
    ifstream plik(path);
    vector<vector<int>> dane;
    if (plik.is_open())
    {
        string linia;
        while (getline(plik, linia))
        {
            vector<int> wiersz;
            stringstream ss(linia);
            int liczba;
            while (ss >> liczba)
            {
                wiersz.push_back(liczba);
            }
            dane.push_back(wiersz);
        }
        plik.close();
    }
    else
    {
        cerr << "Nie można otworzyć pliku!" << endl;
    }
    return dane;
}

int main()
{
    vector<vector<int>> dane1 = wczytajDane("data1.txt");
    vector<vector<int>> dane2 = wczytajDane("data2.txt");
    vector<vector<int>> dane3 = wczytajDane("data3.txt");
    vector<vector<int>> dane4 = wczytajDane("data4.txt");

    for(const auto& row : dane1)
    {
        cout << "r: " << row[0]<< endl << "p: " << row[1] << endl << "q: " << row[2] << endl;
    }

    return 0;
}
