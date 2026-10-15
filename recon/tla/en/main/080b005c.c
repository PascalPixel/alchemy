/* Near miss, differ 2 of 17 (one reordered pair), 2026-10-15: The Broken
 * Seal's Owner_GetDefaultElement on this game's preset table (48 presets of
 * 24 bytes at Data_080c6684) and the unit's enemy id. Remaining: the native
 * loads the table's address from the pool (ldr r3) before it doubles the
 * index (lsls r2, r1, #1); this doubles first. The table in a local behind
 * an empty asm or a one-pass block loads it first but addresses the word as
 * [r2, r3], not [r3, r2], whatever the index's form; pinning the local to r3
 * does the same. The draft it replaces did not compile.
 */
#include "types.h"
#include "battle_unit.h"
#include "runtime_interfaces.h"

struct ElementPreset {
    u32 element;
    u8 digits[4];
    u8 unknown_08[16];
};

extern const struct ElementPreset Data_080c6684[];

s32 Owner_GetDefaultElement(struct BtlUnit *state)
{
    u8 value = Owner_GetRecord(state->enemy_id)[42];

    if (value > 47)
        value = 0;
    return Data_080c6684[value].element;
}
