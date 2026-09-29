/* Draft, whole main:080ba978, 612 bytes including pool.
 * Baseline: 612/612 bytes, 261 differing halfwords, 144 aligned edits.
 * H1: transfer exact RUN_SIMPLE's typed work and unsigned angle input;
 * narrow after the offset, correct the 0x80000 angle, use signed member IDs,
 * and snapshot each motion record's child count before copying its values.
 * The caller passes mode 0/1/2; LIST_TARGETS owns the complete 84-byte output.
 * H1 result: 592/612 bytes, 275 differing halfwords, 114 aligned edits;
 * frame 88 and high-register save set now match. The signed target loop and
 * child-copy body are structurally correct. First divergence is the target
 * angle branch; the same-team ternary also omits reference materialized
 * boolean branches. Input/transition and flags/object roles remain swapped.
 * H2: materialize the same-team predicate as a local on each branch, as the
 * reference tests a 0/1 result rather than branching directly on both IDs.
 * H2 result: 604/612 bytes, 269 differing halfwords, 110 aligned edits.
 * Boolean materialization returned 12 bytes, but the two branch tails still
 * merge. The target-angle conditional is if-converted; row-offset lifetime
 * still keeps r8 instead of the reference r6 and spills the wrong loop role.
 * Stop after the single structural followup: no established closing path.
 * No matching-C credit claimed.
 * 2026-09-29 alchemy permute (seed 1, 3 jobs, 10 minutes): testing the
 * primary side as > 7 with the far target first, kept here, gives 1690
 * against 1930 (after names first); the search's 1660 also assigned the
 * script test to an unused local, which is not kept. The first divergence
 * is still the target angle branch.
 */
#include "TYPES.H"
#include "MOTION_OBJECT.H"
#include "BATTLE_PRESENTATION.H"
#include "FIXED_MATH.H"

struct PresentationInput { u8 primary; u8 reserved_01; u8 secondary; u8 reserved_03[0x4d]; s32 coordinate; u8 reserved_54[4]; u32 flags; s32 script; };
struct PresentationWork {
    s32 flags;
    s32 secondary_is_low_id;
    s32 primary_id;
    s32 secondary_id;
    s32 initial_value;
    s32 entry_count;
    s32 battle_mode;
    s32 scripted;
    s32 reserved_20;
    s16 members[8];
    u8 values[8][4];
};
struct MotionEntry { u8 reserved_00[39]; u8 count; void *children[1]; };
struct MotionChild { u8 reserved_00[5]; u8 value; };
extern struct BattlePresentationTransition *gTransitionWork;
extern u8 *gBattleWork;
s32 Scheduler_AddOrUpdateCallback(void *, s32);
void Object_SetMode(void *, s32);
void ObjectDispatch_ApplyValueToChildrenFar(void *, s32);
void UiWindow_DrawPartyStatusContentsFar(s32);
void Actor_ResetMotionAtAnchor(s32);
s32 BattlePres_BuildTargetList(void *, struct PresentationWork *);
u32 BattleEv_DispatchQueued(void);
u32 BattleEv_Push(u32, u32);
void BattleEventRuntime_WaitForReady(void);
void BattlePres_SetActorModes(u16 *, s32);
void BattleFx_PlayUnitElementEffect(s32, s32, s32, s32);
void BattlePres_RunWithZeroArguments(void);
void BattleFx_DispatchByIdRangeFar(struct PresentationWork *);
void BattleFx_DispatchModeFar(struct PresentationWork *);
void Audio_PlayCue(s32);

s32 Func_080ba978(struct PresentationInput *input, s32 flags)
{
    struct PresentationWork work;
    struct BattlePresentationTransition *transition = gTransitionWork;
    struct MotionObject *object;
    s32 i;

    if (input->flags & 0x40000) {
        transition->target_yaw = input->primary <= 7 ? -0x2000 : 0x5000;
        transition->frames = 60;
    } else {
        struct MotionObject *actor = GetBattleObjectSlot(input->primary)->object;
        s32 angle = (u16)ArcTan2(actor->x, actor->z);
        s32 current = angle - 0x1800;
        s32 target;
        s32 same_team;
        if (input->primary > 7)
            current = angle + 0x1800;
        current = (s16)current;
        if (input->primary > 7)
            target = -0x2000;
        else
            target = 0x2000;
        current += (target - current) * 3 / 4;
        if (input->secondary <= 7)
            same_team = input->primary <= 7;
        else
            same_team = input->primary > 7;
        if (same_team)
            current = input->primary <= 7 ? 0x2400 : -0x2400;
        if (transition->target_yaw != current)
            transition->target_yaw = current;
    }
    if (input->flags & 0x80000) {
        transition->target_yaw = input->primary <= 7 ? -0x2000 : 0x2000;
        transition->frames = 60;
    }

    BattlePres_BuildTargetList(input, &work);
    i = flags & 1;
    if (i)
        work.scripted = 1;
    BattlePres_SetActorModes(0, 0);
    UiWindow_DrawPartyStatusContentsFar(gBattleWork[65] & ~1);
    object = GetBattleObjectSlot(work.primary_id)->object;
    Object_SetMode(object, 3);
    ObjectDispatch_ApplyValueToChildrenFar(object, 16);
    Audio_PlayCue(0x9a);
    if (flags & 2)
        BattleFx_PlayUnitElementEffect(work.primary_id, input->coordinate, 1, 0);
    else if (!i)
        BattleFx_PlayUnitElementEffect(work.primary_id, input->coordinate, 0, 0);
    if (input->secondary <= 7)
        work.secondary_is_low_id = 1;
    else
        work.secondary_is_low_id = 0;

    for (i = 0; i != work.entry_count; i++) {
        struct MotionEntry *entry = GetMotionRecord(
            GetBattleObjectSlot(work.members[i])->object, 0);
        s32 count = entry->count - 1;
        s32 j;
        for (j = 0; j != count; j++)
            work.values[i][j] =
                ((struct MotionChild *)entry->children[j])->value;
    }
    if (input->script != 0) {
        if (input->script == 1) {
            BattleEv_Push(0, input->primary);
            BattleEv_Push(4, 0x856);
        } else {
            BattleEv_Push(4, 0x855);
        }
        BattleEv_DispatchQueued();
        BattlePres_RunWithZeroArguments();
    } else {
        Scheduler_AddOrUpdateCallback((void *)0x080bd899, 0xc80);
        if (work.flags) {
            if (input->flags & 0x4000)
                BattleFx_DispatchByIdRangeFar(&work);
            else
                BattleFx_DispatchModeFar(&work);
        } else {
            BattlePres_RunWithZeroArguments();
        }
        BattleEventRuntime_WaitForReady();
        Object_SetMode(object, 1);
        for (i = 0; i != work.entry_count; i++)
            Actor_ResetMotionAtAnchor(work.members[i]);
    }
    return 0;
}
