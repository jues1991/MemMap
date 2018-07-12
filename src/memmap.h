#ifndef MEMMAP_H
#define MEMMAP_H
#include <iostream>
#include <assert.h>

class MemMap
{
public:
    MemMap();
    ~MemMap();

    bool map(const char* szFileName);
    void unMap();

    void* getData()  { return m_pData; }
    size_t  getSize() const { return m_uSize; }

public:
    template<typename T>
    T* mapNew( void *position )
    {
        char *pos = (char*)position;
        char *begin = (char*)this->m_pData;
        char *end = begin+this->m_uSize;
        //
        if ( begin > position || end < (pos+sizeof(T) ))
        {
            assert(false);
            return (T*)nullptr;
        }
        T *t = (T*)pos;
        //
        new(t) T;
        //
        return t;
    }

    template<typename T>
    void mapDelete( T *t )
    {
        if ( nullptr == t ){
            return;
        }
        //
        t->~T();
        //free(t);
    }


    template<typename T>
    T* mapNewArray( void *position, size_t count )
    {
        char *pos = (char*)position;
        char *begin = (char*)this->m_pData;
        char *end = begin+this->m_uSize;
        //
        if ( begin > position || end < (pos+sizeof(size_t)+sizeof(T)*count ))
        {
            assert(false);
            return (T*)nullptr;
        }
        size_t *p = (size_t*)pos;
        *p = count;
        //
        T *t = (T*)(p+1);
        for (size_t i=0;count>i;i++)
        {
            new(t+i) T;
        }
        //
        return t;
    }

    template<typename T>
    void mapDeleteArray( T *t )
    {
        if ( nullptr == t ){
            return;
        }
        //
        size_t *p = ((size_t*)t)-1;
        size_t count = *p;
        for (size_t i=0;count>i;i++)
        {
            t[i].~T();
        }
        //free(p);
    }
private:
    void*     m_pData;
    size_t    m_uSize;
    int       m_nFile;
};

#endif // MEMMAP_H
