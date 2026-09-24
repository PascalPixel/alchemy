#include "TYPES.H"
#include "FIELD_EVENT.H"

#define GetStagedActorEffect Func_0200374e
#define CanStartStagedActorEffect Func_0200371e
#define SceneTransition_Phase Data_02000240[225]

#include "STAGED_ACTOR.H"
#include "STAGED_ACTOR_EFFECT.H"

typedef struct { u8 filler0[0x28]; s16 *unk28; } Sub;

struct PlacementResult_02000a68 {
    s32 words[6];
};

struct Struct3848 {
    u8 pad00[8];
    u32 field08;
    s32 field0c;
    u32 field10;
};

struct Struct2798 {
    u8 pad00[0x18];
    s32 field18;
    u8 pad1c[0x38 - 0x1c];
    s32 field38;
    s32 field3c;
    s32 field40;
};

struct Sub { u8 pad00[9]; u8 f09; u8 pad0a[28]; u8 f26; };

struct Obj {
    u8 pad00[0x18];
    s32 f18;
    u8 pad1c[7];
    u8 f23;
    u8 pad24[12];
    s32 f30;
    s32 f34;
    u8 pad38[24];
    struct Sub *f50;
    u8 pad54[1];
    u8 f55;
};

struct Struct288c {
    u8 pad00[8];
    s32 field08;
    u8 pad0c[4];
    s32 field10;
};

struct Struct5702 {
    u8 pad00[0x0c];
    s32 field0c;
    u8 pad10[0x55 - 0x10];
    u8 field55;
};

extern s32 Data_0200b398;
extern u8 Data_0200b2d0[];
extern s32 Data_0200b394;
extern s32 Data_0200b390;
extern u8 Data_0200b2e4[];

u32 Func_0200359e(void);
s32 Func_02000eea(struct PlacementResult_02000a68 *res);
void Func_02001096(struct PlacementResult_02000a68 res);
u8 *Func_020036ee(s32);
s32 Func_02000d3c_grid(s32, s32, s32, s32, s32, s32);
u8 *Func_0200372a(s32);
struct StagedActorEffect *Func_0200374e(s32 actor_index);
struct Struct3848 *Func_02003848(s32 arg0);
void Func_0200176a(u32 *arg0);
s32 Func_02000ef4();
void Func_02001530();
void Func_02001582();
void Func_02001588();
void Func_02001a22();
s32 Func_020038a6();
s32 Func_020038da();
struct Obj *Func_020053a6(s32, s32, s32, s32);
void Func_0200540e(struct Obj *, s32, s32, s32);
s32 Func_020054ca(struct Struct288c *arg0, u8 *arg1);
struct Struct5702 *Func_02005702(s32 arg0);
struct Struct5702 *Func_0200572a(s32 arg0);
struct Struct5702 *Func_02005752(s32 arg0);
struct Struct5702 *Func_0200577a(s32 arg0);

/*
 * Distance between two three-component 16.16 fixed-point positions.  Each
 * argument walks three consecutive words in x, y, z order; the per-axis deltas
 * are shifted down to integers, squared, summed, and passed to the resident
 * IWRAM integer square root.  The walking-pointer form is what reproduces the
 * reference and must not become struct field access.
 */

/* Clear the pending object cell after restoring its mode. */

/*
 * The clamp store takes its zero from a local.  Written as a literal it is
 * materialised from the literal pool, which changes the store's source
 * register and costs a pool word.  The owner ends at 0x02000a4a; the veneer
 * that follows is not part of it.
 */

/* Placement query followed by the tile-(10,20) scene transition. */

/* Return this overlay's state block. */

/*
 * Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds, not a runtime address.
 */

/*
 * These wrappers pass their constants straight into the argument registers.
 * A direct call precomputes a costly constant into a pseudo that is then
 * shared with later uses in the block.  A value-returning call also sets r0
 * last of its arguments.
 */

