#ifndef MEMMAP_H
#define MEMMAP_H
#include <iostream>
#include <assert.h>

using namespace std;

class MemMap
{
public:
    MemMap();
    ~MemMap();

    bool map(const char* szFileName,void *addr = nullptr, const bool is_share = true);
    void unMap();

    void* getData()  { return this->m_pData; }
    size_t  getSize() const { return this->m_uSize; }

    size_t position() const { return this->m_position; }

    void setPosition2( size_t *position2 ){ this->m_position = *position2; this->m_position2 = position2; }
public:
    //mapNew
    template<typename T,typename ... A>
    T* mapNew( void *position,A ...args )
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
        new(t) T(args...);
        //
        return t;
    }
    //
    template<typename T,typename ... A>
    T* mapNew(A ... args)
    {
        if ( nullptr != this->m_position2 )
        {
            this->m_position = (*this->m_position2);
        }
        T *t= this->mapNew<T>((void*)this->m_position,args...);
        //
        if ( nullptr != t )
        {
            this->m_position += sizeof(T);
            if ( nullptr != this->m_position2 )
            {
                (*this->m_position2) = this->m_position;
            }
        }
        //
        return t;
    }

    //mapDelete
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


    //mapNewArray
    template<typename T,typename ... A>
    T* mapNewArray( void *position, size_t count, A ... args )
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
            new(t+i) T(args...);
        }
        //
        return t;
    }

    //
    template<typename T,typename ... A>
    T* mapNewArray( size_t count, A ... args )
    {
        if ( nullptr != this->m_position2 )
        {
            this->m_position = (*this->m_position2);
        }

        T *t= this->mapNewArray<T,A...>((void*)this->m_position,count,args...);

        if ( nullptr != t )
        {
            this->m_position += sizeof(size_t)+sizeof(T)*count;
            if ( nullptr != this->m_position2 )
            {
                (*this->m_position2) = this->m_position;
            }
        }
        //
        return t;
    }

    //mapDeleteArray
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
    size_t     m_position;
    size_t     *m_position2;

    void*     m_pData;
    size_t    m_uSize;
    int       m_nFile;
};

#endif // MEMMAP_H
