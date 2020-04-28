#ifndef MY_EXPECTS
#define MY_EXPECTS

#include <exception>

struct division_by_zero : public std::exception
{
    const char * what () const throw () { return "division by 0";}
};

struct unknown_type : public std::exception
{
    const char * what () const throw () { return "unknown type";}
};

struct unknown_type_of_file : public std::exception
{
    const char * what () const throw () { return "unknown type of file";}
};

struct unable_to_reading_file : public std::exception
{
    const char * what () const throw () { return "unable to read file";}
};

#endif