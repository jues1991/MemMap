#include "memmap.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

MemMap::MemMap() : m_pData(0), m_uSize(0), m_nFile(0)
{
}

MemMap::~MemMap()
{
    unMap();
}

bool MemMap::map(const char* szFileName, const bool &readOnly,void *addr,const bool isShare)
{
    unMap();
    m_nFile = open(szFileName, (true==readOnly)? O_RDONLY : O_RDWR);
    if (m_nFile < 0)
    {
        m_nFile = 0;
        return false;
    }

    struct stat status;
    fstat(m_nFile, &status);

    m_uSize = status.st_size;
    //int pageSize = getpagesize();
    //if ( 0 == (m_uSize % pageSize) )
    {
        m_pData = mmap(addr, m_uSize, (true==readOnly)? PROT_READ : (PROT_READ|PROT_WRITE), (true==isShare)? MAP_SHARED : MAP_PRIVATE, m_nFile, 0);
        if (MAP_FAILED != m_pData)
        {
            this->m_position = (size_t)m_pData;
            return true;
        }
    }

    close(m_nFile);
    m_pData = NULL;
    m_nFile = 0;
    m_uSize = 0;
    return false;
}

void MemMap::unMap()
{
    if(m_pData)
    {
        munmap(m_pData, m_uSize);
        m_pData = NULL;
    }

    if(m_nFile)
    {
        close(m_nFile);
        m_nFile = 0;
    }

    m_uSize = 0;
}
