#include "layout_guard.h"
#include "types.h"

struct Combatant_080b6d30 {
    u8 unknown_000[0x128];
    u8 character;
    u8 unavailable;
};

struct PortraitSlots_080b6d30 {
    u8 unknown_00[4];
    s16 owner[6];
};

LAYOUT_OFFSET_GUARD(
    Combatant080b6d30_Character,
    struct Combatant_080b6d30,
    character,
    0x128);
LAYOUT_OFFSET_GUARD(
    Combatant080b6d30_Unavailable,
    struct Combatant_080b6d30,
    unavailable,
    0x129);
LAYOUT_OFFSET_GUARD(
    PortraitSlots080b6d30_Owner,
    struct PortraitSlots_080b6d30,
    owner,
    4);
LAYOUT_SIZE_GUARD(
    PortraitSlots080b6d30_Size,
    struct PortraitSlots_080b6d30,
    16);

extern struct PortraitSlots_080b6d30 *Data_03001e74;

struct Combatant_080b6d30 *Func_08077008(s32 actor);
s32 Func_080c23c0(s32 character);
s32 Func_080c2384(s32 character);
s32 Func_080c23a0(s32 character);
s32 Func_08009270(
    s32 slot,
    void *destination,
    s32 resource,
    s32 attributes);

/*
 * Reserve portrait-work slots and load the selected actor's resources.
 *
 * Most character portraits occupy two consecutive 0x4000-byte slots; table
 * entries marked as single-slot may use any one free slot.  Resources 0x1dc
 * and 0x1e3 have a second consecutive part, loaded into a separately reserved
 * slot.  The return value packs the first slot in bits 12.. and its resource
 * ID in the low bits.
 */
s32 Func_080b6d30(s32 actor)
{
    struct PortraitSlots_080b6d30 *slots = Data_03001e74;
    struct Combatant_080b6d30 *combatant = Func_08077008(actor);
    s32 single_slot = Func_080c23c0(combatant->character);
    s32 resource = Func_080c2384(combatant->character);
    s32 packed = 0;
    s32 part;
    s32 part_count;

    if (combatant->unavailable != 0)
        return 0;

    part_count =
        (resource == 0x1dc || resource == 0x1e3) ? 2 : 1;

    for (part = 0; part < part_count; part++) {
        s32 slot;

        for (slot = 0; slot < 6; slot++) {
            if (slots->owner[slot] != 0)
                continue;
            if (single_slot == 0 &&
                (slot == 5 || slots->owner[slot + 1] != 0)) {
                continue;
            }
            break;
        }

        if (slot == 6)
            break;

        if (Func_08009270(
                slot,
                (u8 *)0x02018000 + slot * 0x4000,
                resource + part,
                Func_080c23a0(combatant->character)) == 0) {
            return 0;
        }

        if (part == 0)
            packed = (slot << 12) | resource;

        slots->owner[slot] = (s16)actor;
        if (single_slot == 0)
            slots->owner[slot + 1] = (s16)actor;
    }

    return packed;
}
