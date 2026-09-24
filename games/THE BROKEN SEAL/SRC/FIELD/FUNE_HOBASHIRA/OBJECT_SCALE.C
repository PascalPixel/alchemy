#include "TYPES.H"
#include "FIELD_EVENT.H"

#define Scene_GetRecord_1(args...) Func_020019fc(args)
#define Object_NotifyLastActiveOfEvent_1(a0) Call1(Func_020019fe, a0)
#define Scene_GetRecord_2(args...) Func_02001a6c(args)
#define Object_SetPosition_1(a0, a1, a2, a3) Call4(Func_02001a5e, a0, a1, a2, a3)
#define Object_CommitPosition_1(args...) Func_02001a6c_a(args)
#define Object_SetPosition_2(a0, a1, a2, a3) Call4(Func_02001a74, a0, a1, a2, a3)
#define Object_CommitPosition_2(args...) Func_02001a82(args)
#define Object_SetPosition_3(a0, a1, a2, a3) Call4(Func_02001a8a, a0, a1, a2, a3)
#define Object_CommitPosition_3(args...) Func_02001a98(args)
#define Object_SetPosition_4(a0, a1, a2, a3) Call4(Func_02001aa0, a0, a1, a2, a3)
#define Object_SetPosition_5(a0, a1, a2, a3) Call4(Func_02001b24, a0, a1, a2, a3)
#define Object_CommitPosition_4(args...) Func_02001b32(args)
#define Object_SetPosition_6(a0, a1, a2, a3) Call4(Func_02001b48, a0, a1, a2, a3)
#define Object_CommitPosition_5(args...) Func_02001b56(args)
#define Object_SetPosition_7(a0, a1, a2, a3) Call4(Func_02001b5e, a0, a1, a2, a3)
#define Object_CommitPosition_6(args...) Func_02001b6c_a(args)
#define Object_SetPosition_8(a0, a1, a2, a3) Call4(Func_02001b74, a0, a1, a2, a3)
#define Object_CommitPosition_7(args...) Func_02001b82(args)
#define Object_SetPosition_9(a0, a1, a2, a3) Call4(Func_02001b8a_a, a0, a1, a2, a3)
#define Object_CommitPosition_8(args...) Func_02001b98(args)
#define Object_SetPosition_10(a0, a1, a2, a3) Call4(Func_02001ba0, a0, a1, a2, a3)
#define SCENE_PHASE (*(s32 *)(*(u8 **)0x03001ebc + 0x1c0))
#define RECORD_S32(rec, off) (*(s32 *)((rec) + (off)))
#define Scene_GetRecord_1_02000af8(args...) Func_02001d3e(args)
#define Object_NotifyLastActiveOfEvent_1_02000af8(a0) Call1(Func_02001d40, a0)
#define Object_NotifyLastActiveOfEvent_2(a0) Call1(Func_02001d4c, a0)
#define Scene_GetRecord_2_02000af8(args...) Func_02001efc(args)
#define Scene_GetRecord_3(args...) Func_02001f98(args)
#define Scene_GetRecord_4(args...) Func_02002042(args)
#define FieldScene_RunSevenActorEnsemble Func_02000e78

extern u8 Data_020095c0[];
extern u8 Data_02009680[];
extern u8 Data_020096a0[];
extern u8 Data_020096c4[];
extern u8 Data_0200988c[];
extern u8 Value_00000923;
extern u8 Value_00000922;
extern u8 Value_00000924;
extern s32 Data_020093a4[];
extern s32 Data_020098f8[];
extern u16 Data_02000240[];
extern u8 Value_0000006f;

void Func_020014d0(void);
void Func_02000908();
s32 Func_020016f8();
s32 Func_020016ba();
void Func_020019fe();
void Func_02001a5e();
void Func_02001a74();
void Func_02001a82();
void Func_02001a8a();
void Func_02001a98();
void Func_02001aa0();
void Func_02001b24();
void Func_02001b32();
void Func_02001b48();
void Func_02001b56();
void Func_02001b5e();
void Func_02001b6c_a();
void Func_02001b74();
void Func_02001b82();
void Func_02001b8a_a();
void Func_02001b98();
void Func_02001ba0();
u8 *Func_020019fc();
u8 *Func_02001a6c();
void Func_02001a6c_a();
u8 *Func_02001cbe(void);
u32 Func_02001c64(void);
u32 Func_02001c70_a(void);
void Func_02001d40();
void Func_02001d4c();
s32 Func_02001f98();
s32 Func_02002042();
void Func_020020ba();
s32 Func_020020be();

