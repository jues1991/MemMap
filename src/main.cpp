#include <string.h>
#include <stdio.h>
#include "memmap.h"

static int g_i = 0;
class test1
{
public:
    test1()
    {
        g_i++;
        this->i = g_i;
        std::cout<<"test1::test1()"<<" "<<i<<std::endl;
    }
    ~test1()
    {
        std::cout<<"test1::~test1()"<<" "<<i<<std::endl;
    }
protected:
    int i;
};

int main(int argc, char *argv[])
{
    MemMap m;

    //
    if ( false == m.map("../MemMapDemo/1.txt",false) ){
        return false;
    }
    //
    char *data = (char*)m.getData();
    //size_t size = m.getSize();
    //
    test1 *t1 = m.mapNewArray<test1>(data,10);
    //
    m.mapDeleteArray(t1);
}
