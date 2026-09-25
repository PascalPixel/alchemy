#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "FIELD_EFFECT.H"
#define OverlayObject_IntegrateVelocities Effect_Move

#define NULL ((void *)0)
#define StagedActorDirectionSteps Data_0200d0e8
#define FindActorAtPosition Func_020006e6
#define FindActorAtForwardPosition Func_02000712
#define FindActorAbovePosition Func_0200073e
#define CheckStagedActorMove Func_0200528c
#define SetStagedActorMoveTarget Func_0200528a
#define SetLeadActorMoveTarget Func_0200529a
#define UpdateStagedActorState Func_020052a8
#define FinalizeStagedActorUpdate Func_02005494
#define BattleEffect_PlayQueuedSound_1(args...) Func_02005874(args)
#define BattleEffect_RunRisingObjectSequence_1(args...) Func_02007e92(args)
#define OverlayObject_IntegrateVelocities Func_020000e0

#include "OVERLAY_OBJECT.H"
#include "STAGED_ACTOR.H"

/*
 * Effect spawning for overlay resource_39f.  Every cutscene beat in the
 * overlay creates its effects through this routine.
 */

enum EntrySetupMessage {
    MSG_DOOR_TIGHTLY_LOCKED = 0x953,
    MSG_VENUS_LIGHTHOUSE_WAS_ATTACKED_BY = 0x266d,
    MSG_IVE_WAITED_LONG_SEE_ITS = 0x2670,
    MSG_THOUGHT_ID_EXPLORE_AFTER_DOOR = 0x267d,
    MSG_THERE_WORDS_CARVED_INTO_RELIEF = 0x2682,
    MSG_STATUE_SPEAKS_ROBIN_SOUL_YE = 0x2688,
    MSG_HMMM_WE_CANT_PUSH_BLOCK = 0x2691
};

struct Sprite {
    u8 pad00[9];
    u8 flags9;
    u8 pad0a[20];
    u16 angle;
    u8 pad20[6];
    u8 state26;
};

struct Effect {
    u8 pad00[24];
    s32 accum18;
    s32 accum1c;
    u8 pad20[3];
    u8 flatla3;
    u8 pad24[12];
    s32 rate30;
    s32 rate34;
    u8 pad38[12];
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    struct Sprite *sprite;
    u8 pad54;
    u8 mode55;
    u8 pad56[14];
    u16 step64;
    u8 pad66[6];
    u32 callback;
};

typedef struct {
    u8 filler0[8];
    s32 unk8;
    u8 fillerC[4];
    s32 unk10;
    u8 filler14[15];
    u8 unk23;
    u8 filler24[49];
    u8 unk55;
} Struct_1a14;

struct Args_02003fe4 {
    s32 first;
    s32 unused;
    s32 last;
};

struct Args_02003ffc {
    s32 first;
    s32 unused;
    s32 last;
};

struct Args_02004014 {
    s32 first;
    s32 unused;
    s32 last;
};

struct Args_02004030 {
    s32 first;
    s32 unused;
    s32 last;
};

struct BusyObject {
    u8 filler00[0x28];
    s32 busy;
};

typedef struct {
    u8 filler0[9];
    u8 unk9;
} Struct_Sub;

typedef struct {
    u8 filler0[8];
    s32 unk8;
    u8 fillerC[4];
    s32 unk10;
    u8 filler14[48];
    s32 unk44;
    u8 filler48[8];
    Struct_Sub *unk50;
} Struct_1a50;

struct Object_020005e4 { u8 unknown_00[8]; s32 x; s32 y; s32 z; };

struct EffectParams_020005e4 { s32 unk00; s32 mode; s32 color1; s32 color2; u8 unknown_10[0x18]; };

struct Object_020006a0 {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
};

struct EffectParams_020006a0 {
    s32 unk00;
    s32 mode;
    s32 color1;
    s32 color2;
    u8 unknown_10[0x18];
};

struct EffectObject {
    u8 filler00[0x30];
    s32 color;
    u8 filler34[0x38];
    s32 callback;
};

struct SceneObject {
    u8 filler00[8];
    s32 x;
    s32 y;
    s32 z;
};

struct Vec {
    s32 x;
    s32 y;
    s32 z;
};

struct EffectParams {
    s32 unk00;
    s32 mode;
    u8 filler08[0x1c];
    s32 callback;
};

struct SceneObject_02000b98 {
    u8 filler00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 filler14[0x24];
    s32 near;
    u8 filler3c[4];
    s32 far;
};

struct EffectParams_02000b98 {
    s32 unk00;
    s32 mode;
    s32 color1;
    s32 color2;
    s32 unk10;
    s32 unk14;
    u16 angle;
    u8 filler1a[0xa];
    s32 callback;
};

struct EffectObject_02000c5c {
    u8 filler00[0xc];
    s32 timer;
    u8 filler10[0x13];
    u8 mode;
    u8 filler24[0xc];
    s32 scale_x;
    s32 scale_y;
    u8 filler38[0x1d];
    u8 state;
    u8 filler56[3];
    u8 phase;
    u8 filler5a[9];
    u8 flag;
    u8 filler64[8];
    s32 callback;
};

typedef struct {
    u8 filler0[8];
    s32 unk8;
    u8 fillerC[4];
    s32 unk10;
} Struct_A;

typedef struct {
    u8 filler0[10];
    s16 unkA;
    u8 fillerC[6];
    s16 unk12;
} Struct_B;

struct Actor_020008c8 { u8 unknown_00[8]; s32 x; s32 y; s32 z; u8 unknown_14[0xf]; u8 flatla3; };

struct Position {
    u32 filler00[2];
    s32 x;
    s32 y;
    s32 z;
};

typedef struct {
    u8 filler0[8];
    s32 unk8;
    u8 fillerC[4];
    s32 unk10;
} Struct_0ff0;

typedef struct {
    u8 filler0[8];
    s32 unk8;
    u8 fillerC[4];
    s32 unk10;
} Struct_199c;

typedef struct {
    u8 filler0[8];
    s32 unk8;
    s32 unkC;
    s32 unk10;
} Struct_19c0;

typedef struct {
    u8 filler0[8];
    s32 unk8;
} Struct_1a9c;

typedef struct {
    s16 filler0[50];
    s32 unk64;
} Struct_22a4;

typedef struct {
    s32 filler0[3];
    s32 unkC;
} Struct_22a4b;

/*
 * Per-frame motion for an effect record in resource_3c8.
 */
struct Sprite_0200096c {
    u8 pad00[9];
    u8 flags9;
    u8 pad0a[20];
    u16 angle;
    u8 pad20[6];
    u8 state26;
};

struct Effect_0200096c {
    u8 pad00[8];
    s32 position[3];
    u8 pad14[4];
    s32 accum18;
    s32 accum1c;
    u8 pad20[3];
    u8 flatla3;
    u8 pad24[12];
    s32 rate30;
    s32 rate34;
    u8 pad38[12];
    s32 velocity[3];
    struct Sprite_0200096c *sprite;
    u8 pad54;
    u8 mode55;
    u8 pad56[14];
    u16 step64;
    u8 pad66[6];
    u32 callback;
};

struct SceneObject_02000cc8 {
    u8 filler00[6];
    u16 angle;
    s32 x;
    s32 y;
    s32 z;
    u8 filler14[0x14];
    s32 accel;
    u8 filler2c[4];
    s32 scale_x;
    s32 scale_y;
    u8 filler38[0x1d];
    u8 state;
};

