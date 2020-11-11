#include <iostream>

#include "Heapfile.h"


int main(int argc, char* argv[])
{
    Heapfile *hp;
    try{
        hp = new Heapfile(10, 10, "hp");
        hp->writeData(30);
        std::cout << "DATA IN FILE (PAGE=0,SLOT=0): " << hp->readData(Rid(0,0)) << std::endl;
    }catch(std::exception &e) {
        std::cout << e.what() << std::endl;
    }

    

    if(!hp) delete hp;



    system("pause");
    
    return 0;
}
