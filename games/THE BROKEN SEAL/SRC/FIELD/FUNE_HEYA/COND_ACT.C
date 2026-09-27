/* H2 exact (2026-09-27): 892/892 bytes including all thirteen pool words,
 * zero differing halfwords or aligned edits. One numeric request owner and
 * a saved lifetime beginning at repeated dialogue retain independent r5/r0
 * literal loads from one pool word. Complete frame/calls/branches match.
 * H1's cse dump retains pseudo 38 before the initial message call and
 * substitutes it for r0. Numeric inline-argument temporaries themselves
 * disappear, unlike the symbol-backed pseudo 180. Start the saved request
 * at the subsequent repeated-dialogue sequence, after the first emote.
 * Predict the direct numeric r0 operand survives CSE while the later saved
 * request shares the same SI minipool value. Freeze the r0 reload and read
 * any remaining saved-load movement separately. Second/final causal model.
 * Admitted mixed witness is preserved at 6453a4f72.
 *
 * NONMATCHING admitted reload witness: 896/892 bytes, 9 halfwords / 6 edits.
 * 2026-09-27 repeat is byte-identical to historical sun-east-3b1-preload.
 * Complete normalized diff: the extra numeric 0xa01b pool word and its four
 * dependent literal offsets only. The independent r5 then r0 loads survive;
 * frame, calls, branches and all other instructions retain reference shape.
 * -da assembly equals normal compilation. Fourteen pool words versus thirteen.
 * Keep the symbol-backed saved request distinct from the numeric call
 * operand, as in the historical preload output. The r0 pool load is the
 * admission invariant even with a larger whole-owner pool. The two-edit
 * diagnostic source remains in Git at 0d2e51d86; rejected one-owner H1 at
 * f814cb136. No new DONE bytes.
 *
 * REJECTED H1: 892/892 bytes, 5 halfwords / 5 aligned edits (2026-09-27).
 * One numeric owner for the packed speaker request gives exactly one SI
 * pool word, but fails reload admission. cse changes insn 885 from the
 * numeric constant to saved pseudo 38; greg assigns it r5. sched2 then
 * hoists the r5 initialization above EventSetMessage. The complete diff
 * has no other regions; all thirteen pool words/offsets match. -da assembly
 * is identical to ordinary compilation. Preserve this counterexample in
 * Git, then retain the admitted mixed-constant preload witness instead.
 * No new DONE bytes. No wrapper/declaration/width sweep is justified.
 *
 * H1 hypothesis: one numeric owner for the packed speaker request.
 * Inversion admission: reload speaker into r0 from the pool, not r5.
 * Historical preload output reproduces this load but has two words: the
 * numeric 40987 and Value_0000a01b. arm.c's add_minipool_forward_ref requires
 * equal RTL code, mode and value, so linker equality cannot deduplicate them.
 * Current CSE already shares symbol pseudo 180 before allocation; greg maps
 * it to r5 and the call operand remains a copy. Replace the artificial link
 * constant across this owner, retaining the explicit saved speaker lifetime.
 * Predict one SI pool word and the r0 reload; inspect any earlier r5 hoist
 * separately. Budget two causal models, 25 minutes. Require whole 892 bytes
 * including pools, repeat, compare-all/coverage/verify for adoption.
 *
 * Previous NONMATCHING: 892 of 892 bytes, 2 differing halfwords (2026-09-24). Flag
 * branches restructured from the listing (0x92b/0x929 walk to x 0x1d6,
 * 0x92a/else to 0x19a). The 0x1b0 x coordinate lands in r8 because v7 = 0 is
 * set early in the branch (sched2 sinks it), and 0xa01b is a Value_ link
 * symbol so its r5 load is not hoisted above the message call. Remaining: the
 * ShowMessageAndWait speaker is copied from r5 where the reference reloads it
 * from the same pool entry. Numeric speaker plus an explicit saved-link
 * lifetime reproduced the load but added a separate four-byte pool entry;
 * isolating the call in a one-pass block left four halfwords different.
 * A halfword-mode speaker introduced an early pool and grew to 908 bytes.
 * 2026-09-27: transferred canonical FIELD_EVENT.H signatures, pointer-returning
 * actor lookup and FieldActor coordinate accesses from exact deck scenes.
 * H1 remains byte-identical to the 892-byte baseline (cmp), 2 halfwords/2 edits.
 * H2 reuses Event_ShowMessageAndWait, already exact in other cabin scenes;
 * the complete output is again byte-identical. Preserve these proven types,
 * but stop this interface boundary: neither changes the speaker reload. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

void FieldScene_RunSceneStep();
void OverlayObject_SetPositionAndHeading();
void FuneHeya_PlaceFoundActors();
s32 FuneHeya_FindFirstSetFlag();
void FieldScene_RunStepThen10();
void FieldScene_CallPairWith10();



/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

enum { CabinSpeakerRequest = 0xa01b };

void Scene_RunConditionalActorPresentation(s32 a0)
{
    u32 i;
    s32 rec7;
    struct FieldActor *record;
    s32 v6;
    s32 v7;
    s32 base5_a01b;
    s32 x;

    Engine_EventBegin();
    FieldScene_RunSceneStep(24, 0, 0);
    x = 0x1b0;
    v6 = 0x8000;
    OverlayObject_SetPositionAndHeading(0, x, 134, v6);
    FuneHeya_PlaceFoundActors(1);
    Engine_TaskWait(1);
    Engine_EventOpenScreen();
    Call3(Engine_ActorSetSpeed, 0, 0xcccc, 0x6666);
    Call3(Engine_ActorWalkToAndWait, 0, 0x196, 134);
    Call3(Engine_ActorWalkToAndWait, 0, 0x196, 152);
    Call3(Engine_ActorWalkToAndWait, 0, 0x1a5, 152);
    Engine_ActorRunRepeatedMotion(27, 1);
    Engine_EventWait(20);
    Engine_ActorFaceEachOther(27, 0, 10);
    if (Value1(Engine_GameFlagIsSet, 0x300) == 0) {
    } else {
        rec7 = FuneHeya_FindFirstSetFlag(a0, 0);
        Engine_ActorRunRepeatedMotion(27, 1);
        Engine_EventWait(20);
        Engine_ActorFaceEachOther(27, 0, 10);
        Call1(Engine_EventSetMessage, 0x1ebc);
        Call1(FieldScene_RunStepThen10, CabinSpeakerRequest);
        Engine_ActorSetAnimationAndWait(0, 3);
        v7 = 0;
        Call3(Engine_ActorSetSpeed, 0, 0x10000, v6);
        ((void (*)())Engine_ActorWalkToAndWait)(0, x, 168);
        Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
        record = Engine_ActorGet(0);
        if (record != 0) {
            Engine_ActorSetPosition(rec7, record->x.fixed, record->z.fixed);
        }
        Call3(Engine_ActorSetSpeed, rec7, 0x10000, v6);
        Call3(Engine_ActorWalkToAndWait, rec7, 0x1c0, 168);
        Call3(Engine_ActorFaceDirection, rec7, 0xb000, 20);
        Call3(Engine_ActorFaceDirection, 27, 0x3000, 20);
        Engine_ActorSetAnimation(27, 3);
        FieldScene_RunStepThen10(27);
        Call3(Engine_ActorShowEmote, rec7, 0x102, 60);
        Engine_ActorRunRepeatedMotion(27, 1);
        Engine_ActorSetAnimation(27, 3);
        FieldScene_RunStepThen10(27);
        Engine_ActorSetAnimationAndWait(rec7, 3);
        if (Value1(Engine_GameFlagIsSet, 0x92b) != 0) {
            Call3(Engine_ActorFaceDirection, 0, 0x2000, 0);
            Call3(Engine_ActorFaceDirection, 27, 0x3000, 0);
            Call3(Engine_ActorWalkToAndWait, rec7, 0x1d6, 204);
            FieldScene_CallPairWith10(rec7, 0xb000);
        } else if (Value1(Engine_GameFlagIsSet, 0x92a) != 0) {
            Call3(Engine_ActorWalkToAndWait, 0, 0x1a6, 154);
            Call3(Engine_ActorFaceDirection, 0, 0x6000, 0);
            Call3(Engine_ActorFaceDirection, 27, 0x5000, 0);
            Call3(Engine_ActorWalkToAndWait, rec7, 0x19a, 204);
            v7 = 1;
            FieldScene_CallPairWith10(rec7, 0xd000);
        } else if (Value1(Engine_GameFlagIsSet, 0x929) != 0) {
            Call3(Engine_ActorFaceDirection, 0, 0x2000, 0);
            Call3(Engine_ActorFaceDirection, 27, 0x3000, 0);
            Call3(Engine_ActorWalkToAndWait, rec7, 0x1d6, 172);
            FieldScene_CallPairWith10(rec7, 0xb000);
        } else {
            Call3(Engine_ActorWalkToAndWait, 0, 0x1a6, 154);
            Call3(Engine_ActorFaceDirection, 0, 0x6000, 0);
            Call3(Engine_ActorFaceDirection, 27, 0x5000, 0);
            Call3(Engine_ActorWalkToAndWait, rec7, 0x19a, 172);
            v7 = 1;
            FieldScene_CallPairWith10(rec7, 0xd000);
        }
        Engine_ActorFaceEachOther(27, 0, 20);
        Call1(FieldScene_RunStepThen10, 0x201b);
        Engine_ActorSetAnimationAndWait(0, 3);
        Engine_ActorSetAnimationAndWait(27, 3);
        Call3(Engine_ActorSetSpeed, 27, 0x10000, 0x8000);
        if (v7 != 0) {
            Call3(Engine_ActorWalkToAndWait, 27, 0x1ac, 164);
            Call3(Engine_ActorWalkToAndWait, 27, 0x198, 164);
        }
        Call3(Engine_ActorWalkToAndWait, 27, 0x198, 134);
        Call3(Engine_ActorWalkTo, 27, 0x1b8, 134);
        Engine_EventWait(40);
        FieldScene_RunSceneStep(9, 10, 0);
        goto L_02004592;
    }
    Call1(Engine_EventSetMessage, 0x1eb7);
    Event_ShowMessageAndWait(CabinSpeakerRequest, 0, 40);
    Call3(Engine_ActorShowEmote, 27, 0x101, 60);
    /* FAKEMATCH: start the saved speaker at repeated dialogue; the initial
     * call must retain its independent load from the same literal word. */
    base5_a01b = CabinSpeakerRequest;
    FieldScene_RunStepThen10(base5_a01b);
    Call2(Engine_ActorSetAttachedEffect, 0, 0x102);
    Engine_EventWait(60);
    Call3(Engine_ActorShowEmote, 27, 0x103, 40);
    Engine_ActorStartRepeatedMotion(27, 2);
    FieldScene_RunStepThen10(base5_a01b);
    Call3(Engine_ActorShowEmote, 27, 0x105, 40);
    FieldScene_RunStepThen10(base5_a01b);
    Engine_ActorSetAnimationAndWait(27, 4);
    FieldScene_RunStepThen10(base5_a01b);
    Engine_ActorSetAnimationAndWait(0, 3);
    Engine_EventWait(20);
    Engine_EventRequestExit(4);
    L_02004592:;
}
