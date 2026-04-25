#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include <cmath>
#include <chrono>
#include <vector>
#include <algorithm>
#include <sstream>
#include <chrono>
using namespace std;

struct operacja
{
    int p;
    int w;
    int d;
    int index;
};

vector<vector<operacja>> WczytajDane(const string &path)
{
    vector<vector<operacja>> operacje;
    ifstream plik(path);
    string line;
    while (getline(plik, line))
    {

        if (line.find("data.") == 0)
        {
            getline(plik, line);
            int liczba_operacji = stoi(line);
            vector<operacja> aktualne_operacje;
            for (int i = 0; i < liczba_operacji; i++)
            {
                getline(plik, line);
                stringstream ss(line);
                operacja op;

                ss >> op.p >> op.w >> op.d;
                op.index = i + 1;
                aktualne_operacje.push_back(op);
            }
            operacje.push_back(aktualne_operacje);
        }
    }

    return operacje;
}
map<int, pair<int, vector<operacja>>> mapa_optymalnych_czasow;

pair<int, vector<operacja>> algorytm(vector<operacja> &operacje, int ocena_0, int maska)
{
    int ilość_operacji = operacje.size();
    if (maska == (1 << ilość_operacji) - 1)
    {
        return {0, {}};
    }
    if (mapa_optymalnych_czasow.count(maska))
    {
        return mapa_optymalnych_czasow[maska];
    }
    int najmniejsza_ocena = 99999999;
    vector<operacja> najlepsza_kolejnosc;
    for (int i = 0; i < ilość_operacji; i++)
    {
        int obecna_operacja = (1 << i);
        if ((maska & obecna_operacja) == 0)
        {
            int C = ocena_0 + operacje[i].p;
            int K = operacje[i].w * max(0, C - operacje[i].d);

            auto wynik = algorytm(operacje, C, maska | (1 << i));
            if (wynik.first + K < najmniejsza_ocena)
            {
                najmniejsza_ocena = wynik.first + K;
                najlepsza_kolejnosc = wynik.second;
                najlepsza_kolejnosc.insert(najlepsza_kolejnosc.begin(), operacje[i]);
            }
        }
    }
    mapa_optymalnych_czasow[maska] = {najmniejsza_ocena, najlepsza_kolejnosc};
    return mapa_optymalnych_czasow[maska];
}
std::pair<int, vector<operacja>> algorytm_przegląd_zupełny(vector<operacja> operacje, int ocena_0, vector<operacja> kolejnosc)

{
    int ilosc_operacji = operacje.size();
    if (ilosc_operacji == 0)
    {
        return {0, kolejnosc};
    }
    int najmniejsza_ocena = 99999999;
    vector<operacja> najlepsza_kolejnosc = kolejnosc;
    for (int i = 0; i < ilosc_operacji; i++)
    {
        int C = ocena_0 + operacje[i].p;
        int K = operacje[i].w * max(0, C - operacje[i].d);

        vector<operacja> operacje_kopia = operacje;
        operacje_kopia.erase(operacje_kopia.begin() + i);

        vector<operacja> kolejnosc_kopia = kolejnosc;
        kolejnosc_kopia.push_back(operacje[i]);

        auto [ocena, kolejnosc_wykonania] = algorytm_przegląd_zupełny(operacje_kopia, C, kolejnosc_kopia);
        najmniejsza_ocena = min(najmniejsza_ocena, ocena + K);
        if (najmniejsza_ocena == ocena + K)
        {

            najlepsza_kolejnosc = kolejnosc_wykonania;
        }
    }

    return {najmniejsza_ocena, najlepsza_kolejnosc};
}
int wywołanie_z_czasem()
{
    auto operacje = WczytajDane("witi.data.txt");
    for (auto operacje_set : operacje)
    {
        mapa_optymalnych_czasow = {};
        auto start = chrono::high_resolution_clock::now();
        auto [ocena, kolejnosc] = algorytm(operacje_set, 0, 0);
        auto end = chrono::high_resolution_clock::now();
        cout << "Ocena: " << ocena << endl;
        cout << "Kolejność: ";
        for (const auto &op : kolejnosc)
        {
            cout << op.index << " ";
        }
        cout << "Czas wykonania: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl;

        cout << endl;
        if (operacje_set.size() <= 13)
        {
            auto start2 = chrono::high_resolution_clock::now();
            auto [ocena_zupełna, kolejnosc_zupełna] = algorytm_przegląd_zupełny(operacje_set, 0, {});
            auto end2 = chrono::high_resolution_clock::now();

            cout << "Ocena (przegląd zupełny): " << ocena_zupełna << endl;
            cout << "Kolejność (przegląd zupełny): ";
            for (const auto &op : kolejnosc_zupełna)
            {
                cout << op.index << " ";
            }
            cout << "Czas wykonania (przegląd zupełny): " << chrono::duration_cast<chrono::milliseconds>(end2 - start2).count() << " ms" << endl;
            cout << endl;
        }
    }
    return 0;
}
int wywołanie_bez_czasu()
{
    auto operacje = WczytajDane("witi.data.txt");
    for (auto operacje_set : operacje)
    {
        mapa_optymalnych_czasow = {};
        auto [ocena, kolejnosc] = algorytm(operacje_set, 0, 0);

        cout << "Ocena: " << ocena << endl;
        cout << "Kolejność: ";
        for (const auto &op : kolejnosc)
        {
            cout << op.index << " ";
        }

        cout << endl;

    }
    return 0;
}
int main()
{
    
    wywołanie_bez_czasu();
    return 0;
}