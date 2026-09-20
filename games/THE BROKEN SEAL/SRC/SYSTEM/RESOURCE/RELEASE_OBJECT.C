#include "DMA.H"
struct Work { u8 unknown[28]; u8 resource, flags; u8 unknown_1e[10]; void *children[4]; };
void Resource_ResetEntry(s32);
void ResourceMetadata_ClearRecord(void *);
void ResourceObject_Release(struct Work *work)
{
    volatile u32 zero;
    s32 count;
    void **child;
    if (work) {
        if (!(work->flags & 1)) Resource_ResetEntry(work->resource);
        child = work->children;
        count = 3;
        do { ResourceMetadata_ClearRecord(*child++); } while (--count >= 0);
        zero = 0;
        Dma_Set(&zero, work, 0x8500000e, (volatile u32 *)0x040000d4);
    }
}
