#include "types.h"
#include "scene.h"
#include "abi/graphics/prepare_transfer_and_run.h"

typedef void (*WorkFunc)(void *);

void Graphics_PrepareTransferAndRun(void *src, void *dst)
{
    u8 work[48];

    Sys_Place(src, dst, work);
    ((WorkFunc)0x030002c0)(work);
}
