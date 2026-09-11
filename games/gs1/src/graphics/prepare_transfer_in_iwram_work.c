#include "types.h"
#include "scene.h"
#include "abi/graphics/prepare_transfer_in_iwram_work.h"

void Graphics_PrepareTransferInIwramWork(s32 src, s32 dst)
{
    Sys_Place(src, dst, 0x03000350);
}
