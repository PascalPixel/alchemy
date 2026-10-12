#include "types.h"
#include "iwram_call.h"

s32 Render_ProjectPoint(s32 *point, s32 *screen);

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
    u8 unknown_08[16];
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
