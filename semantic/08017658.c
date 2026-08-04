#include "types.h"

typedef u8 PlacementContext_08017658[8];

s32 Func_08018038(s32 object, s32 mode);
void Func_0801868c(
    s32 object,
    s32 *x,
    s32 *y,
    s32 *width,
    s32 *height,
    PlacementContext_08017658 *context,
    s32 mode);
s32 Func_080162d4(
    u16 x,
    u16 y,
    u16 width,
    u16 height,
    s32 flags);
void *Func_080165d8(
    s32 created,
    s32 object,
    s32 arg2,
    s32 arg3,
    PlacementContext_08017658 *context,
    s32 arg5);
void Func_08016418(s32 created, s32 mode);

s32 Func_08017658(s32 object, s32 x, s32 y, s32 options)
{
    u8 *state = *(u8 **)0x03001e8c;
    s32 object_index;
    s32 width;
    s32 height;
    s32 flags = 0;
    s32 created;
    PlacementContext_08017658 context;

    *(u16 *)(state + 0x12f4) = ((u32)options << 4) >> 20;
    *(u16 *)(state + 0x12f6) = 0;
    options &= 0xffff;
    object_index = Func_08018038(object, 1);

    if (*(u16 *)(state + 0xeb0 + object_index * 2) == 0)
        return 0;

    Func_0801868c(
        object_index, &x, &y, &width, &height, &context, 0);
    if (width == 0 && height == 0)
        return 0;

    if (!(options & 1))
        flags |= 2;
    if (options & 8)
        flags |= 8;
    if (options & 0x10)
        flags |= 0x80;
    if (options & 0x20)
        flags |= 0x100;

    created = Func_080162d4(
        (u16)x, (u16)y, (u16)width, (u16)height, flags);
    if (created == 0)
        return 0;

    if (Func_080165d8(
            created, object_index, 0, 0, &context, 0) == 0) {
        Func_08016418(created, 1);
        return 0;
    }

    state[0x12fa] = 0;
    state[0x12fb] = 0;
    return created;
}
