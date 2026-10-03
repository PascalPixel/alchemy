#include "BATTLE_RUNTIME.H"
#include "BATTLE_TYPES.H"
#include "BATTLE_WORK.H"
#include "PARTY_STATE.H"
#include "BATTLE_PARTY.H"
#include "TYPES.H"
#include "BATTLE_COMMAND.H"

s32 Party_CountActiveOwnersFar(void);

/* Caps the active party at four owners, or three in the alternate battle mode,
 * optionally writes their identifiers with a 0xff terminator, marks each
 * selected battle unit with status 2, and returns the selected count. */
s32 BattleParty_PrepareActiveOwners(u16 *owners)
{
    s32 limit;
    s32 count;
    s32 index;

    limit = 4;
    if (gBattleWork->two_sided != 0)
        limit = 3;

    count = Party_CountActiveOwnersFar();
    if (count > limit)
        count = limit;

    for (index = 0; index < count; index++) {
        s32 owner = gGameState.active_owners[index];

        if (owners != 0)
            *owners++ = owner;
        Owner_GetStateFar(owner)->status_12a = BATTLE_UNIT_PARTY;
    }

    if (owners != 0)
        *owners = BATTLE_UNIT_LIST_END;
    return count;
}

s32 BattleParty_ListPresentEnemies(s16 *unit_ids)
{
    s16 *output;
    s32 battle_result;
    s32 id;
    s32 entry_limit;
    s32 entry_count;

    output = unit_ids;
    entry_count = 0;
    entry_limit = 6;
    battle_result = 0;
    if (output != NULL) {
        if (GameFlag_TestFar(0x16C) != 0) {
            entry_limit = 3;
        }
        id = 0x80;
        entry_limit += 0x80;
        for (; id < entry_limit; id += 1) {
            if (Owner_GetStateFar(id)->status_12a != 0) {
                *output = (s16)id;
                entry_count += 1;
                output += 1;
            }
        }
        *output = BATTLE_UNIT_LIST_END;
        battle_result = entry_count;
    }
    return battle_result;
}

s32 BattleParty_ListLivingUnits(s32 side_mask, u16 *unit_ids)
{
    u16 active_members[8];
    u16 *base;
    s32 remaining;
    s32 enemy_limit;
    u16 *output;
    s32 living_count;
    s32 enemy_capacity;
    u16 *member;
    s32 unit_id;
    s16 hp;
    s32 active_count;
    struct BattleUnit *unit;

    output = unit_ids;
    living_count = 0;
    enemy_capacity = 6;
    if (GameFlag_TestFar(0x16C) != 0) {
        enemy_capacity = 3;
    }
    if (side_mask & BATTLE_SIDE_PARTY) {
        base = active_members;
        active_count = BattleParty_PrepareActiveOwners(base);
        if (living_count < active_count) {
            member = base;
            remaining = active_count;
            do {
                unit_id = *member;
                member += 1;
                hp = Owner_GetStateFar(unit_id)->hp;
                if (hp > 0) {
                    if (output != NULL) {
                        *output = unit_id;
                        output += 1;
                    }
                    living_count += 1;
                }
                remaining -= 1;
            } while (remaining != 0);
        }
    }
    if (side_mask & BATTLE_SIDE_ENEMIES) {
        remaining = 0x80;
        enemy_limit = enemy_capacity + 0x80;
        if (remaining < enemy_limit) {
            do {
                unit = Owner_GetStateFar(remaining);
                if ((unit->status_12a != 0) && ((s32)unit->hp > 0)) {
                    if (output != NULL) {
                        *output = (u16)remaining;
                        output += 1;
                    }
                    living_count += 1;
                }
                remaining += 1;
            } while (remaining < enemy_limit);
        }
    }
    if (output != NULL) {
        *output = BATTLE_UNIT_LIST_END;
    }
    return living_count;
}

/* Counts the units in the lists selected by groups, bit 0 for the
 * party and bit 1 for the enemies, skipping removed 254 entries.
 * When dst is given, the unit ids are also written there and terminated
 * with 255. */
s32 BattleParty_ListActorIds(s32 groups, u16 *dst)
{
    struct BattleSession *order;
    s32 count;
    s32 i;

    order = gBattleWork;
    count = 0;
    if (groups & BATTLE_SIDE_PARTY) {
        for (i = 0; order->party_units[i] != BATTLE_UNIT_LIST_END; i++) {
            if (order->party_units[i] != BATTLE_UNIT_REMOVED) {
                if (dst != NULL)
                    *dst++ = order->party_units[i];
                count++;
            }
        }
    }
    if (groups & BATTLE_SIDE_ENEMIES) {
        for (i = 0; order->enemy_units[i] != BATTLE_UNIT_LIST_END; i++) {
            if (order->enemy_units[i] != BATTLE_UNIT_REMOVED) {
                if (dst != NULL)
                    *dst++ = order->enemy_units[i];
                count++;
            }
        }
    }
    if (dst != NULL)
        *dst = BATTLE_UNIT_LIST_END;
    return count;
}
