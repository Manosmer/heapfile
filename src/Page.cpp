#include <iostream>

#include "Page.h"


Page::Page() {
    this->id = -1;
    this->prevPage = -1;
    this->nextPage = -1;
    this->maxSlots = 0;
    this->usedSlots = 0;

    this->dirtyBit = false;
    this->fullBit = false;

    validMap = NULL;
    data = NULL;
}

Page::Page(PageId id, unsigned int maxSlots) {
    this->id = id;
    this->prevPage = -1;
    this->nextPage = -1;
    this->maxSlots = maxSlots;
    this->usedSlots = 0;

    this->dirtyBit = false;
    this->fullBit = false;

    validMap = new bool[maxSlots];
    for(unsigned int i = 0; i < maxSlots; i++)
        validMap[i] = false;

    data = new int[maxSlots];
}

Page::~Page() {
    if(data != NULL) {
        delete[] data;
    }

    if(validMap != NULL) {
        delete[] validMap;
    }
}

unsigned int Page::sizeInDisk() {
    return sizeof(PageId) * 3 + sizeof(unsigned int) * 2 + sizeof(bool) * (1 + this->maxSlots) + sizeof(int) * this->maxSlots;
}


void Page::writePage(unsigned int readPosition, std::fstream *file) {
    this->dirtyBit = false;

    file->seekp(readPosition, std::ios_base::beg);

    file->write((char*) &this->id, sizeof(PageId));
    file->write((char*) &this->prevPage, sizeof(PageId));
    file->write((char*) &this->nextPage, sizeof(PageId));

    file->write((char*) &this->maxSlots, sizeof(unsigned int));
    file->write((char*) &this->usedSlots, sizeof(unsigned int));

    file->write((char*) &this->fullBit, sizeof(bool));

    file->write((char*) this->validMap, sizeof(bool)*this->maxSlots);

    file->write((char*) this->data, sizeof(int)*this->maxSlots);
}

void Page::readPage(unsigned int readPosition, std::fstream *file) {
    file->seekg(readPosition, std::ios_base::beg);

    file->read((char*) &this->id, sizeof(PageId));
    file->read((char*) &this->prevPage, sizeof(PageId));
    file->read((char*) &this->nextPage, sizeof(PageId));

    file->read((char*) &this->maxSlots, sizeof(unsigned int));
    file->read((char*) &this->usedSlots, sizeof(unsigned int));

    file->read((char*) &this->fullBit, sizeof(bool));

    validMap = new bool[this->maxSlots];
    file->read((char*) this->validMap, sizeof(bool)*this->maxSlots);

    data = new int[this->maxSlots];
    file->read((char*) this->data, sizeof(int)*this->maxSlots);
}


PageId Page::getId() {
    return this->id;
}

unsigned int Page::getUsedSlots() {
    return this->usedSlots;
}


void Page::free() {
    fullBit = false;
    usedSlots = 0;
    this->dirtyBit = true;

    // make all slots invalid
    for(unsigned int i = 0; i < this->maxSlots; i++) {
        validMap[i] = false;
    }
}


bool Page::isFull() {
    return this->fullBit;
}

PageId Page::getPrev() {
    return this->prevPage;
}

void Page::setPrev(PageId prev) {
    this->prevPage = prev;
    this->dirtyBit = true;
}

PageId Page::getNext() {
    return this->nextPage;
}

void Page::setNext(PageId next) {
    this->nextPage = next;
    this->dirtyBit = true;
}

Rid Page::writeData(int el) {
    SlotId sid;
    unsigned int i;

    if(fullBit) {
        throw "Trying to write data in a full page";
    }
    
    i = 0;
    while(validMap[i] == true && i < maxSlots) {
        i++;
    }
    sid = i;

    validMap[sid] = true;
    data[sid] = el;
    this->dirtyBit = true;
    
    this->usedSlots++;
    if(usedSlots == maxSlots) fullBit = true;
    return Rid(this->id, sid);
}

int Page::readData(SlotId sid) {
    if(sid >= maxSlots) {
        throw PageAccessOutOfBounds();
    }

    if(!validMap[sid]) {
        throw "Slot is empty";
    }

    return data[sid];
}


void Page::deleteSlot(SlotId sid) {
    if(sid >= maxSlots) {
        throw "Trying to delete unused slot";
    }

    // if the slot is already free just return
    if(validMap[sid] == false) return;

    validMap[sid] = false;
    this->usedSlots--;
    if(fullBit) fullBit = false;

    this->dirtyBit = true;
}