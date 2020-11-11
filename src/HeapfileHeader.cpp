#include <iostream>

#include "HeapfileHeader.h"
#include "PageHeader.h"


HeapfileHeader::HeapfileHeader() {
    this->usedPages = 0;
    this->pageCapacity = 0;
    this->maxPages = 0;
    this->pageSize = 0;
    this->file = NULL;
    this->dataStartLocation = 0;
}

HeapfileHeader::HeapfileHeader(unsigned int maxPages, unsigned int pageCap, std::fstream *file) {
    this->maxPages = maxPages;
    this->pageCapacity = pageCap;
    this->usedPages = 0;
    this->pageSize = sizeof(PageHeader) + pageCap*sizeof(int);

    this->dataStartLocation = sizeof(HeapfileHeader);
    this->file = file;
    

    createPage(0);
}


// Returns the set capacity of each page measured in slots.
unsigned int HeapfileHeader::getPageCapacity() {
    return pageCapacity;
}

unsigned int HeapfileHeader::getMaxPages() {
    return this->maxPages;
}

void HeapfileHeader::updateFileStream(std::fstream *file) {
    this->file = file;
}


/** 
 * Writes data to file.
 * This method checks if there's enough space to store new
 * data in the given page.
 * 
 * It invokes the corresponding method of the PageHeader of the given page.
 * 
 * @param pid PageId: the output page
 * @param data int: the output data
 * @returns the location of the new data
 * 
 */
Rid HeapfileHeader::writeData(PageId pid, int data) {
    PageHeader phBuffer(0);
    Rid outputLocation;
    unsigned int pos;

    file->seekg(dataStartLocation + pid*pageSize, std::ios_base::beg);
    if(!file->is_open()) {
        throw HeapFileNotOpen();
    }
    file->read((char*) &phBuffer, sizeof(PageHeader));

    // write the actual data
    outputLocation = phBuffer.getFreeSlot();
    pos = calcPosSlot(outputLocation);

    file->seekp(pos, std::ios_base::beg);
    file->write((char*)&data, sizeof(int));

    return outputLocation;
}

Rid HeapfileHeader::writeData(int data) {
    // TODO: finds a page to write the new data and uses the
    // writeData(PageId, int) to write it
    return Rid(0,0);
}



int HeapfileHeader::readData(Rid record) {
    PageHeader phBuffer(0);
    int data;
    unsigned int pos = calcPosSlot(record);

    file->seekg(pos, std::ios_base::beg);
    file->read((char*) &data, sizeof(int));

    return data;
}




/**
 * Creates a new page in the heapfile.
 * 
 * @param data int: the first record contained in the new page
 * @returns the PageId of the new page.
 */
PageId HeapfileHeader::createPage(int data) {
    // For now let's create a page at pid=0
    PageId pid = 0;
    PageHeader *ph = new PageHeader(pid);
    
    if(this->maxPages - this->usedPages <= 0) {
        throw HeapFileOutOfPages();
    }

    if(!file->is_open()) {
        throw HeapFileNotOpen();
    }
    
    this->usedPages++;

    file->seekp(dataStartLocation + pid*pageSize, std::ios_base::beg);
    file->write((char*) ph, sizeof(PageHeader));

    delete ph;

    // TODO
    return pid;
}

// Deletes the page with id pid.
void HeapfileHeader::deletePage(PageId pid) {
    // check if page with pid exists
    // free the page
    
    // then
    if(this->usedPages > 0) {
        this->usedPages--;
    }
}


unsigned int HeapfileHeader::calcPosSlot(Rid rid) {
    return dataStartLocation + rid.pid*pageSize + sizeof(PageHeader) + rid.sid;
}
