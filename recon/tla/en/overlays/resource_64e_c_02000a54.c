/* CANONICAL DRAFT: VINASU_CHOJO opening bank, 2026-10-02.
 * Saved-tree default scorer: startup610/15, setup11025/152, departure
 *24410/362; overlay imports are unresolved in its main-only namespace.
 * This diagnostic does not replace the complete native identity judgments.
 * Intended adoption expands the existing FIELD/VINASU_CHOJO/PREPARE.C in
 * native order: startup, current idle hook4, setup, departure. No raw gap
 * or new per-function owner is crossed. Current listing: resource_64e.
 *
 * Closed ordinary forms: enum-first JA4172/4236, Intl4192/4264; truthful
 * table-owned Msg symbol JA4176/4236, Intl4196/4264. An EventWork-local
 * form and separate checked actor scopes emit identical EN assembly to
 * the latter. No compiler-steering device, ASM, pin or artificial storage.
 * The self-contained PartyState data view emits Intl4204/native4264;
 * its ordinary halfword cast makes GCC load5 and22 from scalar pools.
 * JA remains4176/native4236. This representation is not assembly-identical
 * to the proposed typed-header form, and no such fidelity is claimed.
 *
 * Current EN complete spans: startup212/native212 (97 nonrelocation byte
 * differences); idle4/4 exact; setup528/native516 (300, includes natural
 * internal2 after its526-byte symbol); departure3460/native3532 (1668).
 * JA startup184/native184 has zero nonrelocation differences; every other
 * edition still requires its complete binding/extent to match for credit.
 * These are same-offset byte differences, not an instruction-edit score.
 *
 * All483 ordered calls per edition resolve to the actual current native
 * targets (2898/2898 identities). Natural module offsets differ, so this
 * is an UNLINKED near miss; full emitted BL/pool equality is not claimed.
 * Broad constant sharing/high-register lifetimes and scalar pool/extent
 * differences remain. Native numeric values judge only; the source uses
 * the actual PO-owned Msg token whole, never a literal message number.
 *
 * The earlier local record named only observed1ac/1b4 stores, and used a
 * halfword view of the international1e4/1e6 reserve. Those fields now have
 * maintained owners below; the earlier byte judgments need a fresh run.
 * Func_02001c34 has an ignored opaque pointer-return view pending durable
 * API ownership; remaining numeric imports are current raw physical roles
 * and confer no resident-source readiness or credit.
 */

/* The current EventWork and PartyState owners now name the draft fields.
 * Their maintained views replace the former local record; retained scores
 * describe the earlier attempt, not a fresh compile or complete match. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "PARTY_STATE.H"
#include "RAM_BUFFER.H"
#include "EDITION.H"
#include "OBJDISP.H"
extern u8 MsgVinasuChojoAreYouSureWeShouldBeLeaving[];

void Object_SetPartAttribute(struct DispatchObject *, s32);
void ObjectMotion_SetSpeedParameters(u32, s32, s32);
void ObjectMotion_ArmCallback(s32, s32, s32);
void ObjectMotion_SetPositionAndReset(u32, s32, s32);
void ObjectMotion_ResetAndSetPositionInMode2(u32, s32, s32);
void ObjectMotion_ResetAndSetPosition(u32, s32, s32);
void ObjectMotion_CommitCurrentPositionAndActivate(u32);
void ObjectMotion_CommitPositionAndActivate(u32, s32, s32);
void ObjectMotion_SetVariantCallback(u32, s32);
void Motion_SetModeAndWaitAnimation(u32, s32);
void Motion_SetVarCbAndRefresh(u32, s32);
void Motion_CamBounds(s32, s32, s32, s32);
void Object_LinkObjectAndSetCallback(u32, u32);
void Object_LinkPair(s32, s32, s32);
void Object_AttachWorkTargetToObject(s32, s32);
void Event_SetStatus1c6(void);
void Event_ClearStatus1c6(void);
void Event_WaitValue1c8Frames(void);
void GameFlag_ClearBit(s32);
void Party_RemoveActiveOwner(s32);

/* Current numeric import roles; no resident owner is adopted or renamed. */
void Func_02001be4(u32, s32, s32);
void Func_02001c1c(s32);
void Func_02001c24(s32, s32);
void *Func_02001c34(u32, s32, s32);
void Func_02001c4c(void);
void Func_02001c54(s32);
void Func_02001ccc(u32, s32);
void Func_02001cd4(u32);
void Func_02000a18(void);
void SceneState_RunActor13AtColumn42Setup(void);

