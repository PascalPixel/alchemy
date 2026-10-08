#include "types.h"

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
