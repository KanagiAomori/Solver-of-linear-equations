#ifndef SOLVER
#define SOLVER

#include "My_excepcts.h"
#include <iostream>
#include "Macierz.h"

template <typename T>
class Solver 
{
 private:
  Macierz<T>& mat;
  const int n;
  void skalujWiersz(int ktory, double skala);
  void addWiersze(int ktory, int do_ktorego, double skala = 1.0);

 public:
  Solver(Macierz<T>& mat_) : mat(mat_), n(mat.getIleWierszy()) {}

  void rozwiaz();
};



template<typename T>
void Solver<T>::skalujWiersz(int ktory, double skala) {
  // mnozy kazdy element wiersza przez skalar
  for (int i = 0; i < n + 1; ++i) {
    mat.element(ktory, i) *= skala;
  }
}

template<typename T>
void Solver<T>::addWiersze(int ktory, int do_ktorego, double skala) {
  // dodaje pierwszy rzad do drugiego, mnozac wczesniej jego wartosci przez skalar
  for (int i = 0; i < n + 1; ++i) {
    mat.element(do_ktorego, i) += skala * mat.element(ktory, i);
  }
}


template<typename T>
void Solver<T>::rozwiaz() {
  
  for (int i = 0; i < n; ++i) {           // rozwiazuj kazda kolumne z osobna
    double pierwszy = mat.element(i, i);  // znormalizuj dany wiersz (na diagonalnej musza byc same 1)
    if (pierwszy != 0)
      skalujWiersz(i, 1.0 / pierwszy);
    else throw division_by_zero();

    for (int j = 0; j < n; ++j) {         // uzyj i-tego wiersza do wyeliminowania wszystkich pozostalych
      if (i == j) continue;               // nie wyeliminuj siebie
      double skala = -1.0 * mat.element(j, i); // przemnoz wiersz przez te wartosc, by wyeliminowac element na i-tym miejscu
      addWiersze(i, j, skala);
    }
  }
}

#endif