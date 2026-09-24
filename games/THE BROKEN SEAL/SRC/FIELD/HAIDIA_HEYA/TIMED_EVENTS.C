/*
 * A run of unnamed leaf owners in resource_376: scene data getters, actor
 * dialogue beats, numbered scene steps, and the long presentation
 * sequence.
 */

#include "TYPES.H"

#define NULL ((void *)0)

/*
 * Each Func_ symbol names the pre-relocation call word the image holds,
 * not a runtime address.
 */
#define CalculateFacingAngle Func_020011b6

#include "FACING_OBJECT.H"
#include "FIELD_EVENT.H"

typedef struct {
    u8 pad_to_position[8];
    s32 x;
    s32 y;
    u8 pad_to_scale[8];
    s32 scale_x;
    s32 scale_y;
    u8 pad_to_velocity[0x44];
    s16 velocity_x;
    s16 velocity_y;
} ScaledMotion;

union SceneActor {
    s32 words[26];
    u16 halfwords[52];
};

/*
 * The fields at 100 and 102 are consecutive halfwords, so this byte and
 * halfword view needs two-byte layout rather than the default four.
 */
union SceneField {
    u16 value;
    u8 bytes[2];
} __attribute__((packed, aligned(2)));

extern u8 Data_020098b8[];
extern u8 Data_02009738[];
extern u8 Data_020095b8[];
extern u8 Data_000011a4[];
extern u8 Data_00001c40[];
extern u8 Data_02000240[];
extern s16 Data_02000240_t[][1];
extern u8 Data_020092fc[];
extern u8 Data_02009400[];
extern u32 Data_03001e40;
extern u8 Data_02009440[];

s32 Func_020011b6(s32, s32);
void Func_020012cc(void *);
void Func_0200144c();
void Func_02001588(void);
void Func_020005ca(int value);
void Func_020005d6(int value);
void Func_020005e2(int value);
void Func_020005ee(int value);
void Func_020005fa(int value);
void Func_02000606(int value);
void Func_02000612(int value);
void Func_0200061e(int value);
void Func_02000aba();
void Func_02001620();
u8 *Func_02001664();
void Func_020016ce();
void Func_020016ec();
void Func_02001706_b();
u8 *Func_020016a6(s32 actor_id);
void Func_02000808();
void Func_02001864();
s32 Func_0200188a();
s32 Func_02001896();
s32 Func_020018a2();
s32 Func_020018ae();
s32 Func_020018ba();
s32 Func_020018c4();
void Func_020019fe_b();
void Func_02001a08();
void Func_02001a12_b();
void Func_02001a1c_a();
s32 Func_02001a90();
void Func_02001c7a();
s32 Func_02001ffe();
s32 Func_02002062();
s32 Func_02002248();
void Func_020090c1();
union SceneActor *Func_020022b0(s32);
s32 *Func_020022bc(s32);

/*
 * Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the
 * block. A value-returning call sets r0 last of its arguments, so a callee
 * is declared to return a value here even where the result is unused.
 */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];

    f(a0, a1);
}

/* Advance the scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];

    return f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    extern u8 Data_03001ebc[];
    void Camera_MoveTo();

    f(a0, a1, a2, a3);
}

static __inline__ void Call11(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8, s32 a9, s32 a10)
{
    extern u8 Data_03001ebc[];
    void Camera_MoveTo();

    f(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
}

s32 OverlayObject_UpdateFacingTowardTarget(struct FacingObject *obj)
{
    s32 delta;
    u16 old;
    s32 angle;
    struct FacingObject *target;

    target = obj->facing_target;
    if (target != NULL) {
        obj->facing_flags = (u8)(0xFE & obj->facing_flags);
        angle = (u16)CalculateFacingAngle(target->position_z - obj->position_z, target->position_x - obj->position_x);
        old = obj->facing;
        delta = (s16)(angle - old);
        if (delta != 0) {
            if (delta > 0x1000) {
                delta = 0x1000;
            }
            /* The loader relocates the stored pool word to -0x1000. */
            if (delta < -0x1000) {
                delta = -0x1000;
            }
            obj->facing = (u16)(old + delta);
        }
    }
    return 1;
}

