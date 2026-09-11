#include "types.h"
#include "scene.h"

/* graphics/prepare_transfer_in_iwram_work.c */
void Graphics_PrepareTransferInIwramWork(s32 src, s32 dst)
{
    Sys_Place(src, dst, 0x03000350);
}

/* graphics/prepare_transfer_and_run.c */
typedef void (*WorkFunc)(void *);

void Graphics_PrepareTransferAndRun(void *src, void *dst)
{
    u8 work[48];

    Sys_Place(src, dst, work);
    ((WorkFunc)0x030002c0)(work);
}