/* AUDITED GENERATED CALL SCRIPT for FieldScene_RunSevenActorEnsemble:
 * Seven actor setup and motion sequences, two scene-workspace state writes,
 * the closing map-state publication, and the final selector branch. */

/*
 * Scripted per-object step for resource_3b0.  It turns the object to one of two
 * fixed headings when a sampled value picks them out, then reports finished.
 */

/* Old-style declarations: overlay imports vary in arity between call sites. */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */

/* Phase/status word at 0x1c0 of the shared scene work record. */

/* A record's four-byte field at the given byte offset. Used below for the
 * seven fields that get the same reset pattern twice, once per record. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call3_020004b0(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call3_020007b0(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    u8 *Func_02001d3e();
    u8 *Func_02001efc();

    f(a0, a1, a2);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    s32 Func_02001d3e();
    s32 Func_02001efc();

    f(a0, a1);
}

static __inline__ void Call3_02000af8(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    s32 Func_02001d3e();
    s32 Func_02001efc();

    f(a0, a1, a2);
}

static __inline__ void Call2_02000e78(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3_02000e78(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/*
 * The shift pair is a windowed extraction of bits 10..15 of the sample, giving
 * a value in 0..63; a plain right shift would let larger values reach the
 * comparisons.  The headings 0xd000 and 0xb000 are built from an immediate and
 * a shift, and the local in each arm is what forces that.  The halfword at
 * object + 6 is the facing angle.  What Engine_RandomNext samples is not
 * established here.
 */
s32 SceneActor_SetFacingFromSample(u8 *object)
{
    u32 sample = (u32)(Random_Next() << 6) >> 16;   /* bits 10..15 */

    if (sample == 6) {
        s32 value = 0xd000;

        *(u16 *)(object + 6) = value;
    } else if (sample == 9) {
        s32 value = 0xb000;

        *(u16 *)(object + 6) = value;
    }

    return 1;
}

void OverlayObject_Add160ToFields18And1c(u8 *o)
{
    if (*(s32 *)(o + 24) < 0x10000) {
        *(s32 *)(o + 24) += 160;
        *(s32 *)(o + 28) += 160;
    }
}

s32 OverlayObject_ShrinkScaleThenStop(u8 *o)
{
    u8 *t = *(u8 **)(o + 0x50);

    t[9] |= 12;
    *(s32 *)(o + 48) = 0x20000;
    *(s32 *)(o + 52) = 0x10000;
    if (*(s32 *)(o + 24) > 0x1000) {
        *(s32 *)(o + 24) += 0xFFFFFC00;
        *(s32 *)(o + 28) += 0xFFFFFC00;
    } else {
        *(s32 *)(o + 8) = 0;
        *(s32 *)(o + 12) = 0;
        *(s32 *)(o + 16) = 0;
        *(s32 *)(o + 36) = 0;
        *(s32 *)(o + 40) = 0;
        *(s32 *)(o + 44) = 0;
    }
    return 1;
}

u8 *SceneData_GetTable95c0(void)
{
    return Data_020095c0;
}

u8 *SceneData_GetTable9680(void)
{
    return Data_02009680;
}

u8 *SceneData_GetTable96a0(void)
{
    return Data_020096a0;
}

u8 *SceneData_GetTable96C4(void)
{
    return Data_020096c4;
}

u8 *SceneData_GetTable988c(void)
{
    return Data_0200988c;
}

void FieldScene_CallHelper14d0(void)
{
    Func_020014d0();
}

void FieldScene_RunActor232SceneWhenFlag923Or922(void)
{
    if (GameFlag_IsSet((s32)&Value_00000923) != 0 || GameFlag_IsSet((s32)&Value_00000922) != 0) {
        Event_Begin();
        Item_ShowFound(0xE8, 3);
        Party_GiveItem(0xE8, 0);
        GameFlag_Set((s32)&Value_00000924);
        Event_End();
    }
}