s32 AdvancePositionScaleAndVelocity(ScaledMotion *motion)
{
    motion->x += motion->velocity_x << 8;
    motion->y += motion->velocity_y << 8;
    motion->scale_x += 0x666;
    motion->scale_y += 0x666;
    motion->velocity_x += 5;
    motion->velocity_y -= 1;
    return 0;
}

/* The eight-byte owner includes its one pool word. */
void *SceneData_GetTable9478(void)
{
    return (void *)0x02009478;
}

/* A four-byte leaf that returns zero. */
int SceneData_ReturnZero(void)
{
    return 0;
}

/* The 36-byte owner includes its three pool words. */
void *SceneData_SelectTable9568ByFlag(void)
{
    if (GameFlag_IsSet(0x834) != 0)
        return (void *)0x02009590;
    return (void *)0x02009568;
}

void *SceneData_SelectFlaggedTable(void)
{
    void *tbl;

    if (GameFlag_IsSet(0x87a)) {
        tbl = Data_020098b8;
    } else if (GameFlag_IsSet(0x815)) {
        tbl = Data_02009738;
    } else {
        tbl = Data_020095b8;
    }
    Func_020012cc(tbl);
    return tbl;
}

/* The 80-byte owner includes its seven pool words. */
void *SceneData_SelectTable9c00ByFlags(void)
{
    if (GameFlag_IsSet(0x834) != 0)
        return (void *)0x02009ac8;
    if (GameFlag_IsSet(0x87a) != 0)
        return (void *)0x02009ffc;
    if (GameFlag_IsSet(0x815) != 0)
        return (void *)0x02009da4;
    return (void *)0x02009c00;
}

/* The 44-byte actor-15 scene owner includes its one pool word. */
void SceneDialogue_RunActor15Message0f6d(void)
{
    Event_Begin();
    Event_SetMessage(0xf6d);
    Actor_FaceEachOther(0, 15, 6);
    Event_AskYesNo(15, 0);
    Event_End();
}

/* The 44-byte actor-19 scene owner includes its one pool word. */
void SceneDialogue_RunActor19Message0f73(void)
{
    Event_Begin();
    Event_SetMessage(0xf73);
    Actor_FaceEachOther(0, 19, 6);
    Event_AskYesNo(19, 0);
    Event_End();
}

void FieldScene_RunScene376_020001e8(void)
{
    s32 callback;
    s32 base5_11a4;

    Event_Begin();
    if (GameFlag_IsSet(0x81b) != 0) {
        Event_SetMessage(0x11a6);
        Event_ShowMessage(20, 0);
        callback = 0x20092fc;
        Call3(Func_0200144c, 20, 0x10000, callback);
    } else {
        base5_11a4 = (s32)Data_000011a4;
        Event_SetMessage(base5_11a4);
        Event_ShowMessageAndWait(20, 0, 20);
        Message_ShowCentered((base5_11a4 + 1), 1);
        Party_GiveItem(180, 0);
        GameFlag_Set(0x81b);
    }
    Event_End();
}

/* The 32-byte actor-16 dialogue owner includes its one pool word. */
void SceneDialogue_RunActor16Message11be(void)
{
    Event_Begin();
    Event_SetMessage(0x11be);
    Event_AskYesNo(16, 0);
    Event_End();
}

/* The 32-byte actor-10 dialogue owner includes its one pool word. */
void SceneDialogue_RunActor10Message1c3d(void)
{
    Event_Begin();
    Event_SetMessage(0x1c3d);
    Event_AskYesNo(10, 0);
    Event_End();
}

void FieldScene_RunScene376_02000298(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Event_Begin();
    Event_SetMessage((s32)Data_00001c40);
    Event_ShowMessage(0x800b, 0);
    Event_End();
}

/* The 76-byte shared numbered-scene owner includes its two pool words. */
void SceneState_SetRuntimeWord448To521AndRun(s32 value)
{
    extern u8 *Data_03001ebc;

    if (GameFlag_IsSet(0x834) != 0)
        Func_02001588();
    Audio_PlayCue(123);
    *(s32 *)(Data_03001ebc + 448) = 521;
    *(s32 *)(Data_03001ebc + 456) = 16;
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(value);
}

/* Eight numbered-scene wrappers follow, each a twelve-byte owner. */
void FieldScene_RunIndexedStep1(void)
{
    Func_020005ca(1);
}

