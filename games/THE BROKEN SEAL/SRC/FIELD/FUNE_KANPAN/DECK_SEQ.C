/* EXACT: 856 bytes, candidate 856, 0 differing halfwords, 0 halfword
 * edits (2026-09-27). FieldScene_RunActorSequence in
 * FIELD/FUNE_KANPAN/DECK_SEQ.C is a single-overlay unit binding its names at
 * their runtime addresses (an import veneer's listing offset plus 0x8000).
 * Complete extent 020022c0..02002618: first zero/pool 23b8..23d4,
 * second zero/pool 252c..2554, return 2606 and final pool 2608..2614.
 * The 24fc call is ActorSetDestination (runtime veneer 0200c344), not
 * ActorWalkToAndWait (0200c35c); retained this independently verified fix.
 * Three structural trials: sharing the early actor-result scalar with the
 * loop counter produced 860 bytes / 361 differing halfwords / 135 edits,
 * adding unwanted saved-register copies to the initial facing stores.
 * One shared halfword-zero record across both phases gave 856 / 85 / 57:
 * second zero uses saved r6 and the store order matches, but actor stays r5,
 * first zero also moves to r6, and the second pool remains four bytes early.
 * Phase-scoped actor pointers gave 848 / 211 / 94, removing the reference's
 * saved-pointer copies for actors 30 and 0; the single shared actor survives.
 * Retained the original lifetimes plus the destination-call correction.
 * Previous remaining: actor/counter r5/r6 versus r6/r5, second zero in r2 versus r5,
 * second pool four bytes early, and callback address hoisted before its
 * speed call. Those lifetime-only trials are closed.
 * New interface model: exact FIELD_EVENT.H FieldActor accesses, canonical
 * void Engine_ActorEnableActionCallback and named FuneKanpan_SailorActions, supported by
 * OBJECT/BY_ID.C and exact FUNE_KANPAN deck scenes. Keep all original local
 * lifetimes and calls; this reduces 83 halfwords/55 edits to 3/2. Complete
 * 856-byte owner, saved registers and all three pools now agree. Only the
 * loop increment at 247c is early: candidate adds r5 before the scale_y
 * store and movs r0,#1; reference adds after both, immediately before wait.
 * Follow-up: compiler dumps locate that independent increment before
 * NOTE_INSN_LOOP_CONT. Make it the natural for-loop continuation after
 * the wait instead of a pre-wait body statement. The scheduler then emits
 * the reference store/movs/increment/call order: all 856 bytes and pools
 * exact. No counter type, declaration-order or fixed-register changes. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
extern u8 FuneKanpan_CrewScript[];

extern u8 FuneKanpan_DeckActionsA[];
extern u8 FuneKanpan_DeckActionsB[];
extern u8 FuneKanpan_DeckActionsC[];
extern u8 FuneKanpan_SailorActions[];
void FieldScene_RunScene3af_02000bb8();
void FieldScene_CallPairWith10();
void ObjectDispatch_SetSingleChildField26Far(struct FieldActor *object, s32 value);
void Event_CallWithLastActiveObjectId();
void Engine_ObjectMotionSetPositionAndCommit();
void Graphics_EnableObjLayerAndCallbacks();
void ObjectDispatch_StopCallbacksAndHideLayers();
void UiText_ShowCenteredMessage();
void Object_SetActionCallbackAndRefreshById();
void Ui_SetRenderResultFromObject();
void Event_ClearStatus1c6Far();
void Event_WaitValue1c8FramesFar();

/* FAKEMATCH: Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. The legacy
 * Value2 callback calls retain that return-shape adapter around the actual
 * void service; their unused nominal results are not game values. */

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

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

