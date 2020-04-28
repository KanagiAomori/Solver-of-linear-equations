#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include "Macierz.h"
#include "Solver.h"
#include "My_excepcts.h"


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

int main() {

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




  return 0;
}

enum filetype {macierz, macierz_pro, uklad};
enum matrixtype {doub, floa};

void read_matrix_menu(std::string fileName);

template <typename T>
void read_matrix_file(std::ifstream& fp);
template< typename T >
void read_matrix_pro_file(std::ifstream& fp);
void read_uklad_file();

matrixtype def_type_of_matrix(std::ifstream& fp);
filetype def_type_of_file(std::string fileName);


void read_matrix_menu(std::string fileName) { // interpretation of type of matrix
  std::ifstream fp;
  fp.open(fileName, std::ios::in);  // tryb czytania

  matrixtype type_of_matrix_to_read = def_type_of_matrix(fp);
  filetype type_of_file_to_read = def_type_of_file(fileName);

  switch (type_of_file_to_read)
  {
    case macierz:
      if(type_of_matrix_to_read == doub)
        read_matrix_file<double>(fp);
      else if(type_of_matrix_to_read == floa)
        read_matrix_file<float>(fp);
      break;
    case macierz_pro:
      break;
    case uklad:
      break;
    default:
        throw unknown_type_of_file();
      break;
  }
  throw unable_to_reading_file();
  fp.close();
}

matrixtype def_type_of_matrix(std::ifstream& fp) {
  char F_or_D = fp.get();
  if(F_or_D == 'D')
    return doub;  // type of double
  if(F_or_D == 'F')
    return floa;  // type of float
  throw unknown_type_of_file();
}

filetype def_type_of_file(std::string filename) { // ustala formę pliku na podstawie nazwy
  std::string m("macierz"), p("pro"), u("uklad");

  std::size_t found = filename.find(macierz);
  if (found!=std::string::npos)
    return macierz;

  found = filename.find(p);
  if (found!=std::string::npos)
    return macierz_pro;

  found = filename.find(u);
  if (found!=std::string::npos)
    return uklad;
  //throw unknown_type_of_file("nieznany typ pliku - [def_type_of_file]");
}

template< typename T >
Macierz<T> read_matrix_file(std::ifstream& fp) {  // czytanie pliku z macierzą
  int n, j, i;
  T number;
  std::vector<T> temp;  // useless
  std::string line;
  fp.seekg(std::ios::beg);  // set begining of file
  std::getline(fp, line); // skip first format line
  std::getline(fp, line);
  n = std::count(temp.begin(), temp.end(), ' ');
  // odczytywanie po wierszu w tym miejscu jest bez sensu bo znamy już rozmiar macierzy
  Macierz<T> odczytanaMacierz(n);
  int iloscDanych = odczytanaMacierz.getIleWierszy() * (odczytanaMacierz.getIleWierszy() + 1);
  fp.seekg(2, std::ios::beg);  /// ifstream ustaw na początek pliku
  for (j = 0; j < n; j++)
    for (i = 0; fp >> number && number > 0; i++)
        temp.push_back(number);
  
  Solver<T> s(odczytanaMacierz);
  s.rozwiaz();
  //return odczytanaMacierz;
}

template< typename T >
Macierz<T> read_matrix_pro_file(std::ifstream& fp) {

}