void FieldScene_RunIndexedStep2(void)
{
    Func_020005d6(2);
}

void FieldScene_RunIndexedStep3(void)
{
    Func_020005e2(3);
}

void FieldScene_RunIndexedStep4(void)
{
    Func_020005ee(4);
}

void FieldScene_RunIndexedStep5(void)
{
    Func_020005fa(5);
}

void FieldScene_RunIndexedStep6(void)
{
    Func_02000606(6);
}

void FieldScene_RunIndexedStep7(void)
{
    Func_02000612(7);
}

void FieldScene_RunIndexedStep8(void)
{
    Func_0200061e(8);
}

/*
 * The record the fourth scene work cell (0x03001ec8) points to. The main-image
 * routine at 080949a8 counts the halfword at 0x1f80 down and, while the
 * halfword at 0x1f84 is set, plays cue 172 rather than 171.
 */
struct FlashCueWork {
    u8 unknown_0000[0x1f84];
    s16 alternate_cue;
};

s32 Func_02000368(void)
{
    extern u8 Data_03001ebc[];

    u8 *record;
    u8 **scene = (u8 **)Data_03001ebc;

    *(s32 *)(scene[0] + 0x1c0) = 0x209;
    if (GameFlag_IsSet(0x834) != 0) {
        Actor_SetPosition(8, 0, 0);
        Actor_SetPosition(9, 0, 0);
        Actor_SetPosition(10, 0, 0);
        Actor_SetPosition(11, 0, 0);
        Actor_SetPosition(12, 0, 0);
        Actor_SetPosition(13, 0, 0);
        Actor_SetPosition(14, 0, 0);
        Actor_SetPosition(15, 0, 0);
        ((void (*)())Engine_ActorSetPosition)(16, 0, 0);
        Actor_SetPosition(17, 0, 0);
        Actor_SetPosition(18, 0, 0);
        Actor_SetPosition(19, 0, 0);
        Actor_SetPosition(20, 0, 0);
        Actor_SetPosition(21, 0, 0);
        Actor_SetPosition(22, 0, 0);
        Func_020016ce();
        ((struct FlashCueWork *)scene[3])->alternate_cue = 1;
        Func_020016ec();
        Task_Wait(30);
        Event_OpenScreen();
        Event_WaitForScreen();
        Func_02001706_b();
    }
    if (GameFlag_IsSet(0x87a) != 0) {
        if (Data_02000240_t[225][0] == 6) {
            if (GameFlag_IsSet(0x81d) == 0) {
                Func_02000aba();
            }
        }
        Actor_Get(10)->collision_flags |= 0x80;
    }
    if (Data_02000240_t[225][0] == 2) {
        if (GameFlag_IsSet(0x815) != 0) {
            Actor_SetPosition(13, 0x1c60000, 0x960000);
            record = Func_02001664(13);
            Actor_SetSpriteFlags((s32)record, 0);
            Actor_SetAnimation(13, 5);
            Func_02001620(4);
        }
    }
    return 0;
}

/*
 * Choose what follows from actor zero's directional halfword at +6 and two
 * story flags. The halfword is reduced by 0xa001 modulo 2^32 and compared
 * unsigned with 0x3ffe, so the first arm covers 0xa001..0xdfff; what that
 * range means is not established. The record is fetched before either path
 * is chosen. The 128-byte owner includes its seven pool words.
 */
void FieldScene_RunByActorDirectionAndFlags(void)
{

    u8 *p;
    u32 dir;

    p = Func_020016a6(0);
    dir = *(u16 *)(p + 6);
    dir += 0xffff5fff;

    if (dir <= 0x3ffe) {
        Shop_Open(1, 21);
        return;
    }

    Event_Begin();
    if (GameFlag_IsSet(0x87a) != 0) {
        Func_02001750(0x1c06);
        Event_AskYesNo(21, 0);
    } else {
        if (GameFlag_IsSet(0x815) != 0) {
            Func_0200176a(0x11a2);
        } else {
            Event_SetMessage(0x0f53);
        }
        Event_ShowMessage(21, 0);
    }
    Event_End();
}

