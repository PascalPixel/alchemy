#include "types.h"

#define BattlePresentation_RunUnitAction Func_080b8c1c
#define BattleObject_IsValidId Func_080b8808
#define BattleParty_ListLivingUnits Func_080b6b40
#define GetBattleObjectSlot Func_080b7dd0
#define Actor_ResetMotionAtAnchor Func_080b8000

struct BattlePresentationTransition {
    s32 angle;
    s32 timer;
};

struct BattlePresentationWork {
    s32 stage;
    s32 is_party_target;
    s32 primary_unit;
    s32 secondary_unit;
    s32 action_value;
    s32 unit_count;
    s32 reserved_18;
    u8 reserved_1c[8];
    s16 units[8];
    u8 child_values[32];
};

struct MotionChild {
    u8 reserved_00[5];
    u8 value;
};

struct MotionEntry {
    u8 reserved_00[39];
    u8 child_count;
    struct MotionChild *children[1];
};

struct BattleMotionActor {
    u8 reserved_00[80];
    struct MotionEntry *motion;
};

struct BattleMotionSlot {
    struct BattleMotionActor *actor;
};

extern struct BattlePresentationTransition *Data_03001f00;
extern u8 *Data_03001e74;

void WaitFrames(s32 frames);
s32 Func_080b8808(s32 object_id);
s32 Func_080b6b40(s32 side_mask, s16 *unit_ids);
void Func_08015130(s32 mode);
struct BattleMotionSlot *Func_080b7dd0(s32 unit_id);
void Func_08009080(void *actor, s32 mode);
void Func_08009088(void *actor, s32 mode);
void Func_080c9020(struct BattlePresentationWork *work);
void Func_080c9018(struct BattlePresentationWork *work);
void Func_080b8000(s32 unit_id);

s32 BattlePresentation_RunUnitAction(s16 *action)
{
    s16 *saved_action = action;
    struct BattlePresentationTransition *transition;
    struct BattlePresentationWork work;
    struct BattleMotionActor *primary_actor;
    s32 facing;
    s32 side_mask;
    s32 final_count;
    s16 *unit_list;
    s32 i;

    transition = Data_03001f00;
    facing = -0x2000;
    if (saved_action[0] <= 4)
        facing = 0x2000;
    if (transition->angle == facing) {
        transition->timer = 40;
        WaitFrames(40);
    } else {
        transition->angle = facing;
        transition->timer = 40;
        WaitFrames(40);
    }

    work.stage = saved_action[4];
    work.action_value = saved_action[6];
    work.primary_unit = saved_action[0];
    work.secondary_unit = saved_action[5];
    if (BattleObject_IsValidId(work.primary_unit) < 0)
        return -1;

    if (work.secondary_unit > 127) {
        unit_list = work.units;
        side_mask = 2;
    } else {
        unit_list = work.units;
        side_mask = 1;
    }
    work.unit_count = BattleParty_ListLivingUnits(side_mask, unit_list);
    Func_08015130(Data_03001e74[65] & ~1);

    primary_actor = GetBattleObjectSlot(work.primary_unit)->actor;
    Func_08009080(primary_actor, 3);
    Func_08009088(primary_actor, 16);

    if ((u16)saved_action[5] <= 7) {
        final_count = 1;
        work.is_party_target = final_count;
        BattleParty_ListLivingUnits(1, unit_list);
    } else {
        work.is_party_target = 0;
        BattleParty_ListLivingUnits(2, unit_list);
        final_count = 1;
    }
    work.unit_count = final_count;

    i = 0;
    if (work.unit_count != 0) {
        s32 value_offset = 0;
        do {
            struct BattleMotionActor *actor;
            struct MotionEntry *motion;
            s32 j;

            actor = GetBattleObjectSlot(work.units[i])->actor;
            motion = actor->motion;
            j = 0;
            if (motion->child_count - 1 != 0) {
                do {
                    work.child_values[value_offset + j] = motion->children[j]->value;
                    j++;
                } while (j != motion->child_count - 1);
            }
            i++;
            value_offset += 4;
        } while (i != work.unit_count);
    }

    work.stage = 0;
    work.reserved_18 = 0;
    Func_080c9020(&work);
    work.stage = 1;
    Func_080c9020(&work);
    work.stage = 2;
    Func_080c9020(&work);
    work.stage = 3;
    Func_080c9020(&work);
    work.stage = 0;
    Func_080c9018(&work);

    Func_08009080(primary_actor, 1);
    i = 0;
    if (work.unit_count != 0) {
        do {
            Actor_ResetMotionAtAnchor(work.units[i]);
            i++;
        } while (i != work.unit_count);
    }
    Actor_ResetMotionAtAnchor(work.primary_unit);
    return 0;
}
