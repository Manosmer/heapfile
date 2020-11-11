#pragma once

#include "PageHeader.h"


class HeapfileHeader {
    private:
        unsigned int usedPages;
        unsigned int pageCapacity;
        unsigned int maxPages;
        unsigned int pageSize;

        unsigned int dataStartLocation;
        std::fstream *file;

        PageId createPage(int data);
        void deletePage(PageId pid);
        unsigned int calcPosSlot(Rid rid);
    public:
        HeapfileHeader();
        HeapfileHeader(unsigned int maxPages, unsigned int pageCap, std::fstream *file);

        // Getters
        unsigned int getPageCapacity();
        unsigned int getMaxPages();

        void updateFileStream(std::fstream *file);

        
        Rid writeData(PageId pid, int data);
        Rid writeData(int data);

        int readData(Rid record);
};