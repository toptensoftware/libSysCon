#include "common.h"


const TCHAR* marshal_str(const TCHAR* psz)
{
    if (IS_HI_MEM(psz))
    {
        return stralloc(psz);
    }

    return psz;
}

void free_marshalled_str(const TCHAR* pszOriginal, const TCHAR* pszMarshalled)
{
    if (pszMarshalled != pszOriginal)
    {
        free(pszMarshalled);
    }
}

void* marshal_buffer(void* pBuff, uint16_t length, bool copy)
{
    if (IS_LO_MEM(pBuff))
        return pBuff;

    void* p = malloc(length);
    if (copy)
        memcpy(p, pBuff, length);
    return p;
}

void unmarshal_buffer(void* pBuff, void* pMarshal, uint16_t length)
{
    if (pBuff != pMarshal)
    {
        if (length)
            memcpy(pBuff, pMarshal, length);
        free(pMarshal);
    }
}
