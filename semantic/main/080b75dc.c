#include "layout_guard.h"
#include "types.h"

typedef struct BattleState_080b75dc {
    u8 padding_000[0x66];
    s16 party_ids_066[6];
    u8 padding_072[0x26a];
    u8 owner_slots_2dc[14];
} BattleState_080b75dc;

LAYOUT_OFFSET_GUARD(
    BattleState080b75dc_PartyIds,
    BattleState_080b75dc,
    party_ids_066,
    0x66);
LAYOUT_OFFSET_GUARD(
    BattleState080b75dc_OwnerSlots,
    BattleState_080b75dc,
    owner_slots_2dc,
    0x2dc);
extern BattleState_080b75dc *Data_03001e74;

s32 Func_080b6a60(u16 *);
void Func_080b6f44(void *, s32, s32, s32);
void Func_080b7424(u16 *, s32, s32 *, s32 *);
void *Func_080b7dd0(s32);

/*
 * Rebuild the battle object's owner-slot map and position all active and
 * party-list markers.  Party placeholder 254 participates in spacing but has
 * no object to initialize.
 */
void Func_080b75dc(void)
{
    const s8 *positions = (const s8 *)0x080c2a62;
    BattleState_080b75dc *state = Data_03001e74;
    u16 ids[6];
    s32 xs[6];
    s32 ys[6];
    s32 active_count = Func_080b6a60(ids);
    s32 index;
    s32 party_count;

    for (index = 13; index >= 0; index--)
        state->owner_slots_2dc[index] = 0xff;

    for (index = 0; index < 6; index++)
        state->owner_slots_2dc[13 - index] = 13 - index;

    for (index = 0; index < active_count; index++) {
        u16 owner = ids[index];

        state->owner_slots_2dc[owner] = index;
        Func_080b6f44(
            Func_080b7dd0(owner),
            owner,
            positions[index * 2],
            positions[index * 2 + 1]);
    }

    party_count = 0;
    while (party_count < 6 &&
           state->party_ids_066[party_count] != 255) {
        ids[party_count] = state->party_ids_066[party_count];
        party_count++;
    }

    Func_080b7424(ids, party_count, xs, ys);

    for (index = 0; index < party_count; index++) {
        s32 owner = state->party_ids_066[index];

        if (owner != 254) {
            Func_080b6f44(
                Func_080b7dd0(owner),
                owner,
                xs[index],
                ys[index]);
        }
    }
}