void FieldScene_RunScene3b0_0200040c(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Task_Wait(1);
    *(u8 *)(Func_020016f8() + 85) = 0;
    Call3(Engine_CameraMoveTo, 0xa40000, 0x400000, 0x1410000);
    Map_Redraw();
    Task_Wait(1);
    Actor_SetPosition(0, 0, 0);
    Func_02000908();
    Event_End();
}

void FieldScene_RunScene3b0_02000468(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Actor_SetPosition(0, 0xa40000, 0x1410000);
    Actor_SetChildValue(0, 15);
    record = Func_020016ba(0);
    Actor_SetSpriteFlags(record, 0);
    Task_Wait(1);
    Map_Redraw();
    Task_Wait(1);
    FieldScene_RunScene3b0_020004b0();
    Event_End();
}

void FieldScene_RunScene3b0_020004b0(void)
{
    SCENE_PHASE = 0x202;
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(20);
    Actor_SetSpeed(8, 0x10000, 0x8000);
    Actor_WalkToAndWait(8, 164, 0x141);
    Actor_FaceDirection(8, 0xd000, 40);
    Actor_FaceDirection(8, 0xb000, 40);
    Actor_FaceDirection(8, 0xd000, 40);
    Actor_FaceDirection(8, 0x3000, 10);
    Actor_WalkToAndWait(8, 164, 0x14e);
    Actor_Jump(8, 4, 40);
    Actor_StartRepeatedMotion(8, 2);
    Event_SetMessage(0x1e3a);
    Event_ShowMessageAndWait(8, 0, 20);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(10);
}

/* Runs the record-8/record-9 pair through two near-identical setup-then-move
 * cycles (position waypoints, a movement flag reset, then animation/sound
 * calls), followed by a shorter closing cycle for record 8 alone. */
