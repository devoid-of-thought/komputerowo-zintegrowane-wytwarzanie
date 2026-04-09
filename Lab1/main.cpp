#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include <cmath>
#include <chrono>
#include <vector>
#include <algorithm>

using namespace std;

struct operacja
{
    int r;
    int p;
    int q;
    int index;
};
vector<operacja> wczytajDane(const string &path)
{
    cout << "Wczytywanie danych z pliku: " << path << endl;
    ifstream plik(path);
    vector<operacja> dane;
    if (plik.is_open())
    {
        operacja op;
        int index = 0;
        string linia;
        while (getline(plik, linia))
        {
            vector<int> wiersz;
            stringstream ss(linia);
            int liczba;
            while (ss >> liczba)            {
                wiersz.push_back(liczba);
            }
            op.index = index;
            op.r = wiersz[0];
            op.p = wiersz[1];
            op.q = wiersz[2];
            dane.push_back(op);
        }
        plik.close();
    }
    else
    {
        cerr << "Nie można otworzyć pliku!" << endl;
    }
    return dane;
}
int obliczCmax(const vector<operacja> &dane, const vector<pair<int, operacja>> &kolejnosc)
{
    int czas = 0;
    int cmax = 0;

    for (size_t i = 0; i < kolejnosc.size(); i++)
    {
        int indeks = kolejnosc[i].first;
        int r = dane[indeks].r;
        int p = dane[indeks].p;
        int q = dane[indeks].q;

        czas = max(czas, r) + p;
        cmax = max(cmax, czas + q);
    }

    return cmax;
}

vector<pair<int, operacja>> manageTime(const vector<operacja> &dane)
{
    vector<pair<int, operacja>> kolejnosc;
    vector<operacja> dostepne = dane;
    sort(dostepne.begin(), dostepne.end(), [](const operacja &a, const operacja &b) {
        return a.r < b.r;
    });
    int czas = 0;
    while (!dostepne.empty())
    {
        vector<operacja> gotowe;
        for (size_t i = 0; i < dostepne.size(); i++)
        {
            if (dostepne[i].r <= czas)
            {
                gotowe.push_back(dostepne[i]);
            }
        }
        if (gotowe.empty())
        {
            czas = dostepne[0].r;
            kolejnosc.push_back({dostepne[0].index, dostepne[0]});
            dostepne.erase(dostepne.begin());
            continue;
        }
      
        auto maxQ = max_element(gotowe.begin(), gotowe.end(), [](const operacja &a, const operacja &b) {
            return a.q < b.q;
        });
        kolejnosc.push_back({maxQ->index, *maxQ});
        dostepne.erase(remove_if(dostepne.begin(), dostepne.end(), [&](const operacja &op) {
            return op.index == maxQ->index;
        }), dostepne.end());
    }
    return kolejnosc;
}
int main()
{
    auto start = chrono::high_resolution_clock::now();
    vector<operacja> dane1 = wczytajDane("data1.txt");
    vector<operacja> dane2 = wczytajDane("data2.txt");
    vector<operacja> dane3 = wczytajDane("data3.txt");
    vector<operacja> dane4 = wczytajDane("data4.txt");

    int timeSum = 0;
    timeSum += obliczCmax(dane1, manageTime(dane1));
    timeSum += obliczCmax(dane2, manageTime(dane2));
    timeSum += obliczCmax(dane3, manageTime(dane3));
    timeSum += obliczCmax(dane4, manageTime(dane4));
    cout << "Wynik: " << timeSum << endl;
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    cout << "Czas wykonania: " << duration.count() << " ms" << endl;

    return 0;
}
