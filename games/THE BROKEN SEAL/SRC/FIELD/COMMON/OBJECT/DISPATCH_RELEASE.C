#include "DMA.H"
#include "OBJECT_DISPATCH.H"
void ResourceObject_Release(void *);
void ObjectDispatch_Release(struct DispatchObject *work)
{
    volatile u32 zero;
    s32 count;
    void **child;
    if (work) {
        switch (work->kind & 15) {
        case 1: ResourceObject_Release(work->target.child); break;
        case 2:
            child = work->target.children;
            count = 3;
            do { void *entry = *child++; if (entry) ResourceObject_Release(entry); } while (--count >= 0);
            break;
        }
        zero = 0;
        Dma_Set(&zero, work, 0x8500001c, (volatile u32 *)0x040000d4);
    }
}
