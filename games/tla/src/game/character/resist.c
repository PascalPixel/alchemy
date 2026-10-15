#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "owner_state.h"
#include "battle_unit.h"
#include "runtime_interfaces.h"


s32 Owner_GetResistanceValue(s32 owner, s32 index)
{
    struct OwnerInventoryState *state = UnitGet(owner);
    s32 values[4];
    s32 result = 0;

    if (index <= 3) {
        Owner_GetDigitValues(state->class_id, (const u8 *)&((struct OwnerDjinnState *)state)->flags.banks.owned, values);
        result = values[index] / 10;
    }
    return result;
}

// One of the 48 element presets an enemy record names.
struct ElementPreset {
    u32 element;
    u8 digits[4];
    u8 unknown_08[16];
};

extern const struct ElementPreset Data_080c6684[];

// The element of the preset the unit's enemy record names; an id past the
// table's end reads the first preset.
s32 Owner_GetDefaultElement(void *state)
{
    struct BtlUnit *unit = state;
    u8 preset = Owner_GetRecord(unit->enemy_id)[42];
    const struct ElementPreset *table;
    u32 offset;

    if (preset > 47)
        preset = 0;
    table = Data_080c6684;
    /* FAKEMATCH: the barrier loads the table's address before the preset is
       scaled, as the native does; plainly the scaling is started first (one
       reordered pair, differ 2). Indexed as table[preset] the word is then
       addressed as [r2, r3], not [r3, r2] (differ 2), so the offset is a
       local. Measured against it: a one-pass block (differ 2), an
       operand-tied asm on the table (4) or on the offset (14), and both
       pinned to the r3 and r2 they take, without the barrier (2). */
    asm("");
    offset = preset * sizeof(struct ElementPreset);
    return *(u32 *)((u8 *)table + offset);
}