void FieldScene_RunScene376_0200055c(void)
{
    extern u8 Data_03001ebc[];

    struct FieldActor *actor;

    actor = Actor_Get(0);
    if ((u32)(actor->facing - 0xa001) <= 0x3ffe) {
        Shop_Open(2, 22);
    } else {
        ((void (*)())Engine_EventBegin)();
        if (GameFlag_IsSet(0x87a) != 0) {
            Event_SetMessage(0x1c09);
        } else {
            if (GameFlag_IsSet(0x815) != 0) {
                Event_SetMessage(0x11a3);
            } else {
                Event_SetMessage(0xf54);
            }
        }
        Event_ShowMessage(22, 0);
        Event_End();
    }
}

void FieldScene_RunScene376_020005d4(void)
{
    extern u8 Data_03001ebc[];

    struct FieldActor *actor;

    actor = Actor_Get(0);
    if ((u32)(actor->facing - 0xa001) <= 0x3ffe) {
        Shop_Open(3, 20);
    } else {
        if (GameFlag_IsSet(0x87a) != 0) {
            Event_Begin();
            Event_SetMessage(0x1c0a);
            Event_ShowMessage(20, 0);
            Event_End();
        } else {
            if (GameFlag_IsSet(0x815) != 0) {
                Func_02000808();
            } else {
                Event_Begin();
                Event_SetMessage(0xf55);
                Event_ShowMessage(20, 0);
                Event_End();
            }
        }
    }
}