union VecView {
    struct Vec v;
    s16 h[6];
};

typedef struct {
    u8 filler0[8];
    s32 unk8;
    s32 unkC;
    u8 filler10[4];
    s32 unk14;
    u8 filler18[61];
    u8 unk55;
} Struct_1644;

typedef struct {
    u8 filler0[8];
    s32 unk8;
    u8 fillerC[4];
    s32 unk10;
} Struct_18f8;

extern u32 Data_0200d0e8[];
extern u8 *Data_03001e70;
extern u8 Data_02010000[];
extern u8 Value_00000212;
extern u32 Data_03001e40;
extern u8 Value_000000b5;
extern u8 Value_000000b7;
extern u8 Value_000000b8;
extern u8 Value_000000b9;
extern u8 Value_000000ba;
extern u8 Data_0200dd68[];
extern u8 Data_0200e020[];
extern u8 Data_0200e230[];
extern u8 Data_0200e350[];
extern u8 Data_0200e548[];
extern u8 Data_0200ddc8[];
extern u8 Data_0200e740[];
extern u8 Value_000000b6;
extern u8 Data_0200e8ec[];
extern u8 Data_0200e904[];
extern u8 Data_0200e9c4[];
extern u8 Data_0200eb74[];
extern u8 Data_0200ec04[];
extern u8 Data_0200ec64[];
extern u8 Data_0200ecf4[];
extern u8 Data_0200ee44[];
extern u8 Data_0200f120[];
extern u8 Data_0200f300[];
extern u8 Data_0200f3b4[];
extern u8 Data_0200f4f8[];
extern u8 Data_0200ef1c[];

