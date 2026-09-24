#include "TYPES.H"
#include "FIELD_EVENT.H"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
#define Scene_GetRecord_3(args...) Func_02005cde_a(args)
#define Scene_GetRecord_4(args...) Func_02005cf0(args)
#define Engine_ActorWalkToAndWait(args...) Actor_WalkToAndWait(args)
#define Scene_GetRecord_5(args...) Func_02005d20(args)
#define Scene_GetRecord_6(args...) Func_02005d30(args)
#define Engine_ActorFaceDirection(args...) Actor_FaceDirection(args)
#define Scene_GetRecord_7(a0) Value1(Engine_ActorGet, a0)
#define Engine_ActorSetPosition(args...) Actor_SetPosition(args)
#define Engine_ActorWalkToAndWait(args...) Actor_WalkToAndWait(args)
#define Engine_ActorFaceDirection(args...) Actor_FaceDirection(args)
#define Engine_ActorFaceDirection(args...) Actor_FaceDirection(args)
#define Engine_ActorFaceDirection(args...) Actor_FaceDirection(args)
#define Engine_ActorFaceDirection(args...) Actor_FaceDirection(args)
#define Engine_ActorFaceDirection(args...) Actor_FaceDirection(args)
#define Engine_ActorFaceDirection(args...) Actor_FaceDirection(args)
#define Engine_ActorFaceDirection(args...) Actor_FaceDirection(args)
#define Scene_GetRecord_8(args...) Func_02006168(args)
#define Scene_GetRecord_9(args...) Func_0200617a(args)
#define Engine_ActorWalkToAndWait(args...) Actor_WalkToAndWait(args)
#define Scene_GetRecord_10(args...) Func_020061aa(args)
#define Scene_GetRecord_11(args...) Func_020061ba(args)
#define Engine_ActorWalkToAndWait(args...) Actor_WalkToAndWait(args)
#define Engine_ActorWalkToAndWait(args...) Actor_WalkToAndWait(args)
#define Engine_ActorSetPosition(args...) Actor_SetPosition(args)
#define Engine_ActorWalkToAndWait(args...) Actor_WalkToAndWait(args)
#define Engine_ActorSetPosition(args...) Actor_SetPosition(args)
#define Engine_ActorWalkToAndWait(args...) Actor_WalkToAndWait(args)
#define Engine_ActorWalkToAndWait(args...) Actor_WalkToAndWait(args)
#define Engine_ActorSetPosition(args...) Actor_SetPosition(args)
#define Engine_ActorSetPosition(args...) Actor_SetPosition(args)
#define SCENE_FIELD_1C8 (*(s32 *)(*(u8 **)Data_03001ebc + 0x1c8))
#define SCENE_FIELD_1C0 (*(s32 *)(*(u8 **)Data_03001ebc + 0x1c0))
#define GetOrbitingSceneObject Func_02007664
#define AllocateEffectTransfer Func_02007604
#define UpdateOrbitingSceneObject Value_0200b9d5

#include "RESOURCE_3A8_EFFECT.H"

enum StagedGroupMessage {
    MSG_ROBIN_PEERED_INTO = 0x947,
    MSG_CAME_KALAY_BECAUSE_DIDNT_LIKE = 0x1a7c,
    MSG_WHY_WE_STOPPING_AT_PLACE = 0x1a92,
    MSG_THATS_WE_CANT_WAIT_ANY = 0x1ab2,
    MSG_LORD_HAMMET_SELLS_HIS_BEST = 0x1acf,
    MSG_DID_FIND_NEEDED_IN_WEAPON = 0x1ad1,
    MSG_LADY_LAYANA_SHARED_IN_LORD = 0x1ad5,
    MSG_LORD_HAMMETS_PALACE_LORD_AWAY = 0x1b05,
    MSG_WEVE_ARRIVED_HAMMET = 0x256f,
    MSG_DO_WANT_GO_CAVE_UP = 0x2584,
    MSG_LAYANA_WAS_VERY_HARD_ON = 0x25b3,
    MSG_VERY_CLEAN_MAINTAINED = 0x29df
};


struct Obj {
    u8 filler00[6];
    u16 f06;
};

struct Obj_02000040 {
    u8 filler00[100];
    u16 f64;
    u16 f66;
};

struct Object {
    u8 filler00[6];
    u16 x;
    u8 filler08[94];
    s16 cnt;
};

typedef struct Effect {
    unsigned char pad00[0xC];
    s32 y;
    unsigned char pad10[0x13];
    s8 state23;
} Effect;

struct SceneWork {
    u8 unknown_000[0x1c0];
    s32 request;
    u8 unknown_1c4[4];
    s32 setup;
    u8 unknown_1cc[12];
    u16 step;
};

struct Obj_020036f8 {
    u8 filler00[8];
    s32 f08;
    s32 f0c;
    u8 filler10[8];
    s32 f18;
    s32 f1c;
    s32 f20;
    s32 f24;
    s32 f28;
    u8 filler2c[56];
    s16 f64;
};

typedef struct RenderData {
    unsigned char pad00[0x1E];
    s16 rotation;
} RenderData;

typedef struct Effect_0200390c {
    unsigned char pad00[8];
    s32 x;
    s32 y;
    unsigned char pad10[0x20];
    s32 angle;
    unsigned char pad34[4];
    s32 base_x;
    s32 base_y;
    unsigned char pad40[0x10];
    RenderData *render;
} Effect_0200390c;

typedef struct OrbitingSceneObjectSprite {
    u8 padding_00[5];
    u8 flags_05_low : 5;
    u8 flags_05_bit_5 : 1;
    u8 flags_05_high : 2;
    u8 padding_06[3];
    u8 flags_09_low : 2;
    u8 flags_09_mode : 2;
    u8 flags_09_high : 4;
    u8 padding_0a[18];
    u8 palette;
    u8 padding_1d[10];
    u8 state;
} OrbitingSceneObjectSprite;

typedef struct OrbitingSceneObject {
    u8 padding_00[8];
    s32 x;
    s32 y;
    u8 padding_10[19];
    u8 flags_23;
    u8 padding_24[12];
    s32 orbit_angle;
    u8 padding_34[4];
    s32 orbit_center_x;
    s32 orbit_center_y;
    u8 padding_40[16];
    OrbitingSceneObjectSprite *sprite;
    u8 padding_54;
    u8 mode;
    u8 state;
    u8 padding_57[5];
    u8 active;
    u8 padding_5d[4];
    u8 visible;
    u8 padding_62[10];
    u32 callback;
} OrbitingSceneObject;

extern u8 Data_0200beac[];
extern u8 Value_00000063;
extern u8 Value_00000066;
extern u8 Value_00000099;
extern u8 Value_0000009a;
extern u8 Value_0000009b;
extern u8 Value_0000009c;
extern u8 Data_0200c2c8[];
extern u8 Data_0200c448[];
extern u8 Data_0200c4a8[];
extern u8 Data_0200c520[];
extern u8 Data_0200c580[];
extern u8 Data_0200c628[];
extern u8 Data_0200c298[];
extern u8 Data_0200c6b8[];
extern u8 Data_0200c768[];
extern u8 Data_0200ca20[];
extern u8 Data_0200ca80[];
extern u8 Data_0200cb58[];
extern u8 Data_0200c750[];
extern u8 Data_0200cbf4[];
extern u8 Data_0200ce88[];
extern u8 Data_0200cedc[];
extern u8 Data_0200cf24[];
extern u8 Data_0200cf54[];
extern u8 Data_0200cf9c[];
extern u8 Data_0200cbe8[];
extern u8 Data_0200b90d[];
extern u8 Value_00001a9e[];
extern u8 Value_00001aa2[];
extern const u32 SceneAction_GroupMotion[];
extern const u32 SceneAction_GroupOffsetMotion[];
extern s16 Data_02000240_t[][1];
extern s32 Data_0200bd84[];
extern const u8 Data_0200c21c[];
extern const u8 Data_0200c230[];
extern u8 Value_0200b9d5;

void Func_02003d96(s32, s32, s32);
s32 Func_0200521a(s32);
void Func_0200567c(s32);
u8 *Func_0200416e();
void Func_0200513a(s32);
s32 Func_02005202(s32);
void Func_02003c04();
void Func_02003cec();
void Func_02003bc4();
void Func_02003c3c();
void Func_02003c4c();
void Func_02001ed8();
void Func_02003b24();
void Func_02003d54();
void Func_02002d24(void);
void Func_02005532();
u8 *Func_02005606();
void Func_02005692();
void Func_02005a24();
void Func_02005bbc();
s32 Func_02005cde_a();
s32 Func_02005cf0();
u8 *Func_02005d20();
u8 *Func_02005d30();
s32 Func_02006168();
s32 Func_0200617a();
u8 *Scene_GetRecord_12();
u8 *Scene_GetRecord_13();
u8 *Func_020061aa();
u8 *Func_020061ba();
void Func_0200652a();
s32 Func_02006dd2();
void Func_02006dd4();
void Func_02006de6();
void Func_02006df4();
void Func_02006fd8();
void Func_02007056();
void Func_0200709e();
s32 Func_020073fc();
void Func_02007540();
void Func_02007588();
u8 *Func_02003d1c_a();
void Func_0200727e_a(void);
OrbitingSceneObject *Func_02007664();
u8 *Func_02007604(s32, s32);

/* Two early long branches share the scene-skip tail. Four polling loops
 * wait on signed actor fields; calls bind at loader runtime addresses. */

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

/* Moves the next dialogue line on by amount messages. */
static __inline__ void bump_step(s32 amount)
{
    gEventWork->message += amount;
}

/* Contiguous unnamed leaf-owner run for resource_3a8. */

/* resource_3a8 owner at 0x02000504, 138 bytes. Scene arrival sequence. */
static __inline__ void SetOffset(s32 actor, s32 axis, s32 offset)
{
    Actor_SetDestinationOffset(actor, axis, offset);
}

/* Verified scene siblings use these call forms for independently evaluated
 * large constants in repeated actor operations. */