void Func_02000b2c(void);
void Func_02000d30(void);

/* The entering party joins the lighthouse departure, or hides its actors. */
s32 Func_02000a54(void)
{
    struct EventWork *work = Ram_HeapSlots->event_work;

    work->screen_effect = 0x204;
    if (gPartyState.entrance == 99) {
#if EDITION_INTERNATIONAL
        gPartyState.saved_scene = 5;
        gPartyState.saved_entrance = 22;
#endif
        Engine_EventBegin();
        Engine_EventPrepareSpeakers(0);
        Func_02000b2c();
        Func_02000d30();
        Engine_EventEnd();
    } else {
        Object_SetPartAttribute((struct DispatchObject *)Engine_ActorGet(10), 6);
        Object_SetPartAttribute((struct DispatchObject *)Engine_ActorGet(17), 6);
        Object_SetPartAttribute((struct DispatchObject *)Engine_ActorGet(18), 6);
        Object_SetPartAttribute((struct DispatchObject *)Engine_ActorGet(19), 6);
        Object_SetPartAttribute((struct DispatchObject *)Engine_ActorGet(20), 6);
        Object_SetPartAttribute((struct DispatchObject *)Engine_ActorGet(21), 6);
        Object_SetPartAttribute((struct DispatchObject *)Engine_ActorGet(22), 6);
        Object_SetPartAttribute((struct DispatchObject *)Engine_ActorGet(23), 6);
        Object_SetPartAttribute((struct DispatchObject *)Engine_ActorGet(24), 6);
        Object_SetPartAttribute((struct DispatchObject *)Engine_ActorGet(25), 6);
    }
    return 0;
}

/* The scene's last loader hook; this scene needs nothing. */
s32 Scene_PrepareMap(void)
{
    return 0;
}

/* Positions the three speakers and the lighthouse camera before departure. */
void Func_02000b2c(void)
{
    struct EventWork *work;

    Func_02001be4(4, (712 << 16), (816 << 16));
    Func_02001be4(5, (696 << 16), (840 << 16));
    Func_02001be4(8, (728 << 16), (840 << 16));
    ObjectMotion_ArmCallback(4, 0x4000, 0);
    ObjectMotion_ArmCallback(5, 0xc000, 0);
    ObjectMotion_ArmCallback(8, 0xc000, 0);
    ObjectMotion_SetSpeedParameters(4, (1 << 16), 0x8000);
    ObjectMotion_SetSpeedParameters(5, (1 << 16), 0x8000);
    ObjectMotion_SetSpeedParameters(8, (1 << 16), 0x8000);
    ObjectMotion_SetSpeedParameters(9, (1 << 16), 0x8000);
    Motion_CamBounds((664 << 16), -1, (824 << 16), 0);
    work = Ram_HeapSlots->event_work;
    work->screen_effect = 0x100;
    work->delay = 60;
    Event_SetStatus1c6();
    Event_WaitValue1c8Frames();
    Engine_EventWait(20);
    Motion_SetModeAndWaitAnimation(4, 3);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(5, 3);
    Motion_SetModeAndWaitAnimation(8, 3);
    Engine_EventWait(10);
    ObjectMotion_SetPositionAndReset(4, 0x298, 0x338);
    ObjectMotion_ArmCallback(4, 0xc000, 0);
    Engine_EventWait(10);
    ObjectMotion_SetPositionAndReset(8, 0x2c8, 0x338);
    ObjectMotion_ArmCallback(5, 0xa000, 0);
    Engine_EventWait(20);
    Func_02000a18();
    SceneState_RunActor13AtColumn42Setup();
    Engine_EventWait(20);
    ObjectMotion_SetPositionAndReset(8, 0x2d8, 0x348);
    ObjectMotion_ArmCallback(8, 0xc000, 0);
    ObjectMotion_ArmCallback(5, 0xc000, 0);
    ObjectMotion_SetPositionAndReset(4, 0x2c8, 0x330);
    ObjectMotion_ArmCallback(4, 0x4000, 0);
    Engine_EventWait(30);
    Motion_SetModeAndWaitAnimation(4, 3);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(5, 3);
    Motion_SetModeAndWaitAnimation(8, 3);
    Engine_EventWait(10);
    Object_LinkObjectAndSetCallback(5, 4);
    Object_LinkObjectAndSetCallback(8, 4);
    ObjectMotion_SetPositionAndReset(4, 0x2e8, 0x330);
    ObjectMotion_SetPositionAndReset(4, 0x2e8, 0x30c);
    Engine_AudioPlayCue(129);
    Func_02001ccc(4, -1);
    Engine_EventWait(50);
    ObjectMotion_ArmCallback(5, 0xc000, 0);
    ObjectMotion_ArmCallback(8, 0xc000, 0);
    Func_02001be4(4, 0, 0);
}