typedef s32(*IwramSqrt020002f0)(s32);
void Func_02007252();
struct StagedActor *Func_020006e6(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *Func_02000712(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *Func_0200073e(s32 *arg0, struct StagedActor *arg1);
s32 Func_0200528c(struct StagedActor *arg0, s32 *arg1);
void Func_0200528a(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_0200529a(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_020052a8();
void Func_02005494(void);
void Func_020041fa(void);
void Func_0200803c();
void Func_02008054();
void Func_0200806e();
void Func_02008088();
void Func_02008b1a();
void Func_0200078e();
s32 Func_0200081a();
s32 Func_02000852();
struct SceneObject *Func_020059c2(void);
void Func_02000c8a(s32, s32, s32, s32, s32, s32, s32, struct EffectParams *);
void Func_02000d52(s32, s32, s32, s32, s32, s32, s32, struct EffectParams *);
void Func_02005874();
void Func_02000b9a();
void Func_02005ac6();
void Func_0200602e();
void Func_02007e92();
void Func_02008b2a();
void Func_02006818(s32, s32, s32, s32);
void Func_02006842(s32, s32, s32, s32);
void Func_02002400(void);
void Func_02002422(void);
void Func_02005ae6(s32, s32, struct Vec *);
s32 Func_02005b5e(struct SceneObject *, struct Vec *);
s32 Func_0200105a(struct Vec *, struct SceneObject *);
void Func_02005b2c(s32, s32, struct Vec *);
s32 Func_02001094(struct Vec *, struct SceneObject *);
s32 Func_02005bb0(struct SceneObject *, struct Vec *);
void Func_02005d78(u8 *);
void Func_02005a3a(void);
void Func_02003c9e(void);

/* Complete scene setup, including the two global halfword state writes. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
void SceneEffect_SpawnNineRadialEffects(void);

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call8(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7)
{
    f(a0, a1, a2, a3, a4, a5, a6, a7);
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void SetActorDirection(s32 actor, s32 direction, s32 duration)
{
    Actor_FaceDirection(actor, direction, duration);
}

void ConfigureOverlayObject(struct OverlayObject *object, s32 parameter)
{
    object->unknown_55 = 0;
    object->unknown_59 = 8;
    Actor_SetSpriteFlags(object, 0);
    Object_SetPalette(object, parameter);
}

void *OverlayObject_SpawnWithMode14(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    void *obj;
    u8 *p;
    s32 mask;
    u8 flag;

    obj = Object_Create(arg3, arg0, arg1, arg2);
    if (obj != 0) {
        p = *(u8 **)((u8 *)obj + 0x50);
        mask = 13;
        flag = p[9];
        mask = -mask;
        mask &= flag;
        p[9] = mask;
        ConfigureOverlayObject(obj, 0xE);
        Object_SetBlendMode(obj, 1);
        return obj;
    }
    return 0;
}

void *OverlayObject_PrepareObjectWithCommand15(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    void *object;
    u8 *rec;
    s32 flags;
    s32 result;

    object = Object_Create(arg3, arg0, arg1, arg2);
    if (object != 0) {
        rec = *(u8 **)((u8 *)object + 0x50);
        flags = rec[9];
        flags = (flags & -13) | 4;
        rec[9] = flags;
        ConfigureOverlayObject(object, 0xF);
        flags = *((u8 *)object + 0x23);
        result = 2;
        result |= flags;
        *((u8 *)object + 0x23) = result;
        return object;
    }
    return 0;
}

s32 SceneActor_CalculateFixedPointDistance(s32 *a, s32 *b)
{
    s32 dx = (*a++ - *b++) >> 16;
    s32 dy = (*a++ - *b++) >> 16;
    s32 dz = (*a - *b) >> 16;
    s32 dxsq = dx *dx;
    s32 dysq = dy *dy;
    s32 dzsq = dz *dz;

    return ((IwramSqrt020002f0) 0x030001D8)(dxsq + dysq + dzsq);
}

/*
 * CALL SYMBOLS ARE PER-SITE: the raw assembly spells each of these eight
 * calls as a direct `bl sub_020072xx` to an address inside this overlay's
 * own 0x0200xxxx range (verified via `arm-none-eabi-objdump -dr -M
 * force-thumb` on the assembled .o, which resolves the *ABS* targets before
 * linking) -- lifted verbatim, not the veneer-math final target names this
 * file used before. The one true indirect call (selector's local-effect
 * dispatch) is routed automatically through this overlay's own
 * `_call_via_rN` bank; SceneEffect_SpawnNineRadialEffects is correct for it, unchanged.
 */
s32 *SceneData_FindSlotAtPosition(s32 *pos)
{
    extern u8 *Data_03001ebc;
    void Func_02000b08();

    s32 **slots = (s32 **)(Data_03001ebc + 0x14);
    u32 i;

    for (i = 8; i <= 65; i++) {
        s32 *p = slots[i];

        if ((pos[0] >> 20) == (p[2] >> 20)
            && (pos[1] >> 20) == (p[3] >> 20)
            && (pos[2] >> 20) == (p[4] >> 20)) {
            return p;
        }
    }
    return 0;
}

void RunStagedActorTransition(void)
{
    s32 target_position[3];
    struct StagedActor *leader;
    struct StagedActor *actor;
    struct StagedActor *blocking_actor;
    s32 direction_index;
    u32 packed_step;
    s32 move_rate;
    s32 transition_busy;

    leader = Actor_Get(ACTOR_PARTY_LEADER);
    direction_index = leader->direction_and_kind >> 12;
    packed_step = StagedActorDirectionSteps[direction_index];
    target_position[0] = leader->x.value + (packed_step & 0xffff0000);
    target_position[1] = leader->y;
    packed_step <<= 16;
    target_position[2] = leader->z.value + packed_step;
    actor = FindActorAtPosition(target_position, leader);
    if (actor == 0) return;

    packed_step = StagedActorDirectionSteps[direction_index];
    target_position[0] = actor->x.value + (packed_step & 0xffff0000);
    target_position[1] = actor->y;
    packed_step <<= 16;
    target_position[2] = actor->z.value + packed_step;
    blocking_actor = FindActorAtForwardPosition(target_position, actor);
    if (blocking_actor != 0 && (blocking_actor->collision_flags & 1) != 0) return;

    target_position[0] = actor->x.value;
    target_position[1] = actor->y + 0x100000;
    target_position[2] = actor->z.value;
    blocking_actor = FindActorAbovePosition(target_position, actor);
    if (blocking_actor != 0 && (blocking_actor->collision_flags & 1) != 0) return;

    actor->transition_mode = 2;
    packed_step = StagedActorDirectionSteps[direction_index];
    target_position[0] = actor->x.value + (packed_step & 0xffff0000);
    target_position[1] = actor->y;
    packed_step <<= 16;
    target_position[2] = actor->z.value + packed_step;
    if (CheckStagedActorMove(actor, target_position) > 0) return;

    transition_busy = actor->transition_busy;
    if (transition_busy != 0) return;

    Object_SetAnimation(leader, 8);
    move_rate = 0x3333;
    Task_Wait(15);
    Audio_PlayCue(185);
    actor->move_rate_x = move_rate;
    actor->move_rate_z = move_rate;
    SetStagedActorMoveTarget(actor, target_position[0], target_position[1], target_position[2]);
    leader->move_rate_x = move_rate;
    leader->move_rate_z = move_rate;
    SetLeadActorMoveTarget(leader, target_position[0], target_position[1], target_position[2]);
    UpdateStagedActorState(actor);
    FinalizeStagedActorUpdate();
    actor->x.value = target_position[0];
    actor->z.value = target_position[2];
    actor->unknown_24 = transition_busy;
    actor->unknown_2c = transition_busy;
    leader->unknown_38 = 0x80000000;
    leader->unknown_40 = 0x80000000;
    leader->unknown_24 = transition_busy;
    leader->unknown_2c = transition_busy;
    leader->x.value = leader->x.parts.cell << 16;
    leader->z.value = leader->z.parts.cell << 16;
    Object_SetAnimation(leader, 1);
}

s32 SceneState_FillGridCellByte2(u32 no, s32 x, s32 y, u32 w, u32 h, s32 val)
{
    u8 *g = Data_03001e70;
    u8 *base;
    u32 i;
    u32 j;

    if (g != 0) {
        if (no <= 2) {
            u32 off = no * 48 + 304;

            base = *(u8 **)(g + off);
        } else {
            base = Data_02010000;
        }
        base += (x + (y << 7)) * 4;
        for (i = 0; i < h; i++) {
            u8 *p = base + (i << 9);

            for (j = 0; j < w; j++) {
                p[2] = (u8)val;
                p += 4;
            }
        }
    }
    return 0;
}

/* Named shorthand for one fixed argument pair, in overlay resource_3c8. */
void SceneActor_ApplySlotsMatchingKind212(void)
{
    extern u8 *Data_03001ebc;

    s32 **slots = (s32 **)(Data_03001ebc + 0x14);
    u32 i;
    s32 lim = (s32)&Value_00000212;

    for (i = 8; i <= 65; i++) {
        s32 *p = slots[i];
        u32 h = *(u16 *)((u8 *)p + 100);
        s32 t = h << 16;

        if ((t >> 20) == lim) {
            s32 m = 15;
            m &= h;
            Object_SetPalette(p, m);
        }
    }
}

s32 OverlayObject_ApplyLowNibbleOfField100(void *obj)
{
    Object_SetPalette(obj, *(u16 *)((u8 *)obj + 100) & 15);
    return 0;
}

s32 OverlayObject_UpdateEveryFourFrames(void *obj)
{
    if ((*(u32 *)0x03001e40 & 3) == 0)
        Object_SetPalette(obj, 7);
    else
        Object_SetPalette(obj, 0);

    if ((*(u32 *)0x03001e40 & 7) == 0)
        Audio_PlayCue(138);
    return 0;
}

s32 SceneEffect_SpawnRandomEveryEightFramesB(struct Object_020005e4 *object)
{
    struct EffectParams_020005e4 params;
    s32 phase, x, y, speed;
    phase = *(u32 *)0x03001e40 & 7;
    if (phase != 0) goto done;
    params.unk00 = 3 - (s32)((u32)(Random_Next() * 2) >> 16);
    params.color1 = 0x6666;
    params.color2 = 0x6666;
    params.mode = 14;
    x = object->x + (((s32)((u32)(Random_Next() * 9) >> 16) - 4) << 16);
    y = object->y + ((32 - (s32)((u32)(Random_Next() * 32) >> 16)) << 16);
    speed = Math_Divide(((s32)((u32)(Random_Next() * 5) >> 16) << 16) + 0x00050000, 10);
    Func_0200078e(x, y, object->z, 0, speed, phase, 0x000b0000, &params);
done:
    return 0;
}

s32 OverlayObject_ApplyZero(void *obj)
{
    Actor_SetSpriteFlags(obj, 0);
    return 0;
}

s32 SceneEffect_SpawnTwoRandomizedParticles(struct Object_020006a0 *obj)
{
    struct EffectParams_020006a0 params;
    s32 speed;
    s32 phase;

    params.color1 = 0x0000cccc;
    params.color2 = 0x0000cccc;
    params.unk00 = 0;

    speed = (s32)((u32)(Random_Next() * 8) >> 16) * 0x3333;
    phase = Data_03001e40 & 15;

    Func_0200081a(
        obj->x + ((8 - phase) << 16),
        obj->y + 0x001a0000,
        obj->z,
        0,
        -speed,
        0,
        0x000a0000,
        &params);

    phase = Data_03001e40 & 15;
    if (phase == 0) {
        params.color1 = 0x00008000;
        params.color2 = 0x00008000;
        Func_02000852(
            obj->x
                + (((s32)((u32)(Random_Next() * 9) >> 16) - 4) << 16),
            obj->y,
            obj->z,
            0,
            0,
            0,
            0x000a0000,
            &params);
    }

    return 0;
}

void SceneEffect_RunObjectZeroColorSequence(void)
{
    struct EffectObject *obj;
    u8 *state;

    state = *(u8 **)0x03001ebc;
    obj = Actor_Get(ACTOR_PARTY_LEADER);
    Event_Begin();
    Audio_PlayCue(228);
    obj->callback = 0x020086a1;
    obj->color = 0x3333;
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 2);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -6);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
    obj->callback = 0;
    Event_Wait(30);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(*(s16 *)(state + 0x16c));
    Event_End();
}

/* Runs a guarded one-shot setup on an entry record: only fires the first
 * time (while a global 0x109 lookup is still unset), positions the entry
 * from its own stored coordinates, drives an effect/param sequence, then
 * writes a stage byte and an override field on the entry before returning. */
void FieldScene_RunOpeningAuxiliarySequence(void)
{
    u32 i;
    u8 *entry;
    u8 *guard;
    u8 *sub;

    entry = Actor_Get(ACTOR_PARTY_LEADER);
    guard = GameFlag_IsSet(0x109);
    if (guard == 0) {
        Event_Begin();
        Camera_MoveTo(-1, -1, -1, 0);
        /* Stage byte at +85 of the entry record. */
        entry[85] = guard;
        /* Position, from the entry's own s16 coordinates at +10/+18
         * (converted to 16.16 fixed point; the y term is offset by -16.0). */
        Actor_SetPosition(ACTOR_PARTY_LEADER, (*(s16 *)(entry + 10) << 16), ((*(s16 *)(entry + 18) << 16) + -0x100000));
        Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
        sub = Actor_Get(ACTOR_PARTY_LEADER);
        Actor_SetSpriteFlags(sub, 0);
        Event_OpenScreen();
        Event_WaitForScreen();
        Audio_PlayCue(228);
        /* Override field at +108 of the entry record; holds an EWRAM
         * address while the effect sequence below runs. */
        *(s32 *)(entry + 108) = 0x20086a1;
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x6666, 0x3333);
        Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, 8);
        Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
        sub = Actor_Get(ACTOR_PARTY_LEADER);
        Actor_SetSpriteFlags(sub, 1);
        Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, 8);
        entry[85] = 3;
        /* Restore the +108 override field to the original (unset) value. */
        *(s32 *)(entry + 108) = guard;
        BattleEffect_PlayQueuedSound_1();
        Event_End();
    }
}