static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{

    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{

    f(a0, a1, a2);
}

static __inline__ void Call1_020015b4(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1_020015b4(s32 (*f)(), s32 a0)
{
    return f(a0);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step_020015b4(s32 amount)
{
    gEventWork->message += amount;
}

/* Field at 0x1c8 of the shared scene work record. */

/* Field at 0x1c0 of the shared scene work record. */

static __inline__ void Call1_0200164c(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1_0200164c(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2_0200164c(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3_0200164c(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step_0200164c(s32 amount)
{
    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ void Call1_02003768(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

static __inline__ void Call1_02003864(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call1_020026c0(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2_020026c0(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call6_020026c0(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

void SceneState_ApplyValues12And2And3(void)
{
    Func_02003d96(0xC, 2, 3);
}

s32 SceneEffect_UpdateRandomAction(struct Resource3a8Effect *effect)
{
    u32 action;
    u32 next_timer;

    if (effect->action_timer == 0) {
        action = (u32)(Random_Next() * 8) >> 0x10;
        switch (action) {
        case 0:
            Object_SetAnimation(effect, 3);
            break;
        case 1:
            Object_SetAnimation(effect, 4);
            break;
        case 3:
        case 4:
            effect->unknown_06 += (u32)(Random_Next() << 0xF) >> 0x10;
            break;
        }
        next_timer = (u32)(Random_Next() * 0x50) >> 0x10;
        effect->action_timer = (s16)next_timer;
        if (next_timer != 0) {
            goto decrement_timer;
        }
    } else {
decrement_timer:
        --effect->action_timer;
    }
    return 1;
}

s32 OverlayObject_AdvanceXWhenCounterExpires(struct Object *obj)
{
    s32 cnt = *(u16 *)&obj->cnt;
    s32 loaded = obj->cnt;
    s32 amount;

    if (loaded == 0) {
        obj->x += (u32)(Random_Next() << 15) >> 16;
        amount = (u32)(Random_Next() * 80) >> 16;
        obj->cnt = amount;
        if (amount == 0) {
            goto done;
        }
        cnt = amount;
    }
    obj->cnt = cnt - 1;
done:
    return 1;
}

/* Contiguous unnamed leaf-owner run for resource_3a8. */
s32 SceneData_SelectTableByWord224(void)
{
    s16 v = gGameState.scene;

    if (v == (s32)&Value_00000063) {
        return (s32)Data_0200c2c8;
    }
    if (v == (s32)&Value_00000066) {
        return (s32)Data_0200c448;
    }
    if (v == (s32)&Value_00000099) {
        return (s32)Data_0200c4a8;
    }
    if (v == (s32)&Value_0000009a) {
        return (s32)Data_0200c520;
    }
    if (v == (s32)&Value_0000009b) {
        return (s32)Data_0200c580;
    }
    if (v == (s32)&Value_0000009c) {
        return (s32)Data_0200c628;
    }
    return (s32)Data_0200c298;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetTablec6b8(void)
{
    return Data_0200c6b8;
}

s32 SceneData_SelectTableByWord224B(void)
{
    s16 v = gGameState.scene;

    if (v == (s32)&Value_00000063) {
        return (s32)Data_0200c768;
    }
    if (v == (s32)&Value_00000066) {
        return (s32)Data_0200ca20;
    }
    if (v == (s32)&Value_00000099) {
        return (s32)Data_0200ca80;
    }
    if (v == (s32)&Value_0000009c) {
        return (s32)Data_0200cb58;
    }
    return (s32)Data_0200c750;
}

void FieldScene_RunStepWithValue29df(void)
{
    Event_Begin();
    Message_ShowCentered(MSG_ROBIN_PEERED_INTO, 1);
    Message_ShowCentered(MSG_VERY_CLEAN_MAINTAINED, 1);
    Event_End();
}

void FieldScene_RunStepWithValueFd6(void)
{
    Event_Begin();
    Actor_SetPosition(12, 0, 0);
    GameFlag_Set(0xfd6);
    Item_ShowFound(181, 3);
    Party_GiveItem(181, 0);
    Event_End();
}

s32 SceneData_SelectTableBySelector224(void)
{
    s16 v = gGameState.scene;

    if (v == (s32)&Value_00000063) {
        return (s32)Data_0200cbf4;
    }
    if (v == (s32)&Value_00000066) {
        return (s32)Data_0200ce88;
    }
    if (v == (s32)&Value_00000099) {
        return (s32)Data_0200cedc;
    }
    if (v == (s32)&Value_0000009a) {
        return (s32)Data_0200cf24;
    }
    if (v == (s32)&Value_0000009b) {
        return (s32)Data_0200cf54;
    }
    if (v == (s32)&Value_0000009c) {
        return (s32)Data_0200cf9c;
    }
    return (s32)Data_0200cbe8;
}

void SceneDialogue_RunActorNineteenDialogue(void)
{
    Event_Begin();
    Event_SetMessage(MSG_CAME_KALAY_BECAUSE_DIDNT_LIKE);
    Event_AskYesNo(19, 0);
    Event_End();
}

void FieldScene_RunSlotZeroFacingSequence(void)
{
    extern u8 *Data_03001ebc;

    struct Obj *o;
    u32 v;
    u16 *q;

    o = Actor_Get(0);
    v = (o->f06 + 0xfffff000) << 16;
    if (v > 0x60000000) {
        Event_Begin();
        Actor_FaceEachOther(0, 8, 0);
        Event_Wait(10);
        Event_SetMessage(MSG_DO_WANT_GO_CAVE_UP);
        Event_OpenMessage(8, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            Actor_SetAnimationAndWait(8, 4);
            Event_ShowMessage(8, 0);
        } else {
            q = (u16 *)(Data_03001ebc + 472);
            *q = *q + 1;
            Actor_SetAnimationAndWait(8, 3);
            Event_ShowMessage(8, 0);
        }
        Event_End();
    }
}

void SceneDialogue_RunActorTenDialogue(void)
{
    Event_Begin();
    Event_SetMessage(MSG_LAYANA_WAS_VERY_HARD_ON);
    Event_AskYesNo(10, 0);
    Event_End();
}

void SceneState_BranchOnSlotZeroFacing(void)
{
    struct Obj *o;
    u32 v;

    o = Actor_Get(0);
    v = (o->f06 - 0x2000) << 16;
    if (v > 0x80000000) {
        Shop_Open(22, 22);
    } else {
        Event_Begin();
        Event_SetMessage(MSG_LORD_HAMMET_SELLS_HIS_BEST);
        Event_ShowMessage(22, 0);
        Event_End();
    }
}

void SceneDialogue_RunActorTwentyThreeByLeaderHeading(void)
{
    struct Obj *o;
    u32 v;

    o = Actor_Get(0);
    v = (o->f06 - 0x6001) << 16;
    if (v <= 0x7ffe0000) {
        Shop_Open(23, 23);
    } else {
        Event_Begin();
        Event_SetMessage(MSG_DID_FIND_NEEDED_IN_WEAPON);
        Event_AskYesNo(23, 0);
        Event_End();
    }
}

void FieldScene_RunActorTwentyFourAngleDialogue(void)
{
    struct Obj *o;
    u32 v;

    o = Actor_Get(0);
    v = (o->f06 - 0x2000) << 16;
    if (v > 0xC0000000) {
        Shop_Open(24, 24);
    } else {
        Event_Begin();
        Event_SetMessage(MSG_LADY_LAYANA_SHARED_IN_LORD);
        Event_ShowMessage(24, 0);
        Event_End();
    }
}

void RunSceneArrivalSetup(void)
{
    s32 two = 2;

    Event_Begin();
    Audio_PlayCue(188);
    Map_CopyCellsTo(36, 23, 43, 12, two, two);
    Task_Wait(5);
    Map_CopyCellsTo(39, 23, 43, 12, two, two);
    Task_Wait(5);
    Actor_SetSpeed(0, 0x8000, 0x4000);
    Func_0200416e(0)[85] = 0;
    Actor_SetAnimation(0, 2);
    SetOffset(0, 0, -8);
    Event_Wait(10);
    Event_RequestExit(2);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_End();
}

void FieldScene_RunStagedGroupSequence(void)
{

    s32 messageId;
    const u32 *actionDescriptor;
    s32 idleState;
    s32 actionEnabled;
    u8 *actor20PairedWait;
    u8 *actor21PairedWait;
    u8 *closingWaitRecord;
    u8 *actor14FinalFacing;
    u8 *actor20FinalFacing;
    u8 *actor21FinalFacing;
    u8 *playerFinalFacing;
    u8 *actor21OpeningWait;
    u8 *actor20BeforeFirstMove;
    u8 *actor20AfterFirstMove;
    u8 *actor20BeforeSecondMove;
    u8 *actor20AfterSecondMove;
    u8 *sceneWorkspace;

    sceneWorkspace = *(u8 **)0x03001ebc;
    Event_Begin();
    if (GameFlag_IsSet(2320) == 0) {
        goto skip_scene;
    }
    if (GameFlag_IsSet(2321) != 0) {
        goto skip_scene;
    }
    Func_02003c04(33605960);
    Actor_SetPosition(20, 16515072, 17825792);
    Actor_SetPosition(27, 18612224, 17301504);
    Actor_SetPosition(28, 18612224, 18350080);
    Actor_SetPosition(29, 19660800, 17301504);
    Actor_SetPosition(30, 19660800, 18350080);
    Actor_SetPosition(32, 20709376, 17301504);
    Actor_SetPosition(31, 20709376, 18350080);
    Actor_SetPosition(33, 21757952, 17301504);
    Actor_SetPosition(34, 21757952, 18350080);
    Actor_SetPosition(21, 23855104, 17825792);
    Audio_PlayCue(17);
    Func_02003cec(20);
    Func_02003bc4(6801, 1, 0);
    Audio_PlayCue(9);
    Event_Wait(10);
    Actor_RunRepeatedMotion(0, 2);
    if (*(s16 *)(sceneWorkspace + 364) == 9) {
        Camera_SetSpeed(157286, 19660);
        Actor_FaceDirection(0, 57344, 20);
    } else {
        Camera_SetSpeed(78643, 9830);
        Actor_FaceDirection(0, 0, 20);
    }
    Actor_SetSpeed(20, 72089, 36044);
    Actor_SetSpeed(27, 65536, 32768);
    Actor_SetSpeed(28, 65536, 32768);
    Actor_SetSpeed(29, 58982, 29491);
    Actor_SetSpeed(30, 58982, 29491);
    Actor_SetSpeed(32, 52428, 26214);
    Actor_SetSpeed(31, 52428, 26214);
    Actor_SetSpeed(33, 45875, 22937);
    Actor_SetSpeed(34, 45875, 22937);
    Actor_SetSpeed(21, 39321, 19660);
    actionDescriptor = SceneAction_GroupMotion;
    Engine_ActorEnableActionCallback(20, actionDescriptor);
    Engine_ActorEnableActionCallback(27, actionDescriptor);
    Engine_ActorEnableActionCallback(28, actionDescriptor);
    Engine_ActorEnableActionCallback(29, actionDescriptor);
    Engine_ActorEnableActionCallback(30, actionDescriptor);
    Engine_ActorEnableActionCallback(32, actionDescriptor);
    Engine_ActorEnableActionCallback(31, actionDescriptor);
    Engine_ActorEnableActionCallback(33, actionDescriptor);
    Engine_ActorEnableActionCallback(34, actionDescriptor);
    actor21OpeningWait = Actor_Get(21);
    {
        s32 value = 0;
        *(u16 *)(actor21OpeningWait + 100) = value;
    }
    Engine_ActorEnableActionCallback(21, actionDescriptor);
    Camera_MoveTo(12189696, -1, 17825792, 1);
    Func_02003c3c(20);
    Actor_FaceDirection(20, 0, 0);
    do {
        Task_Wait(1);
    } while (*(s16 *)((u8 *)actor21OpeningWait + 100) == 0);
    Event_Wait(40);
    Actor_RunRepeatedMotion(27, 2);
    Actor_FaceDirection(27, 20480, 20);
    Event_SetMessage(MSG_WHY_WE_STOPPING_AT_PLACE);
    Event_ShowMessageAndWait(27, 0, 10);
    Actor_RunRepeatedMotion(28, 2);
    Actor_FaceDirection(28, 45056, 10);
    Actor_SetAnimation(28, 3);
    Event_ShowMessageAndWait(28, 0, 10);
    Actor_SetAttachedEffect(32, 258);
    Event_Wait(40);
    Event_ShowMessageAndWait(32, 0, 10);
    Actor_ShowEmote(31, 256, 40);
    Actor_FaceDirection(31, 45056, 10);
    Event_ShowMessageAndWait(31, 0, 10);
    Actor_FaceDirection(31, 32768, 10);
    Actor_RunRepeatedMotion(31, 2);
    Actor_SetAnimation(31, 4);
    Event_ShowMessageAndWait(31, 0, 10);
    Actor_FaceDirection(31, 45056, 0);
    Actor_FaceDirection(32, 20480, 20);
    Actor_SetAnimation(31, 3);
    Actor_SetAnimationAndWait(32, 3);
    Actor_RunRepeatedMotion(20, 2);
    Actor_SetAttachedEffect(20, 258);
    Event_Wait(40);
    Event_ShowMessageAndWait(20, 0, 10);
    Actor_RunRepeatedMotion(20, 2);
    actor20BeforeFirstMove = Actor_Get(20);
    {
        s32 flags = actor20BeforeFirstMove[90] & 0xfe;
        idleState = 0;
        actor20BeforeFirstMove[90] = flags;
    }
    Actor_WalkToAndWait(20, 172, 264);
    Event_Wait(1);
    actor20AfterFirstMove = Actor_Get(20);
    actionEnabled = 1;
    {
        s32 flags = actor20AfterFirstMove[90];
        flags |= actionEnabled;
        actor20AfterFirstMove[90] = flags;
    }
    Actor_FaceDirection(27, 32768, 0);
    Actor_FaceDirection(28, 32768, 0);
    Actor_FaceDirection(32, 32768, 0);
    Actor_FaceDirection(31, 32768, 20);
    Actor_SetAnimationAndWait(20, 3);
    Event_Wait(20);
    actor20BeforeSecondMove = Actor_Get(20);
    *(u8 *)((u8 *)(actor20BeforeSecondMove) + 90) &= 0xfe;
    Actor_WalkToAndWait(20, 172, 272);
    Event_Wait(1);
    actor20AfterSecondMove = Actor_Get(20);
    actionEnabled |= actor20AfterSecondMove[90];
    actor20AfterSecondMove[90] = actionEnabled;
    Actor_WalkToAndWait(20, 180, 272);
    Actor_FaceDirection(20, 0, 0);
    Event_ShowMessageAndWait(20, 0, 10);
    Actor_ShowEmote(34, 261, 0);
    Actor_RunRepeatedMotion(34, 1);
    Actor_SetAnimationAndWait(34, 3);
    Event_ShowMessageAndWait(34, 0, 10);
    Actor_RunRepeatedMotion(33, 1);
    Event_ShowMessageAndWait(33, 0, 10);
    Actor_SetAnimation(33, 4);
    Event_ShowMessageAndWait(33, 0, 10);
    Actor_RunRepeatedMotion(21, 2);
    Actor_ShowEmote(21, 258, 0);
    Event_ShowMessageAndWait(21, 0, 10);
    Actor_Jump(20, 2, 20);
    Actor_Jump(20, 4, 40);
    Actor_RunRepeatedMotion(20, 2);
    Event_ShowMessageAndWait(20, 0, 10);
    Actor_SetSpeed(21, 104857, 52428);
    Actor_WalkToAndWait(21, 265, 282);
    Actor_WalkToAndWait(21, 251, 284);
    Actor_WalkToAndWait(21, 246, 296);
    Actor_FaceDirection(21, 49152, 0);
    SceneState_SetValues27Through34();
    Event_Wait(40);
    Actor_SetSpeed(21, 104857, 52428);
    Actor_WalkToAndWait(21, 228, 296);
    Actor_FaceDirection(21, 49152, 40);
    Actor_WalkToAndWait(21, 212, 296);
    Actor_FaceDirection(21, 49152, 40);
    Actor_WalkToAndWait(21, 192, 296);
    Actor_FaceDirection(21, 49152, 40);
    Actor_StartRepeatedMotion(21, 2);
    Actor_ShowEmote(21, 256, 60);
    Actor_FaceDirection(20, 12288, 0);
    Actor_WalkToAndWait(21, 184, 286);
    Actor_FaceDirection(21, 45056, 10);
    Actor_SetAttachedEffect(21, 258);
    Event_Wait(40);
    Actor_SetAnimationAndWait(21, 4);
    Actor_ShowEmote(20, 257, 40);
    Actor_SetAnimationAndWait(20, 3);
    Actor_RunRepeatedMotion(20, 2);
    Actor_FaceDirection(20, 0, 0);
    Actor_FaceDirection(21, 0, 60);
    Actor_FaceDirection(20, 12288, 0);
    Actor_FaceDirection(21, 45056, 10);
    Actor_SetAnimationAndWait(20, 3);
    Actor_SetAnimationAndWait(21, 3);
    Actor_FaceDirection(21, 0, 0);
    Actor_SetSpeed(20, 104857, 52428);
    Func_02003c4c(20, 33603504);
    Actor_WalkToAndWait(20, 228, 296);
    Actor_FaceDirection(20, 49152, 40);
    Actor_WalkToAndWait(20, 212, 296);
    Actor_FaceDirection(20, 49152, 40);
    Actor_WalkToAndWait(20, 192, 296);
    Actor_FaceDirection(20, 49152, 40);
    Actor_FaceDirection(20, 45056, 0);
    Actor_FaceDirection(21, 12288, 10);
    Actor_SetAttachedEffect(20, 258);
    Event_Wait(60);
    Actor_SetAnimationAndWait(20, 4);
    messageId = (s32)Value_00001a9e;
    Event_SetMessage(messageId);
    Event_ShowMessageAndWait(20, 0, 40);
    Func_02001ed8();
    Actor_WalkToAndWait(20, 178, 272);
    Actor_FaceDirection(20, 0, 0);
    Event_Wait(240);
    Actor_Stop(27);
    Task_Wait(1);
    Actor_FaceDirection(27, 32768, 10);
    Actor_ShowEmote(27, 257, 60);
    Event_ShowMessageAndWait(27, 0, 10);
    SceneEffect_SetSlotVariantAndDescriptor(27);
    Event_Wait(80);
    Actor_Stop(28);
    Task_Wait(1);
    Actor_FaceDirection(28, 53248, 20);
    Actor_RunRepeatedMotion(28, 2);
    Event_ShowMessageAndWait(28, 0, 10);
    SceneEffect_SetSlotVariantAndDescriptor(28);
    Event_Wait(160);
    Actor_Stop(32);
    Task_Wait(1);
    Actor_FaceDirection(32, 20480, 10);
    Actor_ShowEmote(32, 257, 60);
    Event_ShowMessageAndWait(32, 0, 10);
    SceneEffect_SetSlotVariantAndDescriptor(32);
    Event_Wait(80);
    Actor_Stop(30);
    Task_Wait(1);
    Actor_FaceDirection(30, 45056, 10);
    Actor_RunRepeatedMotion(30, 1);
    Event_SetMessage(messageId + 6);
    Event_ShowMessageAndWait(30, 0, 10);
    Func_02003b24(33595157);
    Actor_Stop(20);
    Actor_Stop(21);
    Task_Wait(1);
    actor20PairedWait = Actor_Get(20);
    *(u16 *)(actor20PairedWait + 100) = idleState;
    actor21PairedWait = Actor_Get(21);
    *(u16 *)(actor21PairedWait + 100) = idleState;
    Actor_SetSpeed(20, 52428, 26214);
    Actor_SetSpeed(21, 52428, 26214);
    Engine_ActorEnableActionCallback(20, 33603636);
    Engine_ActorEnableActionCallback(21, 33603788);
    Actor_Stop(29);
    Task_Wait(1);
    Actor_FaceDirection(29, 20480, 10);
    messageId += 5;
    Actor_RunRepeatedMotion(29, 2);
    Event_SetMessage(messageId);
    Event_ShowMessageAndWait(29, 0, 20);
    SceneEffect_SetSlotVariantAndDescriptor(29);
    SceneEffect_SetSlotVariantAndDescriptor(30);
    do {
        Task_Wait(1);
    } while (*(s16 *)((u8 *)Actor_Get(20) + 100) == 0 ||
             *(s16 *)((u8 *)Actor_Get(21) + 100) != 1);
    Engine_ActorEnableActionCallback(20, 33603940);
    Engine_ActorEnableActionCallback(21, 33604012);
    Actor_Stop(31);
    Task_Wait(1);
    Actor_FaceDirection(31, 0x5000, 10);
    Actor_RunRepeatedMotion(31, 1);
    Actor_SetAnimationAndWait(31, 4);
    messageId = (s32)Value_00001aa2;
    Event_SetMessage(messageId);
    Event_ShowMessageAndWait(31, 0, 10);
    SceneEffect_SetSlotVariantAndDescriptor(31);
    Actor_Stop(34);
    Actor_Stop(33);
    Task_Wait(1);
    Actor_ShowEmote(34, 261, 40);
    Actor_ShowEmote(33, 261, 60);
    Actor_FaceDirection(34, 45056, 10);
    Actor_FaceDirection(33, 20480, 10);
    messageId += 3;
    Actor_SetAnimationAndWait(34, 4);
    Event_SetMessage(messageId);
    Event_ShowMessageAndWait(34, 0, 10);
    Actor_RunRepeatedMotion(33, 1);
    Actor_SetAnimation(33, 4);
    Event_ShowMessageAndWait(33, 0, 10);
    Actor_ShowEmote(34, 258, 60);
    Actor_ShowEmote(20, 259, 0);
    Actor_RunRepeatedMotion(20, 2);
    Event_SetMessage(MSG_THATS_WE_CANT_WAIT_ANY);
    Event_ShowMessageAndWait(20, 0, 10);
    Actor_Stop(27);
    Actor_Stop(28);
    Actor_Stop(29);
    Actor_Stop(30);
    Actor_Stop(32);
    Actor_Stop(31);
    Actor_Stop(33);
    Actor_Stop(34);
    Actor_Stop(20);
    Actor_Stop(21);
    Task_Wait(1);
    Actor_Jump(27, 2, 0);
    Actor_Jump(28, 2, 0);
    Actor_Jump(29, 2, 0);
    Actor_Jump(30, 2, 0);
    Actor_Jump(32, 2, 0);
    Actor_Jump(31, 2, 0);
    Actor_Jump(33, 2, 0);
    Actor_Jump(34, 2, 0);
    Actor_Jump(21, 2, 40);
    Actor_FaceDirection(27, 32768, 0);
    Actor_FaceDirection(28, 32768, 0);
    Actor_FaceDirection(29, 32768, 0);
    Actor_FaceDirection(30, 32768, 0);
    Actor_FaceDirection(32, 32768, 0);
    Actor_FaceDirection(31, 32768, 0);
    Actor_FaceDirection(33, 32768, 0);
    Actor_FaceDirection(34, 32768, 40);
    Actor_Jump(21, 4, 40);
    Event_ShowMessageAndWait(21, 0, 10);
    Actor_RunRepeatedMotion(20, 1);
    Event_ShowMessageAndWait(20, 0, 10);
    Actor_SetAnimationAndWait(21, 3);
    Event_ShowMessageAndWait(21, 0, 10);
    Actor_SetAnimationAndWait(20, 3);
    Event_ShowMessageAndWait(20, 0, 10);
    Actor_ShowEmote(27, 258, 40);
    Actor_StartRepeatedMotion(27, 1);
    Event_ShowMessageAndWait(27, 0, 10);
    Actor_ShowEmote(28, 258, 40);
    Event_ShowMessageAndWait(28, 0, 10);
    Actor_SetAnimationAndWait(21, 4);
    Event_Wait(40);
    Actor_SetAnimationAndWait(21, 3);
    Event_ShowMessageAndWait(21, 0, 20);
    Actor_SetAnimationAndWait(20, 3);
    Event_ShowMessageAndWait(20, 0, 10);
    Actor_FaceDirection(27, 20480, 0);
    Actor_FaceDirection(28, 45056, 4);
    Actor_FaceDirection(29, 20480, 0);
    Actor_FaceDirection(30, 45056, 4);
    Actor_FaceDirection(32, 20480, 0);
    Actor_FaceDirection(31, 45056, 4);
    Actor_FaceDirection(33, 20480, 0);
    Actor_FaceDirection(34, 45056, 4);
    Actor_SetAnimation(27, 3);
    Actor_SetAnimationAndWait(28, 3);
    Actor_SetAnimation(29, 3);
    Actor_SetAnimationAndWait(30, 3);
    Actor_SetAnimation(32, 3);
    Actor_SetAnimationAndWait(31, 3);
    Actor_SetAnimation(33, 3);
    Actor_SetAnimationAndWait(34, 3);
    Actor_Jump(20, 2, 40);
    Event_ShowMessageAndWait(20, 0, 10);
    Actor_FaceDirection(27, 32768, 0);
    Actor_FaceDirection(28, 32768, 4);
    Actor_FaceDirection(29, 32768, 0);
    Actor_FaceDirection(30, 32768, 4);
    Actor_FaceDirection(32, 32768, 0);
    Actor_FaceDirection(31, 32768, 4);
    Actor_FaceDirection(33, 32768, 0);
    Actor_FaceDirection(34, 32768, 4);
    Actor_SetSpeed(20, 72089, 36044);
    Actor_SetSpeed(27, 68812, 34406);
    Actor_SetSpeed(28, 68812, 34406);
    Actor_SetSpeed(29, 65536, 32768);
    Actor_SetSpeed(30, 65536, 32768);
    Actor_SetSpeed(32, 0xf333, 0x7999);
    Actor_SetSpeed(31, 0xf333, 0x7999);
    Actor_SetSpeed(33, 58982, 29491);
    Actor_SetSpeed(34, 58982, 29491);
    Actor_SetSpeed(21, 55705, 27852);
    Actor_SetSpritePriority(27, 1);
    Actor_SetSpritePriority(28, 1);
    Actor_SetSpritePriority(29, 1);
    Actor_SetSpritePriority(30, 1);
    Actor_SetSpritePriority(32, 1);
    Actor_SetSpritePriority(31, 1);
    Actor_SetSpritePriority(33, 1);
    Actor_SetSpritePriority(34, 1);
    Actor_SetSpritePriority(20, 1);
    Actor_SetSpritePriority(21, 1);
    Actor_Stop(27);
    Actor_Stop(28);
    Actor_Stop(29);
    Actor_Stop(30);
    Actor_Stop(32);
    Actor_Stop(31);
    Actor_Stop(33);
    Actor_Stop(34);
    Actor_Stop(20);
    Actor_Stop(21);
    Task_Wait(1);
    actionDescriptor = SceneAction_GroupOffsetMotion;
    Engine_ActorEnableActionCallback(20, actionDescriptor);
    Engine_ActorEnableActionCallback(27, actionDescriptor);
    Engine_ActorEnableActionCallback(28, actionDescriptor);
    Engine_ActorEnableActionCallback(29, actionDescriptor);
    Engine_ActorEnableActionCallback(30, actionDescriptor);
    Engine_ActorEnableActionCallback(32, actionDescriptor);
    Engine_ActorEnableActionCallback(31, actionDescriptor);
    Engine_ActorEnableActionCallback(33, actionDescriptor);
    Engine_ActorEnableActionCallback(34, actionDescriptor);
    closingWaitRecord = Actor_Get(21);
    {
        u16 *state = (u16 *)(closingWaitRecord + 100);
        s32 value = 0;
        *state = value;
    }
    Engine_ActorEnableActionCallback(21, actionDescriptor);
    do {
        Task_Wait(1);
    } while (*(s16 *)((u8 *)Actor_Get(21) + 100) != 1);
    Event_Wait(80);
    Actor_SetPosition(14, 22282240, 17956864);
    Task_Wait(1);
    Actor_SetSpeed(14, 65536, 32768);
    Actor_WalkToAndWait(14, 224, 274);
    Actor_FaceDirection(14, 0, 40);
    Actor_FaceDirection(14, 32768, 40);
    Actor_FaceDirection(14, 49152, 40);
    Actor_FaceDirection(14, 20480, 40);
    Actor_ShowEmote(14, 257, 60);
    Event_ShowMessageAndWait(14, 0, 10);
    Actor_FaceDirection(14, 0, 40);
    Actor_FaceDirection(14, 49152, 40);
    Actor_FaceDirection(14, 32768, 40);
    Actor_SetAttachedEffect(14, 258);
    Actor_Jump(14, 4, 40);
    Event_ShowMessageAndWait(14, 0, 20);
    Actor_RunRepeatedMotion(14, 2);
    Event_ShowMessageAndWait(14, 0, 10);
    Actor_Jump(14, 4, 40);
    Actor_SetSpeed(14, 78643, 39321);
    closingWaitRecord = Actor_Get(14);
    {
        u16 *state = (u16 *)(closingWaitRecord + 100);
        s32 value = 0;
        *state = value;
    }
    Engine_ActorEnableActionCallback(14, SceneAction_GroupOffsetMotion);
    do {
        Task_Wait(1);
    } while (*(s16 *)((u8 *)Actor_Get(14) + 100) != 1);
    Actor_SetPosition(14, 23527424, 20578304);
    actor14FinalFacing = Actor_Get(14);
    {
        s32 value = 53248;
        *(u16 *)(actor14FinalFacing + 6) = value;
    }
    Actor_SetPosition(20, 29818880, 28442624);
    actor20FinalFacing = Actor_Get(20);
    {
        s32 value = 53248;
        *(u16 *)(actor20FinalFacing + 6) = value;
    }
    Actor_SetPosition(21, 30408704, 27262976);
    actor21FinalFacing = Actor_Get(21);
    {
        s32 value = 20480;
        *(u16 *)(actor21FinalFacing + 6) = value;
    }
    Actor_Destroy(27);
    Actor_Destroy(28);
    Actor_Destroy(29);
    Actor_Destroy(30);
    Actor_Destroy(31);
    Actor_Destroy(32);
    Actor_Destroy(33);
    Actor_Destroy(34);
    Audio_PlayCue(17);
    if (*(s16 *)(sceneWorkspace + 364) == 9) {
        Actor_WalkToAndWait(0, 224, 458);
        playerFinalFacing = Actor_Get(0);
        {
            s32 value = 49152;
            *(u16 *)(playerFinalFacing + 6) = value;
        }
    } else {
        Actor_WalkToAndWait(0, 40, 248);
        playerFinalFacing = Actor_Get(0);
        {
            s32 value = 16384;
            *(u16 *)(playerFinalFacing + 6) = value;
        }
    }
    Func_02003d54();
    GameFlag_Set(0x911);
    goto finish;
skip_scene:
    Audio_PlayCue(123);
    Event_RequestExit(*(s16 *)(sceneWorkspace + 364));
    Event_CloseScreen();
    Event_WaitForScreen();
finish:
    Event_End();
}

void SceneState_ClearWord24WhenFlag200(void)
{
    extern u8 *Data_03001ee0;

    if (GameFlag_IsSet(0x200) != 0) {
        *(s32 *)(Data_03001ee0 + 24) = 0;
        GameFlag_Clear(0x200);
    }
}

void SceneState_LinkRecordZeroWhenFlag200Clear(void)
{
    extern u8 *Data_03001ee0;

    u8 *work;

    if (GameFlag_IsSet(0x200) == 0) {
        work = Data_03001ee0;
        *(s32 *)(work + 24) = Value1_020015b4(Engine_ActorGet, 0);
        GameFlag_Set(0x200);
    }
}

void SceneActor_SetActorZeroByte35Bit0(void)
{
    u8 *p;
    s32 v;

    p = (u8 *)(Func_02005202(0) + 35);
    v = 1;
    v |= *p;
    *p = v;
}

void SceneState_ClearRecordZeroBit0At35(void)
{
    u8 *p;
    s32 v;

    p = (u8 *)(Func_0200521a(0) + 35);
    v = 254;
    v &= *p;
    *p = v;
}

void SceneDialogue_RunActor181Line916(void)
{
    Func_0200513a((s32)Data_0200b90d);
    Task_Wait(1);
    Actor_SetPosition(26, 0, 0);
    GameFlag_Set(0x916);
    Item_ShowFound(181, 3);
    Party_GiveItem(181, 0);
}

s32 FieldScene_DispatchBySceneId(void)
{
    s32 v;

    GameFlag_Set(0x87a);
    v = gGameState.scene;
    if (v == (s32)&Value_00000063) {
        Func_02002d24();
    } else if (v == (s32)&Value_00000066) {
        SceneState_CheckFlags941And940();
    } else if (v == (s32)&Value_00000099) {
        SceneState_ApplyFlagGatedActorEightSetup();
    } else if (v == (s32)&Value_0000009b) {
        SceneState_SetWork448AndRunFlag915Step();
    } else if (v == (s32)&Value_0000009c) {
        FieldScene_RunMiddleSequence();
    }
    return 0;
}

void SceneState_CheckFlags941And940(void)
{
    if (GameFlag_IsSet(0x941) != 0) {
        GameFlag_Set(0x321);
        GameFlag_Set(0x913);
        GameFlag_Set(0x912);
        GameFlag_Set(0x915);
    }
    if (GameFlag_IsSet(0x940) != 0) {
        GameFlag_Set(0x321);
    }
    if (gGameState.entrance != 0) {
        if (GameFlag_IsSet(0x912) == 0) {
            FieldScene_RunTwoActorCutsceneSequence();
        }
    }
}

void SceneState_ApplyFlagGatedActorEightSetup(void)
{
    struct Obj *o;

    if (GameFlag_IsSet(0xfd6) == 0) {
        InitializeOrbitingRenderEffect(12);
    }
    if (GameFlag_IsSet(0x915) != 0) {
        o = Actor_Get(8);
        o->f06 = 0;
    }
    if (gGameState.entrance == 10) {
        FieldScene_RunSecondaryGroupSequence();
    }
}

void SceneState_SetWork448AndRunFlag915Step(void)
{
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    if (GameFlag_IsSet(0x915) != 0) {
        s32 k = 2;
        Map_CopyCellsTo(58, 5, 58, 8, k, 3);
        { s32 a = 8, b = 10; Map_CopyCellAttributes(8, 11, 2, 1, a, b); }
        Map_CopyCellsTo(8, 12, 8, 11, k, 1);
        Map_Redraw();
        Task_Wait(1);
    }
    if (gGameState.entrance <= 3) {
        Func_0200567c(170);
    }
}

void FieldScene_RunMiddleSequence(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 rec7;
    u8 *record;
    s32 r0;
    s32 v5;
    u8 *p5;
    u8 **base = (u8 **)Data_03001ebc;

    *(s32 *)(base[0] + 0x1c0) = 0x204;
    Func_02005692(0);
    rec7 = GameFlag_IsSet(0x109);
    if (rec7 != 0) {
        p5 = base[9];
        r0 = GameFlag_IsSet(0x200);
        if (r0 != 0) {
            r0 = Value1_0200164c(Engine_ActorGet, 0);
        }
        *(s32 *)(p5 + 24) = r0;
    } else {
        GameFlag_Set(0x200);
        if (Data_02000240_t[225][0] == 4) {
            *(s32 *)(base[9] + 24) = rec7;
            GameFlag_Clear(0x200);
        }
    }
    if (GameFlag_IsSet(0x302) != 0) {
        Actor_SetPosition(11, 0x960000, 0x2d80000);
        if (GameFlag_IsSet(0x201) != 0) {
            Actor_Get(11);
            v5 = 9;
            Actor_SetAnimation(11, 5);
            Map_CopyCellAttributes(0, 0, 1, 1, v5, 14);
            Map_CopyCellAttributes(0, 0, 1, 1, v5, 45);
            {
                u8 *p = Func_02005606(11) + 35;
                s32 v = 2;

                v |= *p;
                *p = v;
            }
        }
    }
    record = Actor_Get(8);
    Actor_SetSpriteFlags((s32)record, 0);
    record = Actor_Get(9);
    Actor_SetSpriteFlags((s32)record, 0);
    Call2_0200164c(Func_02005532, 0x200b769, 0xc80);
    if (GameFlag_IsSet(0x915) != 0) {
        Actor_SetPosition(10, 0x1aa0000, 0x2da0000);
        record = Actor_Get(10);
        {
            s32 shown = 0x5000;

            *(u16 *)((s32)record + 6) = shown;
        }
        Map_CopyCellsTo(88, 48, 88, 45, 2, 3);
        Map_CopyCellsTo(24, 49, 24, 48, 2, 1);
        Map_CopyCellsTo(25, 42, 25, 47, 1, 1);
        Map_CopyCellAttributes(22, 50, 2, 1, 24, 49);
    }
    if (GameFlag_IsSet(0x302) == 0) {
    } else {
        Actor_SetPosition(8, 0xe80000, 0x2dc0000);
        Map_CopyCellAttributes(7, 44, 1, 1, 0, 1);
        Map_CopyCellsTo(74, 58, 78, 41, 1, 5);
        Map_CopyCellsTo(16, 109, 13, 109, 3, 2);
        Map_CopyCellsTo(67, 64, 71, 44, 1, 2);
        Map_CopyCellsTo(67, 64, 72, 44, 1, 2);
        Map_CopyCellsTo(67, 68, 73, 43, 1, 2);
        Map_CopyCellsTo(67, 68, 74, 43, 1, 2);
        Map_CopyCellsTo(67, 64, 75, 44, 1, 2);
        Map_CopyCellsTo(67, 66, 76, 44, 1, 2);
        Map_CopyCellsTo(67, 64, 77, 44, 1, 2);
        Map_CopyCellsTo(67, 64, 78, 44, 1, 2);
        Map_CopyCellsTo(67, 64, 79, 44, 1, 2);
        Map_CopyCellsTo(67, 66, 80, 44, 1, 2);
        Map_CopyCellsTo(2, 0, 9, 42, 2, 2);
        Map_CopyCellsTo(68, 64, 71, 44, 1, 2);
        Map_CopyCellsTo(68, 64, 72, 44, 1, 2);
        Map_CopyCellsTo(68, 68, 73, 43, 1, 2);
        Map_CopyCellsTo(68, 68, 74, 43, 1, 2);
        Map_CopyCellsTo(68, 64, 75, 44, 1, 2);
        Map_CopyCellsTo(68, 66, 76, 44, 1, 2);
        Map_CopyCellsTo(68, 64, 77, 44, 1, 2);
        Map_CopyCellsTo(68, 64, 78, 44, 1, 2);
        Map_CopyCellsTo(68, 64, 79, 44, 1, 2);
        Map_CopyCellsTo(68, 66, 80, 44, 1, 2);
        Map_CopyCellsTo(4, 0, 9, 42, 2, 2);
        Map_CopyCellsTo(7, 11, 7, 42, 10, 8);
        Map_CopyCellsTo(71, 12, 71, 43, 10, 13);
        Map_CopyCellAttributes(6, 13, 12, 12, 6, 44);
        Map_CopyCellAttributes(0, 1, 1, 1, 7, 44);
        goto L_02001cc2;
    }
    switch (Data_02000240_t[225][0]) {
    case 1:
    case 2:
        Func_02005a24(170);
        break;
    }
    L_02001cc2:;
    if (GameFlag_IsSet(0x303) == 0) {
    } else {
        Actor_SetPosition(9, 0x2b80000, 0x2dc0000);
        Map_CopyCellsTo(74, 58, 107, 41, 1, 5);
        Map_CopyCellsTo(45, 109, 42, 109, 3, 2);
        Map_CopyCellsTo(67, 64, 102, 44, 1, 2);
        Map_CopyCellsTo(67, 64, 103, 44, 1, 2);
        Map_CopyCellsTo(67, 64, 104, 44, 1, 2);
        Map_CopyCellsTo(67, 66, 105, 44, 1, 2);
        Map_CopyCellsTo(67, 64, 106, 44, 1, 2);
        Map_CopyCellsTo(67, 64, 107, 44, 1, 2);
        Map_CopyCellsTo(67, 64, 108, 44, 1, 2);
        Map_CopyCellsTo(67, 66, 109, 44, 1, 2);
        Map_CopyCellsTo(68, 64, 102, 44, 1, 2);
        Map_CopyCellsTo(68, 64, 103, 44, 1, 2);
        Map_CopyCellsTo(68, 64, 104, 44, 1, 2);
        Map_CopyCellsTo(68, 66, 105, 44, 1, 2);
        Map_CopyCellsTo(68, 64, 106, 44, 1, 2);
        Map_CopyCellsTo(68, 64, 107, 44, 1, 2);
        Map_CopyCellsTo(68, 64, 108, 44, 1, 2);
        Map_CopyCellsTo(68, 66, 109, 44, 1, 2);
        Map_CopyCellsTo(38, 14, 38, 44, 8, 4);
        Map_CopyCellsTo(102, 14, 102, 44, 8, 12);
        Map_CopyCellAttributes(37, 13, 10, 12, 37, 43);
        goto L_02001e5a;
    }
    switch (Data_02000240_t[225][0]) {
    case 3:
    case 4:
        Func_02005bbc(170);
        break;
    }
    L_02001e5a:;
}

void SceneState_SetValues27Through34(void)
{
    SceneEffect_SetSlotVariantAndDescriptor(27);
    SceneEffect_SetSlotVariantAndDescriptor(28);
    SceneEffect_SetSlotVariantAndDescriptor(29);
    SceneEffect_SetSlotVariantAndDescriptor(30);
    SceneEffect_SetSlotVariantAndDescriptor(32);
    SceneEffect_SetSlotVariantAndDescriptor(31);
    SceneEffect_SetSlotVariantAndDescriptor(33);
    SceneEffect_SetSlotVariantAndDescriptor(34);
}

void SceneEffect_SetSlotVariantAndDescriptor(s32 a)
{
    struct Obj_02000040 *p;
    u32 t;

    p = Actor_Get(a);
    p->f64 = a;
    t = Random_Next();
    p->f66 = (t * 5) >> 12;
    Object_SetScript(p, (s32)Data_0200beac);
}

/* Runs a scripted sequence for two actors (8 and 9): sets up their sprite
 * records, moves and animates them in lockstep through a series of timed
 * steps, then hands off to a third actor (2) and a couple of standalone
 * calls (5) before advancing the shared scene step counter and phase word. */
void FieldScene_RunTwoActorCutsceneSequence(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    u8 *record;

    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Task_Wait(1);
    Actor_SetPosition(0, 0xc00000, 0x1560000);
    Task_Wait(1);
    Camera_SetSpeed(0x3333, 0x666);
    Camera_MoveTo(0xc00000, -1, 0xfc0000, 1);
    SCENE_FIELD_1C8 = 40;
    Event_OpenScreen();
    Actor_SetSpeed(0, 0xcccc, 0x6666);
    Actor_WalkToAndWait(0, 192, 0x116);
    Actor_SetSpeed(8, 0x10000, 0x8000);
    Actor_SetSpeed(9, 0x10000, 0x8000);
    record = Actor_Get(8);
    {
        /* Field at +6 of the record: a visibility/state word. */
        s32 shown = 0x3000;

        *(u16 *)(record + 6) = shown;
    }
    record = Actor_Get(9);
    {
        /* Field at +6 of the record: a visibility/state word. */
        s32 shown = 0x5000;

        *(u16 *)(record + 6) = shown;
    }
    Task_Wait(1);
    /* Clear bit 0 of the flag byte at +90. */
    *(u8 *)(Scene_GetRecord_3(8) + 90) &= 254;
    *(u8 *)(Scene_GetRecord_4(9) + 90) &= 254;
    Actor_WalkTo(8, 184, 232);
    Actor_WalkToAndWait(9, 198, 232);
    Actor_SetAnimation(8, 1);
    Event_Wait(20);
    {
        /* Set bit 0 of the flag byte at +90. */
        u8 bits = 1;
        u8 *flags = Scene_GetRecord_5(8) + 90;
        u8 value = *flags;

        value |= bits;
        *flags = value;
        flags = Scene_GetRecord_6(9) + 90;
        bits |= *flags;
        *flags = bits;
    }
    Event_Wait(20);
    Actor_SetAnimationAndWait(8, 4);
    Event_SetMessage(MSG_LORD_HAMMETS_PALACE_LORD_AWAY);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_FaceDirection(9, 0x5000, 10);
    Actor_SetAnimationAndWait(9, 3);
    Event_ShowMessageAndWait(9, 0, 10);
    Actor_SetAttachedEffect(0, 0x102);
    Event_Wait(60);
    record = Scene_GetRecord_7(0);
    if (record != 0) {
        Actor_SetPosition(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Actor_SetSpeed(2, 0xcccc, 0x6666);
    Actor_WalkToAndWait(2, 212, 0x10c);
    Actor_FaceDirection(0, 0xe000, 0);
    Actor_FaceDirection(2, 0x6000, 20);
    Event_ShowMessageAndWait(0x4002, 0, 20);
    Actor_FaceDirection(0, 0xc000, 0);
    Actor_WalkToAndWait(2, 202, 254);
    Event_Wait(20);
    Actor_SetAnimationAndWait(2, 3);
    Event_Wait(10);
    Event_ShowMessageAndWait(2, 0, 10);
    Actor_ShowEmote(8, 0x101, 0);
    Actor_ShowEmote(9, 0x101, 40);
    Actor_FaceDirection(8, 0, 0);
    Actor_FaceDirection(9, 0x8000, 40);
    Actor_FaceDirection(8, 0x3000, 0);
    Actor_FaceDirection(9, 0x3000, 20);
    Actor_ShowEmote(2, 0x102, 60);
    Actor_RunRepeatedMotion(2, 1);
    Event_ShowMessageAndWait(2, 0, 10);
    Actor_RunRepeatedMotion(8, 2);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_ShowEmote(8, 0x105, 60);
    Actor_StartRepeatedMotion(8, 2);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_RunRepeatedMotion(9, 2);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_RunRepeatedMotion(2, 1);
    Event_Wait(10);
    Actor_FaceDirection(2, 0x6000, 10);
    Event_AskYesNo(0x4002, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(8, 1);
    Actor_SetAnimationAndWait(8, 4);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_ShowEmote(2, 0x101, 40);
    Actor_FaceDirection(2, 0xa000, 20);
    Actor_RunRepeatedMotion(9, 1);
    Actor_SetAnimationAndWait(9, 3);
    Actor_FaceDirection(2, 0xc000, 0);
    Event_ShowMessageAndWait(9, 0, 10);
    Actor_FaceDirection(2, 0x6000, 60);
    Actor_FaceDirection(2, 0xc000, 10);
    Event_ShowMessageAndWait(2, 0, 10);
    Actor_FaceDirection(8, 0, 0);
    Actor_FaceDirection(9, 0x8000, 40);
    Actor_FaceDirection(8, 0x3000, 0);
    Actor_FaceDirection(9, 0x3000, 10);
    Actor_RunRepeatedMotion(8, 1);
    Event_OpenMessage(8, 0);
    Actor_FaceDirection(2, 0x6000, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Actor_RunRepeatedMotion(2, 2);
        Event_ShowMessageAndWait(0x4002, 0, 10);
        bump_step_0200164c(1);
    } else {
        bump_step_0200164c(1);
        Actor_SetAnimationAndWait(2, 4);
        Event_ShowMessageAndWait(0x4002, 0, 10);
    }
    Actor_RunRepeatedMotion(9, 2);
    Event_ShowMessageAndWait(9, 0, 10);
    Actor_FaceDirection(2, 0xc000, 10);
    Actor_SetAnimationAndWait(2, 4);
    Event_ShowMessageAndWait(2, 0, 10);
    Actor_SetAttachedEffect(8, 0x102);
    Event_Wait(60);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_FaceDirection(2, 0xa000, 10);
    Actor_SetAnimationAndWait(2, 3);
    Event_ShowMessageAndWait(2, 0, 10);
    Actor_SetAttachedEffect(8, 0x102);
    Actor_SetAttachedEffect(9, 0x102);
    Event_Wait(60);
    Actor_RunRepeatedMotion(8, 2);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_RunRepeatedMotion(9, 2);
    Actor_FaceDirection(9, 0x8000, 10);
    Event_ShowMessageAndWait(9, 0, 10);
    Actor_FaceDirection(8, 0, 10);
    Actor_SetAnimationAndWait(8, 3);
    Actor_FaceDirection(8, 0x3000, 10);
    Event_ShowMessage(8, 0);
    Actor_FaceDirection(9, 0x3000, 10);
    Actor_SetAnimationAndWait(2, 3);
    Actor_FaceDirection(2, 0x6000, 10);
    Event_AskYesNo(0x4002, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(2, 1);
    Event_ShowMessageAndWait(0x4002, 0, 10);
    Actor_SetAnimationAndWait(0, 3);
    Actor_SetAnimationAndWait(2, 3);
    Actor_FaceDirection(2, 0xc000, 10);
    Actor_SetAnimationAndWait(2, 3);
    Call3_0200164c((void (*)())Engine_ActorSetSpeed, 8, 0xcccc, 0x6666);
    Call3_0200164c((void (*)())Engine_ActorSetSpeed, 9, 0xcccc, 0x6666);
    record = Actor_Get(8);
    {
        /* Field at +6 of the record: a visibility/state word. */
        s32 shown = 0;

        *(u16 *)(record + 6) = shown;
    }
    record = Actor_Get(9);
    {
        s32 shown = 0x8000;

        *(u16 *)(record + 6) = shown;
    }
    /* Clear bit 0 of the flag byte at +90. */
    *(u8 *)(Scene_GetRecord_8(8) + 90) &= 254;
    *(u8 *)(Scene_GetRecord_9(9) + 90) &= 254;
    Actor_WalkTo(8, 168, 232);
    Actor_WalkToAndWait(9, 212, 232);
    Actor_SetAnimation(8, 1);
    Event_Wait(20);
    {
        /* Set bit 0 of the flag byte at +90. */
        u8 bits = 1;
        u8 *flags = Scene_GetRecord_10(8) + 90;
        u8 value = *flags;

        value |= bits;
        *flags = value;
        flags = Scene_GetRecord_11(9) + 90;
        bits |= *flags;
        *flags = bits;
    }
    Actor_WalkToAndWait(2, 192, 232);
    Actor_FaceDirection(2, 0xc000, 0);
    Audio_PlayCue(188);
    Map_CopyCellsTo(36, 23, 43, 12, 2, 2);
    Task_Wait(5);
    Map_CopyCellsTo(39, 23, 43, 12, 2, 2);
    Task_Wait(5);
    Actor_WalkToAndWait(2, 192, 222);
    Actor_SetPosition(2, 0, 0);
    Actor_SetSpeed(0, 0x13333, 0x9999);
    Actor_WalkToAndWait(0, 192, 222);
    Actor_SetPosition(0, 0, 0);
    Actor_SetAnimation(8, 3);
    Actor_SetAnimationAndWait(9, 3);
    Actor_WalkTo(8, 184, 232);
    Actor_WalkToAndWait(9, 198, 232);
    Actor_WalkTo(8, 188, 212);
    Actor_WalkToAndWait(9, 194, 212);
    Actor_SetPosition(8, 0, 0);
    Actor_SetPosition(9, 0, 0);
    SCENE_FIELD_1C8 = 24;
    SCENE_FIELD_1C0 = 0x201;
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(5);
    Event_End();
}

void FieldScene_RunSecondaryGroupSequence(void)
{
    extern struct SceneWork *Data_03001ebc;

    u8 *fieldActor;
    u8 *object;
    struct SceneWork *work;
    u32 random;
    u32 motionPhase;
    const u8 *motionActions;
    s32 scale;
    const u8 *exitActions;

    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Task_Wait(1);
    Actor_SetPosition(0, 0, 0);
    Actor_SetPosition(9, 0x1b80000, 0x20a0000);
    object = Actor_Get(9);
    Actor_SetSpriteFlags(object, 0);
    Camera_MoveTo(0x1b80000, -1, 0x20a0000, 0);
    Map_Redraw();
    Task_Wait(1);
    Event_OpenScreen();
    Camera_SetSpeed(0x19999, 0x3333);
    Camera_MoveTo(0x1b80000, -1, 0x1900000, 1);
    Audio_PlayCue(141);
    Actor_SetSpeed(9, 0x19999, 0xcccc);
    Actor_WalkToAndWait(9, 0x1b8, 0x190);
    Camera_SetSpeed(0xc000, 0x1800);
    Camera_MoveTo(0x1b80000, -1, 0x12c0000, 1);
    Actor_SetSpeed(9, 0xcccc, 0x6666);
    Actor_WalkToAndWait(9, 0x1b8, 0x12c);
    Actor_SetAnimation(9, 0);
    Audio_PlayCue(0x121);
    Event_Wait(40);
    Actor_SetSpeed(11, 0xcccc, 0x6666);
    Actor_SetPosition(11, 0x1b70000, 0x1320000);
    Actor_Jump(11, 4, 0);
    Actor_WalkToAndWait(11, 0x1b7, 0x138);
    Actor_WalkToAndWait(11, 0x1a0, 0x138);
    Actor_WalkToAndWait(11, 0x190, 0x100);
    Actor_FaceDirection(11, 0x3000, 40);
    Camera_MoveTo(0x19a0000, -1, 0x1180000, 1);
    Actor_SetSpeed(10, 0x9999, 0x4ccc);
    Actor_SetPosition(10, 0x1b70000, 0x1320000);
    Actor_Jump(10, 4, 0);
    Actor_WalkToAndWait(10, 0x1b7, 0x138);
    Actor_WalkToAndWait(10, 0x1a0, 0x138);
    Actor_WalkToAndWait(10, 0x184, 0x10e);
    Actor_FaceDirection(10, 0xd000, 10);
    Actor_FaceDirection(11, 0x5000, 10);
    Actor_SetSpeed(0, 0xcccc, 0x6666);
    Actor_SetPosition(0, 0x1b70000, 0x1320000);
    Actor_Jump(0, 4, 0);
    Actor_WalkToAndWait(0, 0x1b7, 0x138);
    Actor_WalkToAndWait(0, 0x1a0, 0x138);
    Actor_WalkToAndWait(0, 0x184, 0x12c);
    Actor_FaceDirection(0, 0xc000, 40);
    fieldActor = Actor_Get(0);
    random = Random_Next();
    motionPhase = random * 5;
    motionActions = Data_0200c21c;
    fieldActor += 102;
    *(u16 *)fieldActor = motionPhase >> 12;
    Actor_EnableActionCallback(0, motionActions);
    Actor_Jump(11, 2, 20);
    Actor_SetAnimationAndWait(11, 3);
    Event_SetMessage(MSG_WEVE_ARRIVED_HAMMET);
    Event_ShowMessageAndWait(11, 0, 10);
    Actor_SetAnimationAndWait(10, 3);
    object = Actor_Get(0);
    if (object != 0) {
        Actor_SetPosition(1, *(s32 *)(object + 8), *(s32 *)(object + 16));
    }
    object = Actor_Get(0);
    if (object != 0) {
        Actor_SetPosition(2, *(s32 *)(object + 8), *(s32 *)(object + 16));
    }
    object = Actor_Get(0);
    if (object != 0) {
        Actor_SetPosition(3, *(s32 *)(object + 8), *(s32 *)(object + 16));
    }
    Actor_SetSpeed(1, 0xcccc, 0x6666);
    Actor_SetSpeed(2, 0xcccc, 0x6666);
    Actor_SetSpeed(3, 0xcccc, 0x6666);
    Actor_WalkTo(1, 0x17a, 0x136);
    Actor_WalkTo(2, 0x190, 0x120);
    Actor_WalkToAndWait(3, 0x19a, 0x134);
    Actor_SetAnimation(1, 1);
    Actor_SetAnimation(2, 1);
    Actor_FaceDirection(1, 0xc000, 0);
    Actor_FaceDirection(2, 0xc000, 0);
    Actor_FaceDirection(3, 0xc000, 40);
    fieldActor = Actor_Get(1);
    random = Random_Next();
    fieldActor += 102;
    *(u16 *)fieldActor = ((random * 5) >> 12);
    fieldActor = Actor_Get(2);
    random = Random_Next();
    fieldActor += 102;
    *(u16 *)fieldActor = ((random * 5) >> 12);
    fieldActor = Actor_Get(3);
    random = Random_Next();
    fieldActor += 102;
    *(u16 *)fieldActor = ((random * 5) >> 12);
    Actor_EnableActionCallback(1, motionActions);
    Actor_EnableActionCallback(2, motionActions);
    Actor_EnableActionCallback(3, motionActions);
    Actor_ShowEmote(2, 0x101, 60);
    Event_ShowMessageAndWait(0x2002, 0, 10);
    Actor_ShowEmote(10, 0x100, 40);
    Actor_FaceDirection(10, 0x3000, 20);
    Event_ShowMessageAndWait(10, 0, 10);
    Actor_Stop(0);
    Actor_Stop(1);
    Actor_Stop(2);
    Actor_Stop(3);
    Task_Wait(1);
    Actor_FaceDirection(2, 0xc000, 10);
    Actor_SetAnimationAndWait(2, 3);
    Actor_FaceDirection(0, 0xc000, 0);
    Actor_FaceDirection(1, 0xc000, 0);
    Actor_FaceDirection(3, 0xc000, 20);
    Actor_SetAnimationAndWait(10, 3);
    Event_ShowMessageAndWait(10, 0, 10);
    Actor_ShowEmote(3, 0x101, 60);
    Event_ShowMessageAndWait(0x2003, 0, 10);
    Actor_SetAnimationAndWait(10, 3);
    Event_ShowMessageAndWait(10, 0, 10);
    Actor_ShowEmote(1, 0x101, 60);
    Event_ShowMessageAndWait(1, 0, 10);
    Actor_FaceDirection(10, 0x5000, 0);
    Actor_RunRepeatedMotion(11, 2);
    Event_ShowMessageAndWait(11, 0, 10);
    Actor_FaceDirection(0, 0x6000, 0);
    Actor_FaceDirection(1, 0xe000, 0);
    Actor_FaceDirection(2, 0x2000, 0);
    Actor_FaceDirection(3, 0xa000, 20);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimation(2, 3);
    Actor_SetAnimationAndWait(3, 3);
    Event_Wait(10);
    Actor_FaceDirection(0, 0xc000, 0);
    Actor_FaceDirection(1, 0xc000, 0);
    Actor_FaceDirection(2, 0xc000, 0);
    Actor_FaceDirection(3, 0xc000, 10);
    Actor_FaceDirection(10, 0xd000, 10);
    Actor_SetAnimationAndWait(10, 3);
    Event_ShowMessageAndWait(10, 0, 10);
    Actor_SetAnimationAndWait(11, 3);
    Actor_SetAttachedEffect(2, 0x102);
    Event_Wait(40);
    Actor_StartRepeatedMotion(2, 2);
    Event_ShowMessageAndWait(0x2002, 0, 10);
    Actor_FaceDirection(11, 0x3000, 10);
    Actor_SetAnimation(11, 4);
    Event_ShowMessageAndWait(11, 0, 10);
    Actor_ShowEmote(3, 0x101, 40);
    Event_ShowMessageAndWait(0x2003, 0, 10);
    Actor_FaceDirection(10, 0x3000, 10);
    Actor_ShowEmote(10, 0x108, 20);
    Event_ShowMessageAndWait(10, 0, 10);
    Actor_RunRepeatedMotion(1, 2);
    Event_ShowMessageAndWait(1, 0, 10);
    Actor_FaceDirection(11, 0x5000, 0);
    Actor_FaceDirection(10, 0x5000, 10);
    Actor_SetAnimation(11, 4);
    Event_ShowMessageAndWait(11, 0, 10);
    Actor_FaceDirection(0, 0x6000, 0);
    Actor_FaceDirection(1, 0xe000, 40);
    Actor_FaceDirection(0, 0xc000, 0);
    Actor_FaceDirection(1, 0xc000, 10);
    Actor_SetAnimationAndWait(11, 4);
    Event_ShowMessageAndWait(11, 0, 10);
    Actor_SetAnimationAndWait(10, 4);
    Event_ShowMessageAndWait(10, 0, 10);
    Actor_ShowEmote(0, 0x105, 0);
    Actor_ShowEmote(1, 0x105, 0);
    Actor_ShowEmote(2, 0x105, 0);
    Actor_ShowEmote(3, 0x105, 60);
    Actor_FaceDirection(10, 0x3000, 10);
    Actor_SetAnimation(10, 3);
    Event_OpenMessage(10, 0);
    Actor_FaceDirection(1, 0xe000, 0);
    Actor_FaceDirection(2, 0x6000, 0);
    Actor_FaceDirection(3, 0xa000, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(20);
        Data_03001ebc->step += 3;
    } else {
        Event_Wait(20);
        Actor_RunRepeatedMotion(11, 2);
        Event_ShowMessageAndWait(11, 0, 40);
        Event_ShowMessageAndWait(11, 0, 10);
        Actor_ShowEmote(3, 0x106, 40);
        Actor_FaceDirection(3, 0xc000, 20);
        Event_ShowMessageAndWait(0x2003, 0, 10);
    }
    Actor_FaceDirection(1, 0xc000, 0);
    Actor_FaceDirection(2, 0xc000, 0);
    Actor_FaceDirection(3, 0xc000, 10);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimation(2, 3);
    Actor_SetAnimationAndWait(3, 3);
    Event_Wait(20);
    Actor_FaceDirection(8, 0x3000, 0);
    Actor_SetAnimationAndWait(10, 3);
    Camera_SetSpeed(0x10000, 0x2000);
    Camera_MoveTo(0x1180000, -1, 0xc80000, 1);
    Actor_WalkToAndWait(10, 0x14d, 222);
    Actor_WalkToAndWait(10, 0x11c, 198);
    Actor_FaceDirection(10, 0x8000, 10);
    Actor_SetAnimationAndWait(10, 3);
    Actor_FaceDirection(8, 0, 10);
    Actor_SetAnimationAndWait(8, 3);
    Actor_SetSpeed(8, 0xcccc, 0x6666);
    Actor_WalkToAndWait(8, 0x10c, 198);
    Actor_FaceDirection(8, 0xc000, 10);
    Actor_RunRepeatedMotion(8, 2);
    Audio_PlayCue(125);
    Map_CopyCellsTo(71, 60, 76, 11, 2, 1);
    Map_CopyCellAttributes(71, 60, 2, 1, 16, 11);
    Event_Wait(20);
    Actor_WalkToAndWait(8, 246, 198);
    Actor_FaceDirection(8, 0, 20);
    *(u8 *)(Func_02003d1c_a() + 85) = 0;
    Camera_SetSpeed(0x9999, 0x1333);
    Camera_MoveTo(0xf80000, -1, 0xaa0000, 1);
    Actor_WalkToAndWait(10, 0x10e, 198);
    Actor_WalkToAndWait(10, 0x10e, 174);
    Actor_WalkToAndWait(10, 224, 170);
    Actor_WalkToAndWait(10, 210, 158);
    Actor_WalkToAndWait(10, 246, 148);
    Actor_WalkToAndWait(10, 246, 142);
    Actor_SetPosition(10, 0, 0);
    Data_03001ebc->request = 514;
    Event_CloseScreen();
    Event_WaitForScreen();
    Actor_SetPosition(9, 0x1b80000, 0x1540000);
    object = Actor_Get(9);
    scale = 0x4000;
    *(u16 *)(object + 6) = scale;
    Camera_MoveTo(0x17c0000, -1, 0x1180000, 0);
    Map_Redraw();
    Task_Wait(10);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(40);
    Actor_SetAnimationAndWait(11, 3);
    Event_Wait(20);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimation(2, 3);
    ((void (*)())Engine_ActorSetAnimationAndWait)(3, 3);
    Actor_WalkToAndWait(11, 0x1a4, 0x11a);
    Actor_WalkToAndWait(11, 0x1a4, 0x138);
    Actor_WalkToAndWait(11, 0x1b7, 0x138);
    Actor_WalkToAndWait(11, 0x1b7, 0x132);
    Actor_SetPosition(11, 0, 0);
    exitActions = Data_0200c230;
    Actor_EnableActionCallback(1, exitActions);
    Actor_EnableActionCallback(2, (s32)exitActions);
    Call2_020026c0(Func_02003c4c, 3, (s32)exitActions);
    Camera_MoveTo(0x19a0000, -1, 0x12c0000, 1);
    Actor_WalkToAndWait(0, 0x1a0, 0x138);
    Actor_WalkToAndWait(0, 0x1b7, 0x138);
    Actor_WalkToAndWait(0, 0x1b7, 0x132);
    Actor_SetPosition(0, 0, 0);
    Audio_PlayCue(141);
    Actor_SetSpeed(9, 0xcccc, 0x6666);
    Camera_SetSpeed(0x10000, 0x2000);
    Camera_MoveTo(0x1b80000, -1, 0x1a40000, 1);
    Actor_WalkToAndWait(9, 0x1b8, 0x1a4);
    Camera_SetSpeed(0x20000, scale);
    Camera_MoveTo(0x1b80000, -1, 0x2580000, 1);
    Actor_SetSpeed(9, 0x19999, 0xcccc);
    Actor_WalkToAndWait(9, 0x1b8, 0x1f4);
    Actor_WalkTo(9, 0x1b8, 0x258);
    Audio_PlayCue(0x121);
    work = Data_03001ebc;
    work->setup = 24;
    work->request = 0x100;
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(10);
    Event_End();
}

s32 *SceneActor_FindAtTileXZ(s32 x, s32 z)
{
    extern u8 *Data_03001ebc;

    s32 **tbl = (s32 **)(Data_03001ebc + 0x14);
    u32 i;

    for (i = 8; i <= 65; i++) {
        s32 *p = tbl[i];

        if (x == (p[2] >> 20) && z == (p[4] >> 20)) {
            return p;
        }
    }
    return 0;
}

void FieldScene_RunLateSequence(void)
{
    s32 tmp[3];
    s32 record;
    s32 rec;
    s32 idx;
    s32 w;
    s32 w2;
    s32 a;
    s32 *dst;
    s32 zero;
    s32 k;

    record = Value1_0200164c(Engine_ActorGet, 0);
    idx = (s32)((u32)*(u16 *)(record + 6) >> 12);
    a = *(s16 *)(record + 10);
    w = Data_0200bd84[idx];
    rec = Value2(SceneActor_FindAtTileXZ,
                 (a + (w >> 16)) >> 4,
                 (*(s16 *)(record + 18) + (s32)(s16)w) >> 4);
    if (rec != 0) {
        zero = 0;
        *(u8 *)(rec + 34) = 2;
        dst = tmp;
        w2 = Data_0200bd84[idx];
        dst[0] = *(s32 *)(rec + 8) + (w2 & -0x10000);
        dst[1] = *(s32 *)(rec + 12);
        dst[2] = *(s32 *)(rec + 16) + (w2 << 16);
        if (Value2(Func_02006dd2, rec, (s32)dst) <= 0) {
            Object_SetAnimation(record, 8);
            k = 0x3333;
            Task_Wait(15);
            Audio_PlayCue(185);
            *(s32 *)(rec + 48) = k;
            *(s32 *)(rec + 52) = k;
            Func_02006dd4(rec, dst[0], dst[1], dst[2]);
            *(s32 *)(record + 48) = k;
            *(s32 *)(record + 52) = k;
            Func_02006de6(record, dst[0], dst[1], dst[2]);
            Func_02006df4(rec);
            Func_02006fd8();
            *(s32 *)(rec + 8) = dst[0];
            *(s32 *)(rec + 16) = dst[2];
            *(s32 *)(rec + 36) = zero;
            *(s32 *)(rec + 44) = zero;
            Object_SetAnimation(record, 1);
            Func_0200652a();
        }
    }
}

void Func_020032a4(void)
{
    s32 rec8;
    s32 rec4;
    s32 rec7;
    s32 rec2;
    u32 i;
    s32 v5;
    s32 v6;
    s32 v7;

    rec8 = Value1_0200164c(Engine_ActorGet, 8);
    rec4 = Value1_0200164c(Engine_ActorGet, 9);
    rec7 = GameFlag_IsSet(0x302);
    if (rec7 != 0) {
    } else if ((*(s32 *)(rec8 + 8) >> 19) > 29) {
    } else {
        rec2 = Value1_0200164c(Engine_ActorGet, 11);
        Event_Begin();
        Map_CopyCellAttributes(7, 44, 1, 1, rec7, 1);
        i = 67;
        v7 = 1;
        v6 = 5;
        do {
            Map_CopyCellsTo(i, 58, 78, 41, v7, v6);
            Event_Wait(4);
            if (i == 70) {
                GameFlag_Set(0x302);
            }
            i++;
        } while (i <= 74);
        v5 = 2;
        Map_CopyCellsTo(16, 109, 13, 109, 3, v5);
        Event_Wait(40);
        *(s32 *)(rec2 + 24) = 0x1999;
        *(s32 *)(rec2 + 28) = 0x1999;
        Actor_SetPosition(11, 0x960000, 0x2d80000);
        Value2(Engine_ActorEnableActionCallback, 11, 0x200c268);
        v6 = 1;
        Map_CopyCellsTo(67, 64, 71, 44, v6, v5);
        Map_CopyCellsTo(67, 64, 72, 44, v6, v5);
        Map_CopyCellsTo(67, 68, 73, 43, v6, v5);
        Map_CopyCellsTo(67, 68, 74, 43, v6, v5);
        Map_CopyCellsTo(67, 64, 75, 44, v6, v5);
        Map_CopyCellsTo(67, 66, 76, 44, v6, v5);
        Map_CopyCellsTo(67, 64, 77, 44, v6, v5);
        Map_CopyCellsTo(67, 64, 78, 44, v6, v5);
        Map_CopyCellsTo(67, 64, 79, 44, v6, v5);
        Map_CopyCellsTo(67, 66, 80, 44, v6, v5);
        Map_CopyCellsTo(2, 0, 9, 42, v5, v5);
        Event_Wait(40);
        Map_CopyCellsTo(68, 64, 71, 44, v6, v5);
        Map_CopyCellsTo(68, 64, 72, 44, v6, v5);
        Map_CopyCellsTo(68, 68, 73, 43, v6, v5);
        Map_CopyCellsTo(68, 68, 74, 43, v6, v5);
        Map_CopyCellsTo(68, 64, 75, 44, v6, v5);
        Map_CopyCellsTo(68, 66, 76, 44, v6, v5);
        Map_CopyCellsTo(68, 64, 77, 44, v6, v5);
        Map_CopyCellsTo(68, 64, 78, 44, v6, v5);
        Map_CopyCellsTo(68, 64, 79, 44, v6, v5);
        Map_CopyCellsTo(68, 66, 80, 44, v6, v5);
        Map_CopyCellsTo(4, 0, 9, 42, v5, v5);
        Event_Wait(40);
        v5 = 10;
        Map_CopyCellsTo(7, 11, 7, 42, v5, 8);
        Map_CopyCellsTo(71, 12, 71, 43, v5, 13);
        v5 = 44;
        Map_CopyCellAttributes(6, 13, 12, 12, 6, v5);
        Event_Wait(40);
        SceneState_LinkRecordZeroWhenFlag200Clear();
        Map_CopyCellAttributes(0, 1, 1, 1, 7, v5);
        Event_End();
    }
    if (GameFlag_IsSet(0x303) != 0) {
    } else if ((*(s32 *)(rec4 + 8) >> 19) > 87) {
    } else {
        Event_Begin();
        i = 67;
        v7 = 1;
        v6 = 5;
        do {
            Map_CopyCellsTo(i, 58, 107, 41, v7, v6);
            Event_Wait(4);
            if (i == 70) {
                GameFlag_Set(0x303);
            }
            i++;
        } while (i <= 74);
        v6 = 2;
        Map_CopyCellsTo(45, 109, 42, 109, 3, v6);
        Event_Wait(40);
        v5 = 1;
        Map_CopyCellsTo(67, 64, 102, 44, v5, v6);
        Map_CopyCellsTo(67, 64, 103, 44, v5, v6);
        Map_CopyCellsTo(67, 64, 104, 44, v5, v6);
        Map_CopyCellsTo(67, 66, 105, 44, v5, v6);
        Map_CopyCellsTo(67, 64, 106, 44, v5, v6);
        Map_CopyCellsTo(67, 64, 107, 44, v5, v6);
        Map_CopyCellsTo(67, 64, 108, 44, v5, v6);
        Map_CopyCellsTo(67, 66, 109, 44, v5, v6);
        Event_Wait(40);
        Map_CopyCellsTo(68, 64, 102, 44, v5, v6);
        Map_CopyCellsTo(68, 64, 103, 44, v5, v6);
        Map_CopyCellsTo(68, 64, 104, 44, v5, v6);
        Map_CopyCellsTo(68, 66, 105, 44, v5, v6);
        Map_CopyCellsTo(68, 64, 106, 44, v5, v6);
        Map_CopyCellsTo(68, 64, 107, 44, v5, v6);
        Map_CopyCellsTo(68, 64, 108, 44, v5, v6);
        Map_CopyCellsTo(68, 66, 109, 44, v5, v6);
        Event_Wait(40);
        v5 = 8;
        Map_CopyCellsTo(38, 14, 38, 44, v5, 4);
        Map_CopyCellsTo(102, 14, 102, 44, v5, 12);
        Map_CopyCellAttributes(37, 13, 10, 12, 37, 43);
        Event_Wait(40);
        SceneState_LinkRecordZeroWhenFlag200Clear();
        Event_End();
    }
}

void SceneEffect_UpdateMotionWithDamping(struct Obj_020036f8 *p)
{
    s16 *h;
    s32 v;
    s32 a;
    s32 b;

    h = &p->f64;
    v = *h;
    if (v == 0) {
        Func_0200727e_a();
    } else if (v == 1) {
        p->f24 = 0;
        p->f28 = 0;
        p->f08 = 0;
        p->f0c = 0;
    } else {
        p->f18 += 0x800;
        p->f1c += 0x800;
    }
    p->f08 += p->f24;
    p->f0c += p->f28;
    a = p->f24;
    b = p->f28;
    p->f24 = a - a / 256;
    p->f28 = b - b / 16;
    { s32 t = *(u16 *)h; t -= 1; *(u16 *)h = t; }
}

void FieldScene_RunSupplementalSequenceOne(s32 a0)
{
    s32 rec7;
    s32 rec8;
    s32 record;
    s32 mask;
    s32 c12;
    s32 old;
    u8 *rp;
    u8 *p4;

    rec7 = Value1_0200164c(Engine_ActorGet, 8);
    rec8 = Actor_Get(9);
    if ((u32)(*(s16 *)(rec7 + 10) + -0x17d) <= 12) {
        if (*(s16 *)(rec7 + 18) <= 0x309) {
            goto L_020037ae;
        }
        record = Value1_0200164c(Engine_ActorGet, 0);
        p4 = (u8 *)*(s32 *)(rec7 + 80);
        rp = (u8 *)*(s32 *)(record + 80);
        c12 = 12 & rp[9];
        old = p4[9];
        mask = -13;
        mask &= old;
        p4[9] = (mask | c12);
    } else {
        L_020037ae:;
        if (GameFlag_IsSet(0x302) == 0) {
            if (*(s16 *)(rec7 + 10) <= 245) {
                if ((*(volatile s32 *)0x03001e40 & 1) == 0) {
                    if (GameFlag_IsSet(0x202) == 0) {
                        Call1_02003768(Func_02007540, -1);
                        Audio_PlayCue(230);
                        GameFlag_Set(0x202);
                    }
                    Func_02007056(*(s32 *)(rec7 + 8), *(s32 *)(rec7 + 12), *(s32 *)(rec7 + 16));
                }
            }
        }
    }
    if (GameFlag_IsSet(0x303) == 0) {
        if (*(s16 *)(rec8 + 10) <= 0x2c5) {
            if ((*(volatile s32 *)0x03001e40 & 1) == 0) {
                if (GameFlag_IsSet(0x203) == 0) {
                    Call1_02003768(Func_02007588, -1);
                    Audio_PlayCue(230);
                    GameFlag_Set(0x203);
                }
                Func_0200709e(*(s32 *)(rec8 + 8), *(s32 *)(rec8 + 12), *(s32 *)(rec8 + 16));
            }
        }
    }
}

void Func_02003864(s32 a0, s32 a1, s32 a2)
{
    s32 p8;
    u8 *rec7;
    s32 value;
    s32 mask;
    u8 *link;

    p8 = a2;
    value = Value0(Engine_RandomNext);
    rec7 = (u8 *)Value4(Func_020073fc, 222, (a0 + -0x80000), (((((u32)(value << 3) >> 16) << 16) + a1) + 0x100000), p8);
    if ((s32)rec7 != 0) {
        rec7[85] = (mask = 0);
        link = (u8 *)*(s32 *)((s32)rec7 + 80);
        mask -= 13;
        link[9] = ((link[9] & mask) | 8);
        Object_SetPalette((s32)rec7, 9);
        Actor_SetSpriteFlags((s32)rec7, 0);
        value = Random_Next();
        *(s32 *)((s32)rec7 + 36) = ((((u32)(value << 1) >> 16) - 1) << 16);
        value = Value0(Engine_RandomNext);
        *(s32 *)((s32)rec7 + 40) = ((((u32)(((value << 1) + value) << 1) >> 16) - 3) << 16);
        {
            u16 *target = (u16 *)((s32)rec7 + 100);
            s32 shown = 20;

            *target = shown;
            *((u8 *)target - 3) = 1;
        }
        Object_SetAnimation((s32)rec7, 1);
        Engine_ObjectSetScript((s32)rec7, 0x200d120);
    }
}

void SceneEffect_UpdateLobeOrbitEffect26(void)
{
    Effect_0200390c *effect = Actor_Get(26);
    RenderData *render = effect->render;
    s32 offset = Math_Sin(effect->angle) * 2;
    s32 first;

    if (offset > 0) {
        offset = -offset;
    }
    effect->x = effect->base_x + Math_Cos(effect->angle) * 2;
    effect->y = effect->base_y + offset;
    render->rotation = Math_Cos(effect->angle + 0x8000) >> 3;
    first = Random_Next();
    effect->angle +=
        ((u32)(first << 9) >> 16)
        + ((u32)(Random_Next() << 9) >> 16)
        + 0x400;
}

void FieldScene_DrawTilesAndRaiseActor11(void)
{
    Effect *effect = Actor_Get(11);

    Map_CopyCellAttributes(0, 0, 1, 1, 9, 14);
    Map_CopyCellAttributes(0, 0, 1, 1, 9, 45);
    if (effect != 0) {
        Actor_SetSpriteFlags(effect, 0);
        effect->y -= 0x200000;
        effect->state23 = 2;
    }
    GameFlag_Set(0x201);
}

s32 SceneEffect_UpdateOrbitingEffect(Effect_0200390c *effect)
{
    RenderData *render = effect->render;
    s32 ofs = Math_Sin(effect->angle) * 2;
    s32 first;

    if (ofs > 0) {
        ofs = -ofs;
    }
    effect->x = effect->base_x + Math_Cos(effect->angle) * 2;
    effect->y = effect->base_y + ofs;
    render->rotation = Math_Cos(effect->angle + 0x8000) / 8;
    first = Random_Next();
    effect->angle +=
        ((u32)(first << 9) >> 16)
        + ((u32)(Random_Next() << 9) >> 16)
        + 0x400;
    return 0;
}

void InitializeOrbitingRenderEffect(void)
{
    OrbitingSceneObject *actor;
    OrbitingSceneObjectSprite *sprite;
    u8 *transfer;
    s32 zero;

    actor = GetOrbitingSceneObject();
    sprite = actor->sprite;
    sprite->flags_09_mode = 1;
    sprite->flags_05_bit_5 = 0;
    sprite->flags_09_high = 0;

    zero = 0;
    sprite->state = zero;
    Actor_SetSpriteFlags(actor, zero);
    actor->active = zero;
    actor->mode = zero;

    if (GameFlag_IsSet(0x109) == 0)
        actor->y += 0x200000;

    actor->flags_23 &= 0xfe;
    actor->visible = 1;

    transfer = AllocateEffectTransfer(17, 0x608);
    Item_LoadIcon(181);
    transfer += 0x400;
    Vram_Load(sprite->palette, 128, transfer);
    Heap_Release(17);

    actor->orbit_center_x = actor->x;
    actor->orbit_angle = zero;
    actor->orbit_center_y = actor->y;
    actor->active = 1;
    actor->callback = (u32)&UpdateOrbitingSceneObject;
    actor->state = zero;
}
