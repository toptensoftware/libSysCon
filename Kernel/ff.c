#include "common.h"

FRESULT imp_f_init();
FRESULT imp_f_mount(FATFS* fs,const TCHAR* path,BYTE opt);
FRESULT imp_f_open(FIL* fp, const TCHAR* path, BYTE mode);
FRESULT imp_f_close(FIL* fp);
FRESULT imp_f_read(FIL* fp, void* buff, UINT btr, UINT* br);
FRESULT imp_f_write(FIL* fp, const void* buff, UINT btw, UINT* bw);
FRESULT imp_f_lseek(FIL* fp, FSIZE_t ofs);
FRESULT imp_f_current_sector(FIL* fp, LBA_t* psector);
FRESULT imp_f_create_sector(FIL* fp, LBA_t* psector);
FRESULT imp_f_unlink(const TCHAR* path);
FRESULT imp_f_rename(const TCHAR* path_old, const TCHAR* path_new);
FRESULT imp_f_opendir(DIR* dp, const TCHAR* path);
FRESULT imp_f_closedir(DIR *dp);
FRESULT imp_f_readdir(DIR* dp, FILINFO* fno);
FRESULT imp_f_mkdir(const TCHAR* path);
FRESULT imp_f_truncate(FIL* fp);
int imp_pattern_matching(const TCHAR* pat, const TCHAR* nam, int skip, int inf);
void* imp_malloc(size_t size);
void imp_free(void* ptr);
void* imp_realloc(void* ptr, size_t size);

// Helper to switch banks, call the function and switch back.
#define FF_CALL(r) \
    ENTER_BANK(BANK_FATFS) \
    FRESULT res = (r); \
    LEAVE_BANK()

#define FF_CHECK_LO_MEM(x) CHECK_LO_MEM(x, FR_INVALID_PARAMETER)


FRESULT f_init()
{
    FF_CALL(imp_f_init());
    return 0;
}


// NB: path not marshalled, must be in low mem
FRESULT f_mount(FATFS* fs, const TCHAR* path, BYTE opt)
{
    // Marshal
    FF_CHECK_LO_MEM(fs);
    FF_CHECK_LO_MEM(path);

    // Invoke
    FF_CALL(imp_f_mount(fs, path, opt));

    // Clean up
    return res;
}

FRESULT f_open(FIL* fp, const TCHAR* path, BYTE mode)
{
    // Marshal
    FF_CHECK_LO_MEM(fp);
    const TCHAR* m_path = marshal_str(path);

    // Invoke
    FF_CALL(imp_f_open(fp, m_path, mode));

    // Clean up
    free_marshalled_str(path, m_path);
    return res;
}

FRESULT f_close(FIL* fp)
{
    FF_CHECK_LO_MEM(fp);
    FF_CALL(imp_f_close(fp));
    return res;
}

FRESULT f_read_no_marshal(FIL* fp, void* buff, UINT btr, UINT* br)
{
   FF_CALL(imp_f_read(fp, buff, btr, br));
   return res;
}

FRESULT f_read(FIL* fp, void* buff, UINT btr, UINT* br)
{
    // Marshal
    FF_CHECK_LO_MEM(fp);
    void* m_buff = marshal_buffer(buff, btr, false);
    UINT m_br;

    // Invoke
    FF_CALL(imp_f_read(fp, buff, btr, &m_br));

    // Clean up
    unmarshal_buffer(buff, m_buff, m_br);
    *br = m_br;
    return res;
}

FRESULT f_write(FIL* fp, const void* buff, UINT btw, UINT* bw)
{
    // Marshal
    FF_CHECK_LO_MEM(fp);
    void* m_buff = marshal_buffer((void*)buff, btw, true);
    UINT m_bw;

    // Invoke
    FF_CALL(imp_f_write(fp, m_buff, btw, &m_bw));

    // Clean up
    unmarshal_buffer((void*)buff, m_buff, 0);
    *bw = m_bw;
    return res;
}

FRESULT f_lseek(FIL* fp, FSIZE_t ofs)
{
    FF_CHECK_LO_MEM(fp);
    FF_CALL(imp_f_lseek(fp, ofs));
    return res;
}

FRESULT f_current_sector(FIL* fp, LBA_t* psector)
{
    // Marshal
    FF_CHECK_LO_MEM(fp);
    LBA_t m_sector;

    // Invoke
    FF_CALL(imp_f_current_sector(fp, &m_sector));

    // Clean up
    *psector = m_sector;
    return res;
}

FRESULT f_create_sector(FIL* fp, LBA_t* psector)
{
    // Marshal
    FF_CHECK_LO_MEM(fp);
    LBA_t m_sector;

    // Invoke
    FF_CALL(imp_f_create_sector(fp, &m_sector));

    // Clean up
    *psector = m_sector;
    return res;
}

FRESULT f_unlink(const TCHAR* path)
{
    // Marshal
    const TCHAR* m_path = marshal_str(path);

    // Invoke
    FF_CALL(imp_f_unlink(m_path));

    // Clean up
    free_marshalled_str(path, m_path);
    return res;
}

FRESULT f_rename(const TCHAR* path_old, const TCHAR* path_new)
{
    // Marshal
    const TCHAR* m_path_old = marshal_str(path_old);
    const TCHAR* m_path_new = marshal_str(path_new);

    // Invoke
    FF_CALL(imp_f_rename(m_path_old, m_path_new));

    // Clean up
    free_marshalled_str(path_old, m_path_old);
    free_marshalled_str(path_new, m_path_new);
    return res;
}

FRESULT f_opendir(DIR* dp, const TCHAR* path)
{
    // Marshal
    FF_CHECK_LO_MEM(dp);
    const TCHAR* m_path = marshal_str(path);

    // Invoke
    FF_CALL(imp_f_opendir(dp, m_path));

    // Clean up
    free_marshalled_str(path, m_path);
    return res;
}

FRESULT f_closedir(DIR *dp)
{
    FF_CHECK_LO_MEM(dp);
    FF_CALL(imp_f_closedir(dp));
    return res;
}

FRESULT f_readdir(DIR* dp, FILINFO* fno)
{
    FF_CHECK_LO_MEM(dp);
    FF_CHECK_LO_MEM(fno);
    FF_CALL(imp_f_readdir(dp, fno));
    return res;
}

FRESULT f_mkdir(const TCHAR* path)
{
    FF_CALL(imp_f_mkdir(path));
    return res;
}

FRESULT f_truncate(FIL* fp)
{
    FF_CHECK_LO_MEM(fp);
    FF_CALL(imp_f_truncate(fp));
    return res;
}

int pattern_matching(const TCHAR* pat, const TCHAR* nam, int skip, int inf)
{
    // For performance reasons, these must already be in lo mem
    if (IS_HI_MEM(pat))
        return 0;
    if (IS_HI_MEM(nam))
        return 0;

    // Invoke
    FF_CALL(imp_pattern_matching(pat, nam, skip, inf));
    return res;
}

void* malloc(size_t size)
{
    ENTER_BANK(BANK_FATFS);
    void* retv = imp_malloc(size);
    LEAVE_BANK();
    return retv;
}

void free(void* ptr)
{
    ENTER_BANK(BANK_FATFS);
    imp_free(ptr);
    LEAVE_BANK();
}

void* realloc(void* ptr, size_t size)
{
    ENTER_BANK(BANK_FATFS);
    void* retv = imp_realloc(ptr, size);
    LEAVE_BANK();
    return retv;
}
