#include "types.h"
#include "resource.h"

void VramBlock_Reset93(void)
{
    Resource_ResetEntry(93);
}

extern const u8 Data_0802e89c[];

void VramBlock_Load93(void)
{
    const u8 *data = Data_0802e89c;
    s32 size = 128;

    /* FAKEMATCH: the native builds the size before the block id; the plain call builds the id first (one reordered pair, differ 2). */
    asm volatile("" : "+l"(data), "+l"(size));
    VramBlock_LoadCached(93, size, data);
}
