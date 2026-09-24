#include "TYPES.H"
#include "FIELD_EVENT.H"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
#define NewEffectObject           Func_02000048
#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
#define Scene_GetRecord_2(a0) Value1(Engine_ActorGet, a0)
#define Scene_GetRecord_3(a0) Value1(Engine_ActorGet, a0)
#define Scene_GetRecord_4(a0) Value1(Engine_ActorGet, a0)
#define Object_SetModeById_1(a0, a1) Value2(Engine_ActorSetAnimation, a0, a1)
#define ObjectMotion_CallThenWaitForAnimationChange_9(args...) ((void (*)())Engine_ActorSetAnimationAndWait)(args)
#define ObjectMotion_EnableActionAndSetCallback_1(a0, a1) Value2(Engine_ActorEnableActionCallback, a0, a1)
#define ACTOR_FIELD_108(rec) (*(s32 *)((rec) + 108))
#define ClearSceneExitGateAtEntry Func_02004f0a
#define GetSceneExitPendingWork Func_02004f1e
#define ResetSceneExitPendingWork Func_02004f2a
#define SetSceneExitCompletionMode Func_02004f34
#define ClearSceneExitField40 Func_02004f40
#define SetSceneExitGate Func_02004f48
#define ClearSceneExitGateBeforeDescriptor Func_02004fae
#define RunFacingVariantA       Scene_RunPrimarySequence
#define RunFacingVariantB       FieldScene_RunForwardArcBurst
#define RunFacingVariantC       FieldScene_RunDescentBurst
#define RunFacingVariantD       Func_02002062
#define RunSceneVariant         Func_020023c8
#define Scene_GetRecord_4_02002778(args...) Func_02007e22(args)

struct EffectRec {
    u8 pad[9];
    u8 f0 : 2;
    u8 f1 : 2;
};

struct EffectWork {
    u8 pad[0x50];
    struct EffectRec *rec;
};

struct SceneRecordHeading {
    u8 pad[6];
    u16 heading;
};

struct SceneActor_02001334 {
    u8 unknown_00[6];
    u16 facing;
};

struct Params {
    s32 unused0;
    s32 field1;
    s32 field2;
    s32 field3;
    u8 pad[24];
};

struct Descriptor_02000484 {
    u32 field0;
    u8 unused[12];
    u32 field16;
    u32 field20;
    u16 field24;
    u16 unused26;
    u32 field28;
    u8 unused32[8];
};

struct Descriptor_020041ec {
    u32 field0;
    u32 field4;
    u8 unused8[16];
    u16 field24;
    u8 unused26[6];
    u8 unused32[8];
};

extern u8 Value_0000003c;
extern u8 Data_0200c7a8[];
extern u8 Data_0200c838[];
extern u8 Data_0200c8c8[];
extern u8 Data_0200cb90[];
extern u8 Data_0200d184[];
extern u8 Data_0200cd40[];
extern s32 Data_03001e40;
extern u8 Data_000017e0[];

s16 Func_020046a2(s32, s32);
void Func_02006a34();
void Func_02006a92();
void Func_0200a5b9();
u8 *Func_02004f0a(s32 slot);
u8 *Func_02004f1e(s32 slot);
u8 *Func_02004f2a(s32 slot);
u8 *Func_02004f34(s32 slot);
u8 *Func_02004f40(s32 slot);
u8 *Func_02004f48(s32 slot);
u8 *Func_02004fae(s32 slot);
void Func_02000dae();
void Func_02000dc6();
void Func_02000de4();
void Func_02000f4c(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, u32 flags, u8 *extra);
void Func_02000f62(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, u32 flags, u8 *extra);
void Func_02000f86(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, u32 flags, u8 *extra);
void Func_02000fa2(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, u32 flags, u8 *extra);
u8 *Func_020056ee(s32 slot);
u8 *Func_020056fe(s32 slot);
void Func_02001fe2(void);
void Func_0200247c(void);
void Func_02002350(s32 arg0);
void Func_02002062(void);
void Func_020023c8(s32 variant);
void Func_020020d8(void);
void Func_0200246a();
void Func_020025a4(void);
void Func_02002486();
void Func_02004d00(s32 arg0, s32 arg1);
void Func_02004d12(s32 arg0, s32 arg1);
void Func_020042d6(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, u32 flags, u8 *extra);
u8 *Func_02008606(s32 id);
u8 *Func_0200863a(s32 id);
u8 *Func_020086fa(s32 id);
u8 *Func_02008704(s32 id);
void Func_020043fe(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, u32 flags, u8 *extra);
s32 Func_02006b96();
s32 Func_02006c4e();
u8 *Func_02007e22();
void Func_02007e60();
s32 Func_02004b56();
s32 Func_02004bd4();

/*
 * resource_39e owner at 0x02001494, 2236 bytes.
 *
 * Second ensemble beat of this overlay's scene script: 233 calls into the
 * shared scene-script helper block, then a two-stage story branch.
 *
 *   - the opening call sets story flag 0x89a;
 *   - stage 1 (Engine_EventChooseYesNo(0, 0) == 0) joins with id 16 and sets flag 0x898
 *     -- the flag the dispatcher at 0x020012e0 tests and 0x02001dbc clears;
 *   - stage 1 otherwise bumps the step counter at +472 of the scene work
 *     record and runs stage 2: its ==0 arm joins with id 18 and also sets
 *     0x898, its else arm bumps that counter again and sets flag 0x899.
 *
 * Both join arms share the tail at 0x02001cb4: Engine_EventShowMessageAndWait(id, 0, 20) with
 * the id carried in r0, then Engine_GameFlagSet(0x898).
 *
 * Midway the beat clears then sets bit 0 of byte +90 of record 16 around a
 * Engine_ActorWalkToAndWait reposition, plays sounds 158/159 with Engine_MapAnimateCells text
 * calls, runs the slot-19/20 sequence (Engine_ActorSetPosition(19/20, 232 << 16,
 * 168 << 16), record +12 = 0xc0000, +60 = 0x80000000, +24 = 0xcccc, +30 of
 * the +80 sub-object = 0x8000, sound 124) that also closes the owners at
 * 0x02001160 and 0x02001dbc, and ends with Engine_ActorEnableActionCallback(12, 0x0200c638) as
 * 0x02001dbc does.
 *
 * Complete owner: `push {r5, lr}` at 0x02001494 through `pop {r5} / pop {r0} /
 * bx r0` at 0x02001d42-0x02001d46, two inline pools plus two trailing pool
 * words ending 0x02001d4f; the next owner (0x02001d50, `push {lr}`) follows.
 * Called once from within this overlay.
 *
 * Call binding. This overlay reaches the shared helpers through its own veneer
 * bank, so every emitted call word names a veneer, not the main-image address.
 * The legacy alias a reference site produces is derived from that site's own
 * position, so one alias can name two different veneers and several aliases
 * can name one veneer. Each helper is therefore declared once, under an alias
 * whose reference sites all resolve to that helper's veneer, and is spelled in
 * the body under the main-image function it reaches. The bound veneer -- and
 * so the emitted call word -- depends only on the alias, never on where the
 * call lands in the candidate.
 *
 * Two call forms in the branch tail carry meaning beyond their arguments. The
 * three sites spelled through Value2 keep the callee's result register live
 * across the argument setup, which orders the r0 write after r1; the sites
 * spelled through Call2 discard it and order r0 first. Both forms appear in
 * the sibling beat at 0x02001dbc.
 *
 * The set of bit 0 in byte +90 reads the byte into its own local before the
 * or, as the staged-actor scene at resource_373:02003fb0 does at the same
 * offset; the paired clear needs no local because an and against 0xfe already
 * holds its mask in a word register.
 *
 * Uncertainty: helper roles and argument roles are read from call shape only;
 * ids, dialogue ids and constants are transcribed. 0x0200c77a, 0x0200c790 and
 * 0x0200c638 are overlay-image data pointers whose contents are unidentified.
 */

/* Veneer aliases this overlay binds; see Call binding above. */

/* The main-image helper each veneer above reaches. */
void FieldScene_RunForwardArcBurst(void);

void FieldScene_RunDescentBurst(void);

static __inline__ void SetScale(s32 actor, s32 horizontal, s32 vertical)
{
    Actor_SetSpeed(actor, horizontal, vertical);
}

static __inline__ void SetPosition(s32 actor, s32 horizontal, s32 vertical)
{
    Actor_WalkToAndWait(actor, horizontal, vertical);
}

/* Field cleared to 0 on each actor record below. */

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

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

/* Moves the next dialogue line on by amount messages. */
static __inline__ void bump_step(s32 amount)
{
    gEventWork->message += amount;
}

/* Newly named raw call sites: each engine function below was reached only
 * through an unresolved veneer island until its own source was read; the
 * per-site macro keeps the site's original calling form. */

static __inline__ void Call1_02001dbc(void (*f)(), s32 a0)
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

