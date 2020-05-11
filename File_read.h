#include <iostream>
#include "Macierz.h"
#include "Solver.h"
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>

enum filetype {macierz, macierz_pro, uklad};
enum matrixtype {doub, floa};

matrixtype def_type_of_matrix(std::ifstream& fp);
filetype def_type_of_file(std::string fileName);

int def_size_of_matrix(std::ifstream& fp);
void set_file_name(std::string& filename);
void read_file_menu();

bool isNumber(char c);  // char helpers
bool newNumber(char c);
bool newWolny(char c);
bool isVariable(char c);


template<typename T>
void read_uklad(std::ifstream& fp, int size_of_matrix, matrixtype type_of_matrix_to_read) {
    std::string buffer;
    Macierz<T> m(size_of_matrix);
    fp.seekg(2, std::ios::beg);
    for (std::string smallbuff; std::getline(fp, smallbuff); ){
        buffer.append(smallbuff);
    }
    std::remove(buffer.begin(), buffer.end(), ' ');
    std::remove(buffer.begin(), buffer.end(), '*');
    std::cout << buffer;
    std::vector<T> temp;
    int histogram[32];
    int i, j;
    std::vector<char> nazwy_zmiennych;
    for (i = 0; buffer[i] != '='; i++)                      // sposób na zapamiętanie nazw zmiennych
        if (buffer[i] >= 'a' && buffer[i] <= ('a' + 32))    // bardziej pracochłonna metoda histogram
            nazwy_zmiennych.push_back(buffer[i]);
    std::cout << "nazwy niewiadomych: ";
    for (char nazwa : nazwy_zmiennych)   // wyświetlenie zmiennych
        std::cout << nazwa << ", ";
    std::cout << std::endl; 
    for (char nazwa : nazwy_zmiennych)  // usun nazwy zmiennych z buffora
        std::remove(buffer.begin(), buffer.end(), nazwa);
    std::cout << buffer << std::endl;

    if (type_of_matrix_to_read = floa){
        for (int idx = 0, i = 0; i  < size_of_matrix * (size_of_matrix + 1); idx++){
            //if (buffer[idx] == '+' || buffer[idx] == '=')
            //    idx++;
            std::cout << "******" << i << "**" << idx << "****" << std::endl;
            if (isNumber(buffer[idx])){
                m.element(i++) = std::stof(buffer.substr(idx));
            if(buffer[idx] == '=')
                m.element(i++) = std::stof(buffer.substr(idx));
            }
        }
    }
    else {
        ;
    }
    m.pokaz();

    // rozwiazanie
    /*
    i = 0;
    for(char nazwa : nazwy_zmiennych){
        std::cout << nazwa << "= " << m.wolny(i++) << std::endl; 
    }
    */
}

// działa <3
template< typename T >
std::vector<T> read_macierz_to_vector(std::ifstream& fp) {
    T number;
    std::vector<T> temp;
    fp.seekg(2, std::ios::beg);
    while(fp.peek() != EOF) {
        fp >> number;
        temp.push_back(number);
    }
    return temp;
}

// działa <3
template <typename T>
void read_vector_to_macierz(std::ifstream& fp, int size_of_matrix, filetype type_of_file_to_read, matrixtype type_of_matrix_to_read){
    int i, j;
    if(type_of_file_to_read == uklad) {
        read_uklad<T>(fp, size_of_matrix, type_of_matrix_to_read);
    }
    else if(type_of_file_to_read == macierz) {
        std::vector<T> temp;
        temp = read_macierz_to_vector<T>(fp);   
        // or read macierz to vector
        int vecIndex = 0;
        Macierz<T> m(size_of_matrix);
        int n = size_of_matrix * (size_of_matrix + 1);
        for (i = 0; i < n; i++){
                    m.element(i) = temp[vecIndex++];
        }
        std::cout << "macierz przed rozwiazaniem: \n";
        m.pokaz();
        std::cout << std::endl << "macierz po rozwiazaniu \n";
        Solver<T> s(m);
        s.rozwiaz();
        m.pokaz();
        std::cout << std::endl;
    }
}


/*
template<typename T>
void read_uklad_pro() {
    int i;
    int uklad_size = 0;
    char variable_table[32];
    char variable_histogram[32];
    for(i = 0; i < 32; i++)
        variable_table[i] = 'a' + i;
}
*/

////////////////////////////////////////////////////
// MENU FOR READ                                  //
////////////////////////////////////////////////////
void read_file_menu() { 
  std::string fileName;
  std::ifstream fp;
  
  set_file_name(fileName);  // write in name of file
  std::cout << "Sciezka: " << fileName << std::endl;
  fp.open(fileName, std::ios::in);  // read mode
  if (!(fp.is_open())) { // check if file is open
      throw unable_to_reading_file();
  }

  // early solivng configurations of file and matrix
  matrixtype type_of_matrix_to_read = def_type_of_matrix(fp);   // interpretation of type of matrix
  filetype type_of_file_to_read = def_type_of_file(fileName);   // interpreting name of file
  size_t size_of_matrix = def_size_of_matrix(fp);               // acknowlage of size of matrix
  std::cout << "size: " << size_of_matrix << "|| type of matrix: " << type_of_matrix_to_read;
  std::cout << "type of file: " << type_of_file_to_read << std::endl;
  // create matrix and sovle depending on the type
    if (type_of_matrix_to_read == doub){
      read_vector_to_macierz<double> (fp, size_of_matrix, type_of_file_to_read, type_of_matrix_to_read);
    }
    else if (type_of_matrix_to_read == floa){
      read_vector_to_macierz<float>(fp, size_of_matrix, type_of_file_to_read, type_of_matrix_to_read);
    }
  fp.close();
}


void set_file_name(std::string& filename) {
    std::string path("./Przykladowe_macierze/");
    std::string name;
    bool reAsk = true;
    do {
        std::cout << "nazwa pliku do otwarcia: " << std::endl;
        std::cin >> name;
        if (std::cin.fail() == true){
            std::cout << "Wykryto wprowadzenie błednych danych" << std::endl;
            std::cin.clear();
            std::cin.ignore(100 , '\n');
        }
        else reAsk = false;
    } while(reAsk);
    path.append(name + ".txt");
    filename = path;
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
    std::size_t found;

    found = filename.find(p);
    if (found != std::string::npos)
        return macierz_pro;

    found = filename.find(u);
    if (found != std::string::npos)
        return uklad;

    found = filename.find(m);
    if (found != std::string::npos)
        return macierz;

    throw unknown_type_of_file();
}

int def_size_of_matrix(std::ifstream& fp) {
  std::string line;
  int count = 0;
  fp.seekg(2, std::ios::beg);  /// ifstream ustaw na początek pliku
  std::getline(fp, line);
  count += std::count(line.begin(), line.end(), '.');
  count += std::count(line.begin(), line.end(), ',');
  count--;
  return count; // count^matrix
}

bool isNumber(char c) {
    if( (c >= '0') && (c <= '9') || c == ',' || c == '.' || c == '-')
        return true;
    else return false;
}

bool isVariable(char c) {
    if(c >= 'a' && c <= 'a' + 32)
        return true;
    else return false;

}

bool newNumber(char c) { // następna liczba do wczytania do macierzy
    if( c =='+' || c == '=')
        return true;
    else return false;
}

bool newWolny(char c) {  // następny wyraz to wolny
    if(c == '=')
        return true;
    else return false;
} 