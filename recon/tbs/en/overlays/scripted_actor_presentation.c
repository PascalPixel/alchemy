/* NONMATCHING: 2492/2508 bytes, 1154 differing halfwords, 486 aligned edits.
 * H2 is preserved at d73d75772; canonical plain-zero baseline restored.
 * 2026-09-27 Sol Venus Summit H2: a Value_00000000 producer prevents CSE
 * from forwarding the coordinate's low zero bits. It restores three pool
 * groups and a real load, but that SI load follows the motion store instead
 * of preceding the z store; first pool is +4, middle and final pools -4.
 * Complete normalized diff and allocator output reviewed. Initial zero is
 * still r6 rather than r8, actor base r5 rather than r6, motion pointer r8
 * rather than r5. Prediction failed; no credit. Preserve this counterexample
 * before restoring the plain-zero baseline. No second symbol-width sweep.
 * 2026-09-27 Sol Venus Summit H1: reuse the initial actor local for the
 * lead actor, transferring Haidia's typed lookup lifetime model. Prediction:
 * lead base r6 and motion-field pointer r5, with the initial zero retained
 * separately from the later camera zero. Complete normalized diff is
 * byte-identical to baseline: lead r5, motion pointer sl, missing first
 * zero pool. Both lookups were already typed; variable reuse supplies no
 * new alias information. Close this axis; keep the simpler shared local.
 * 2026-09-26 corrected model: equal control-flow topology, no stack frame;
 * first zero literal pool missing, early actor/flag ownership differs.
 * H1: u16 zero instead of s32 gives byte-identical output: no improvement.
 * H2: exact ACTOR_TRANSITION.C Half aggregate also gives identical output.
 * Both bounded variants closed; no adoption and no new DONE bytes.
 * Reopen only with evidence that prevents the zero from being represented
 * by the low byte of the preceding 0x940000 coordinate, not declaration
 * permutations. The missing pool starts at reference offset 0x1b8.
 * Own-ROM extent 020008b4..02001280, pools at 0a6c, 0e98 and 124c.
 * All 255 call sites agree in order and runtime target with the registered
 * draft. The alternate resource_3c9_c_020008b4.c recovery has wrong service
 * names at repeated encoded BL labels; it is not the active model.
 * Exact neighbour Scene_RunActorEntrySequence rechecks at 3604/3604 bytes.
 * Reuse its maintained event interfaces and call-scope helpers, FieldActor
 * coordinate/motion/facing fields, and the three audited local callees.
 * Old registered draft failed to compile: ObjectRuntime has no unknown_00.
 */
#include "TYPES.H"
#include "VINASU.H"
#include "FIELD_EVENT.H"

/* Preserve the original measured FIELD_EVENT adapter context of this draft. */
static inline s32 Event_ChooseYesNo(s32 actor, s32 flags)
{
    return Engine_EventChooseYesNo(actor, flags);
}

extern u8 MsgVinasuLongLastTime[];

void Scene_SetPresentationActorState(s32 actor, s32 active);
void Main_0808a0b0(s32 actor, const u8 *actions);
extern const u8 Data_0200dfc4[];

/* FAKEMATCH trial: the exact actor-transition neighbour's one-halfword
 * aggregate did not preserve HImode here; retain this completed attempt. */
struct Half {
    u16 v;
};

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

