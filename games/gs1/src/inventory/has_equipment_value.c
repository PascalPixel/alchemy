#include "scene.h"
#include "inventory.h"
#include "types.h"
#include "owner_state.h"
#include "game_flags.h"
#include "party_state.h"
#include "global_progress.h"
#include "battle_summon.h"
#include "runtime_interfaces.h"

/* inventory/has_equipment_value.c */
s32 Equipment_HasValue(s32 owner, s32 value)
{
    u8 *entry = OwnerState_Get(owner);
    s32 mask = 0x3fff;
    s32 index = 0;

    entry += 88;
    do {
        s32 current = *(u16 *)entry;

        current &= mask;
        entry += 4;
        if (current == value) {
            return 1;
        }
        index++;
    } while (index <= 31);
    return 0;
}
