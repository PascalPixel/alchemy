/* Debug: rebuild the party from preset number `preset` of the debug party
   table, whose presets are runs of rows ended by an owner of -1. Each row
   re-adds one owner at its level, clears its Djinn and Psynergy, gives its
   Djinn in order per element, and replaces its inventory with equipped
   items. Returns 1 when the table ran out before the preset was found. */
#include "TYPES.H"
#include "OWNER_STATE.H"

struct DebugPartyEntry {
    s8 owner;
    u8 level;
    s8 djinn_counts[4];
    u16 items[4];
    u16 actions[2];
    u16 unknown_12;
};

extern struct DebugPartyEntry Data_080c3f34[];

void Party_RemoveActiveOwnerFar(s32 owner);
void Party_AddActiveOwnerFar(s32 owner);
void Func_080771f0(s32 owner, s32 level);
void OwnerAction_AddFar(s32 owner, s32 action);
s32 Djinn_AddToOwnerFar(s32 owner, s32 element, s32 djinn);
void Djinn_ActivateFar(s32 owner, s32 element, s32 djinn);
s32 Inventory_RemoveFar(s32 owner, s32 slot);
s32 Inventory_AddItemFar(s32 owner, s32 item);
void Inventory_EquipFar(s32 owner, s32 slot);

s32 DebugParty_LoadPreset(s32 preset)
{
    s32 next_djinn[4];
    struct OwnerBitState *state;
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
        owner = Data_080c3f34[row].owner;
        if ((u32)row > 332) {
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
        Func_080771f0(owner, Data_080c3f34[row].level);
        state = (struct OwnerBitState *)OwnerState_GetFar(owner);
        for (i = 0; i < 4; i++) {
            state->bit_counts[i] = 0;
            state->bit_counts[i + 4] = 0;
            state->bits[i] = 0;
            state->bits[i + 4] = 0;
        }
        for (i = 0; i < 32; i++)
            /* FAKEMATCH: the halfword store through a u16 cast loads its zero
               from the literal pool, as the reference does. */
            *(u16 *)&((struct OwnerActionState *)state)->action_slots[i] = 0;
        for (i = 0; i < 2; i++) {
            if (Data_080c3f34[row].actions[i] != 0)
                OwnerAction_AddFar(owner, Data_080c3f34[row].actions[i]);
        }
        for (i = 0; i < 4; i++) {
            for (j = 0; j < Data_080c3f34[row].djinn_counts[i]; j++) {
                Djinn_AddToOwnerFar(owner, i, next_djinn[i]);
                Djinn_ActivateFar(owner, i, next_djinn[i]);
                next_djinn[i]++;
            }
        }
        for (i = 0; i < 16; i++)
            Inventory_RemoveFar(owner, 0);
        for (i = 0; i < 4; i++) {
            if (Data_080c3f34[row].items[i] != 0)
                Inventory_EquipFar(owner, Inventory_AddItemFar(owner, Data_080c3f34[row].items[i]));
        }
    }
    return done;
}