void Scene_RunScriptedActorPresentation(void)
{
    struct Half zero;
    struct FieldActor *actor;
    struct FieldActor *record;
    s32 initial_flags;
    s32 active_mask;
    s32 selector_4013;
    s32 selector_2014;
    s32 heading_scale;
    s32 selector_8015;
    s32 selector_a014;
    s32 selector_8001;
    const u8 *action_script;

    actor = Actor_Get(18);
    Engine_EventBegin();
    Scene_SetPresentationActorState(1, 0);
    Scene_SetPresentationActorState(2, 0);
    Scene_SetPresentationActorState(3, 0);
    Call4(Engine_CameraMoveTo, -1, -1, -1, 0);
    Engine_TaskWait(1);
    initial_flags = 0;
    actor->motion_flags = initial_flags;
    {
        s32 value = 2;
        value |= actor->priority_flags;
        actor->priority_flags = value;
    }
    record = Object_GetById(18);
    Engine_ActorSetSpriteFlags(record, 0);
    Engine_ActorSetSpritePriority(18, 1);
    Call3(Engine_ActorSetPosition, 18, 0x2440000, 0x1520000);
    actor = Object_GetById(0);
    actor->motion_flags = initial_flags;
    Engine_ActorSetSpritePriority(0, 1);
    Call3(Engine_ActorSetPosition, 0, 0x2450000, 0x1200000);
    Engine_TaskWait(1);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(20);
    Call3(Engine_ActorSetSpeed, 18, 0x10000, 0x8000);
    Call3(Engine_ActorSetSpeed, 0, 0x10000, 0x8000);
    Object_GetById(0)->unknown_5a &= 254;
    Call3(Engine_ActorSetDestination, 18, 0x244, 221);
    Call3(Engine_ActorMoveToAndWait, 0, 0x245, 171);
    Call3(Engine_ActorSetDestination, 18, 0x212, 211);
    Call3(Engine_ActorMoveToAndWait, 0, 0x213, 161);
    Call3(Engine_ActorSetDestination, 18, 0x208, 191);
    Call3(Engine_ActorMoveToAndWait, 0, 0x209, 141);
    record = Object_GetById(18);
    Engine_ActorSetSpriteFlags(record, 1);
    Call3(Engine_ActorSetDestination, 18, 0x203, 171);
    Call3(Engine_ActorMoveToAndWait, 0, 0x204, 121);
    Call1(Engine_AudioPlayCue, 0x120);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(0, 6);
    Engine_ActorStop(0);
    actor->x.fixed = 0x2040000;
    actor->y.fixed = 0x80000;
    actor->z.fixed = 0x940000;
    {
        s32 shown = 0x8000;

        actor->facing = shown;
    }
    actor->motion_flags = 3;
    zero.v = 0;
    Engine_AudioPlayCue(152);
    actor->velocity_y = 0x40000;
    Engine_AudioPlayCue(152);
    record = Object_GetById(0);
    Engine_ActorSetSpriteFlags(record, 1);
    Call3(Engine_ActorMoveToAndWait, 0, 0x1f8, 148);
    active_mask = 1;
    Engine_EventWait(10);
    Object_GetById(0)->unknown_5a |= active_mask;
    actor->y.fixed = -0x200000;
    {
        s32 shown = 0x4000;

        actor->facing = shown;
    }
    Engine_EventWait(20);
    Call1(Engine_AudioPlayCue, 0x134);
    Call3(Engine_ActorMoveToAndWait, 18, 0x20c, 191);
    record = Object_GetById(18);
    Engine_ActorSetSpriteFlags(record, 0);
    Call3(Engine_ActorMoveToAndWait, 18, 0x212, 211);
    Call3(Engine_ActorMoveToAndWait, 18, 0x244, 221);
    Call3(Engine_ActorSetDestination, 18, 0x244, 0x152);
    {
        struct FieldActor *record = Object_GetById(0);
        u8 value = *(u8 *)&record->priority_flags;

        record->priority_flags = (u8)(value | active_mask);
    }
    Call3(Engine_ActorSetSpeed, 0, 0xcccc, 0x6666);
    Scene_SetPresentationActorState(1, 1);
    Scene_SetPresentationActorState(2, 1);
    Scene_SetPresentationActorState(3, 1);
    Call3(Engine_ActorWalkTo, 0, 0x1ec, 164);
    Call3(Engine_ActorWalkTo, 1, 0x202, 164);
    Call3(Engine_ActorWalkTo, 2, 0x1ec, 140);
    Call3(Engine_ActorWalkToAndWait, 3, 0x202, 140);
    Engine_ActorSetAnimation(0, 1);
    Engine_ActorSetAnimation(1, 1);
    Engine_ActorSetAnimation(2, 1);
    Engine_ActorFaceDirection(0, 0x4000, 0);
    Engine_ActorFaceDirection(1, 0x4000, 0);
    Engine_ActorFaceDirection(2, 0x4000, 0);
    Engine_ActorFaceDirection(3, 0x4000, 0);
    Engine_ActorWaitForMove(18);
    Engine_ActorSetPosition(18, 0, 0);
    selector_4013 = 0x4013;
    Call1(Engine_AudioPlayCue, 0x121);
    Call1(Engine_EventSetMessage, (s32)MsgVinasuLongLastTime);
    VinasuChojo_ShowMessage(selector_4013);
    Engine_ActorFaceDirection(0, 0x8000, 0);
    Engine_ActorFaceDirection(1, 0x8000, 0);
    Engine_ActorFaceDirection(2, 0x8000, 0);
    VinasuChojo_FaceActor( 3, 0x8000);
    Engine_EventGetViewCenter()->motion_flags = zero.v;
    Call2(Engine_CameraSetSpeed, 0x4cccc, 0x9999);
    Call4(Engine_CameraMoveTo, 0x1300000, 0x200000, 0x9e0000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(20);
    selector_2014 = 0x2014;
    VinasuChojo_FaceActor(20, 0xd000);
    Engine_ActorRunRepeatedMotion(20, 1);
    Engine_AudioPlayCue(61);
    VinasuChojo_ShowMessage(selector_2014);
    Engine_ActorSetAnimation(19, 4);
    VinasuChojo_ShowMessage(selector_4013);
    VinasuChojo_FaceActor(20, 0xb000);
    Call3(Engine_ActorShowEmote, 20, 0x105, 40);
    VinasuChojo_ShowMessage(selector_2014);
    VinasuChojo_ShowMessage(21);
    Call3(Engine_ActorShowEmote, 19, 0x100, 0);
    Call3(Engine_ActorShowEmote, 20, 0x100, 20);
    Call3(Engine_ActorFaceDirection, 6, 0x3000, 0);
    Call3(Engine_ActorFaceDirection, 19, 0x5000, 0);
    Call3(Engine_ActorFaceDirection, 20, 0x5000, 20);
    Call2(Engine_CameraSetSpeed, 0x19999, 0x3333);
    Call4(Engine_CameraMoveTo, 0x1260000, -1, 0xc20000, 1);
    Engine_EventWait(20);
    Call3(Engine_ActorSetSpeed, 21, 0xcccc, 0x6666);
    Call3(Engine_ActorWalkToAndWait, 21, 0x110, 200);
    Engine_ActorRunRepeatedMotion(20, 1);
    Engine_EventWait(20);
    VinasuChojo_ShowMessage(selector_2014);
    Call3(Engine_ActorShowEmote, 19, 0x103, 20);
    VinasuChojo_ShowMessage(19);
    Engine_ActorSetAnimationAndWait(21, 3);
    VinasuChojo_ShowMessage(21);
    Call3(Engine_ActorShowEmote, 20, 0x101, 40);
    VinasuChojo_ShowMessage(selector_2014);
    Engine_ActorSetAnimationAndWait(21, 4);
    VinasuChojo_ShowMessage(21);
    Call3(Engine_ActorShowEmote, 19, 0x101, 60);
    Engine_EventShowMessageAndWait(19, 0, 40);
    Call3(Engine_ActorShowEmote, 19, 0x106, 40);
    VinasuChojo_FaceActor(19, 0x8000);
    VinasuChojo_ShowMessage(0x2013);
    Engine_ActorFaceDirection(6, 0, 0);
    Call3(Engine_ActorShowEmote, 21, 0x103, 40);
    VinasuChojo_ShowMessage(21);
    Call3(Engine_ActorFaceDirection, 19, 0x3000, 0);
    Engine_ActorFaceDirection(20, 0xb000, 40);
    Engine_ActorRunRepeatedMotion(21, 1);
    heading_scale = 160;
    VinasuChojo_ShowMessage(21);
    Call3(Engine_ActorFaceDirection, 20, 0x5000, 0);
    VinasuChojo_FaceActor( 19, (heading_scale << 7));
    Call3(Engine_ActorShowEmote, 19, 0x108, 20);
    VinasuChojo_ShowMessage(0x2013);
    Call3(Engine_ActorShowEmote, 21, 0x103, 20);
    Engine_EventShowMessageAndWait(21, 0, 20);
    Engine_ActorFaceDirection(20, 0x8000, 40);
    Engine_ActorSetAnimationAndWait(20, 4);
    Engine_EventShowMessageAndWait(selector_2014, 0, 40);
    Engine_ActorSetAnimationAndWait(21, 3);
    Engine_EventShowMessageAndWait(21, 0, 20);
    Engine_ActorFaceDirection( 20, (heading_scale << 7), 20);
    Engine_ActorStartRepeatedMotion(21, 2);
    VinasuChojo_ShowMessage(21);
    Call3(Engine_ActorShowEmote, 20, 0x105, 0);
    Call3(Engine_ActorShowEmote, 19, 0x105, 80);
    Engine_ActorStartRepeatedMotion(21, 2);
    VinasuChojo_ShowMessage(21);
    Call3(Engine_ActorShowEmote, 19, 0x101, 60);
    VinasuChojo_ShowMessage(0x2013);
    Engine_ActorSetAnimationAndWait(21, 3);
    VinasuChojo_ShowMessage(21);
    Engine_ActorStartRepeatedMotion(19, 1);
    Engine_ActorRunRepeatedMotion(20, 1);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(21, 4);
    Engine_EventShowMessageAndWait(21, 0, 20);
    Call3(Engine_ActorShowEmote, 20, 0x105, 60);
    Engine_EventShowMessageAndWait(selector_2014, 0, 20);
    VinasuChojo_FaceActor(21, 0xb000);
    VinasuChojo_ShowMessage(21);
    VinasuChojo_FaceActor( 6, 0x3000);
    Engine_ActorRunRepeatedMotion(6, 2);
    Engine_EventWait(20);
    VinasuChojo_FaceActor(21, 0xd000);
    Engine_ActorSetAnimationAndWait(19, 4);
    VinasuChojo_ShowMessage(0x2013);
    Engine_ActorFaceDirection(6, 0, 0);
    Engine_ActorRunRepeatedMotion(21, 1);
    VinasuChojo_ShowMessage(21);
    Engine_ActorSetAnimationAndWait(20, 3);
    VinasuChojo_ShowMessage(selector_2014);
    Engine_ActorSetAnimationAndWait(21, 4);
    VinasuChojo_ShowMessage(21);
    Call3(Engine_ActorShowEmote, 20, 0x101, 0);
    Call3(Engine_ActorShowEmote, 19, 0x101, 80);
    Engine_ActorFaceDirection(19, 0x8000, 0);
    Engine_ActorFaceDirection(20, 0x8000, 0);
    Call2(Engine_CameraSetSpeed, 0x6666, 0xccc);
    Call4(Engine_CameraMoveTo, 0x1260000, -1, 0xb40000, 1);
    Call3(Engine_ActorWalkToAndWait, 21, 0x106, 176);
    selector_8015 = 0x8015;
    Engine_ActorFaceDirection(21, 0x8000, 40);
    Engine_ActorFaceDirection(21, 0, 20);
    Engine_ActorStartRepeatedMotion(21, 2);
    VinasuChojo_ShowMessage(selector_8015);
    Call3(Engine_ActorShowEmote, 19, 0x100, 20);
    VinasuChojo_ShowMessage(0x2013);
    Engine_ActorStartRepeatedMotion(21, 2);
    VinasuChojo_ShowMessage(selector_8015);
    Call3(Engine_ActorShowEmote, 20, 0x103, 40);
    Call3(Engine_EventShowMessageAndWait, 0xa014, 0, 20);
    Call3(Engine_ActorShowEmote, 21, 0x105, 20);
    VinasuChojo_ShowMessage(selector_8015);
    Call3(Engine_ActorShowEmote, 19, 0x103, 20);
    VinasuChojo_ShowMessage(0x2013);
    Call3(Engine_ActorShowEmote, 21, 0x101, 40);
    selector_a014 = 0xa014;
    VinasuChojo_ShowMessage(selector_8015);
    Engine_ActorSetAnimationAndWait(20, 4);
    VinasuChojo_ShowMessage(selector_a014);
    Engine_ActorSetAnimationAndWait(19, 3);
    VinasuChojo_ShowMessage(0x2013);
    Call3(Engine_ActorShowEmote, 21, 0x103, 60);
    Engine_ActorFaceDirection(21, 0x8000, 20);
    Call3(Engine_EventShowMessageAndWait, 0xa015, 0, 40);
    Call3(Engine_ActorShowEmote, 6, 0x105, 120);
    Call3(Engine_ActorShowEmote, 20, 0x105, 60);
    VinasuChojo_ShowMessage(selector_a014);
    Engine_ActorFaceDirection(21, 0, 40);
    Engine_ActorSetAnimation(19, 3);
    VinasuChojo_ShowMessage(0x2013);
    Engine_ActorRunRepeatedMotion(21, 1);
    Engine_EventShowMessageAndWait(selector_8015, 0, 20);
    VinasuChojo_ShowMessage(selector_a014);
    Call3(Engine_ActorShowEmote, 21, 0x100, 40);
    Engine_ActorSetAnimation(19, 4);
    VinasuChojo_ShowMessage(0x2013);
    Call3(Engine_ActorShowEmote, 6, 0x105, 40);
    Call3(Engine_ActorShowEmote, 20, 0x108, 40);
    VinasuChojo_ShowMessage(selector_a014);
    Call3(Engine_ActorShowEmote, 19, 0x103, 20);
    VinasuChojo_ShowMessage(0x2013);
    Engine_ActorRunRepeatedMotion(21, 1);
    Engine_EventWait(20);
    Engine_ActorStartRepeatedMotion(20, 2);
    VinasuChojo_ShowMessage(selector_a014);
    Engine_ActorSetAnimation(19, 4);
    VinasuChojo_ShowMessage(0x2013);
    gEventWork->start_transition = 0x202;
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Call4(Engine_CameraMoveTo, 0x1f80000, -0x180000, 0xa80000, 0);
    Engine_TaskWait(1);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    selector_8001 = 0x8001;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(1, 1);
    VinasuChojo_ShowMessage(selector_8001);
    Call3(Engine_ActorShowEmote, 3, 0x101, 40);
    VinasuChojo_ShowMessage(3);
    Engine_ActorSetAnimationAndWait(2, 3);
    Call3(Engine_EventShowMessageAndWait, 0x1002, 0, 40);
    Engine_ActorJump(1, 2, 20);
    VinasuChojo_ShowMessage(selector_8001);
    Event_OpenMessage(selector_8001, 0);
    Engine_ActorFaceDirection(0, 0, 0);
    Engine_ActorFaceDirection(2, 0x4000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0x2000, 0);
    if (Event_ChooseYesNo(0, 0) == 1) {
        gEventWork->message += 1;
    }
    (Engine_EventWait)(20);
    VinasuChojo_ShowMessage(1);
    Engine_ActorFaceDirection(2, 0, 0);
    Engine_ActorFaceDirection( 3, 0x8000, 20);
    Engine_ActorSetAnimation(0, 3);
    Engine_ActorSetAnimation(1, 3);
    Engine_ActorSetAnimation(2, 3);
    Engine_ActorSetAnimationAndWait(3, 3);
    action_script = Data_0200dfc4;
    Engine_ActorEnableActionCallback(1, action_script);
    Engine_ActorEnableActionCallback( 2, action_script);
    Main_0808a0b0(3, action_script);
    Engine_EventWait(20);
    Engine_EventEnd();
}
