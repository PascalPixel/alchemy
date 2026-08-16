#include "types.h"

typedef struct PackedField {
    unsigned short value : 10;
    unsigned short rest : 6;
} PackedField;

s32 Func_08021c64(s32, s32);
void Func_08021cb8(void *, s32, s32);

void Func_08021d88(u8 *base, s32 index, s32 value)
{
    u8 *record;
    s32 offset;
    s32 storeOffset;

    offset = index * 0x1C;
    record = base + offset;
    index *= 0x10;
    record += 0x104;
    Func_08021cb8(base, value, index);
    storeOffset = offset + 0x11C;
    *(s32 *)(base + storeOffset) = value;
    *(u32 *)(record + 4) = 0x80002000;
    *(s32 *)(record + 8) = 0;
    offset += 0x110;
    ((PackedField *)(record + 8))->value =
        Func_08021c64(*(u16 *)(base + offset), value);
}