/* The departure choreography, in the original event order. */
void Func_02000d30(void)
{
    struct EventWork *work;

    Func_02001c1c((s32)MsgVinasuChojoAreYouSureWeShouldBeLeaving);
    Object_AttachWorkTargetToObject(8, 1);
    Func_02001c4c();
    Engine_EventWait(30);
    ObjectMotion_SetPositionAndReset(5, 0x298, 0x348);
    ObjectMotion_ArmCallback(5, 0, 0);
    ObjectMotion_SetPositionAndReset(8, 0x2b8, 0x348);
    Engine_EventWait(20);
    Motion_SetModeAndWaitAnimation(8, 3);
    Engine_EventWait(10);
    Motion_SetModeAndWaitAnimation(5, 3);
    Engine_EventWait(10);
    ObjectMotion_ResetAndSetPositionInMode2(8, 0x2b8, 0x398);
    Engine_EventWait(10);
    ObjectMotion_SetPositionAndReset(5, 0x298, 0x378);
    ObjectMotion_CommitCurrentPositionAndActivate(8);
    Func_02001c34(8, 0x100, 30);
    ObjectMotion_ArmCallback(8, 0xa000, 0);
    Engine_EventWait(20);
    ObjectMotion_SetPositionAndReset(8, 0x2b8, 0x378);
    ObjectMotion_ArmCallback(5, 0, 0);
    ObjectMotion_ArmCallback(8, 0x8000, 0);
    Engine_EventWait(20);
    Motion_CamBounds((696 << 16), -1, (920 << 16), 1);
    Func_02001c4c();
    Engine_EventWait(20);
    Motion_SetVarCbAndRefresh(5, 2);
    Engine_EventWait(10);
    Func_02001c24(5, 0);
    Engine_EventWait(10);
    Func_02001c34(8, 0x101, 50);
    Func_02001c24(5, 0);
    Engine_EventWait(10);
    Motion_SetModeAndWaitAnimation(8, 3);
    Engine_EventWait(20);
    Motion_SetVarCbAndRefresh(5, 2);
    Engine_EventWait(10);
    Func_02001c24(5, 0);
    Engine_EventWait(10);
    Func_02001c34(8, 0x101, 30);
    Func_02001c24(0x2008, 0);
    Engine_EventWait(10);
    ObjectMotion_ArmCallback(5, 0xe000, 0);
    Engine_EventWait(30);
    Func_02001c24(5, 0);
    Engine_EventWait(10);
    Func_02001c34(8, 0x102, 40);
    Func_02001c24(0x2008, 0);
    Engine_EventWait(10);
    Motion_SetModeAndWaitAnimation(5, 3);
    Engine_EventWait(10);
    Func_02001c24(5, 0);
    Engine_EventWait(30);
    Engine_AudioPlayCue(198);
    Func_02001be4(9, (680 << 16), (920 << 16));
    ObjectMotion_ArmCallback(9, 0xb000, 0);
    Func_02001cd4(9);
    ObjectMotion_ArmCallback(9, 0xb000, 0);
    Engine_EventWait(20);
    ObjectMotion_ArmCallback(5, 0x4000, 0);
    ObjectMotion_ArmCallback(8, 0x4000, 0);
    Engine_EventWait(10);
    Engine_EventWait(20);
    Motion_SetModeAndWaitAnimation(9, 4);
    Func_02001c24(9, 0);
    Engine_EventWait(10);
    ObjectMotion_SetVariantCallback(5, 2);
    Motion_SetVarCbAndRefresh(8, 2);
    Engine_EventWait(20);
    Object_LinkPair(5, 8, 40);
    Func_02001c34(9, 0x100, 30);
    Func_02001c24(9, 0);
    Engine_EventWait(10);
    ObjectMotion_ArmCallback(5, 0x4000, 0);
    ObjectMotion_ArmCallback(8, 0x4000, 0);
    Engine_EventWait(20);
    Func_02001c34(8, 0x102, 40);
    Func_02001c24(0x2008, 0);
    Engine_EventWait(10);
    Func_02001c34(9, 0x101, 30);
    Engine_EventWait(10);
    ObjectMotion_ArmCallback(9, 0, 0);
    Engine_EventWait(30);
    ObjectMotion_ArmCallback(9, 0x8000, 0);
    Engine_EventWait(30);
    ObjectMotion_ArmCallback(9, 0xc000, 0);
    Engine_EventWait(30);
    Func_02001c24(9, 0);
    Engine_EventWait(10);
    Motion_SetVarCbAndRefresh(5, 2);
    Engine_EventWait(20);
    ObjectMotion_SetSpeedParameters(5, 0x8000, 0x4000);
    Object_LinkObjectAndSetCallback(8, 5);
    ObjectMotion_CommitPositionAndActivate(5, 0, -32);
    Engine_EventWait(10);
    Func_02001c24(5, 0);
    Engine_EventWait(10);
    Func_02001c34(9, 0x105, 30);
    ObjectMotion_CommitPositionAndActivate(9, -8, -8);
    Func_02001c24(9, 0);
    Engine_EventWait(10);
    Motion_SetVarCbAndRefresh(8, 2);
    Engine_EventWait(10);
    ObjectMotion_ArmCallback(8, 0x5000, 0);
    Engine_EventWait(20);
    Func_02001c24(0x2008, 0);
    Engine_EventWait(10);
    Func_02001c34(9, 0x100, 30);
    ObjectMotion_ArmCallback(9, 0xe000, 0);
    Engine_EventWait(30);
    Func_02001c24(9, 0);
    Engine_EventWait(10);
    Func_02001c34(9, 0x101, 30);
    Func_02001c24(9, 0);
    Engine_EventWait(10);
    Func_02001c34(8, 0x102, 0);
    Func_02001c34(5, 0x102, 40);
    Func_02001c24(0x2008, 0);
    Engine_EventWait(10);
    Func_02001c34(9, 0x108, 30);
    ObjectMotion_ArmCallback(9, 0xf000, 0);
    Engine_EventWait(30);
    Func_02001c24(9, 0);
    Engine_EventWait(20);
    Func_02001c34(5, 0x106, 40);
    Func_02001c24(5, 0);
    Engine_EventWait(10);
    ObjectMotion_ArmCallback(9, 0xb000, 0);
    Engine_EventWait(20);
    ObjectMotion_ArmCallback(5, 0x3000, 0);
    Engine_EventWait(30);
    Func_02001c24(5, 0);
    Engine_EventWait(10);
    Func_02001c34(9, 0x109, 30);
    Engine_EventWait(20);
    Motion_SetVarCbAndRefresh(9, 2);
    Engine_EventWait(10);
    Func_02001c24(9, 0);
    Engine_EventWait(10);
    Motion_SetModeAndWaitAnimation(9, 4);
    Func_02001c24(9, 0);
    Engine_EventWait(10);
    Motion_SetModeAndWaitAnimation(5, 4);
    Engine_EventWait(10);
    ObjectMotion_SetSpeedParameters(5, (1 << 16), 0x8000);
    ObjectMotion_CommitPositionAndActivate(5, 0, 16);
    ObjectMotion_ArmCallback(5, 0x3000, 0);
    Engine_EventWait(10);
    Func_02001c24(5, 0);
    Engine_EventWait(10);
    Func_02001c34(5, 0x107, 30);
    Func_02001c24(5, 0);
    Engine_EventWait(10);
    Motion_SetVarCbAndRefresh(9, 2);
    Engine_EventWait(10);
    Func_02001c24(9, 0);
    Engine_EventWait(10);
    Motion_SetModeAndWaitAnimation(9, 7);
    Engine_EventWait(30);
    ObjectMotion_ArmCallback(8, 0x9000, 0);
    Engine_EventWait(20);
    Func_02001c34(8, 0x102, 40);
    Func_02001c24(0x2008, 0);
    Engine_EventWait(10);
    ObjectMotion_ArmCallback(5, 0x1000, 0);
    Engine_EventWait(20);
    Motion_SetModeAndWaitAnimation(5, 4);
    Func_02001c24(5, 0);
    Engine_EventWait(10);
    ObjectMotion_ArmCallback(5, 0x4000, 0);
    Engine_EventWait(20);
    ObjectMotion_CommitPositionAndActivate(5, 0, 16);
    ObjectMotion_ArmCallback(5, 0x3000, 0);
    Engine_EventWait(10);
    Func_02001c24(5, 0);
    Engine_EventWait(10);
    Motion_SetModeAndWaitAnimation(9, 4);
    Func_02001c24(9, 0);
    Engine_EventWait(10);
    Func_02001c34(5, 0x105, 50);
    Motion_SetVarCbAndRefresh(9, 2);
    Engine_EventWait(10);
    Func_02001c24(9, 0);
    Engine_EventWait(10);
    Motion_SetModeAndWaitAnimation(5, 3);
    Engine_EventWait(10);
    Func_02001c24(5, 0);
    Engine_EventWait(10);
    Motion_SetVarCbAndRefresh(8, 2);
    Engine_EventWait(10);
    Func_02001c24(0x2008, 0);
    Engine_EventWait(10);
    ObjectMotion_ArmCallback(5, 0, 0);
    Engine_EventWait(20);
    Motion_SetModeAndWaitAnimation(5, 3);
    Engine_EventWait(20);
    Motion_SetVarCbAndRefresh(9, 2);
    Engine_EventWait(10);
    Func_02001c24(9, 0);
    Engine_EventWait(10);
    ObjectMotion_ArmCallback(5, 0x3000, 0);
    Engine_EventWait(20);
    Motion_SetModeAndWaitAnimation(5, 3);
    Engine_EventWait(30);
    Func_02001c34(8, 0x102, 40);
    Func_02001c24(0x2008, 0);
    Engine_EventWait(10);
    ObjectMotion_ArmCallback(5, 0, 0);
    Engine_EventWait(20);
    Func_02001c24(5, 0);
    Engine_EventWait(10);
    Func_02001c34(8, 0x109, 30);
    Func_02001c24(0x2008, 0);
    Engine_EventWait(10);
    Motion_SetVarCbAndRefresh(5, 2);
    Engine_EventWait(10);
    Func_02001c24(5, 0);
    Engine_EventWait(10);
    Func_02001c34(5, 0x102, 40);
    Func_02001c24(5, 0);
    Engine_EventWait(10);
    Motion_SetModeAndWaitAnimation(9, 4);
    Func_02001c24(9, 0);
    Engine_EventWait(10);
    Func_02001c34(8, 0x105, 30);
    Func_02001c24(0x2008, 0);
    Engine_EventWait(10);
    Func_02001c34(5, 0x107, 30);
    Func_02001c24(5, 0);
    Engine_EventWait(10);
    ObjectMotion_ArmCallback(8, 0xe000, 0);
    Engine_EventWait(30);
    Motion_SetModeAndWaitAnimation(8, 3);
    Engine_EventWait(10);
    Func_02001c24(0x2008, 0);
    Engine_EventWait(10);
    Motion_SetVarCbAndRefresh(5, 2);
    Engine_EventWait(10);
    Func_02001c24(5, 0);
    Engine_EventWait(10);
    ObjectMotion_ArmCallback(8, 0x8000, 0);
    Engine_EventWait(30);
    ObjectMotion_CommitPositionAndActivate(5, 0, -16);
    Engine_EventWait(10);
    ObjectMotion_ArmCallback(5, 0xe000, 0);
    Engine_EventWait(30);
    Motion_SetModeAndWaitAnimation(5, 3);
    Engine_EventWait(20);
    Func_02001c24(5, 0);
    Engine_EventWait(10);
    ObjectMotion_ArmCallback(9, 0xe000, 0);
    ObjectMotion_ArmCallback(8, 0x5000, 0);
    Engine_EventWait(50);
    ObjectMotion_ArmCallback(8, 0x9000, 0);
    ObjectMotion_ArmCallback(9, 0xd000, 0);
    Engine_EventWait(20);
    ObjectMotion_ArmCallback(5, 0x1000, 0);
    Engine_EventWait(30);
    Func_02001c24(5, 0);
    Engine_EventWait(10);
    ObjectMotion_ArmCallback(9, 0xb000, 0);
    Engine_EventWait(20);
    Motion_SetModeAndWaitAnimation(9, 4);
    Func_02001c24(9, 0);
    Engine_EventWait(10);
    Func_02001c34(5, 0x100, 30);
    ObjectMotion_ArmCallback(5, 0x3000, 0);
    Engine_EventWait(20);
    Func_02001c24(5, 0);
    Engine_EventWait(10);
    Motion_SetModeAndWaitAnimation(9, 4);
    Func_02001c24(9, 0);
    Motion_SetVarCbAndRefresh(5, 2);
    Engine_EventWait(20);
    Func_02001c24(5, 0);
    Engine_EventWait(10);
    ObjectMotion_ArmCallback(9, 0xf000, 0);
    Engine_EventWait(25);
    Func_02001c24(9, 0);
    Engine_EventWait(10);
    ObjectMotion_ArmCallback(8, 0x4000, 0);
    Engine_EventWait(20);
    Motion_SetModeAndWaitAnimation(8, 3);
    Engine_EventWait(10);
    Func_02001c24(0x2008, 0);
    Engine_EventWait(10);
    Motion_SetVarCbAndRefresh(9, 2);
    Engine_EventWait(10);
    Func_02001c24(9, 0);
    Engine_EventWait(10);
    Func_02001c34(5, 0x102, 40);
    Func_02001c24(5, 0);
    Engine_EventWait(10);
    ObjectMotion_ArmCallback(9, 0xa000, 0);
    Engine_EventWait(20);
    Motion_SetModeAndWaitAnimation(9, 3);
    Engine_EventWait(10);
    Func_02001c24(9, 0);
    Engine_EventWait(10);
    Object_LinkPair(5, 8, 50);
    ObjectMotion_ArmCallback(9, 0x4000, 0);
    Engine_EventWait(30);
    Func_02001c34(9, 0x108, 30);
    Func_02001c24(9, 0);
    Engine_EventWait(10);
    Motion_SetModeAndWaitAnimation(9, 6);
    Engine_EventWait(30);
    Func_02001c34(8, 0x101, 30);
    Engine_EventWait(10);
    ObjectMotion_ArmCallback(8, 0x5000, 0);
    Engine_EventWait(20);
    Func_02001c24(0x2008, 0);
    Engine_EventWait(10);
    Func_02001c34(9, 0x100, 30);
    ObjectMotion_ArmCallback(9, 0xf000, 0);
    Engine_EventWait(20);
    Func_02001c24(9, 0);
    Engine_EventWait(10);
    Func_02001c34(5, 0x101, 30);
    ObjectMotion_ArmCallback(5, 0x4000, 0);
    Engine_EventWait(20);
    Func_02001c24(5, 0);
    Engine_EventWait(10);
    Func_02001c34(9, 0x102, 40);
    ObjectMotion_ArmCallback(9, 0xb000, 0);
    Engine_EventWait(30);
    Func_02001c24(9, 0);
    Engine_EventWait(10);
    Motion_SetVarCbAndRefresh(9, 2);
    Engine_EventWait(10);
    Func_02001c24(9, 0);
    Engine_EventWait(10);
    ObjectMotion_ArmCallback(9, 0xf000, 0);
    Engine_EventWait(10);
    Func_02001c24(9, 0);
    Engine_EventWait(10);
    Motion_SetModeAndWaitAnimation(8, 3);
    Engine_EventWait(10);
    Func_02001c24(0x2008, 0);
    Engine_EventWait(10);
    ObjectMotion_ArmCallback(9, 0xb000, 0);
    Engine_EventWait(20);
    Func_02001c24(9, 0);
    Engine_EventWait(10);
    Motion_SetModeAndWaitAnimation(5, 3);
    Engine_EventWait(10);
    Func_02001c24(5, 0);
    Engine_EventWait(20);
    Motion_SetModeAndWaitAnimation(9, 4);
    Func_02001c24(9, 0);
    Engine_EventWait(10);
    Motion_SetVarCbAndRefresh(9, 2);
    Engine_EventWait(10);
    Func_02001c24(9, 0);
    Engine_EventWait(10);
    Motion_SetModeAndWaitAnimation(8, 3);
    Engine_EventWait(20);
    ObjectMotion_ArmCallback(8, 0x8000, 0);
    Engine_EventWait(20);
    Func_02001c24(0x2008, 0);
    Engine_EventWait(10);
    Motion_SetVarCbAndRefresh(5, 2);
    Engine_EventWait(10);
    ObjectMotion_ArmCallback(5, 0x1000, 0);
    Engine_EventWait(30);
    Func_02001c34(5, 0x105, 30);
    Func_02001c24(5, 0);
    Engine_EventWait(10);
    Motion_SetModeAndWaitAnimation(5, 3);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(9, 3);
    Motion_SetModeAndWaitAnimation(8, 3);
    Engine_EventWait(20);
    ObjectMotion_SetSpeedParameters(8, 0x13333, 0x9999);
    Engine_ActorSetAnimation(8, 2);
    {
        struct FieldActor *obj = Engine_ActorGet(5);

        if (obj) {
            ObjectMotion_ResetAndSetPosition(8, (s16)(obj->x >> 16), (s16)(obj->z >> 16));
        }
    }
    ObjectMotion_CommitCurrentPositionAndActivate(8);
    Func_02001be4(8, 0, 0);
    ObjectMotion_SetSpeedParameters(9, 0x13333, 0x9999);
    Engine_ActorSetAnimation(9, 2);
    {
        struct FieldActor *obj = Engine_ActorGet(5);

        if (obj) {
            ObjectMotion_ResetAndSetPosition(9, (s16)(obj->x >> 16), (s16)(obj->z >> 16));
        }
    }
    ObjectMotion_CommitCurrentPositionAndActivate(9);
    Func_02001be4(9, 0, 0);
    Engine_EventWait(10);
    ObjectMotion_CommitPositionAndActivate(5, 16, 0);
    ObjectMotion_SetPositionAndReset(5, 0x2a8, 0x3b0);
    Engine_EventWait(15);
    ObjectMotion_ArmCallback(5, 0xd000, 0);
    Engine_EventWait(50);
    ObjectMotion_ArmCallback(5, 0, 0);
    Engine_EventWait(15);
    ObjectMotion_SetPositionAndReset(5, 0x2d8, 0x3b0);
    ObjectMotion_SetPositionAndReset(5, 0x2d8, 0x3a0);
    Func_02001be4(5, (728 << 16), (912 << 16));
    Engine_AudioPlayCue(129);
    Func_02001ccc(5, -1);
    Engine_EventWait(40);
    Engine_MapCopyCellsTo(53, 50, 42, 49, 1, 1);
    Engine_MapCopyCellsTo(55, 117, 41, 117, 3, 5);
    GameFlag_ClearBit(0x201);
    Func_02001be4(10, (664 << 16), (792 << 16));
    Motion_CamBounds(-1, -1, -1, 0);
    gPartyState.current_owner = 5;
    Party_RemoveActiveOwner(4);
    Engine_EventWait(50);
    work = Ram_HeapSlots->event_work;
    work->delay = 60;
    work->screen_effect = 0x100;
    Event_ClearStatus1c6();
    Event_WaitValue1c8Frames();
    Func_02001c54(22);
}
