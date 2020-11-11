#include "PageHeader.h"


PageHeader::PageHeader(PageId id) {
    this->id = id;
    this->usedSlots = 0;
    this->prevPage = 0;
    this->nextPage = 0;
}


unsigned int PageHeader::getUsedSlots() {
    return this->usedSlots;
}

PageId PageHeader::getId() {
    return this->id;
}


Rid PageHeader::getFreeSlot() {
    return Rid(0,0);
}