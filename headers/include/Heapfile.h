#pragma once

#include <string>

#include "HeapfileHeader.h"


enum CapacityLevel {
    hpPage,
    hpFile
};

class Heapfile {
    private:
        HeapfileHeader *head;
        std::string filename;
        std::fstream *file;

        unsigned int numOfPages;

        void updateHeader();
        void checkOpenOrThrow();
    public:
        // Constructors
        Heapfile();
        Heapfile(unsigned int pageCapacity, unsigned int pagesNo, std::string filename);

        //Destructor
        ~Heapfile();


        void open(std::string filename);
    
        unsigned int getCapacity(CapacityLevel flag);

        Rid writeData(int data);
        int readData(Rid record);


        void close();
};