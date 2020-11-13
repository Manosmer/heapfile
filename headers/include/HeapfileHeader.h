#pragma once

#include "Page.h"


class HeapfileHeader {
    private:
        unsigned int currPages;
        unsigned int pageCapacity;
        unsigned int maxPages;
        unsigned int pageSize;

        unsigned int dataStartLocation;
        std::fstream *file;

        PageId fullListPointer; // points to list of full pages
        PageId freeListPointer; // points to list of pages with available free space

        PageId createPage();
        void deletePage(PageId pid);
        
        unsigned int calcPagePosition(PageId pid);
        Rid writeData(PageId pid, int data);
    public:
        HeapfileHeader();
        HeapfileHeader(unsigned int maxPages, unsigned int pageCap, std::fstream *file);

        // Getters
        unsigned int getPageCapacity();
        unsigned int getMaxPages();

        void setFileStream(std::fstream *file);

        
        Rid writeData(int data);

        int readData(Rid record);

        void deleteSlot(Rid record);
};