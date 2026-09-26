#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#if defined(TBS_EDITION_JA)
#define MSG_LIGHTHOUSE_ALREADY_LIT 0x1725
#elif defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
#define MSG_LIGHTHOUSE_ALREADY_LIT 0x157e
#else
#define MSG_LIGHTHOUSE_ALREADY_LIT 0x159c
#endif
#if defined(TBS_EDITION_DE)
#define Scene58Work Data_03001ecc
#else
#define Scene58Work Data_03001ebc
#endif

extern u8 *Scene58Work;
extern u8 Value_0000003a;

#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

/* Unresolved declarations retain their existing call signatures. */
s32 Func_0200421e();
s32 Func_02004256();
s32 Func_0200426a();
s32 Func_0200427e();
s32 Func_02004410();
void Func_0200441c();
void Func_0200446a();
s32 Func_020044d6();
void Func_020045ba();
s32 Func_0200484e();
void Func_02004988();
s32 Func_02004ac6();
s32 Func_02004b02();
void Func_02004c54();
s32 Func_02004d66_a();
s32 Func_02004d80();
s32 Func_02004dbe();
s32 Func_02004dd0();
s32 Func_02004de0();
s32 Func_02004dec_b();
s32 Func_02004e02();
s32 Func_02004e0e();
void Func_02004e14();
s32 Func_02004fa2();
s32 Func_02004fc2();
s32 Func_020050e6();
s32 Func_0200511e();
s32 Func_02005140_a();

s32 Func_02005170();
void Func_02005294();
void Func_0200529e();

s32 MeasureFixedPointPositionDistance(s32 *first_position, s32 *second_position);

static __inline__ void Call0(void (*f)())
{
    f();
}

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

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

extern u8 *Data_03001ebc;
extern u8 *Data_03001e70;

#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))
#define EVENT_TRANSITION(workspace) FIELD((workspace), s32 *, 0x1C0)
#define EVENT_MESSAGE(workspace) FIELD((workspace), u16 *, 0x1D8)

extern void Func_02003710();
extern void Func_02003728();

/* Loader-relocated ROM calls: each site names the pre-relocation call word the image holds. */
s32 Func_020051ec();
s32 Func_0200521c();
void Func_02005266();
s32 Func_02005276();
void Func_02005352();
s32 Func_02005470();
void Func_02005498();
s32 Func_02005674();
void Func_020056c4_a();
s32 Func_020057d2();
s32 Func_0200582c();
void Func_02005880_a();
void Func_020058f8();
void Func_02005926();
s32 Func_020059c4();
void Func_02005aee();
s32 Func_02005b94();
s32 Func_02005c7a();
s32 Func_02005c92();
s32 Func_02005c9c_a();
s32 Func_02005df6();
s32 Func_02005ea8();
s32 Func_02005fa8();
s32 Func_02006092();
s32 Func_020060e4();
s32 Func_02006156();
void Func_0200615c();
s32 Func_02006258();

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
#define OverlayObject_PrepareSpawnedObject      Func_02000048
#define AcquireOverlayObject      Func_020036ca
#define RunOverlayObjectCommand0  Func_02003710
#define RunOverlayObjectCommand1  Func_02003728
#define CreateOverlayObject Func_02003722
#define SetOverlayObjectMode Func_0200376c
#define SetOverlayObjectSlot Func_02003844
void Effect_Move(union Slot *object);

#include "OVERLAY_OBJECT.H"
#include "CREATE_CONFIGURED_OVERLAY_OBJECT.H"

struct OverlayObjectRecordFieldView {
    u8 unknown_00[9];
    u8 unknown_00_0 : 2;
    u8 field_01 : 2;
    u8 unknown_04_0 : 4;
};

union Slot {
    s32 w;
    u16 h[2];
    void *p;
};

void *Func_020036ca(s32, s32, s32, s32);
void Func_02003710(void *, s32);
void Func_02003728(void *, s32);

extern u8 Data_0200b938[];

struct Record {
    u8 pad00[85];
    u8 mode55;              /* +85, 0x55 */
};

extern u8 Data_0200b9c8[];
extern u8 Data_0200b9d4[];
extern u8 Data_0200bbe4[];

struct Record *Func_020041c6();
struct Record *Func_020041d2();
struct Record *Func_020041de();
struct Record *Func_020041e8();
struct Record *Func_020041f2();
struct Record *Func_020041fc();
struct Record *Func_02004206();
void Func_02006c82(void);

/* Contiguous unnamed leaf-owner run for resource_39d. */

/* Configure and publish the scene's presentation layer. */

#define Scene_GetRecord_1(args...) Func_02003c6a(args)
#define Scene_GetRecord_2(args...) Func_02003c76(args)
#define Scene_GetRecord_3(args...) Func_02003c82(args)
#define Scene_GetRecord_4(args...) Func_02003c8e(args)
#define RuntimeBlock_GetOffset1e0Pointer_1(args...) Func_02003dd4(args)
#define RuntimeBlock_GetOffset1e0Pointer_2(args...) Func_02003dd4(args)
#define Scene_GetRecord_5(args...) Func_02003dd8(args)
#define Scene_GetRecord_6(args...) Func_02003e14(args)
#define Scene_GetRecord_7(args...) Func_02003e5c(args)
#define Scene_GetRecord_8(args...) Func_02003e82(args)
#define Scene_GetRecord_9(args...) Func_02003ee0(args)
#define Scene_GetRecord_10(args...) Func_02003ef8(args)
#define Scene_GetRecord_11(args...) Func_02003f10(args)
#define SCENE_PHASE (*(s32 *)(*(u8 **)0x03001ebc + 0x1c0))
#define ACTOR_FIELD_0XA(record) (*(s16 *)((record) + 10))
#define ACTOR_FIELD_0X12(record) (*(s16 *)((record) + 18))

extern u8 Data_0200bc50[];

void Func_02003c3c();
u8 *Func_02003c6a();
u8 *Func_02003c76();
u8 *Func_02003c82();
u8 *Func_02003c8e();
s32 Func_02003dd4();
u8 *Func_02003e14();
u8 *Func_02003e5c();
u8 *Func_02003e82();
u8 *Func_02003ee0();
u8 *Func_02003ef8();
u8 *Func_02003f10();
u8 *Func_02003dd8();
s32 Func_020040f2();
s32 Func_02004112();
void Func_0200619e();
s32 Func_020064f2();
s32 Func_0200650c();
s32 Func_02006516();
s32 Func_020065ec();
void Func_020060fc();

/* Newly named call sites: the mechanical pass left these spelled raw because
 * their calling form (a cast, or an odd argument count) did not match its
 * patterns, or because the engine function itself had no name yet. */

/* Status/phase word at 0x1c0 of the shared scene work record. */

/* Two s16 fields (offsets 0xa and 0x12) read together from an actor slot
 * record and forwarded as a pair of arguments. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

/* Configures four actor slots (0-3), advances the shared scene phase, then
 * drives handles 1-3 through a sequence of timed calls, some of which use
 * the two s16 fields read back from an actor slot record. */

typedef s32(*IwramIntegerSquareRoot)(s32);

#define GetActor Func_02003fd0
#define MeasurePositionDistance MeasureFixedPointPositionDistance

typedef struct {
    u8 pad_to_kind[0x28];
    s16 *kind;
} ActorData;

typedef struct {
    u8 pad_to_data[0x50];
    ActorData *data;
} Actor;

Actor *Func_02003fd0(s32 actor_id);

void SetOverlayObjectRecordField1(struct OverlayObject *object, s32 value)
{
    struct OverlayObjectRecordFieldView *record;
    record = (struct OverlayObjectRecordFieldView *)object->record;
    record->field_01 = value;
}

void *OverlayObject_PrepareSpawnedObject(s32 first, s32 second, s32 third, s32 fourth)
{
    void *obj;
    void *rec;
    s32 mask;

    obj = AcquireOverlayObject(fourth, first, second, third);
    if (obj != NULL) {
        rec = FIELD_AT_OFFSET(obj, void *, 0x50);
        mask = -0xD;
        FIELD_AT_OFFSET(rec, u8, 9) = (u8)(mask & FIELD_AT_OFFSET(rec, u8, 9));
        FIELD_AT_OFFSET(obj, u8, 0x55) = 0;
        FIELD_AT_OFFSET(obj, u8, 0x59) = 8;
        RunOverlayObjectCommand0(obj, 0);
        Object_SetPalette(obj, 0xE);
        RunOverlayObjectCommand1(obj, 1);
        return obj;
    }
    return NULL;
}

void *OverlayObject_CreateConfiguredObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
#include "CREATE_CONFIGURED_OVERLAY_OBJECT_BODY.INC"
}

void *SceneData_GetTableB938(void)
{
    return Data_0200b938;
}

s32 Func_0200031c(void)
{
    return 0;
}

void *SceneData_GetTableb9c8(void)
{
    return Data_0200b9c8;
}

void *SceneData_GetTableB9d4AfterStateCheck(void)
{
    if (gGameState.entrance != 1) {
        GameFlag_Set(0x253);
    }
    return Data_0200b9d4;
}

void *SceneData_GetTablebbe4(void)
{
    return Data_0200bbe4;
}

