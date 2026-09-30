#include "DMA.H"
#include "TBS_EDITION.H"

extern u8 gWindowWork[];
void UiWork_UploadDirtyBlocks(void)
{
    u8 *work = *(u8 **)gWindowWork;
    u32 flags;
    u8 *src;
    u8 *dst;
    if (!work[RENDER_MENU_BUSY_OFS]) {
        flags = work[RENDER_DIRTY_OFS];
        if (flags) {
            dst = (u8 *)0x06002000;
            src = work;
            if (flags & 1) flags = 63;
            flags &= 63;
            flags >>= 1;
            do {
                if (flags & 1) Dma_Set(src, dst, 0x84000040, (volatile u32 *)0x040000d4);
                flags >>= 1;
                src += 256;
                dst += 256;
            } while (flags);
            work[RENDER_DIRTY_OFS] = flags;
        }
    }
}