void SceneState_StoreLookupZeroToWord24(void)
{
    *(s32 *)(*(u8 **)0x03001ee0 + 24) = Actor_Get(ACTOR_PARTY_LEADER);
}

void SceneState_ClearWorkspaceWord24(void)
{
    *(s32 *)(*(u8 **)0x03001ee0 + 24) = 0;
}

s32 SceneActor_SetFlagBitByRelativeDepth(struct Actor_020008c8 *actor)
{
    struct Actor_020008c8 *ref;
    u8 *fp;
    u8 flags;
    ref = Actor_Get(ACTOR_PARTY_LEADER);
    fp = &actor->flatla3;
    flags = *fp | 2;
    *fp = flags;
    if (ref->z < actor->z) {
        s32 diff = actor->z - ref->z;
        s32 lim;
        diff += 0x00040000;
        lim = actor->y;
        lim += diff;
        if (ref->y <= lim) {
            flags &= 0xfd;
            *fp = flags;
        }
    }
    return 0;
}

void SceneActor_SwapPositionsByDepth(s32 group, s32 index)
{
    struct Position *first;
    struct Position *second;
    s32 value;

    first = Actor_Get(group);
    second = Actor_Get(index);
    if (first->z <= second->z) {
        value = first->x;
        first->x = second->x;
        second->x = value;

        value = first->y;
        first->y = second->y;
        second->y = value;

        value = first->z;
        first->z = second->z;
        second->z = value;
        Task_Wait(1);
    }
}

void OverlayObject_WaitUntilIdle(struct BusyObject *obj)
{
    s32 cnt;
    s32 busy;

    cnt = 60;
    while (cnt != 0) {
        Task_Wait(1);
        busy = obj->busy;
        cnt--;
        if (busy == 0) break;
    }
}

/*
 * Alias for a loader-relocated call word into IWRAM, not a runtime address.
 * It turns a value and a duration into a per-frame step.
 */

/*
 * Owner at 0x0200096c.  Add the velocity at +68/+72/+76 into the position at
 * +8/+12/+16, decay the X and Z velocities, accumulate the rates at +48/+52
 * into +24/+28, and advance the sprite angle by the record's step.  The Z
 * decay must stay written as a signed divide by 16 -- the negative bias and
 * arithmetic shift are what that division compiles to.
 */
void Effect_AdvanceMotion(struct Effect_0200096c *effect)
{
    s32 velocity_z;
    struct Sprite_0200096c *sprite;
    s32 velocity_x;

    /* The block keeps the Z load after the Y store. */
    do {
        velocity_x = effect->velocity[0];
        effect->position[0] += velocity_x;
        effect->position[1] += effect->velocity[1];
    } while (0);
    velocity_z = effect->velocity[2];
    effect->position[2] += velocity_z;

    effect->velocity[0] = velocity_x - Math_Divide(velocity_x, 18);
    effect->velocity[2] = velocity_z - velocity_z / 16;

    effect->accum18 += effect->rate30;
    effect->accum1c += effect->rate34;

    sprite = effect->sprite;
    sprite->angle += effect->step64;
}

void FieldScene_RunSupplementalSequenceOne(void)
{
    struct FieldActor *actor;
    u32 i;
    struct EffectOptions options;

    actor = Actor_Get(ACTOR_PARTY_LEADER);
    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Map_Redraw();
    Task_Wait(1);
    actor->y.fixed = 0x820000;
    *(s32 *)((u8 *)actor + 72) = 0x8000;
    *(s32 *)((u8 *)actor + 68) = 0;
    actor->motion_flags = 0;
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(30);
    Audio_PlayCue(204);
    actor->motion_flags = 3;
    Event_Wait(24);
    options.palette = 7;
    options.update = (void (*)(union FieldObject *))0x200896d;
    options.start_scale_x = 0xcccc;
    options.start_scale_y = 0xcccc;
    for (i = 0; i <= 16; i++) {
        s32 angle;
        s32 velocity[3];

        angle = i << 12;
        velocity[0] = Math_Cos(angle);
        velocity[1] = 0;
        velocity[2] = Math_Sin(angle);
        velocity[0] += velocity[0] / 2;
        Func_02000b9a(actor->x.fixed, actor->y.fixed, actor->z.fixed, velocity[0], velocity[1],
                      velocity[2], 0x1090001, &options);
    }
    Audio_PlayCue(188);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x101);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 22);
    Work_SetValuesIfNonNegative(0x50000, 0x50000, 0x10000);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    MapRender_WaitForValues();
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x100);
    Func_02005ac6();
    *(s32 *)((u8 *)actor + 72) = 0x10000;
    *(s32 *)((u8 *)actor + 68) = 0x4000;
    Event_End();
}

void SceneEffect_SpawnNineRadialEffects(void)
{
    struct SceneObject *object;
    struct Vec vec;
    struct EffectParams params;
    u32 i;
    s32 v;
    s32 x;
    s32 z;

    object = Func_020059c2();
    params.unk00 = 1;
    params.mode = 7;
    params.callback = 0x0200896d;
    for (i = 0; i <= 16; i += 2) {
        v = i << 12;
        vec.x = Math_Cos(v);
        vec.y = 0;
        z = Math_Sin(v);
        x = vec.x;
        vec.z = z;
        x = x + Math_Divide(x, 3);
        vec.x = x;
        Func_02000c8a(object->x, object->y, object->z, x, vec.y, z, 0x01030001, &params);
    }
}

s32 SceneEffect_SpawnRandomizedParticleEveryFourFrames(struct SceneObject_02000b98 *obj)
{
    struct EffectParams_02000b98 params;
    s32 y;
    s32 a;
    s32 b;
    s32 rnd;

    if ((Data_03001e40 & 3) != 0) {
        return 0;
    }
    if ((u32)(Random_Next() * 6) >> 16 == 0) {
        if (obj->near != 0x80000000 || obj->far != 0x80000000) {
            Audio_PlayCue(246);
        }
    }
    y = 0;
    params.angle = 286;
    params.color1 = 0x10000;
    params.color2 = 0x10000;
    params.unk10 = -327;
    params.unk14 = -327;
    rnd = Random_Next();
    a = Math_Divide(((((u32)(rnd * 9)) >> 16) - 4) << 16, 10);
    rnd = Random_Next();
    b = Math_Divide(((((u32)(rnd * 9)) >> 16) - 4) << 16, 10);
    Func_02000d52(obj->x, obj->y, obj->z - 0x10000, a, y, b, 0x001c0001, &params);
    return 0;
}