void FieldScene_RunFourActorPresentation(void)
{
    u32 i;
    u8 *record;

    Event_Begin();
    Func_02003c3c(); /* main:08077268 */
    record = Scene_GetRecord_1(0); /* main:0808a080 */
    Actor_SetSpriteFlags(record, 0); /* main:080091e0 */
    record = Scene_GetRecord_2(1); /* main:0808a080 */
    Actor_SetSpriteFlags(record, 0); /* main:080091e0 */
    record = Scene_GetRecord_3(2); /* main:0808a080 */
    Actor_SetSpriteFlags(record, 0); /* main:080091e0 */
    record = Scene_GetRecord_4(3); /* main:0808a080 */
    Actor_SetSpriteFlags(record, 0); /* main:080091e0 */
    Camera_MoveTo(0x1300000, -1, 0x780000, 0);
    Task_Wait(1); /* main:080000c0 */
    Map_Redraw(); /* main:08009128 */
    Task_Wait(1); /* main:080000c0 */
    Audio_PlayCue(141);
    Work_SetValuesIfNonNegative(0x50000, 0x50000, 0x10000); /* main:080091f0 */
    Audio_PlayCue(0x121);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666); /* main:080091f0 */
    SCENE_PHASE = 0x100;
    Event_OpenScreen(); /* main:0808a360 */
    Event_WaitForScreen(); /* main:0808a370 */
    MapRender_WaitForValues(); /* main:080091f8 */
    Event_Wait(30);
    /* Clear the byte at offset 85 of the record RuntimeBlock_GetOffset1e0Pointer_1() returns. */
    *(u8 *)(RuntimeBlock_GetOffset1e0Pointer_2() + 85) = 0;
    Camera_SetSpeed(0xcccc, 0x1999); /* speed_limit, acceleration */
    Camera_MoveTo(0x2000000, -0x180000, 0xa00000, 1);
    Camera_WaitForMove();
    ColorBuffer_ApplySource(0x10000, 0);
    ColorBuffer_ApplyTarget(0x10005, 0); /* main:0808a330 */
    ColorBuffer_Interpolate(50); /* main:0808a348 */
    Event_Wait(50);
    ColorBuffer_ApplyTarget(0x7fff, 0); /* main:0808a330 */
    ColorBuffer_Interpolate(30); /* main:0808a348 */
    Event_Wait(30);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x1f80000, 0xa80000);
    Actor_SetPosition(ACTOR_GERALD, 0x2100000, 0x900000);
    Actor_SetPosition(ACTOR_IVAN, 0x1e80000, 0x900000);
    Actor_SetPosition(ACTOR_MIA, 0x2000000, 0x980000);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 19); /* object 0, action 19 */
    Actor_SetAnimation(ACTOR_GERALD, 19);
    Actor_SetAnimation(ACTOR_IVAN, 19);
    Actor_SetAnimation(ACTOR_MIA, 19);
    Event_Wait(10);
    ColorBuffer_ApplyTarget(0x10000, 0); /* main:0808a330 */
    ColorBuffer_Interpolate(30); /* main:0808a348 */
    Event_Wait(30);
    Event_Wait(80);
    record = Scene_GetRecord_5(0);
    Actor_SetSpriteFlags(record, 1); /* main:080091e0 */
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 4);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 20);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Event_Wait(60);
    record = Scene_GetRecord_6(1); /* main:0808a080 */
    Actor_SetSpriteFlags(record, 1); /* main:080091e0 */
    Actor_SetAnimation(ACTOR_GERALD, 1);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_GERALD, 0x2000, 0);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 0);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_GERALD, 0, 0);
    record = Scene_GetRecord_7(2);
    Actor_SetSpriteFlags(record, 1); /* main:080091e0 */
    Actor_SetAnimation(ACTOR_IVAN, 1);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 0);
    Event_Wait(40);
    record = Scene_GetRecord_8(3); /* main:0808a080 */
    Actor_SetSpriteFlags(record, 1); /* main:080091e0 */
    Actor_SetAnimation(ACTOR_MIA, 1);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_Wait(20);
    Actor_SetSpeed(ACTOR_GERALD, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_IVAN, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_MIA, 0xcccc, 0x6666);
    Actor_SetAnimation(ACTOR_GERALD, 2);
    Actor_SetAnimation(ACTOR_IVAN, 2);
    Actor_SetAnimation(ACTOR_MIA, 2);
    record = Scene_GetRecord_9(0); /* main:0808a080 */
    if (record != 0) {
        Actor_SetDestination(ACTOR_GERALD, ACTOR_FIELD_0XA(record), ACTOR_FIELD_0X12(record));
    }
    record = Scene_GetRecord_10(0); /* main:0808a080 */
    if (record != 0) {
        Actor_SetDestination(ACTOR_IVAN, ACTOR_FIELD_0XA(record), ACTOR_FIELD_0X12(record));
    }
    record = Scene_GetRecord_11(0); /* main:0808a080 */
    if (record != 0) {
        Actor_SetDestination(ACTOR_MIA, ACTOR_FIELD_0XA(record), ACTOR_FIELD_0X12(record));
    }
    Actor_WaitForMove(ACTOR_MIA);
    Actor_SetPosition(ACTOR_MIA, 0, 0);
    Actor_WaitForMove(ACTOR_IVAN);
    Actor_SetPosition(ACTOR_IVAN, 0, 0);
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Event_End();
}

s32 MeasureFixedPointPositionDistance(s32 *first_position, s32 *second_position)
{
    s32 delta_x = (*first_position++ - *second_position++) >> 16;
    s32 delta_y = (*first_position++ - *second_position++) >> 16;
    s32 delta_z = (*first_position - *second_position) >> 16;
    s32 delta_x_squared = delta_x *delta_x;
    s32 delta_y_squared = delta_y *delta_y;
    s32 delta_z_squared = delta_z *delta_z;

    return ((IwramIntegerSquareRoot)0x030001D8)(delta_x_squared + delta_y_squared + delta_z_squared);
}

s32 FindNearestF2Actor(void)
{
    u8 *work = *(u8 **)0x03001ebc;
    Actor **actor_slot;
    Actor *origin;
    s32 nearest_actor = 0;
    s32 min_dist;
    u32 actor_id;

    min_dist = 640;
    origin = GetActor(0);
    actor_id = 8;
    actor_slot = (Actor **)(work + 0x34);
    do {
        Actor *actor = *actor_slot++;
        if (actor != 0) {
            if (*actor->data->kind == 0xf2) {
                s32 dist = MeasurePositionDistance(
                    (u8 *)origin + 8, (u8 *)actor + 8);
                if (dist < min_dist) {
                    min_dist = dist;
                    nearest_actor = actor_id;
                }
            }
        }
        actor_id++;
    } while (actor_id <= 65);
    return nearest_actor;
}

void FieldScene_RunScene39d_020009fc(void)
{
    u32 i;
    u8 *rec;
    u8 *rec8;
    s32 record;
    s32 base5_200bc50;

    rec = Value1(Func_020040f2, 0);
    Event_Begin();
    record = FindNearestF2Actor();
    base5_200bc50 = (s32)Data_0200bc50;
    *(s32 *)base5_200bc50 = record;
    if (record != 0) {
        GameFlag_Set(0x250);
        rec8 = Value1(Func_02004112, *(s32 *)base5_200bc50);
        rec8[85] = 0;
        rec[85] &= 254;
        *(s32 *)((s32)rec8 + 12) += -0x30000;
        *(s32 *)((s32)rec + 12) += -0x30000;
        *(s32 *)((s32)rec + 20) += -0x30000;
        Task_Wait(2);
        *(s32 *)((s32)rec8 + 12) += -0x20000;
        *(s32 *)((s32)rec + 12) += -0x20000;
        *(s32 *)((s32)rec + 20) += -0x20000;
        Task_Wait(10);
        *(s32 *)((s32)rec8 + 12) += 0x20000;
        *(s32 *)((s32)rec + 12) += 0x20000;
        *(s32 *)((s32)rec + 20) += 0x20000;
        Task_Wait(4);
        *(s32 *)((s32)rec8 + 12) += 0x20000;
        *(s32 *)((s32)rec + 12) += 0x20000;
        *(s32 *)((s32)rec + 20) += 0x20000;
        Task_Wait(4);
        *(s32 *)((s32)rec8 + 12) += 0x10000;
        *(s32 *)((s32)rec + 12) += 0x10000;
        *(s32 *)((s32)rec + 20) += 0x10000;
    }
    Event_End();
}

void SceneActor_SetMode55OnSevenRecords(void)
{
    Func_020041c6(0)->mode55 = 3;
    Func_020041d2(14)->mode55 = 4;
    Func_020041de(15)->mode55 = 4;
    Func_020041e8(16)->mode55 = 4;
    Func_020041f2(17)->mode55 = 4;
    Func_020041fc(18)->mode55 = 4;
    Func_02004206(19)->mode55 = 4;
}

