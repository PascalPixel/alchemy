#include "TYPES.H"
#include "FIELD_EVENT.H"

#define UnsignedRemainder Func_020046fa
#define ACTOR_ID 22
#define ObjectMotion_EnableActionAndSetCallback_1(a0, a1) Value2(Engine_ActorEnableActionCallback, a0, a1)
#define ObjectMotion_MarkActiveAndSetActionCallback_1(a0, a1) Call2(Func_020070aa, a0, a1)
#define BattleEffect_PlayQueuedSound_1(args...) Func_02007232(args)
#define Scene_GetRecord_1_020028a4(a0) Value1(Engine_ActorGet, a0)
#define Scene_GetRecord_2(a0) Value1(Engine_ActorGet, a0)
#define Scene_GetRecord_3(a0) Value1(Engine_ActorGet, a0)
#define ObjectMotion_EnableActionAndSetCallback_3(a0, a1) Value2(Engine_ActorEnableActionCallback, a0, a1)
#define Object_LookupAndStep_1(args...) Func_02007176(args)
#define BattleRuntime_WaitIfModeZero_3(args...) ((void (*)())Engine_EventWait)(args)
#define BattleEffect_PlayQueuedSound_2(args...) Func_020072f6(args)
#define BattleEffect_PlayQueuedSound_3(args...) Func_02007320(args)
#define Object_LookupAndStep_2(args...) Func_020071f2(args)
#define BattleEffect_PlayQueuedSound_4(args...) Func_02007356(args)
#define ObjectMotion_EnableActionAndSetCallback_7(a0, a1) Value2(Engine_ActorEnableActionCallback, a0, a1)
#define Object_LookupAndStep_3(args...) Func_0200726e(args)
#define Scene_GetRecord_4(a0) Value1(Engine_ActorGet, a0)

struct Object {
    u8 filler00[6];
    u16 x;
    u8 filler08[92];
    s16 counter;
};

typedef struct Obj {
    u8 pad00[8];
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 pad14[4];
    s32 f18;
    s32 f1c;
    u8 pad20[0x44];
    u16 f64;
    u8 pad66[2];
    struct Obj *f68;
} Obj;

typedef struct Ent {
    u8 pad00[9];
    u8 b01:2;
    u8 f:2;
    u8 b45:4;
} Ent;

typedef struct Rec {
    u8 pad00[0x50];
    Ent *f50;
} Rec;

struct FixedPointCountdown {
    u8 pad_00[0x18];
    s32 fixed_point_18;
    s32 fixed_point_1c;
    u8 pad_20[0x44];
    s16 countdown;
};

typedef struct {
    u8 filler0[8];
    s32 unk8;
    u8 fillerC[4];
    s32 unk10;
} Thing1;

typedef struct {
    u8 filler0[10];
    s16 unkA;
    u8 fillerC[6];
    s16 unk12;
} Thing2;

struct Actor {
    u8 reserved_00[80];
    u8 *presentation;
};

extern u8 Data_0200c8bc[];
extern u8 Data_0200d0e4[];
extern u8 Data_0200d27c[];
extern u8 Data_0200d2b8[];
extern u8 Data_0200d558[];
extern u8 Data_0200d774[];
extern u8 Value_00000101;
extern u8 Data_0200d78a[];
extern u8 Value_000002d7;
extern u8 Value_00000205;
extern u8 Value_00000109;
extern u8 Value_00000206;
extern u8 Value_00000831;
extern u8 Value_00000832;
extern u8 Value_00000833;
extern u8 Value_00000837;
extern u8 Value_00000838;
extern u8 Value_00000841;
extern u8 Value_00000842;
extern u8 Value_0000083a;
extern u8 Value_0000087b;
extern u8 Value_00000834;
extern u8 Data_0200c9f4[];
extern u8 Data_0200cec8[];
extern u8 Data_0200c5b9[];
extern u8 Data_02000240[];
extern u8 Data_03001ebc[];
extern u8 Data_00000e5c[];
extern u8 Data_00000e67[];
extern u8 Data_00000ed0[];
extern u32 Data_03001e40;
extern u8 Data_00000e74[];
extern s32 Data_0200d7f8;
extern u8 Value_00000e8c;
extern u8 Value_0000201a;
extern u8 Value_00001120;
extern u8 Data_00000e9b[];
extern u8 Data_00000ea1[];
extern u8 Value_0000030d;
s32 Func_02004684(s32, s32);
s32 Func_02008950(s32, s32);
s32 Func_02008966(s32, s32);
void Func_02008758(s32);
s32 Func_02008998(s32, s32);
Rec *Func_02008cea(s32);
Rec *Func_02008cf2(s32);
Rec *Func_02008d0e(s32);
u32 Func_020046fa(u32 value, u32 divisor);
void Func_02004ce4(s32);
s32 Func_02004bea(s32, s32);
s32 Func_02004c24(s32, s32);
s32 Func_02004c5a(s32, s32);
s32 Func_02004c92(s32, s32);
void Func_02004cea(u8 *, s32);
s32 Func_02004d50(s32, s32);
s32 Func_02004d92(s32, s32);
s32 Func_02004dd2(s32, s32);
void Func_020050bc(void);
void Func_020050c8(void);
void Func_02005248(void);
void Func_02002df0(void);
void Func_020049c8(void);
s32 Func_02008aa0(s32, s32);
s32 Func_02008ae4(s32, s32);
void Func_020088fe(void);
void Func_02005264_a();
void Func_02005270();
void Func_02005284();
void Func_020052ea();
void Func_0200537c();
void Func_02005acc();
void Func_02005d2e();
void Func_02005e24();
void Func_02002e5e();
s32 Func_0200891e(s32, s32);
void Func_02008710(s32);
s32 Func_02005ff6();
void Func_0200606c();
void Func_02006246();
void Func_0200630e();
void Func_02006318();
s32 Func_02006350();
void Func_0200635e();
void Func_02006632();
void Func_02006638_a();
void Func_0200686c();
void Func_0200c56d();
s32 Func_020067f6(s32, s32);
s32 Func_02006850(s32, s32);
s32 Func_02006890(s32, s32);
void Func_02006c84(void);
void Func_02006db4(void);
void Func_02005116(void);
void Func_02008300();
void Func_02008306();
void Func_0200830c();
void Func_02008312();
void Func_02008318();
void Func_0200831e();
void Func_02008356();
void Func_0200835c();
void Func_02008362();
void Func_02008368();
void Func_0200836e();
void Func_02008374();
void Func_0200861e_a();
void Func_0200887a_a(void);
void Func_02008884_a(void);
void Func_0200888e(void);
void Func_020088a0(void);
s32 Func_02007008();
s32 Func_02007020();
s32 Func_02007038();
void Func_020070aa();
void Func_02007176();
void Func_020071f2();
void Func_02007232();
void Func_0200726e();
void Func_020072f6();
void Func_02007320();
void Func_02007356();
s32 Func_020073ea(s32, s32);
void Func_02008e00(void);

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
void SceneActor_RunActor22PlacementSequence(s32 x, s32 y);

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

