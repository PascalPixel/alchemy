/* DRAFT, rewritten 2026-10-02 in plain C: same instruction count, 51 lines
 * differ. Two things remain, both allocation:
 * 1. where the target is a party member the ROM holds the 1 in r8 across the
 *    list call and copies it down for the count store that both arms share;
 *    here it sits in r5 and each arm stores for itself;
 * 2. the unit index is r7 in the ROM (the list pointer's register, once that
 *    is dead) and r8 here, which takes the frame from 92 bytes to 84 and the
 *    work block from sp+8 to sp+0.
 * Settled: the index is cleared once before the four stage calls and the
 * reset loop continues from it; the facing and the list side are if/else.
 * Tried without effect: a count variable set before or after the call, in
 * its own variable or in i or j; the four stage calls as a loop; five
 * minutes of alchemy permute. Func_080c9020 still needs its name. */
#include "TYPES.H"

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

extern struct BattlePresentationTransition *gTransitionWork;
extern u8 *gBattleWork;

void WaitFrames(s32 frames);
s32 BattleObject_IsValidId(s32 object_id);
s32 BattleParty_ListLivingUnits(s32 side_mask, s16 *unit_ids);
void UiWindow_DrawPartyStatusContentsFar(s32 mode);
struct BattleMotionSlot *GetBattleObjectSlot(s32 unit_id);
void Object_SetMode(void *actor, s32 mode);
void ObjectDispatch_ApplyValueToChildrenFar(void *actor, s32 mode);
void Func_080c9020(struct BattlePresentationWork *work);
void BattleFx_DispatchModeFar(struct BattlePresentationWork *work);
void Actor_ResetMotionAtAnchor(s32 unit_id);

s32 BattlePres_RunUnitAction(s16 *action)
{
    struct BattlePresentationWork work;
    struct BattlePresentationTransition *transition = gTransitionWork;
    struct BattleMotionActor *actor;
    s32 facing;
    s32 i;
    s32 j;

    if (action[0] > 4)
        facing = -0x2000;
    else
        facing = 0x2000;
    if (transition->angle == facing) {
        transition->timer = 40;
        WaitFrames(40);
    } else {
        transition->angle = facing;
        transition->timer = 40;
        WaitFrames(40);
    }
    work.stage = action[4];
    work.action_value = action[6];
    work.primary_unit = action[0];
    work.secondary_unit = action[5];
    if (BattleObject_IsValidId(work.primary_unit) < 0)
        return -1;
    if (work.secondary_unit > 127)
        work.unit_count = BattleParty_ListLivingUnits(2, work.units);
    else
        work.unit_count = BattleParty_ListLivingUnits(1, work.units);
    UiWindow_DrawPartyStatusContentsFar(gBattleWork[65] & ~1);
    actor = GetBattleObjectSlot(work.primary_unit)->actor;
    Object_SetMode(actor, 3);
    ObjectDispatch_ApplyValueToChildrenFar(actor, 16);
    if ((u16)action[5] <= 7) {
        work.is_party_target = 1;
        BattleParty_ListLivingUnits(1, work.units);
        work.unit_count = 1;
    } else {
        work.is_party_target = 0;
        BattleParty_ListLivingUnits(2, work.units);
        work.unit_count = 1;
    }
    for (i = 0; i != work.unit_count; i++) {
        struct MotionEntry *motion = GetBattleObjectSlot(work.units[i])->actor->motion;

        for (j = 0; j != motion->child_count - 1; j++)
            work.child_values[i * 4 + j] = motion->children[j]->value;
    }
    i = 0;
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
    BattleFx_DispatchModeFar(&work);
    Object_SetMode(actor, 1);
    for (; i != work.unit_count; i++)
        Actor_ResetMotionAtAnchor(work.units[i]);
    Actor_ResetMotionAtAnchor(work.primary_unit);
    return 0;
}
