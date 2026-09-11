#include "types.h"
#include "scene.h"
#include "battle_party.h"
#include "battle_runtime.h"
#include "global_cells.h"

/* battle/pres_wait_sync.c */
/* battle/presentation/misc/wait_sync.c */
struct LinkWork {
    u8 pad0[0x44];
    u8 enabled;
    u8 pad1[0x0b];
    u8 side;
    u8 pad2;
    u8 paused;
};

#define LINK_WORK (*(struct LinkWork **)0x03001e74)
#define LINK_REC 0x02002024
#define LINK_LOCAL ((u16 *)0x02002224)
#define LINK_STAT (*(u16 *)0x03001f64)

s32 BattlePres_WaitSync(void)
{
    struct LinkWork *work = LINK_WORK;
    u16 *peer;
    u16 *sync;
    s32 miss = 0;
    s32 i;

    if (work->enabled != 0) {
        u32 side = work->side;
        u32 other = 1;

        other ^= side;
        side = other << 1;
        side += other;
        side <<= 3;
        peer = (u16 *)(LINK_REC + side);
        sync = LINK_LOCAL;
        if (work->paused == 0) {
            sync[0] = 'E';
            sync[1] = 'X';
            sync[2] = 'E';
            sync[3] = 'C';

            for (i = 0; i <= 29; i++) {
                if ((LINK_STAT & 3) != 3) {
                    miss++;
                    if (miss > 24) {
                        return -1;
                    }
                } else {
                    miss = 0;
                    if (sync[2] == peer[2] && sync[3] == peer[3]) {
                        return 0;
                    }
                }
                WaitFrames(1);
            }
        }
        return -1;
    }
    return 0;
}

/* battle/party/assign_member_slots.c */
extern u8 *gBattleWork;

s32 BattleParty_AssignMemberSlots(void)
{
    u16 active_members[8];
    u8 *battle_state = gBattleWork;
    s32 party_size = BattleParty_ListActiveMembers(active_members);
    s32 member_slot;
    s32 unit_id;

    for (member_slot = 0; member_slot < party_size; member_slot++) {
        unit_id = active_members[member_slot];
        unit_id += 72;
        battle_state[unit_id] = (s8)(member_slot - 128);
    }
}

/* battle/runtime/apply_value_to_work_2224.c */
/*
 * Apply a value to the battle work record at 0x02002224.
 */

/*
 * Battle_SetMode names a `bx rN` slot: the call is indirect through the
 * register that slot selects, and the trailing argument is the callee
 * address at 0x03000164. That routine is reached with two arguments at
 * some sites and three at others, so its shape is not established.
 */

/*
 * The third argument reads val before val is written, so it carries
 * whatever the register already holds; it must not be respelled as a fresh
 * load. The two assignments that follow the call keep that order.
 */
char Battle_ApplyValueToWork2224(s16 arg2)
{
  s16 val;
  s16 val2;
  Battle_SetMode(0x02002224, 0x10, val, 0x03000164);
  val2 = arg2;
  val = val2;
}

/* battle/party/list_present_enemies.c */
/* battle/party/list_present_enemies.c */
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
        if (BattleFlag_Test(0x16C) != 0) {
            entry_limit = 3;
        }
        id = 0x80;
        entry_limit += 0x80;
        for (; id < entry_limit; id += 1) {
            if (FIELD_AT_OFFSET(BattleUnit_Get(id), u8 *, 0x12A) != 0) {
                *output = (s16)id;
                entry_count += 1;
                output += 1;
            }
        }
        *output = 0xFF;
        battle_result = entry_count;
    }
    return battle_result;
}

/* battle/party/list_living_units.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

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
    void *unit;

    output = unit_ids;
    living_count = 0;
    enemy_capacity = 6;
    if (BattleFlag_Test(0x16C) != 0) {
        enemy_capacity = 3;
    }
    if (side_mask & BATTLE_SIDE_PARTY) {
        base = active_members;
        active_count = BattleParty_ListActiveMembers(base);
        if (living_count < active_count) {
            member = base;
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
                if ((FIELD_AT_OFFSET(unit, u8, 0x12A) != 0) && ((s32)FIELD_AT_OFFSET(unit, s16, 0x38) > 0)) {
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
        *output = 0xFF;
    }
    return living_count;
}

/* battle/actor_commit_placement.c */
/* battle/actor/commit_placement.c */
void BattleActor_CommitPlacement(void)
{
    u8 actor_slots[28];

    Actor_Apply(3, actor_slots);
    Actor_Apply2(actor_slots, 1);
}

/* battle/party/list_all_units_and_submit.c */
void BattleParty_ListAllUnitsAndSubmit(void)
{
    u8 local[28];
    Battle_Apply(3, local);
    Battle_Apply2(local, 0);
}

/* battle/motion/get_slot_field_14.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void *GetBattleObjectSlot();

s32 BattleMotion_GetSlotField14(void)
{
    return FIELD_AT_OFFSET(GetBattleObjectSlot(), s32 *, 0x14);
}

/* battle/summon/class_is_valid.c */
struct Layout {
    u8 pad[4];
    s16 field[6];
};

s32 Summon_ClassValid(s32 arg0)
{
    struct Layout *ptr;
    s32 retval;
    s32 i;

    retval = Battle_Check(arg0);
    ptr = *(struct Layout **)ADDR_03001E74;
    for (i = 0; i <= 5; i++) {
        if (ptr->field[i] != 0)
            continue;
        if (retval != 0)
            break;
        if (i <= 4 && ptr->field[i + 1] == 0)
            break;
    }
    return i != 6;
}