struct EffectObject_02000c5c *SceneEffect_SpawnEffect284AtCell(s32 x, s32 z, s32 arg2)
{
    struct EffectObject_02000c5c *obj;
    s32 sx;
    s32 sz;

    sx = x << 16;
    sz = z << 16;
    obj = Object_Create(284, sx, 0, sz);
    if (obj == 0) {
        return 0;
    }
    obj->scale_x = 0x10000;
    obj->scale_y = 0x10000;
    Actor_SetSpriteFlags(obj, 0);
    Object_SetAnimation(obj, 7);
    obj->state = 0;
    obj->timer = 0;
    obj->phase = 0;
    obj->mode = 2;
    obj->callback = 0x02008b99;
    obj->flag = 0;
    Object_SetScript(obj, arg2);
    return obj;
}

s32 SceneActor_TryMoveActorZeroTwoTilesAhead(void)
{
    struct SceneObject_02000cc8 *obj;
    struct Vec vec;
    u8 *state;
    u8 old;
    s32 m;

    obj = Actor_Get(ACTOR_PARTY_LEADER);
    state = &obj->state;
    old = *state;
    vec.x = (obj->x & 0xfff00000) + 0x80000;
    vec.y = obj->y;
    vec.z = (obj->z & 0xfff00000) + 0x80000;
    m = (obj->angle + 0x2000) & 0xc000;
    Func_02005ae6(0x100000, m, &vec);
    if (Func_02005b5e(obj, &vec) != 1 && Func_0200105a(&vec, obj) == 0) {
        vec.x = (obj->x & 0xfff00000) + 0x80000;
        vec.y = obj->y;
        vec.z = (obj->z & 0xfff00000) + 0x80000;
        Func_02005b2c(0x200000, (obj->angle + 0x2000) & 0xc000, &vec);
        if (Func_02001094(&vec, obj) == 0 && Func_02005bb0(obj, &vec) == 0) {
            Event_Begin();
            Object_SetAnimation(obj, 6);
            Task_Wait(6);
            Audio_PlayCue(152);
            Object_SetAnimation(obj, 7);
            obj->scale_x = 0x30000;
            obj->scale_y = 0x20000;
            obj->accel = 0x40000;
            *state &= 0x7e;
            Actor_SetSpriteFlags(obj, 0);
            Actor_MoveToAndWait(ACTOR_PARTY_LEADER, ((union VecView *)&vec)->h[1], ((union VecView *)&vec)->h[5]);
            Object_SetAnimation(obj, 6);
            Actor_SetSpriteFlags(obj, 1);
            *state = old;
            Event_End();
            return 1;
        }
    }
    return 0;
}

/* Contiguous unnamed leaf-owner run for resource_3c8. */
s32 SceneData_SelectTableBySceneB5ToBa(void)
{
    s16 v = gGameState.scene;

    if (v == (s32)&Value_000000b5) {
        return (s32)Data_0200dd68;
    }
    if (v == (s32)&Value_000000b7) {
        return (s32)Data_0200e020;
    }
    if (v == (s32)&Value_000000b8) {
        return (s32)Data_0200e230;
    }
    if (v == (s32)&Value_000000b9) {
        return (s32)Data_0200e350;
    }
    if (v == (s32)&Value_000000ba) {
        return (s32)Data_0200e548;
    }
    return (s32)Data_0200ddc8;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetPrimaryTable(void)
{
    return Data_0200e740;
}

u8 *SceneData_SelectTableBySceneAndApply(void)
{
    extern s16 Data_02000240[];

    u8 *ret;
    s16 *tbl;
    s16 v;

    tbl = Data_02000240;
    v = tbl[224];
    if (v == (s32)(u32)&Value_000000b5) {
        return Data_0200e904;
    }
    if (v == (s32)(u32)&Value_000000b6) {
        ret = Data_0200e9c4;
    } else if (v == (s32)(u32)&Value_000000b7) {
        ret = Data_0200eb74;
    } else if (v == (s32)(u32)&Value_000000b8) {
        ret = Data_0200ec04;
    } else if (v == (s32)(u32)&Value_000000b9) {
        ret = Data_0200ec64;
    } else if (v == (s32)(u32)&Value_000000ba) {
        ret = Data_0200ecf4;
    } else {
        goto no_match;
    }
    Func_02005d78(ret);
    return ret;

no_match:
    return Data_0200e8ec;
}

void SceneState_SetFlag953(void)
{
    Event_Begin();
    Message_ShowCentered(MSG_DOOR_TIGHTLY_LOCKED, 1);
    Event_End();
}

void FieldScene_RunActorEightTenStepLoop(void)
{
    u32 n;
    u32 w;
    s32 a;
    s32 b;

    Event_Begin();
    Actor_RunRepeatedMotion(8, 3);
    Event_SetMessage(MSG_VENUS_LIGHTHOUSE_WAS_ATTACKED_BY);
    n = 10;
    w = 8;
    Event_ShowMessageAndWait(8, 0, 20);
    do {
        Actor_SetChildValue(8, 15);
        Task_Wait(2);
        Actor_SetChildValue(8, 0);
        Task_Wait(w);
        if (w > 3) {
            w--;
        }
        n--;
    } while (n != 0);
    GameFlag_Set(0x981);
    Actor_SetPosition(8, 0, 0);
    a = 7;
    b = 16;
    Map_CopyCellAttributes(7, 17, 2, 1, a, b);
    Event_End();
}

void SceneDialogue_RunActorElevenDialogue(void)
{
    Event_Begin();
    Event_SetMessage(MSG_IVE_WAITED_LONG_SEE_ITS);
    Event_ShowMessageAndWait(11, 0, 20);
    Actor_RunRepeatedMotion(11, 2);
    Event_ShowMessage(11, 0);
    Event_End();
}

void FieldScene_SetFlag987AtActorTwelveTile(void)
{
    Struct_0ff0 *s;

    s = Actor_Get(12);
    Event_Begin();
    if (s->unk8 >> 20 == 54 || s->unk10 >> 20 == 6) {
        GameFlag_Set(0x987);
    }
    Event_End();
}

void SceneDialogue_RunLine2682(void)
{
    Event_Begin();
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Message_ShowCentered(MSG_THERE_WORDS_CARVED_INTO_RELIEF, 1);
    Event_End();
}

void SceneState_ApplySixRectsAfter161(void)
{
    s32 x;
    s32 y;
    s32 a;
    s32 b;

    GameFlag_Clear(0x161);
    x = 23;
    y = 8;
    Map_CopyCellAttributes(35, 8, 1, 3, x, y);
    a = 3;
    b = 1;
    Map_CopyCellsTo(35, 8, 23, 8, b, a);
    Map_CopyCellsTo(99, 8, 87, 8, b, a);
    x = 46;
    y = 55;
    Map_CopyCellAttributes(57, 55, 3, 3, x, y);
    Map_CopyCellsTo(57, 55, 46, 55, a, a);
    Map_CopyCellsTo(121, 55, 110, 55, a, a);
}

void SceneState_ApplySixRectsAfterFlag161(void)
{
    s32 x;
    s32 y;
    s32 a;
    s32 b;

    GameFlag_Set(0x161);
    x = 23;
    y = 8;
    Map_CopyCellAttributes(36, 8, 1, 3, x, y);
    a = 3;
    b = 1;
    Map_CopyCellsTo(36, 8, 23, 8, b, a);
    Map_CopyCellsTo(100, 8, 87, 8, b, a);
    x = 46;
    y = 55;
    Map_CopyCellAttributes(53, 55, 3, 3, x, y);
    Map_CopyCellsTo(53, 55, 46, 55, a, a);
    Map_CopyCellsTo(117, 55, 110, 55, a, a);
}

static __inline__ void SetFlagBits(u8 *flags, u8 bits)
{
    *flags |= bits;
}

void FieldScene_RunLeaderSurpriseApproach(void)
{
    struct FieldActor *actor;
    s32 z;

    actor = Actor_Get(ACTOR_PARTY_LEADER);
    Event_Begin();
    Actor_EnableActionCallback(ACTOR_PARTY_LEADER, 0x200d21c);
    Func_0200602e(0);
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 6);
    actor->velocity_y = 0x40000;
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x40000, 0x20000);
    if (actor->z.fixed >> 20 <= 54) {
        Actor_Get(ACTOR_PARTY_LEADER)->unknown_5a &= 254;
        z = 210;
    } else {
        Actor_Get(ACTOR_PARTY_LEADER)->unknown_5a &= 254;
        z = 238;
    }
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, actor->x.part.pixel, z << 2);
    Event_Wait(1);
    SetFlagBits(&Actor_Get(ACTOR_PARTY_LEADER)->unknown_5a, 1);
    Event_Wait(20);
    actor->update = (void (*)(union FieldObject *))0x20085e5;
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 60);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 4);
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 4);
    actor->update = 0;
    Event_End();
}