void FieldScene_RunActorSequence(void)
{
    u32 i;
    struct FieldActor *rec8;
    struct FieldActor *record;
    s32 base5_200c8c4;
    s32 base5_200c8b0;
    s32 base5_200c8d8;
    s32 base5_0;
    const u8 *base5_200c888;

    Engine_EventBegin();
    Engine_ActorSetChildValue(0, 15);
    record = Engine_ActorGet(0);
    ObjectDispatch_SetSingleChildField26Far(record, 0);
    Call1(Event_CallWithLastActiveObjectId, (u32)FuneKanpan_CrewScript);
    Engine_TaskWait(1);
    Call1(Event_CallWithLastActiveObjectId, 0x200d340);
    Engine_TaskWait(1);
    Call3(Engine_ActorSetPosition, 22, 0xb00000, 0x2b80000);
    record = Engine_ActorGet(22);
    {
        s32 shown = 0xd000;

        record->facing = shown;
    }
    Call3(Engine_ActorSetPosition, 21, 0x1080000, 0x2960000);
    record = Engine_ActorGet(21);
    {
        s32 shown = 0xb000;

        record->facing = shown;
    }
    Call3(Engine_ActorSetPosition, 24, 0xb80000, 0x2a00000);
    Call3(Engine_ActorSetPosition, 25, 0xca0000, 0x2b40000);
    Call3(Engine_ActorSetPosition, 26, 0xfc0000, 0x2860000);
    Call3(Engine_ActorSetPosition, 27, 0x1000000, 0x2ae0000);
    Call3(Engine_ActorSetPosition, 28, 0xac0000, 0x2780000);
    Call3(Engine_ActorSetPosition, 29, 0x1000000, 0x26e0000);
    {
        /* FAKEMATCH: halfword zero retains the short literal-pool reach. */
        struct { u16 v; } zero;

        zero.v = 0;
        Engine_ActorGet(24)->rise_enabled = zero.v;
        Engine_ActorGet(25)->rise_enabled = 1;
        Engine_ActorGet(26)->rise_enabled = zero.v;
        Engine_ActorGet(27)->rise_enabled = 2;
    }
    Call3(Engine_ActorSetPosition, 20, 0, 0);
    Engine_ActorEnableActionCallback(24, FuneKanpan_DeckActionsA);
    Value2(Engine_ActorEnableActionCallback, 25, (s32)FuneKanpan_DeckActionsA);
    Engine_ActorEnableActionCallback(26, FuneKanpan_DeckActionsB);
    Value2(Engine_ActorEnableActionCallback, 27, (s32)FuneKanpan_DeckActionsB);
    Engine_ActorEnableActionCallback(28, FuneKanpan_DeckActionsC);
    Value2(Engine_ActorEnableActionCallback, 29, (s32)FuneKanpan_DeckActionsC);
    Engine_ActorSetChildValue(24, 3);
    Engine_ActorSetChildValue(25, 3);
    Engine_ActorSetChildValue(26, 3);
    Engine_ActorSetChildValue(27, 3);
    Engine_ActorSetChildValue(28, 3);
    Engine_ActorSetChildValue(29, 3);
    *(s32 *)((*(s32 *)&gEventWork + 0x1c0)) = 0x202;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(80);
    Engine_AudioPlayCue(147);
    rec8 = Engine_ActorGet(31);
    rec8->scale_x = 0x1999;
    rec8->scale_y = 0x1999;
    rec8->x.fixed = 0xc20000;
    rec8->z.fixed = 0x2820000;
    for (base5_0 = 0; (u32)base5_0 < 16; base5_0++) {
        rec8->scale_x += 0xf5c;
        rec8->scale_y += 0xf5c;
        Engine_TaskWait(1);
    }
    rec8 = Engine_ActorGet(30);
    rec8->scale_x = 0x11999;
    rec8->scale_y = 0x11999;
    rec8->x.fixed = 0xc20000;
    rec8->y.fixed = 0x500000;
    rec8->z.fixed = 0x2820000;
    {
        s32 shown = 0x5000;

        rec8->facing = shown;
    }
    *(s32 *)((s32)rec8 + 68) = 0x6666;
    *(s32 *)((s32)rec8 + 72) = 0x20000;
    Engine_EventWait(80);
    Engine_AudioPlayCue(147);
    Engine_ActorSetPosition(31, 0, 0);
    record = Engine_ActorGet(30);
    Engine_ActorSetSpriteFlags(record, 1);
    Call3(Engine_ActorSetSpeed, 0, 0x19999, 0xcccc);
    rec8 = Engine_ActorGet(0);
    {
        /* FAKEMATCH: halfword zero retains the short literal-pool reach. */
        struct { u16 v; } zero;

        zero.v = 0;
        rec8->motion_flags = zero.v;
    }
    Call3(Engine_ActorSetDestination, 0, 216, 0x264);
    Call3(Engine_ActorSetSpeed, 30, 0x19999, 0xcccc);
    Call3(Engine_ObjectMotionSetPositionAndCommit, 30, 196, 0x258);
    Call3(Engine_ObjectMotionSetPositionAndCommit, 30, 216, 0x258);
    Engine_ActorStop(28);
    Engine_TaskWait(1);
    Call3(Engine_ActorSetSpeed, 28, 0x19999, 0xcccc);
    base5_200c888 = FuneKanpan_SailorActions;
    Engine_ActorEnableActionCallback(28, base5_200c888);
    Call2(FieldScene_CallPairWith10, 30, 0xd000);
    FieldScene_RunScene3af_02000bb8();
    Engine_EventWait(10);
    Engine_ActorEnableActionCallback(30, base5_200c888);
    Engine_ActorStop(29);
    Engine_TaskWait(1);
    Call3(Engine_ActorSetSpeed, 29, 0x19999, 0xcccc);
    Object_SetActionCallbackAndRefreshById(29, base5_200c888);
    Engine_EventWait(20);
    Event_ClearStatus1c6Far();
    Event_WaitValue1c8FramesFar();
    Engine_ActorStop(24);
    Engine_ActorStop(25);
    Engine_ActorStop(26);
    Engine_ActorStop(27);
    Engine_ActorStop(28);
    Engine_ActorStop(29);
    Engine_EventWait(10);
    Graphics_EnableObjLayerAndCallbacks();
    Ui_SetRenderResultFromObject(21);
    Call3(UiText_ShowCenteredMessage, 0x1e45, 1, 0);
    ObjectDispatch_StopCallbacksAndHideLayers();
    Engine_EventRequestExit(14);
}
