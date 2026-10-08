#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "owner_state.h"

#define Inventory_EquipFar ((void (*)(s32,s32))Inventory_EquipFar)

struct DebugPartyEntry {
    s8 owner;
    u8 level;
    s8 djinn_counts[4];
    u16 items[4];
    u16 actions[2];
    u16 unknown_12;
};

extern struct DebugPartyEntry Debug_PartyPresets[];

s32 DebugParty_LoadPreset(s32 preset)
{
    s32 next_djinn[4];
    struct BtlUnit *state;
    s32 owner;
    s32 done;
    s32 row;
    s32 j;
    s32 set;
    s32 i;

    done = 0;
    Party_RemoveActiveOwnerFar(0);
    Party_RemoveActiveOwnerFar(1);
    Party_RemoveActiveOwnerFar(2);
    Party_RemoveActiveOwnerFar(3);
    Party_RemoveActiveOwnerFar(5);

    set = 0;
    for (i = 0; i < 4; i++)
        next_djinn[i] = 0;

    for (row = 0;; row++) {
        owner = Debug_PartyPresets[row].owner;
        if (row < 0 || row > 332) {
            done = 1;
            break;
        }
        if (owner == -1) {
            if (set == preset)
                break;
            set++;
            continue;
        }
        if (set != preset)
            continue;

        Party_AddActiveOwnerFar(owner);
        Party_AdvanceOwnerCountToTargetFar(owner, Debug_PartyPresets[row].level);
        state = (struct BtlUnit *)UnitGetFar(owner);
        for (i = 0; i < 4; i++) {
            state->djinn_owned_counts[i] = 0;
            state->djinn_active_counts[i] = 0;
            state->djinn_available[i] = 0;
            state->djinn_active[i] = 0;
        }
        for (i = 0; i < 32; i++)
            /* FAKEMATCH: the halfword store through a u16 cast loads its zero from the literal pool, as the reference does. */
            *(u16 *)&((struct BtlUnit *)state)->action_slots[i] = 0;
        for (i = 0; i < 2; i++) {
            if (Debug_PartyPresets[row].actions[i] != 0)
                OwnerAction_AddFar(owner, Debug_PartyPresets[row].actions[i]);
        }
        for (i = 0; i < 4; i++) {
            for (j = 0; j < Debug_PartyPresets[row].djinn_counts[i]; j++) {
                Djinn_AddToOwnerFar(owner, i, next_djinn[i]);
                Djinn_ActivateFar(owner, i, next_djinn[i]);
                next_djinn[i]++;
            }
        }
        for (i = 0; i < 16; i++)
            Inventory_RemoveFar(owner, 0);
        for (i = 0; i < 4; i++) {
            if (Debug_PartyPresets[row].items[i] != 0)
                Inventory_EquipFar(owner, Inventory_AddItemFar(owner, Debug_PartyPresets[row].items[i]));
        }
    }
    return done;
}
