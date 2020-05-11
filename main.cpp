#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include "Macierz.h"
#include "Solver.h"
#include "My_excepcts.h"
#include "File_read.h"


template<typename T>
void demo3(Macierz<T>& m) {
  m.element(0, 0) =  2.2;
  m.element(0, 1) = -1.5;
  m.element(0, 2) =  0.6;
  m.wolny(0) = 1.3;
  m.element(1, 0) = -0.7;
  m.element(1, 1) =  1.2;
  m.element(1, 2) =  1.8;
  m.wolny(1) = 1.85;
  m.element(2, 0) =  1.2;
  m.element(2, 1) =  0.8;
  m.element(2, 2) = -2.1;
  m.wolny(2) = -4.55;
}

template<typename T>
void demo4(Macierz<T>& m) {
  m.element(0, 0) = -0.2;
  m.element(0, 1) =  1.1;
  m.element(0, 2) =  0.1;
  m.element(0, 3) =  0.9;
  m.wolny(0) = 1.95;
  m.element(1, 0) = -1.2;
  m.element(1, 1) =  3.5;
  m.element(1, 2) = -2.6;
  m.element(1, 3) =  1.9;
  m.wolny(1) = 5.43;
  m.element(2, 0) =  0.8;
  m.element(2, 1) = -2.3;
  m.element(2, 2) =  1.2;
  m.element(2, 3) = -0.8;
  m.wolny(2) = -3.28;
  m.element(3, 0) =  0.2;
  m.element(3, 1) = -1.7;
  m.element(3, 2) =  1.9;
  m.element(3, 3) =  1.3;
  m.wolny(3) = -0.05;
}

void play_demo3() {
  // macierz 1
  try {
    Macierz<double> m1(3);
    demo3<double>(m1);
    m1.pokaz();
    std::cout << std::endl;
    Solver<double> s1(m1);
    s1.rozwiaz();
    m1.pokaz();
  }
  catch (std::out_of_range& ofr){
   std::cout << "macierz(1): "<< ofr.what() << std::endl; 
  }
  catch (std::bad_alloc& ba){
    std::cout << "macierz(1): " << ba.what() << std::endl;
  }
  catch (division_by_zero& me){  // me - my exception
    std::cout  << "macierz(2): " << me.what() << std::endl;
  }
}

void play_demo4()  {
  // macierz 2  
  try {  
  Macierz<double> m2(4);
  demo4<double>(m2);
  m2.pokaz();
  std::cout << std::endl;
  Solver<double> s2(m2);
  s2.rozwiaz();
  m2.pokaz();
  }
  catch (std::out_of_range& ofr){
   std::cout << "macierz(2): "<< ofr.what() << std::endl; 
  }
  catch (std::bad_alloc& ba){
    std::cout << "macierz(2): " << ba.what() << std::endl;
  }
  catch (division_by_zero& me){  // me - my exception
    std::cout  << "macierz(2): " << me.what() << std::endl;
  }
 
}
int main() {

  play_demo3();
  play_demo4();
  read_file_menu();
  return 0;
}

