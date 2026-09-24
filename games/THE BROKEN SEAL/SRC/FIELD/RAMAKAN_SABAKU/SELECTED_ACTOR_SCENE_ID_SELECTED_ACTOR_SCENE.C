#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#define NULL ((void *)0)
#define CreateOverlayObject Func_02001dae
#define SetOverlayObjectMode Func_02001e10
#define SetOverlayObjectSlot Func_02001eb8

#include "CREATE_CONFIGURED_OVERLAY_OBJECT.H"

extern u8 Value_00000059;
extern u8 Value_0000005a;
extern u8 Value_0000005b;
extern u8 Value_0000005c;
extern u8 Data_0200a174[];
extern u8 Data_0200a3b0[];
extern u8 Data_0200a3c8[];
extern u8 Data_0200a410[];
extern u8 Data_0200a4b8[];
extern u8 Data_0200a1d4[];
extern u8 Data_0200a234[];
extern u8 Data_0200a2dc[];
extern u8 Data_0200a12c[];
extern u8 Data_03001ebc[];
extern u8 Data_0000005b[];

void Func_02002152();
void Func_02002a54();
void Func_02002a5e();
typedef s32(*IwramSqrt)(s32);
void Func_02002512();
void Func_02003216_a();
s32 Func_020032c4();
s32 Func_0200332c();

/*
 * Waits for an object field to fall to a limit -- resource_3a5.
 */

/* Contiguous unnamed leaf-owner run for resource_3a5. */

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

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call3_02001874(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

void *OverlayObject_PrepareObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    u8 *obj = Object_Create(arg3, arg0, arg1, arg2);

    if (obj != NULL) {
        u8 *rec = *(u8 **)(obj + 0x50);
        s32 flags;
        s32 mask = 13;

        flags = rec[9];
        mask = -mask;
        mask &= flags;
        rec[9] = mask;
        obj[0x55] = 0;
        obj[0x59] = 8;
        Actor_SetSpriteFlags(obj, 0);
        Object_SetPalette(obj, 14);
        Object_SetBlendMode(obj, 1);
        return obj;
    }
    return NULL;
}

void *OverlayObject_CreateConfiguredObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
#include "CREATE_CONFIGURED_OVERLAY_OBJECT_BODY.INC"
}

void FieldScene_ApplyActor13Values3And3(void) { Func_02002152(13, 3, 3); }

/*
 * The countdown is tested at the top of the loop and decremented inside the
 * body, after the call.  A post-decrement test would move the subtract ahead
 * of the call.
 */
void OverlayObject_WaitUntilField12BelowLimit(u8 *o, s32 limit)
{
    s32 frames = 60;

    while (frames != 0) {
        Task_Wait(1);
        frames--;
        if (*(s32 *)(o + 12) <= limit) break;
    }
}