struct EventWorkState {
    u8 unknown_000[0xcb6];
    u16 field_cb6;
    u8 unknown_cb8[2];
    u16 field_cba;
};

void FieldScene_RunStatueDialogueSequence(void)
{
    extern struct EventWorkState *Data_03001ebc;
    struct EventWorkState *work;

    work = Data_03001ebc;
    work->field_cba = 0;
    work->field_cb6 = 1;
    Event_Begin();
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Message_ShowCentered(MSG_STATUE_SPEAKS_ROBIN_SOUL_YE, 1);
    ColorBuffer_ApplySource(0x10000, 0);
    ColorBuffer_ApplyTarget(0x10005, 0);
    ColorBuffer_Interpolate(120);
    Event_Wait(100);
    Audio_PlayCue(142);
    Event_Wait(30);
    ColorBuffer_ApplyTarget(0x7fff, 0);
    ColorBuffer_Interpolate(60);
    Event_Wait(70);
    if (GameFlag_IsSet(0x982) == 0) {
        if (GameFlag_IsSet(0x983) == 0) {
            if ((Data_03001e40 & 1) != 0) {
                GameFlag_Set(0x982);
            } else {
                GameFlag_Set(0x983);
            }
        }
    }
    if (GameFlag_IsSet(0x982) == 0) {
        GameFlag_Set(0x982);
        GameFlag_Clear(0x983);
        Map_CopyCellsTo(103, 27, 89, 27, 7, 8);
        Map_CopyCellsTo(41, 90, 27, 92, 3, 2);
        Map_CopyCellsTo(41, 90, 29, 93, 3, 2);
        Map_CopyCellsTo(41, 90, 27, 94, 3, 2);
        Map_CopyCellsTo(41, 90, 27, 96, 3, 2);
        Map_CopyCellsTo(41, 90, 29, 97, 3, 2);
        Map_CopyCellsTo(41, 96, 25, 91, 3, 2);
        Map_CopyCellsTo(41, 92, 25, 93, 3, 2);
        Map_CopyCellsTo(41, 96, 25, 95, 3, 2);
        Map_CopyCellsTo(41, 96, 25, 97, 3, 2);
        Map_CopyCellsTo(41, 96, 27, 96, 3, 2);
        Map_CopyCellsTo(41, 96, 29, 97, 3, 2);
    } else {
        GameFlag_Set(0x983);
        GameFlag_Clear(0x982);
        Map_CopyCellsTo(111, 27, 89, 27, 7, 8);
        Map_CopyCellsTo(41, 90, 25, 91, 3, 2);
        Map_CopyCellsTo(41, 90, 25, 93, 3, 2);
        Map_CopyCellsTo(41, 90, 25, 95, 3, 2);
        Map_CopyCellsTo(41, 90, 25, 97, 3, 2);
        Map_CopyCellsTo(41, 90, 27, 96, 3, 2);
        Map_CopyCellsTo(41, 90, 29, 97, 3, 2);
        Map_CopyCellsTo(41, 94, 27, 92, 3, 2);
        Map_CopyCellsTo(41, 96, 29, 93, 3, 2);
        Map_CopyCellsTo(41, 94, 27, 94, 3, 2);
        Map_CopyCellsTo(41, 96, 27, 96, 3, 2);
        Map_CopyCellsTo(41, 96, 29, 97, 3, 2);
    }
    ColorBuffer_ApplyTarget(0x10000, 0);
    ColorBuffer_Interpolate(20);
    Event_Wait(40);
    Camera_SetSpeed(0x8000, 0x1000);
    Camera_MoveTo(0x1c80000, -1, 0x21e0000, 1);
    Camera_WaitForMove();
    Event_Wait(50);
    Camera_MoveTo(0x1c80000, -1, 0x1a70000, 1);
    Camera_WaitForMove();
    Event_End();
    work->field_cb6 = 0;
}

