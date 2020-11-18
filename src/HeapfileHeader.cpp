#include <iostream>
#include <string>

#include "HeapfileHeader.h"
#include "Page.h"


HeapfileHeader::HeapfileHeader() {
    this->currPages = 0;
    this->pageCapacity = 0;
    this->maxPages = 0;
    this->pageSize = 0;
    this->file = NULL;
    this->dataStartLocation = 0;
    this->fullListPointer = -1;
    this->freeListPointer = -1;
}

HeapfileHeader::HeapfileHeader(unsigned int maxPages, unsigned int pageCap, std::fstream *file) {

    this->maxPages = maxPages;
    this->pageCapacity = pageCap;
    this->currPages = 0;
    this->pageSize = Page(-1, this->pageCapacity).sizeInDisk();

    this->dataStartLocation = sizeof(HeapfileHeader);
    this->file = file;

    createPage();

    this->freeListPointer = 0;
    this->fullListPointer = -1;
}


// Returns the set capacity of each page measured in slots.
unsigned int HeapfileHeader::getPageCapacity() {
    return pageCapacity;
}

unsigned int HeapfileHeader::getMaxPages() {
    return this->maxPages;
}

void HeapfileHeader::setFileStream(std::fstream *file) {
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
    Page pBuffer;
    Rid outputLocation;

    if(!file->is_open()) {
        throw HeapFileNotOpen();
    }
    
    pBuffer.readPage(calcPagePosition(pid), file);
    outputLocation = pBuffer.writeData(data);

    // if page becomes full add it to the full pages list
    if(pBuffer.isFull()) {
        PageId prevId = pBuffer.getPrev(), nextId = pBuffer.getNext();
        Page previousPage, nextPage, fullhead;


        // change previousPage's next pointer
        if(pBuffer.getId() != freeListPointer) {
            // if pBuffer is not the head of the free space list

            previousPage.readPage(calcPagePosition(prevId), file);
            previousPage.setNext(nextId);
            previousPage.writePage(calcPagePosition(prevId), file);
            
        } else {
            freeListPointer = nextId;
        }

        // change nextPage's previous pointer
        if(nextId != -1) {
            // if pBuffer is not the end of the free space list

            nextPage.readPage(calcPagePosition(nextId), file);
            nextPage.setPrev(prevId);
            nextPage.writePage(calcPagePosition(nextId), file);
        }

        // change full list head's previous pointer
        if(fullListPointer != -1) {
            fullhead.readPage(calcPagePosition(fullListPointer), file);
            fullhead.setPrev(pBuffer.getId());
            fullhead.writePage(calcPagePosition(fullListPointer), file);
        }

        pBuffer.setPrev(-1);
        pBuffer.setNext(fullListPointer);
        fullListPointer = pBuffer.getId();
    }
    pBuffer.writePage(calcPagePosition(pid), file);

    return outputLocation;
}


Rid HeapfileHeader::writeData(int data) {
    if(freeListPointer == -1) {
        createPage();
        freeListPointer = this->currPages - 1;
    }
    return writeData(freeListPointer, data);
}



int HeapfileHeader::readData(Rid record) {
    Page pBuffer;

    if(record.pid >= currPages) {
        throw "No page with that pid";
    }

    pBuffer.readPage(calcPagePosition(record.pid), file);

    return pBuffer.readData(record.sid);
}

void HeapfileHeader::deleteData(Rid record) {
    Page pBuffer;
    if(record.pid >= this->currPages) {
        throw "No page with that pid";
    }

    pBuffer.readPage(calcPagePosition(record.pid), file);

    if(pBuffer.isFull()) {
    // TODO: check if page was full and add it to free space list
        PageId prevId = pBuffer.getPrev(), nextId = pBuffer.getNext();
        Page previous, next, freeheader;

        if(nextId != -1) {
            // if next exists change its prev
            next.readPage(calcPagePosition(nextId), file);
            next.setPrev(prevId);
            next.writePage(calcPagePosition(nextId), file);
        }

        if(pBuffer.getId() != fullListPointer) {
            // if pBuffer is not the full list head
            previous.readPage(calcPagePosition(prevId), file);
            previous.setNext(nextId);
            previous.writePage(calcPagePosition(prevId), file);
        }

        if(freeListPointer != -1) {
            // if free list is not empty
            freeheader.readPage(calcPagePosition(freeListPointer), file);
            freeheader.setPrev(pBuffer.getId());
            freeheader.writePage(calcPagePosition(freeListPointer), file);
        }

        pBuffer.setPrev(-1);
        pBuffer.setNext(freeListPointer);
        freeListPointer = pBuffer.getId();
    }

    pBuffer.freeSlot(record.sid);
    pBuffer.writePage(calcPagePosition(record.pid), file);
}


/**
 * Creates a new page in the heapfile.
 * 
 * @returns the PageId of the new page.
 */
PageId HeapfileHeader::createPage() {
    unsigned int pid = this->currPages;
    Page *p = new Page(pid, this->pageCapacity);


    if(this->maxPages - this->currPages <= 0) {
        throw HeapFileOutOfPages();
    }

    if(!file->is_open()) {
        throw HeapFileNotOpen();
    }
    
    this->currPages++;

    p->writePage(calcPagePosition(pid), file);
    delete p;

    return pid;
}

// Deletes the page with id pid.
void HeapfileHeader::deletePage(PageId pid) {
    // check if page with pid exists
    // free the page
    Page pBuffer;
    if(pid >= this->currPages) {
        // pid is 0...currPages-1
        throw "No page with that pid";
    }

    pBuffer.readPage(calcPagePosition(pid), file);
    pBuffer.free();
    // TODO: change things if it was in the full list
    pBuffer.writePage(calcPagePosition(pid), file);
}


unsigned int HeapfileHeader::calcPagePosition(PageId pid) {
    return dataStartLocation + pid*pageSize;
}