void FieldScene_RunLongPresentationSequence(void)
{
    extern u8 Data_03001ebc[];
    void Camera_MoveTo();

    u32 i;
    s32 record;
    struct FieldActor *actor;
    s32 v6;
    s32 base7_20090c1;
    s32 base5_20092fc;
    s32 base5_2009400;

    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Task_Wait(1);
    Actor_SetSpritePriority(3, 1);
    Actor_SetSpeed(0, 0x6666, 0x3333);
    Actor_SetSpeed(1, 0x6666, 0x3333);
    Actor_SetSpeed(2, 0x6666, 0x3333);
    Actor_SetSpeed(3, 0x6666, 0x3333);
    Actor_SetAnimation(8, 5);
    Actor_WalkTo(0, 0x328, 0x1fc);
    record = Func_0200188a(23);
    Actor_SetSpriteFlags(record, 0);
    record = Func_02001896(24);
    Actor_SetSpriteFlags(record, 0);
    record = Func_020018a2(25);
    Actor_SetSpriteFlags(record, 0);
    v6 = 0;
    *(u8 *)(Func_020018ae(23) + 85) = v6;
    *(u8 *)(Func_020018ba(24) + 85) = v6;
    *(u8 *)(Func_020018c4(25) + 85) = v6;
    base7_20090c1 = (s32)Func_020090c1;
    Call2(Func_02001864, base7_20090c1, 0xc80);
    Task_Wait(1);
    *(s32 *)(*(u8 **)Data_03001ebc + 0x1c8) = 32;
    Event_OpenScreen();
    Event_WaitForScreen();
    Actor_WaitForMove(0);
    Actor_SetAnimation(0, 1);
    actor = Actor_Get(0);
    if (actor != NULL) {
        Actor_SetPosition(1, actor->x.fixed, actor->z.fixed);
    }
    actor = Actor_Get(0);
    if (actor != NULL) {
        Actor_SetPosition(2, actor->x.fixed, actor->z.fixed);
    }
    actor = Actor_Get(0);
    if (actor != NULL) {
        Actor_SetPosition(3, actor->x.fixed, actor->z.fixed);
    }
    Actor_WalkTo(1, 0x318, 0x200);
    Actor_WalkTo(2, 0x338, 0x1f8);
    Actor_WalkToAndWait(3, 0x332, 0x20c);
    Actor_SetAnimation(1, 1);
    Actor_SetAnimation(2, 1);
    Event_Wait(10);
    base5_20092fc = (s32)Data_020092fc;
    Call3(Func_020019fe_b, 0, 0x1000a, base5_20092fc);
    Call3(Func_02001a08, 1, 0x1000a, base5_20092fc);
    Call3(Func_02001a12_b, 2, 0x1000a, base5_20092fc);
    Call3(Func_02001a1c_a, 3, 0x1000a, base5_20092fc);
    Event_Wait(0x12c);
    *(u8 *)(Func_02001a90() + 85) = v6;
    Camera_SetSpeed(0x1999, 0x333);
    Camera_MoveTo(0x3120000, 0, 0x1ae0000, 1);
    Event_Wait(240);
    Actor_Stop(10);
    Actor_ShowEmote(10, 0x102, 80);
    Actor_WalkToAndWait(10, 0x333, 0x195);
    Event_Wait(40);
    Actor_SetAnimationAndWait(10, 4);
    Event_Wait(40);
    Actor_FaceDirection(10, 0xd000, 20);
    Event_SetMessage(0x1c1e);
    Event_ShowMessageAndWait(0x900a, 0, 20);
    Actor_Stop(0);
    Actor_Stop(1);
    Actor_Stop(2);
    Actor_Stop(3);
    Actor_ShowEmote(11, 0x100, 40);
    Event_ShowMessageAndWait(0x200b, 0, 20);
    Actor_RunRepeatedMotion(10, 2);
    Event_Wait(40);
    Event_ShowMessageAndWait(0x900a, 0, 10);
    Actor_FaceDirection(11, 0x5000, 10);
    Event_ShowMessageAndWait(0x200b, 0, 40);
    Actor_RunRepeatedMotion(10, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x900a, 0, 20);
    Actor_ShowEmote(0, 0x102, 80);
    Actor_ShowEmote(11, 0x106, 40);
    Event_ShowMessageAndWait(0x200b, 0, 40);
    Actor_StartRepeatedMotion(10, 2);
    Actor_ShowEmote(10, 0x102, 20);
    Actor_SetAnimation(10, 4);
    Event_ShowMessageAndWait(0x900a, 0, 10);
    Actor_StartRepeatedMotion(11, 1);
    Actor_SetAnimationAndWait(11, 3);
    Event_Wait(20);
    Actor_StartRepeatedMotion(10, 1);
    Actor_SetAnimationAndWait(10, 4);
    Actor_StartRepeatedMotion(11, 1);
    Actor_SetAnimationAndWait(11, 3);
    Actor_StartRepeatedMotion(10, 1);
    Actor_SetAnimationAndWait(10, 4);
    Actor_ShowEmote(9, 0x105, 0);
    Actor_RunRepeatedMotion(9, 1);
    Event_Wait(20);
    Actor_FaceDirection(9, 0x1000, 40);
    Actor_RunRepeatedMotion(9, 2);
    Event_Wait(60);
    Actor_RunRepeatedMotion(9, 3);
    Event_Wait(40);
    Event_ShowMessageAndWait(0x4009, 0, 40);
    Actor_SetAnimation(11, 0);
    Actor_RunRepeatedMotion(11, 2);
    Event_ShowMessageAndWait(0x200b, 0, 10);
    Actor_SetAnimationAndWait(9, 4);
    Actor_RunRepeatedMotion(9, 2);
    Event_ShowMessageAndWait(0x4009, 0, 10);
    Actor_ShowEmote(10, 0x100, 20);
    Actor_FaceDirection(10, 0x5000, 40);
    Actor_SetAnimationAndWait(10, 3);
    Event_ShowMessageAndWait(0x400a, 0, 10);
    Actor_SetAnimationAndWait(9, 4);
    Actor_FaceDirection(9, 0xd000, 10);
    Actor_Jump(9, 2, 0);
    Actor_SetAnimation(9, 4);
    Event_ShowMessageAndWait(0x4009, 0, 10);
    Actor_ShowEmote(11, 0x101, 0);
    Actor_ShowEmote(10, 0x101, 40);
    Actor_FaceDirection(10, 0xd000, 80);
    Actor_FaceDirection(10, 0x5000, 60);
    Actor_StartRepeatedMotion(10, 2);
    Actor_StartRepeatedMotion(11, 2);
    Call11(Engine_EventShowTwoMessagesAndWait, 10, 11, 6, 6, 6, 11, 12, 1, 7, 1, v6);
    Event_Wait(20);
    Camera_SetSpeed(0x19999, 0x3333);
    Camera_MoveTo(0x3090000, 0, 0x1d40000, 1);
    Camera_WaitForMove();
    Event_Wait(40);
    Actor_SetAnimationAndWait(1, 3);
    Event_ShowMessageAndWait(0x1001, 0, 20);
    Actor_RunRepeatedMotion(8, 2);
    Func_02001c7a(base7_20090c1);
    Event_Wait(40);
    Actor_SetAnimationAndWait(8, 6);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x4008, 0, 20);
    Camera_MoveTo(0x2ee0000, 0, 0x1c30000, 1);
    Event_Wait(20);
    Actor_FaceDirection(11, 0x5000, 0);
    Actor_FaceDirection(10, 0x5000, 10);
    Actor_FaceDirection(8, 0x1000, 40);
    Actor_ShowEmote(8, 0x100, 40);
    Actor_FaceDirection(8, 0x3000, 20);
    Actor_FaceDirection(8, 0x1000, 20);
    Actor_FaceDirection(8, 0x3000, 40);
    Actor_SetAnimationAndWait(8, 6);
    Event_Wait(60);
    Actor_Jump(8, 6, 0);
    Event_ShowMessageAndWait(0x4008, 0, 20);
    Actor_SetSpeed(1, 0x19999, 0xcccc);
    Actor_WalkToAndWait(1, 0x315, 0x1d9);
    Actor_FaceDirection(1, 0x7000, 20);
    Actor_SetAnimationAndWait(1, 3);
    Event_ShowMessageAndWait(0x4001, 0, 10);
    Actor_FaceDirection(8, 0x1000, 20);
    Actor_SetAnimationAndWait(8, 3);
    Event_OpenMessage(0x4008, 0);
    Actor_FaceDirection(10, 0x5000, 0);
    Actor_FaceDirection(9, 0x1000, 0);
    Actor_FaceDirection(1, 0x3000, 0);
    Actor_FaceDirection(2, 0x7000, 0);
    Actor_FaceDirection(3, 0xb000, 0);
    if (Event_ChooseYesNo(0, 0) == 1) {
        bump_step(1);
    }
    Camera_MoveTo(0x3090000, 0, 0x1ac0000, 1);
    ((void (*)())Engine_EventWait)(20);
    Actor_RunRepeatedMotion(10, 2);
    Event_ShowMessage(10, 0);
    Actor_SetAnimationAndWait(11, 4);
    Event_Wait(20);
    Event_SetMessage(0x1c33);
    Event_ShowMessage(0x200b, 0);
    Camera_MoveTo(0x3090000, 0, 0x1d40000, 1);
    Event_Wait(20);
    Actor_FaceDirection(1, 0xd000, 20);
    Actor_SetAnimationAndWait(1, 3);
    Event_Wait(20);
    Actor_SetAnimationAndWait(9, 4);
    Actor_FaceDirection(9, 0xd000, 10);
    Event_ShowMessage(0x4009, 0);
    Actor_SetAnimationAndWait(8, 3);
    Event_ShowMessage(0x4008, 0);
    Actor_FaceDirection(1, 0x7000, 10);
    Actor_SetAnimationAndWait(1, 3);
    Actor_FaceDirection(9, 0x1000, 10);
    Actor_SetAnimation(11, 3);
    Actor_SetAnimation(10, 3);
    Actor_SetAnimation(9, 3);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Actor_FaceDirection(1, 0x3000, 20);
    Actor_ShowEmote(1, 0x102, 80);
    Actor_FaceDirection(1, 0x7000, 20);
    Event_ShowMessageAndWait(0x4001, 0, 20);
    Actor_FaceDirection(1, 0x3000, 10);
    Actor_FaceDirection(0, 0, 40);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimationAndWait(1, 3);
    Event_Wait(20);
    Actor_FaceDirection(0, 0x4000, 20);
    Actor_SetAnimationAndWait(3, 3);
    Event_Wait(20);
    Actor_FaceDirection(1, 0x1000, 0);
    Actor_FaceDirection(0, 0xe000, 0);
    Actor_SetSpeed(2, 0x10000, 0x8000);
    Actor_WalkToAndWait(2, 0x333, 0x1e9);
    Actor_FaceDirection(2, 0xb000, 40);
    Actor_RunRepeatedMotion(2, 2);
    Event_ShowMessageAndWait(2, 0, 20);
    Actor_SetAnimationAndWait(2, 3);
    Actor_SetAnimation(8, 3);
    Actor_SetAnimation(9, 3);
    Actor_SetAnimation(10, 3);
    Actor_SetAnimationAndWait(9, 3);
    *(u8 *)(Func_02001ffe(3) + 35) &= 254;
    Actor_SetSpritePriority(3, 1);
    Actor_SetSpeed(3, 0x10000, 0x8000);
    Actor_WalkToAndWait(3, 0x31a, 0x208);
    Actor_FaceDirection(1, 0x5000, 0);
    Actor_FaceDirection(0, 0xa000, 0);
    Actor_WalkToAndWait(3, 0x310, 0x1f0);
    Actor_FaceDirection(3, 0x9000, 10);
    *(u8 *)(Func_02002062(3) + 35) |= 1;
    Event_ShowMessageAndWait(3, 0, 20);
    Actor_SetAnimation(8, 3);
    Actor_SetAnimation(9, 3);
    Actor_SetAnimation(10, 3);
    Actor_SetAnimationAndWait(9, 3);
    Event_Wait(20);
    Camera_MoveTo(0x3090000, 0, 0x1ac0000, 1);
    Event_Wait(20);
    Actor_SetSpeed(11, 0x6666, 0x3333);
    Actor_WalkToAndWait(11, 0x343, 0x184);
    Actor_FaceDirection(11, 0x5000, 0);
    Actor_ShowEmote(11, 0x108, 40);
    Event_ShowMessageAndWait(0x200b, 0, 20);
    Camera_MoveTo(0x3090000, 0, 0x1d40000, 1);
    Event_Wait(40);
    Actor_FaceDirection(2, 0x7000, 0);
    Actor_FaceDirection(3, 0xf000, 40);
    Actor_FaceDirection(2, 0x9000, 0);
    Actor_FaceDirection(3, 0xd000, 20);
    Actor_SetAnimation(2, 3);
    Actor_SetAnimationAndWait(3, 3);
    Event_Wait(20);
    Actor_RunRepeatedMotion(10, 1);
    Event_Wait(20);
    Actor_SetAnimation(10, 3);
    Event_ShowMessageAndWait(10, 0, 20);
    Actor_FaceDirection(0, 0xc000, 0);
    Actor_FaceDirection(1, 0xd000, 0);
    Actor_FaceDirection(2, 0xb000, 0);
    Actor_FaceDirection(3, 0xd000, 40);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimation(2, 3);
    Actor_SetAnimationAndWait(3, 3);
    Event_Wait(20);
    Actor_SetSpeed(2, 0x10000, 0x8000);
    base5_2009400 = (s32)Data_02009400;
    Actor_EnableActionCallback(1, base5_2009400);
    Value2(Engine_ActorEnableActionCallback, 2, base5_2009400);
    Value2(Func_02002248, 3, base5_2009400);
    Value2(Engine_ActorEnableActionCallback, 10, 0x2009310);
    Actor_WalkToAndWait(11, 0x345, 0x178);
    Actor_FaceDirection(11, 0xd000, 20);
    GameFlag_Set(0x81d);
    Event_End();
}

void Scene_UpdateTimedActor(void)
{
    u32 Func_0200220e_a(u32, u32);

    s32 no;
    u32 phase;
    union SceneActor *actor;
    s32 *other;

    phase = Func_0200220e_a(Data_03001e40, 180);
    no = 23;
    switch (phase) {
    case 10:
        break;
    case 20:
        no = 24;
        break;
    case 30:
        no = 25;
        break;
    default:
        return;
    }
    actor = Func_020022b0(no);
    if (actor == NULL) {
        return;
    }
    other = Func_020022bc(8);
    if (other != NULL) {
        Actor_SetPosition(no, other[2], other[4]);
    }
    actor->words[6] = 0x6666;
    actor->words[7] = 0x6666;
    {
        s32 y = actor->words[3] + 0x180000;
        union SceneField *dst = (union SceneField *)(actor->halfwords + 50);
        s32 value;
        actor->words[3] = y;
        actor->words[15] = y;
        value = 25;
        dst->value = value;
        dst++;
        value = 128;
        dst->value = value;
    }
    Actor_EnableActionCallback(no, Data_02009440);
}