void FieldScene_RunFlag986ActorOneScene(void)
{
    Struct_A *o;
    Struct_B *u;
    s32 g;
    s32 m1;
    s32 m2;
    s32 h;
    s32 k;

    g = 0x986;
    m1 = 0xcccc;
    m2 = 0x6666;
    h = 0x100;
    k = 0x338;
    Event_Begin();
    o = Actor_Get(12);
    if (o->unk8 >> 20 == 53) {
        if (GameFlag_IsSet(g) == 0) {
            GameFlag_Set(g);
            o = Actor_Get(ACTOR_PARTY_LEADER);
            if (o != 0) {
                Actor_SetPosition(ACTOR_GERALD, o->unk8, o->unk10);
            }
            Actor_SetSpeed(ACTOR_GERALD, m1, m2);
            Actor_WalkToAndWait(ACTOR_GERALD, k, 88);
            Actor_WalkToAndWait(ACTOR_GERALD, k, 104);
            Actor_FaceEachOther(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
            Event_Wait(20);
            Actor_SetAnimationAndWait(ACTOR_GERALD, 4);
            Event_Wait(20);
            Event_SetMessage(MSG_HMMM_WE_CANT_PUSH_BLOCK);
            Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
            Actor_FaceDirection(ACTOR_GERALD, 0, 10);
            Actor_ShowEmote(ACTOR_GERALD, h, 60);
            Actor_FaceActor(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
            Event_Wait(20);
            Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
            Event_Wait(20);
            Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
            Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
            Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
            Event_Wait(30);
            Actor_WalkToAndWait(ACTOR_GERALD, k, 88);
            Actor_SetAnimation(ACTOR_GERALD, 2);
            u = Actor_Get(ACTOR_PARTY_LEADER);
            if (u != 0) {
                Actor_SetDestination(ACTOR_GERALD, u->unkA, u->unk12);
            }
            Actor_WaitForMove(ACTOR_GERALD);
            Actor_SetPosition(ACTOR_GERALD, 0, 0);
            Event_End();
        }
    }
}

void FieldScene_RunFiveCallSequence(void)
{

    Event_Begin();
    RunStagedActorTransition();
    Event_Wait(20);
    Event_End();
    FieldScene_RunFlag986ActorOneScene();
}

void SceneState_RunActor13AtColumn42Setup(void)
{
    Struct_1644 *obj;
    s32 val;
    s32 a;
    s32 b;

    obj = Actor_Get(13);
    Event_Begin();
    if (obj->unk8 >> 20 == 42) {
        Event_Wait(30);
        Audio_PlayCue(188);
        obj->unk55 = 0;
        val = 0xfffe0000;
        obj->unk14 = val;
        obj->unkC = val;
        GameFlag_Set(0x200);
        a = 3;
        b = 5;
        Map_CopyCellsTo(44, 117, 41, 117, a, b);
    }
    Event_End();
}

void Func_02001780(void)
{
    u8 *state = *(u8 **)0x03001ebc;
    {
        u16 *target = (u16 *)(state + 0xcba);
        s32 shown = 0;

        *target = shown;
    }
    {
        u16 *target = (u16 *)(state + 0xcb6);
        s32 shown = 1;

        *target = shown;
    }
    Event_Begin();
    Event_SetMessage(MSG_THOUGHT_ID_EXPLORE_AFTER_DOOR);
    Actor_FaceActor(10, ACTOR_PARTY_LEADER, 0);
    Event_Wait(10);
    Event_ShowMessageAndWait(10, 0, 20);
    SetActorDirection(10, 57344, 0);
    Camera_SetSpeed(65536, 8192);
    Camera_MoveTo(29360128, -1, 28311552, 1);
    Camera_WaitForMove();
    Event_ShowMessage(10, 0);
    Event_End();
}

void FieldScene_PlaceAndPinSlots8To10(void)
{

    u32 i;
    Struct_18f8 *rec;
    s32 x;
    s32 y;
    s32 a;
    s32 b;

    Actor_Get(8);
    Event_Begin();
    x = 12;
    y = 44;
    Map_CopyCellAttributes(19, 44, 4, 1, x, y);
    x = 11;
    y = 51;
    Map_CopyCellAttributes(17, 51, 2, 2, x, y);
    i = 0;
    do {
        rec = Actor_Get(i + 8);
        a = rec->unk8 >> 20;
        b = rec->unk10 >> 20;
        Map_CopyCellAttributes(12, 50, 1, 1, a, b);
        i++;
    } while (i <= 2);
    SceneActor_SwapPositionsByDepth(10, 9);
    Event_End();
}

void SceneState_ApplyRectAt19_44AndRunThree(void)
{
    s32 x;
    s32 y;

    Event_Begin();
    x = 12;
    y = 44;
    Map_CopyCellAttributes(19, 44, 4, 1, x, y);
    RunStagedActorTransition();
    FieldScene_PlaceAndPinSlots8To10();
    Event_End();
}

void SceneActor_ApplyKind45AtActorsElevenAndTwelve(void)
{
    u32 i;
    Struct_199c *p;

    i = 0;
    do {
        p = Actor_Get(i + 11);
        i++;
        Func_02006818(0, p->unk8, p->unk10, 45);
    } while (i <= 1);
}

void SceneActor_ApplyPositionsOfActors11And12(void)
{
    u32 i;
    Struct_19c0 *p;

    i = 0;
    do {
        p = Actor_Get(i + 11);
        if (p->unkC > -0x100000) {
            Func_02006842(0, p->unk8, p->unk10, 255);
        }
        i++;
    } while (i <= 1);
}

void FieldScene_RunGuardedThreeStepSetup(void)
{

    Event_Begin();
    if (SceneActor_TryMoveActorZeroTwoTilesAhead() == 0) {
        SceneActor_ApplyKind45AtActorsElevenAndTwelve();
        RunStagedActorTransition();
        SceneActor_ApplyPositionsOfActors11And12();
    }
    Event_End();
}

void SceneState_MarkActorAndApplyRectAtTile(Struct_1a14 *obj)
{
    s32 x;
    s32 z;

    obj->unk23 |= 2;
    obj->unk55 = 0;
    x = obj->unk8 >> 20;
    z = obj->unk10 >> 20;
    Map_CopyCellAttributes(9, 24, 1, 1, x, z);
}

void OverlayObject_ResetObjectWhenFlatbs2Set(Struct_1a50 *o)
{
    Struct_Sub *q;
    s32 v;
    s32 z;
    s32 t;
    s32 m;

    q = o->unk50;
    v = q->unk9;
    if ((v & 12) == 12) {
        m = -13;
        m &= v;
        m |= 4;
        {
            u8 *pq = &q->unk9;
            *pq = m;
        }
        z = 0;
        o->unk44 = z;
        t = OverlayObject_SpawnWithMode14(o->unk8, 0, 0x2000000, 223);
        OverlayObject_WaitUntilIdle(o);
        o->unk8 = z;
        o->unk10 = z;
        Engine_ObjectDispatchRelease(t);
    } else {
        SceneActor_ApplyPositionsOfActors11And12();
    }
}

void SceneActor_UpdateSlots11And12ByTile(void)
{
    Struct_1a9c *o;

    Event_Begin();
    o = Actor_Get(11);
    if (o->unk8 >> 20 == 8) {
        Func_02002400();
        SceneState_MarkActorAndApplyRectAtTile(o);
    } else {
        OverlayObject_ResetObjectWhenFlatbs2Set(o);
    }
    o = Actor_Get(12);
    if (o->unk8 >> 20 == 7) {
        Func_02002422();
        SceneState_MarkActorAndApplyRectAtTile(o);
    } else {
        OverlayObject_ResetObjectWhenFlatbs2Set(o);
    }
    Event_End();
}

void FieldScene_RunGuardedRectStep(void)
{
    s32 x;
    s32 y;

    Event_Begin();
    if (SceneActor_TryMoveActorZeroTwoTilesAhead() == 0) {
        x = 45;
        y = 43;
        Map_CopyCellAttributes(109, 43, 7, 5, x, y);
        RunStagedActorTransition();
    }
    Event_End();
    Func_02003c9e();
}

void SceneState_RunConditionalStep(void)
{
    Event_Begin();
    if (SceneActor_TryMoveActorZeroTwoTilesAhead() == 0) {
        s32 k5 = 44, k6 = 39;
        Map_CopyCellAttributes(108, 39, 13, 7, k5, k6);
        RunStagedActorTransition();
    }
    Event_End();
    Func_020041fa();
}

s32 SceneActor_SetHeightAboveLinkedRecord(Struct_22a4 *obj)
{
    Struct_22a4b *rec;

    rec = Actor_Get(((s16 *)obj)[50]);
    ((s32 *)obj)[3] = rec->unkC + 0x100000;
    return 0;
}

/*
 * resource_3c8 owner at 0x020023d4, 168 bytes: among scene slots 8-13,
 * locate candidates sharing the selected slot's x/z tile, retain the highest
 * candidate at least one 16.16 unit above its y value, store that candidate id
 * at selected+100, then move/release the selected slot and run its local effect.
 *
 * Complete owner: high-register prologue and four-byte frame at 0x020023d4
 * through the sole interworking return at 0x02002468-0x02002474, followed by
 * alignment and one referenced pool word through 0x0200247b.  Eight static
 * calls across seven targets match independently; the two scene-accessor call
 * sites sit inside the bounded six-iteration loop.
 */
void SceneActor_PickHighestSlotAtSameTileAndRelease(s32 selector)
{
    void Func_02000b08();

    u8 *cand;
    s32 highest = (s32)0xffb00000;
    u8 *sel = 0;
    u32 i;

    for (i = 0; i <= 5; i++) {
        s32 no = i + 8;
        s32 cand_y;

        if (no == selector) {
            continue;
        }

        cand = Actor_Get(no);
        sel = Actor_Get(selector);

        if ((*(s32 *)(cand + 8) >> 20)
                != (*(s32 *)(sel + 8) >> 20)
            || (*(s32 *)(cand + 16) >> 20)
                != (*(s32 *)(sel + 16) >> 20)) {
            continue;
        }

        cand_y = *(s32 *)(cand + 12) + 0x100000;
        if (highest <= cand_y) {
            *(u16 *)(sel + 100) = (u16)no;
            highest = cand_y;
        }
    }

    Actor_SetSpeed(selector, 0x40000, 0x20000);
    Func_02007252(sel,
                  *(s32 *)(sel + 8),
                  highest,
                  *(s32 *)(sel + 16));
    Actor_WaitForMove(selector);
    Audio_PlayCue(188);
    SceneEffect_SpawnNineRadialEffects(selector);
    Event_Wait(30);
}

void FieldScene_RunThreeCallSequence(void)
{

    Event_Begin();
    RunStagedActorTransition();
    Event_End();
}

void FieldScene_DrawTilesWhenCheckClear(void)
{

    Event_Begin();
    if (SceneActor_TryMoveActorZeroTwoTilesAhead() == 0) {
        { s32 k5 = 5, k6 = 48; Map_CopyCellAttributes(69, 48, 4, 2, k5, k6); }
        { s32 j5 = 9, j6 = 37; Map_CopyCellAttributes(73, 37, 9, 13, j5, j6); }
        RunStagedActorTransition();
    }
    Event_End();
    Func_02005a3a();
}

/* Runs a fixed sequence of setup calls with literal parameters; most share
 * a leading 0 argument. */
void FieldScene_RunApproachAndSpawnEffect(void)
{
    Event_Begin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x208, 0x2c8);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 10);
    (void)OverlayObject_SpawnWithMode14(0x2080000, 0, 0x3100000, 223);
    BattleEffect_RunRisingObjectSequence_1(0, 6, 0);
    Event_Wait(60);
    Event_RequestExit(20); /* main:0808a248 */
    Event_End();
}