void FieldScene_RunActorNinePresentationCycles(void)
{
    u8 *Func_02001d3e();
    u8 *Func_02001efc();

    u32 i;
    u8 *rec9;
    u8 *record;

    Event_Begin();
    Actor_SetChildValue(0, 15);
    record = Scene_GetRecord_1(0);
    Actor_SetSpriteFlags(record, 0);
    Task_Wait(1);
    Object_NotifyLastActiveOfEvent_1(0x200976c);
    Task_Wait(1);
    OverlayObject_InitWithRandomFields(9);
    OverlayObject_InitWithRandomFields(10);
    OverlayObject_InitWithRandomFields(11);
    OverlayObject_InitWithRandomFields(12);
    OverlayObject_InitWithRandomFields(13);
    OverlayObject_InitWithRandomFields(14);
    OverlayObject_InitWithRandomFields(15);
    Engine_ActorEnableActionCallback(8, 0x200939c);
    SCENE_PHASE = 0x203;
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(120);
    rec9 = Scene_GetRecord_2(9);
    Actor_Stop(9);
    /* Reset record 9's waypoint/velocity fields: three fields to the
     * minimum s32, then four fields to zero. */
    RECORD_S32(rec9, 56) = -0x80000000;
    RECORD_S32(rec9, 60) = -0x80000000;
    RECORD_S32(rec9, 64) = -0x80000000;
    RECORD_S32(rec9, 36) = 0;
    RECORD_S32(rec9, 40) = 0;
    RECORD_S32(rec9, 44) = 0;
    RECORD_S32(rec9, 76) = 0;
    Event_Wait(20);
    Actor_SetSpeed(9, 0x80000, 0x40000);
    Object_SetPosition_1(rec9, 0xa40000, 0x900000, 0x1410000);
    Object_CommitPosition_1(rec9);
    Object_SetPosition_2(rec9, 0xa40000, 0x680000, 0x1410000);
    Object_CommitPosition_2(rec9);
    Object_SetPosition_3(rec9, 0xcc0000, 0x7c0000, 0x1410000);
    Object_CommitPosition_3(rec9);
    Object_SetPosition_4(rec9, 0x900000, 0, 0xa90000);
    Actor_Stop(8);
    Task_Wait(1);
    Actor_FaceDirection(8, 0x8000, 0);
    Actor_ShowEmote(8, 0x103, 60);
    Actor_SetSpeed(9, 0x20000, 0x10000);
    OverlayObject_InitWithRandomFields(9);
    Event_Wait(20);
    Engine_ActorEnableActionCallback(8, 0x200939c);
    Event_Wait(120);
    Actor_Stop(9);
    /* Same reset pattern on record 9 for the second cycle. */
    RECORD_S32(rec9, 56) = -0x80000000;
    RECORD_S32(rec9, 60) = -0x80000000;
    RECORD_S32(rec9, 64) = -0x80000000;
    RECORD_S32(rec9, 36) = 0;
    RECORD_S32(rec9, 40) = 0;
    RECORD_S32(rec9, 44) = 0;
    RECORD_S32(rec9, 76) = 0;
    Event_Wait(20);
    Actor_SetSpeed(9, 0x80000, 0x40000);
    Object_SetPosition_5(rec9, 0xa40000, 0x900000, 0x1410000);
    Object_CommitPosition_4(rec9);
    Actor_SetSpeed(9, 0x50000, 0x28000);
    Object_SetPosition_6(rec9, 0xa40000, 0x680000, 0x1410000);
    Object_CommitPosition_5(rec9);
    Object_SetPosition_7(rec9, 0xa40000, 0x720000, 0x1410000);
    Object_CommitPosition_6(rec9);
    Object_SetPosition_8(rec9, 0xa40000, 0x680000, 0x1410000);
    Object_CommitPosition_7(rec9);
    Object_SetPosition_9(rec9, 0xcc0000, 0x7c0000, 0x1410000);
    Object_CommitPosition_8(rec9);
    Object_SetPosition_10(rec9, 0x900000, 0, 0xa90000);
    Actor_Stop(8);
    Task_Wait(1);
    Actor_FaceDirection(8, 0x8000, 0);
    Actor_ShowEmote(8, 0x103, 60);
    Actor_SetSpeed(9, 0x20000, 0x10000);
    OverlayObject_InitWithRandomFields(9);
    Actor_Jump(8, 4, 20);
    Actor_Jump(8, 6, 40);
    Audio_PlayCue(29);
    GameFlag_Set(0x8f0);
    Event_SetMessage(0x1e49);
    Event_ShowMessageAndWait(16, 0, 20);
    Actor_ShowEmote(8, 0x100, 0);
    Actor_WalkToAndWait(8, 164, 0x158);
    Event_Wait(40);
    Actor_RunRepeatedMotion(8, 2);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(12);
    Event_End();
}

void OverlayObject_InitWithRandomFields(s32 a)
{
    u8 *obj;
    u32 x;

    obj = Func_02001cbe();
    Actor_SetSpritePriority(a, 1);
    obj[0x55] = 0;
    *(u16 *)(obj + 0x64) = Func_02001c64() >> 15;
    *(u16 *)(obj + 0x66) = Func_02001c70_a() >> 15;
    x = Random_Next();
    x <<= 2;
    x >>= 16;
    x <<= 16;
    x += 0x60000;
    *(s32 *)(obj + 0xc) = x;
    x = Random_Next();
    *(s32 *)(obj + 0x4c) = ((x * 3 << 13) >> 16) - 0x3000;
    *(s32 *)(obj + 0x18) = 0x14000;
    *(s32 *)(obj + 0x1c) = 0x14000;
    Actor_EnableActionCallback(a, Data_020093a4);
}

/* Drives ids 8 through 18 through position, scale, and flag updates in
 * sequence, stepping the shared scene phase at the start and end. */
