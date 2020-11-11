#include "Heapfile.h"


// Create an empty heapfile object
Heapfile::Heapfile() {
    head = NULL;
    file = NULL;
    numOfPages = 0;
    filename = "";
}

// create new file
Heapfile::Heapfile(unsigned int pageCapacity, unsigned int pagesNo, std::string filename) {
    std::ofstream ff(filename, std::ios::binary);
    ff.close();

    this->file = new std::fstream();


    this->filename = filename;
    this->numOfPages = pagesNo;

    // write header to file
    file->open(filename, std::ios::out | std::ios::in | std::ios::binary);


    if(file->is_open()) {
        // create new header
        this->head = new HeapfileHeader(pagesNo, pageCapacity, file);
        
        file->write((char*) head, sizeof(HeapfileHeader));
    }

    if(!file->good()) {
        throw FileNoGoodException();
    }
}

Heapfile::~Heapfile() {
    close();
    
    delete head;
    delete file;
}


// Open existing hp file and associate it with this object.
void Heapfile::open(std::string filename) {
    this->file = new std::fstream;
    
    this->head = new HeapfileHeader();
    
    // read header from file
    this->filename = filename;

    
    file->open(filename, std::ios_base::out | std::ios_base::in | std::ios_base::binary);
    if(file->is_open()) {
        file->read((char*) this->head, sizeof(HeapfileHeader));
        this->head->updateFileStream(file);
        this->numOfPages = this->head->getMaxPages();
    } else {
        throw FileNoGoodException();
    }   
}

/**
 * @param flag The level of capacity measured. Takes two 
 * values: hpPage, hpFile. 1st returns the capacity of a 
 * single page, 2nd the total capacity of the hp file.
 * @returns the capacity of the heapfile measured in slots
 */
unsigned int Heapfile::getCapacity(CapacityLevel flag) {
    checkOpenOrThrow();

    if(flag == hpPage) {
        return head->getPageCapacity();
    } else if(flag == hpFile) {
        return head->getPageCapacity() * numOfPages;
    }
}



/**
 * Writes the given data to heapfile.
 * This function invokes the corresponding writeData(int) 
 * method of the heapfileHeader.
 * 
 * @param data int: the data to be stored
 * @returns the file location of the new data
 */
Rid Heapfile::writeData(int data) {
    checkOpenOrThrow();

    // We will use page 0 for now
    return this->head->writeData(0, data);
}


int Heapfile::readData(Rid record) {
    checkOpenOrThrow();

    return head->readData(record);
}


void Heapfile::updateHeader() {
    if(file->is_open()) {
        file->write((char*) head, sizeof(HeapfileHeader));
    }
}

void Heapfile::close() {
    if(!file) {
        return;
    }

    updateHeader();
    if(file->is_open()) {
        file->close();
    } 
}


void Heapfile::checkOpenOrThrow() {
    if(!file) {
        throw HeapFileNotOpen();
    } else if(!file->is_open()) {
        throw HeapFileNotOpen();
    }
}