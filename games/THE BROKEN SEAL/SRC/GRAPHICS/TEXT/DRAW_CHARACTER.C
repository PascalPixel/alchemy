#include "TYPES.H"
#include "TBS_EDITION.H"
#undef UiText_DrawCharacter
#define UiText_DrawCharacter Func_08021d88


void UiText_LoadRemappedGlyph(u8 *base, u32 value, s32 slot_a, s32 slot_b);
s32 Resource_LoadIndexedIntoBuffer(u16 packed, u32 value);

typedef struct
{
    u16 code : 10;
    u16 style : 6;
} CharacterCell;

void UiText_DrawCharacter(u8 *base, s32 index, u32 value)
{
    u8 *entry;
    s32 offset;
    s32 store_offset;
    s32 load_offset;

    offset = index * 28;
    entry = base + offset + 0x104;
    UiText_LoadRemappedGlyph(base, value, index * 16, index * 16);
    store_offset = offset + 0x11C;
    *(u32 *)(base + store_offset) = value;
    /* FAKEMATCH: the empty do-while around this store only moves the
       scheduler; the reference sets the constant after the value store. */
    do { *(u32 *)(entry + 4) = 0x80002000; } while (0);
    *(u32 *)(entry + 8) = 0;
    load_offset = offset + 0x110;
    ((CharacterCell *)(entry + 8))->code =
        Resource_LoadIndexedIntoBuffer(*(u16 *)(base + load_offset), value);
}
