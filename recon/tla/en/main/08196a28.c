#include "types.h"
#include "iwram_call.h"

struct Texture {
    u8 log_width;
    u8 log_height;
    u16 unknown_02;
    u32 pixels;
};

u32 Runtime_GetRemainingIwram(void);
void *MemAlloc(s32 size);

s32 Func_08196a28(struct Texture *source, struct Texture *copy)
{
    s32 size = (1 << source->log_width) << source->log_height;
    u32 pixels;
    s32 misalign;

    if (Runtime_GetRemainingIwram() < size + 8)
        return 0;
    *(u16 *)copy = *(u16 *)source;
    copy->pixels = (u32)MemAlloc(size + 8);
    pixels = source->pixels;
    misalign = pixels & 3;
    Iwram_CopyWords((void *)copy->pixels, (void *)(pixels - misalign), (size + misalign + 3) & ~3);
    copy->pixels += misalign;
    return 1;
}
