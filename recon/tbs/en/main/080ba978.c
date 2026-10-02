/* DRAFT (43 instructions differ), reworked 2026-10-02: same frame and the
 * same saved registers as the ROM; input, transition, work and the scripted
 * bit sit in the ROM's registers.
 * 1. The ROM reads the primary id once into r4 and copies it into a fresh
 *    register before each of its four tests; here the first copy is reused
 *    and the same-side test reloads the byte. A local for the id, or an
 *    inline predicate per test, both score worse.
 * 2. In both member loops the ROM keeps the index in r4, saved around the
 *    calls, with the row offset in r6 and the child count copied from r3 to
 *    ip after the zero test; here the index takes r6, the offset r8 and the
 *    count r4, and flags and the object trade r8 and r10.
 * Settled: the far target is a conditional expression inside the three-quarter
 * step; the same-side test is an inline function of unsigned ids, which is
 * what materialises its 0/1 result on both arms; the scripted bit and each
 * loop's index are separate variables.
 * Still literal: messages 0x855 "But the Psynergy was blocked!" and 0x856
 * "...But doesn't have enough PP!" need catalogue names, and the callback is
 * BattleEvent_Playback. */
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

static inline s32 SameSide(u32 first, u32 second)
{
    if (second <= 7)
        return first <= 7;
    return first > 7;
}

s32 Func_080ba978(struct PresentationInput *input, s32 flags)
{
    struct PresentationWork work;
    struct BattlePresentationTransition *transition = gTransitionWork;
    struct MotionObject *object;
    s32 scripted;
    s32 i;

    if (input->flags & 0x40000) {
        transition->target_yaw = input->primary <= 7 ? -0x2000 : 0x5000;
        transition->frames = 60;
    } else {
        struct MotionObject *actor = GetBattleObjectSlot(input->primary)->object;
        s32 angle = (u16)ArcTan2(actor->x, actor->z);
        u32 primary = input->primary;
        s32 current = angle - 0x1800;
        if (primary > 7)
            current = angle + 0x1800;
        current = (s16)current;
        current += ((primary <= 7 ? 0x2000 : -0x2000) - current) * 3 / 4;
        if (SameSide(primary, input->secondary))
            current = primary <= 7 ? 0x2400 : -0x2400;
        if (transition->target_yaw != current)
            transition->target_yaw = current;
    }
    if (input->flags & 0x80000) {
        transition->target_yaw = input->primary <= 7 ? -0x2000 : 0x2000;
        transition->frames = 60;
    }

    BattlePres_BuildTargetList(input, &work);
    scripted = flags & 1;
    if (scripted)
        work.scripted = 1;
    BattlePres_SetActorModes(0, 0);
    UiWindow_DrawPartyStatusContentsFar(gBattleWork[65] & ~1);
    object = GetBattleObjectSlot(work.primary_id)->object;
    Object_SetMode(object, 3);
    ObjectDispatch_ApplyValueToChildrenFar(object, 16);
    Audio_PlayCue(0x9a);
    if (flags & 2)
        BattleFx_PlayUnitElementEffect(work.primary_id, input->coordinate, 1, 0);
    else if (!scripted)
        BattleFx_PlayUnitElementEffect(work.primary_id, input->coordinate, 0, 0);
    if (input->secondary <= 7)
        work.secondary_is_low_id = 1;
    else
        work.secondary_is_low_id = 0;

    {
    for (i = 0; i != work.entry_count; i++) {
        struct MotionEntry *entry = GetMotionRecord(
            GetBattleObjectSlot(work.members[i])->object, 0);
        s32 j;
        for (j = 0; j != entry->count - 1; j++)
            work.values[i][j] =
                ((struct MotionChild *)entry->children[j])->value;
    }
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
        {
        for (i = 0; i != work.entry_count; i++)
            Actor_ResetMotionAtAnchor(work.members[i]);
        }
    }
    return 0;
}
