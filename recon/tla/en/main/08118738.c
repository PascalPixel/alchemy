/* differ 12: five loads, the spilled done flag, the table base, the owner byte
   and the set counter, issue one slot earlier in the native (arm9tdmi load
   latency). */
#include "types.h"
#include "owner_state.h"

struct DebugPartyEntry {
    s8 owner;
    u8 level;
    s8 djinn_counts[4];
    u16 items[6];
    u16 actions[2];
    u16 unknown_16;
};

extern struct DebugPartyEntry Data_0812a174[];

void Party_RemoveActiveOwnerFar(s32 owner);
struct TradeOfferState *Trade_GetOfferStateFar(s32 owner);
s32 Party_AddActiveOwnerFar(s32 owner);
void Party_AdvanceOwnerCountToTargetFar(s32 owner, s32 level);
void OwnerAction_AddFar(s32 owner, s32 action);
s32 Djinn_AddToOwnerFar(s32 owner, s32 element, s32 index);
s32 Djinn_ActivateFar(s32 owner, s32 element, s32 index);
s32 Inventory_RemoveFar(s32 owner, s32 slot);
s32 Inventory_AddItemFar(s32 owner, s32 item);
s32 Inventory_EquipFar(s32 owner, s32 slot);
void Owner_RefreshDerivedDataFar(s32 owner);

s32 DebugParty_LoadPreset(s32 preset)
{
    s32 next_djinn[4];
    void *state;
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
    Party_RemoveActiveOwnerFar(4);
    Party_RemoveActiveOwnerFar(6);
    Party_RemoveActiveOwnerFar(7);
    Trade_GetOfferStateFar(0)->count = done;

    set = 0;
    for (i = 0; i < 4; i++)
        next_djinn[i] = 0;

    for (row = 0;; row++) {
        owner = Data_0812a174[row].owner;
        if ((u32)row > 440) {
            done = 1;
            break;
        }
        if (owner == -1) {
            if (set <= 69) {
                if (set == preset)
                    break;
            } else if (set == (preset | 1)) {
                break;
            }
            set++;
            continue;
        }
        if (set < preset)
            continue;

        Party_AddActiveOwnerFar(owner);
        Party_AdvanceOwnerCountToTargetFar(owner, Data_0812a174[row].level);
        state = UnitGet(owner);
        for (i = 0; i < 4; i++) {
            ((struct OwnerDjinnState *)state)->counts.banks.owned[i] = 0;
            ((struct OwnerDjinnState *)state)->counts.banks.active[i] = 0;
            ((struct OwnerDjinnState *)state)->flags.banks.owned[i] = 0;
            ((struct OwnerDjinnState *)state)->flags.banks.active[i] = 0;
        }
        for (i = 0; i < 32; i++)
            /* FAKEMATCH: the halfword store through a u16 cast loads its zero from the literal pool, as the reference does. */
            *(u16 *)&((struct OwnerActionState *)state)->action_slots[i] = 0;
        for (i = 0; i < 2; i++) {
            if (Data_0812a174[row].actions[i] != 0)
                OwnerAction_AddFar(owner, Data_0812a174[row].actions[i]);
        }
        for (i = 0; i < 4; i++) {
            for (j = 0; j < Data_0812a174[row].djinn_counts[i]; j++) {
                Djinn_AddToOwnerFar(owner, i, (next_djinn[i] + 7) % 18);
                Djinn_ActivateFar(owner, i, (next_djinn[i] + 7) % 18);
                next_djinn[i]++;
            }
        }
        for (i = 0; i < 16; i++)
            Inventory_RemoveFar(owner, 0);
        for (i = 0; i < 6; i++) {
            if (Data_0812a174[row].items[i] != 0)
                Inventory_EquipFar(owner, Inventory_AddItemFar(owner, Data_0812a174[row].items[i]));
        }
        Owner_RefreshDerivedDataFar(owner);
    }
    return done;
}