static __inline__ void Call3_02000398(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call3_020003cc(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call3_02000400(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* Configures actor 22 (position, pose, and movement/sprite flags) for the
 * scene. */

static __inline__ void Call1_02001828(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call6_02001828(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ s32 Value1_020028a4(s32 (*f)(), s32 a0)
{
    return f(a0);
}

s32 OverlayObject_SetField6OnCountdown(struct Object *object)
{
    s32 loaded = object->counter;
    s32 counter = (s16)loaded;

    if (loaded == 0) {
        object->x = Random_Next();
        counter = Func_02004684(Random_Next(), 20) + 20;
        object->counter = counter;
    }
    object->counter = counter - 1;
    return 1;
}

s32 UpdateFixedPointCountdown(struct FixedPointCountdown *state)
{
    switch (state->countdown) {
    case 6:
        state->fixed_point_18 += (s32) 0xFFFFC000;
        state->fixed_point_1c += 0x2000;
        break;
    case 4:
        state->fixed_point_18 += 0x2000;
        state->fixed_point_1c += -0x1000;
        break;
    case 2:
        state->fixed_point_18 += 0x1000;
        state->fixed_point_1c += (s32) 0xFFFFF800;
        break;
    case 0:
        state->fixed_point_18 = 0x10000;
        state->fixed_point_1c = 0x10000;
        state->countdown = (s16)(UnsignedRemainder(Random_Next(), 90) + 60);
        break;
    }
    state->countdown--;
    return 1;
}

u8 *SceneData_GetTableD0E4(void)
{
    return Data_0200d0e4;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetTableD27c(void)
{
    return Data_0200d27c;
}

u8 *SceneData_GetTableD2B8(void)
{
    return Data_0200d2b8;
}

u8 *SceneData_GetTableD558(void)
{
    return Data_0200d558;
}

void SceneState_SetFlag210AndConfigureRegion40_84(void)
{
    s32 a;
    s32 b;

    GameFlag_Set(0x210);
    a = 10;
    b = 84;
    Map_CopyCellAttributes(40, 84, 7, 4, a, b);
}

void SceneState_SetFlag210AndConfigureRegion40_89(void)
{

    s32 a;
    s32 b;

    GameFlag_Clear(0x210);
    a = 10;
    b = 84;
    Map_CopyCellAttributes(40, 89, 7, 4, a, b);
}

void SceneState_SetWork1c0AndRunObject(u8 *o)
{

    u8 *state;

    if (GameFlag_IsSet((s32)&Value_00000834) != 0) {
        Func_020049c8();
    }
    state = *(u8 **)0x03001ebc;
    *(s32 *)(state + 0x1C0) = 0x100;
    *(s32 *)(state + 0x1C8) = 16;
    Event_RequestExit(o);
}

void FieldScene_SetupDescriptorD774(void)
{
    Audio_PlayCue(0x9E);
    Map_AnimateCells(Data_0200d774, 45, 11);
    Actor_WalkTo(0, (s32)&Value_00000101, 0x1A4);
    Event_Wait(3);
    SceneState_SetWork1c0AndRunObject(11);
}

void SceneState_SetValue123Mode1(void)
{

    Audio_PlayCue(0x7B);
    SceneState_SetWork1c0AndRunObject(1);
}

void SceneState_ApplyValues123And3(void)
{

    Audio_PlayCue(0x7B);
    SceneState_SetWork1c0AndRunObject(3);
}

void SceneState_SetValue123Mode4(void)
{

    Audio_PlayCue(0x7B);
    SceneState_SetWork1c0AndRunObject(4);
}

void FieldScene_RunStep7BAndCheckFlags841And842(void)
{
    Audio_PlayCue(0x7B);
    if (GameFlag_IsSet((s32)&Value_00000841) != 0
        && GameFlag_IsSet((s32)&Value_00000842) == 0) {
        FieldScene_ConfigureActorTwentyTwoScene();
    }
    SceneState_SetWork1c0AndRunObject(2);
}

void FieldScene_SetupDescriptorD78a(void)
{
    Audio_PlayCue(0x9E);
    Map_AnimateCells(Data_0200d78a, 54, 32);
    Engine_ActorWalkTo(0, 0x196, (s32)&Value_000002d7);
    Event_Wait(3);
    SceneState_SetWork1c0AndRunObject(5);
}

void FieldScene_RunScene372_02000278(void)
{
    u32 i;
    s32 record;

    if (GameFlag_IsSet(0x206) == 0) {
        Audio_PlayCue(158);
        Map_AnimateCells(0x200d7a0, 45, 39);
    }
    if (GameFlag_IsSet(0x835) == 0) {
        record = GameFlag_IsSet(0x831);
        if (record != 0) {
            goto L_020002b4;
        }
        FieldScene_RunScene372SequenceA();
        GameFlag_Set(0x206);
    } else {
        L_020002b4:;
        Actor_WalkTo(0, 0x106, 0x325);
        Event_Wait(3);
        SceneState_SetWork1c0AndRunObject(6);
    }
}

void FieldScene_SetupDescriptorD78aIfFlag205Clear(void)
{
    if (GameFlag_IsSet((s32)&Value_00000205) == 0) {
        Audio_PlayCue(0x9E);
        Map_AnimateCells(Data_0200d78a, 50, 44);
    }
    Actor_WalkTo(0, 0x154, 0x378);
    Event_Wait(3);
    SceneState_SetWork1c0AndRunObject(7);
}

void FieldScene_SetupWithDescriptorD7A0(void)
{
    Audio_PlayCue(158);
    Map_AnimateCells((void *)0x0200d7a0, 49, 69);
    Actor_WalkTo(0, 0x146, 0x466);
    Event_Wait(3);
    SceneState_SetWork1c0AndRunObject(8);
}

void FieldScene_SetupDescriptorD7b6(void)
{
    Audio_PlayCue(158);
    Map_AnimateCells((void *)0x0200d7b6, 52, 76);
    Actor_WalkTo(0, 0x176, 0x4d6);
    Event_Wait(3);
    SceneState_SetWork1c0AndRunObject(9);
}

void FieldScene_RunScene372_02000398(void)
{
    u32 i;
    s32 record;

    Audio_PlayCue(158);
    Map_AnimateCells(0x200d78a, 35, 74);
    Actor_WalkTo(0, 102, 0x4b6);
    Event_Wait(3);
    SceneState_SetWork1c0AndRunObject(10);
}

void FieldScene_RunScene372_020003cc(void)
{
    u32 i;
    s32 record;

    Audio_PlayCue(158);
    Map_AnimateCells(0x200d78a, 35, 73);
    Actor_WalkTo(0, 102, 0x4b6);
    Event_Wait(3);
    SceneState_SetWork1c0AndRunObject(12);
}

void FieldScene_RunScene372_02000400(void)
{
    u32 i;
    s32 record;

    Audio_PlayCue(158);
    Map_AnimateCells(0x200d7a0, 38, 72);
    Actor_WalkTo(0, 146, 0x49e);
    Event_Wait(3);
    SceneState_SetWork1c0AndRunObject(13);
}

s32 FieldScene_RunFlagGatedActorSetup(void)
{
    s32 m;
    s32 t;
    s32 m2;
    s32 h;
    s32 k;
    s32 w1 = 0x14E0000;
    s32 w2 = 0x3A40000;
    s32 w3 = 0xE00000;
    s32 w4 = 0x3680000;
    s32 w5 = 0x400000;
    s32 w6 = 0x1B00000;
    s32 w7 = 0x720000;
    s32 w8 = 0xC00000;
    s32 w9 = 0x2000;
    s32 w10 = 0xE30000;
    s32 w11 = 0x4000;
    s32 w12 = 0xF70000;
    s32 w13 = 0x4000;
    s32 w14 = 0xF30000;
    s32 w15 = 0x1900000;
    s32 w16 = 0x1A80000;
    s32 w17 = 0x190;
    s32 w18 = 0x1A8;
    s32 w19 = 0x1A80000;
    s32 w20 = 0x1A8;
    s32 b1 = 0x4BE0000;
    s32 b2 = 0x4BE0000;
    s32 b3 = 0x4BE0000;
    s32 w21 = 0xA50000;
    s32 w22 = 0xA50000;
    s32 p1 = 0x2BF0000;
    s32 p2 = 0x47B0000;
    s32 p3 = 0x14D0000;
    s32 p4 = 0x4FD0000;
    s32 p5 = 0x2630000;
    s32 p6 = 0x2730000;
    s32 p7 = 0x2730000;
    s32 c1 = 0x26B;
    s32 c2 = 0x101;
    s32 c3 = 0x26B;

    Func_02004ce4(0xAA);
    Actor_SetPosition(23, 0, 0);
    if (GameFlag_IsSet((s32)&Value_00000109) != 0) {
        GameFlag_Clear((s32)&Value_00000205);
        GameFlag_Clear((s32)&Value_00000206);
    }
    if (GameFlag_IsSet(0x830) != 0) {
        Actor_SetPosition(11, w1, w2);
        FieldScene_RunScene372_02000ec4();
    }
    if (GameFlag_IsSet((s32)&Value_00000831) != 0) {
        Actor_SetPosition(12, w3, w4);
        ActorPresentation_SetEightSceneCells();
    }
    if (GameFlag_IsSet((s32)&Value_00000832) != 0) {
        Actor_SetPosition(13, w5, p1);
        SceneState_ApplyFourRects();
    }
    if (GameFlag_IsSet((s32)&Value_00000833) != 0) {
        Actor_SetPosition(14, w6, p2);
        FieldScene_DrawFiveTileBlocks();
    }
    {
        u8 *q;
        q = Actor_Get(11);
        q += 0x59;
        m = 4;
        *q = *q | m;
        q = Actor_Get(12);
        q += 0x59;
        *q = *q | m;
        q = Actor_Get(13);
        q += 0x59;
        *q = *q | m;
        q = Actor_Get(14);
        q += 0x59;
        *q = *q | m;
        q = Actor_Get(15);
        q += 0x59;
        *q = *q | m;
        q = Actor_Get(16);
        q += 0x59;
        *q = *q | m;
        q = Actor_Get(17);
        q += 0x59;
        *q = *q | m;
        q = Actor_Get(18);
        q += 0x59;
        { u8 tmp = m | *q; *q = tmp; }
    }
    if (GameFlag_IsSet((s32)&Value_00000837) != 0) {
        Actor_SetPosition(22, 0, 0);
    }
    {
        u8 *q;
        q = Actor_Get(19);
        *(s32 *)(q + 0x18) = 0x20000;
        *(s32 *)(q + 0x1C) = 0x20000;
    }
    if (GameFlag_IsSet((s32)&Value_00000838) != 0) {
        Actor_SetPosition(19, w7, p3);
    } else {
        Actor_EnableActionCallback(19, Data_0200c9f4);
    }
    if (GameFlag_IsSet((s32)&Value_00000841) != 0) {
        s32 h2;
        u8 *tbl;
        FieldScene_BuildPlacementGrid();
        Actor_SetPosition(9, w21, 0x4CD0000);
        {
            u8 *o;
            s32 v;
            o = Actor_Get(9);
            h2 = 0xE000;
            *(u16 *)(o + 6) = h2;
            v = Func_02004bea(Random_Next(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
            tbl = Data_0200cec8;
            o += 0x66;
            t = 1;
            *(u16 *)o = t;
            Actor_EnableActionCallback(9, tbl);
        }
        Actor_SetPosition(26, w22, 0x4E60000);
        {
            u8 *o;
            s32 v;
            o = Actor_Get(26);
            *(u16 *)(o + 6) = h2;
            v = Func_02004c24(Random_Next(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
            o += 0x66;
            t = 2;
            *(u16 *)o = t;
            Actor_EnableActionCallback(26, tbl);
        }
        Actor_SetPosition(22, 0x980000, 0x5050000);
        {
            u8 *o;
            s32 v;
            o = Actor_Get(22);
            *(u16 *)(o + 6) = h2;
            v = Func_02004c5a(Random_Next(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
            o += 0x66;
            t = 3;
            *(u16 *)o = t;
            Actor_EnableActionCallback(22, tbl);
        }
        Actor_SetPosition(8, 0xB80000, 0x5180000);
        {
            u8 *o;
            s32 v;
            o = Actor_Get(8);
            *(u16 *)(o + 6) = h2;
            v = Func_02004c92(Random_Next(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
            o += 0x66;
            t = 4;
            *(u16 *)o = t;
            Actor_EnableActionCallback(8, tbl);
        }
        Actor_SetAnimation(8, 6);
        {
            u8 *r;
            r = Actor_Get(22);
            r += 0x23;
            m2 = 0xFE;
            *r = *r & m2;
            r = Actor_Get(8);
            r += 0x23;
            *r = m2 & *r;
        }
        Func_02004cea(Data_0200c5b9, 0xC80);
        Actor_SetPosition(24, 0, 0);
        Actor_SetPosition(25, 0, 0);
        Actor_SetPosition(23, 0, 0);
        Actor_SetPosition(19, 0, 0);
        if (GameFlag_IsSet((s32)&Value_00000842) != 0) {
            Actor_SetPosition(22, 0, 0);
        }
    } else if (GameFlag_IsSet((s32)&Value_0000083a) != 0) {
        u8 *tbl;
        Actor_SetPosition(10, w8, b1);
        Actor_FaceDirection(10, w9, 0);
        Actor_SetAnimation(10, 5);
        {
            u8 *o;
            s32 v;
            o = Actor_Get(10);
            v = Func_02004d50(Random_Next(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
        }
        tbl = Data_0200cec8;
        Actor_EnableActionCallback(10, tbl);
        Actor_SetPosition(24, w10, b2);
        Actor_FaceDirection(24, w11, 0);
        Actor_SetAnimation(24, 6);
        {
            u8 *o;
            s32 v;
            o = Actor_Get(24);
            v = Func_02004d92(Random_Next(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
        }
        Actor_EnableActionCallback(24, tbl);
        Actor_SetPosition(25, w12, b3);
        Actor_FaceDirection(25, w13, 0);
        Actor_SetAnimation(25, 6);
        {
            u8 *o;
            s32 v;
            o = Actor_Get(25);
            v = Func_02004dd2(Random_Next(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
        }
        Actor_EnableActionCallback(25, tbl);
        Actor_SetPosition(23, w14, p4);
        Actor_FaceDirection(23, 0xC000, 0);
        Actor_SetSpriteFlags(Actor_Get(23), 0);
        Actor_SetPosition(17, 0, 0);
        Actor_SetPosition(18, 0, 0);
    } else {
        Actor_SetPosition(17, 0, 0);
        Actor_SetPosition(18, 0, 0);
    }
    {
        s16 *table = (s16 *)Data_02000240;
        if (table[225] != 15 || GameFlag_IsSet((s32)&Value_0000087b) != 0) {
            Func_020050bc();
            Func_020050c8();
        }
    }
    if (GameFlag_IsSet(0x210) != 0) {
        SceneState_SetFlag210AndConfigureRegion40_84();
    }
    GameFlag_Set((s32)&Value_00000834);
    k = 46;
    Map_CopyCellAttributes(29, 24, 1, 2, 26, k);
    Map_CopyCellAttributes(29, 25, 1, 1, 27, k);
    Map_CopyCellAttributes(29, 25, 1, 1, 28, k);
    k = 20;
    Map_CopyCellAttributes(19, 0x5A, 1, 1, k, 0x58);
    Map_CopyCellAttributes(19, 0x5A, 1, 1, k, 0x59);
    {
        u8 *o;
        u8 *q;
        o = Actor_Get(21);
        q = o + 0x55;
        *q = 0;
        *(s32 *)(o + 0xC) = 0xC00000;
        q += 4;
        *q = 8;
        Actor_SetSpriteFlags(o, 0);
    }
    Task_Wait(1);
    if (GameFlag_IsSet((s32)&Value_0000087b) == 0) {
        s16 *table = (s16 *)Data_02000240;
        if (table[225] == 15) {
            FieldScene_RunScene372_02000a10();
            return 0;
        }
    }
    Actor_SetAnimation(23, 7);
    if (GameFlag_IsSet((s32)&Value_00000837) == 0) {
        Event_Begin();
        Actor_SetAttachedEffect(22, c2);
        Actor_SetPosition(22, w15, p5);
        Actor_SetPosition(21, w16, p6);
        Actor_WalkTo(22, w17, c1);
        Actor_WalkToAndWait(21, w18, 0x26B);
        Actor_SetAnimation(21, 2);
        Actor_SetAnimation(22, 5);
        Event_End();
    } else {
        Event_Begin();
        Actor_SetPosition(21, w19, p7);
        Actor_WalkToAndWait(21, w20, c3);
        Actor_SetAnimation(21, 3);
        Event_End();
    }
    {
        u8 *state = *(u8 **)0x03001EBC;
        *(s32 *)(state + 0x1C0) = 0x100;
        *(s32 *)(state + 0x1C8) = 24;
    }
    Event_OpenScreen();
    Event_WaitForScreen();
    Func_02005248();
    return 0;
}

void FieldScene_RunScene372_02000a10(void)
{
    u32 i;
    s32 record;
    u8 *work;
    s32 base5_e5c;

    Event_Begin();
    Func_02005264_a();
    Func_02005270();
    Func_02005284();
    Task_Wait(60);
    Camera_SetSpeed(0x4000, 0x800);
    Camera_MoveTo(0x13c0000, 0xa00000, 0x3700000, 1);
    Actor_SetPosition(10, 0x1260000, 0x3640000);
    Actor_SetPosition(0, 0, 0);
    work = *(u8 **)Data_03001ebc;
    *(s32 *)(work + 0x1c0) = 0x100;
    *(s32 *)(work + 0x1c8) = 16;
    Event_OpenScreen();
    Event_WaitForScreen();
    Func_020052ea();
    Audio_PlayCue(158);
    Map_AnimateCells(0x200d78a, 50, 44);
    Actor_SetAttachedEffect(22, 0x101);
    Actor_SetSpeed(9, 0xcccc, 0x6666);
    Actor_SetSpeed(0, 0xcccc, 0x6666);
    Actor_SetSpeed(10, 0xcccc, 0x6666);
    Actor_SetPosition(9, 0x1560000, 0x37a0000);
    Actor_WalkToAndWait(9, 0x156, 0x389);
    Func_0200537c();
    Actor_WalkTo(9, 0x128, 0x389);
    Actor_SetPosition(0, 0x1560000, 0x37a0000);
    Actor_WalkTo(0, 0x156, 0x37a);
    Actor_WalkToAndWait(0, 0x156, 0x389);
    Actor_WalkToAndWait(0, 0x13e, 0x389);
    Actor_SetAnimation(9, 1);
    Actor_RunRepeatedMotion(9, 1);
    Actor_FaceDirection(9, 0xc000, 60);
    base5_e5c = (s32)Data_00000e5c;
    Event_SetMessage(base5_e5c);
    Event_ShowMessage(9, 0);
    Actor_WalkToAndWait(10, 0x126, 0x346);
    Event_Wait(40);
    Actor_SetAnimationAndWait(10, 4);
    Event_ShowMessage(10, 0);
    Actor_FaceEachOther(0, 9, 0);
    Event_Wait(40);
    Actor_FaceDirection(10, 0x4000, 0);
    Event_ShowMessageAndWait(10, 0, 20);
    Actor_ShowEmote(9, 0x101, 20);
    Actor_FaceDirection(9, 0xc000, 10);
    Event_ShowMessageAndWait(9, 0, 10);
    Actor_SetAnimationAndWait(10, 4);
    Event_ShowMessage(10, 0);
    Actor_SetAttachedEffect(9, 0x102);
    Event_Wait(30);
    Actor_FaceDirection(9, 0, 50);
    Actor_FaceDirection(9, 0xc000, 10);
    Actor_SetSpeed(9, 0x18000, 0xc000);
    Actor_WalkToAndWait(9, 0x121, 0x373);
    Actor_FaceDirection(9, 0xe000, 0);
    Event_ShowMessage(9, 0);
    Actor_RunRepeatedMotion(10, 2);
    Event_ShowMessage(10, 0);
    Actor_SetAnimationAndWait(9, 4);
    Event_ShowMessageAndWait(9, 0, 10);
    Actor_FaceDirection(9, 0x2000, 10);
    Event_SetMessage((base5_e5c + 8));
    Event_OpenMessage(9, 0);
    Actor_WalkToAndWait(0, 0x12e, 0x389);
    Actor_FaceDirection(0, 0xc000, 0);
    while (Event_ChooseYesNo(0, 0) == 1) {
        Actor_RunRepeatedMotion(9, 1);
        Event_SetMessage(0xe65);
        Event_OpenMessage(9, 0);
    }
    Actor_SetAnimationAndWait(9, 3);
    Event_SetMessage(0xe66);
    Event_ShowMessageAndWait(9, 0, 10);
    Actor_SetAnimationAndWait(0, 3);
    Actor_SetSpeed(10, 0x18000, 0xc000);
    Actor_WalkTo(10, 0x129, 0x2ee);
    Event_Wait(10);
    Actor_WalkToAndWait(9, 0x129, 0x2ee);
    Actor_SetPosition(9, 0, 0);
    Actor_SetPosition(10, 0, 0);
    Actor_SetAnimation(10, 1);
    Actor_SetAnimation(21, 2);
    Actor_SetAnimation(22, 5);
    GameFlag_Clear(0x12f);
    GameFlag_Set(0x87b);
    GameFlag_Set(0x205);
    Event_End();
}

void FieldScene_RunScene372_02000ec4(void)
{
    u32 i;
    s32 record;

    Map_CopyCellAttributes(29, 64, 1, 1, 21, 57);
    Map_CopyCellAttributes(29, 64, 1, 1, 21, 58);
    Map_CopyCellAttributes(29, 64, 1, 1, 22, 58);
    Map_CopyCellAttributes(29, 64, 1, 1, 20, 58);
    Map_CopyCellAttributes(28, 20, 1, 1, 20, 57);
}

void ActorPresentation_SetEightSceneCells(void)
{
    s32 a = 15;
    s32 d = 0x35;
    s32 e;
    s32 b;
    s32 c;
    s32 f;

    Map_CopyCellAttributes(29, 23, 1, 1, a, d);
    b = 14;
    Map_CopyCellAttributes(29, 23, 1, 1, b, d);
    c = 13;
    Map_CopyCellAttributes(29, 23, 1, 1, c, d);
    Map_CopyCellAttributes(26, 20, 2, 1, b, 0x34);
    e = 0x36;
    Map_CopyCellAttributes(25, 21, 1, 1, c, e);
    Map_CopyCellAttributes(25, 21, 1, 1, a, e);
    Map_CopyCellAttributes(14, 0x35, 1, 1, b, e);
    f = 0x37;
    Map_CopyCellAttributes(13, 0x37, 1, 1, a, f);
}

void FieldScene_RunOpeningAuxiliarySequence(void)
{
    if (GameFlag_IsSet(0x312) == 0) {
        Event_Begin();
        if (GameFlag_IsSet(0x832) == 0) {
            struct FieldActor *actor = Actor_Get(13);
            struct FieldActor *leader = Actor_Get(0);
            u16 priority = leader->sprite->priority;
            u8 flags = leader->priority_flags;

            Work_SetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
            Audio_PlayCue(141);
            Task_Wait(40);
            Audio_PlayCue(145);
            Actor_SetSpritePriority(0, 3);
            Actor_Get(0)->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
            Actor_SetPosition(13, 0, 0x2bf0000);
            actor->speed = 0x18000;
            actor->acceleration = 0x18000;
            actor->y.fixed += 0x500000;
            *(s32 *)((u8 *)actor + 0x3c) = actor->y.fixed;
            *(s32 *)((u8 *)actor + 0x44) = 0x8000;
            Actor_WalkToAndWait(13, 64, 0x2bf);
            Event_Wait(40);
            Audio_PlayCue(0x121);
            Work_SetValuesIfNonNegative(-1, -1, 0xe666);
            MapRender_WaitForValues();
            Func_02005acc();
            GameFlag_Set(0x832);
            Actor_SetSpritePriority(0, priority);
            Actor_Get(0)->priority_flags |= ACTOR_PRIORITY_AUTOMATIC;
            leader->priority_flags = flags;
        }
        SceneState_ApplyFourRects();
        GameFlag_Set(0x312);
        if (GameFlag_IsSet(0x837) != 0) {
            if (GameFlag_IsSet(0x841) == 0) {
                if (GameFlag_IsSet(0x30c) == 0) {
                    if (Actor_Get(0)->z.fixed <= 0x2b4ffff) {
                        Value2(SceneActor_RunActor22PlacementSequence, 62, 0x29d);
                        Actor_WalkToAndWait(0, 27, 0x273);
                    } else {
                        Value2(SceneActor_RunActor22PlacementSequence, 75, 0x2cb);
                        Actor_WalkToAndWait(0, 67, 0x2f5);
                    }
                    GameFlag_Set(0x30c);
                }
            }
        }
        Event_End();
    }
}

void SceneState_ApplyFourRects(void)
{

    s32 a = 0x2a;
    s32 b;

    Map_CopyCellAttributes(29, 22, 1, 1, 3, a);
    b = 2;
    Map_CopyCellAttributes(29, 21, 1, 1, b, a);
    Map_CopyCellAttributes(29, 21, 1, 1, 4, a);
    Map_CopyCellAttributes(23, 20, 3, 1, b, 0x2b);
}

void FieldScene_DrawFiveTileBlocks(void)
{
    s32 a = 26;
    s32 h = 0x47;
    s32 b;
    s32 a2;

    Map_CopyCellAttributes(29, 20, 1, 1, a, h);
    b = 0x46;
    Map_CopyCellAttributes(29, 20, 1, 1, a, b);
    a2 = 27;
    Map_CopyCellAttributes(29, 20, 1, 1, a2, b);
    Map_CopyCellAttributes(28, 21, 1, 1, 28, h);
    Map_CopyCellAttributes(28, 22, 1, 1, a2, 0x48);
}

void FieldScene_RunScene372SequenceA(void)
{
    u32 i;
    s32 record;
    s32 base5_e67;

    Event_Begin();
    Actor_WalkToAndWait(0, 0x106, 0x32a);
    Actor_SetPosition(20, 0x1060000, 0x3250000);
    Actor_WalkTo(20, 0x106, 0x339);
    Actor_SetSpeed(0, 0x20000, 0x10000);
    Actor_Jump(0, 2, 0);
    Actor_WalkToAndWait(0, 0x11a, 0x357);
    Actor_SetAnimation(20, 1);
    Actor_Jump(0, 4, 0);
    Actor_FaceEachOther(0, 20, 0);
    Func_02005e24();
    Event_Wait(30);
    Actor_RunRepeatedMotion(0, 2);
    Actor_ShowEmote(20, 0x100, 20);
    base5_e67 = (s32)Data_00000e67;
    Event_SetMessage(base5_e67);
    Event_ShowMessage(20, 0);
    Event_Wait(20);
    Event_AskYesNo(20, 0);
    Actor_RunRepeatedMotion(20, 2);
    Event_SetMessage((base5_e67 + 4));
    Event_ShowMessageAndWait(20, 0, 20);
    Call2(Func_02005d2e, 20, 0x200c8c0);
    GameFlag_Set(0x835);
    Event_End();
}

void FieldScene_RunScene372SequenceB(void)
{
    u32 i;
    s32 record;

    if (GameFlag_IsSet(0x836) == 0) {
        if (GameFlag_IsSet(0x837) == 0) {
            Event_Begin();
            Event_SetMessage(0xe6c);
            Event_ShowMessageAndWait(22, 0, 20);
            Actor_ShowEmote(0, 0x101, 40);
            Actor_WalkToAndWait(0, 0x17e, 0x26b);
            Actor_FaceActor(0, 22, 0);
            Actor_RunRepeatedMotion(0, 2);
            Event_Wait(30);
            Event_ShowMessage(22, 0);
            GameFlag_Set(0x836);
            Event_End();
        }
    }
}

void FieldScene_RunActor22SceneWhenFlag836Only(void)
{
    if (GameFlag_IsSet(0x837) == 0 && GameFlag_IsSet(0x836) != 0) {
        Event_Begin();
        Actor_RunRepeatedMotion(22, 2);
        Event_Wait(20);
        Event_SetMessage(0xe71);
        Func_02002df0();
        Event_End();
    }
}

void FieldScene_RunScene372SequenceC(void)
{
    u32 i;
    s32 record;

    if (GameFlag_IsSet(0x841) != 0) {
        Event_Begin();
        Actor_FaceActor(22, 0, 0);
        Event_Wait(20);
        Event_SetMessage((s32)Data_00000ed0);
        Event_ShowMessage(22, 0);
        Actor_FaceDirection(22, 0xe000, 10);
        Event_End();
    } else {
        if (GameFlag_IsSet(0x837) == 0) {
            Event_Begin();
            Event_SetMessage(0xe6e);
            Func_02002e5e();
            Event_End();
        }
    }
}

void FieldScene_RunScene372SequenceE(void)
{
    u32 i;
    s32 record;
    s32 base5_e74;
    s32 v6;

    if (GameFlag_IsSet(0x837) != 0) {
    } else {
        Event_Begin();
        Actor_SetAttachedEffect(22, 0x100);
        base5_e74 = (s32)Data_00000e74;
        Event_SetMessage(base5_e74);
        Event_ShowMessage(22, 0);
        Actor_ShowEmote(0, 0x100, 20);
        Actor_FaceDirection(0, 0x4000, 0);
        Camera_SetSpeed(0x6666, 0xccc);
        Camera_MoveTo(0x1000000, -1, 0x24c0000, 1);
        Actor_SetSpeed(22, 0x20000, 0x10000);
        Value2(Func_02005ff6, 22, 0x200c934);
        Actor_FaceEachOther(0, 22, 0);
        Event_Wait(30);
        Value2(Engine_ActorEnableActionCallback, 22, 0x200c984);
        Event_ShowMessage(22, 0);
        v6 = 128;
        record = Actor_Get(22);
        *(volatile s32 *)(record + 28) = (v6 << 9);
        Actor_RunRepeatedMotion(22, 1);
        Event_Wait(20);
        Event_AskYesNo(22, 0);
        Event_Wait(40);
        Actor_RunRepeatedMotion(22, 1);
        Event_SetMessage((base5_e74 + 5));
        Event_ShowMessageAndWait(22, 0, 20);
        Actor_SetAnimationAndWait(0, 3);
        Actor_SetAnimationAndWait(22, 3);
        Event_ShowMessage(22, 0);
        Actor_SetSpeed(22, (v6 << 9), 0x8000);
        Actor_SetAnimation(22, 2);
        record = Value1(Engine_ActorGet, 0);
        if (record != 0) {
            Actor_SetDestination(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Actor_WaitForMove(22);
        Actor_SetPosition(22, 0, 0);
        Func_0200606c(1, 1);
        Actor_SetAnimation(21, 3);
        GameFlag_Set(0x837);
        Event_End();
    }
}

void FieldScene_RunScene372SequenceD(void)
{
    u32 i;
    s32 record;
    struct FieldActor *actor;

    Event_Begin();
    actor = Actor_Get(0);
    if (actor != NULL) {
        Actor_SetPosition(22, actor->x.fixed, actor->z.fixed);
    }
    Actor_SetSpeed(22, 0x10000, 0x8000);
    Actor_WalkToAndWait(22, 0x119, 0x1fb);
    Actor_FaceEachOther(22, 0, 0);
    Event_Wait(30);
    Event_SetMessage(0xe7b);
    Event_ShowMessage(22, 0);
    Actor_FaceActor(0, 22, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(0, 1);
    Event_Wait(20);
    Actor_FaceDirection(22, 0x4000, 0);
    Event_ShowMessage(22, 0);
    Actor_SetAnimation(22, 2);
    record = Value1(Engine_ActorGet, 0);
    if (record != 0) {
        Actor_SetDestination(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(22);
    Actor_SetPosition(22, 0, 0);
    Actor_WalkToAndWait(0, 0x100, 0x205);
    Event_End();
}

void SceneActor_RunActor22PlacementSequence(s32 x, s32 y)
{
    Thing1 *a;
    s32 w = 0x10000;
    s32 h = 0x8000;
    Thing2 *b;

    a = Actor_Get(0);
    if (a != 0) {
        Actor_SetPosition(22, a->unk8, a->unk10);
    }
    Actor_SetSpeed(22, w, h);
    Actor_WalkToAndWait(22, x, y);
    Actor_FaceEachOther(0, 22, 0);
    Event_Wait(20);
    Actor_SetAttachedEffect(0, 0x102);
    Event_Wait(40);
    Event_SetMessage(0xe7d);
    Event_ShowMessage(22, 0);
    Actor_RunRepeatedMotion(22, 2);
    Event_ShowMessage(22, 0);
    Actor_SetAnimationAndWait(0, 3);
    Actor_SetAnimation(22, 2);
    b = Actor_Get(0);
    if (b != 0) {
        Actor_SetDestination(22, b->unkA, b->unk12);
    }
    Actor_WaitForMove(22);
    Actor_SetPosition(22, 0, 0);
}

void FieldScene_RunReturnScript(void)
{
    extern s32 Data_0200d7fc;

    u32 i;
    s32 record;
    s32 *phase;
    struct FieldActor *actor;
    s32 base7_200c56d;
    s32 base_200c5a9;
    s32 shifted;

    if (GameFlag_IsSet(0x838) == 0) {
        Event_Begin();
        Call1_02001828(Func_02006246, 0x200d4b0);
        SceneState_SetValue140Mode0();
        Task_Wait(1);
        Audio_PlayCue(141);
        Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
        Event_Wait(30);
        Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
        Audio_PlayCue(145);
        Event_Wait(30);
        actor = Actor_Get(0);
        if (actor != NULL) {
            Actor_SetPosition(22, actor->x.fixed, actor->z.fixed);
        }
        Actor_SetSpeed(0, 0x20000, 0x10000);
        Actor_SetSpeed(22, 0x20000, 0x10000);
        Value2(Engine_ActorEnableActionCallback, 0, 0x200ca00);
        Call2(Func_02006318, 22, 0x200ca3c);
        Func_0200630e(0);
        Actor_ShowEmote(0, 0x100, 0);
        Actor_ShowEmote(22, 0x100, 30);
        Work_SetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
        Audio_PlayCue(145);
        Event_Wait(40);
        Work_SetValuesIfNonNegative(0x50000, 0x50000, 0x10000);
        Audio_PlayCue(145);
        Event_Wait(20);
        Actor_SetAttachedEffect(0, 0x102);
        Actor_SetAttachedEffect(22, 0x102);
        Event_Wait(40);
        Actor_SetAnimation(32, 5);
        Actor_SetAnimation(33, 5);
        Actor_SetAnimation(30, 8);
        Actor_SetAnimation(29, 8);
        Actor_Get(30)->scale_x = -0x10000;
        Actor_SetSpritePriority(32, 2);
        Actor_SetSpritePriority(33, 2);
        Actor_SetSpritePriority(30, 3);
        Actor_SetSpritePriority(29, 3);
        Event_SetMessage(0xe7f);
        Event_ShowMessageAndWait(28, 0, 20);
        Actor_FaceDirection(0, 0xc000, 0);
        Actor_FaceDirection(22, 0xc000, 20);
        Camera_SetSpeed(0x40000, 0x8000);
        Camera_MoveTo(0x700000, -1, 0x14b0000, 1);
        Camera_WaitForMove();
        for (i = 0; i < 40; i++) {
            SceneActor_SetModeByFrameBit1(Actor_Get(32));
            SceneActor_SetModeByFrameBit1(Actor_Get(33));
            SceneActor_SetModeByFrameBit1(Actor_Get(30));
            SceneActor_SetModeByFrameBit1(Actor_Get(29));
            Task_Wait(1);
        }
        phase = &Data_0200d7fc;
        base7_200c56d = (s32)Func_0200c56d;
        Data_0200d7f8 = 0;
        *phase = 0;
        Value2(Func_02006350, base7_200c56d, 0xc80);
        base_200c5a9 = 0x200c5a9;
        Call2(Func_0200635e, base_200c5a9, 0xc80);
        Event_Wait(40);
        *phase = 1;
        Event_Wait(30);
        Actor_SetPosition(19, 0x720000, 0x1220000);
        record = Actor_Get(19);
        shifted = *(s32 *)(record + 12) + 0x400000;
        *(s32 *)(record + 12) = shifted;
        *(s32 *)(record + 60) = shifted;
        Actor_SetSpeed(19, 0xcccc, 0x6666);
        Audio_PlayCue(145);
        Actor_MoveToAndWait(19, 114, 0x14d);
        Actor_SetAnimation(19, 2);
        Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
        Audio_PlayCue(145);
        *phase = 0;
        Actor_SetSpeed(19, 0x6666, 0x3333);
        Actor_MoveToAndWait(19, 114, 0x12c);
        Actor_SetAnimation(19, 2);
        Work_SetValuesIfNonNegative(0x50000, 0x50000, 0x10000);
        Audio_PlayCue(145);
        *phase = 2;
        Actor_SetSpeed(19, 0xcccc, 0x6666);
        Actor_MoveToAndWait(19, 114, 0x14d);
        Actor_SetAnimation(19, 2);
        Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
        Audio_PlayCue(145);
        *phase = 0;
        Actor_SetSpeed(19, 0x6666, 0x3333);
        Actor_MoveToAndWait(19, 114, 0x12c);
        Actor_SetAnimation(19, 2);
        Work_SetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
        Audio_PlayCue(145);
        *phase = 2;
        Actor_SetSpeed(19, 0xcccc, 0x6666);
        Actor_MoveToAndWait(19, 114, 0x14d);
        Actor_SetAnimation(19, 2);
        Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
        Audio_PlayCue(145);
        *phase = 1;
        Event_Wait(20);
        Actor_SetAttachedEffect(32, 0x102);
        Actor_RunRepeatedMotion(32, 2);
        Event_ShowMessage(31, 0);
        Actor_ShowEmote(33, 0x100, 0);
        Actor_RunRepeatedMotion(33, 2);
        Event_ShowMessageAndWait(28, 0, 40);
        Actor_SetAttachedEffect(30, 0x102);
        Actor_RunRepeatedMotion(30, 2);
        Event_ShowMessage(30, 0);
        Data_0200d7f8 = 1;
        Actor_SetAnimation(29, 1);
        Task_Wait(1);
        Actor_SetChildValue(29, 0);
        Actor_ShowEmote(29, 0x105, 20);
        Actor_FaceDirection(29, 0x8000, 40);
        Actor_FaceDirection(29, 0, 20);
        Actor_FaceDirection(29, 0x8000, 20);
        Actor_FaceDirection(29, 0x4000, 40);
        Actor_ShowEmote(29, 0x100, 0);
        Actor_RunRepeatedMotion(29, 2);
        Actor_Jump(29, 4, 40);
        Actor_SetAnimation(29, 9);
        Event_Wait(10);
        Event_ShowMessageAndWait(29, 0, 20);
        Audio_PlayCue(0x121);
        Work_SetValuesIfNonNegative(-1, -1, 0xe666);
        Camera_SetSpeed(0x60000, 0xc000);
        Camera_MoveTo(0x540000, -1, 0x2340000, 1);
        Camera_WaitForMove();
        Func_0200686c();
        Actor_FaceActor(22, 0, 0);
        Event_Wait(20);
        Actor_SetAttachedEffect(22, 0x102);
        Event_Wait(30);
        Func_02006632(base7_200c56d);
        Func_02006638_a(base_200c5a9);
        Event_ShowMessage(22, 0);
        Actor_FaceActor(0, 22, 0);
        Event_Wait(20);
        FieldScene_RunSingleStep();
        Actor_SetAnimation(0, 3);
        Actor_SetAnimationAndWait(22, 3);
        Event_Wait(20);
        Actor_SetAnimation(22, 2);
        record = Value1(Engine_ActorGet, 0);
        if (record != 0) {
            Actor_SetDestination(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Actor_WaitForMove(22);
        Actor_SetPosition(22, 0, 0);
        Actor_Destroy(31);
        Actor_Destroy(28);
        Actor_Destroy(30);
        Actor_Destroy(29);
        Actor_Destroy(32);
        Actor_Destroy(33);
        GameFlag_Set(0x838);
        Event_End();
    }
}

void OverlayObject_SetChildByte5AndMark(u8 *o, s32 n)
{
    if ((*(u8 *)(o + 0x54) & 15) == 1) {
        u8 *c = *(u8 **)(o + 0x50);
        s32 idx = n - 1;
        u8 cnt;
        if (n == 0) {
            idx = Data_0200c8bc[(*(u32 *)0x03001E40 >> 1) & (*(u8 *)(o + 0x54) & 15)];
        }
        cnt = *(u8 *)(c + 0x27);
        if (cnt != 0) {
            u8 **p = (u8 **)(c + 0x28);
            s32 k = cnt;
            do {
                u8 *e = *p++;
                if (e != 0 && *(s32 *)(e + 16) != 0) {
                    *(u8 *)(e + 5) = idx;
                }
                k--;
            } while (k != 0);
        }
        *(u8 *)(c + 0x25) = 1;
    }
}

void ActorPresentation_SetFourActorsModeByBit(void)
{
    extern s32 Data_0200d7fc;

    if (((*(u32 *)0x03001E40 >> Data_0200d7fc) & 3) != 0) {
        OverlayObject_SetChildByte5AndMark(Actor_Get(32), 1);
        OverlayObject_SetChildByte5AndMark(Actor_Get(33), 1);
        OverlayObject_SetChildByte5AndMark(Actor_Get(30), 1);
        OverlayObject_SetChildByte5AndMark(Actor_Get(29), 1);
    } else {
        OverlayObject_SetChildByte5AndMark(Actor_Get(32), 8);
        OverlayObject_SetChildByte5AndMark(Actor_Get(33), 8);
        OverlayObject_SetChildByte5AndMark(Actor_Get(30), 8);
        OverlayObject_SetChildByte5AndMark(Actor_Get(29), 8);
    }
}

void FieldScene_RunFlagGatedActorSequence(void)
{
    s32 kc0_1 = 0xC00000;
    s32 k4be_1 = 0x4BE0000;
    s32 k2000_1 = 0x2000;
    s32 kc0_2 = 0xC00000;
    s32 k4a5_1 = 0x4A50000;
    s32 k2000_2 = 0x2000;
    s32 ke3_1 = 0xE30000;
    s32 k4be_2 = 0x4BE0000;
    s32 k4000_1 = 0x4000;
    s32 kfa_1 = 0xFA0000;
    s32 k4be_3 = 0x4BE0000;
    s32 k4000_2 = 0x4000;
    s32 ke3_2 = 0xE30000;
    s32 k4a5_2 = 0x4A50000;
    s32 k2000_3 = 0x3000;
    s32 kf3_1 = 0xF30000;
    s32 k4fd_1 = 0x4FD0000;
    s32 kc000_1 = 0xC000;
    s32 k100_1 = 0x100;
    s32 k446_1 = 0x446;
    s32 k446_2 = 0x446;
    s32 k4000_3 = 0x4000;
    s32 k4000_4 = 0x4000;
    s32 k40000_1 = 0x40000;
    s32 k8000_1 = 0x8000;
    s32 kd8_1 = 0xD80000;
    s32 ng1 = -1;
    s32 k4d0_1 = 0x4D00000;
    s32 k3000_1 = 0x3000;
    s32 k30000_1 = 0x30000;
    s32 k6000_1 = 0x6000;
    s32 ke8_1 = 0xE80000;
    s32 ng2 = -1;
    s32 k4e5_1 = 0x4E50000;
    s32 k9999_1 = 0x9999;
    s32 k1333_1 = 0x1333;
    s32 kd8_2 = 0xD80000;
    s32 ng3 = -1;
    s32 k4d0_2 = 0x4D00000;
    s32 k102_1 = 0x102;
    s32 k102_2 = 0x102;
    s32 k4b5_1 = 0x4B5;
    s32 k4b1_1 = 0x4B1;
    s32 ke8_2 = 0xE80000;
    s32 ng4 = -1;
    s32 k4e5_2 = 0x4E50000;
    s32 kf3_2 = 0xF30000;
    s32 k4fd_2 = 0x4FD0000;
    s32 k20000_1 = 0x20000;
    s32 k19999_1 = 0x19999;
    s32 k3333_1 = 0x3333;
    s32 kd8_3 = 0xD80000;
    s32 ng5 = -1;
    s32 k4d0_3 = 0x4D00000;
    s32 k105_1 = 0x105;
    s32 k800a_1 = 0x800A;
    s32 k9999_2 = 0x9999;
    s32 k4ccc_1 = 0x4CCC;
    s32 k9999_3 = 0x9999;
    s32 k4ccc_2 = 0x4CCC;
    s32 k4ba_1 = 0x4BA;
    s32 k4a5_3 = 0x4A5;
    s32 k6000_2 = 0x6000;
    s32 k8000_2 = 0x8000;
    s32 k8018_1 = 0x8018;
    s32 kc000_2 = 0xC000;
    s32 k800a_2 = 0x800A;
    s32 k105_2 = 0x105;
    s32 k105_3 = 0x105;
    s32 k106_1 = 0x106;
    s32 k8000_3 = 0x8000;
    s32 kc000_3 = 0xC000;
    s32 k4000_5 = 0x4000;
    s32 kc000_4 = 0xC000;
    s32 k9000_1 = 0x9000;
    s32 ka000_1 = 0xA000;
    s32 k8000_4 = 0x8000;
    s32 k800a_3 = 0x800A;
    s32 k105_4 = 0x105;
    s32 k105_5 = 0x105;
    s32 k105_6 = 0x105;
    s32 k105_7 = 0x105;
    s32 k8000_5 = 0x8000;
    s32 k8000_6 = 0x8000;
    s32 k800a_4 = 0x800A;
    s32 kd000_1 = 0xD000;
    s32 k2000_4 = 0x2000;
    s32 ka000_2 = 0xA000;
    s32 k8000_7 = 0x8000;
    s32 v83a_2 = 0x83A;
    u8 *tbl;
    s32 w16;
    s32 m;
    s32 one;

    if (GameFlag_IsSet((s32)&Value_0000083a) != 0) {
        return;
    }
    Event_Begin();
    Actor_SetPosition(10, kc0_1, k4be_1);
    Actor_FaceDirection(10, k2000_1, 0);
    Actor_SetAnimation(10, 5);
    {
        u8 *o;
        s32 v;
        o = Actor_Get(10);
        v = Func_020067f6(Random_Next(), 0x5A) + 60;
        tbl = Data_0200cec8;
        *(u16 *)(o + 0x64) = v;
        Actor_EnableActionCallback(10, tbl);
    }
    Actor_SetPosition(9, kc0_2, k4a5_1);
    Actor_FaceDirection(9, k2000_2, 0);
    Actor_SetPosition(24, ke3_1, k4be_2);
    Actor_FaceDirection(24, k4000_1, 0);
    Actor_SetAnimation(24, 6);
    {
        u8 *o;
        s32 v;
        o = Actor_Get(24);
        v = Func_02006850(Random_Next(), 0x5A) + 60;
        *(u16 *)(o + 0x64) = v;
        Actor_EnableActionCallback(24, tbl);
    }
    Actor_SetPosition(25, kfa_1, k4be_3);
    Actor_FaceDirection(25, k4000_2, 0);
    Actor_SetAnimation(25, 6);
    {
        u8 *o;
        s32 v;
        o = Actor_Get(25);
        v = Func_02006890(Random_Next(), 0x5A) + 60;
        *(u16 *)(o + 0x64) = v;
        Actor_EnableActionCallback(25, tbl);
    }
    Actor_SetPosition(26, ke3_2, k4a5_2);
    Actor_FaceDirection(26, k2000_3, 0);
    Actor_SetPosition(23, kf3_1, k4fd_1);
    Actor_FaceDirection(23, kc000_1, 0);
    Actor_SetSpriteFlags(Actor_Get(23), 0);
    Task_Wait(3);
    Event_SetMessage((s32)&Value_00000e8c);
    Event_ShowMessage((s32)&Value_0000201a, 0);
    Actor_ShowEmote(0, k100_1, 20);
    Actor_WalkToAndWait(0, 150, k446_1);
    {
        u8 *p;
        p = Actor_Get(0);
        if (p != 0) {
            Actor_SetPosition(22, *(s32 *)(p + 8), *(s32 *)(p + 16));
        }
    }
    Actor_WalkToAndWait(22, 132, k446_2);
    Actor_FaceEachOther(0, 22, 0);
    Event_Wait(40);
    Actor_FaceDirection(0, k4000_3, 0);
    Actor_FaceDirection(22, k4000_4, 20);
    Camera_SetSpeed(k40000_1, k8000_1);
    Camera_MoveTo(kd8_1, ng1, k4d0_1, 1);
    Camera_WaitForMove();
    Event_Wait(40);
    Actor_RunRepeatedMotion(10, 2);
    Event_ShowMessageAndWait(10, 0, 10);
    Actor_RunRepeatedMotion(23, 3);
    Actor_FaceDirection(9, 0, 10);
    Actor_SetAnimationAndWait(9, 3);
    Event_ShowMessageAndWait(9, 0, 10);
    Actor_FaceDirection(9, k3000_1, 10);
    Camera_SetSpeed(k30000_1, k6000_1);
    Camera_MoveTo(ke8_1, ng2, k4e5_1, 1);
    Camera_WaitForMove();
    Event_Wait(20);
    Audio_PlayCue(134);
    Actor_Jump(23, 4, 0);
    Actor_SetAnimation(23, 6);
    Event_Wait(10);
    Actor_SetPosition(23, 0, 0);
    Event_Wait(60);
    Func_02006c84();
    Actor_SetAnimation(10, 1);
    {
        u8 *o;
        o = Actor_Get(10);
        w16 = 0x10000;
        *(s32 *)(o + 0x18) = w16;
        *(s32 *)(o + 0x1C) = w16;
    }
    Actor_SetAnimation(24, 1);
    {
        u8 *o;
        o = Actor_Get(24);
        *(s32 *)(o + 0x18) = w16;
        *(s32 *)(o + 0x1C) = w16;
    }
    Actor_SetAnimation(25, 1);
    {
        u8 *o;
        o = Actor_Get(25);
        *(s32 *)(o + 0x18) = w16;
        *(s32 *)(o + 0x1C) = w16;
    }
    Actor_StartRepeatedMotion(10, 2);
    Actor_StartRepeatedMotion(9, 2);
    Actor_StartRepeatedMotion(24, 2);
    Actor_StartRepeatedMotion(25, 2);
    Actor_RunRepeatedMotion(26, 2);
    Camera_SetSpeed(k9999_1, k1333_1);
    Camera_MoveTo(kd8_2, ng3, k4d0_2, 1);
    Camera_WaitForMove();
    Actor_SetAttachedEffect(26, k102_1);
    Actor_SetAttachedEffect(9, k102_2);
    Event_Wait(60);
    Actor_RunRepeatedMotion(26, 2);
    Actor_StartRepeatedMotion(26, 3);
    Event_ShowMessage(26, 0);
    Actor_Jump(25, 2, 0);
    Actor_SetDestination(25, 234, k4b5_1);
    Actor_Jump(26, 2, 0);
    Actor_SetDestination(26, 227, k4b1_1);
    Event_Wait(90);
    Camera_MoveTo(ke8_2, ng4, k4e5_2, 1);
    Camera_WaitForMove();
    Actor_SetPosition(23, kf3_2, k4fd_2);
    Task_Wait(1);
    Audio_PlayCue(106);
    {
        u8 *o;
        o = Actor_Get(23);
        *(s32 *)(o + 0x28) = k20000_1;
    }
    Event_Wait(6);
    Actor_SetAnimation(23, 7);
    Event_Wait(20);
    Func_02006db4();
    Event_Wait(20);
    Camera_SetSpeed(k19999_1, k3333_1);
    Camera_MoveTo(kd8_3, ng5, k4d0_3, 1);
    Camera_WaitForMove();
    Actor_RunRepeatedMotion(24, 2);
    Event_Wait(20);
    Actor_ShowEmote(24, k105_1, 40);
    Actor_FaceEachOther(24, 10, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(10, 2);
    Event_ShowMessage(k800a_1, 0);
    {
        u8 *o;
        o = Actor_Get(25);
        o += 0x5A;
        m = 0xFE;
        {
            u8 lv = m & *o;
            *o = lv;
        }
    }
    {
        u8 *o;
        o = Actor_Get(26);
        o += 0x5A;
        *o = *o & m;
    }
    Actor_SetSpeed(25, k9999_2, k4ccc_1);
    Actor_SetSpeed(26, k9999_3, k4ccc_2);
    Actor_SetDestination(25, 247, k4ba_1);
    Actor_MoveToAndWait(26, 227, k4a5_3);
    {
        u8 *o;
        o = Actor_Get(25);
        o += 0x5A;
        one = 1;
        {
            u8 lv = *o | one;
            *o = lv;
        }
    }
    {
        u8 *o;
        o = Actor_Get(26);
        o += 0x5A;
        {
            u8 lv = one | *o;
            *o = lv;
        }
    }
    Actor_FaceDirection(26, k6000_2, 0);
    Actor_FaceDirection(25, k8000_2, 10);
    Actor_SetAnimation(24, 4);
    Event_ShowMessageAndWait(k8018_1, 0, 10);
    Actor_FaceDirection(10, kc000_2, 20);
    Actor_FaceDirection(10, 0, 10);
    Actor_SetAnimation(10, 4);
    Event_ShowMessageAndWait(k800a_2, 0, 10);
    Actor_ShowEmote(24, k105_2, 0);
    Actor_ShowEmote(10, k105_3, 60);
    Actor_ShowEmote(9, k106_1, 20);
    Actor_FaceDirection(9, k8000_3, 40);
    Actor_FaceDirection(9, kc000_3, 20);
    Actor_FaceDirection(9, 0, 30);
    Actor_FaceDirection(9, k4000_5, 10);
    Event_ShowMessageAndWait(9, 0, 10);
    Actor_FaceDirection(10, kc000_4, 0);
    Actor_FaceDirection(25, k9000_1, 0);
    Actor_FaceDirection(24, ka000_1, 0);
    Actor_FaceDirection(26, k8000_4, 10);
    Actor_RunRepeatedMotion(10, 1);
    Event_ShowMessageAndWait(k800a_3, 0, 10);
    Actor_SetAnimation(9, 4);
    Event_ShowMessageAndWait(9, 0, 10);
    Actor_ShowEmote(10, k105_4, 0);
    Actor_ShowEmote(24, k105_5, 0);
    Actor_ShowEmote(25, k105_6, 0);
    Actor_ShowEmote(26, k105_7, 40);
    Actor_FaceDirection(9, 0, 10);
    Actor_FaceEachOther(24, 25, 0);
    Event_Wait(20);
    Actor_FaceDirection(9, 0, 0);
    Actor_FaceDirection(10, 0, 10);
    Actor_FaceDirection(24, k8000_5, 0);
    Actor_FaceDirection(25, k8000_6, 10);
    Actor_SetAnimation(24, 3);
    Actor_SetAnimationAndWait(25, 3);
    Actor_FaceEachOther(10, 9, 0);
    Event_Wait(20);
    Actor_RunRepeatedMotion(10, 1);
    Event_ShowMessageAndWait(k800a_4, 0, 10);
    Actor_SetAnimationAndWait(9, 3);
    Actor_FaceDirection(24, kd000_1, 10);
    Actor_StartRepeatedMotion(24, 1);
    Event_ShowMessageAndWait(24, 0, 10);
    Actor_FaceDirection(10, 0, 0);
    Actor_FaceDirection(9, 0, 0);
    Actor_RunRepeatedMotion(26, 1);
    Actor_FaceDirection(26, k2000_4, 20);
    Actor_FaceDirection(25, ka000_2, 20);
    Actor_SetAnimationAndWait(25, 3);
    Event_ShowMessageAndWait(25, 0, 10);
    Actor_SetAnimationAndWait(26, 3);
    Event_Wait(20);
    Actor_RunRepeatedMotion(9, 2);
    Actor_SetAnimationAndWait(9, 3);
    Event_ShowMessageAndWait(9, 0, 10);
    Actor_FaceDirection(26, k8000_7, 10);
    Actor_SetAnimationAndWait(26, 3);
    Event_Wait(20);
    Actor_RunRepeatedMotion(9, 1);
    Event_ShowMessageAndWait(9, 0, 10);
    Func_02005116();
    GameFlag_Set(v83a_2);
    Event_End();
}

/* Sets up records 26, 24, 25, 9 and 10 (position/speed, facing, movement),
 * runs several timed particle/object sequences against constant tables, then
 * takes two branches whose outcome picks entries out of the Data_00000e9b
 * and Data_00000ea1 byte tables to drive further record 9/22 setup calls. */
void RunEventScript01(void)
{
    u32 i;
    s32 entry;
    s32 record;
    s32 base5_200cec8;
    s32 base5_e9b;
    s32 base5_ea1;

    Actor_FaceDirection(26, 0x3000, 0);
    Actor_FaceDirection(24, 0xd000, 0);
    Actor_FaceDirection(25, 0xb000, 0);
    Actor_FaceDirection(9, 0x3000, 0);
    Actor_FaceDirection(10, 0xd000, 20);
    Actor_SetAnimation(26, 3);
    Actor_SetAnimation(24, 3);
    Actor_SetAnimation(25, 3);
    Actor_SetAnimation(9, 3);
    Actor_SetAnimationAndWait(25, 3);
    Event_Wait(20);
    Camera_SetSpeed(0x10000, 0x2000);
    Camera_MoveTo(0x860000, -1, 0x4ab0000, 1);
    Actor_SetSpeed(26, 0x19999, 0xcccc);
    Actor_SetSpeed(9, 0x19999, 0xcccc);
    ObjectMotion_EnableActionAndSetCallback_1(26, 0x200cab4);
    ObjectMotion_MarkActiveAndSetActionCallback_1(9, 0x200ca78);
    Audio_PlayCue(158);
    Map_AnimateCells(0x200d7a0, 38, 72);
    Event_Wait(10);
    Actor_WalkToAndWait(9, 149, 0x497);
    Actor_SetPosition(9, 0, 0);
    Actor_WalkToAndWait(25, 250, 0x4be);
    BattleEffect_PlayQueuedSound_1();
    Actor_FaceDirection(10, 0x3000, 0);
    Actor_FaceDirection(24, 0x3000, 0);
    Actor_FaceDirection(25, 0x3000, 0);
    Actor_SetAnimation(10, 5);
    Actor_SetAnimation(24, 6);
    Actor_SetAnimation(25, 6);
    /* For records 10, 24 and 25: fetch the record's entry pointer, fetch a
     * value from that record's own state, and store a derived value into
     * the entry's field at offset 100. */
    entry = Scene_GetRecord_1_020028a4(10);
    record = Random_Next();
    *(u16 *)(entry + 100) = (Func_02007008(record, 90) + 60);
    entry = Scene_GetRecord_2(24);
    record = Random_Next();
    *(u16 *)(entry + 100) = (Func_02007020(record, 90) + 60);
    entry = Scene_GetRecord_3(25);
    record = Random_Next();
    *(u16 *)(entry + 100) = (Func_02007038(record, 90) + 60);
    base5_200cec8 = (s32)Data_0200cec8;
    Actor_EnableActionCallback(10, base5_200cec8);
    ObjectMotion_EnableActionAndSetCallback_3(24, base5_200cec8);
    Actor_EnableActionCallback(25, base5_200cec8);
    Object_LookupAndStep_1(26);
    BattleRuntime_WaitIfModeZero_3(10);
    Audio_PlayCue(159);
    Map_AnimateCells(0x200d7e2, 38, 72);
    Event_Wait(30);
    BattleEffect_PlayQueuedSound_2();
    Camera_MoveTo(0x700000, -1, 0x4c90000, 1);
    Audio_PlayCue(158);
    Map_AnimateCells(0x200d78a, 35, 73);
    Event_Wait(20);
    BattleEffect_PlayQueuedSound_3();
    Actor_EnableActionCallback(9, 0x200cb28);
    Event_Wait(20);
    Actor_EnableActionCallback(26, 0x200cb9c);
    Event_Wait(40);
    Audio_PlayCue(159);
    Map_AnimateCells(0x200d7cc, 35, 73);
    Object_LookupAndStep_2(26);
    BattleEffect_PlayQueuedSound_4();
    Event_Wait(40);
    base5_e9b = (s32)Data_00000e9b;
    Event_SetMessage(base5_e9b);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_SetAnimationAndWait(26, 3);
    Event_ShowMessageAndWait(0x201a, 0, 40);
    Actor_SetAnimation(9, 3);
    Actor_SetAnimationAndWait(26, 3);
    Event_Wait(30);
    ObjectMotion_EnableActionAndSetCallback_7(9, 0x200cc0c);
    Actor_EnableActionCallback(26, 0x200cc5c);
    Event_Wait(40);
    Camera_SetSpeed(0x20000, 0x4000);
    Camera_MoveTo(0x690000, -1, 0x43e0000, 1);
    Object_LookupAndStep_3(9);
    Actor_FaceDirection(9, 0, 0);
    Actor_ShowEmote(9, 0x100, 40);
    Event_ShowMessageAndWait(9, 0, 10);
    Actor_FaceDirection(0, 0x8000, 0);
    Actor_FaceDirection(22, 0x8000, 10);
    Actor_WalkToAndWait(9, 105, 0x43e);
    Actor_RunRepeatedMotion(9, 2);
    Event_OpenMessage(0x8009, 0);
    Actor_FaceDirection(22, 0, 0);
    /* Branch on a condition; pass byte 4 or byte 5 of the Data_00000e9b
     * table to the corresponding follow-up call. */
    if (Event_ChooseYesNo(0, 0) == 0) {
        Actor_SetAnimationAndWait(9, 3);
        Event_SetMessage((base5_e9b + 4));
    } else {
        Actor_RunRepeatedMotion(9, 2);
        Event_SetMessage((base5_e9b + 5));
    }
    Event_ShowMessage(0x8009, 0);
    Actor_FaceDirection(22, 0x8000, 40);
    Actor_ShowEmote(9, 0x100, 30);
    base5_ea1 = (s32)Data_00000ea1;
    Event_SetMessage(base5_ea1);
    Event_OpenMessage(0x8009, 0);
    /* Branch on a condition; each side reads a different byte of the
     * Data_00000ea1 table and runs its own follow-up sequence. */
    if (Event_ChooseYesNo(0, 0) == 0) {
        Actor_SetAnimationAndWait(9, 3);
        Event_SetMessage((base5_ea1 + 1));
        Event_ShowMessageAndWait(0x8009, 0, 30);
        Actor_FaceDirection(22, 0x8000, 20);
        Actor_SetAnimation(0, 3);
        Actor_SetAnimation(22, 3);
        Actor_SetAnimationAndWait(9, 3);
        Event_Wait(40);
    } else {
        Actor_ShowEmote(9, 0x105, 90);
        Actor_ShowEmote(9, 0x103, 40);
        Actor_SetAnimation(9, 4);
        Event_SetMessage((base5_ea1 + 2));
        Event_ShowMessage(0x8009, 0);
    }
    Actor_EnableActionCallback(9, 0x200cca8);
    Event_Wait(90);
    Actor_FaceEachOther(0, 22, 0);
    Event_Wait(40);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimationAndWait(22, 3);
    Event_Wait(20);
    Actor_SetAnimation(22, 2);
    /* If a record pointer is returned, pass its s16 fields at offsets 10
     * and 18 through to the follow-up call. */
    record = Scene_GetRecord_4(0);
    if (record != 0) {
        Actor_SetDestination(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(22);
    Actor_SetPosition(22, 0, 0);
}

void SceneDialogue_RunActorTenFlag30dDialogue(void)
{
    s32 v2000 = 0x2000;
    s32 msg1 = 0xEA5;
    s32 msg0 = 0xEA4;
    u8 *tbl;

    Event_Begin();
    Actor_SetAnimation(10, 1);
    Event_Wait(10);
    Actor_FaceEachOther(10, 0, 20);
    if (GameFlag_IsSet((s32)&Value_0000030d) != 0) {
        Event_SetMessage(msg1);
        Event_ShowMessageAndWait(10, 0, 10);
    } else {
        Event_SetMessage(msg0);
        Actor_StartRepeatedMotion(10, 1);
        Event_ShowMessageAndWait(10, 0, 10);
        Actor_StartRepeatedMotion(10, 2);
        Event_ShowMessageAndWait(10, 0, 10);
    }
    Actor_FaceDirection(10, v2000, 20);
    Actor_SetAnimation(10, 5);
    Event_Wait(10);
    {
        u8 *rec;
        s32 v;
        rec = Actor_Get(10);
        v = Func_020073ea(Random_Next(), 0x5A) + 60;
        tbl = Data_0200cec8;
        *(u16 *)(rec + 0x64) = v;
        Actor_EnableActionCallback(10, tbl);
    }
    Event_Wait(20);
    GameFlag_Set((s32)&Value_0000030d);
    Event_End();
}

void FieldScene_RunScene372_02003c48(void)
{
    u32 i;
    s32 record;

    Task_Wait(20);
    GameFlag_Set(0x166);
    Func_02008300(0);
    Func_02008306(1);
    Func_0200830c(2);
    Func_02008312(3);
    Func_02008318(4);
    Func_0200831e(5);
    ColorBuffer_ApplyTarget(0x10003, 1);
    ColorBuffer_ApplyTarget(0x10000, 2);
    ColorBuffer_Interpolate(1);
    Task_Wait(120);
    ColorBuffer_ApplyTarget(0, 0);
    ColorBuffer_Interpolate(60);
    Task_Wait(60);
    GameFlag_Clear(0x166);
    Func_02008356(0);
    Func_0200835c(1);
    Func_02008362(2);
    Func_02008368(3);
    Func_0200836e(4);
    Func_02008374(5);
}

void ActorPresentation_SelectActorTwentySevenState(void)
{
    struct Actor *actor = Actor_Get(27);
    u32 flags = Data_03001e40;
    u8 *presentation = actor->presentation;

    if (flags & 1) {
        u8 *state = presentation + 35;
        *state = 2;
    } else {
        u8 *state = presentation + 35;
        *state = 64;
    }
}

void FieldScene_RunScene372_02003e48(void)
{
    u32 i;
    s32 rec7;
    s32 rec8;
    s32 record;

    rec8 = Value1(Engine_ActorGet, 0);
    rec7 = Value1(Engine_ActorGet, 8);
    Event_Begin();
    if (GameFlag_IsSet(0x305) != 0) {
        Actor_Stop(8);
        Event_Wait(10);
        Actor_RunRepeatedMotion(8, 2);
        Event_Wait(40);
        if (*(s16 *)(rec8 + 6) >= 0) {
            Actor_SetAnimation(8, 7);
        } else {
            Actor_SetAnimation(8, 8);
        }
        Actor_RunRepeatedMotion(8, 2);
        Event_Wait(20);
        Event_SetMessage(0xed2);
        Event_ShowMessage(8, 0);
        Value2(Engine_ActorEnableActionCallback, 8, 0x200cec8);
        Actor_SetAnimation(8, 6);
    } else {
        Actor_Stop(8);
        *(s32 *)(rec7 + 24) = 0x10000;
        *(s32 *)(rec7 + 28) = 0x10000;
        Actor_FaceDirection(8, 0x1000, 0);
        if (*(s16 *)(rec8 + 6) >= 0) {
            Actor_SetAnimation(8, 7);
        } else {
            Actor_SetAnimation(8, 8);
        }
        Event_Wait(20);
        Event_SetMessage(0xed1);
        Event_ShowMessageAndWait(8, 0, 20);
        Actor_SetAnimation(8, 1);
        Actor_Jump(8, 4, 0);
        Event_Wait(80);
        Actor_RunRepeatedMotion(8, 2);
        Event_Wait(40);
        if (*(s16 *)(rec8 + 6) >= 0) {
            Actor_SetAnimation(8, 7);
        } else {
            Actor_SetAnimation(8, 8);
        }
        Event_Wait(2);
        Actor_Jump(8, 2, 0);
        Event_Wait(60);
        Actor_RunRepeatedMotion(8, 2);
        Event_Wait(20);
        Event_ShowMessage(8, 0);
        Value2(Engine_ActorEnableActionCallback, 8, 0x200cec8);
        Actor_SetAnimation(8, 6);
        GameFlag_Set(0x305);
    }
    Event_End();
}

void FieldScene_ConfigureActorTwentyTwoScene(void)
{
    u32 i;
    u8 *record;

    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Actor_Stop(ACTOR_ID);
    Call1_02001828(Func_0200861e_a, 0x200c5b9);
    Actor_WalkToAndWait(0, 0x1e0, 0x570);
    Actor_SetPosition(0, 0, 0);
    Actor_FaceDirection(ACTOR_ID, 0x3000, 20);
    Actor_Get(ACTOR_ID)->priority_flags |= ACTOR_PRIORITY_AUTOMATIC;
    Actor_SetPosition(ACTOR_ID, 0xf90000, 0x4d80000);
    Task_Wait(1);
    Event_SetMessage(0xed3);
    Event_ShowMessage(0x1016, 0);
    Actor_SetPosition(ACTOR_ID, 0xac0000, 0x4fe0000);
    Task_Wait(1);
    Camera_SetSpeed(0x40000, 0x8000);
    Camera_MoveTo(0xa20000, 0, 0x5050000, 1);
    Camera_WaitForMove();
    Event_Wait(40);
    Actor_SetAnimationAndWait(ACTOR_ID, 4);
    Event_ShowMessageAndWait(0x1016, 0, 10);
    Actor_FaceDirection(ACTOR_ID, 0xc000, 20);
    Actor_RunRepeatedMotion(ACTOR_ID, 2);
    Event_ShowMessageAndWait(0x1016, 0, 10);
    Actor_FaceDirection(ACTOR_ID, 0x1000, 20);
    Actor_SetAnimationAndWait(ACTOR_ID, 3);
    Actor_SetSpeed(ACTOR_ID, 0x20000, 0x10000);
    Actor_WalkToAndWait(ACTOR_ID, 165, 0x514);
    Actor_WalkToAndWait(ACTOR_ID, 195, 0x598);
    GameFlag_Set(0x842);
}

/* Each of the 15 placement calls below takes the same 6-argument shape:
 * two coordinate-like values, two more coordinate-like values, and a
 * trailing pair of small counts. The final call takes no arguments. */
void FieldScene_BuildPlacementGrid(void)
{
    u32 i;
    u8 *record;

    Map_CopyCellsTo(16, 96, 11, 73, 6, 3); /* main:08009180 */
    Map_CopyCellsTo(16, 96, 34, 68, 14, 10); /* main:08009180 */
    Map_CopyCellsTo(16, 96, 64, 68, 7, 7); /* main:08009180 */
    Map_CopyCellsTo(9, 95, 11, 73, 6, 3); /* main:08009180 */
    Map_CopyCellsTo(40, 94, 34, 68, 14, 10); /* main:08009180 */
    Map_CopyCellsTo(54, 94, 64, 68, 8, 7); /* main:08009180 */
    Map_CopyCellsTo(72, 75, 72, 76, 1, 1); /* main:08009180 */
    Map_CopyCellsTo(72, 75, 74, 76, 1, 1); /* main:08009180 */
    Map_CopyCellAttributes(7, 75, 1, 1, 6, 75); /* main:080091c0 */
    Map_CopyCellAttributes(8, 70, 3, 1, 8, 71); /* main:080091c0 */
    Map_CopyCellAttributes(8, 70, 2, 1, 9, 72); /* main:080091c0 */
    Map_CopyCellAttributes(8, 70, 2, 1, 9, 73); /* main:080091c0 */
    Map_CopyCellAttributes(11, 66, 1, 1, 8, 73); /* main:080091c0 */
    Map_CopyCellAttributes(12, 66, 1, 4, 11, 73); /* main:080091c0 */
    Map_CopyCellAttributes(25, 0, 1, 1, 6, 74); /* main:080091c0 */
    /* No-argument call that closes out the sequence started above. */
    Map_Redraw(); /* main:08009128 */
}

void SceneState_SetWords1c0And1c8AndRun(void)
{

    u8 *state;

    Event_Begin();
    state = *(u8 **)0x03001ebc;
    *(s32 *)(state + 0x1C0) = 0x200;
    *(s32 *)(state + 0x1C8) = 64;
    GameFlag_Set(0x87c);
    Func_02008aa0(12, 2);
    GameFlag_Set(0x900);
    Event_End();
}

void SceneState_SetWorkWordsAndFlag87f(void)
{

    u8 *state;

    Event_Begin();
    state = *(u8 **)0x03001ebc;
    *(s32 *)(state + 0x1C0) = 0x200;
    *(s32 *)(state + 0x1C8) = 64;
    GameFlag_Set(0x87f);
    Func_02008ae4(12, 3);
    GameFlag_Set(0x900);
    Event_End();
}

void SceneActor_SetModeByFrameBit1(s32 o)
{
    s32 v;

    if ((*(volatile s32 *)0x03001e40 & 2) != 0) {
        Object_SetPartPalettes(o, 7);
    } else {
        Object_SetPartPalettes(o, 0);
    }
    {
        volatile s32 *q = (volatile s32 *)0x03001e40;
        v = (*(s32 *)0x0200d7fc << 3) + 16;
        if (Func_0200891e(*q, v) == 0) {
            Func_02008710(o);
        }
    }
}

void OverlayObject_UpdateRandomSlotByFrame(s32 obj)
{
    volatile s32 *fc = (volatile s32 *)0x03001e40;
    s32 t;
    s32 n;

    if ((*fc & 1) != 0) {
        t = (s32)((u32)*fc >> 1);
        Object_SetPartPalettes(obj, Func_02008950(t, 6));
    }
    n = (*(s32 *)0x0200d7fc << 3) + 16;
    if (Func_02008966(*fc, n) == 0) {
        Func_02008758(obj);
    }
}

void OverlayObject_ApplyIwramWord1e40(s32 o)
{
    volatile s32 *p = (s32 *)0x03001e40;
    s32 t;

    if ((*p & 1) != 0) {
        t = (s32)((u32)*p >> 1);
        Object_SetPartPalettes(o, Func_02008998(t, 6));
    }
}

void SceneEffect_UpdateArcOverAnchor(Obj *o)
{
    Obj *b;
    s32 t;
    s32 d;
    s32 k;

    b = o->f68;
    o->f64++;
    t = (s16)o->f64;
    t = (s16)o->f64;
    if (t > 31) {
        Engine_ObjectDispatchRelease(o);
    } else {
        d = Math_Sin(t << 10);
        o->f18 = d;
        o->f1c = d;
        o->f08 = b->f08;
        k = 0x10000;
        o->f0c += k;
        k -= d;
        o->f10 = b->f10 + ((k << 2) + k) + 0x80000;
    }
}

void OverlayObject_UpdateArcFromParent(Obj *o)
{
    Obj *b;
    s32 t;
    s32 d;
    s32 k;

    b = o->f68;
    o->f64++;
    t = (s16)o->f64;
    t = (s16)o->f64;
    if (t > 31) {
        Engine_ObjectDispatchRelease(o);
    } else {
        d = Math_Sin(t << 10);
        o->f18 = d;
        o->f1c = -d;
        o->f08 = b->f08;
        k = 0x10000;
        o->f0c += k;
        k -= d;
        o->f10 = b->f10 - ((k << 2) + k) + 0x100000;
    }
}

void SceneState_SetValue140Mode0(void)
{

    Psynergy_Begin(140, 0);
}

void FieldScene_RunSingleStep(void)
{
    Func_02008e00();
}

void FieldScene_RunFourPairedSteps(void)
{
    Actor_Get(32);
    Func_0200887a_a();
    Actor_Get(33);
    Func_02008884_a();
    Actor_Get(30);
    Func_0200888e();
    if (*(s32 *)0x0200d7f8 == 0) {
        Actor_Get(29);
        Func_020088a0();
    }
}

void SceneState_SetValue19ThenCall(void)
{

    Actor_Get(19);
    Func_020088fe();
}

void OverlayObject_CopyRecordField1ToSlots22And8(void)
{
    Ent *src;
    Ent *dst;
    Ent *dst2;

    src = Func_02008cea(0)->f50;
    dst = Func_02008cf2(22)->f50;
    dst->f = src->f;
    dst2 = Func_02008d0e(8)->f50;
    dst2->f = src->f;
}

void SceneState_SetValueEe4(void)
{

    Event_Begin();
    Message_ShowCentered(0xee4, 1);
    Event_End();
}

void FieldScene_RunScriptedStep1120(void)
{
    Event_Begin();
    Message_ShowCentered((s32)&Value_00001120, 1);
    Event_End();
}