u8 *SceneData_SelectTableBySceneId(void)
{
    extern u8 Data_02000240[];

    s32 off = 0x1c0;
    s32 v = *(s16 *)(Data_02000240 + off);

    if (v == (s32)&Value_000000b5) {
        return Data_0200ee44;
    }
    if (v == (s32)&Value_000000b6) {
        return Data_0200ef1c;
    }
    if (v == (s32)&Value_000000b7) {
        return Data_0200f120;
    }
    if (v == (s32)&Value_000000b8) {
        return Data_0200f300;
    }
    if (v == (s32)&Value_000000b9) {
        return Data_0200f3b4;
    }
    if (v == (s32)&Value_000000ba) {
        return Data_0200f4f8;
    }
    return Data_0200ef1c;
}

void SceneActor_ClearActorModeAndSetState5(s32 no)
{
    u8 *p;
    s32 mask;

    p = Actor_Get(no);
    p[0x55] = 0;
    mask = 252;
    mask &= p[0x59];
    p[0x59] = mask;
    Actor_SetSpriteFlags(p, 0);
    Object_SetAnimation(p, 5);
    Actor_SetSpritePriority(no, 3);
    {
        s32 v = 2;
        v |= p[0x23];
        p[0x23] = v;
    }
}

void SceneState_ApplyStepToSlots15To18(void)
{
    u32 i;

    i = 15;
    do {
        Actor_Get(i);
        i++;
    } while (i <= 18);
}

void SceneState_PassZeroAndMinusOneRecord(void)
{
    struct Args_02003fe4 args;

    args.first = 0;
    args.last = -1;
    Func_0200803c(&args);
}

void SceneState_PassRange0To1(void)
{
    struct Args_02003ffc args;

    args.first = 0;
    args.last = 1;
    Func_02008054(&args);
}

void SceneState_PassRangeNeg1To0(void)
{
    struct Args_02004014 args;

    args.first = -1;
    args.last = 0;
    Func_0200806e(&args);
}

void SceneState_CallHandlerWithFlagPair(void)
{
    struct Args_02004030 args;

    args.first = 1;
    args.last = 0;
    Func_02008088(&args);
}

/*
 * Func_02008b1a is the relocated call word for the in-overlay routine at
 * image offset 0x4520, not a runtime address.  The 16-byte owner loads no
 * literal, so it carries no pool word and no alignment halfword.  432 is
 * built from a shifted immediate and passed straight to the callee as a
 * value, not used as a displacement.
 */
void SceneState_CallWith432And32(void)
{
    Func_02008b1a(432, 32);
}

void FieldScene_CallWith560And44(void)
{
    Func_02008b2a(0x230, 44);
}

void FieldScene_RunLeaderDropSequence(void)
{
    u32 i;
    u8 *p8;
    s32 rec;
    u8 *rec8;
    s32 record;
    s32 none;
    s32 v2;
    s32 slot0;

    rec = Value1(Engine_ActorGet, ACTOR_PARTY_LEADER);
    rec8 = Value1(Engine_ActorGet, 20);
    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Map_Redraw();
    Task_Wait(1);
    *(s32 *)(rec + 12) = 0x820000;
    *(s32 *)(rec + 72) = 0x8000;
    none = 0;
    *(s32 *)(rec + 68) = none;
    p8 = rec + 85;
    *p8 = none;
    Event_OpenScreen();
    Event_WaitForScreen();
    Audio_PlayCue(204);
    Event_Wait(30);
    *p8 = 3;
    Event_Wait(24);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x101);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 22);
    *p8 &= 254;
    *(s32 *)((s32)rec8 + 12) += -0x30000;
    *(s32 *)(rec + 12) += -0x30000;
    *(s32 *)(rec + 20) += -0x30000;
    Task_Wait(2);
    *(s32 *)((s32)rec8 + 12) += -0x20000;
    *(s32 *)(rec + 12) += -0x20000;
    *(s32 *)(rec + 20) += -0x20000;
    Task_Wait(10);
    *(s32 *)((s32)rec8 + 12) += 0x20000;
    *(s32 *)(rec + 12) += 0x20000;
    *(s32 *)(rec + 20) += 0x20000;
    Task_Wait(4);
    *(s32 *)((s32)rec8 + 12) += 0x20000;
    *(s32 *)(rec + 12) += 0x20000;
    *(s32 *)(rec + 20) += 0x20000;
    Task_Wait(4);
    *(s32 *)((s32)rec8 + 12) += 0x10000;
    *(s32 *)(rec + 12) += 0x10000;
    *(s32 *)(rec + 20) += 0x10000;
    *p8 = none;
    rec8[85] = none;
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x100);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Event_Wait(40);
    *(s32 *)(rec + 108) = 0x200c969;
    Event_Wait(60);
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 1);
    Actor_SetSpritePriority(20, 1);
    Audio_PlayCue(17);
    Audio_PlayCue(0x134);
    GameFlag_Set(0x101);
    v2 = 0;
    do {
        *(s32 *)(rec + 12) += 0x10000;
        *(s32 *)(rec + 20) += 0x10000;
        *(s32 *)((s32)rec8 + 12) += 0x10000;
        slot0 = v2;
        Task_Wait(1);
        v2 = slot0;
        v2 = (v2 + 1);
    } while ((u32)v2 <= 127);
    Event_RequestExit(21);
}
