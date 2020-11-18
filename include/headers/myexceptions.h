#pragma once

#include <exception>


struct HeapFileOutOfSpace: public std::exception {
    const char* what() const throw() {
        return "Heap file is full";
    }
};

struct HeapFileOutOfPages: public std::exception{
    const char* what() const throw() {
        return "Heap file out of pages";
    }
};

struct PageAccessOutOfBounds: public std::exception{
    const char* what() const throw() {
        return "Accessing page out of bounds";
    }
};

struct FileNoGoodException: public std::exception{
    const char* what() const throw() {
        return "File is not in good state";
    }
};

struct HeapFileNotOpen: public std::exception{
    const char* what() const throw() {
        return "The heap file object has not opened or is not associated with any storage file";
    }
};