void RunScene58Sequence(void)
{
    extern u8 Data_02000240[];
    void *temp_r0;
    void *temp_r0_10;
    void *temp_r0_11;
    void *temp_r0_12;
    void *temp_r0_13;
    void *temp_r0_2;
    void *temp_r0_3;
    void *temp_r0_4;
    void *temp_r0_5;
    void *temp_r0_6;
    void *temp_r0_7;
    void *temp_r0_8;
    void *temp_r0_9;
    void *temp_r2;
    void *temp_r2_2;
    void *temp_r2_3;
    void *temp_r2_4;
    s32 flag;
    s32 flag2;
    s32 bits;

    Call0(Engine_EventBegin);
    temp_r0 = Value1(Func_0200421e, 0x11);
    FIELD(temp_r0, u8 *, 0x55) = (u8)(0xFA & FIELD(temp_r0, u8 *, 0x55));
    Call3(Engine_ActorSetSpeed, 0, 0x0000cccc, 0x00006666);
    Call3(Engine_ActorSetSpeed, 1, 0x0000cccc, 0x00006666);
    Call3(Engine_ActorSetSpeed, 2, 0x0000cccc, 0x00006666);
    Call3(Engine_ActorSetSpeed, 3, 0x0000cccc, 0x00006666);
    temp_r0_2 = Value1(Func_02004256, 0);
    if (temp_r0_2 != 0) {
        Call3(Engine_ActorSetPosition, 1, FIELD(temp_r0_2, s32 *, 8), FIELD(temp_r0_2, s32 *, 0x10));
    }
    temp_r0_3 = Value1(Func_0200426a, 0);
    if (temp_r0_3 != 0) {
        Call3(Engine_ActorSetPosition, 2, FIELD(temp_r0_3, s32 *, 8), FIELD(temp_r0_3, s32 *, 0x10));
    }
    temp_r0_4 = Value1(Func_0200427e, 0);
    if (temp_r0_4 != 0) {
        Call3(Engine_ActorSetPosition, 3, FIELD(temp_r0_4, s32 *, 8), FIELD(temp_r0_4, s32 *, 0x10));
    }
    Task_Wait(1);
    Call3(Engine_ActorWalkTo, 0, 0x158, 0xE8);
    Call3(Engine_ActorWalkTo, 1, 0x148, 0xE8);
    Call3(Engine_ActorWalkTo, 2, 0x158, 0xF8);
    Call3(Engine_ActorWalkTo, 3, 0x148, 0xF8);
    Call1(Engine_ActorWaitForMove, 0);
    Call3(Engine_ActorFaceDirection, 0, 0xA000, 0);
    Call1(Engine_ActorWaitForMove, 1);
    Call3(Engine_ActorFaceDirection, 1, 0xC000, 0);
    Call1(Engine_ActorWaitForMove, 2);
    Call3(Engine_ActorFaceDirection, 2, 0xA000, 0);
    Call1(Engine_ActorWaitForMove, 3);
    Call3(Engine_ActorFaceDirection, 3, 0xC000, 0);
    Call1(Engine_EventWait, 0x32);
    Call2(Engine_ActorRunRepeatedMotion, 1, 2);
    Call1(Engine_EventWait, 0x14);
    Call2(Engine_CameraSetSpeed, 0x18000, 0x3000);
    Call4(Engine_CameraMoveTo, 0x01480000, 0x280000, 0xB00000, 1);
    FIELD(Value0(Func_02004410), s8 *, 0x55) = 0;
    Call3(Engine_ActorSetSpeed, 1, 0x18000, 0xC000);
    Call3(Engine_ActorWalkToAndWait, 1, 0x148, 0xD8);
    Call0(Engine_CameraWaitForMove);
    Call3(Engine_ActorSetSpeed, 1, 0x0000cccc, 0x00006666);
    Call1(Engine_EventWait, 0x3C);
    Call4(Engine_CameraMoveTo, 0x01580000, 0x180000, 0xE80000, 1);
    Call3(Engine_ActorFaceDirection, 1, 0x2000, 0x14);
    Call0(Engine_CameraWaitForMove);
    Call2(Engine_ActorSetAnimationAndWait, 1, 4);
    Call1(Func_0200441c, MSG_LIGHTHOUSE_ALREADY_LIT);
    Call3(Engine_EventShowMessageAndWait, 1, 0, 0x14);
    Call2(Engine_ActorSetAttachedEffect, 3, 0x102);
    Call1(Engine_EventWait, 0x3C);
    Call3(Engine_ActorSetSpeed, 3, 0x18000, 0xC000);
    Call3(Engine_ActorWalkToAndWait, 3, 0x148, 0xE8);
    Call1(Engine_EventWait, 0xA);
    Call3(Engine_EventShowMessageAndWait, 3, 0, 0x14);
    Call2(Engine_ActorSetAnimationAndWait, 3, 4);
    Call1(Engine_EventWait, 0x14);
    Call3(Engine_EventShowMessageAndWait, 3, 0, 0x1E);
    Call3(Func_0200446a, 2, 0, 0);
    Call1(Engine_EventWait, 0x3C);
    Call3(Engine_ActorWalkToAndWait, 2, 0x150, 0xF8);
    Call3(Engine_ActorFaceDirection, 2, 0xA000, 0);
    Call3(Engine_ActorFaceDirection, 0, 0x8000, 0);
    Call1(Engine_EventWait, 0x1E);
    Call3(Engine_EventShowMessageAndWait, 2, 0, 0x14);
    Call3(Engine_ActorFaceDirection, 3, 0, 0x1E);
    Value2(Func_020044d6, 3, 0);
    if (Value2(Engine_EventChooseYesNo, 0, 0) == 0) {
        Call1(Engine_EventWait, 0x14);
        Call3(Engine_ActorShowEmote, 3, 0x100, 0x3C);
        Call3(Engine_ActorFaceDirection, 1, 0x4000, 0xA);
        Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
        Call3(Engine_EventShowMessageAndWait, 1, 0, 0x14);
        Call3(Engine_ActorFaceDirection, 3, 0xC000, 0);
        Call2(Engine_ActorSetAnimationAndWait, 1, 4);
        Call1(Engine_EventWait, 0x14);
        Call3(Engine_EventShowMessageAndWait, 1, 0, 0x14);
        temp_r2 = Scene58Work;
        FIELD(temp_r2, u16 *, 0x1D8) = (u16)(FIELD(temp_r2, u16 *, 0x1D8) + 2);
    } else {
        temp_r2_2 = Scene58Work;
        FIELD(temp_r2_2, u16 *, 0x1D8) = (u16)(FIELD(temp_r2_2, u16 *, 0x1D8) + 2);
        Event_Wait(0x14);
        Call3(Engine_ActorShowEmote, 3, 0x00000101, 0x3C);
        Call3(Engine_ActorFaceDirection, 1, 0x4000, 0xA);
        Call2(Engine_ActorSetAnimationAndWait, 1, 4);
        Event_ShowMessageAndWait(ACTOR_GERALD, 0, 0x14);
        Call3(Engine_ActorFaceDirection, 3, 0xC000, 0);
        Call2(Engine_ActorRunRepeatedMotion, 1, 2);
        Call1(Engine_EventWait, 0x14);
        Call3(Engine_EventShowMessageAndWait, 1, 0, 0x14);
    }
    Call3(Engine_ActorShowEmote, 3, 0x00000101, 0x3C);
    Call3(Engine_EventShowMessageAndWait, 3, 0, 0x14);
    Call3(Func_020045ba, 0, 1, 0);
    Call1(Engine_EventWait, 0x3C);
    Call3(Engine_ActorFaceDirection, 0, 0x8000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0x4000, 0x14);
    Call2(Engine_ActorSetAnimation, 0, 3);
    Call2(Engine_ActorSetAnimationAndWait, 1, 3);
    Call1(Engine_EventWait, 0x14);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 0x14);
    Call2(Engine_ActorRunRepeatedMotion, 3, 2);
    Call3(Engine_EventShowMessageAndWait, 3, 0, 0x14);
    Call2(Engine_ActorSetAnimation, 0, 3);
    Call2(Engine_ActorSetAnimation, 1, 3);
    Call2(Engine_ActorSetAnimationAndWait, 2, 3);
    Event_Wait(0x14);
    Call2(Engine_ActorSetAnimationAndWait, 3, 4);
    Call1(Engine_EventWait, 0xA);
    Call3(Engine_ActorFaceDirection, 3, 0, 0xA);
    Actor_SetAnimation(ACTOR_MIA, 0x10);
    Call3(Engine_EventShowMessageAndWait, 3, 0, 0x3C);
    Call1(Engine_AudioPlayCue, 0x11);
    Call3(Engine_ActorSetPosition, 5, 0xD80000, 0xC80000);
    Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 0x14);
    Call3(Engine_ActorSetPosition, 5, 0x780000, 0xA00000);
    Call3(Engine_ActorFaceDirection, 5, 0, 0);
    Call2(Engine_ActorSetAnimation, 3, 1);
    Call2(Engine_ActorStartRepeatedMotion, 0, 1);
    Call2(Engine_ActorRunRepeatedMotion, 1, 1);
    Call3(Engine_ActorFaceDirection, 0, 0, 0xA);
    Call3(Engine_ActorFaceDirection, 1, 0xE000, 0x14);
    Call3(Engine_ActorFaceDirection, 2, 0xA000, 0xA);
    Call3(Engine_ActorFaceDirection, 3, 0x8000, 0x14);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xA000, 5);
    Call3(Engine_ActorFaceDirection, 1, 0xC000, 0xA);
    Call3(Engine_ActorFaceDirection, 2, 0x8000, 5);
    Call3(Engine_ActorFaceDirection, 0, 0x8000, 5);
    Call2(Engine_ActorStartRepeatedMotion, 0, 2);
    Call3(Engine_ActorFaceDirection, 1, 0x8000, 0xA);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(0x14);
    Call2(Engine_CameraSetSpeed, 0x30000, 0x6000);
    Call4(Engine_CameraMoveTo, 0x780000, 0xffe80000, 0xA80000, 1);
    Call0(Engine_CameraWaitForMove);
    Call1(Engine_EventWait, 0x28);
    Call2(Engine_ActorRunRepeatedMotion, 0x15, 1);
    Call1(Engine_EventWait, 0x14);
    Call1(Engine_AudioPlayCue, 0x3D);
    Call3(Engine_EventShowMessageAndWait, 0x15, 0, 0x14);
    Call2(Engine_ActorRunRepeatedMotion, 0x17, 1);
    Call1(Engine_EventWait, 0x14);
    Call3(Engine_EventShowMessageAndWait, 0x17, 0, 0x14);
    Call3(Engine_ActorFaceDirection, 0x17, 0xC000, 0x3C);
    Actor_FaceDirection(0x17, 0, 0x14);
    Call3(Engine_EventShowMessageAndWait, 0x17, 0, 0x14);
    Call3(Engine_ActorSetPosition, 1, 0x01080000, 0x01200000);
    Call3(Engine_EventShowMessageAndWait, 1, 0, 0x14);
    Call3(Engine_ActorSetPosition, 1, 0x01480000, 0xD80000);
    Call2(Engine_ActorSetAnimationAndWait, 0x17, 4);
    Call1(Engine_EventWait, 0x14);
    Event_ShowMessageAndWait(0x17, 0, 0x14);
    Actor_SetAnimationAndWait(0x17, 3);
    Call1(Engine_EventWait, 0x14);
    Call3(Engine_EventShowMessageAndWait, 0x17, 0, 0x14);
    Call3(Engine_ActorFaceDirection, 0x17, 0xA000, 0xA);
    Call3(Engine_ActorFaceDirection, 0x15, 0x2000, 0xA);
    Actor_RunRepeatedMotion(0x15, 1);
    Call1(Engine_EventWait, 0x14);
    Call3(Engine_ActorFaceDirection, 5, 0x8000, 0);
    Call3(Engine_ActorFaceDirection, 0x14, 0x8000, 0x1E);
    Call3(Engine_ActorFaceDirection, 0x15, 0, 0x1E);
    Call2(Engine_ActorRunRepeatedMotion, 5, 2);
    Call2(Engine_ActorSetAnimationAndWait, 0x15, 4);
    Call1(Engine_EventWait, 0x14);
    Call3(Engine_ActorFaceDirection, 0x15, 0x2000, 0xA);
    Call2(Engine_ActorSetAnimationAndWait, 0x15, 3);
    Call1(Engine_EventWait, 0x14);
    Call2(Engine_ActorSetAnimationAndWait, 0x17, 3);
    Call3(Engine_ActorSetSpeed, 0x15, 0x0000cccc, 0x00006666);
    Value3(Func_0200484e, 0x15, 0x68, 0xA8);
    Call1(Engine_EventWait, 0x14);
    Call3(Engine_ActorFaceDirection, 0x17, 0, 0x14);
    Call3(Engine_ActorFaceDirection, 0x14, 0, 0xA);
    Call3(Engine_ActorShowEmote, 5, 0x102, 0);
    Actor_ShowEmote(0x14, 0x102, 0x46);
    Call3(Engine_ActorSetPosition, 0x16, 0x01080000, 0x01200000);
    Event_ShowMessageAndWait(0x16, 0, 0x14);
    Call3(Engine_ActorSetPosition, 0x16, 0x01280000, 0x780000);
    Call3(Engine_ActorFaceDirection, 0, 0xA000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0xC000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0xC000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0xC000, 0);
    Call3(Engine_ActorFaceDirection, 5, 0, 0);
    Call3(Engine_ActorFaceDirection, 0x14, 0, 0);
    Call2(Engine_CameraSetSpeed, 0x18000, 0x3000);
    Call4(Engine_CameraMoveTo, 0xE80000, 0x280000, 0x980000, 1);
    Call3(Engine_ActorSetSpeed, 0x16, 0x0000cccc, 0x00006666);
    Call3(Engine_ActorWalkToAndWait, 0x16, 0x110, 0x80);
    Call3(Engine_ActorWalkToAndWait, 0x16, 0x108, 0x98);
    Call3(Engine_ActorWalkToAndWait, 0x16, 0x118, 0xA8);
    Call0(Engine_CameraWaitForMove);
    Call3(Engine_ActorFaceDirection, 0x16, 0x5000, 0x14);
    Call1(Engine_EventWait, 0x14);
    Call2(Engine_EventShowMessage, 0x17, 0);
    Call1(Engine_EventWait, 0x14);
    Actor_SetAnimationAndWait(0x16, 3);
    Call1(Engine_EventWait, 0x14);
    Call2(Engine_EventShowMessage, 0x16, 0);
    Call4(Engine_CameraMoveTo, 0x01280000, 0x280000, 0xD80000, 1);
    Call3(Engine_ActorWalkToAndWait, 0x16, 0x130, 0xB0);
    Call3(Engine_ActorFaceDirection, 0x16, 0x2000, 0);
    Call0(Engine_CameraWaitForMove);
    Call1(Engine_EventWait, 0x14);
    Call2(Engine_ActorStartRepeatedMotion, 0, 1);
    Call2(Engine_ActorStartRepeatedMotion, 1, 1);
    Call2(Engine_ActorStartRepeatedMotion, 2, 1);
    Call2(Engine_ActorRunRepeatedMotion, 3, 1);
    Event_Wait(0x14);
    Call1(Engine_EventWait, 0x14);
    Call2(Engine_ActorSetAnimationAndWait, 0x16, 3);
    Call1(Engine_EventWait, 0x14);
    Call2(Engine_EventShowMessage, 0x16, 0);
    Call1(Engine_EventWait, 0xA);
    Call3(Engine_ActorShowEmote, 0, 0x00000107, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x00000107, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x00000107, 0);
    Call3(Engine_ActorShowEmote, 2, 0x00000107, 0x46);
    Actor_RunRepeatedMotion(0x16, 1);
    Event_Wait(0x1E);
    Event_ShowMessage(0x16, 0);
    Call1(Engine_EventWait, 0xA);
    Call3(Engine_ActorShowEmote, 0, 0x00000105, 0);
    Call3(Engine_ActorShowEmote, 1, 0x00000105, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x00000105, 0);
    Call3(Engine_ActorShowEmote, 3, 0x00000105, 0x46);
    Actor_SetAnimationAndWait(0x16, 3);
    Call1(Engine_EventWait, 0x14);
    Event_ShowMessage(0x16, 0);
    Call1(Engine_EventWait, 0x14);
    Call2(Engine_EventShowMessage, 0x17, 0);
    Call1(Engine_EventWait, 0xA);
    Actor_FaceDirection(0x16, 0x5000, 0);
    Event_Wait(0x28);
    Call2(Engine_EventShowMessage, 0x16, 0);
    Call1(Engine_EventWait, 0x1E);
    Event_ShowMessage(0x17, 0);
    Call1(Engine_EventWait, 0xA);
    Call3(Engine_ActorFaceDirection, 0x16, 0x3000, 0);
    Call1(Engine_EventWait, 0x1E);
    Call2(Engine_EventShowMessage, 0x16, 0);
    Call1(Engine_EventWait, 0x14);
    temp_r0_5 = Value1(Func_02004ac6, 1);
    FIELD(temp_r0_5, u8 *, 0x5A) = (u8)(0xFE & FIELD(temp_r0_5, u8 *, 0x5A));
    Call3(Engine_ActorWalkTo, 1, 0x148, 0xE0);
    Call3(Engine_ActorWalkTo, 0, 0x158, 0xE0);
    Actor_WalkTo(ACTOR_IVAN, 0x158, 0xE8);
    Call1(Engine_ActorWaitForMove, 1);
    flag = 1;
    temp_r0_6 = Value1(Func_02004b02, 1);
    bits = FIELD(temp_r0_6, u8 *, 0x5A) | flag;
    FIELD(temp_r0_6, u8 *, 0x5A) = bits;
    Call3(Engine_ActorFaceDirection, 1, 0xC000, 0);
    Call1(Engine_ActorWaitForMove, 0);
    Call3(Engine_ActorFaceDirection, 0, 0xA000, 0);
    Call1(Engine_ActorWaitForMove, 2);
    Call3(Engine_ActorFaceDirection, 2, 0xC000, 0);
    Event_Wait(0x1E);
    Call3(Engine_ActorSetPosition, 0x17, 0xA80000, 0xC80000);
    Call3(Engine_EventShowMessageAndWait, 0x17, 0, 0x14);
    Call3(Engine_ActorSetPosition, 0x17, 0x680000, 0xC80000);
    Call2(Engine_ActorRunRepeatedMotion, 0x16, 1);
    Call1(Engine_EventWait, 0x14);
    Call3(Engine_EventShowMessageAndWait, 0x16, 0, 0x14);
    Call2(Engine_CameraSetSpeed, 0x30000, 0x6000);
    Call4(Engine_CameraMoveTo, 0x780000, 0xffe80000, 0xA80000, 1);
    Call0(Engine_CameraWaitForMove);
    Call2(Engine_ActorSetAnimationAndWait, 0x17, 3);
    Call1(Engine_EventWait, 0xA);
    Call3(Engine_ActorFaceDirection, 0x17, 0xD000, 0x14);
    Call2(Engine_ActorSetAnimationAndWait, 0x17, 3);
    Call1(Engine_EventWait, 0x14);
    Event_ShowMessageAndWait(0x17, 0, 0x14);
    Call3(Engine_ActorFaceDirection, 5, 0x5000, 0);
    Actor_FaceDirection(0x14, 0x3000, 0x46);
    Call3(Func_02004c54, 5, 0x14, 0);
    Call1(Engine_EventWait, 0x32);
    Call3(Engine_ActorFaceDirection, 5, 0x5000, 0);
    Call3(Engine_ActorFaceDirection, 0x14, 0x3000, 0x14);
    Actor_SetAnimationAndWait(ACTOR_JASMINE, 4);
    Call1(Engine_EventWait, 0x14);
    Call3(Engine_ActorShowEmote, 0x17, 0x100, 0x3C);
    Call3(Engine_EventShowMessageAndWait, 0x17, 0, 0x14);
    Call3(Engine_ActorShowEmote, 5, 0x00000105, 0x3C);
    Actor_FaceDirection(0x15, 0xD000, 0x14);
    Call2(Engine_ActorRunRepeatedMotion, 0x15, 1);
    Call2(Engine_ActorSetAnimationAndWait, 0x17, 3);
    Call1(Engine_EventWait, 0x14);
    Call3(Engine_EventShowMessageAndWait, 0x17, 0, 0x14);
    Call2(Engine_ActorSetAnimationAndWait, 0x17, 4);
    Call1(Engine_EventWait, 0x14);
    Call3(Engine_EventShowMessageAndWait, 0x17, 0, 0x14);
    Call2(Engine_ActorSetAttachedEffect, 0x15, 0x102);
    Call1(Engine_EventWait, 0x3C);
    Actor_SetAnimationAndWait(0x15, 4);
    Event_Wait(0x14);
    Call3(Engine_EventShowMessageAndWait, 0x15, 0, 0x14);
    Actor_SetAttachedEffect(ACTOR_JASMINE, 0x102);
    Call1(Engine_EventWait, 0x3C);
    Call3(Engine_EventShowMessageAndWait, 5, 0, 0x28);
    Call2(Engine_ActorSetAnimationAndWait, 0x15, 3);
    Call1(Engine_EventWait, 0x3C);
    Call3(Engine_ActorFaceDirection, 5, 0xA000, 0xA);
    Call2(Engine_ActorSetAnimationAndWait, 0x14, 3);
    Call1(Engine_EventWait, 0x14);
    Call3(Engine_EventShowMessageAndWait, 0x14, 0, 0x1E);
    Call3(Engine_ActorSetSpeed, 5, 0x0000b333, 0x00005999);
    Call3(Engine_ActorSetSpeed, 0x14, 0x0000b333, 0x00005999);
    Call3(Engine_ActorWalkTo, 5, 0x80, 0x90);
    Call3(Engine_ActorWalkToAndWait, 0x14, 0x78, 0x88);
    Actor_FaceDirection(0x14, 0x5000, 0);
    Call1(Engine_ActorWaitForMove, 5);
    Actor_FaceDirection(ACTOR_JASMINE, 0, 0x14);
    Call3(Engine_ActorFaceDirection, 0x15, 0x3000, 0x14);
    temp_r0_7 = Func_02004d66_a(0x15);
    FIELD(temp_r0_7, u8 *, 0x5A) = (u8)(0xFE & FIELD(temp_r0_7, u8 *, 0x5A));
    Call3(Engine_ActorWalkToAndWait, 0x15, 0x58, 0x98);
    temp_r0_8 = Value1(Func_02004d80, 0x15);
    bits = FIELD(temp_r0_8, u8 *, 0x5A) | flag;
    FIELD(temp_r0_8, u8 *, 0x5A) = bits;
    Call3(Engine_ActorFaceDirection, 0x17, 0xB000, 0x14);
    Actor_SetAnimation(0x17, 3);
    Actor_SetAnimationAndWait(0x15, 3);
    Call1(Engine_EventWait, 0x28);
    Call3(Engine_ActorSetSpeed, 0x17, 0x30000, 0x20000);
    FIELD(Value1(Func_02004dbe, 0x17), s32 *, 0x28) = 0x40000;
    Call1(Engine_AudioPlayCue, 0x98);
    temp_r0_9 = Value1(Func_02004dd0, 0x17);
    FIELD(temp_r0_9, u8 *, 0x55) = (u8)(0x7E & FIELD(temp_r0_9, u8 *, 0x55));
    Call2(Engine_ActorSetSpriteFlags, Func_02004de0(0x17), 0);
    FIELD(Func_02004dec_b(0x11), s8 *, 0x55) = 4;
    Call3(Func_02004e14, 0x17, 0x68, 0xA8);
    Call2(Engine_ActorSetSpriteFlags, Func_02004e02(0x17), 1);
    FIELD(Value1(Func_02004e0e, 0x17), s8 *, 0x55) = 3;
    Call3(Engine_ActorFaceDirection, 0x17, 0, 0x1E);
    Call3(Engine_ActorFaceDirection, 0x15, 0, 0xA);
    Call3(Engine_EventShowMessageAndWait, 0x15, 0, 0x14);
    Call2(Engine_ActorRunRepeatedMotion, 5, 2);
    Call2(Engine_ActorSetAnimationAndWait, 5, 4);
    Call1(Engine_EventWait, 0x14);
    Call3(Engine_EventShowMessageAndWait, 5, 0, 0x14);
    Call3(Engine_EventShowMessageAndWait, 0x17, 0, 0x14);
    Call2(Engine_ActorSetSpritePriority, 0x11, 0);
    Actor_SetSpritePriority(0x12, 0);
    Call0(Func_02004988);
    Call2(Engine_ActorSetSpritePriority, 0x11, 1);
    Actor_SetSpritePriority(0x12, 1);
    Call3(Engine_ActorFaceDirection, 0, 0x6000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0x6000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0x6000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0x6000, 0);
    Call4(Engine_CameraMoveTo, 0x01300000, 0x200000, 0xD80000, 1);
    Call0(Engine_CameraWaitForMove);
    Actor_SetPosition(0x14, 0x01100000, 0x01180000);
    Call3(Engine_EventShowMessageAndWait, 0x14, 0, 0x14);
    Actor_SetPosition(0x14, 0, 0);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    Call1(Engine_EventWait, 0x14);
    Call3(Engine_EventShowMessageAndWait, 1, 0, 0x14);
    Call3(Engine_ActorSetSpeed, 1, 0x18000, 0xC000);
    Call3(Engine_ActorWalkToAndWait, 1, 0x138, 0xD8);
    Call3(Engine_ActorFaceDirection, 1, 0x6000, 0xA);
    Call3(Engine_EventShowMessageAndWait, 0x16, 0, 0x14);
    Call3(Engine_ActorFaceDirection, 0, 0xA000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0xC000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0xC000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0xC000, 0);
    Call3(Engine_ActorWalkToAndWait, 0x16, 0x138, 0xB8);
    Call3(Engine_ActorFaceDirection, 0x16, 0x3000, 0x14);
    Call3(Engine_EventShowMessageAndWait, 3, 0, 0x14);
    Call3(Engine_ActorShowEmote, 1, 0x100, 0x3C);
    Call3(Engine_ActorSetSpeed, 1, 0x20000, 0x10000);
    temp_r0_10 = Value1(Func_02004fa2, 1);
    FIELD(temp_r0_10, u8 *, 0x5A) = (u8)(0xFE & FIELD(temp_r0_10, u8 *, 0x5A));
    Actor_WalkToAndWait(ACTOR_GERALD, 0x148, 0xE0);
    Call1(Engine_EventWait, 1);
    temp_r0_11 = Value1(Func_02004fc2, 1);
    flag |= FIELD(temp_r0_11, u8 *, 0x5A);
    FIELD(temp_r0_11, u8 *, 0x5A) = flag;
    Call2(Engine_ActorStartRepeatedMotion, 0, 1);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 1);
    Call2(Engine_ActorStartRepeatedMotion, 2, 1);
    Call2(Engine_ActorRunRepeatedMotion, 3, 1);
    Call1(Engine_EventWait, 0x14);
    Call2(Engine_ActorRunRepeatedMotion, 0x16, 1);
    Event_Wait(0xA);
    Event_ShowMessageAndWait(0x16, 0, 0x14);
    Call2(Engine_ActorRunRepeatedMotion, 1, 2);
    Call1(Engine_EventWait, 0xA);
    Value2(Engine_ActorSetAnimationAndWait, 1, 4);
    Call1(Engine_EventWait, 0xA);
    Call3(Engine_EventShowMessageAndWait, 1, 0, 0x14);
    Call2(Engine_ActorRunRepeatedMotion, 2, 1);
    Call1(Engine_EventWait, 0xA);
    Call2(Engine_ActorSetAnimationAndWait, 2, 3);
    Event_Wait(0x14);
    Call3(Engine_EventShowMessageAndWait, 2, 0, 0x14);
    Call3(Engine_ActorShowEmote, 0x16, 0x00000101, 0x3C);
    Value2(Func_020050e6, 0x16, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Call1(Engine_EventWait, 0x14);
        Call2(Engine_ActorSetAnimationAndWait, 0x16, 4);
        Value1(Engine_EventWait, 0x14);
        Call3(Engine_EventShowMessageAndWait, 0x16, 0, 0x14);
        temp_r2_3 = Scene58Work;
        FIELD(temp_r2_3, u16 *, 0x1D8) = (u16)(FIELD(temp_r2_3, u16 *, 0x1D8) + 1);
    } else {
        Event_Wait(0x14);
        Call2(Engine_ActorSetAnimationAndWait, 0x16, 4);
        Call1(Engine_EventWait, 0x14);
        temp_r2_4 = Scene58Work;
        FIELD(temp_r2_4, u16 *, 0x1D8) = (u16)(FIELD(temp_r2_4, u16 *, 0x1D8) + 1);
        Value3(Func_02005170, 0x16, 0, 0x14);
    }
    Actor_RunRepeatedMotion(0x16, 2);
    Call1(Engine_EventWait, 0x14);
    Call3(Engine_EventShowMessageAndWait, 0x16, 0, 0x14);
    Call3(Engine_ActorWalkToAndWait, 0x16, 0x148, 0xC8);
    Call2(Engine_ActorRunRepeatedMotion, 0x16, 2);
    Call3(Engine_ActorFaceDirection, 0x16, 0xB000, 0x14);
    temp_r0_12 = Value1(Func_0200511e, 0x16);
    FIELD(temp_r0_12, u8 *, 0x5A) = (u8)(0xFE & FIELD(temp_r0_12, u8 *, 0x5A));
    Call3(Engine_ActorWalkToAndWait, 0x16, 0x150, 0xD0);
    Call1(Engine_EventWait, 1);
    temp_r0_13 = Func_02005140_a(0x16);
    flag2 = 1;
    flag2 |= FIELD(temp_r0_13, u8 *, 0x5A);
    FIELD(temp_r0_13, u8 *, 0x5A) = flag2;
    Call3(Engine_ActorShowEmote, 0x16, 0x102, 0x3C);
    Call3(Engine_ActorShowEmote, 3, 0x00000101, 0x3C);
    Call3(Engine_EventShowMessageAndWait, 3, 0, 0x14);
    Call3(Engine_EventShowMessageAndWait, 0x16, 0, 0x14);
    Call3(Engine_ActorFaceDirection, 0x16, 0x5000, 0x14);
    Call2(Engine_ActorRunRepeatedMotion, 0x16, 2);
    Call1(Engine_EventWait, 0x14);
    Call3(Engine_EventShowMessageAndWait, 0x16, 0, 0x14);
    Call3(Engine_ActorWalkToAndWait, 0x16, 0x150, 0xD8);
    Call2(Func_02005294, (s32)&Value_0000003a, 2);
    do {
        Data_02000240[0x22B] = 3;
    } while (0);
    Call2(Func_0200529e, 0x24, 2);
    Call0(Engine_EventEnd);
}

