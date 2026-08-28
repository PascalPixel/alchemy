#include "types.h"
#include "gs1_edition.h"
#undef UiText_DrawCharacter
#define UiText_DrawCharacter Func_08021d88


void Func_08021cb8(u8 *base, u32 value, s32 slot_a, s32 slot_b);
s32 Func_08021c64(u16 packed, u32 value);

void UiText_DrawCharacter(u8 *base, s32 index, u32 value)
{
    u8 *entry;
    s32 offset;
    s32 store_offset;
    s32 load_offset;

    offset = index * 28;
    entry = base + offset + 0x104;
    Func_08021cb8(base, value, index * 16, index * 16);
    store_offset = offset + 0x11C;
    *(u32 *)(base + store_offset) = value;
    *(u32 *)(entry + 4) = 0x80002000;
    *(u32 *)(entry + 8) = 0;
    load_offset = offset + 0x110;
    *(u16 *)(entry + 8) =
        (Func_08021c64(*(u16 *)(base + load_offset), value) & 0x3FF)
        | (*(u16 *)(entry + 8) & 0xFFFFFC80);
}
