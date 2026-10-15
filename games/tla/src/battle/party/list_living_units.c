#include "battle_party.h"
#include "game_flags.h"
#include "owner_state.h"
#include "types.h"
#include "battle_unit.h"

s32 BtlEnmList(s16 *unit_ids)
{
    s16 *output;
    s32 count;
    s32 id;
    s32 limit;
    s32 listed;

    output = unit_ids;
    listed = 0;
    limit = 6;
    count = 0;
    if (output != NULL) {
        if (GameFlag_Test(0x16c) != 0)
            limit = 3;
        id = 0x80;
        limit += 0x80;
        for (; id < limit; id++) {
            if (((struct BtlUnit *)UnitGet(id))->side != 0) {
                *output = id;
                listed++;
                output++;
            }
        }
        count = listed;
        *output = 0xff;
    }
    return count;
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
    struct BtlUnit *unit;

    output = unit_ids;
    living_count = 0;
    enemy_capacity = 6;
    if (GameFlag_Test(0x16C) != 0) {
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
                hp = ((struct BtlUnit *)UnitGet(unit_id))->hp;
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
                unit = UnitGet(remaining);
                if (unit->side != 0 && unit->hp > 0) {
                    if (output != NULL) {
                        *output = remaining;
                        output += 1;
                    }
                    living_count += 1;
                }
                remaining += 1;
            } while (remaining < enemy_limit);
        }
    }
    if (output != NULL) {
        *output = 0xFF;
    }
    return living_count;
}

s32 BattleParty_PrepareReserveOwners(u16 *owners);

// As BattleParty_ListLivingUnits, with the reserve owners listed after the
// active ones.
s32 Func_0811a24c(s32 side_mask, u16 *unit_ids)
{
    u16 members[10];
    u16 *base;
    s32 remaining;
    s32 enemy_limit;
    u16 *output;
    s32 living_count;
    s32 enemy_capacity;
    u16 *member;
    s32 unit_id;
    s16 hp;
    s32 count;
    struct BtlUnit *unit;

    output = unit_ids;
    living_count = 0;
    enemy_capacity = 6;
    if (GameFlag_Test(0x16C) != 0) {
        enemy_capacity = 3;
    }
    if (side_mask & BATTLE_SIDE_PARTY) {
        base = members;
        count = BattleParty_PrepareActiveOwners(base);
        count += BattleParty_PrepareReserveOwners(base + count);
        if (living_count < count) {
            member = base;
            remaining = count;
            do {
                unit_id = *member;
                member += 1;
                hp = ((struct BtlUnit *)UnitGet(unit_id))->hp;
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
                unit = UnitGet(remaining);
                if (unit->side != 0 && unit->hp > 0) {
                    if (output != NULL) {
                        *output = remaining;
                        output += 1;
                    }
                    living_count += 1;
                }
                remaining += 1;
            } while (remaining < enemy_limit);
        }
    }
    if (output != NULL) {
        *output = 0xFF;
    }
    return living_count;
}
