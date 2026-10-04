/* 2026-10-04 (finisher 1): 169 -> 53 differing instructions. The header
 * now gives BattlePresentationWork its real tail, s16 actors[8] then
 * u8 params[8][4] at 0x34; indexing work.params[i][j] with the count read
 * each pass gives the native per-iteration addressing (no hoisted base, no
 * extra r9 save; plan r7, mode sl, object r8 all match). Left: the same-side
 * test. Native evaluates each arm's actor test as a value and tests it in the
 * arm (cmp/bne/b, cmp/beq), and copies the actor id (adds rN, r4, #0) for
 * every test; this draft shares one copy and joins the arms first. Tried
 * here: ?: in the if, (a <= 7) == (b <= 7), explicit 0/1 assignments, inverted
 * tests (all 53-61); static inline IsParty/IsEnemy only in the ?: arms gives
 * the native arm shape but rereads the id from memory and moves plan to r6
 * (80-86); inline for every test 82-93. */
/* Draft: complete native extent [080ba978,080babdc), 612 bytes.
 * Earlier 2026-10-02 work recorded 12 differing instructions with the native
 * frame/register allocation. Byte predicates recovered the same-side copy
 * but made shifted copies in three single side tests; rereading the actor
 * moved the plan from r7. The post-angle cast was also scheduled late.
 * The prior form kept one actor local, one index for both target loops,
 * the child-count test in its loop and a conditional three-quarter target.
 * Those are attempt history, not fresh proof. Scheduling-only byte wrappers
 * are removed. The unsigned low-16 angle and signed-16 adjustment remain.
 * EN message 0x855 is insufficient PP; 0x856 is blocked Psynergy. They still
 * need catalogue names before edition adoption; no symbol is invented here.
 * 2026-10-03 T0 typed baseline: 612/612 bytes, local frame 88 and work sp+4;
 * saved-register area 28/24 bytes (extra r9), total stack 116/112. Plan
 * uses r8/r7 and mode r7/r10. Full relocation-normalized comparison has
 * 540 differing bytes, not an aligned instruction score. All 25 call targets
 * and their order agree. Both have 28 relocations; all eight pool words
 * and their relocation positions agree.
 * Instructions total 264 in both; stores are 9/10 and branches 62/63.
 * The natural byte-view loop rereads count instead of caching count - 1;
 * its first actor-index spill disappears. Direct side tests omit native
 * boolean/copy sequences, and register allocation and scheduling differ.
 * Native zero return is retained; production's discarded void declaration
 * still needs closure before adoption. Stopped after this single baseline,
 * with no device, scheduling follow-up or matching-C credit.
 * T1, same date: sample count - 1 once, matching the native lifetime.
 * This is 604/612 bytes: 567 differing bytes in the shared 604-byte span
 * plus eight missing bytes after full relocation normalization. Local frame
 * 88/work sp+4 and the extra r9 save remain, with total stack 116/112.
 * Instructions are 260/264, stores 9/10 and branches 62/63. All 25 call
 * targets and their order agree; both have 28 relocations. The eight resolved
 * pool words agree but start eight bytes earlier. The bound is held in r4
 * rather than native r12; repeated count loads disappear, while the missing
 * first index spill, direct side-test shape and register differences remain.
 * Stopped after this one count-lifetime change, with no device or further
 * variation. T0 contracts and deferred adoption closures are unchanged.
 * T2, 2026-10-04: score 4465 -> 3330. Each side test re-reads
 * plan->actor_id (CSE turns the re-reads into the native register copies,
 * adds rN, r4, #0) and the same-side result is a 0/1 local set in an
 * if/else. Native still tests that value inside each arm (cmp/bne/b and
 * cmp/beq) where this draft joins the arms first; plan/mode live in r7/r10
 * natively but r8/r7 here, which adds the r9 save. Static inline
 * IsParty/IsSameSide predicates scored 4775-5640 (they materialize all four
 * tests); (u8)/(s8) casts on the arms fold away (4360).
 */
#include "TYPES.H"
#include "BATTLE_EVENT.H"
#include "BATTLE_WORK.H"
#include "MOTION_OBJECT.H"
#include "BATTLE_PRESENTATION.H"
#include "ANIMSPR.H"
#include "OBJDISP.H"
#include "CALLBACK_SCHEDULER.H"
#include "FIXED_MATH.H"