void RunScene59Sequence(void)
{
    s32 actor9_fixed_y;
    void *actor_one_record;
    void *scene_counter_initial;
    void *scene_counter_initial_alt;
    void *scene_counter_mid_a;
    void *scene_counter_mid_b;
    void *scene_counter_first_a;
    void *scene_counter_first_b;
    void *scene_counter_system_a;
    void *scene_counter_system_b;
    void *scene_counter_later_a;
    void *scene_counter_later_b;
    void *scene_counter_final_a;
    void *scene_counter_final_b;

    Call0(Engine_EventBegin);
    FIELD(Value1(Func_020051ec, 9), s8 *, 0x55) = 0;
    Call3(Engine_ActorSetPosition, 0, 0x01580000, 0xE00000);
    Call3(Engine_ActorSetPosition, 1, 0x01480000, 0xE00000);
    Call3(Engine_ActorSetPosition, 2, 0x01580000, 0xE80000);
    Call3(Engine_ActorSetPosition, 3, 0x01480000, 0xE80000);
    Call3(Engine_ActorFaceDirection, 0, 0xA000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0xC000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0xC000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0xC000, 0);
    Call3(Engine_ActorSetPosition, 0x16, 0x01500000, 0xB00000);
    Call2(Engine_ActorSetAnimation, 0x16, 9);
    Value2(Func_0200521c, Func_02005276(0x16), 0);
    Call4(Engine_CameraMoveTo, 0x01500000, -1, 0xD00000, 0);
    Call1(Engine_TaskWait, 1);
    Call0(Engine_MapRedraw);
    Call1(Engine_TaskWait, 1);
    /* The ROM loads this IWRAM pointer cell before the request store. */
    EVENT_TRANSITION(Data_03001ebc) = 0x100;
    Call0(Engine_EventOpenScreen);
    Call0(Engine_EventWaitForScreen);
    Call1(Engine_EventWait, 0x3C);
    Call2(Engine_ActorRunRepeatedMotion, 1, 2);
    Call1(Engine_EventWait, 0x14);
    Call3(Engine_ActorFaceDirection, 1, 0, 0x14);
    Call1(Func_02005352, 0x000015d4);
    Call3(Engine_EventShowMessageAndWait, 1, 0, 0x14);
    Call3(Engine_ActorFaceDirection, 0, 0x8000, 0x14);
    Call2(Engine_ActorSetAnimationAndWait, 0, 3);
    Call1(Engine_EventWait, 0x14);
    Call2(Engine_ActorRunRepeatedMotion, 0x16, 2);
    Call1(Engine_EventWait, 0x14);
    Call3(Engine_EventShowMessageAndWait, 0x16, 0, 0x14);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xC000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xC000, 0x14);
    Call3(Engine_ActorShowEmote, 2, 0x00000101, 0x3C);
    Call3(Engine_EventShowMessageAndWait, 2, 0, 0x14);
    Call2(Engine_ActorRunRepeatedMotion, 0x16, 2);
    Call1(Engine_EventWait, 0x14);
    Call3(Engine_EventShowMessageAndWait, 0x16, 0, 0x1E);
    Call3(Engine_ActorSetPosition, 0x18, 0x01380000, 0x700000);
    Call1(Engine_AudioPlayCue, 0x120);
    Call3(Engine_EventShowMessageAndWait, 0x18, 0, 0x14);
    Call3(Engine_ActorShowEmote, 3, 0x100, 0x3C);
    Call3(Engine_EventShowMessageAndWait, 3, 0, 0x1E);
    Call1(Engine_AudioPlayCue, 0x1D);
    Call2(Engine_CameraSetSpeed, 0x0000cccc, 0x00001999);
    FIELD(Value0(Func_02005470), s8 *, 0x55) = 0;
    Camera_MoveTo(0x01500000, -1, 0xA80000, 1);
    Call1(Engine_EventWait, 0x14);
    Call3(Engine_ActorSetSpeed, 0x18, 0x0000cccc, 0x00006666);
    Call3(Engine_ActorWalkToAndWait, 0x18, 0x158, 0x88);
    Actor_FaceDirection(0x18, 0x5000, 0x14);
    Call0(Engine_CameraWaitForMove);
    Call3(Engine_EventShowMessageAndWait, 3, 0, 0x1E);
    Call4(Engine_CameraMoveTo, 0x01500000, -1, 0xB80000, 1);
    Call3(Engine_ActorWalkToAndWait, 0x18, 0x158, 0xA0);
    Call3(Engine_ActorWalkToAndWait, 0x18, 0x148, 0xA8);
    Call3(Engine_ActorWalkToAndWait, 0x18, 0x138, 0xB0);
    Call3(Engine_ActorFaceDirection, 0x18, 0x3000, 0x14);
    Call0(Engine_CameraWaitForMove);
    Call2(Engine_ActorSetAnimationAndWait, 0x18, 3);
    Call1(Engine_EventWait, 0x14);
    Call3(Engine_EventShowMessageAndWait, 0x18, 0, 0x14);
    Call3(Engine_ActorFaceDirection, 0, 0x6000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x4000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0x8000, 0x14);
    Call2(Engine_ActorRunRepeatedMotion, 3, 2);
    Call1(Engine_EventWait, 0x14);
    Call3(Engine_EventShowMessageAndWait, 3, 0, 0x14);
    Call2(Engine_ActorSetAnimationAndWait, 0x18, 4);
    Call1(Engine_EventWait, 0x14);
    Call3(Engine_EventShowMessageAndWait, 0x18, 0, 0x14);
    Call3(Engine_ActorFaceDirection, 1, 0x2000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0xA000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0xE000, 0x3C);
    Call3(Engine_ActorFaceDirection, 0x18, 0xB000, 0x14);
    Call3(Engine_ActorFaceDirection, 0, 0xA000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0xC000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0xA000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0xC000, 0x14);
    Call3(Engine_EventShowMessageAndWait, 0x18, 0, 0x14);
    Actor_SetAnimationAndWait(ACTOR_MIA, 4);
    Call1(Engine_EventWait, 0x14);
    Call3(Engine_EventShowMessageAndWait, 3, 0, 0x14);
    Call2(Engine_ActorStartRepeatedMotion, 0x18, 2);
    Call3(Engine_ActorShowEmote, 0x18, 0x100, 0x3C);
    Call3(Engine_ActorFaceDirection, 0x18, 0x3000, 0x14);
    Call3(Engine_EventShowMessageAndWait, 0x18, 0, 0x14);
    Actor_ShowEmote(ACTOR_MIA, 0x00000101, 0x3C);
    Call2(Engine_ActorSetAnimationAndWait, 0x18, 4);
    Call1(Engine_EventWait, 0x14);
    Event_ShowMessageAndWait(0x18, 0, 0x14);
    Call2(Engine_ActorRunRepeatedMotion, 3, 2);
    Event_Wait(0x14);
    Call3(Engine_EventShowMessageAndWait, 3, 0, 0x14);
    Call2(Engine_ActorSetAnimationAndWait, 0x18, 3);
    Call1(Engine_EventWait, 0x14);
    Call3(Engine_EventShowMessageAndWait, 0x18, 0, 0x14);
    Actor_RunRepeatedMotion(ACTOR_MIA, 1);
    Call1(Engine_EventWait, 0x14);
    Call3(Engine_EventShowMessageAndWait, 3, 0, 0x14);
    Call3(Engine_ActorWalkToAndWait, 0x18, 0x138, 0xB8);
    Call3(Engine_ActorFaceDirection, 0x18, 0x3000, 0x14);
    Call3(Engine_EventShowMessageAndWait, 0x18, 0, 0x14);
    Call2(Engine_ActorRunRepeatedMotion, 1, 2);
    Call1(Engine_EventWait, 0xA);
    Call3(Engine_ActorFaceDirection, 1, 0, 0x14);
    Value2(Func_02005674, 1, 0);
    if (Value2(Engine_EventChooseYesNo, 0, 0) == 0) {
        Call2(Engine_ActorRunRepeatedMotion, 1, 2);
        Call3(Engine_EventShowMessageAndWait, 1, 0, 0x14);
        scene_counter_initial = Data_03001ebc;
        EVENT_MESSAGE(scene_counter_initial) = (u16)(EVENT_MESSAGE(scene_counter_initial) + 1);
    } else {
        scene_counter_initial_alt = Data_03001ebc;
        EVENT_MESSAGE(scene_counter_initial_alt) = (u16)(EVENT_MESSAGE(scene_counter_initial_alt) + 1);
        Call2(Engine_ActorRunRepeatedMotion, 1, 1);
        Event_ShowMessageAndWait(ACTOR_GERALD, 0, 0x14);
    }
    Call3(Engine_EventShowMessageAndWait, 2, 0, 0x14);
    Call3(Engine_ActorFaceDirection, 1, 0xC000, 0x14);
    Call2(Engine_ActorSetAnimationAndWait, 0x18, 3);
    Call1(Engine_EventWait, 0x14);
    Call3(Engine_EventShowMessageAndWait, 0x18, 0, 0x14);
    Call3(Engine_ActorFaceDirection, 0x18, 0xD000, 0x14);
    Call3(Engine_EventShowMessageAndWait, 0x18, 0, 0x14);
    Actor_FaceDirection(0x18, 0x3000, 0x14);
    Call2(Engine_ActorSetAnimationAndWait, 0x18, 4);
    Call1(Engine_EventWait, 0x14);
    Call3(Engine_EventShowMessageAndWait, 0x18, 0, 0x14);
    Call3(Engine_ActorShowEmote, 3, 0x00000101, 0x3C);
    Call3(Engine_EventShowMessageAndWait, 3, 0, 0x14);
    Call2(Engine_ActorSetAnimationAndWait, 0x18, 4);
    Event_Wait(0x14);
    Call3(Engine_EventShowMessageAndWait, 0x18, 0, 0x14);
    Call3(Engine_ActorFaceDirection, 0x18, 0xB000, 0x14);
    Event_ShowMessageAndWait(0x18, 0, 0x14);
    Event_Wait(0xA);
    Call2(Engine_ActorRunRepeatedMotion, 0x18, 2);
    Call1(Engine_EventWait, 0x1E);
    Call2(Engine_EventShowMessage, 0x18, 0);
    Call1(Engine_EventWait, 0xA);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Call1(Engine_EventWait, 0x1E);
    Call2(Engine_EventShowMessage, 1, 0);
    Call1(Engine_EventWait, 0xA);
    Call3(Engine_ActorFaceDirection, 0x18, 0x2000, 0);
    Call1(Engine_EventWait, 0x1E);
    Call2(Engine_ActorSetAnimationAndWait, 0x18, 4);
    Event_Wait(0x14);
    Call2(Engine_EventShowMessage, 0x18, 0);
    Call1(Engine_EventWait, 0x14);
    Actor_RunRepeatedMotion(0x18, 2);
    Call1(Engine_EventWait, 0x1E);
    Call2(Engine_EventShowMessage, 0x18, 0);
    Call1(Engine_EventWait, 0xA);
    Call3(Engine_ActorShowEmote, 2, 0x100, 0x3C);
    Call2(Engine_EventShowMessage, 2, 0);
    Event_Wait(0xA);
    Call2(Engine_ActorSetAnimationAndWait, 0x18, 3);
    Event_Wait(0x1E);
    Event_ShowMessage(0x18, 0);
    Call1(Engine_EventWait, 0x14);
    Call2(Engine_ActorRunRepeatedMotion, 0x18, 2);
    Call1(Engine_EventWait, 0x1E);
    Event_ShowMessage(0x18, 0);
    Call1(Engine_EventWait, 0xA);
    Call3(Engine_ActorFaceDirection, 0x18, 0x1000, 0);
    Event_Wait(0x1E);
    Call3(Engine_ActorShowEmote, 0x18, 0x108, 0x3C);
    Call1(Engine_EventWait, 0xA);
    Call2(Engine_ActorRunRepeatedMotion, 0x16, 2);
    Call1(Engine_EventWait, 0x1E);
    Call2(Engine_ActorSetAnimation, 0x16, 8);
    Call1(Engine_EventWait, 0x2D);
    Call2(Engine_ActorSetAnimation, 0x16, 1);
    Value2(Func_020057d2, Func_0200582c(0x16), 1);
    Call1(Engine_EventWait, 0x28);
    Call3(Engine_ActorShowEmote, 2, 0x102, 0x3C);
    Call2(Engine_EventShowMessage, 2, 0);
    Call1(Engine_EventWait, 0xA);
    Actor_FaceDirection(0x16, 0x2000, 0);
    Event_Wait(0x14);
    Call2(Engine_ActorSetAnimationAndWait, 0x16, 4);
    Call1(Engine_EventWait, 0x1E);
    Call2(Engine_EventShowMessage, 0x16, 0);
    Event_Wait(0x14);
    Call3(Engine_ActorFaceDirection, 0x18, 0x2000, 0);
    Call1(Engine_EventWait, 0x1E);
    Call2(Engine_EventShowMessage, 0x18, 0);
    Event_Wait(0xA);
    Call2(Engine_ActorRunRepeatedMotion, 3, 2);
    Call1(Engine_EventWait, 0x1E);
    Call2(Engine_EventShowMessage, 3, 0);
    Call1(Engine_EventWait, 0xA);
    Call3(Func_02005926, 0x18, 0x16, 0);
    Call1(Engine_EventWait, 0x23);
    Actor_SetAnimationAndWait(0x16, 3);
    Call1(Engine_EventWait, 0x1E);
    Actor_SetAnimationAndWait(0x18, 3);
    Call1(Engine_EventWait, 0x14);
    Actor_WalkToAndWait(0x18, 0x148, 0xB0);
    Call3(Engine_ActorFaceDirection, 0x18, 0x3000, 0x14);
    Call2(Engine_ActorSetAnimation, 0x18, 5);
    Call2(Engine_ActorSetAnimation, 0x16, 7);
    Event_Wait(0x14);
    Call2(Engine_ActorRunRepeatedMotion, 1, 1);
    Call1(Engine_EventWait, 0x14);
    Call2(Engine_EventShowMessage, 1, 0);
    Call1(Engine_EventWait, 0xA);
    Call3(Engine_ActorShowEmote, 0x18, 0x00000101, 0x3C);
    Value2(Func_020059c4, 0x18, 0);
    Event_Wait(0x1E);
    Call2(Engine_ActorStartRepeatedMotion, 1, 2);
    Call2(Engine_ActorStartRepeatedMotion, 2, 2);
    Call2(Engine_ActorRunRepeatedMotion, 3, 2);
    Call3(Engine_ActorFaceDirection, 1, 0, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xC000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0xE000, 0);
    Call1(Engine_EventWait, 0x14);
    if (Value2(Engine_EventChooseYesNo, 0, 0) == 0) {
        Call1(Engine_EventWait, 0x1E);
        Call2(Engine_EventShowMessage, 0x18, 0);
        scene_counter_first_a = Data_03001ebc;
        EVENT_MESSAGE(scene_counter_first_a) = (u16)(EVENT_MESSAGE(scene_counter_first_a) + 1);
    } else {
        Call1(Engine_EventWait, 0x1E);
        /* This branch needs its own workspace load; stale register contents
         * are not a valid C dependency. */
        scene_counter_first_b = Data_03001ebc;
        EVENT_MESSAGE(scene_counter_first_b) = (u16)(EVENT_MESSAGE(scene_counter_first_b) + 1);
        Call2(Engine_EventShowMessage, 0x18, 0);
    }
    Call1(Engine_EventWait, 0x14);
    Call2(Engine_ActorSetAnimationAndWait, 0x18, 4);
    Call1(Engine_EventWait, 0x14);
    Call3(Engine_ActorFaceDirection, 1, 0xC000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0xA000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xC000, 0);
    Call1(Engine_EventWait, 0x14);
    Call2(Engine_EventShowMessage, 0x18, 0);
    Call2(Engine_ActorRunRepeatedMotion, 0x16, 1);
    Event_Wait(0x14);
    Call3(Engine_EventShowMessageAndWait, 0x16, 0, 0x14);
    Call0(Func_02005266);
    Call1(Engine_EventWait, 0x14);
    Call2(Engine_ActorStartRepeatedMotion, 0, 2);
    Call2(Engine_ActorStartRepeatedMotion, 1, 2);
    Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
    Call2(Engine_ActorRunRepeatedMotion, 3, 2);
    Call3(Engine_ActorFaceDirection, 0, 0, 5);
    Call3(Engine_ActorFaceDirection, 1, 0xE000, 5);
    Call3(Engine_ActorFaceDirection, 2, 0xA000, 5);
    Call3(Engine_ActorFaceDirection, 3, 0x8000, 5);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xE000, 5);
    Call3(Engine_ActorFaceDirection, 1, 0xA000, 5);
    Call3(Engine_ActorFaceDirection, 2, 0x8000, 5);
    Actor_FaceDirection(ACTOR_MIA, 0, 5);
    FIELD(Value0(Func_02005b94), s8 *, 0x55) = 0;
    Call4(Engine_CameraMoveTo, 0x01180000, -1, 0xE80000, 1);
    Call0(Engine_CameraWaitForMove);
    Call2(Engine_ActorSetChildValue, 0x16, 0xF);
    Actor_SetChildValue(0x18, 0xF);
    Actor_SetPosition(0x16, 0xF00000, 0xD00000);
    Call3(Engine_ActorSetPosition, 0x18, 0xE80000, 0xD00000);
    Call3(Engine_ActorFaceDirection, 0x16, 0x5000, 0);
    Call3(Engine_ActorFaceDirection, 0x18, 0x3000, 0);
    Call0(Func_02005498);
    Call3(Engine_ActorFaceDirection, 0, 0x8000, 0xA);
    Call3(Engine_ActorFaceDirection, 1, 0x8000, 0xF);
    Call3(Engine_ActorFaceDirection, 2, 0x8000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0x8000, 0x14);
    Call3(Engine_ActorWalkTo, 0, 0x148, 0xD0);
    Call3(Engine_ActorWalkTo, 2, 0x150, 0xE0);
    Actor_WalkToAndWait(ACTOR_GERALD, 0x138, 0xD8);
    Call3(Engine_ActorFaceDirection, 1, 0x8000, 0);
    Call1(Engine_ActorWaitForMove, 2);
    Call3(Engine_ActorFaceDirection, 2, 0x8000, 0);
    Call1(Engine_ActorWaitForMove, 0);
    Call3(Engine_ActorFaceDirection, 0, 0x8000, 0x1E);
    Call3(Engine_EventShowMessageAndWait, 1, 0, 0x14);
    Call2(Engine_ActorRunRepeatedMotion, 3, 2);
    Event_Wait(0x14);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 0x14);
    Call3(Engine_ActorFaceDirection, 1, 0x5000, 0x14);
    Call3(Engine_EventShowMessageAndWait, 1, 0, 0x14);
    Call3(Engine_ActorFaceDirection, 1, 0x8000, 0x14);
    Call2(Engine_ActorRunRepeatedMotion, 2, 1);
    Actor_FaceDirection(ACTOR_IVAN, 0x6000, 0x14);
    Call3(Engine_EventShowMessageAndWait, 2, 0, 0x14);
    Actor_SetAnimationAndWait(ACTOR_MIA, 4);
    Call1(Engine_EventWait, 0x14);
    Call3(Engine_EventShowMessageAndWait, 3, 0, 0x14);
    Call3(Engine_ActorFaceDirection, 2, 0x8000, 0x14);
    Call3(Engine_EventShowMessageAndWait, 0x18, 0, 0x14);
    Call2(Engine_CameraSetSpeed, 0x30000, 0x6000);
    {
        void **scene_system_cell;
        Call4(Engine_CameraMoveTo, 0x980000, -1, 0xD80000, 1);
        scene_system_cell = (void **)(Data_03001e70 + 0x164);
        FIELD(scene_system_cell, s32 *, 0xC) = 0x03800000;
        scene_system_cell = (void **)&Data_03001e70;
        Call0(Engine_MapRedraw);
        Call1(Engine_TaskWait, 1);
        FIELD(Value1(Func_02005c7a, 9), s8 *, 0x55) = 0;
        Call3(Engine_ActorSetPosition, 9, 0x680000, 0x01080000);
        actor9_fixed_y = 0xffe00000;
        FIELD(Value1(Func_02005c92, 9), s32 *, 0xC) = actor9_fixed_y;
        FIELD(Func_02005c9c_a(9), s32 *, 0x3C) = actor9_fixed_y;
        Call6(Engine_MapCopyCellsTo, 0x1D, 0x4A, 4, 0x4A, 5, 4);
        Call2(Engine_ActorSetSpritePriority, 0x11, 0);
        Call2(Engine_ActorSetSpritePriority, 0x12, 0);
        Call0(Func_020058f8);
        Call2(Engine_ActorSetSpritePriority, 0x11, 1);
        Call2(Engine_ActorSetSpritePriority, 0x12, 1);
        Call0(Engine_CameraWaitForMove);
        Call1(Engine_EventWait, 0x1E);
        Call3(Engine_ActorFaceDirection, 0x18, 0x8000, 0x14);
        Call3(Engine_EventShowMessageAndWait, 0x18, 0, 0x14);
        Call4(Engine_CameraMoveTo, 0x01180000, -1, 0xD80000, 1);
        Call0(Engine_CameraWaitForMove);
        Call3(Engine_ActorFaceDirection, 0x18, 0x3000, 0x14);
        Call2(Engine_ActorSetAnimation, 0x18, 5);
        Call3(Engine_EventShowMessageAndWait, 0x18, 0, 0x14);
        Call2(Engine_ActorRunRepeatedMotion, 2, 2);
        Call1(Engine_EventWait, 0x14);
        Call3(Engine_ActorFaceDirection, 2, 0xA000, 0x14);
        Call3(Engine_EventShowMessageAndWait, 2, 0, 0x14);
        Call2(Engine_ActorRunRepeatedMotion, 1, 1);
        Call1(Engine_EventWait, 0x14);
        Call3(Engine_EventShowMessageAndWait, 1, 0, 0x14);
        Call3(Engine_ActorShowEmote, 0x18, 0x00000101, 0x3C);
        Value2(Func_02005df6, 0x18, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            Call1(Engine_EventWait, 0x14);
            Call2(Engine_ActorRunRepeatedMotion, 0x16, 2);
            Event_Wait(0x14);
            Call3(Engine_EventShowMessageAndWait, 0x16, 0, 0x14);
            scene_counter_system_a = FIELD(scene_system_cell, void **, 0x4C);
            EVENT_MESSAGE(scene_counter_system_a) = (u16)(EVENT_MESSAGE(scene_counter_system_a) + 1);
        } else {
            scene_counter_system_b = FIELD(scene_system_cell, void **, 0x4C);
            EVENT_MESSAGE(scene_counter_system_b) = (u16)(EVENT_MESSAGE(scene_counter_system_b) + 1);
            Call1(Engine_EventWait, 0x14);
            Call3(Engine_EventShowMessageAndWait, 0x16, 0, 0x14);
        }
    }
    Call2(Engine_ActorRunRepeatedMotion, 0x18, 2);
    Call1(Engine_EventWait, 0x14);
    Call3(Engine_EventShowMessageAndWait, 0x18, 0, 0x14);
    Call3(Engine_ActorShowEmote, 0, 0x00000101, 0);
    Call3(Engine_ActorShowEmote, 1, 0x00000101, 0);
    Call3(Engine_ActorShowEmote, 2, 0x00000101, 0);
    Call3(Engine_ActorShowEmote, 3, 0x00000101, 0x3C);
    Value2(Func_02005ea8, 0x18, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Call1(Engine_EventWait, 0x14);
        Call2(Engine_ActorRunRepeatedMotion, 0x18, 2);
        Event_Wait(0x14);
        Call3(Engine_EventShowMessageAndWait, 0x18, 0, 0x14);
        scene_counter_mid_a = Data_03001ebc;
        EVENT_MESSAGE(scene_counter_mid_a) = (u16)(EVENT_MESSAGE(scene_counter_mid_a) + 1);
    } else {
        scene_counter_mid_b = Data_03001ebc;
        EVENT_MESSAGE(scene_counter_mid_b) = (u16)(EVENT_MESSAGE(scene_counter_mid_b) + 1);
        Call1(Engine_EventWait, 0x14);
        Call2(Engine_ActorRunRepeatedMotion, 0x18, 1);
        Call1(Engine_EventWait, 0x14);
        Event_ShowMessageAndWait(0x18, 0, 0x14);
    }
    Call1(Engine_EventWait, 0x14);
    Call0(Func_020056c4_a);
    Call1(Engine_EventWait, 0x14);
    Call4(Engine_CameraMoveTo, 0x800000, -1, 0xC80000, 1);
    Call0(Engine_CameraWaitForMove);
    Call2(Engine_ActorSetAnimation, 0x16, 1);
    Call2(Engine_ActorSetAnimation, 0x18, 1);
    Call2(Engine_ActorSetChildValue, 0x16, 0xF);
    Call2(Engine_ActorSetChildValue, 0x18, 0xF);
    Call3(Engine_ActorSetPosition, 0x16, 0x780000, 0x980000);
    Call3(Engine_ActorSetPosition, 0x18, 0x700000, 0xA00000);
    Actor_FaceDirection(0x16, 0x5000, 0);
    Call3(Engine_ActorFaceDirection, 0x18, 0x3000, 0);
    Call0(Func_02005880_a);
    Call1(Engine_EventWait, 0x1E);
    Call3(Engine_ActorFaceDirection, 0x18, 0, 0x14);
    Value2(Func_02005fa8, 0x18, 0);
    if (Value2(Engine_EventChooseYesNo, 0, 0) == 0) {
        Call2(Engine_ActorSetAnimationAndWait, 0x18, 3);
        Event_Wait(0x14);
        Call3(Engine_EventShowMessageAndWait, 0x18, 0, 0x14);
        scene_counter_later_a = Data_03001ebc;
        EVENT_MESSAGE(scene_counter_later_a) = (u16)(EVENT_MESSAGE(scene_counter_later_a) + 1);
    } else {
        scene_counter_later_b = Data_03001ebc;
        EVENT_MESSAGE(scene_counter_later_b) = (u16)(EVENT_MESSAGE(scene_counter_later_b) + 1);
        Call2(Engine_ActorSetAnimationAndWait, 0x18, 4);
        Call1(Engine_EventWait, 0x14);
        Call3(Engine_EventShowMessageAndWait, 0x18, 0, 0x14);
    }
    Call3(Engine_EventShowMessageAndWait, 0x18, 0, 0x14);
    Actor_RunRepeatedMotion(0x16, 1);
    Call1(Engine_EventWait, 0x14);
    Call3(Engine_ActorFaceDirection, 0x16, 0, 0x14);
    Call3(Engine_EventShowMessageAndWait, 0x16, 0, 0x14);
    Event_Wait(0x14);
    Call3(Engine_ActorFaceDirection, 0x18, 0, 0x14);
    Call2(Engine_ActorSetSpritePriority, 0x11, 0);
    Call2(Engine_ActorSetSpritePriority, 0x12, 0);
    Call0(Func_02005aee);
    Call2(Engine_ActorSetSpritePriority, 0x11, 1);
    Call2(Engine_ActorSetSpritePriority, 0x12, 1);
    Call1(Engine_AudioPlayCue, 0x11);
    Call4(Engine_CameraMoveTo, 0x01500000, -1, 0xD80000, 1);
    Call0(Engine_CameraWaitForMove);
    Call2(Engine_ActorSetAnimationAndWait, 2, 4);
    Event_Wait(0x14);
    Call0(Func_0200615c);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 0x14);
    Call3(Engine_ActorFaceDirection, 0, 0x4000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0x3000, 0x16);
    Call2(Engine_ActorRunRepeatedMotion, 1, 2);
    Call1(Engine_EventWait, 0xA);
    Call3(Engine_EventShowMessageAndWait, 1, 0, 0x14);
    Call3(Engine_ActorShowEmote, 3, 0x00000105, 0x3C);
    Call3(Engine_EventShowMessageAndWait, 3, 0, 0x14);
    Actor_FaceDirection(ACTOR_MIA, 0xA000, 0x14);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 0x14);
    Call3(Engine_ActorFaceDirection, 3, 0x8000, 0x14);
    Call2(Engine_ActorSetAnimation, 3, 0x10);
    FIELD(Value1(Func_02006092, 3), s32 *, 0x18) = (s32) 0xffff0000;
    Call1(Engine_EventWait, 0x14);
    Call3(Engine_EventShowMessageAndWait, 3, 0, 0x28);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Call1(Engine_EventWait, 0x14);
    Call3(Engine_ActorFaceDirection, 2, 0xA000, 0x14);
    Call2(Engine_ActorSetAnimationAndWait, 2, 4);
    Event_Wait(0x14);
    Value2(Func_02006156, 2, 0);
    Call2(Engine_ActorSetAnimation, 3, 1);
    FIELD(Value1(Func_020060e4, 3), s32 *, 0x18) = 0x10000;
    Call3(Engine_ActorFaceDirection, 0, 0x4000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0xE000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0xC000, 0x14);
    if (Value2(Engine_EventChooseYesNo, 0, 0) == 0) {
        Call2(Engine_ActorSetAnimationAndWait, 2, 3);
        Event_Wait(0x14);
        Call3(Engine_EventShowMessageAndWait, 2, 0, 0x14);
        scene_counter_final_a = Data_03001ebc;
        EVENT_MESSAGE(scene_counter_final_a) = (u16)(EVENT_MESSAGE(scene_counter_final_a) + 1);
    } else {
        scene_counter_final_b = Data_03001ebc;
        EVENT_MESSAGE(scene_counter_final_b) = (u16)(EVENT_MESSAGE(scene_counter_final_b) + 1);
        Call2(Engine_ActorRunRepeatedMotion, 2, 2);
        Call1(Engine_EventWait, 0x14);
        Call3(Engine_EventShowMessageAndWait, 2, 0, 0x14);
    }
    Call3(Engine_ActorFaceDirection, 2, 0x6000, 0x14);
    Call2(Engine_ActorSetAnimationAndWait, 2, 3);
    Call1(Engine_EventWait, 0x14);
    Call3(Engine_EventShowMessageAndWait, 2, 0, 0x14);
    Call3(Engine_ActorFaceDirection, 3, 0xE000, 0x1E);
    Call3(Engine_ActorFaceDirection, 1, 0x2000, 0x14);
    Call2(Engine_ActorRunRepeatedMotion, 1, 1);
    Call1(Engine_EventWait, 0xA);
    Call3(Engine_EventShowMessageAndWait, 1, 0, 0x14);
    Call3(Engine_ActorFaceDirection, 3, 0xA000, 0x14);
    Call2(Engine_ActorSetAnimationAndWait, 3, 3);
    Call1(Engine_EventWait, 0x14);
    Call3(Engine_EventShowMessageAndWait, 3, 0, 0x14);
    Call2(Engine_ActorRunRepeatedMotion, 3, 1);
    Call3(Engine_EventShowMessageAndWait, 3, 0, 0x14);
    Call3(Engine_ActorShowEmote, 0, 0x00000101, 0);
    Call3(Engine_ActorShowEmote, 1, 0x00000101, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x00000101, 0x3C);
    Call3(Engine_ActorSetSpeed, 3, 0x0000cccc, 0x00006666);
    Call3(Engine_ActorSetSpeed, 1, 0x0000cccc, 0x00006666);
    Call3(Engine_ActorWalkToAndWait, 3, 0x148, 0xD8);
    Call3(Engine_ActorFaceDirection, 2, 0xA000, 0);
    actor_one_record = Value1(Func_02006258, 1);
    FIELD(actor_one_record, u8 *, 0x5A) = (u8)(0xFE & FIELD(actor_one_record, u8 *, 0x5A));
    Call3(Engine_ActorWalkTo, 1, 0x138, 0xC8);
    Call3(Engine_ActorWalkTo, 3, 0x118, 0xD8);
    Call1(Engine_ActorWaitForMove, 1);
    Call3(Engine_ActorFaceDirection, 0, 0x6000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0x6000, 0);
    Actor_WaitForMove(ACTOR_MIA);
    Call2(Engine_ActorRunRepeatedMotion, 3, 2);
    Call1(Engine_EventWait, 0x1E);
    Call3(Engine_ActorFaceDirection, 3, 0, 0x14);
    Call3(Engine_EventShowMessageAndWait, 3, 0, 0x14);
    Call2(Engine_ActorRunRepeatedMotion, 1, 1);
    Call1(Engine_EventWait, 0x14);
    Call3(Engine_EventShowMessageAndWait, 1, 0, 0x14);
    Call2(Engine_ActorSetAnimationAndWait, 3, 3);
    Call2(Engine_ActorStartRepeatedMotion, 0, 2);
    Call2(Engine_ActorStartRepeatedMotion, 2, 2);
    Call2(Engine_ActorStartRepeatedMotion, 1, 2);
    Call2(Engine_ActorSetAttachedEffect, 0, 0x102);
    Call2(Engine_ActorSetAttachedEffect, 1, 0x102);
    Actor_SetAttachedEffect(ACTOR_IVAN, 0x102);
    Call1(Engine_EventWait, 0x3C);
    Call3(Engine_EventShowMessageAndWait, 2, 0, 0x14);
    Call2(Engine_ActorSetAnimationAndWait, 3, 4);
    Call1(Engine_EventWait, 0x14);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 0x14);
    Call3(Engine_ActorFaceDirection, 3, 0xE000, 0x14);
    Call3(Engine_EventShowMessageAndWait, 3, 0, 0x14);
    Call3(Engine_ActorFaceDirection, 3, 0, 0x14);
    Call2(Engine_ActorSetAnimationAndWait, 3, 4);
    Call1(Engine_EventWait, 0x14);
    Call3(Engine_EventShowMessageAndWait, 3, 0, 0x1E);
    Call3(Engine_ActorFaceDirection, 1, 0x2000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0xA000, 0x14);
    Call2(Engine_ActorSetAnimation, 0, 3);
    Call2(Engine_ActorSetAnimation, 1, 3);
    Call2(Engine_ActorSetAnimationAndWait, 2, 3);
    Call1(Engine_EventWait, 0x1E);
    Call3(Engine_ActorFaceDirection, 0, 0x6000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0x6000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x6000, 0x28);
    Call2(Engine_ActorSetAnimation, 0, 3);
    Call2(Engine_ActorSetAnimation, 1, 3);
    Call2(Engine_ActorSetAnimationAndWait, 2, 3);
    Event_Wait(0x14);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Call1(Engine_EventWait, 0x14);
    Call3(Engine_EventShowMessageAndWait, 3, 0, 0x14);
    Call3(Engine_ActorWalkTo, 0, 0x138, 0xD8);
    Actor_WalkTo(ACTOR_GERALD, 0x138, 0xD8);
    Call3(Engine_ActorWalkTo, 2, 0x138, 0xD8);
    Call3(Engine_ActorWalkTo, 3, 0x138, 0xD8);
    Call1(Engine_ActorWaitForMove, 0);
    Call1(Engine_ActorWaitForMove, 1);
    Call3(Engine_ActorSetPosition, 1, 0, 0);
    Call1(Engine_ActorWaitForMove, 2);
    Call3(Engine_ActorSetPosition, 2, 0, 0);
    Call1(Engine_ActorWaitForMove, 3);
    Call3(Engine_ActorSetPosition, 3, 0, 0);
    {
        void **record;
        record = (void **)(Data_03001e70 + 0x164);
        FIELD(record, s32 *, 0xC) = 0x04000000;
    }
    Call0(Engine_MapRedraw);
    Task_Wait(1);
    Call6(Engine_MapCopyCellsTo, 4, 0x46, 4, 0x4A, 5, 4);
    GameFlag_Set(0x880);
    GameFlag_Set(0x00000881);
    Call0(Engine_EventEnd);
}

