#include <iostream>

#include "Heapfile.h"


int main(int argc, char* argv[])
{
    Heapfile *hp;
    try {
        hp = new Heapfile(10,1,"hp");
        // hp->open("hp");
        hp->writeData(30);
        hp->writeData(40);
        hp->deleteSlot(Rid(0,1));
        hp->writeData(50);

        std::cout << "DATA IN FILE (PAGE=0,SLOT=0): " << hp->readData(Rid(0,1)) << std::endl;
    } catch(std::exception &e) {
        std::cout << e.what() << std::endl;
    } catch(const char* e) {
        std::cout << e << std::endl;
    }

    if(hp != NULL) delete hp;
    
    system("pause");
    
    return 0;
}
