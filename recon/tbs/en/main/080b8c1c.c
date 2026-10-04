/* 2026-10-04 (second-wave finisher 1), alchemy diff 82, unchanged: the
 * draft now uses the shared BattlePresentationWork / Transition /
 * MotionObject types and indexes work.params[i][j] as the adopted
 * BattlePres_RunPsynergyCast does; the code is identical. Reading of the
 * ROM tail: its &work (r5) is a fresh pseudo that loop.c hoisted, and the
 * two spilled r2 copies at sp+0/sp+4 are low-register-only base pseudos
 * that found no low register because i holds r7 and the offset giv r6;
 * here cse2 folds the tail's &work into the long-lived r6, so i falls to
 * r8 and nothing spills. Tried without effect on that: inline
 * Work_GetCount/Work_GetActor/Work_ResetFrom helpers on &work or a tail
 * pointer (73-96), a w pointer for the head with work. in the tail (125,
 * frame 88), pinning i to r7 (138: a global register variable; 138 too
 * when the pin is scoped to a block from the first unit loop on).
 * -dg on this draft: the tail's &work is pseudo 44 (35 refs over 131
 * insns, the GCSE copy of &work for the whole body); -fno-gcse (diagnosis
 * only) gives sp-relative tail loads instead, so the ROM's fresh tail base
 * is not plain GCSE either. A head written with w = &work and w-> (the List
 * calls on work.actors, which keeps add rN, sp, #44 there) gives frame 88
 * with one spilled tail copy (122); its tail then reuses w. Inline
 * Pres_GetCount/GetActor/GetObject helpers on w or &work in the head loop
 * and tail: 91-122. 132 generated tail spellings on this draft (for/while/
 * do loops, i/j, work./w->/ids[] access, i = 0 before or after the stage
 * calls): best 72.
 */
/* 2026-10-04 (finisher 3), 82 rows off by dd.ts. The 8 bytes at sp+0 and
 * sp+4 are caller-save slots, not a local: in the ROM's reset loop the
 * units base (r2) and the count base (r1) are two pseudos for &work held in
 * call-clobbered registers and saved around Actor_ResetMotionAtAnchor,
 * because the three call-saved low registers are taken by a fresh &work
 * (r5, also read for primary_unit), the units offset (r6) and i (r7). Here
 * i sits in r8, the old &work (r6) reaches the tail and one copy suffices.
 * So the source must give i a low register and a separate &work in the
 * tail. Tried without effect: block pointers for the tail (one or two, 73),
 * an inline helper for the loop or for its reads, w-> throughout with
 * work. in the tail (134), pinning i to r7 (138), while and if/do forms,
 * j as the tail index, and the 1 of the party arm as a shared count
 * variable or pinned to r8. devsearch reaches 60 only with a frame reserve
 * and barriers, which the caller-save explanation rules out.
 *
 * 2026-10-04 (main-5), score 2269 unchanged: a work pointer throughout
 * (2353), one count variable stored after the arms (2867; in i, 3037) and
 * while forms of both unit loops (2269) did not help. Four minutes of
 * permute reached 1400 only by moving i = 0 to the reset loop and making
 * transition register; that keeps the 92/88 frame gap and moves i to r4.
 * The ROM's reset loop uses a fresh work address (r5) and two spilled
 * copies, which suggests the stage/reset tail is a separate block.
 *
 * DRAFT, rewritten 2026-10-02 in plain C: same instruction count, 51 lines
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
 * minutes of alchemy permute. */
#include "TYPES.H"
#include "BATTLE_EVENT.H"
#include "BATTLE_WORK.H"
#include "MOTION_OBJECT.H"
#include "BATTLE_PRESENTATION.H"
#include "ANIMSPR.H"

extern struct BattlePresentationTransition *gTransitionWork;
void WaitFrames(s32 frames);
s32 BattleParty_ListLivingUnits(s32 side_mask, s16 *unit_ids);
void UiWindow_DrawPartyStatusContentsFar(s32 mode);
void Object_SetMode(void *object, s32 mode);
void ObjectDispatch_ApplyValueToChildrenFar(struct MotionObject *object, s32 value);
void BattleFx_RunIceFleckBurstFar(struct BattlePresentationWork *work);
void BattleFx_DispatchModeFar(struct BattlePresentationWork *work);
void Actor_ResetMotionAtAnchor(s32 actor);

s32 BattlePres_RunUnitAction(s16 *action)
{
    struct BattlePresentationWork work;
    struct BattlePresentationTransition *transition = gTransitionWork;
    struct MotionObject *object;
    s32 facing;
    s32 i;

    if (action[0] > 4)
        facing = -0x2000;
    else
        facing = 0x2000;
    if (transition->target_yaw == facing) {
        transition->frames = 40;
        WaitFrames(40);
    } else {
        transition->target_yaw = facing;
        transition->frames = 40;
        WaitFrames(40);
    }
    work.kind = action[4];
    work.initial_value = action[6];
    work.actor = action[0];
    work.first_target = action[5];
    if (BattleObject_IsValidId(work.actor) < 0)
        return -1;
    if (work.first_target > 127)
        work.count = BattleParty_ListLivingUnits(2, work.actors);
    else
        work.count = BattleParty_ListLivingUnits(1, work.actors);
    UiWindow_DrawPartyStatusContentsFar(gBattleWork->party_status_mode & ~1);
    object = GetBattleObjectSlot(work.actor)->object;
    Object_SetMode(object, 3);
    ObjectDispatch_ApplyValueToChildrenFar(object, 16);
    if ((u16)action[5] <= 7) {
        work.side = 1;
        BattleParty_ListLivingUnits(1, work.actors);
        work.count = 1;
    } else {
        work.side = 0;
        BattleParty_ListLivingUnits(2, work.actors);
        work.count = 1;
    }
    for (i = 0; i != work.count; i++) {
        struct AnimationObject *animation =
            GetBattleObjectSlot(work.actors[i])->object->records;
        s32 j;

        for (j = 0; j != animation->count - 1; j++)
            work.params[i][j] = animation->entries[j]->param;
    }
    i = 0;
    work.kind = 0;
    work.variant = 0;
    BattleFx_RunIceFleckBurstFar(&work);
    work.kind = 1;
    BattleFx_RunIceFleckBurstFar(&work);
    work.kind = 2;
    BattleFx_RunIceFleckBurstFar(&work);
    work.kind = 3;
    BattleFx_RunIceFleckBurstFar(&work);
    work.kind = 0;
    BattleFx_DispatchModeFar(&work);
    Object_SetMode(object, 1);
    for (; i != work.count; i++)
        Actor_ResetMotionAtAnchor(work.actors[i]);
    Actor_ResetMotionAtAnchor(work.actor);
    return 0;
}