void FieldScene_RunScene39d_02002ddc(void)
{
    s32 record;
    u8 *work;

    work = Data_03001e70 + 0x164;
    Event_Begin();
    *(s32 *)(work + 12) = 0x3800000;
    Map_Redraw();
    Task_Wait(1);
    *(u8 *)(Func_020064f2(9) + 85) = 0;
    Actor_SetPosition(9, 0x680000, 0x1080000);
    record = Func_0200650c(9);
    *(s32 *)(record + 12) = -0x200000;
    record = Value1(Func_02006516, 9);
    *(s32 *)(record + 60) = -0x200000;
    *(u8 *)(Func_020065ec() + 85) = 0;
    Camera_SetSpeed(0xcccc, 0x1999);
    Camera_MoveTo(0x800000, -1, 0xb80000, 1);
    Camera_WaitForMove();
    Event_Wait(30);
    Map_CopyCellsTo(29, 74, 4, 74, 5, 4);
    Actor_SetSpritePriority(17, 0);
    Actor_SetSpritePriority(18, 0);
    Func_0200619e();
    Actor_SetSpritePriority(17, 1);
    Actor_SetSpritePriority(18, 1);
    Event_Wait(20);
    GameFlag_Set(0x251);
    Event_End();
}

void FieldScene_RunScene39d_02002eb8(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 104, 152);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 60);
    Actor_SetSpritePriority(17, 0);
    Actor_SetSpritePriority(18, 0);
    Func_020060fc();
    Camera_MoveTo(-1, -1, -1, 0);
    Event_RequestExit(1);
    Event_End();
}

void FieldScene_ConfigureValue93Scene(void)
{
    Psynergy_Begin(93, 1);
    Psynergy_SetTarget(24, 9);
    Psynergy_RaiseHands();
    Psynergy_PlayEffect(1);
    Psynergy_LowerHands();
    Func_02006c82();
}

void Func_02003460(struct OverlayObject *object)
{
    struct OverlayObject *parent;
    u16 angle;

    parent = object->linked_object;
    angle = object->angle_64;
    object->coordinate_08 = parent->coordinate_08 + Math_Cos(angle) * (object->field_30 + 28);
    object->coordinate_10 = (Math_Sin(angle) << 4) + 0x900000;
    object->coordinate_38 = object->coordinate_08;
    object->coordinate_40 = object->coordinate_10;
    object->angle_64 -= 0x200;
}
