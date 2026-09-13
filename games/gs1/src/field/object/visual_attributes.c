#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(u32);

void ObjectVisual_CopyAttributes(u32 target_id, u32 source_id)
{
    void *p;
    u8 flags;
    u32 shape;
    u32 dst_attr;
    u32 merged;

    p = Object_GetById(source_id);
    p = *(void **)((u8 *)p + 0x50);
    flags = *(u8 *)((u8 *)p + 0x1C);
    shape = *(u16 *)((u8 *)p + 0x8);

    p = Object_GetById(target_id);
    p = *(void **)((u8 *)p + 0x50);
    dst_attr = *(u16 *)((u8 *)p + 0x8);
    *(u8 *)((u8 *)p + 0x1C) = flags;
    shape <<= 22;
    shape >>= 22;
    merged = 0xfffffc00;
    merged &= dst_attr;
    merged |= shape;
    *(u16 *)((u8 *)p + 0x8) = merged;
}

void ObjectVisual_ReservedNoOp(void)
{
}
