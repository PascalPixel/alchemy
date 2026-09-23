#include "GLOBAL_CELLS.H"
#include "TYPES.H"

s32 Func_08018038(s32 value, s32 mode);
s32 Func_080165d8(s32 no, s32 entry, s32, s32, s32, s32);

/* Clears the message cursor pair, then opens message `no` for the entry
   `argument` selects when that entry's slot is live. */
s32 UiText_OpenEntryMessage(s32 no, s32 argument)
{
    u8 *base = *(u8 **)ADDR_03001E8C;
    s32 entry;
    s32 entry_offset;
    s32 result = 0;
    /* FAKEMATCH: an unused buffer reproduces the reference's 16-byte frame. */
    u8 unused[8];

    *(u16 *)(base + 0x12f4) = 0;
    *(u16 *)(base + 0x12f6) = 0;
    entry = Func_08018038(argument, 1);
    entry_offset = entry * 2;
    entry_offset += 0xeb0;
    if (*(u16 *)(base + entry_offset) == 0)
        return 0;
    if (no == 0)
        return 0;
    result = Func_080165d8(no, entry, 0, 0, 0, 1);
    if (result == 0)
        return 0;
    return result;
}
