#include "types.h"
#include "iwram_call.h"
#include "dma.h"
#include "mem.h"
#include "io_reg.h"

s32 Render_ProjectPoint(s32 *point, s32 *screen);

extern const u8 Data_0813b9a0[];
extern u8 Data_0813b9a0CodeSize[];

void Func_08196958(s32 arg0, s32 arg1, s32 arg2)
{
    void (*routine)(s32, s32, s32);
    u32 size;

    /* FAKEMATCH: the wrapper keeps the size load after the parameter copies, as in UiText_LookupMessage. */
    do {
        size = (u32)Data_0813b9a0CodeSize;
    } while (0);
    routine = (void (*)(s32, s32, s32))MemAlloc(size);
    Dma_Set((const void *)Data_0813b9a0, routine, 0x84000000 | (size >> 2), REG_DMA3);
    routine(arg0, arg1, arg2);
    MemFree(routine);
}

void Func_081969ac(s8 *vertices, s16 *screen, s32 count)
{
    s32 point[3];

    for (; count != 0; count--) {
        point[0] = vertices[0] << 16;
        point[1] = vertices[1] << 16;
        point[2] = vertices[2] << 16;
        Render_ProjectPoint(point, point);
        screen[0] = point[0];
        screen[1] = point[1];
        screen[2] = point[2];
        vertices += 4;
        screen += 4;
    }
}

struct PoolEntry {
    s32 head;
    s32 next;
    u8 unk08[16];
    s32 tail;
};

void *Runtime_BumpAllocateAlternatePool(s32 size);

struct PoolEntry *Func_081969f8(s32 count)
{
    struct PoolEntry *entries = Runtime_BumpAllocateAlternatePool((count + 1) * sizeof(struct PoolEntry));
    s32 i;

    for (i = 0; i < count + 1; i++) {
        entries[i].head = 0;
        entries[i].next = 0;
        entries[i].tail = 0;
    }
    return entries;
}

struct Texture {
    u8 logWidth;
    u8 logHeight;
    u16 unk02;
    u32 pixels;
};

u32 Runtime_GetRemainingIwram(void);
void *MemAlloc(s32 size);

s32 Func_08196a28(struct Texture *source, struct Texture *copy)
{
    s32 size = (1 << source->logWidth) << source->logHeight;
    u32 pixels;
    s32 misalign;
    s32 mask;

    if (Runtime_GetRemainingIwram() < size + 8)
        return 0;
    *(u16 *)copy = *(u16 *)source;
    copy->pixels = (u32)MemAlloc(size + 8);
    asm volatile(""); /* FAKEMATCH: ⚓️ stores the allocation before it starts the mask; without the barrier the forced mask below is scheduled before the store */
    mask = 4;
    asm volatile("" : "+l"(mask)); /* FAKEMATCH: ⚓️ starts the -4 mask before it loads the pixels; as a plain constant the mask starts after the misalignment is added (differ 2) */
    pixels = source->pixels;
    misalign = pixels & 3;
    Iwram_CopyWords((void *)copy->pixels, (void *)(pixels - misalign), (size + misalign + 3) & -mask);
    copy->pixels += misalign;
    return 1;
}