void FieldScene_RunPrimarySequence(void)
{
    s32 Func_02001d3e();
    s32 Func_02001efc();

    u32 i;
    s32 rec;
    s32 id0_state;

    Event_Begin();
    Actor_SetChildValue(0, 15);
    id0_state = Scene_GetRecord_1_02000af8(0);
    Actor_SetSpriteFlags(id0_state, 0);
    Task_Wait(1);
    Object_NotifyLastActiveOfEvent_1_02000af8(0x200976c);
    Task_Wait(1);
    Object_NotifyLastActiveOfEvent_2(0x2009844);
    Task_Wait(1);
    OverlayObject_InitWithRandomFields(9);
    OverlayObject_InitWithRandomFields(10);
    OverlayObject_InitWithRandomFields(11);
    OverlayObject_InitWithRandomFields(12);
    OverlayObject_InitWithRandomFields(13);
    OverlayObject_InitWithRandomFields(14);
    OverlayObject_InitWithRandomFields(15);
    Actor_EnableActionCallback(8, 0x200939c);
    SCENE_PHASE = 0x203;
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(0x12c);
    Audio_PlayCue(147);
    Event_Wait(100);
    Actor_Stop(9);
    Actor_Stop(10);
    Actor_Stop(11);
    Actor_Stop(12);
    Actor_Stop(13);
    Actor_Stop(14);
    Actor_Stop(15);
    Actor_SetSpeed(9, 0x30000, 0x18000);
    Actor_SetSpeed(10, 0x30000, 0x18000);
    Actor_SetSpeed(11, 0x30000, 0x18000);
    Actor_SetSpeed(12, 0x30000, 0x18000);
    Actor_SetSpeed(13, 0x30000, 0x18000);
    Actor_SetSpeed(14, 0x30000, 0x18000);
    Actor_SetSpeed(15, 0x30000, 0x18000);
    Actor_SetDestination(9, 0, 100);
    Actor_SetDestination(10, 60, 100);
    Actor_SetDestination(11, 120, 100);
    Actor_SetDestination(12, 180, 100);
    Actor_SetDestination(13, 240, 100);
    Actor_SetDestination(14, 0x140, 100);
    Actor_SetDestination(15, 0x17c, 100);
    Event_Wait(40);
    Actor_ShowEmote(8, 0x101, 0);
    Event_Wait(20);
    Actor_SetPosition(9, 0, 0);
    Actor_SetPosition(10, 0, 0);
    Actor_SetPosition(11, 0, 0);
    Actor_SetPosition(12, 0, 0);
    Actor_SetPosition(13, 0, 0);
    Actor_SetPosition(14, 0, 0);
    Actor_SetPosition(15, 0, 0);
    Event_Wait(100);
    rec = Scene_GetRecord_2_02000af8(18);
    *(s32 *)(rec + 24) = 0x1999;
    *(s32 *)(rec + 28) = 0x1999;
    Actor_SetPosition(18, 0xac0000, 0x1540000);
    Actor_Stop(8);
    Task_Wait(1);
    Actor_RunRepeatedMotion(8, 1);
    Actor_FaceDirection(8, 0x3000, 0);
    Audio_PlayCue(29);
    GameFlag_Set(0x8f0);
    for (i = 0; i < 32; i++) {
        *(s32 *)(rec + 24) += 0xccc;
        *(s32 *)(rec + 28) += 0xccc;
        Task_Wait(1);
    }
    Actor_ShowEmote(8, 0x101, 60);
    Actor_RunRepeatedMotion(8, 2);
    Actor_WalkToAndWait(8, 168, 0x154);
    Actor_WalkToAndWait(8, 200, 0x154);
    Actor_FaceDirection(8, 0x8000, 0);
    rec = Scene_GetRecord_3(17);
    *(s32 *)(rec + 24) = 0x12666;
    *(s32 *)(rec + 28) = 0x12666;
    *(s32 *)(rec + 8) = 0xac0000;
    *(s32 *)(rec + 12) = 0xa00000;
    *(s32 *)(rec + 16) = 0x1540000;
    {
        /* Clear the flag word at +6. */
        s32 shown = 0;

        *(u16 *)(rec + 6) = shown;
    }
    *(s32 *)(rec + 68) = 0x6666;
    *(s32 *)(rec + 72) = 0x30000;
    Event_Wait(20);
    Actor_Jump(8, 6, 20);
    Audio_PlayCue(147);
    Event_Wait(20);
    Actor_EnableActionCallback(8, 0x20093ac);
    Event_Wait(80);
    Actor_SetSpritePriority(17, 1);
    Actor_SetSpeed(17, 0x10000, 0x8000);
    *(s32 *)(rec + 68) = 0x1999;
    *(s32 *)(rec + 72) = 0xb333;
    Audio_PlayCue(153);
    *(s32 *)(rec + 40) = 0x80000;
    Actor_SetDestination(17, 132, 0x168);
    Actor_SetDestination(18, 132, 0x168);
    Event_Wait(40);
    Actor_SetPosition(17, 0, 0);
    rec = Scene_GetRecord_4(8);
    *(s32 *)(rec + 24) = 0x10000;
    *(s32 *)(rec + 28) = 0x10000;
    {
        /* Set the flag word at +6. */
        s32 shown = 0x5000;

        *(u16 *)(rec + 6) = shown;
    }
    Event_Wait(40);
    SCENE_PHASE = 0x202;
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(13);
    Event_End();
}