extern struct BattlePresentationTransition *gTransitionWork;
u16 ArcTan2(s32 x, s32 y);
void Object_SetMode(void *object, s32 mode);
void ObjectDispatch_ApplyValueToChildrenFar(struct DispatchObject *object, s32 value);
void UiWindow_DrawPartyStatusContentsFar(s32 mode);
void Actor_ResetMotionAtAnchor(s32 actor);
void BattlePres_SetActorModes(u16 *actors, s32 mode);
void BattleFx_PlayUnitElementEffect(s32 actor, s32 kind, s32 mode, s32 variant);
void BattlePres_RunWithZeroArguments(void);
void BattleFx_DispatchByIdRangeFar(s32 *work);
void BattleFx_DispatchModeFar(s32 *work);
void AudioCommand_PlayFar(s32 cue);
void BattleEvent_Playback(void);

s32 Func_080ba978(struct BattlePlan *plan, s32 mode)
{
    struct BattlePresentationWork work;
    struct BattlePresentationTransition *transition = gTransitionWork;
    struct MotionObject *object;
    s32 scripted;
    s32 i;

    if (plan->presentation_flags & 0x40000) {
        transition->target_yaw = plan->actor_id <= 7 ? -0x2000 : 0x5000;
        transition->frames = 60;
    } else {
        struct MotionObject *actor = GetBattleObjectSlot(plan->actor_id)->object;
        s32 angle = (u16)ArcTan2(actor->x, actor->z);
        s32 current = angle - 0x1800;
        s32 same;

        if (plan->actor_id > 7)
            current = angle + 0x1800;
        current = (s16)current;
        current += ((plan->actor_id <= 7 ? 0x2000 : -0x2000) - current) * 3 / 4;
        if (plan->target_ids[0] <= 7)
            same = plan->actor_id <= 7;
        else
            same = plan->actor_id > 7;
        if (same)
            current = plan->actor_id <= 7 ? 0x2400 : -0x2400;
        if (transition->target_yaw != current)
            transition->target_yaw = current;
    }
    if (plan->presentation_flags & 0x80000) {
        transition->target_yaw = plan->actor_id <= 7 ? -0x2000 : 0x2000;
        transition->frames = 60;
    }

    BattlePres_BuildTargetList(plan, &work);
    scripted = mode & 1;
    if (scripted)
        work.flags = 1;
    BattlePres_SetActorModes(0, 0);
    UiWindow_DrawPartyStatusContentsFar(gBattleWork->party_status_mode & ~1);
    object = GetBattleObjectSlot(work.actor)->object;
    Object_SetMode(object, 3);
    ObjectDispatch_ApplyValueToChildrenFar((struct DispatchObject *)object, 16);
    AudioCommand_PlayFar(0x9a);
    if (mode & 2)
        BattleFx_PlayUnitElementEffect(work.actor, plan->range_index, 1, 0);
    else if (!scripted)
        BattleFx_PlayUnitElementEffect(work.actor, plan->range_index, 0, 0);
    if (plan->target_ids[0] <= 7)
        work.side = 1;
    else
        work.side = 0;

    for (i = 0; i != work.count; i++) {
        struct AnimationObject *animation = GetMotionRecord(
            GetBattleObjectSlot(work.actors[i])->object, 0);
        s32 j;

        for (j = 0; j != animation->count - 1; j++)
            work.params[i][j] = animation->entries[j]->param;
    }
    if (plan->failure != 0) {
        if (plan->failure == 1) {
            BattleEv_Push(0, plan->actor_id);
            BattleEv_Push(4, 0x856);
        } else {
            BattleEv_Push(4, 0x855);
        }
        BattleEv_DispatchQueued();
        BattlePres_RunWithZeroArguments();
    } else {
        Scheduler_AddOrUpdateCallback((s32)BattleEvent_Playback, 0xc80);
        if (work.kind) {
            if (plan->presentation_flags & 0x4000)
                BattleFx_DispatchByIdRangeFar((s32 *)&work);
            else
                BattleFx_DispatchModeFar((s32 *)&work);
        } else {
            BattlePres_RunWithZeroArguments();
        }
        BattleEventRuntime_WaitForReady();
        Object_SetMode(object, 1);
        for (i = 0; i != work.count; i++)
            Actor_ResetMotionAtAnchor(work.actors[i]);
    }
    return 0;
}
