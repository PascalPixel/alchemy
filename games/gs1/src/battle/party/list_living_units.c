#include "battle_party.h"
#include "battle_runtime.h"
#include "types.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 BattleParty_ListLivingUnits(s32 side_mask, u16 *unit_ids) {
    u16 active_members[8];
    u16 *active_members_base;
    s32 remaining;
    s32 enemy_limit;
    u16 *output;
    s32 living_count;
    s32 enemy_capacity;
    u16 *member;
    s32 unit_id;
    s16 hp;
    s32 active_count;
    void *unit;

    output = unit_ids;
    living_count = 0;
    enemy_capacity = 6;
    if (BattleFlag_Test(0x16C) != 0) {
        enemy_capacity = 3;
    }
    if (side_mask & BATTLE_SIDE_PARTY) {
        active_members_base = active_members;
        active_count = BattleParty_ListActiveMembers(active_members_base);
        if (living_count < active_count) {
            member = active_members_base;
            remaining = active_count;
            do {
                unit_id = *member;
                member += 1;
                hp = FIELD_AT_OFFSET(BattleUnit_Get(unit_id), s16, 0x38);
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
                unit = BattleUnit_Get(remaining);
                if ((FIELD_AT_OFFSET(unit, u8, 0x12A) != 0) && ((s32) FIELD_AT_OFFSET(unit, s16, 0x38) > 0)) {
                    if (output != NULL) {
                        *output = (u16) remaining;
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
