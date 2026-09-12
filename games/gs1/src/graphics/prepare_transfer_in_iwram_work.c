#include "types.h"
#include "scene.h"
#include "global_cells.h"

/* graphics/prepare_transfer_in_iwram_work.c */
/* graphics/prepare_transfer_in_iwram_work.c */
void Graphics_PrepareTransferInIwramWork(s32 src, s32 dst)
{
    Sys_Place(src, dst, 0x03000350);
}
