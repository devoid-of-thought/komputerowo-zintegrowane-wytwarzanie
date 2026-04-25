
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
    vector<int> p;
    int index;
};
vector<pair<int, string>> wyniki;

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
            stringstream ss(line);
            int liczba_operacji;
            int liczba_maszyn;
            ss >> liczba_operacji >> liczba_maszyn;
            vector<operacja> aktualne_operacje;

            for (int i = 0; i < liczba_operacji; i++)
            {
                getline(plik, line);
                stringstream ss(line);
                operacja op;
                for (int j = 0; j < liczba_maszyn; j++)
                {
                    int czas;
                    ss >> czas;
                    op.p.push_back(czas);
                }

                op.index = i + 1;
                aktualne_operacje.push_back(op);
            }
            operacje.push_back(aktualne_operacje);
        }
        if (line.find("neh") == 0)
        {
            getline(plik, line);
            int wynik = stoi(line);
            getline(plik, line);
            wyniki.emplace_back(wynik, line);
        }
    }

    return operacje;
}
int oblicz_czas(const vector<operacja> &operacje)
{
    int ilość_maszyn = operacje[0].p.size();
    vector<int> czas_na_maszynie(ilość_maszyn, 0);
    for (const auto &aktualna_operacja : operacje)
    {
        for (int indeks_maszyny = 0; indeks_maszyny < ilość_maszyn; indeks_maszyny++)
        {
            if (indeks_maszyny == 0)
            {
                czas_na_maszynie[indeks_maszyny] += aktualna_operacja.p[indeks_maszyny];
            }
            else
            {
                czas_na_maszynie[indeks_maszyny] = max(czas_na_maszynie[indeks_maszyny - 1], czas_na_maszynie[indeks_maszyny]) + aktualna_operacja.p[indeks_maszyny];
            }
        }
    }
    return czas_na_maszynie[ilość_maszyn - 1];
}
pair<int, vector<operacja>> neh(vector<operacja> operacje)
{
    int najlepszy_czas = 0;
    vector<operacja> najlepsze_operacje;
    stable_sort(operacje.begin(), operacje.end(), [](const operacja &a, const operacja &b)
                {
        int suma_a = accumulate(a.p.begin(),a.p.end(),0);
        int suma_b = accumulate(b.p.begin(),b.p.end(),0);
        
        return suma_a > suma_b;
     });
    for (auto &op : operacje)
    {
        
        int najlepszy_czas_dla_tej_operacji = 999999999;
        int najlepszy_index = 0;

        for (int i = 0; i <= najlepsze_operacje.size(); i++)
        {
            najlepsze_operacje.insert(najlepsze_operacje.begin() + i, op);
            int obliczony_czas = oblicz_czas(najlepsze_operacje);

            if (obliczony_czas < najlepszy_czas_dla_tej_operacji)
            {
                najlepszy_czas_dla_tej_operacji = obliczony_czas;
                najlepszy_index = i;
            }
            najlepsze_operacje.erase(najlepsze_operacje.begin() + i);
        }

        najlepsze_operacje.insert(najlepsze_operacje.begin() + najlepszy_index, op);
        najlepszy_czas = najlepszy_czas_dla_tej_operacji;
    }

    return {najlepszy_czas, najlepsze_operacje};
}



int z_wyczytywaniem()
{
    auto operacje = WczytajDane("neh.data.txt");
    int numer_zestawu = 0;
    auto start = chrono::high_resolution_clock::now();
    for (const auto &operacja_set : operacje)
    {
        cout << endl;
        cout << "Zestaw operacji:" << numer_zestawu << endl;
        for (const auto &op : operacja_set)
        {
            cout << "Operacja " << op.index << ": ";
            for (const auto &czas : op.p)
            {
                cout << czas << " ";
            }
            cout << endl;
        }
        cout << endl;
        numer_zestawu++;

        auto wynik = qneh(operacja_set);
        cout << "Najlepszy czas: " << wynik.first << endl;
        cout << "Kolejność operacji: ";
        for (const auto &op : wynik.second)
        {
            cout << op.index << " ";
        }
        cout << endl;
        if (numer_zestawu == 5)
        {
            break;
        }
    }
    auto end = chrono::high_resolution_clock::now();
    auto czas_trwania = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "Czas trwania programu: " << czas_trwania << " ms" << endl;
    return 0;
}
int bez_wyczytywania()
{
    auto operacje = WczytajDane("neh.data.txt");
    int numer_zestawu = 0;
    auto start = chrono::high_resolution_clock::now();
    for (const auto &operacja_set : operacje)
    {
        auto wynik = qneh(operacja_set);
        cout << "Zestaw operacji:" << numer_zestawu++ << endl;
    }
    auto end = chrono::high_resolution_clock::now();
    auto czas_trwania = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "Czas trwania programu: " << czas_trwania << " ms" << endl;
    return 0;
}
int bez_wyczytywania_porównanie_wyników()
{
    auto operacje = WczytajDane("neh.data.txt");
    int numer_zestawu = 0;
    auto start = chrono::high_resolution_clock::now();
    for (const auto &operacja_set : operacje)
    {
        auto wynik = neh(operacja_set);
        if (wynik.first != wyniki[numer_zestawu].first)
        {
            cout << "Błąd w zestawie operacji: " << numer_zestawu;
            cout << " Oczekiwany wynik: " << wyniki[numer_zestawu].first;
            cout << " Otrzymany wynik: " << wynik.first << endl;
        }

        numer_zestawu++;
    }
    auto end = chrono::high_resolution_clock::now();
    auto czas_trwania = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "Czas trwania programu: " << czas_trwania << " ms" << endl;
    return 0;
}
int main()
{
    z_wyczytywaniem();
    return 0;
}