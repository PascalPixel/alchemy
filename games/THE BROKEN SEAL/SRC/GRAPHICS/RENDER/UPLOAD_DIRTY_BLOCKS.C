#include "DMA.H"
void UiWork_UploadDirtyBlocks(void)
{
    u8 *work = *(u8 **)0x03001e8c;
    u32 flags;
    u8 *src;
    u8 *dst;
    if (!work[0xea6]) {
        flags = work[0xea3];
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
            work[0xea3] = flags;
        }
    }
}
