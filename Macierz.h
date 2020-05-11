#ifndef MACIERZ
#define MACIERZ

#include <iomanip>
#include <iostream>
#include <stdexcept>
#include "My_excepcts.h"

template<typename T>
class Macierz {
  private:
    const int ile_wierszy;
    const int dlugosc_wiersza;
    T* dane;
    T** wiersze;
  public:
    Macierz(int n);
    Macierz(const Macierz<T>& other_) = delete;
    void resize(int n);
    ~Macierz();
    void operator=(const Macierz<T>& other_) = delete;
    void alloc(int n);
    void zeruj();
    void pokaz() const; 
    int getIleWierszy() { return ile_wierszy; }
    T& element(int i, int j);
    T& element(int ij);
    T& wolny(int i);
};


template<typename T>
Macierz<T>::Macierz(int n) : ile_wierszy(n), dlugosc_wiersza(n + 1) {  // ustawiamy 2 consty
  this->alloc(n);
}

template<typename T>
void Macierz<T>::alloc(int n) {  // alokacja macierzy kwadratowej [n x n + 1]
  dane = new T[n * (n + 1)]; // throws bad_alloc if fails
  wiersze = new T*[n];
  for (int i = 0; i < n; i++)
    wiersze[i] = dane + i * (n + 1);  // przesuwamy o i * długość wiersza w pamięci || można też &(dane[i * n]);
}

template<typename T>
void Macierz<T>::resize(int n) {
  if(dane != nullptr)
    delete[] dane;
  if(wiersze != nullptr)
    delete[] wiersze;
  this.alloc(n);
}

template<typename T>
Macierz<T>::~Macierz() {
  if(dane != nullptr)
    delete[] dane;
  if(wiersze != nullptr)
    delete[] wiersze;
}

template<typename T>
void Macierz<T>::zeruj() { 
  for (int i = 0; i < ile_wierszy*dlugosc_wiersza; ++i) {
    dane[i] = 0;
  }
}

template<typename T>
void Macierz<T>::pokaz() const {
  using std::cout;
  using std::endl;
  using std::setw;
  using std::fixed;
  using std::setprecision;
  for (int i = 0; i < ile_wierszy; ++i) {
    for (int j = 0; j < dlugosc_wiersza - 1; ++j) {
      cout << setw(8) << setprecision(4) << fixed << wiersze[i][j];
    }
    cout << "  |" << setw(8) << setprecision(4) << wiersze[i][dlugosc_wiersza - 1] << endl;
  }
}

template<typename T>
T& Macierz<T>::element(int i, int j) {
  if (i >= 0 && i < ile_wierszy && j >= 0 && j < dlugosc_wiersza)
   return wiersze[i][j];
  throw std::out_of_range("Przekroczono zakres tablicy 2 wymiarowej (niewiadome)"); 
}

template<typename T>
T& Macierz<T>::element(int ij) {
  if (ij >= 0 && ij < dlugosc_wiersza * ile_wierszy)
   return dane[ij];
  throw std::out_of_range("Przekroczono zakres tablicy 2 wymiarowej (niewiadome)"); 
}

template<typename T>
T& Macierz<T>::wolny(int i) { 
  if( i >= 0 && i < ile_wierszy)
    return wiersze[i][dlugosc_wiersza - 1];
  throw std::out_of_range("Przekroczono zakres tablicy 1 wymiarowej (wyrazy wolne)");//"Wybrano element z poza zakresu tablicy";
}


#endif