s32 SceneData_SelectTableByScene59To5c(void)
{
    s16 v = gGameState.scene;

    if (v == (s32)&Value_00000059) {
        return (s32)Data_0200a174;
    }
    if (v == (s32)&Value_0000005a) {
        return (s32)Data_0200a1d4;
    }
    if (v == (s32)&Value_0000005b) {
        return (s32)Data_0200a234;
    }
    if (v == (s32)&Value_0000005c) {
        return (s32)Data_0200a2dc;
    }
    return (s32)Data_0200a12c;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

void *SceneData_GetTablea36c(void)
{
    return (void *)0x0200a36c;
}

s32 SceneData_SelectTableBySceneId(void)
{
    if (gGameState.scene == (s32)&Value_0000005b) {
        if (gGameState.entrance == 5) {
            GameFlag_Set(0x90a);
        }
    }
    if (gGameState.scene == (s32)&Value_00000059) {
        return (s32)Data_0200a3c8;
    }
    if (gGameState.scene == (s32)&Value_0000005a) {
        return (s32)Data_0200a410;
    }
    if (gGameState.scene == (s32)&Value_0000005b) {
        return (s32)Data_0200a4b8;
    }
    return (s32)Data_0200a3b0;
}

void FieldScene_RunFlags8B2And8B3Steps(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    if (GameFlag_IsSet(0x8b2) == 0) {
        if (GameFlag_IsSet(0x8b3) == 0) {
            GameFlag_Set(0x8b3);
            GameFlag_Set(0x8b2);
        }
    }
    Audio_PlayCue(123);
    Event_RequestExit(3);
    Event_End();
}

void *SceneData_GetTableA500(void)
{
    return (void *)0x0200a500;
}

void FieldScene_RunScene3a5_02000c38(void)
{
    extern u8 Data_02000240[];

    Actor_RunRepeatedMotion(8, 2);
    Func_02002a54((s32)Data_0000005b, 5);
    do {
        Data_02000240[0x22b] = 3;
    } while (0);
    Func_02002a5e(53, 5);
}

void FieldScene_RunScene3a5_02000c6c(s32 a0)
{
    s32 i;
    s32 p8;
    s32 record;
    s32 value;
    s32 v5;
    s32 v6;

    p8 = a0;
    for (i = 0; i <= 2; i++) {
        value = Value0(Engine_RandomNext);
        v6 = (u32)((value << 1) + value) >> 16;
        v5 = v6 + 0x303;
        record = GameFlag_IsSet(v5);
        if (record == 0) {
            GameFlag_Set(v5);
            break;
        }
    }
    Event_Begin();
    Event_SetMessage((s32)((s32)(((s32)p8 << 1) + p8) + v6) + 0x1a10);
    Event_ShowMessage((v6 + 1), 0);
    Event_End();
}

s32 CalculatePlanarDistance(s32 *position_a, s32 *position_b)
{
    s32 dx = (*position_b++ - *position_a++) >> 16;
    s32 dz = (*position_b - position_a[1]) >> 16;
    s32 dz_squared = dz *dz;
    s32 dx_squared = dx *dx;

    return ((IwramSqrt)0x030001d8)(dx_squared + dz_squared);
}

void FieldScene_RunScene3a5_020014b0(void)
{
    s32 rec8;
    s32 record;
    s32 rect[3];
    s32 shown;
    u16 *shown_addr;
    u8 *p5;

    p5 = *(u8 **)Data_03001ebc;
    Func_02002512();
    if (GameFlag_IsSet(0x90a) == 0) {
        rec8 = GameFlag_IsSet(0x200);
        if (rec8 == 0) {
            GameFlag_Set(0x200);
            SceneState_SetHalfwordB030(1);
            shown_addr = (u16 *)(p5 + 0xcba);
            shown = 0x258;
            *shown_addr = shown;
            record = Value1(Engine_ActorGet, ACTOR_PARTY_LEADER);
            *(s32 *)(record + 36) = rec8;
            record = Value1(Engine_ActorGet, ACTOR_PARTY_LEADER);
            *(s32 *)(record + 44) = rec8;
            record = Actor_Get(ACTOR_PARTY_LEADER);
            *(s32 *)(record + 56) = -0x80000000;
            record = Actor_Get(ACTOR_PARTY_LEADER);
            *(s32 *)(record + 64) = -0x80000000;
            Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
            Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 0);
            Event_Wait(40);
            Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
            Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
            Event_Wait(40);
            *(u8 *)(Func_020032c4(0) + 90) &= 254;
            rect[0] = rec8;
            rect[1] = rec8;
            rect[2] = rec8;
            record = Actor_Get(ACTOR_PARTY_LEADER);
            Call3(Func_02003216_a, -0x100000, *(u16 *)(record + 6), (s32)rect);
            Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
            Actor_SetAnimation(ACTOR_PARTY_LEADER, 2);
            Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, rect[0] / 0x10000, rect[2] / 0x10000);
            Actor_WaitForMove(ACTOR_PARTY_LEADER);
            Event_Wait(2);
            *(u8 *)(Func_0200332c(0) + 90) |= 1;
            Event_Wait(30);
            Audio_PlayCue(148);
            Actor_RunRepeatedMotion(8, 2);
            Event_Wait(20);
            Actor_SetSpeed(8, 0x28000, 0x14000);
            Actor_WalkToAndWait(8, 168, 104);
            Actor_SetSpeed(8, 0x8000, 0x4000);
            Actor_WalkToAndWait(8, 168, 92);
            *shown_addr = shown;
            SceneState_SetHalfwordB030(0);
        }
    }
}

void FieldScene_RunScene3a5_02001874(void)
{
    u32 i;
    s32 record;

    Actor_SetSpeed(8, 0x8000, 0x4000);
    Actor_SetAnimation(8, 1);
    Actor_WalkToAndWait(8, 168, 96);
    Actor_SetAnimation(8, 2);
}

void SceneState_SetHalfwordB030(u16 value)
{
    *(u16 *)0x0200b030 = value;
}