/* The scene-transition phase flag in the field-scene table. */
static __inline__ void DrawPlacement_02000a68(
    s32 left, s32 top, s32 width, s32 height, s32 tile, s32 palette)
{
    Map_CopyCellAttributes(left, top, width, height, tile, palette);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

s32 Func_0200371e(struct StagedActorEffect *actor,
                         struct StagedActorEffectRequest *request);

s32 OverlayObject_ClearPendingAndRestoreMode(u8 *object)
{
    u8 **pending = (u8 **)0x0200b390;
    if (*pending) {
        Object_SetAnimation(object, 2);
        *pending = 0;
    }
    return 1;
}

s32 SceneActor_UpdateRandomCounterMode(u8 *object)
{
    u16 *counter = (u16 *)(object + 100);
    *counter = (u16)(*counter + ((Func_0200359e() * 100) >> 16));
    if ((s16)*counter > 1000) Object_SetPalette(object, 7);
    else Object_SetPalette(object, 10);
    if (*(s16 *)counter > 1200) { u16 z = 0; *counter = z; }
    return 1;
}

u8 *SceneData_GetTableb06c(void) { return (u8 *)0x0200b06c; }

s32 SceneData_ReturnZero(void) { return 0; }

u8 *SceneData_GetTableb0cc(void) { return (u8 *)0x0200b0cc; }

u8 *SceneData_GetTableb0e4(void) { return (u8 *)0x0200b0e4; }

void FieldScene_RunTile10x20Transition(void)
{
    struct PlacementResult_02000a68 res;
    Event_Begin();

    if (Func_02000eea(&res)) {
        Func_02001096(res);
        if (res.words[1] == 10 && (res.words[2] >> 20) == 20) {
            u8 *actor;
            s32 zero;

            Actor_SetAnimation(10, 3);
            Actor_SetDestinationOffset(10, -18, 6);
            Event_Wait(30);
            Audio_PlayCue(240);
            Actor_SetAnimation(10, 8);
            Func_020036ee(10)[35] = 2;
            zero = 0;
            DrawPlacement_02000a68(0, 17, 2, 4, 19, 17);
            Func_02000d3c_grid(2, 20, 17, 1, 4, zero);
            GameFlag_Set(0x200);
            actor = Func_0200372a(10);
            Actor_SetSpriteFlags(actor, 0);
        }
    }

    Event_End();
}

s32 StagedActor_RunStepEffect(struct StagedActorEffectRequest *request)
{
    struct StagedActorEffect *actor = GetStagedActorEffect(0);
    u8 *flags = &actor->motion_flags;
    u8 saved = *flags;
    s32 ret = CanStartStagedActorEffect(actor, request);

    if (ret == 0) {
        Event_Begin();
        Object_SetAnimation(actor, 6);
        Task_Wait(6);
        Audio_PlayCue(152);
        Object_SetAnimation(actor, 7);
        actor->move_rate_x = 0x30000;
        actor->move_rate_z = 0x20000;
        actor->elevation_rate = 0x40000;
        *flags &= 0x7e;
        Actor_SetSpriteFlags(actor, 0);
        Actor_MoveToAndWait(0, request->cell_x, request->cell_z);
        Object_SetAnimation(actor, 6);
        Actor_SetSpriteFlags(actor, 1);
        *flags = (u8)ret;
        Actor_SetAnimation(10, 7);
        actor->position_x += 0xffff0000;
        actor->position_z += 0xffff0000;
        Task_Wait(2);
        actor->position_x += 0xffff0000;
        actor->position_z += 0xffff0000;
        Task_Wait(10);
        actor->position_x += 0x10000;
        actor->position_z += 0x10000;
        Task_Wait(4);
        actor->position_x += 0x10000;
        actor->position_z += 0x10000;
        *flags = saved;
        Event_End();
        return 1;
    }
    return 0;
}

void SceneActor_PassSubjectOffsetPosition(void)
{
    extern u8 Data_02000240[];

    u32 buf[3];
    s32 off = 500;
    struct Struct3848 *p = Func_02003848(*(s32 *)(Data_02000240 + off));
    u32 base = p->field08 & 0xfff00000;

    buf[0] = base + 0x80000;
    buf[1] = p->field0c;
    buf[2] = (p->field10 & 0xfff00000) + 0x80000;
    buf[0] = base + 0x280000;
    Func_0200176a(buf);
}

u8 *SceneData_GetTableB294(void) { return (u8 *)0x0200b294; }

s32 FieldScene_RunScene391_02000c68(void)
{
    extern s16 Data_02000240[];

    s32 record;
    s32 zero;

    Func_02001530(10);
    if (GameFlag_IsSet(0x200) != 0) {
        zero = 0;
        *(u8 *)(Func_020038a6(10) + 35) = 2;
        Map_CopyCellAttributes(0, 17, 2, 4, 19, 17);
        record = Func_02000ef4(2, 20, 17, 1, 4, zero);
        record = Func_020038da(10);
        Actor_SetSpriteFlags(record, 0);
    }
    Func_02001582(8);
    Func_02001588(9);
    if (SceneTransition_Phase == 4) {
        if (GameFlag_IsSet(0x843) == 0) {
            Func_02001a22();
        }
    }
    if (GameFlag_IsSet(0x845) != 0) {
        Actor_SetPosition(17, 0, 0);
        Actor_SetPosition(18, 0, 0);
        Actor_SetPosition(19, 0, 0);
        Actor_SetPosition(20, 0, 0);
        Actor_SetPosition(21, 0, 0);
    }
    return 0;
}

#include "TYPES.H"

u8 * Func_02002c24();
void Func_02002c3c();
void Func_02002780();
void Func_02002768();
s32 Func_02002b8c();
s32 Func_02002b94();
void Func_02002c4c();
void Func_02002d14();

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

static __inline__ u8 * Record1(u8 * (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

extern s32 Data_0200b384;
extern s32 Data_0200b388;
extern s32 Data_0200b38c;
extern s32 Data_0200b390;
extern s32 Data_0200b394;
extern s32 Data_0200b398;
extern u8 Data_0200a7c9[];
extern u8 Data_0200a975[];
extern u8 Data_0200aad9[];
extern u8 Data_0200ae20[];
extern u8 Data_0200ae54[];
extern u8 Data_0200ae88[];
extern u8 Data_0200aebc[];
extern u8 Data_0200af48[];
extern u8 Data_0200af6c[];
extern u8 Data_0200afc8[];
extern u8 Data_0200b024[];

void FieldScene_RunBranchingFormationPresentation(void)
{
    u8 *record;
    s32 *flag_work;
    s32 entry_action;
    s32 value;
    s32 flag;
    s32 motion_action;
    s32 *party_flag;
    s32 *formation_flag;
    s32 *sequence_flag;
    s32 *finish_flag;
    s32 reset_action;
    s32 *effect_phase;
    s32 *formation_phase;
    s32 formation_action;
    s32 finish_action;
    s32 *sequence_phase;

    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Task_Wait(1);
    Camera_MoveTo(0xf60000, -1, 0x25c0000, 0);
    flag_work = &Data_0200b394;
    flag = GameFlag_IsSet(3);
    *flag_work = flag;
    record = Func_02002c24(13);
    Actor_SetSpriteFlags((s32)record, 0);
    record = Func_02002c24(14);
    Actor_SetSpriteFlags((s32)record, 0);
    record = Func_02002c24(15);
    Actor_SetSpriteFlags((s32)record, 0);
    record = Func_02002c24(16);
    Actor_SetSpriteFlags((s32)record, 0);
    record = Func_02002c24(17);
    Actor_SetSpriteFlags((s32)record, 0);
    record = Func_02002c24(18);
    Actor_SetSpriteFlags((s32)record, 0);
    record = Func_02002c24(19);
    Actor_SetSpriteFlags((s32)record, 0);
    record = Func_02002c24(20);
    Actor_SetSpriteFlags((s32)record, 0);
    record = Func_02002c24(21);
    Actor_SetSpriteFlags((s32)record, 0);
    entry_action = (s32)Data_0200b024;
    Actor_EnableActionCallback(17, entry_action);
    Actor_EnableActionCallback(18, entry_action);
    Actor_EnableActionCallback(19, entry_action);
    Actor_EnableActionCallback(20, entry_action);
    Actor_EnableActionCallback(21, entry_action);
    Actor_SetPosition(0, 0x740000, 0x25a0000);
    Task_Wait(1);
    Map_Redraw();
    Task_Wait(1);
    Event_OpenScreen();
    Event_WaitForScreen();
    Actor_SetSpeed(0, 0xcccc, 0x6666);
    Actor_WalkToAndWait(0, 254, 0x251);
    Actor_SetSpeed(1, 0x9999, 0x4ccc);
    Actor_SetSpeed(2, 0x9999, 0x4ccc);
    {
        u8 *record = Record1(Func_02002c24, 0);

        if (record != 0) {
            Actor_SetPosition(1, *(s32 *)((s32)record + 8), *(s32 *)((s32)record + 16));
        }
    }
    {
        u8 *record = Record1(Func_02002c24, 0);

        if (record != 0) {
            Actor_SetPosition(2, *(s32 *)((s32)record + 8), *(s32 *)((s32)record + 16));
        }
    }
    Actor_EnableActionCallback(1, (s32)Data_0200ae20);
    Actor_EnableActionCallback(2, (s32)Data_0200ae54);
    if (*flag_work != 0) {
        Actor_SetSpeed(3, 0x9999, 0x4ccc);
        {
            u8 *record = Record1(Func_02002c24, 0);

            if (record != 0) {
                Actor_SetPosition(3, *(s32 *)((s32)record + 8), *(s32 *)((s32)record + 16));
            }
        }
        Actor_EnableActionCallback(3, (s32)Data_0200ae88);
    }
    Func_02002c3c(2);
    Func_02002780(2, 0x2000, 40);
    Func_02002780(2, 0x8000, 20);
    Func_02002780(2, 0x4000, 40);
    Actor_ShowEmote(2, 0x101, 0);
    Event_Wait(60);
    Actor_FaceDirection(1, 0x4000, 0);
    Func_02002780(0, 0x6000, 60);
    value = 160;
    Func_02002780(3, 0x2000, 10);
    Actor_FaceDirection(1, 0x2000, 0);
    Func_02002780(0, (value << 8), 10);
    Actor_ShowEmote(1, 0x101, 0);
    Actor_ShowEmote(0, 0x101, 0);
    Event_Wait(40);
    Actor_FaceDirection(1, 0x4000, 0);
    Func_02002780(0, 0x6000, 10);
    Actor_RunRepeatedMotion(1, 2);
    Event_SetMessage(0x1474);
    Func_02002768(1, 10);
    Event_SetMessage(0x147c);
    Func_02002780(2, 0xc000, 20);
    Actor_SetAnimationAndWait(2, 3);
    Func_02002768(2, 20);
    Func_02002780(1, 0, 20);
    Func_02002780(0, (value << 8), 40);
    Func_02002780(1, 0x4000, 20);
    Func_02002780(0, 0x6000, 30);
    Func_02002780(1, 0x6000, 20);
    Func_02002780(0, 0xe000, 30);
    Actor_RunRepeatedMotion(2, 2);
    Actor_ShowEmote(2, 0x100, 0);
    Event_Wait(40);
    Actor_FaceDirection(1, 0x4000, 0);
    Func_02002780(0, 0x6000, 20);
    Func_02002780(2, 0xc000, 10);
    Audio_PlayCue(17);
    Audio_PlayCue(206);
    ColorBuffer_ApplyTarget(0x7fff, 0);
    ColorBuffer_Interpolate(1);
    Task_Wait(1);
    Data_0200b398 = 1;
    Value2(Func_02002b8c, (s32)Data_0200a7c9, 0xc80);
    Task_Wait(20);
    ColorBuffer_ApplyTarget(0x405210, 1);
    ColorBuffer_ApplyTarget(0x10000, 2);
    ColorBuffer_Interpolate(120);
    Task_Wait(60);
    motion_action = (s32)Data_0200aebc;
    Actor_EnableActionCallback(0, motion_action);
    Actor_EnableActionCallback(1, motion_action);
    Actor_EnableActionCallback(2, motion_action);
    Actor_EnableActionCallback(3, motion_action);
    Event_Wait(100);
    Func_02002768(1, 20);
    Func_02002768(2, 40);
    if (Data_0200b394 != 0) {
        Event_Wait(40);
        Actor_ShowEmote(3, 0x102, 0);
        Event_Wait(40);
        Func_02002768(3, 40);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Event_Wait(20);
    party_flag = &Data_0200b394;
    if (*party_flag != 0) {
        value = 128;
        record = Func_02002c24(3);
        *(s32 *)((s32)record + 40) = (value << 10);
        Event_Wait(10);
        Actor_SetSpeed(3, (value << 10), (value << 10));
        Actor_SetDestinationOffset(3, -2, 0);
        Actor_EnableActionCallback(3, (s32)Data_0200af48);
        record = Func_02002c24(3);
        Actor_SetSpriteFlags((s32)record, 0);
        Actor_SetAnimation(3, 19);
        Event_Wait(10);
    }
    value = 128;
    record = Func_02002c24(0);
    *(s32 *)((s32)record + 40) = (value << 10);
    Event_Wait(10);
    Actor_SetSpeed(0, (value << 10), (value << 10));
    reset_action = (s32)Data_0200af48;
    Actor_EnableActionCallback(0, reset_action);
    record = Func_02002c24(0);
    Actor_SetSpriteFlags((s32)record, 0);
    Actor_SetAnimation(0, 19);
    Event_Wait(20);
    record = Record1(Func_02002c24, 1);
    *(s32 *)((s32)record + 40) = (value << 10);
    Event_Wait(10);
    Actor_SetSpeed(1, (value << 10), (value << 10));
    Actor_EnableActionCallback(1, reset_action);
    record = Func_02002c24(1);
    Actor_SetSpriteFlags((s32)record, 0);
    Actor_SetAnimation(1, 19);
    Event_Wait(40);
    record = Record1(Func_02002c24, 2);
    *(s32 *)((s32)record + 40) = (value << 10);
    Event_Wait(10);
    Actor_EnableActionCallback(2, reset_action);
    record = Func_02002c24(2);
    Actor_SetSpriteFlags((s32)record, 0);
    Actor_SetAnimation(2, 19);
    Data_0200b398 = 0;
    Event_Wait(160);
    Value1(Func_02002b94, (s32)Data_0200a7c9);
    Event_Wait(120);
    ColorBuffer_ApplyTarget(0x406218, 1);
    ColorBuffer_Interpolate(60);
    Task_Wait(60);
    Data_0200b388 = 0;
    effect_phase = &Data_0200b38c;
    Data_0200b384 = 0x800000;
    *effect_phase = 1;
    Value2(Func_02002b8c, (s32)Data_0200a975, 0xc80);
    Event_Wait(180);
    Audio_PlayCue(21);
    Func_02002768(1, 80);
    Func_02002768(2, 40);
    Actor_ShowEmote(0, 0x102, 0);
    Actor_ShowEmote(1, 0x102, 0);
    Actor_ShowEmote(2, 0x102, 0);
    Actor_ShowEmote(3, 0x102, 0);
    Event_Wait(60);
    Func_02002768(2, 20);
    *effect_phase = 2;
    Actor_StartRepeatedMotion(2, 2);
    Event_Wait(20);
    Actor_StartRepeatedMotion(1, 1);
    Event_Wait(40);
    Actor_StartRepeatedMotion(0, 2);
    Actor_StartRepeatedMotion(3, 1);
    Event_Wait(20);
    Actor_StartRepeatedMotion(2, 3);
    Event_Wait(40);
    Actor_StartRepeatedMotion(0, 1);
    Event_Wait(20);
    Actor_StartRepeatedMotion(1, 2);
    Event_Wait(20);
    Actor_StartRepeatedMotion(3, 2);
    Actor_ShowEmote(1, 0x102, 0);
    Func_02002768(1, 20);
    if (*party_flag != 0) {
        Actor_ShowEmote(3, 0x102, 0);
        Func_02002768(3, 10);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    formation_phase = &Data_0200b38c;
    *formation_phase = 3;
    *(u8 *)(Func_02002c24(0) + 35) &= 254;
    *(u8 *)(Func_02002c24(1) + 35) &= 254;
    *(u8 *)(Func_02002c24(2) + 35) &= 254;
    *(u8 *)(Func_02002c24(3) + 35) &= 254;
    Actor_SetSpritePriority(0, 3);
    Actor_SetSpritePriority(1, 3);
    Actor_SetSpritePriority(2, 3);
    value = 0;
    Actor_SetSpritePriority(3, 3);
    Data_0200b390 = value;
    Value2(Func_02002b8c, (s32)Data_0200aad9, 0xc80);
    Audio_PlayCue(220);
    *(u8 *)(Func_02002c24(13) + 35) &= 254;
    Actor_SetSpritePriority(13, 2);
    Actor_SetPosition(13, 0xfd0000, 0x25b0000);
    formation_action = (s32)Data_0200af6c;
    Actor_EnableActionCallback(13, formation_action);
    *(u8 *)(Func_02002c24(14) + 35) &= 254;
    Actor_SetSpritePriority(14, 2);
    Actor_SetPosition(14, 0xe90000, 0x2750000);
    Actor_EnableActionCallback(14, formation_action);
    if (Data_0200b394 != 0) {
        *(u8 *)(Func_02002c24(15) + 35) &= 254;
        Actor_SetSpritePriority(15, 2);
        Actor_SetPosition(15, 0xcf0000, 0x2610000);
        Actor_EnableActionCallback(15, formation_action);
    }
    *(u8 *)(Func_02002c24(16) + 35) &= 254;
    Actor_SetSpritePriority(16, 2);
    Actor_SetPosition(16, 0xe30000, 0x2440000);
    Actor_EnableActionCallback(16, formation_action);
    if (*formation_phase != 0) {
        do {
            Task_Wait(1);
        } while (Data_0200b38c != 0);
    }
    Event_Wait(0x12c);
    Value1(Func_02002b94, (s32)Data_0200a975);
    Event_Wait(120);
    Audio_PlayCue(17);
    ColorBuffer_ApplyTarget(0x10000, 1);
    ColorBuffer_Interpolate(60);
    Task_Wait(60);
    Actor_Stop(13);
    Actor_Stop(14);
    formation_flag = &Data_0200b394;
    if (*formation_flag != 0) {
        Actor_Stop(15);
    }
    Actor_Stop(16);
    Task_Wait(1);
    finish_action = (s32)Data_0200afc8;
    Actor_EnableActionCallback(13, finish_action);
    Actor_EnableActionCallback(14, finish_action);
    if (*formation_flag != 0) {
        Actor_EnableActionCallback(15, finish_action);
    }
    Func_02002c4c(16, finish_action);
    Event_Wait(80);
    Actor_RunRepeatedMotion(1, 2);
    Event_Wait(40);
    Event_OpenMessage(1, 0);
    Actor_SetPosition(11, 0xdc0000, 0x1ee0000);
    Actor_SetPosition(12, 0xdc0000, 0x1ee0000);
    Task_Wait(1);
    if (Event_ChooseYesNo(11, 0) == 1) {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Actor_RunRepeatedMotion(0, 1);
    Event_Wait(20);
    Actor_RunRepeatedMotion(2, 2);
    Func_02002768(2, 20);
    if (*formation_flag != 0) {
        Actor_RunRepeatedMotion(3, 2);
        Event_Wait(10);
        Event_SetMessage(0x1488);
        Func_02002768(3, 40);
    }
    Actor_StartRepeatedMotion(1, 1);
    Actor_ShowEmote(1, 0x101, 0);
    Event_Wait(80);
    Actor_RunRepeatedMotion(2, 2);
    Event_SetMessage(0x1489);
    Func_02002768(2, 40);
    Actor_RunRepeatedMotion(1, 3);
    Event_Wait(40);
    Actor_SetSpritePriority(1, 2);
    *(u8 *)(Func_02002c24(1) + 35) |= 1;
    record = Func_02002c24(1);
    Actor_SetSpriteFlags((s32)record, 1);
    Actor_Jump(1, 6, 0);
    Actor_SetDestinationOffset(1, -3, 0);
    Actor_SetAnimation(1, 1);
    Func_02002780(1, 0x4000, 60);
    Func_02002768(1, 20);
    Actor_StartRepeatedMotion(1, 2);
    Func_02002768(1, 10);
    Actor_RunRepeatedMotion(0, 3);
    Func_02002780(1, 0x2000, 20);
    Actor_ShowEmote(1, 0x101, 0);
    Event_Wait(40);
    Func_02002780(1, 0x6000, 40);
    Func_02002780(1, 0x2000, 20);
    Func_02002780(1, 0x6000, 20);
    Func_02002780(1, 0x2000, 10);
    Actor_Jump(1, 2, 0);
    Event_Wait(40);
    Actor_Jump(1, 2, 0);
    Event_Wait(10);
    Actor_Jump(1, 4, 0);
    Event_Wait(20);
    Func_02002768(1, 20);
    if (*formation_flag != 0) {
        Actor_ShowEmote(3, 0x100, 0);
        Event_Wait(60);
        Actor_RunRepeatedMotion(3, 2);
        Event_Wait(80);
        Actor_SetSpritePriority(3, 2);
        *(u8 *)(Func_02002c24(3) + 35) |= 1;
        record = Func_02002c24(3);
        Actor_SetSpriteFlags((s32)record, 1);
        Actor_Jump(3, 4, 0);
        Actor_SetDestinationOffset(3, -2, 0);
        Actor_SetAnimation(3, 1);
        Func_02002780(3, 0xe000, 60);
        Actor_RunRepeatedMotion(3, 2);
        Event_Wait(20);
        Func_02002768(3, 20);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Actor_Jump(1, 2, 0);
    Func_02002780(1, 0x4000, 20);
    Actor_SetAnimationAndWait(1, 3);
    Func_02002780(1, 0x2000, 10);
    Func_02002768(1, 20);
    Actor_SetAnimationAndWait(1, 3);
    Event_Wait(10);
    Actor_RunRepeatedMotion(2, 1);
    Event_Wait(40);
    Actor_RunRepeatedMotion(2, 2);
    Event_Wait(20);
    value = 1;
    Actor_SetSpritePriority(2, 2);
    *(u8 *)(Func_02002c24(2) + 35) |= value;
    record = Func_02002c24(2);
    Actor_SetSpriteFlags((s32)record, 1);
    Actor_Jump(2, 4, 0);
    Actor_SetAnimation(2, 1);
    Actor_FaceDirection(2, 0xc000, 0);
    Actor_RunRepeatedMotion(0, 2);
    Event_Wait(10);
    Actor_SetSpritePriority(0, 2);
    {
        u8 *record = Func_02002c24(0);
        u8 flags = (u8)(value | record[35]);

        record[35] = flags;
    }
    record = Func_02002c24(0);
    Actor_SetSpriteFlags((s32)record, 1);
    Actor_Jump(0, 4, 0);
    Actor_SetAnimation(0, 1);
    Func_02002780(0, 0x6000, 60);
    Actor_ShowEmote(0, 0x105, 0);
    Actor_ShowEmote(2, 0x105, 0);
    Event_Wait(60);
    Func_02002780(0, 0xa000, 20);
    Actor_SetAnimationAndWait(1, 3);
    Actor_SetAnimationAndWait(0, 3);
    Func_02002780(0, 0x6000, 10);
    Func_02002780(1, 0x4000, 10);
    Actor_FaceDirection(2, 0xc000, 0);
    Actor_SetAnimationAndWait(2, 3);
    Func_02002768(2, 20);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimation(3, 3);
    Actor_SetAnimationAndWait(1, 3);
    Event_Wait(20);
    Actor_RunRepeatedMotion(2, 1);
    Event_Wait(20);
    Event_OpenMessage(2, 0);
    Actor_SetAnimation(2, 3);
    Actor_FaceDirection(2, 0xe000, 0);
    Actor_FaceDirection(1, 0x2000, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Actor_SetAnimation(2, 3);
        Actor_SetAnimationAndWait(1, 3);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    } else {
        Actor_RunRepeatedMotion(1, 2);
        Func_02002780(1, 0x2000, 10);
        Actor_StartRepeatedMotion(1, 2);
        Event_ShowMessage(1, 0);
    }
    Func_02002780(1, 0x4000, 10);
    Actor_SetAnimationAndWait(1, 4);
    Func_02002768(1, 20);
    Func_02002780(2, 0xc000, 10);
    Actor_SetAnimationAndWait(2, 3);
    Func_02002768(2, 10);
    if (Data_0200b394 != 0) {
        Actor_RunRepeatedMotion(3, 2);
        Func_02002780(3, 0, 20);
        Func_02002780(3, 0x2000, 10);
        Actor_SetAnimation(3, 4);
        Func_02002768(3, 10);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Actor_FaceDirection(1, 0x2000, 0);
    Func_02002780(0, 0xa000, 10);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimationAndWait(1, 3);
    value = 128;
    Event_Wait(20);
    Actor_FaceDirection(0, 0x6000, 0);
    Func_02002780(1, (value << 7), 10);
    Actor_SetAnimationAndWait(2, 4);
    Event_Wait(20);
    Actor_ShowEmote(0, 0x102, 0);
    Actor_ShowEmote(1, 0x102, 0);
    Event_Wait(80);
    Func_02002780(2, 0xe000, 10);
    Actor_StartRepeatedMotion(2, 2);
    Func_02002768(2, 20);
    Actor_FaceDirection(1, 0x2000, 0);
    Func_02002780(0, 0xa000, 40);
    Actor_FaceDirection(1, (value << 7), 0);
    Func_02002780(0, 0x6000, 10);
    Func_02002780(2, 0xc000, 10);
    Actor_SetAnimationAndWait(2, 3);
    Func_02002768(2, 10);
    Actor_SetAttachedEffect(1, 0x102);
    Event_Wait(40);
    Func_02002768(1, 20);
    Actor_SetAnimationAndWait(2, 3);
    Event_Wait(20);
    Actor_ShowEmote(1, 0x102, 0);
    Event_Wait(40);
    Func_02002768(1, 20);
    Actor_SetAnimation(2, 3);
    Func_02002768(2, 10);
    Actor_RunRepeatedMotion(1, 2);
    Func_02002780(1, 0x2000, 10);
    Event_OpenMessage(1, 0);
    Actor_FaceDirection(0, 0xa000, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Actor_SetAnimationAndWait(1, 3);
    } else {
        Event_Wait(20);
        Actor_RunRepeatedMotion(1, 2);
        Event_Wait(40);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Event_ShowMessage(1, 0);
    Audio_PlayCue(21);
    ColorBuffer_ApplyTarget(0x406218, 1);
    ColorBuffer_Interpolate(60);
    Task_Wait(60);
    Data_0200b388 = 0;
    Data_0200b384 = 0x800000;
    sequence_phase = &Data_0200b38c;
    *sequence_phase = 1;
    Value2(Func_02002b8c, (s32)Data_0200a975, 0xc80);
    Event_Wait(80);
    Actor_StartRepeatedMotion(0, 2);
    Actor_StartRepeatedMotion(1, 2);
    Actor_StartRepeatedMotion(3, 2);
    Actor_RunRepeatedMotion(2, 2);
    Event_Wait(60);
    Func_02002780(2, 0xc000, 10);
    Event_SetMessage(0x149d);
    Func_02002768(2, 10);
    Func_02002780(1, 0xc000, 10);
    Func_02002780(0, 0xc000, 10);
    sequence_flag = &Data_0200b394;
    if (*sequence_flag != 0) {
        Func_02002780(3, 0xc000, 10);
    }
    *(u8 *)(Func_02002c24(0) + 35) &= 254;
    *(u8 *)(Func_02002c24(1) + 35) &= 254;
    *(u8 *)(Func_02002c24(2) + 35) &= 254;
    *(u8 *)(Func_02002c24(3) + 35) &= 254;
    Actor_SetSpritePriority(0, 3);
    Actor_SetSpritePriority(1, 3);
    Actor_SetSpritePriority(2, 3);
    Actor_SetSpritePriority(3, 3);
    *sequence_phase = 2;
    Audio_PlayCue(220);
    Actor_SetPosition(13, 0xfd0000, 0x25b0000);
    formation_action = (s32)Data_0200af6c;
    Actor_EnableActionCallback(13, formation_action);
    Actor_SetPosition(14, 0xe90000, 0x2750000);
    Actor_EnableActionCallback(14, formation_action);
    if (*sequence_flag != 0) {
        Actor_SetPosition(15, 0xcf0000, 0x2610000);
        Actor_EnableActionCallback(15, formation_action);
    }
    Actor_SetPosition(16, 0xe30000, 0x2440000);
    Actor_EnableActionCallback(16, formation_action);
    Event_Wait(120);
    *sequence_phase = 3;
    do {
        Task_Wait(1);
    } while (Data_0200b38c != 0);
    Func_02002768(11, 80);
    Func_02002768(12, 20);
    Actor_ShowEmote(0, 0x101, 0);
    Actor_ShowEmote(1, 0x101, 0);
    Actor_ShowEmote(2, 0x101, 0);
    Actor_ShowEmote(3, 0x101, 0);
    Event_Wait(60);
    Func_02002768(12, 20);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimation(3, 3);
    Actor_SetAnimationAndWait(2, 3);
    Func_02002768(12, 10);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimation(3, 3);
    Actor_SetAnimationAndWait(2, 3);
    Func_02002768(12, 10);
    Actor_ShowEmote(0, 0x100, 0);
    Actor_ShowEmote(1, 0x100, 0);
    Actor_ShowEmote(2, 0x100, 0);
    Actor_ShowEmote(3, 0x100, 0);
    Event_Wait(40);
    Func_02002768(11, 10);
    Actor_FaceDirection(0, 0x8000, 0);
    Actor_FaceDirection(1, 0x4000, 0);
    Actor_FaceDirection(3, 0, 0);
    Func_02002780(2, 0xc000, 40);
    Func_02002768(12, 10);
    Actor_ShowEmote(0, 0x102, 0);
    Actor_ShowEmote(1, 0x102, 0);
    Actor_ShowEmote(2, 0x102, 0);
    Actor_ShowEmote(3, 0x102, 0);
    Actor_FaceDirection(0, 0xc000, 0);
    Actor_FaceDirection(1, 0xc000, 0);
    Actor_FaceDirection(2, 0xc000, 0);
    Func_02002780(3, 0xc000, 80);
    Func_02002768(12, 10);
    Actor_FaceDirection(0, 0x8000, 0);
    Actor_FaceDirection(1, 0x4000, 0);
    Actor_FaceDirection(2, 0xc000, 0);
    Func_02002780(3, 0, 40);
    Func_02002768(11, 10);
    Actor_FaceDirection(0, 0xc000, 0);
    Actor_FaceDirection(1, 0xc000, 0);
    Actor_FaceDirection(2, 0xc000, 0);
    Func_02002780(3, 0xc000, 10);
    Actor_SetAnimation(0, 4);
    Actor_SetAnimation(1, 4);
    Actor_SetAnimation(3, 4);
    Actor_SetAnimationAndWait(2, 4);
    Event_Wait(60);
    Func_02002768(12, 10);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimation(3, 3);
    Actor_SetAnimationAndWait(2, 3);
    Func_02002768(12, 20);
    Actor_FaceDirection(0, 0x8000, 0);
    Actor_FaceDirection(1, 0x4000, 0);
    Actor_FaceDirection(2, 0xc000, 0);
    Func_02002780(3, 0, 20);
    Func_02002768(12, 10);
    Actor_StartRepeatedMotion(0, 2);
    Actor_StartRepeatedMotion(1, 2);
    Actor_StartRepeatedMotion(3, 2);
    Actor_RunRepeatedMotion(2, 2);
    Actor_FaceDirection(0, 0xc000, 0);
    Actor_FaceDirection(1, 0xc000, 0);
    Actor_FaceDirection(2, 0xc000, 0);
    Actor_FaceDirection(3, 0xc000, 0);
    Func_02002768(12, 20);
    Actor_FaceDirection(0, 0x8000, 0);
    Actor_FaceDirection(1, 0x4000, 0);
    Actor_FaceDirection(2, 0xc000, 0);
    Func_02002780(3, 0, 20);
    Func_02002768(11, 20);
    Actor_ShowEmote(0, 0x102, 0);
    Actor_ShowEmote(1, 0x102, 0);
    Actor_ShowEmote(3, 0x102, 0);
    Actor_ShowEmote(2, 0x102, 0);
    Event_Wait(40);
    Func_02002768(12, 10);
    Actor_FaceDirection(0, 0xc000, 0);
    Actor_FaceDirection(1, 0xc000, 0);
    Actor_FaceDirection(2, 0xc000, 0);
    Func_02002780(3, 0xc000, 10);
    Func_02002768(12, 10);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimation(3, 3);
    Actor_SetAnimationAndWait(2, 3);
    Event_Wait(60);
    Event_ShowMessage(12, 0);
    Event_ShowMessage(11, 0);
    Value1(Func_02002b94, (s32)Data_0200a975);
    Event_Wait(80);
    ColorBuffer_ApplyTarget(0x10000, 1);
    ColorBuffer_Interpolate(60);
    Task_Wait(80);
    Actor_Stop(13);
    Actor_Stop(14);
    finish_flag = &Data_0200b394;
    Actor_Stop(15);
    Actor_Stop(16);
    Task_Wait(1);
    finish_action = (s32)Data_0200afc8;
    Actor_EnableActionCallback(13, finish_action);
    Actor_EnableActionCallback(14, finish_action);
    if (*finish_flag != 0) {
        Actor_EnableActionCallback(15, finish_action);
    }
    Func_02002c4c(16, finish_action);
    Event_Wait(20);
    Actor_SetSpritePriority(0, 2);
    Actor_SetSpritePriority(1, 2);
    Actor_SetSpritePriority(2, 2);
    value = 1;
    Actor_SetSpritePriority(3, 2);
    *(u8 *)(Func_02002c24(0) + 35) |= value;
    *(u8 *)(Func_02002c24(1) + 35) |= value;
    *(u8 *)(Func_02002c24(2) + 35) |= value;
    {
        u8 *record = Func_02002c24(3);
        u8 flags = (u8)(value | record[35]);

        record[35] = flags;
    }
    Actor_RunRepeatedMotion(2, 2);
    Func_02002780(2, 0xe000, 10);
    Event_OpenMessage(2, 0);
    Actor_FaceDirection(1, 0x2000, 0);
    Actor_FaceDirection(3, 0, 0);
    if (Event_ChooseYesNo(0, 0) != 0) {
    } else {
        Actor_RunRepeatedMotion(1, 2);
        Event_Wait(10);
        Event_OpenMessage(1, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            Func_02002780(3, 0, 20);
            Actor_ShowEmote(1, 0x101, 0);
            Actor_ShowEmote(2, 0x101, 0);
            Actor_ShowEmote(3, 0x101, 0);
            Event_Wait(40);
            Func_02002780(1, 0x4000, 20);
            Func_02002768(1, 10);
            Func_02002780(2, 0xc000, 20);
            Func_02002780(2, 0xe000, 20);
            Actor_SetAnimationAndWait(2, 3);
            Func_02002768(2, 20);
            Func_02002780(1, 0x2000, 20);
        } else {
            Func_02002780(3, 0, 20);
            Actor_ShowEmote(1, 0x102, 0);
            Actor_ShowEmote(2, 0x102, 0);
            Actor_ShowEmote(3, 0x102, 0);
            Event_Wait(40);
            Func_02002780(1, 0x4000, 20);
            Event_SetMessage(0x14b4);
            Func_02002768(1, 20);
            Actor_SetAnimationAndWait(2, 3);
            Func_02002768(2, 20);
        }
        Actor_SetAnimation(3, 3);
        Actor_SetAnimationAndWait(1, 3);
        goto L_02002528;
    }
    Event_Wait(20);
    Actor_SetAnimationAndWait(1, 3);
    Event_Wait(10);
    Event_SetMessage(0x14b6);
    Func_02002768(1, 10);
    Actor_FaceDirection(1, 0x4000, 0);
    Func_02002780(0, 0x6000, 20);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimationAndWait(0, 3);
    Event_Wait(10);
    Actor_SetAnimationAndWait(2, 4);
    Event_OpenMessage(2, 0);
    if (Event_ChooseYesNo(0, 0) != 0) {
    } else {
        Event_Wait(20);
        Actor_ShowEmote(2, 0x103, 0);
        Event_Wait(40);
        Func_02002780(2, 0xe000, 10);
        Func_02002768(2, 10);
        if (*finish_flag != 0) {
            Func_02002780(3, 0, 10);
            Actor_StartRepeatedMotion(3, 3);
            Func_02002768(3, 20);
        } else {
            *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
        }
        Actor_ShowEmote(0, 0x102, 0);
        Actor_ShowEmote(1, 0x102, 0);
        Event_Wait(40);
        Actor_RunRepeatedMotion(1, 2);
        Func_02002768(1, 20);
        Actor_ShowEmote(1, 0x105, 0);
        Event_Wait(120);
        Func_02002768(2, 40);
        if (Data_0200b394 != 0) {
            Func_02002780(3, 0x2000, 10);
            Actor_SetAnimationAndWait(3, 4);
            Func_02002768(3, 10);
        } else {
            *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
        }
        Event_Wait(60);
        Actor_RunRepeatedMotion(2, 2);
        if (Data_0200b394 != 0) {
            Func_02002780(2, 0xa000, 40);
            Func_02002780(2, 0xe000, 20);
        }
        Func_02002768(2, 10);
        Actor_StartRepeatedMotion(0, 2);
        Actor_RunRepeatedMotion(1, 2);
        Event_Wait(40);
        Func_02002768(2, 20);
        Actor_SetAnimationAndWait(0, 3);
        Actor_SetAnimationAndWait(1, 3);
        Event_Wait(20);
        Actor_SetAnimation(3, 3);
        L_02002528:;
        Actor_SetAnimationAndWait(2, 3);
        goto L_02002660;
    }
    Actor_ShowEmote(2, 0x105, 0);
    Event_Wait(40);
    Actor_SetAnimationAndWait(2, 3);
    Event_SetMessage(0x14bf);
    Func_02002768(2, 20);
    if (*finish_flag != 0) {
        Func_02002780(3, 0, 10);
        Actor_StartRepeatedMotion(3, 1);
        Func_02002768(3, 20);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Actor_ShowEmote(1, 0x102, 0);
    Actor_ShowEmote(0, 0x102, 0);
    Event_Wait(40);
    Actor_RunRepeatedMotion(1, 2);
    Func_02002768(1, 20);
    Actor_ShowEmote(2, 0x105, 0);
    Event_Wait(80);
    Func_02002768(2, 40);
    if (Data_0200b394 != 0) {
        Func_02002780(3, 0x2000, 20);
        Actor_SetAnimation(3, 4);
        Func_02002768(3, 40);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Actor_RunRepeatedMotion(2, 2);
    Event_Wait(20);
    Func_02002768(2, 20);
    Actor_StartRepeatedMotion(1, 2);
    Actor_RunRepeatedMotion(0, 2);
    Event_Wait(40);
    Func_02002768(2, 20);
    L_02002660:;
    Audio_PlayCue(17);
    Actor_SetSpeed(1, 0x13333, 0x9999);
    Actor_SetSpeed(2, 0x13333, 0x9999);
    Actor_SetSpeed(3, 0x13333, 0x9999);
    Actor_SetAnimation(1, 2);
    {
        u8 *record = Record1(Func_02002c24, 0);

        if (record != 0) {
            Actor_SetDestination(1, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
        }
    }
    Actor_WaitForMove(1);
    Actor_SetPosition(1, 0, 0);
    Actor_SetAnimation(2, 2);
    {
        u8 *record = Record1(Func_02002c24, 0);

        if (record != 0) {
            Actor_SetDestination(2, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
        }
    }
    Actor_WaitForMove(2);
    Actor_SetPosition(2, 0, 0);
    if (Data_0200b394 != 0) {
        Actor_SetAnimation(3, 2);
        {
            u8 *record = Record1(Func_02002c24, 0);

            if (record != 0) {
                Actor_SetDestination(3, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
            }
        }
        Actor_WaitForMove(3);
        Actor_SetPosition(3, 0, 0);
    }
    GameFlag_Set(0x843);
    Actor_SetPosition(11, 0, 0);
    Actor_SetPosition(12, 0, 0);
    Func_02002d14();
    Event_End();
}

void FieldScene_RunSplitPairSteps(s32 a, s32 b)
{
    Event_ShowMessage(a, 0);
    Event_Wait(b);
}

void SceneActor_SetPairZeroAndValue(s32 a, s32 b, s32 c)
{
    Actor_FaceDirection(a, b, 0);
    Event_Wait(c);
}

s32 SceneEffect_AdvanceAngleAndFinishWhenParked(struct Struct2798 *p)
{
    p->field18 += 0x1eb8;
    if (p->field38 == (s32)0x80000000
        && p->field3c == (s32)0x80000000
        && p->field40 == (s32)0x80000000) {
        Engine_ObjectDispatchRelease(p);
    }
    return 1;
}

void SceneEffect_SpawnObject26EveryEightFrames(void)
{
    extern s32 Data_03001e40;

    struct Obj *obj;
    struct Sub *sprite;
    s32 phase;
    s32 v;
    s32 w;
    s32 c1 = 0x00e70000;
    s32 c2 = 0x01cc0000;
    s32 c3 = 0x00e70000;
    s32 c4 = 0x02700000;

    phase = Data_03001e40 & 7;
    if (phase != 0) return;
    if (Data_0200b398 != 0) Audio_PlayCue(200);
    obj = Func_020053a6(26, c1, 0, c2);
    if (obj == 0) return;
    sprite = obj->f50;
    sprite->f26 = phase;
    v = 0xfe;
    v &= obj->f23;
    obj->f23 = v;
    w = ~12;
    w &= sprite->f09;
    w |= 4;
    sprite->f09 = w;
    obj->f18 = 0x1999;
    obj->f30 = 0x80000;
    obj->f34 = 0x80000;
    obj->f55 = phase;
    Object_SetAnimation(obj, 2);
    Func_0200540e(obj, c3, 0, c4);
    Object_SetScript(obj, Data_0200b2d0);
}

s32 OverlayObject_SelectValueByFrameBit1(s32 obj)
{
    extern u32 Data_03001e40;

    if (((Data_03001e40 >> 1) & 1) != 0) {
        Object_SetPalette(obj, 10);
    } else {
        Object_SetPalette(obj, 7);
    }
    return 0;
}

s32 SceneActor_CheckRegionTrigger(struct Struct288c *arg0)
{
    s32 x;

    if (Data_0200b394 != 0) {
        x = arg0->field08;
        if (x > 0xc00000 && x < 0x1120000
            && arg0->field10 > 0x2360000 && arg0->field10 < 0x2640000) {
            goto hit;
        }
        if (x > 0xca0000 && x < 0xff0000
            && arg0->field10 > 0x2250000 && arg0->field10 < 0x2780000) {
            goto hit;
        }
    } else {
        x = arg0->field08;
        if (x > 0xc00000 && x < 0xf40000
            && arg0->field10 > 0x2250000 && arg0->field10 <= 0x248ffff) {
            goto hit;
        }
        if (x > 0xf40000 && x < 0x1120000
            && arg0->field10 > 0x23b0000 && arg0->field10 <= 0x25cffff) {
            goto hit;
        }
        if (x > 0xd30000 && x < 0xff0000
            && arg0->field10 > 0x2540000 && arg0->field10 < 0x2780000) {
            goto hit;
        }
    }
    return 0;
hit:
    Audio_PlayCue(106);
    Func_020054ca(arg0, Data_0200b2e4);
    Data_0200b390 = 1;
    return 0;
}

void SceneActor_AlternateSlots13To16Field0c(void)
{
    extern u32 Data_03001e40;

    struct Struct5702 *p;

    p = Func_02005702(13);
    if (p != 0) {
        p->field55 = 0;
        if ((Data_03001e40 & 1) == 0) {
            p->field0c = 0;
        } else {
            p->field0c = 0x1f40000;
        }
    }
    p = Func_0200572a(14);
    if (p != 0) {
        p->field55 = 0;
        if ((Data_03001e40 & 1) != 0) {
            p->field0c = 0;
        } else {
            p->field0c = 0x1f40000;
        }
    }
    p = Func_02005752(15);
    if (p != 0) {
        p->field55 = 0;
        if ((Data_03001e40 & 1) == 0) {
            p->field0c = 0;
        } else {
            p->field0c = 0x1f40000;
        }
    }
    p = Func_0200577a(16);
    if (p != 0) {
        p->field55 = 0;
        if ((Data_03001e40 & 1) != 0) {
            p->field0c = 0;
        } else {
            p->field0c = 0x1f40000;
        }
    }
}
