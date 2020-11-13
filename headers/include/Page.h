#pragma once

#include <fstream>

#include "myexceptions.h"

typedef int PageId;
typedef unsigned int SlotId;

struct Rid {
    PageId pid;
    SlotId sid;
    
    Rid(): pid{0}, sid{0} {}
    Rid(PageId p, SlotId s): pid{p}, sid{s} {}

    Rid& operator=(const Rid& r) {
        pid = r.pid;
        sid = r.sid;

        return *this;
    }
    
    bool operator==(Rid r) {
        return (pid == r.pid && sid == r.sid);
    }

    bool operator!=(Rid r) {
        return (pid != r.pid || sid != r.sid);
    }
};

class Page {
    private:
        PageId id;
        PageId prevPage;
        PageId nextPage;

        unsigned int maxSlots;
        unsigned int usedSlots;

        bool dirtyBit;
        
        bool fullBit;

        // unpacked version with bitmap
        bool *validMap;
        int *data;

    public:
        Page();
        Page(PageId id, unsigned int maxSlots);

        ~Page();

        unsigned int sizeInDisk();


        void writePage(unsigned int readPosition, std::fstream *file);
        void readPage(unsigned int readPosition, std::fstream *file);

        PageId getId();
        unsigned int getUsedSlots();

        void free();

        bool isFull();

        PageId getPrev();
        void setPrev(PageId prev);

        PageId getNext();
        void setNext(PageId next);

        Rid writeData(int el);
        int readData(SlotId sid);

        void deleteSlot(SlotId sid);
};