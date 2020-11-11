#pragma once

#include <fstream>

#include "myexceptions.h"

typedef unsigned int PageId;
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

class PageHeader {
    private:
        unsigned int usedSlots;
        PageId id;

        PageId prevPage;
        PageId nextPage;
    public:
        PageHeader(PageId id);

        unsigned int getUsedSlots();
        PageId getId();

        Rid getFreeSlot();
};