static __inline__ void Call3_0200268c(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call2_020026d8(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

/* Call sites spelled through these wrappers pass their constants straight into
 * the argument registers; a direct call precomputes a costly constant into a
 * pseudo that the compiler then shares with later uses in the block. A
 * value-returning site also writes r0 last of its arguments. The same wrappers
 * carry the sibling beat at 0x02001dbc. */
static __inline__ void Call1_02001494(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ u8 *Record1(u8 *(*f)(), s32 a0)
{
    return f(a0);
}

/* The scene step counter at +472 of the shared scene work record. */
static __inline__ void Scene_BumpStep(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 472) = (u16)(*(u16 *)(work + 472) + amount);
}

static __inline__ void Call3_02002778(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call1_02002778(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call3_020027ec(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call3_02002ad0(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call1_0200071c(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1_0200071c(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2_0200071c(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2_0200071c(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3_0200071c(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Scene_Call3(void (*func)(s32, s32, s32), s32 a0, s32 a1, s32 a2)
{
    func(a0, a1, a2);
}

static __inline__ void Scene_Call6(void (*func)(s32, s32, s32, s32, s32, s32),
                                  s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    func(a0, a1, a2, a3, a4, a5);
}

void Func_02002766(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6,
                          u32 flags, u8 *extra);

void SetEffectMode(struct EffectWork *work, u32 mode)
{
    work->rec->f1 = mode;
}

void *NewEffectObject(s32 first, s32 second, s32 third, s32 fourth)
{
    void *overlay_object;
    void *object_record;
    s32 flags_mask;

    overlay_object = Object_Create(fourth, first, second, third);
    if (overlay_object != NULL) {
        object_record = FIELD_AT_OFFSET(overlay_object, void *, 0x50);
        flags_mask = -0xD;
        FIELD_AT_OFFSET(object_record, u8, 9) = (u8)(flags_mask & FIELD_AT_OFFSET(object_record, u8, 9));
        FIELD_AT_OFFSET(overlay_object, u8, 0x55) = 0;
        FIELD_AT_OFFSET(overlay_object, u8, 0x59) = 8;
        Actor_SetSpriteFlags(overlay_object, 0);
        Object_SetPalette(overlay_object, 0xE);
        Object_SetBlendMode(overlay_object, 1);
        return overlay_object;
    }
    return NULL;
}

void *NewFlippedEffectObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    u8 *result = Object_Create(arg3, arg0, arg1, arg2);

    if (result != NULL) {
        u8 *object = *(u8 **)(result + 0x50);
        s32 flags;
        s32 mask = 13;

        flags = object[9];
        mask = -mask;
        mask &= flags;
        mask |= 4;
        object[9] = mask;
        result[0x55] = 0;
        result[0x59] = 8;
        Actor_SetSpriteFlags(result, 0);
        Object_SetPalette(result, 15);
        result[0x23] = (result[0x23] & 0xfe) | 2;
        return result;
    }
    return NULL;
}

s32 StopXianActor(void *actor)
{
    Actor_SetSpriteFlags(actor, 0);
    return 0;
}

s32 FaceXianActorToPlayer(void *actor)
{
    void *player = Actor_Get(0);
    FIELD(actor, u16, 6) = Func_020046a2(FIELD(player, s32, 0x10) - FIELD(actor, s32, 0x10), FIELD(player, s32, 8) - FIELD(actor, s32, 8));
    return 0;
}

s32 GetXianScriptData(void)
{
    if (gGameState.scene == (s32)&Value_0000003c) {
        return (s32)Data_0200c7a8;
    }
    return (s32)Data_0200c838;
}

s32 GetXianInitialState(void)
{
    return 0;
}

s32 GetXianMessageData(void)
{
    return (s32)Data_0200c8c8;
}

void FieldScene_RunScene39e_02000414(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Event_SetMessage(0x178a);
    if (GameFlag_IsSet(0x890) != 0) {
        bump_step(4);
    }
    Event_OpenMessage(8, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        GameFlag_Set(0x890);
    } else {
        bump_step(1);
    }
    Event_ShowMessage(8, 0);
    Event_End();
}

void FieldScene_RunFlag88FBranch(void)
{
    extern u8 *Data_03001ebc;

    Event_Begin();
    if (GameFlag_IsSet(0x88F) != 0) {
        Event_SetMessage(0x17D6);
        Event_AskYesNo(12, 0);
        Event_End();
    } else {
        Event_SetMessage(0x1794);
        Event_OpenMessage(12, 0);
        if (Event_ChooseYesNo(0, 0) == 1) {
            u16 *q = (u16 *)(Data_03001ebc + 0x1D8);
            q[0] = q[0] + 1;
            Event_OpenMessage(12, 0);
            if (Event_ChooseYesNo(0, 0) == 1) {
                u16 *r = (u16 *)(Data_03001ebc + 0x1D8);
                r[0] = r[0] + 1;
            }
        }
        Event_ShowMessage(12, 0);
        Event_End();
    }
}

void Func_02000658(void)
{
    Event_Begin();
    GameFlag_Set(2196);
    Actor_FaceActor(9, 0, 0);
    Event_Wait(10);
    Event_SetMessage(6071);
    Actor_RunRepeatedMotion(9, 2);
    Event_Wait(20);
    Scene_Call3(Engine_ActorFaceDirection, 0, 32768, 20);
    Event_AskYesNo(9, 0);
    Event_Wait(10);
    Scene_Call3(Engine_ActorShowEmote, 9, 256, 80);
    Scene_Call3(Engine_ActorFaceDirection, 9, 53248, 20);
    Actor_RunRepeatedMotion(9, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_FaceDirection(9, 0, 20);
    Actor_SetAnimationAndWait(9, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(9, 0, 20);
    Scene_Call6(Engine_MapCopyCellAttributes, 10, 26, 1, 1, 10, 24);
    Event_End();
}

void Scene_RunScene39eSequenceB(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    Event_Begin();
    rec7 = GameFlag_IsSet(0x300);
    if (rec7 != 0) {
        Actor_WalkToAndWait(0, 168, 0x1f8);
        Event_Wait(5);
        Actor_FaceDirection(0, 0xc000, 20);
        *(u8 *)(Func_02004b56(8) + 91) = 0;
        Audio_PlayCue(152);
        record = Actor_Get(8);
        *(s32 *)(record + 40) = 0x80000;
        Actor_SetAnimation(8, 1);
        Event_Wait(30);
        Event_SetMessage(0x17ac);
    } else {
        Event_SetMessage(0x179f);
        FieldScene_SetFlag140AndFinishSequence(0, 8);
        Call1_0200071c((void (*)())Engine_EventWait, 30);
        Event_ShowMessage(8, 0);
        FieldScene_FinishSequence();
        Event_Wait(20);
        Actor_WalkToAndWait(0, 168, 0x1f8);
        Event_Wait(5);
        Actor_FaceDirection(0, 0xc000, 20);
        Audio_PlayCue(152);
        *(u8 *)(Func_02004bd4(8) + 91) = rec7;
        record = Actor_Get(8);
        *(s32 *)(record + 40) = 0x80000;
        Actor_SetAnimation(8, 1);
        Event_Wait(30);
        Event_OpenMessage(8, 0);
        if (Event_ChooseYesNo(0, 0) == 1) {
            Actor_RunRepeatedMotion(8, 2);
            Event_Wait(20);
            Event_ShowMessage(8, 0);
            ((void (*)())Engine_EventWait)(20);
            FieldScene_SetFlag140AndFinishSequence(8, 0);
            Event_Wait(30);
            Actor_RunRepeatedMotion(0, 2);
            Event_Wait(50);
            FieldScene_FinishSequence();
            Event_Wait(30);
            Actor_SetAnimationAndWait(8, 3);
            Event_ShowMessage(8, 0);
        } else {
            *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 2;
            Event_ShowMessage(8, 0);
        }
        Actor_SetAnimationAndWait(8, 3);
        Event_Wait(30);
        Actor_ShowEmote(8, 0x100, 60);
        Event_SetMessage(0x17a4);
        Event_OpenMessage(8, 0);
        if (Event_ChooseYesNo(0, 0) == 1) {
            Actor_ShowEmote(8, 0x105, 60);
            FieldScene_SetFlag140AndFinishSequence(8, 0);
            Event_Wait(30);
            Actor_RunRepeatedMotion(0, 2);
            Event_Wait(50);
            FieldScene_FinishSequence();
            Event_Wait(30);
            Event_ShowMessage(8, 0);
            *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
        } else {
            *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
            Event_Wait(20);
            Actor_SetAnimationAndWait(8, 3);
            Event_Wait(20);
            Event_ShowMessage(8, 0);
        }
        Event_Wait(20);
        Actor_SetAnimationAndWait(8, 4);
        Event_Wait(20);
        Event_ShowMessage(8, 0);
        Actor_RunRepeatedMotion(0, 2);
        Event_Wait(20);
        Event_ShowMessage(8, 0);
        Event_Wait(20);
        Actor_FaceDirection(8, 0xc000, 20);
        Actor_SetSpeed(8, 0x4ccc, 0x2666);
        Actor_WalkToAndWait(8, 168, 0x1d0);
        Event_Wait(60);
        Actor_FaceDirection(8, 0x4000, 40);
        Event_ShowMessageAndWait(8, 0, 10);
        Actor_ShowEmote(0, 0x102, 60);
        Actor_WalkToAndWait(8, 168, 0x1d8);
    }
    Event_OpenMessage(8, 0);
    if (Event_ChooseYesNo(0, 0) == 1) {
        Event_SetMessage(0x17ab);
        Event_ShowMessage(8, 0);
        GameFlag_Set(0x300);
    } else {
        Event_SetMessage(0x17ad);
        Event_Wait(30);
        Actor_SetAnimationAndWait(8, 3);
        Event_Wait(20);
        Camera_SetSpeed(0x8000, 0x1000);
        Camera_MoveToActor(8, 1);
        ColorBuffer_ApplySource(0x10000, 0);
        ColorBuffer_ApplyTarget(0x10003, 1);
        ColorBuffer_Interpolate(30);
        Event_WaitForScreen();
        Camera_WaitForMove();
        FieldScene_SpawnEightShots();
        ColorBuffer_ApplyTarget(0x10000, 0);
        ColorBuffer_Interpolate(30);
        Event_ShowMessage(8, 0);
        Actor_RunRepeatedMotion(8, 2);
        Event_Wait(20);
        Event_ShowMessage(8, 0);
        Actor_StartRepeatedMotion(0, 1);
        Actor_SetAttachedEffect(0, 0x102);
        Event_Wait(60);
        Event_ShowMessageAndWait(8, 0, 10);
        GameFlag_Set(0x891);
    }
    Actor_SetAnimation(8, 5);
    Event_End();
}

void FieldScene_ShowDialogue17B1(void)
{
    Event_Begin();
    Event_SetMessage(0x17B1);
    Event_AskYesNo(8, 0);
    Event_End();
}

void FieldScene_ShowDialogue1825(void)
{
    Event_Begin();
    Event_SetMessage(0x1825);
    Event_AskYesNo(9, 0);
    Event_End();
}

void FieldScene_RunRoofSceneExit(void)
{
    Event_Begin();

    ClearSceneExitGateAtEntry(12)[91] = 0;

    goto testPendingWork;
waitPendingWork:
        Task_Wait(1);
testPendingWork:
    if (*(s32 *)(GetSceneExitPendingWork(12) + 12) > 0) {
        goto waitPendingWork;
    }

    *(s32 *)(ResetSceneExitPendingWork(12) + 12) = 0;

    *(s32 *)(SetSceneExitCompletionMode(12) + 60) = 128 << 24;

    *(s32 *)(ClearSceneExitField40(12) + 40) = 0;

    SetSceneExitGate(12)[91] = 1;

    Actor_FaceActor(12, 0, 0);

    if (GameFlag_IsSet(0x895) != 0) {
        Event_SetMessage(0x1a5b);
    } else if (GameFlag_IsSet(0x89b) != 0) {
        Event_SetMessage(0x189e);
    } else {
        Event_SetMessage(0x182a);
    }

    Event_ShowMessage(12, 0);

    ((struct SceneRecordHeading *)Actor_Get(12))->heading = 128 << 7;

    ClearSceneExitGateBeforeDescriptor(12)[91] = 0;

    Actor_EnableActionCallback(12, (u8 *)0x0200c638);
    Event_End();
}

void FieldScene_ShowDialogue182D(void)
{
    Event_Begin();
    Event_SetMessage(0x182d);
    Event_AskYesNo(15, 0);
    Event_End();
}

void FieldScene_RunForwardArcBurst(void)
{
    u8 *record = Actor_Get(19);
    u32 index;
    s32 angle;

    for (index = 8; index > 3; index--) {
        angle = index << 12;
        *(u16 *)(*(u8 **)(record + 80) + 30) = (u16)angle;
        Task_Wait((index - 4) * 2);
        *(s32 *)(record + 8) += Math_Cos(angle)* 6;
        *(s32 *)(record + 16) += Math_Sin(angle)* 6;
    }

    *(s32 *)(record + 12) = 0x120000;
    *(s32 *)(record + 60) = 0x120000;

    Audio_PlayCue(227);

    Func_02000dae(*(s32 *)(record + 8) - 0xc0000,
                  *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + 0x80000,
                  0xffffcccd, 0x6666, 0, 0, 0);
    Func_02000dc6(*(s32 *)(record + 8),
                  *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + 0x80000,
                  0xffff3334, 0x4ccc, 0, 0, 0);
    Func_02000de4(*(s32 *)(record + 8) + 0xa0000,
                  *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + 0x80000,
                  0xffff0000, 0x3333, 0, 0, 0);
}

void FieldScene_RunDescentBurst(void)
{
    u8 *record = Actor_Get(19);
    u32 i = 0;
    s32 step = 8;
    s32 zero;
    do {
        Task_Wait(step);
        *(s32 *)(record + 16) += 0xffff0000;
        *(u32 *)(record + 64) = 0x80000000;
        i++;
        step -= 2;
    } while (i <= 3);
    zero = 0;
    *(u16 *)(*(u8 **)(record + 80) + 30) = (u16)zero;
    Audio_PlayCue(227);
    Func_02000f4c(*(s32 *)(record + 8), *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + 0xfff80000, 0xffff3334,
                  0, 0xffffcccd, 0, 0);
    Func_02000f62(*(s32 *)(record + 8), *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + 0xfff80000, 0x0000cccc,
                  0, 0xffffcccd, 0, 0);
    Func_02000f86(*(s32 *)(record + 8) + 0xfffa0000, *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + (160 << 12), 0x00003333,
                  0, 0xffff0000, 0, 0);
    Func_02000fa2(*(s32 *)(record + 8) + (192 << 11), *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + (160 << 12), 0x00003333,
                  0, 0xffff0000, 0, 0);
}

void FieldScene_DispatchApproachByFacing(void)
{
    Event_Begin();

    if (*(u16 *)(Func_020056ee(0) + 6) > (128 << 7)
        && *(u16 *)(Func_020056fe(0) + 6) < (192 << 8)) {
        FieldScene_RunForwardArcBurst();
    } else {
        Func_02001fe2();
    }

    if (GameFlag_IsSet(0x898) != 0) {
        Func_0200247c();
    } else {
        Func_02002350(0);
    }

    Event_End();
}

void FieldScene_DispatchByFacing(void)
{
    struct SceneActor_02001334 *record = Actor_Get(0);
    u16 angle;

    Event_Begin();
    Actor_SetAnimation(0, 8);
    Event_Wait(20);

    angle = *(u16 *)((u8 *)record + 6);

    if ((u16)(angle - 0x2000) <= 0x3fffu) {
        RunFacingVariantA();
    } else if ((u16)(angle - 0x6000) <= 0x3fffu) {
        RunFacingVariantB();
    } else if ((u16)(angle + (192 << 7)) <= 0x3fffu) {
        RunFacingVariantC();
    } else {
        RunFacingVariantD();
    }

    Actor_SetAnimation(0, 1);
    RunSceneVariant(1);
    Event_End();
}

void FieldScene_DispatchByFacingAndFlags(void)
{
    u8 *record = Actor_Get(0);
    u16 facing;

    Event_Begin();

    facing = *(u16 *)(record + 6);
    if ((u16)(facing - 0x2000) <= 0x3fff) {
        Scene_RunPrimarySequence();
    } else if ((u16)(facing - 0x6000) <= 0x3fff) {
        FieldScene_RunForwardArcBurst();
    } else if ((u16)(facing + 0x6000) <= 0x3fff) {
        FieldScene_RunDescentBurst();
    } else {
        Func_020020d8();
    }

    Camera_SetSpeed(0x10000, 0x2000);
    Camera_MoveToActor(20, 1);
    Camera_WaitForMove();

    if (*(s16 *)(record + 18) <= 209) {
        if (GameFlag_IsSet(0x89a) == 0) goto scene0;
        if (GameFlag_IsSet(0x89b) != 0) goto scene0;
        goto scene1;
scene0:
        Func_0200246a(0);
        goto firstSceneComplete;
scene1:
        Func_020025a4();
firstSceneComplete:
        Event_End();
        return;
    }

    if (GameFlag_IsSet(0x89b) != 0) {
        Func_02002486(2);
    } else if (GameFlag_IsSet(0x89a) == 0) {
        FieldScene_RunSecondEnsembleBeat();
    } else {
        FieldScene_RunEnsembleStoryBeat();
    }
    Event_End();
}

void FieldScene_RunSecondEnsembleBeat(void)
{
    s32 id;
    u8 *rec;

    GameFlag_Set(0x89a);
    Event_Wait(30);
    Actor_FaceActor(13, 0, 0);
    Actor_FaceActor(15, 0, 0);
    Actor_FaceActor(16, 0, 0);
    Event_Wait(20);
    Actor_ShowEmote(13, 128 << 1, 0);
    Actor_ShowEmote(15, 128 << 1, 0);
    Actor_ShowEmote(16, 128 << 1, 0);
    Event_Wait(60);
    Event_SetMessage(0x183b);
    Event_ShowMessageAndWait(13, 0, 20);
    Actor_FaceActor(0, 13, 0);
    Actor_RunRepeatedMotion(15, 1);
    Event_Wait(20);
    Event_ShowMessageAndWait(15, 0, 20);
    Actor_FaceActor(0, 15, 0);
    Actor_RunRepeatedMotion(16, 2);
    Event_Wait(20);
    Actor_FaceActor(0, 16, 0);
    Event_AskYesNo(16, 0);
    Event_Wait(50);
    Camera_FollowActor(16, 1);
    Actor_SetSpeed(16, 0xcccc, 0x6666);
    Actor_WalkToAndWait(16, 176, 248);
    Actor_WalkToAndWait(16, 154 << 1, 248);
    Actor_FaceDirection(0, 128 << 6, 0);
    Actor_FaceDirection(16, 192 << 8, 20);
    Audio_PlayCue(158);
    Map_AnimateCells(0x0200c77a, 78, 13);
    Actor_RunRepeatedMotion(16, 2);
    Event_Wait(20);
    Actor_SetSpeed(16, 192 << 9, 192 << 8);
    rec = Record1(Engine_ActorGet, 16);
    rec[90] &= 0xfe;
    Actor_WalkToAndWait(16, 154 << 1, 136 << 1);
    Event_Wait(1);
    rec = Record1(Engine_ActorGet, 16);
    {
        /*
         * A result temporary, not the compound or-assign the matching
         * &= 0xfe case above uses. The reference writes the result into
         * the mask register rather than the loaded value, and the
         * two-address ORR only does that when the merged result is its
         * own object; the compound form keeps the loaded value as
         * destination. Same technique already adopted in the sibling
         * owner resource_3bd:020013f8.
         */
        u8 merged = (u8)(rec[90] | 1);

        rec[90] = merged;
    }
    Event_ShowMessageAndWait(16, 0, 50);
    Actor_SetPosition(17, 152 << 17, 216 << 16);
    Actor_WalkToAndWait(17, 152 << 1, 248);
    Actor_FaceActor(9, 17, 0);
    Actor_FaceActor(10, 17, 0);
    Actor_FaceActor(11, 17, 0);
    Actor_FaceActor(12, 17, 0);
    Actor_FaceActor(13, 17, 0);
    Actor_FaceActor(14, 17, 0);
    Actor_FaceActor(15, 17, 0);
    Actor_FaceActor(16, 17, 0);
    Actor_FaceActor(0, 17, 0);
    Event_Wait(10);
    Actor_StartRepeatedMotion(9, 2);
    Actor_StartRepeatedMotion(10, 2);
    Actor_StartRepeatedMotion(11, 2);
    Actor_StartRepeatedMotion(12, 2);
    Actor_StartRepeatedMotion(13, 2);
    Actor_StartRepeatedMotion(14, 2);
    Actor_StartRepeatedMotion(15, 2);
    Actor_RunRepeatedMotion(16, 2);
    Actor_ShowEmote(17, 0x103, 60);
    Actor_SetPosition(18, 152 << 17, 216 << 16);
    Actor_WalkTo(18, 152 << 1, 248);
    Actor_WalkTo(17, 140 << 1, 132 << 1);
    Actor_WaitForMove(18);
    Actor_FaceDirection(18, 160 << 7, 0);
    Actor_WaitForMove(17);
    Audio_PlayCue(159);
    Map_AnimateCells(0x0200c790, 78, 13);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_FaceActor(9, 17, 0);
    Actor_FaceActor(10, 17, 0);
    Actor_FaceActor(11, 17, 0);
    Actor_FaceActor(12, 17, 0);
    Actor_FaceActor(13, 17, 0);
    Actor_FaceActor(14, 17, 0);
    Actor_FaceActor(15, 17, 0);
    Actor_FaceActor(16, 17, 0);
    Actor_FaceActor(0, 17, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(17, 2);
    Event_Wait(20);
    Actor_SetAnimationAndWait(18, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_SetAnimationAndWait(17, 3);
    Event_Wait(20);
    Actor_RunRepeatedMotion(18, 1);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_FaceDirection(17, 208 << 8, 20);
    Event_ShowMessageAndWait(17, 0, 20);
    Actor_ShowEmote(18, 0x102, 60);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_SetAnimationAndWait(17, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(17, 0, 20);
    Actor_SetAnimationAndWait(18, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_SetAnimationAndWait(17, 3);
    Event_Wait(20);
    Actor_SetAnimationAndWait(18, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_RunRepeatedMotion(17, 2);
    Event_Wait(20);
    Actor_FaceDirection(17, 0, 20);
    Event_ShowMessageAndWait(17, 0, 20);
    Actor_FaceDirection(16, 128 << 8, 20);
    Actor_SetAnimationAndWait(16, 3);
    Event_Wait(20);
    Actor_SetAnimationAndWait(17, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(17, 0, 20);
    Actor_SetAnimationAndWait(16, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(16, 0, 20);
    Actor_FaceDirection(17, 128 << 8, 20);
    Event_ShowMessageAndWait(17, 0, 20);
    Actor_SetAnimationAndWait(9, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_FaceDirection(17, 208 << 8, 20);
    Actor_RunRepeatedMotion(17, 1);
    Event_Wait(20);
    Event_ShowMessageAndWait(17, 0, 20);
    Actor_ShowEmote(18, 0x102, 60);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_ShowEmote(17, 0x101, 60);
    Event_ShowMessageAndWait(17, 0, 20);
    Actor_SetAnimationAndWait(18, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_ShowEmote(17, 0x100, 60);
    Event_ShowMessageAndWait(17, 0, 20);
    Actor_SetAnimationAndWait(18, 4);
    Event_Wait(20);
    Actor_ShowEmote(17, 0x103, 60);
    Event_ShowMessageAndWait(17, 0, 20);
    Actor_ShowEmote(18, 0x100, 60);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_SetAnimationAndWait(17, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(17, 0, 20);
    Actor_RunRepeatedMotion(18, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_RunRepeatedMotion(17, 2);
    Event_Wait(10);
    Actor_WalkToAndWait(17, 128 << 1, 140 << 1);
    Actor_FaceDirection(17, 128 << 7, 20);
    Actor_SetPosition(17, 0, 0);
    Actor_Destroy(17);
    Event_Wait(30);
    Actor_RunRepeatedMotion(9, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_RunRepeatedMotion(15, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(15, 0, 20);
    Actor_FaceActor(16, 18, 0);
    Event_Wait(20);
    Actor_RunRepeatedMotion(16, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(16, 0, 20);
    Actor_FaceActor(18, 16, 0);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_SetAnimationAndWait(18, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_SetAnimationAndWait(18, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_SetSpeed(18, 0xcccc, 0x6666);
    Actor_WalkToAndWait(18, 128 << 1, 248);
    Actor_FaceDirection(18, 192 << 8, 20);
    Actor_StartRepeatedMotion(18, 1);
    Actor_ShowEmote(18, 0x100, 60);
    Actor_WalkToAndWait(18, 240, 184);
    Actor_RunRepeatedMotion(18, 2);
    Event_Wait(20);
    Actor_SetPosition(19, 232 << 16, 168 << 16);
    Actor_SetPosition(20, 232 << 16, 168 << 16);
    rec = Record1(Engine_ActorGet, 19);
    *(s32 *)(rec + 12) = 0xc0000;
    rec = Record1(Engine_ActorGet, 19);
    *(s32 *)(rec + 60) = -0x80000000;
    rec = Record1(Engine_ActorGet, 19);
    *(s32 *)(rec + 24) = 0xcccc;
    rec = Record1(Engine_ActorGet, 19);
    {
        u8 *target = *(u8 **)(rec + 80);
        s32 shown = 0x8000;

        *(u16 *)(target + 30) = shown;
    }
    Audio_PlayCue(124);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_FaceDirection(0, 192 << 8, 20);
    Actor_WalkToAndWait(16, 128 << 1, 240);
    Actor_FaceDirection(16, 176 << 8, 20);
    Actor_RunRepeatedMotion(16, 1);
    Event_ShowMessageAndWait(16, 0, 20);
    Actor_FaceActor(9, 0, 0);
    Actor_FaceActor(10, 0, 0);
    Actor_FaceActor(11, 0, 0);
    Actor_FaceActor(12, 0, 0);
    Actor_FaceActor(13, 0, 0);
    Actor_FaceActor(14, 0, 0);
    Actor_FaceActor(15, 0, 0);
    Actor_FaceActor(16, 0, 0);
    Actor_RunRepeatedMotion(18, 2);
    Event_Wait(20);
    Actor_FaceDirection(18, 160 << 7, 20);
    Actor_WalkToAndWait(18, 248, 208);
    Actor_FaceDirection(18, 160 << 7, 20);
    Event_OpenMessage(18, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Value2((s32 (*)())Engine_ActorRunRepeatedMotion, 16, 1);
        Event_Wait(20);
        id = 16;
        goto joinBeat;
    }

    /* Skipped once: bump the workspace skip counter and offer the beat again. */
    Scene_BumpStep(1);
    Event_Wait(20);
    Actor_ShowEmote(18, 0x105, 60);
    Actor_FaceDirection(18, 128 << 7, 20);
    Actor_RunRepeatedMotion(16, 2);
    Event_Wait(20);
    Event_OpenMessage(16, 0);
    if (Event_ChooseYesNo(0, 0) != 0) {
        goto skipTwice;
    }
    Actor_SetAnimationAndWait(16, 3);
    Event_Wait(20);
    Actor_FaceDirection(18, 176 << 8, 20);
    id = 18;

joinBeat:
    Event_ShowMessageAndWait(id, 0, 20);
    GameFlag_Set(0x898);
    goto finish;

skipTwice:
    Scene_BumpStep(1);
    Event_Wait(20);
    Actor_SetAnimationAndWait(18, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 20);
    GameFlag_Set(0x899);

finish:
    Actor_FaceDirection(10, 128 << 8, 0);
    Actor_FaceDirection(11, 128 << 8, 20);
    Actor_SetAnimation(10, 5);
    Actor_SetAnimation(11, 5);
    Engine_ActorEnableActionCallback(12, 0x0200c638);
}

void FieldScene_RunSkippableStoryBeat(void)
{
    extern u8 *Data_03001ebc;

    u8 *workspace;

    Event_Begin();
    Event_SetMessage(0x186e);
    Event_OpenMessage(18, 0);

    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(20);
        Event_ShowMessageAndWait(18, 0, 20);
        GameFlag_Set(0x898);
        Event_End();
    } else {
        workspace = Data_03001ebc;
        *(u16 *)(workspace + 472) += 1;
        Event_ShowMessageAndWait(18, 0, 20);
        Event_End();
    }
}

/* Looks up actors 18, 13, 14, 15 and 16 and clears their +108 field before
 * the scene runs. */
void FieldScene_RunEnsembleStoryBeat(void)
{
    u32 i;
    s32 actor;

    actor = Actor_Get(18);
    ACTOR_FIELD_108(actor) = 0;
    actor = Scene_GetRecord_2(13);
    ACTOR_FIELD_108(actor) = 0;
    actor = Scene_GetRecord_3(14);
    ACTOR_FIELD_108(actor) = 0;
    actor = Scene_GetRecord_4(15);
    ACTOR_FIELD_108(actor) = 0;
    actor = Actor_Get(16);
    ACTOR_FIELD_108(actor) = 0;
    Object_SetModeById_1(11, 1);
    Camera_SetSpeed(0x8000, 0x1000);
    Camera_MoveTo(0xe80000, -1, 0xc80000, 1);
    Camera_WaitForMove();
    Event_SetMessage(0x1883);
    Actor_SetSpeed(10, 0xcccc, 0x6666);
    Actor_SetSpeed(12, 0xcccc, 0x6666);
    Actor_WalkTo(10, 152, 200);
    Actor_WalkToAndWait(12, 144, 248);
    Actor_WaitForMove(10);
    Actor_FaceActor(9, 19, 0);
    Actor_FaceActor(11, 19, 0);
    Actor_FaceActor(13, 19, 0);
    Actor_FaceActor(14, 19, 0);
    Actor_FaceActor(15, 19, 0);
    Actor_FaceActor(16, 19, 0);
    Actor_FaceActor(18, 19, 0);
    Actor_SetSpeed(10, 0x18000, 0xc000);
    Actor_SetSpeed(12, 0x20000, 0x10000);
    Actor_WalkTo(10, 152, 200);
    Actor_WalkToAndWait(12, 144, 248);
    Actor_FaceActor(12, 19, 0);
    Actor_WaitForMove(10);
    Actor_FaceActor(10, 19, 0);
    Actor_RunRepeatedMotion(18, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 40);
    Actor_FaceActor(9, 18, 0);
    Actor_FaceActor(10, 18, 0);
    Actor_FaceDirection(11, 0x3000, 0);
    Actor_FaceActor(12, 18, 0);
    Actor_FaceDirection(13, 0x3000, 0);
    Actor_FaceActor(14, 18, 0);
    Actor_FaceActor(15, 18, 0);
    Actor_FaceActor(16, 18, 0);
    Event_Wait(20);
    Event_ShowMessageAndWait(16, 0, 20);
    Actor_SetAnimationAndWait(18, 3);
    Event_Wait(20);
    Actor_SetAnimationAndWait(16, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(16, 0, 20);
    Actor_ShowEmote(18, 0x105, 60);
    Actor_ShowEmote(16, 0x101, 60);
    Event_ShowMessageAndWait(16, 0, 20);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_ShowEmote(16, 0x102, 60);
    Actor_ShowEmote(15, 0x101, 60);
    Actor_SetSpeed(15, 0xcccc, 0x6666);
    Actor_WalkToAndWait(15, 216, 176);
    Actor_FaceDirection(15, 0x3000, 20);
    Event_ShowMessageAndWait(15, 0, 20);
    Actor_FaceDirection(18, 0xb000, 20);
    Actor_SetAnimationAndWait(18, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_StartRepeatedMotion(9, 2);
    Actor_StartRepeatedMotion(10, 2);
    Actor_StartRepeatedMotion(11, 2);
    Actor_StartRepeatedMotion(12, 2);
    Actor_StartRepeatedMotion(13, 2);
    Actor_StartRepeatedMotion(14, 2);
    Actor_StartRepeatedMotion(15, 2);
    Actor_StartRepeatedMotion(16, 2);
    Event_Wait(40);
    Actor_RunRepeatedMotion(13, 2);
    Event_ShowMessageAndWait(13, 0, 20);
    Actor_SetAnimationAndWait(18, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_FaceDirection(0, 0xe000, 20);
    Actor_FaceDirection(18, 0x5000, 20);
    Event_AskYesNo(18, 0);
    Actor_ShowEmote(9, 0x101, 0);
    Event_Wait(5);
    Actor_ShowEmote(10, 0x101, 0);
    Event_Wait(5);
    Actor_ShowEmote(11, 0x101, 0);
    Event_Wait(5);
    Actor_ShowEmote(12, 0x101, 0);
    Event_Wait(5);
    Actor_ShowEmote(13, 0x101, 0);
    Event_Wait(5);
    Actor_ShowEmote(14, 0x101, 0);
    Event_Wait(5);
    Actor_ShowEmote(15, 0x101, 0);
    Event_Wait(5);
    Actor_ShowEmote(16, 0x101, 0);
    Event_Wait(60);
    Actor_RunRepeatedMotion(16, 2);
    Event_ShowMessageAndWait(16, 0, 20);
    Actor_SetAnimationAndWait(18, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_ShowEmote(15, 0x101, 60);
    Event_ShowMessageAndWait(15, 0, 20);
    Actor_FaceActor(18, 15, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(18, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 40);
    Actor_FaceEachOther(11, 10, 0);
    Actor_FaceEachOther(12, 14, 0);
    Actor_FaceEachOther(13, 15, 0);
    Event_Wait(60);
    Actor_FaceActor(10, 18, 0);
    Actor_FaceActor(11, 18, 0);
    Actor_FaceActor(12, 18, 0);
    Actor_FaceActor(13, 18, 0);
    Actor_FaceActor(14, 18, 0);
    Actor_FaceActor(15, 18, 0);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_RunRepeatedMotion(18, 2);
    Event_Wait(20);
    Actor_SetAnimation(9, 3);
    Actor_SetAnimation(10, 3);
    Actor_SetAnimation(11, 3);
    Actor_SetAnimation(12, 3);
    Actor_SetAnimation(13, 3);
    Actor_SetAnimation(14, 3);
    Actor_SetAnimation(15, 3);
    Actor_SetAnimationAndWait(16, 3);
    Event_Wait(20);
    Actor_FaceDirection(18, 0x5000, 20);
    Event_AskYesNo(18, 0);
    Actor_SetAnimationAndWait(18, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimation(9, 3);
    Actor_SetAnimation(10, 3);
    Actor_SetAnimation(11, 3);
    Actor_SetAnimation(12, 3);
    Actor_SetAnimation(13, 3);
    Actor_SetAnimation(14, 3);
    Actor_SetAnimation(15, 3);
    ObjectMotion_CallThenWaitForAnimationChange_9(16, 3);
    Event_Wait(20);
    Actor_RunRepeatedMotion(18, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 20);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimationAndWait(18, 3);
    Event_Wait(20);
    Actor_FaceDirection(18, 0x8000, 20);
    Actor_RunRepeatedMotion(18, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_SetAnimation(9, 3);
    Actor_SetAnimation(10, 3);
    Actor_SetAnimation(11, 3);
    Actor_SetAnimation(12, 3);
    Actor_SetAnimation(13, 3);
    Actor_SetAnimation(14, 3);
    Actor_SetAnimation(15, 3);
    Actor_SetAnimationAndWait(16, 3);
    Event_Wait(20);
    Actor_WalkTo(10, 120, 200);
    Actor_WalkTo(12, 120, 248);
    Actor_WaitForMove(10);
    Actor_FaceDirection(11, 0x8000, 20);
    Actor_SetAnimation(10, 5);
    Actor_SetAnimation(11, 5);
    Actor_WaitForMove(12);
    ObjectMotion_EnableActionAndSetCallback_1(12, 0x200c638);
    Actor_SetSpeed(15, 0xcccc, 0x6666);
    Actor_WalkToAndWait(15, 216, 168);
    Actor_WalkToAndWait(15, 232, 168);
    Actor_FaceDirection(15, 0xc000, 20);
    Actor_RunRepeatedMotion(15, 3);
    Actor_SetPosition(19, 0xe80000, 0xa80000);
    actor = Actor_Get(19);
    *(s32 *)(actor + 12) = 0xc0000;
    actor = Actor_Get(19);
    *(s32 *)(actor + 60) = -0x80000000;
    actor = Actor_Get(19);
    {
        s32 target = *(s32 *)(actor + 80);
        s32 shown = 0x8000;

        *(u16 *)(target + 30) = shown;
    }
    Audio_PlayCue(124);
    Event_Wait(40);
    Actor_WalkToAndWait(15, 216, 152);
    Actor_FaceDirection(15, 0x4000, 30);
    GameFlag_Clear(0x898);
    GameFlag_Set(0x89b);
}

void FieldScene_ShowDialogue1A58(void)
{
    Event_Begin();
    Event_SetMessage(0x1a58);
    Event_AskYesNo(11, 0);
    Event_End();
}

void StartSchoolDoorEvent(void)
{
    Event_Begin();
    if (GameFlag_IsSet(2202) == 0 && GameFlag_IsSet(2197) == 0) {
        Message_ShowCentered(6317, 1);
        Event_End();
    } else {
        Audio_PlayCue(158);
        Map_AnimateCells(0x0200c77a, 78, 13);
        SetScale(0, 0x8000, 0x4000);
        SetPosition(0, 306, 248);
        Actor_WalkTo(0, 304, 216);
        Event_Wait(20);
        Event_RequestExit(4);
        Event_End();
    }
}

void FieldScene_DispatchByRange(void)
{
    u8 *record;
    u32 biased;

    record = Actor_Get(0);
    biased = *(u16 *)(record + 6);
    Event_Begin();

    biased = biased + 0xffff5fff;
    if (biased <= 0x3ffe) {
        Sanctum_Open(13);
    } else {
        Event_SetMessage(0x1a1c);
        Event_ShowMessage(13, 0);
    }

    Event_End();
}

void FieldScene_ShowDialogue17DF(void)
{
    Event_Begin();
    Actor_RunRepeatedMotion(8, 2);
    Event_SetMessage(0x17df);
    Event_ShowMessage(8, 0);
    Event_End();
}

s32 FieldScene_SelectData(void)
{
    if (gGameState.scene == (s32)&Value_0000003c) {
        return (s32)Data_0200cb90;
    }
    if (gGameState.entrance == 3) {
        return (s32)Data_0200d184;
    }
    return (s32)Data_0200cd40;
}

void FieldScene_SpawnRandomizedParticle(void)
{
    struct Params params;
    u8 *record;
    s32 draw;
    s32 offset;

    record = Actor_Get(0);

    params.field1 = 7;
    draw = (u32)(Random_Next() * 7) >> 16;
    if ((draw & 7) == 0)
        params.field1 = 5;

    params.field2 = 0xb333;
    params.field3 = 0xcccc;

    offset = ((u32)(Random_Next() * 8) >> 16) * 13107;

    Func_02002766(*(s32 *)(record + 8) + ((8 - (Data_03001e40 & 15)) << 16),
                  *(s32 *)(record + 12) + (192 << 13),
                  *(s32 *)(record + 16),
                  0,
                  -offset,
                  0,
                  144 << 12,
                  (u8 *)&params);

    if ((Data_03001e40 & 1) != 0)
        Actor_SetChildValue(0, 15);
    else
        Actor_SetChildValue(0, 1);
}

void FieldScene_ApplyOffset0Neg32(void)
{
    FieldScene_RunOpeningAuxiliarySequence(0, -32);
}

void FieldScene_ApplyOffset0Pos32(void)
{
    Func_02004d00(0, 32);
}

void FieldScene_ApplyOffsetNeg32_0(void)
{
    Func_02004d12(-32, 0);
}

void FieldScene_RunOpeningAuxiliarySequence(s32 a0, s32 a1)
{
    u32 i;
    s32 record;

    Event_Begin();
    Actor_SetSpeed(0, 0x28000, 0x14000);
    Value3(Engine_ActorSetDestinationOffset, 0, a0, a1);
    Actor_Jump(0, 4, 0);
    Actor_SetAnimation(0, 7);
    Actor_WaitForMove(0);
    Actor_SetAnimation(0, 6);
    Event_End();
}

void FieldScene_RunScene39eSequenceA(void)
{
    u32 i;
    s32 record;
    s32 base5_200a5b9;

    Event_Begin();
    base5_200a5b9 = (s32)Func_0200a5b9;
    Call2_020026d8(Func_02006a34, base5_200a5b9, 0xc80);
    Actor_SetSpeed(0, 0x3333, 0x1999);
    gEventWork->transition_frames = 60;
    Event_CloseScreen();
    Audio_PlayCue(154);
    Actor_SetAnimation(0, 2);
    Actor_SetDestinationOffset(0, 0, -6);
    Actor_WaitForMove(0);
    Actor_SetChildValue(0, 15);
    record = Actor_Get(0);
    Actor_SetSpriteFlags(record, 0);
    Func_02006a92(base5_200a5b9);
    Event_WaitForScreen();
    Event_RequestExit(3);
    Event_End();
}

void FieldScene_PlaySound123AndEnable(void)
{
    Audio_PlayCue(123);
    Event_RequestExit(1);
}

void FieldScene_RunScene39e_02002778(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Audio_PlayCue(188);
    Map_AnimateCells(0x200c764, 77, 8);
    *(u8 *)(Func_02006b96(0) + 85) = 0;
    Actor_SetSpeed(0, 0xcccc, 0x6666);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    Actor_SetAnimation(0, 2);
    Actor_SetDestinationOffset(0, 0, -16);
    Event_Wait(16);
    Event_RequestExit(2);
    Event_End();
}

void FieldScene_RunScene39e_020027ec(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Actor_SetSpeed(0, 0x8000, 0x4000);
    Actor_WalkTo(0, 168, 0x1f8);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    Event_OpenScreen();
    Event_WaitForScreen();
    Actor_WaitForMove(0);
    Event_Wait(20);
    Actor_StartRepeatedMotion(8, 2);
    Actor_SetAttachedEffect(8, 0x102);
    Event_Wait(60);
    *(u8 *)(Func_02006c4e(8) + 91) = 0;
    Audio_PlayCue(152);
    record = Actor_Get(8);
    *(s32 *)(record + 40) = 0x80000;
    Actor_SetAnimation(8, 1);
    Event_SetMessage(0x17be);
    Event_ShowMessageAndWait(8, 0, 20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_SetAnimationAndWait(0, 3);
    Event_Wait(20);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_ShowEmote(0, 0x101, 60);
    Event_OpenMessage(8, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(10);
        Actor_SetAnimationAndWait(8, 3);
        Event_Wait(20);
        Event_ShowMessageAndWait(8, 0, 20);
        bump_step(2);
    } else {
        Event_Wait(10);
        Actor_RunRepeatedMotion(8, 2);
        Event_Wait(20);
        bump_step(1);
        Event_ShowMessageAndWait(8, 0, 20);
        Actor_SetAnimationAndWait(8, 3);
        Event_Wait(20);
        Event_ShowMessageAndWait(8, 0, 20);
    }
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_RunRepeatedMotion(8, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_ShowEmote(0, 0x101, 60);
    Event_OpenMessage(8, 0);
    if (Event_ChooseYesNo(0, 0) == 1) {
        Event_Wait(10);
        Actor_ShowEmote(8, 0x102, 60);
        Event_SetMessage(0x17c8);
        Event_OpenMessage(8, 0);
    L_0200299c:
        if (Event_ChooseYesNo(0, 0) == 1) {
            Event_Wait(10);
            Actor_ShowEmote(8, 0x102, 60);
            Event_SetMessage((s32)Data_000017e0);
            Event_OpenMessage(8, 0);
            goto L_0200299c;
        }
    }
    Event_SetMessage(0x17c9);
    Event_Wait(10);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Event_OpenMessage(8, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(10);
        Actor_SetAnimationAndWait(0, 3);
        Event_Wait(20);
        Event_ShowMessageAndWait(8, 0, 20);
        bump_step(1);
    } else {
        Event_Wait(10);
        Actor_RunRepeatedMotion(8, 2);
        bump_step(1);
        Event_ShowMessageAndWait(8, 0, 20);
    }
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_RunRepeatedMotion(8, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_SetAnimationAndWait(0, 3);
    Event_Wait(20);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Actor_SetAnimation(8, 5);
    GameFlag_Set(0x893);
    Event_End();
}

/* Long scripted sequence: sets up and steps a series of actors (indices 0-3,
 * 8-10, 17, 20) through position, pose, animation, wait, and flag-bit calls,
 * copying a couple of fields between some actors' records along the way. */
void FieldScene_RunRoofEnsembleSequence(void)
{
    u32 unused;
    u8 *source_record;

    Event_Begin();
    Actor_SetSpeed(0, 0x6666, 0x3333);
    Actor_WalkTo(0, 0x1d8, 0x218);
    Event_OpenScreen();
    Event_WaitForScreen();
    Actor_WaitForMove(0);
    Actor_FaceDirection(9, 0, 20);
    Actor_RunRepeatedMotion(9, 2);
    Event_Wait(20);
    Event_SetMessage(0x1969);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_FaceDirection(0, 0x8000, 20);
    Actor_SetAnimationAndWait(0, 3);
    Event_Wait(30);
    Actor_FaceDirection(8, 0x4000, 30);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_FaceDirection(0, 0xc000, 20);
    Actor_RunRepeatedMotion(0, 1);
    Event_Wait(20);
    Actor_FaceDirection(9, 0xd000, 0);
    /* Copy the pair of s32 fields at +8/+16 from actor 0's record (as seen
     * through each accessor) into the matching setter for another actor. */
    source_record = Actor_Get(0);
    if (source_record != 0) {
        Actor_SetPosition(1, *(s32 *)(source_record + 8), *(s32 *)(source_record + 16));
    }
    source_record = Actor_Get(0);
    if (source_record != 0) {
        Actor_SetPosition(2, *(s32 *)(source_record + 8), *(s32 *)(source_record + 16));
    }
    source_record = Actor_Get(0);
    if (source_record != 0) {
        Actor_SetPosition(3, *(s32 *)(source_record + 8), *(s32 *)(source_record + 16));
    }
    Actor_SetSpeed(0, 0x8000, 0x4000);
    Actor_SetSpeed(1, 0x8000, 0x4000);
    Actor_SetSpeed(2, 0x8000, 0x4000);
    Actor_SetSpeed(3, 0x8000, 0x4000);
    Actor_WalkTo(0, 0x1d0, 0x1f8);
    Actor_WalkTo(2, 0x1e0, 0x1f8);
    Actor_WalkTo(1, 0x1f0, 0x1f0);
    Actor_WalkTo(3, 0x1c0, 0x1f0);
    Actor_WaitForMove(0);
    Actor_WaitForMove(2);
    Actor_WaitForMove(3);
    Actor_FaceDirection(3, 0xe000, 0);
    Actor_WaitForMove(1);
    Actor_FaceDirection(1, 0xa000, 0);
    Event_Wait(20);
    Actor_ShowEmote(0, 0x101, 0);
    Actor_ShowEmote(1, 0x101, 0);
    Actor_ShowEmote(2, 0x101, 0);
    Actor_ShowEmote(3, 0x101, 60);
    Actor_FaceDirection(1, 0x5000, 20);
    Event_AskYesNo(1, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_FaceDirection(1, 0xa000, 20);
    Actor_ShowEmote(3, 0x100, 60);
    Event_ShowMessageAndWait(3, 0, 20);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_ShowEmote(2, 0x100, 60);
    Event_ShowMessageAndWait(2, 0, 20);
    Actor_SetAnimationAndWait(8, 3);
    Actor_FaceDirection(8, 0x3000, 20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_FaceDirection(0, 0, 0);
    Actor_FaceDirection(1, 0x6000, 0);
    Actor_FaceDirection(3, 0x2000, 0);
    Actor_StartRepeatedMotion(2, 2);
    Actor_SetAttachedEffect(2, 0x102);
    Event_Wait(60);
    Actor_FaceDirection(0, 0xc000, 0);
    Actor_FaceDirection(1, 0xa000, 0);
    Actor_FaceDirection(3, 0xe000, 0);
    Event_Wait(20);
    Event_ShowMessageAndWait(2, 0, 20);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_RunRepeatedMotion(2, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(2, 0, 20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_FaceEachOther(0, 2, 50);
    Actor_FaceDirection(0, 0xc000, 0);
    Actor_FaceDirection(2, 0xc000, 30);
    Actor_SetAnimationAndWait(2, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_ShowEmote(3, 0x101, 60);
    Event_ShowMessageAndWait(3, 0, 20);
    Actor_FaceDirection(8, 0x5000, 20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_RunRepeatedMotion(3, 1);
    Event_Wait(20);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_FaceDirection(0, 0xe000, 0);
    Actor_FaceDirection(1, 0x6000, 0);
    Event_Wait(30);
    Actor_FaceDirection(0, 0xc000, 0);
    Actor_FaceDirection(1, 0xa000, 20);
    Event_ShowMessageAndWait(1, 0, 20);
    Actor_SetAnimationAndWait(8, 4);
    Event_Wait(20);
    Actor_FaceActor(8, 2, 0);
    Event_Wait(10);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_FaceActor(0, 2, 0);
    Actor_FaceActor(1, 2, 0);
    Actor_FaceActor(3, 2, 0);
    Actor_ShowEmote(2, 0x102, 60);
    Event_ShowMessageAndWait(2, 0, 20);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_SetAnimationAndWait(2, 3);
    Event_Wait(20);
    Actor_StartRepeatedMotion(1, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(1, 0, 20);
    Actor_ShowEmote(8, 0x100, 60);
    Actor_FaceActor(8, 1, 0);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_ShowEmote(2, 0x100, 60);
    Event_ShowMessageAndWait(2, 0, 20);
    Actor_FaceActor(8, 1, 0);
    Actor_FaceDirection(0, 0xc000, 0);
    Actor_FaceDirection(1, 0xa000, 0);
    Actor_FaceDirection(3, 0xe000, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_StartRepeatedMotion(2, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(2, 0, 20);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_RunRepeatedMotion(3, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(3, 0, 20);
    Actor_FaceDirection(8, 0x5000, 20);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_StartRepeatedMotion(0, 2);
    Actor_StartRepeatedMotion(1, 2);
    Actor_StartRepeatedMotion(2, 2);
    Actor_RunRepeatedMotion(3, 2);
    Event_Wait(20);
    Actor_SetAnimationAndWait(8, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_RunRepeatedMotion(8, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimation(2, 3);
    Actor_SetAnimationAndWait(3, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_ShowEmote(1, 0x103, 0);
    Actor_StartRepeatedMotion(1, 2);
    Event_Wait(60);
    Event_ShowMessageAndWait(1, 0, 20);
    Actor_SetAnimationAndWait(8, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_ShowEmote(0, 0x101, 0);
    Actor_ShowEmote(1, 0x101, 60);
    Event_ShowMessageAndWait(1, 0, 20);
    Actor_ShowEmote(8, 0x102, 0);
    Actor_StartRepeatedMotion(8, 1);
    Event_Wait(60);
    Actor_SetAnimationAndWait(8, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_StartRepeatedMotion(0, 1);
    Actor_StartRepeatedMotion(1, 1);
    Actor_StartRepeatedMotion(2, 1);
    Actor_RunRepeatedMotion(3, 1);
    Event_Wait(20);
    Actor_SetAnimationAndWait(8, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_ShowEmote(1, 0x101, 60);
    Actor_FaceDirection(1, 0x5000, 20);
    Event_AskYesNo(1, 0);
    Event_Wait(20);
    Actor_RunRepeatedMotion(2, 1);
    Event_Wait(20);
    Event_ShowMessageAndWait(2, 0, 20);
    Actor_FaceDirection(1, 0xa000, 20);
    Actor_FaceDirection(8, 0xc000, 20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimation(2, 3);
    Actor_SetAnimationAndWait(3, 3);
    Event_Wait(20);
    Actor_FaceDirection(8, 0x4000, 20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_StartRepeatedMotion(0, 2);
    Actor_StartRepeatedMotion(1, 2);
    Actor_StartRepeatedMotion(2, 2);
    Actor_RunRepeatedMotion(3, 2);
    Event_Wait(20);
    Actor_SetAnimationAndWait(8, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_ShowEmote(2, 0x101, 60);
    Event_ShowMessageAndWait(2, 0, 20);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_SetAttachedEffect(0, 0x102);
    Actor_SetAttachedEffect(1, 0x102);
    Actor_SetAttachedEffect(2, 0x102);
    Actor_SetAttachedEffect(3, 0x102);
    Event_Wait(60);
    Event_ShowMessageAndWait(3, 0, 20);
    Actor_FaceDirection(8, 0x5000, 20);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_ShowEmote(0, 0x101, 0);
    Actor_ShowEmote(1, 0x101, 0);
    Actor_ShowEmote(2, 0x101, 0);
    Actor_ShowEmote(3, 0x101, 60);
    Actor_RunRepeatedMotion(1, 1);
    Event_Wait(20);
    Event_ShowMessageAndWait(1, 0, 20);
    Actor_FaceActor(0, 1, 0);
    Actor_FaceActor(2, 1, 0);
    Actor_FaceActor(3, 1, 0);
    Event_Wait(20);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimation(2, 3);
    Actor_SetAnimationAndWait(3, 3);
    Event_Wait(60);
    Actor_FaceDirection(0, 0xc000, 0);
    Actor_FaceDirection(2, 0xc000, 0);
    Actor_FaceDirection(3, 0xd000, 0);
    Event_Wait(20);
    Event_AskYesNo(8, 0);
    Actor_ShowEmote(0, 0x102, 0);
    Actor_ShowEmote(1, 0x102, 0);
    Actor_ShowEmote(2, 0x102, 0);
    Actor_ShowEmote(3, 0x102, 0);
    Event_Wait(60);
    Actor_FaceDirection(1, 0x6000, 20);
    Event_AskYesNo(1, 0);
    Event_Wait(20);
    Actor_FaceDirection(3, 0x2000, 20);
    Event_ShowMessageAndWait(3, 0, 20);
    Actor_FaceDirection(1, 0xa000, 0);
    Actor_FaceDirection(3, 0xe000, 30);
    Actor_FaceDirection(8, 0, 20);
    Actor_ShowEmote(8, 0x105, 60);
    Actor_ShowEmote(2, 0x101, 60);
    Event_ShowMessageAndWait(2, 0, 20);
    Actor_FaceDirection(8, 0xc000, 20);
    Actor_ShowEmote(8, 0x105, 60);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_ShowEmote(0, 0x101, 0);
    Actor_ShowEmote(1, 0x101, 0);
    Actor_ShowEmote(2, 0x101, 0);
    Actor_ShowEmote(3, 0x101, 60);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_ShowEmote(0, 0x101, 0);
    Actor_ShowEmote(1, 0x101, 0);
    Actor_ShowEmote(2, 0x101, 0);
    Actor_ShowEmote(3, 0x101, 60);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimation(2, 3);
    Actor_SetAnimationAndWait(3, 3);
    Event_Wait(20);
    Actor_RunRepeatedMotion(8, 1);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_FaceDirection(0, 0, 0);
    Actor_FaceDirection(1, 0x6000, 0);
    Actor_FaceDirection(2, 0x8000, 0);
    Actor_FaceDirection(3, 0x2000, 0);
    Event_Wait(60);
    Actor_FaceDirection(0, 0xc000, 0);
    Actor_FaceDirection(1, 0xa000, 0);
    Actor_FaceDirection(2, 0xc000, 0);
    Actor_FaceDirection(3, 0xe000, 0);
    Event_Wait(30);
    Actor_FaceDirection(8, 0x4000, 20);
    Actor_SetAnimationAndWait(8, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_SetPosition(10, 0x1d80000, 0x2600000);
    Event_ShowMessageAndWait(10, 0, 20);
    Actor_ShowEmote(0, 0x100, 0);
    Actor_ShowEmote(1, 0x100, 0);
    Actor_ShowEmote(2, 0x100, 0);
    Actor_ShowEmote(3, 0x100, 0);
    Actor_ShowEmote(8, 0x100, 0);
    Actor_ShowEmote(9, 0x100, 0);
    Event_Wait(60);
    Actor_FaceDirection(0, 0x4000, 0);
    Actor_FaceDirection(1, 0x4000, 0);
    Actor_FaceDirection(2, 0x4000, 0);
    Actor_FaceDirection(3, 0x4000, 0);
    Actor_FaceDirection(9, 0, 0);
    Event_Wait(30);
    Actor_SetSpeed(10, 0xcccc, 0x6666);
    Actor_WalkToAndWait(10, 0x1d8, 0x218);
    Actor_ShowEmote(8, 0x101, 60);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_FaceDirection(2, 0xc000, 40);
    Actor_WalkToAndWait(2, 0x1e8, 0x200);
    Event_Wait(10);
    Actor_FaceDirection(2, 0x6000, 20);
    Actor_FaceDirection(0, 0, 30);
    Actor_WalkToAndWait(0, 0x1c8, 0x200);
    Event_Wait(10);
    Actor_FaceDirection(0, 0x2000, 20);
    Actor_RunRepeatedMotion(10, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(10, 0, 20);
    Actor_ShowEmote(8, 0x101, 60);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_StartRepeatedMotion(10, 2);
    Actor_SetAttachedEffect(10, 0x102);
    Event_Wait(60);
    Event_ShowMessageAndWait(10, 0, 20);
    Actor_ShowEmote(8, 0x101, 60);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_ShowEmote(10, 0x100, 60);
    Event_ShowMessageAndWait(10, 0, 20);
    Actor_StartRepeatedMotion(8, 1);
    Actor_ShowEmote(8, 0x100, 60);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_FaceDirection(0, 0xe000, 0);
    Actor_FaceDirection(1, 0xa000, 0);
    Actor_FaceDirection(2, 0xa000, 0);
    Actor_FaceDirection(3, 0xe000, 0);
    Event_Wait(40);
    Actor_SetAnimationAndWait(10, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(10, 0, 20);
    Actor_SetAnimationAndWait(8, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_ShowEmote(8, 0x101, 60);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_StartRepeatedMotion(17, 1);
    Event_Wait(20);
    Event_ShowMessageAndWait(10, 0, 20);
    Actor_ShowEmote(8, 0x105, 60);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_ShowEmote(8, 0x100, 30);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_SetAnimation(0, 3);
    Event_Wait(2);
    Actor_SetAnimation(2, 3);
    Event_Wait(1);
    Actor_SetAnimation(3, 3);
    Event_Wait(5);
    Actor_SetAnimationAndWait(1, 3);
    Actor_FaceActor(8, 0, 0);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_SetSpeed(10, 0xcccc, 0x6666);
    Actor_WalkToAndWait(10, 0x1d8, 0x1f8);
    Event_Wait(20);
    Actor_StartRepeatedMotion(10, 1);
    Event_Wait(20);
    Event_ShowMessageAndWait(10, 0, 20);
    Actor_FaceActor(8, 10, 0);
    Actor_SetSpeed(8, 0xcccc, 0x6666);
    Actor_FaceDirection(0, 0x2000, 0);
    Actor_FaceDirection(1, 0x6000, 0);
    Actor_FaceDirection(2, 0x6000, 0);
    Actor_FaceDirection(3, 0x2000, 0);
    Actor_WaitForMove(8);
    Event_Wait(30);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_WalkTo(10, 0x1d8, 0x238);
    Actor_WalkToAndWait(8, 0x1d8, 0x218);
    Actor_RunRepeatedMotion(1, 2);
    Actor_WaitForMove(10);
    Event_ShowMessageAndWait(1, 0, 20);
    Actor_StartRepeatedMotion(8, 2);
    Actor_ShowEmote(8, 0x102, 60);
    Call3_02002ad0((void (*)())Engine_ActorFaceDirection, 10, 0xd000, 0);
    Actor_FaceDirection(8, 0xd000, 20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_StartRepeatedMotion(0, 2);
    Actor_StartRepeatedMotion(1, 2);
    Actor_StartRepeatedMotion(2, 2);
    Actor_RunRepeatedMotion(3, 2);
    Actor_WalkToAndWait(8, 0x1d8, 0x200);
    Actor_FaceDirection(8, 0, 0);
    Actor_FaceDirection(0, 0, 0);
    Actor_FaceDirection(2, 0x8000, 0);
    Actor_FaceDirection(9, 0xd000, 20);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_SetAnimationAndWait(2, 3);
    Event_Wait(20);
    /* Clear bit 0 of the flag byte at +90. */
    Scene_GetRecord_4_02002778(0)[90] &= 0xfe;
    Actor_WalkToAndWait(0, 0x1c0, 0x200);
    Event_Wait(1);
    {
        /* Set bit 0 of the flag byte at +90. */
        u8 *record = Actor_Get(0);
        u8 value = record[90] | 1;

        record[90] = value;
    }
    Event_Wait(20);
    FieldScene_RunParticleRain();
    Event_Wait(60);
    Func_02007e60(2, 144);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimation(2, 3);
    Actor_SetAnimationAndWait(3, 3);
    Event_Wait(20);
    Actor_FaceDirection(0, 0x2000, 0);
    Actor_FaceDirection(1, 0x6000, 0);
    Actor_FaceDirection(2, 0x6000, 0);
    Actor_FaceDirection(3, 0x2000, 0);
    Actor_FaceDirection(9, 0x3000, 0);
    Event_Wait(20);
    Actor_WalkToAndWait(8, 0x1d8, 0x228);
    Actor_RunRepeatedMotion(8, 2);
    Event_Wait(20);
    Actor_FaceDirection(8, 0xd000, 20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_SetSpeed(8, 0x8000, 0x4000);
    Actor_WalkToAndWait(8, 0x1e0, 0x21c);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_SetSpeed(8, 0xcccc, 0x6666);
    Actor_SetSpeed(9, 0xcccc, 0x6666);
    Actor_WalkTo(8, 0x1d8, 0x260);
    Actor_WalkTo(9, 0x1d8, 0x220);
    Actor_WalkToAndWait(10, 0x1d8, 0x260);
    Actor_WaitForMove(9);
    Actor_WalkTo(9, 0x1d8, 0x260);
    Actor_SetPosition(10, 0, 0);
    Actor_WaitForMove(8);
    Actor_SetPosition(8, 0, 0);
    Actor_WaitForMove(9);
    Actor_SetPosition(9, 0, 0);
    Actor_WalkToAndWait(2, 0x1e8, 0x208);
    Actor_ShowEmote(2, 0x101, 60);
    Actor_RunRepeatedMotion(3, 1);
    Event_Wait(20);
    Actor_FaceDirection(3, 0x2000, 20);
    Event_ShowMessageAndWait(3, 0, 20);
    Actor_FaceDirection(0, 0, 0);
    Actor_FaceDirection(2, 0xa000, 20);
    Actor_SetAnimationAndWait(2, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(2, 0, 20);
    Actor_ShowEmote(3, 0x105, 60);
    Actor_RunRepeatedMotion(1, 1);
    Actor_FaceDirection(1, 0x6000, 20);
    Event_AskYesNo(1, 0);
    Actor_SetAnimationAndWait(0, 3);
    Event_Wait(20);
    Actor_RunRepeatedMotion(3, 2);
    Event_Wait(20);
    Actor_FaceDirection(0, 0xc000, 0);
    Actor_FaceDirection(1, 0x8000, 0);
    Event_ShowMessageAndWait(3, 0, 20);
    Actor_ShowEmote(0, 0x101, 0);
    Actor_ShowEmote(1, 0x101, 60);
    Actor_SetAnimationAndWait(3, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(3, 0, 20);
    Actor_ShowEmote(0, 0x102, 0);
    Actor_ShowEmote(1, 0x102, 60);
    Event_OpenMessage(3, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(20);
        Actor_SetAnimationAndWait(1, 3);
        Event_Wait(20);
        Event_ShowMessageAndWait(1, 0, 20);
        /* Same step counter as bump_step(), incremented inline here. */
        *(u16 *)((*(u8 **)0x03001ebc + 0x1d8)) += 1;
    } else {
        Event_Wait(20);
        /* Same step counter as bump_step(), incremented inline here. */
        *(u16 *)((*(u8 **)0x03001ebc + 0x1d8)) += 1;
        Actor_SetAnimationAndWait(3, 3);
        Event_Wait(20);
        Event_ShowMessageAndWait(3, 0, 20);
    }
    Actor_RunRepeatedMotion(1, 1);
    Event_ShowMessageAndWait(1, 0, 20);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimationAndWait(3, 3);
    Event_Wait(20);
    Actor_WalkTo(1, 0x1c0, 0x200);
    Actor_WalkToAndWait(3, 0x1c0, 0x200);
    Actor_SetPosition(3, 0, 0);
    Actor_WaitForMove(1);
    Actor_SetPosition(1, 0, 0);
    Event_Wait(20);
    Actor_FaceActor(0, 2, 20);
    Event_Wait(30);
    Actor_FaceActor(2, 0, 20);
    Event_ShowMessageAndWait(2, 0, 20);
    Actor_ShowEmote(0, 0x102, 60);
    Actor_StartRepeatedMotion(2, 2);
    Actor_SetAttachedEffect(2, 0x102);
    Event_Wait(60);
    Actor_SetAnimationAndWait(2, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(2, 0, 20);
    Actor_WalkToAndWait(2, 0x1c0, 0x200);
    Actor_SetPosition(2, 0, 0);
    Event_End();
    GameFlag_Set(0x895);
}

void FieldScene_SetFlag140AndFinishSequence(s32 arg0, s32 arg1)
{
    u8 *globalCtx;

    GameFlag_Set(160 << 1);
    Psynergy_Begin(141, 1);
    globalCtx = *(u8 **)0x03001f30;
    Psynergy_SetTarget(arg0, arg1);
    globalCtx[0x23] = 0;
    Psynergy_RaiseHands();
    Psynergy_PlayEffect(1);
    Task_Wait(1);
}

void FieldScene_FinishSequence(void)
{
    Actor_SetAnimation(0, 1);
    Psynergy_PlayEffect(2);
    Psynergy_LowerHands();
}

void FieldScene_SpawnEightShots(void)
{
    struct Descriptor_02000484 descriptor;
    u8 *record;
    u32 i;

    record = Actor_Get(8);
    descriptor.field0 = 1;
    descriptor.field24 = 0x0119;
    descriptor.field28 = 0x0200d1d8;
    descriptor.field16 = 224 << 10;
    descriptor.field20 = 192 << 9;
    for (i = 0; i <= 7; i++) {
        Event_Wait(10);
        if (i & 1) {
            Audio_PlayCue(0x82);
        }
        Func_020042d6(*(s32 *)(record + 8), *(s32 *)(record + 12),
                      *(s32 *)(record + 16) + 0xffe80000, 0,
                      0x9999, 0, 0x00360001, (u8 *)&descriptor);
    }
    Event_Wait(60);
}

void FieldScene_SelectActorModeFromInputBit(s32 arg0)
{
    if ((*(u32 *)0x03001e40 >> 1) & 1) {
        Object_SetPalette(arg0, 10);
    } else {
        Object_SetPalette(arg0, 9);
    }
}

void FieldScene_RunParticleRain(void)
{
    struct Descriptor_020041ec descriptor;
    u8 *record;
    u32 i;
    s32 x;
    s32 y;
    s32 scale;

    Audio_PlayCue(0x83);
    *(u32 *)(Func_02008606(8) + 108) = 0x0200c1c5;
    Event_Wait(40);
    ColorBuffer_ApplySource(128 << 9, 0);
    ColorBuffer_ApplyTarget(0x205c54, 1);
    ColorBuffer_Interpolate(60);
    Event_Wait(40);
    Audio_PlayCue(0x83);
    *(u32 *)(Func_0200863a(2) + 108) = 0x0200c1c5;
    Event_Wait(120);
    record = Actor_Get(8);
    descriptor.field0 = 1;
    descriptor.field4 = 2;
    descriptor.field24 = 0x011d;
    for (i = 0; i <= 63; i++) {
        if ((i & 3) == 0) {
            Audio_PlayCue(246);
        }
        x = *(s32 *)(record + 8)
            + ((((u32)(Random_Next() * 3) << 4) >> 16) << 16)
            + 0xfff40000;
        y = *(s32 *)(record + 12)
            + ((((u32)Random_Next() << 5) >> 16) << 16)
            + 0xfff00000;
        scale = (((u32)((u32)Random_Next() << 2) >> 16) << 15) + (128 << 8);
        Func_020043fe(x, y, *(s32 *)(record + 16), 0,
                      scale, 0, 152 << 13, (u8 *)&descriptor);
        Task_Wait(2);
    }
    Audio_PlayCue(220);
    Event_Wait(30);
    ColorBuffer_ApplyTarget(128 << 9, 1);
    ColorBuffer_Interpolate(60);
    Event_Wait(40);
    *(u32 *)(Func_020086fa(8) + 108) = 0;
    *(u32 *)(Func_02008704(2) + 108) = 0;
    Actor_SetChildValue(8, 0);
    Actor_SetChildValue(2, 0);
}
