#include "battle_party.h"
#include "game_flags.h"
#include "owner_state.h"
#include "types.h"
#include "battle_unit.h"

s32 BattleEnemyList(s16 *unit_ids)
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
            if (((struct BattleUnit *)UnitGet(id))->side != 0) {
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

s32 BattleParty_ListLivingUnits(s32 sideMask, u16 *unitIds)
{
    u16 activeMembers[8];
    u16 *base;
    s32 remaining;
    s32 enemyLimit;
    u16 *output;
    s32 livingCount;
    s32 enemyCapacity;
    u16 *member;
    s32 unitId;
    s16 hp;
    s32 activeCount;
    struct BattleUnit *unit;

    output = unitIds;
    livingCount = 0;
    enemyCapacity = 6;
    if (GameFlag_Test(0x16C) != 0) {
        enemyCapacity = 3;
    }
    if (sideMask & BATTLE_SIDE_PARTY) {
        base = activeMembers;
        activeCount = BattleParty_PrepareActiveOwners(base);
        if (livingCount < activeCount) {
            member = base;
            remaining = activeCount;
            do {
                unitId = *member;
                member += 1;
                hp = ((struct BattleUnit *)UnitGet(unitId))->hp;
                if (hp > 0) {
                    if (output != NULL) {
                        *output = unitId;
                        output += 1;
                    }
                    livingCount += 1;
                }
                remaining -= 1;
            } while (remaining != 0);
        }
    }
    if (sideMask & BATTLE_SIDE_ENEMIES) {
        remaining = 0x80;
        enemyLimit = enemyCapacity + 0x80;
        if (remaining < enemyLimit) {
            do {
                unit = UnitGet(remaining);
                if (unit->side != 0 && unit->hp > 0) {
                    if (output != NULL) {
                        *output = remaining;
                        output += 1;
                    }
                    livingCount += 1;
                }
                remaining += 1;
            } while (remaining < enemyLimit);
        }
    }
    if (output != NULL) {
        *output = 0xFF;
    }
    return livingCount;
}

s32 BattleParty_PrepareReserveOwners(u16 *owners);

// As BattleParty_ListLivingUnits, with the reserve owners listed after the
// active ones.
s32 Func_0811a24c(s32 sideMask, u16 *unitIds)
{
    u16 members[10];
    u16 *base;
    s32 remaining;
    s32 enemyLimit;
    u16 *output;
    s32 livingCount;
    s32 enemyCapacity;
    u16 *member;
    s32 unitId;
    s16 hp;
    s32 count;
    struct BattleUnit *unit;

    output = unitIds;
    livingCount = 0;
    enemyCapacity = 6;
    if (GameFlag_Test(0x16C) != 0) {
        enemyCapacity = 3;
    }
    if (sideMask & BATTLE_SIDE_PARTY) {
        base = members;
        count = BattleParty_PrepareActiveOwners(base);
        count += BattleParty_PrepareReserveOwners(base + count);
        if (livingCount < count) {
            member = base;
            remaining = count;
            do {
                unitId = *member;
                member += 1;
                hp = ((struct BattleUnit *)UnitGet(unitId))->hp;
                if (hp > 0) {
                    if (output != NULL) {
                        *output = unitId;
                        output += 1;
                    }
                    livingCount += 1;
                }
                remaining -= 1;
            } while (remaining != 0);
        }
    }
    if (sideMask & BATTLE_SIDE_ENEMIES) {
        remaining = 0x80;
        enemyLimit = enemyCapacity + 0x80;
        if (remaining < enemyLimit) {
            do {
                unit = UnitGet(remaining);
                if (unit->side != 0 && unit->hp > 0) {
                    if (output != NULL) {
                        *output = remaining;
                        output += 1;
                    }
                    livingCount += 1;
                }
                remaining += 1;
            } while (remaining < enemyLimit);
        }
    }
    if (output != NULL) {
        *output = 0xFF;
    }
    return livingCount;
}
