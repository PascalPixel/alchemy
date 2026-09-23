/* Draft, not exact (2026-09-24): candidate=88 reference=88 differing_halfwords=38. Constants the reference loads from
   the literal pool are spelled as link-time Value_ symbols, which restores
   the reference size; wraps marked FAKEMATCH only move scheduling. */
#include "TYPES.H"
extern u8 Value_00000100;
extern u8 Value_00000280;

#define BattlePres_BuildTilemap Func_080c00d8

void BattlePres_BuildTilemap(s32 destination)
{
    s32 *cursor;
    s32 entry;
    u32 index;
    /* The ROM calls the IWRAM fill through call-via veneers, not callees
     * taking a fourth argument. Register lifetimes remain unmatched. */
    void (*fill)(s32 destination, s32 size, s32 value) =
        (void (*)(s32, s32, s32))0x03000168;

    fill(destination, 0x100, -1);
    destination += 0x100;
    fill(destination, 0x80, 0x03ff03ff);
    entry = 0x02010200;
    cursor = (s32 *)(destination + 0x80);

    index = 0;
    do {
        index++;
        *cursor++ = entry;
        entry += 0x00020002;
    } while (index <= 239);

    fill((s32)cursor, (s32)&Value_00000280, 0x03ff03ff);
}
