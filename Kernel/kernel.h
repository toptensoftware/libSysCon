#include "../libFatFS/ff.h"
#include "../libSysCon/libSysCon.h"

// Shared temp buffer
extern char g_szTemp[256];



#define IS_HI_MEM(p) (((uint16_t)p) & 0x8000)
#define IS_LO_MEM(p) (!IS_HI_MEM(p))
#define CHECK_LO_MEM(p, err) if (IS_HI_MEM(p)) return err;

const TCHAR* marshal_str(const TCHAR* psz);
void free_marshalled_str(const TCHAR* pszOriginal, const TCHAR* pszMarshalled);
void* marshal_buffer(void* pBuff, uint16_t length, bool copy);
void unmarshal_buffer(void* pBuff, void* pMarshal, uint16_t length);

// Unmarshalled version of f_read
FRESULT f_read_no_marshal(FIL* fp, void* buff, UINT btr, UINT* br);