void Func_02000e78(void)
{
    s32 ensemble;
    s32 selector;

    Event_Begin();
    Actor_SetChildValue(0, 15);
    ensemble = Func_020020be(0);
    Actor_SetSpriteFlags(ensemble, 0);
    Call1(Func_020020ba, 33593196);
    Task_Wait(1);
    OverlayObject_InitWithRandomFields(9);
    OverlayObject_InitWithRandomFields(10);
    OverlayObject_InitWithRandomFields(11);
    OverlayObject_InitWithRandomFields(12);
    OverlayObject_InitWithRandomFields(13);
    OverlayObject_InitWithRandomFields(14);
    OverlayObject_InitWithRandomFields(15);
    Actor_EnableActionCallback(8, 33592220);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 3);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(400);
    Actor_Stop(9);
    Actor_Stop(10);
    Actor_Stop(11);
    Actor_Stop(12);
    Actor_Stop(13);
    Actor_Stop(14);
    Actor_Stop(15);
    Actor_SetSpeed(9, 196608, 98304);
    Actor_SetSpeed(10, 196608, 98304);
    Actor_SetSpeed(11, 196608, 98304);
    Actor_SetSpeed(12, 196608, 98304);
    Actor_SetSpeed(13, 196608, 98304);
    Actor_SetSpeed(14, 196608, 98304);
    Actor_SetSpeed(15, 196608, 98304);
    Value2(Engine_ActorEnableActionCallback, 9, 33592400);
    Value2(Engine_ActorEnableActionCallback, 10, 33592448);
    Value2(Engine_ActorEnableActionCallback, 11, 33592496);
    Value2(Engine_ActorEnableActionCallback, 12, 33592544);
    Value2(Engine_ActorEnableActionCallback, 13, 33592592);
    Value2(Engine_ActorEnableActionCallback, 14, 33592640);
    Value2(Engine_ActorEnableActionCallback, 15, 33592688);
    Event_Wait(40);
    Actor_StartRepeatedMotion(8, 3);
    Actor_SetAttachedEffect(8, 258);
    Event_Wait(120);
    Actor_StartRepeatedMotion(8, 1);
    Actor_ShowEmote(8, 256, 60);
    Actor_SetSpeed(8, 65536, 32768);
    Actor_WalkToAndWait(8, 164, 344);
    Actor_Jump(8, 4, 10);
    Actor_Jump(8, 6, 20);
    Event_SetMessage(7908);
    Event_ShowMessageAndWait(8, 0, 20);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
    Event_CloseScreen();
    Event_WaitForScreen();
    Data_02000240[226] = (s32)&Value_0000006f;
    Data_02000240[227] = 2;
    selector = SceneData_GetDifferenceOfPairSums();
    if (selector == 11) {
        Event_RequestExit(15);
    } else {
        Event_RequestExit(14);
    }
    Event_End();
}

s32 SceneData_GetDifferenceOfPairSums(void)
{
    s32 a;
    s32 b;

    a = SceneData_GetValueByFirstSetFlag(0);
    a += SceneData_GetValueByFirstSetFlag(2);
    b = SceneData_GetValueByFirstSetFlag(1);
    b += SceneData_GetValueByFirstSetFlag(3);
    return a - b;
}

s32 SceneData_GetValueByFirstSetFlag(u32 a)
{
    s32 base = 0;
    u32 i;

    switch (a) {
    case 0:
        base = 0x92c;
        break;
    case 1:
        base = 0x935;
        break;
    case 2:
        base = 0x917;
        break;
    case 3:
        base = 0x990;
        break;
    }
    for (i = 0; i <= 8; i++) {
        if (GameFlag_IsSet(base + i) != 0)
            return Data_020098f8[i];
    }
    return 0;
}
