#include "types.h"
#include "scene.h"

#include "overlay_object.h"
#include "staged_actor.h"

/*
 * Effect spawning for overlay resource_39f.  Every cutscene beat in the
 * overlay creates its effects through this routine.
 */
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
    u8 flags23;
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

struct Options {
    u8 mode_bits;
    u8 pad01[3];
    s32 mode;
    s32 accum18;
    s32 accum1c;
    s32 target30;
    s32 target34;
    s16 kind;
    u16 pad1a;
    s32 callback_arg;
    u16 angle;
    u16 step;
    u32 callback;
};

struct Descriptor {
    s32 pad00[3];
    s32 dur;
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

struct Args {
    s32 first;
    s32 unused;
    s32 last;
};

struct Args2 {
    s32 first;
    s32 unused;
    s32 last;
};

struct Args3 {
    s32 first;
    s32 unused;
    s32 last;
};

struct Args4 {
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

struct Object { u8 unknown_00[8]; s32 x; s32 y; s32 z; };

struct EffectParams { s32 unk00; s32 mode; s32 color1; s32 color2; u8 unknown_10[0x18]; };

struct Object2 {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
};

struct EffectParams2 {
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

struct SceneObject {
    u8 filler00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 filler14[0x24];
    s32 near;
    u8 filler3c[4];
    s32 far;
};

struct EffectParams3 {
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

struct EffectObject {
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

struct Actor { u8 unknown_00[8]; s32 x; s32 y; s32 z; u8 unknown_14[0xf]; u8 flags23; };

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
struct Sprite {
    u8 pad00[9];
    u8 flags9;
    u8 pad0a[20];
    u16 angle;
    u8 pad20[6];
    u8 state26;
};

struct Effect {
    u8 pad00[8];
    s32 position[3];
    u8 pad14[4];
    s32 accum18;
    s32 accum1c;
    u8 pad20[3];
    u8 flags23;
    u8 pad24[12];
    s32 rate30;
    s32 rate34;
    u8 pad38[12];
    s32 velocity[3];
    struct Sprite *sprite;
    u8 pad54;
    u8 mode55;
    u8 pad56[14];
    u16 step64;
    u8 pad66[6];
    u32 callback;
};

struct SceneObject2 {
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

extern struct Descriptor *gOv[];
extern u32 StagedActorDirectionSteps[];
extern u8 *gCam;
extern u8 gUnk[];
extern u8 Value_00000212;
extern u32 gIw;
extern u8 Value_00000986;
extern u8 Value_000000b5;
extern u8 Value_000000b7;
extern u8 Value_000000b8;
extern u8 Value_000000b9;
extern u8 Value_000000ba;
extern u8 gOv2[];
extern u8 gOv3[];
extern u8 gOv4[];
extern u8 gOv5[];
extern u8 gOv6[];
extern u8 gOv7[];
extern u8 gOv8[];
extern u8 Value_000000b6;
extern u8 gOv9[];
extern u8 gOv10[];
extern u8 gOv11[];
extern u8 gOv12[];
extern u8 gOv13[];
extern u8 gOv14[];
extern u8 gOv15[];
extern u8 gOv16[];
extern u8 gOv17[];
extern u8 gOv18[];
extern u8 gOv19[];
extern u8 gOv20[];
extern u8 gOv21[];

void *Field_unk16_4(s32, s32, s32, s32);

void *Field_unk17_4(s32, s32, s32, s32);
struct Effect *Field_unk18_4();
struct Effect *Field_unk19_4();

typedef s32(*IwramSqrt020002f0)(s32);
u8 *Field_unk20_4();
u8 *Field_unk21_4();

struct StagedActor *GetStagedActor(s32 arg0);
struct StagedActor *FindActorAtPosition(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *FindActorAtForwardPosition(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *FindActorAbovePosition(s32 *arg0, struct StagedActor *arg1);

struct EffectObject *Field_unk22_4(s32);

struct SceneObject *Field_unk23_4(void);

struct EffectObject *Field_unk24_4(s32, s32, s32, s32);

u8 *Scene_GetRecord_1();

u8 *Scene_GetRecord_2();

u8 *Motion_SetHPosTerrain_1();

u8 *Scene_GetRecord_3();

u8 *Field_unk25_4();

Struct_A *Field_unk26_4(s32);

Struct_A *Field_unk27_4(s32);

Struct_B *Field_unk28_4(s32);

struct Actor *Field_unk29_4();
struct Position *Field_unk30_4(s32);
struct Position *Field_unk31_4(s32);

Struct_0ff0 *Field_unk32_4(s32);

Struct_199c *Field_unk33_4(s32);

Struct_19c0 *Field_unk34_4(s32);

Struct_1a9c *Field_unk35_4(s32);

Struct_1a9c *Field_unk36_4(s32);

Struct_22a4b *Field_unk37_4(s32);

struct SceneObject *Field_unk38_4(s32);

Struct_1644 *Field_unk39_4();
Struct_1644 *Field_unk40_4();

Struct_18f8 *Field_unk41_4(s32);

u8 *Field_unk42_4(s32);

void ConfigureOverlayObject(struct OverlayObject *object, s32 parameter)
{
    object->unknown_55 = 0;
    object->unknown_59 = 8;
    RunOverlayObjectCommand0(object, 0);
    RunOverlayObjectCommand1(object, parameter);
}

void *OvObj_SpawnWithMode14(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    void *obj;
    u8 *p;
    s32 mask;
    u8 flag;

    obj = Field_unk16_4(arg3, arg0, arg1, arg2);
    if (obj != 0) {
        p = *(u8 **)((u8 *)obj + 0x50);
        mask = 13;
        flag = p[9];
        mask = -mask;
        mask &= flag;
        p[9] = mask;
        Field_Apply(obj, 0xE);
        Field_Apply2(obj, 1);
        return obj;
    }
    return 0;
}

void *OvObj_PrepareObjectWithCommand15(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    void *object;
    u8 *rec;
    s32 flags;
    s32 result;

    object = Field_unk17_4(arg3, arg0, arg1, arg2);
    if (object != 0) {
        rec = *(u8 **)((u8 *)object + 0x50);
        flags = rec[9];
        flags = (flags & -13) | 4;
        rec[9] = flags;
        Field_Apply3(object, 0xF);
        flags = *((u8 *)object + 0x23);
        result = 2;
        result |= flags;
        *((u8 *)object + 0x23) = result;
        return object;
    }
    return 0;
}

void OvObj_IntegrateVelocities(void *arg0)
{
    u8 *a = arg0;

    *(volatile s32 *)(a + 0x08) += *(s32 *)(a + 0x44);
    *(volatile s32 *)(a + 0x0C) += *(s32 *)(a + 0x48);
    *(volatile s32 *)(a + 0x10) += *(s32 *)(a + 0x4C);
    *(volatile s32 *)(a + 0x18) += *(s32 *)(a + 0x30);
    *(volatile s32 *)(a + 0x1C) += *(s32 *)(a + 0x34);
    *(volatile u16 *)(*(u8 **)(a + 0x50) + 0x1E) += *(u16 *)(a + 0x64);
}

/* Returns the party record; only its presentation block at +80 is read. */

/*
 * Creates an effect at (x, y, z) with the constant velocity (vx, vy, vz) that
 * the per-frame integrator applies, then sets whichever optional fields
 * `flags` selects from `options`.  The 472-byte owner includes its three pool
 * words 0x0200b058, 0x020080e1 and 0xffff0000.
 */
void Effect_SpawnConfigured(s32 x, s32 y,
                   s32 z, s32 vx, s32 vy, s32 vz, u32 flags,
                   const struct Options *options)
{
    u32 off;
    struct Effect *party;
    u32 bits;
    s32 mask;
    u32 tmp;
    struct Effect *effect;
    struct Sprite *block;
    struct Sprite *blk;
    u32 opt;
    u16 *tag;
    s32 dur;
    s32 delta0;
    s32 acc;
    party = Field_unk18_4(0);

    /* 128 << 13.  With this bit set and an options block present the effect's
     * kind comes from the options rather than from the default 222. */
    if ((flags & 0x100000) != 0 && options != 0) {
        effect = Field_unk19_4(options->kind, x, y, z);
    } else {
        effect = Field_unk19_4(222, x, y, z);
    }
    if (effect == 0) return;

    block = effect->sprite;
    blk = block;

    Field_unk43_4(effect, (flags + 1) & 15);
    off = (flags & 15) << 2;
    Field_unk44_4(effect, gOv[off >> 2]);

    effect->mode55 = 0;
    block->state26 = 0;

    /* 0x020080e1 is the per-frame integrator's loader-relocated call word,
     * not a runtime address. */
    effect->callback = 0x020080e1;

    effect->velocity_x = vx;
    x = 3;
    effect->velocity_y = vy;
    effect->velocity_z = vz;

    /* Bits 2 and 3 of the effect's mode byte are copied from the party's. */
    bits = party->sprite->flags9 & 12;
    tmp = *(volatile u8 *)&block->flags9;
    mask = ~12;
    block->flags9 = (u8)((tmp & mask) | bits);

    effect->rate30 = 0;
    effect->rate34 = 0;
    effect->step64 = 0;
    tag = &effect->step64;

    /* The rest is optional detail, skipped unless a high flag bit is set and
     * an options record was supplied. */
    if ((flags & 0xffff0000) == 0 || options == 0) return;

    if ((flags & 0x10000) != 0) {                   /* 128 << 9 */
        Field_unk45_4(effect, options->mode);
    }

    if ((flags & 0x20000) != 0) {                   /* 128 << 10 */
        effect->flags23 &= 0xfe;
        opt = *(const u8 *)options & x;
        block->flags9 = (u8)((*((const u8 *)blk + 9) & mask)
                             | (opt << 2));
    }

    if ((flags & 0x80000) != 0) {                   /* 128 << 12 */
        effect->accum18 = options->accum18;
        effect->accum1c = options->accum1c;
    }

    if ((flags & 0x40000) != 0) {                   /* 128 << 11 */
        const struct Descriptor *desc =
            gOv[off >> 2];
        s32 delta;

        /* This 0x80000 test reads the register the previous block left live
         * and must not be respelled as a fresh load of `flags`.  With a
         * destination supplied the step is measured from it, otherwise the
         * target is biased by -1.0 in 16.16. */
        if ((flags & 0x80000) != 0) {
            delta0 = *(volatile const s32 *)&options->target30;
            acc = *(volatile const s32 *)&effect->accum18;
            delta0 -= acc;
            effect->rate30 = Field_unk8(delta0,
                                           desc->dur);
            delta = options->target34;
            dur = desc->dur;
            delta -= effect->accum1c;
        } else {
            delta0 = options->target30;
            delta0 += (s32)0xffff0000;
            effect->rate30 = Field_unk9(delta0,
                                           desc->dur);
            delta = options->target34;
            dur = desc->dur;
            delta += (s32)0xffff0000;
        }

        /* Only the first call is per-arm.  Both arms join onto one second
         * call site, so each arm computes its own delta and the call is
         * spelled once here. */
        effect->rate34 = Field_unk10(delta, dur);
    }

    if ((flags & 0x200000) != 0) {                  /* 128 << 14 */
        Field_unk46_4(effect, 1);
        Field_unk47_4(effect, options->callback_arg);
    }

    if ((flags & 0x400000) != 0) {                  /* 128 << 15 */
        block->angle = options->angle;
    }

    if ((flags & 0x800000) != 0) {                  /* 128 << 16 */
        *tag = options->step;
    }

    if ((flags & 0x1000000) != 0) {                 /* 128 << 17 */
        effect->callback = options->callback;
    }
}

s32 Actor_CalculateFixedPointDistance(s32 *a, s32 *b)
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
 * `_call_via_rN` bank; Field_unk48_4 is correct for it, unchanged.
 */
s32 *SceneData_FindSlotAtPosition(s32 *pos)
{
    extern u8 *gWork;
    void Effect_SpawnNineRadialEffects();

    s32 **slots = (s32 **)(gWork + 0x14);
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

    leader = GetStagedActor(0);
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

    SetStagedActorMode(leader, 8);
    move_rate = 0x3333;
    WaitSceneFrames(15);
    PlaySoundCue(185);
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
    UpdateLeadActorState(leader, 1);
}

s32 State_FillGridCellByte2(u32 no, s32 x, s32 y, u32 w, u32 h, s32 val)
{
    u8 *g = gCam;
    u8 *base;
    u32 i;
    u32 j;

    if (g != 0) {
        if (no <= 2) {
            u32 off = no * 48 + 304;

            base = *(u8 **)(g + off);
        } else {
            base = gUnk;
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
void Actor_ApplySlotsMatchingKind212(void)
{
    extern u8 *gWork;

    s32 **slots = (s32 **)(gWork + 0x14);
    u32 i;
    s32 lim = (s32)&Value_00000212;

    for (i = 8; i <= 65; i++) {
        s32 *p = slots[i];
        u32 h = *(u16 *)((u8 *)p + 100);
        s32 t = h << 16;

        if ((t >> 20) == lim) {
            s32 m = 15;
            m &= h;
            Field_Apply4(p, m);
        }
    }
}

s32 OvObj_ApplyLowNibbleOfField100(void *obj)
{
    Field_Apply5(obj, *(u16 *)((u8 *)obj + 100) & 15);
    return 0;
}

s32 OvObj_UpdateEveryFourFrames(void *obj)
{
    if ((*(u32 *)0x03001e40 & 3) == 0)
        Field_Apply6(obj, 7);
    else
        Field_Apply7(obj, 0);

    if ((*(u32 *)0x03001e40 & 7) == 0)
        Field_unk9_2(138);
    return 0;
}

s32 Effect_SpawnRandomEveryEightFramesB(struct Object *object)
{
    struct EffectParams params;
    s32 phase, x, y, speed;
    phase = *(u32 *)0x03001e40 & 7;
    if (phase != 0) goto done;
    params.unk00 = 3 - (s32)((u32)(Field_unk11() * 2) >> 16);
    params.color1 = 0x6666;
    params.color2 = 0x6666;
    params.mode = 14;
    x = object->x + (((s32)((u32)(Field_unk12() * 9) >> 16) - 4) << 16);
    y = object->y + ((32 - (s32)((u32)(Field_unk13() * 32) >> 16)) << 16);
    speed = Field_unk14(((s32)((u32)(Field_unk15() * 5) >> 16) << 16) + 0x00050000, 10);
    Field_unk49_4(x, y, object->z, 0, speed, phase, 0x000b0000, &params);
done:
    return 0;
}

s32 OvObj_ApplyZero(void *obj)
{
    Field_Apply8(obj, 0);
    return 0;
}

s32 Effect_SpawnTwoRandomizedParticles(struct Object2 *obj)
{
    struct EffectParams2 params;
    s32 speed;
    s32 phase;

    params.color1 = 0x0000cccc;
    params.color2 = 0x0000cccc;
    params.unk00 = 0;

    speed = (s32)((u32)(Field_unk16() * 8) >> 16) * 0x3333;
    phase = gIw & 15;

    Field_unk17(
        obj->x + ((8 - phase) << 16),
        obj->y + 0x001a0000,
        obj->z,
        0,
        -speed,
        0,
        0x000a0000,
        &params);

    phase = gIw & 15;
    if (phase == 0) {
        params.color1 = 0x00008000;
        params.color2 = 0x00008000;
        Field_unk18(
            obj->x
                + (((s32)((u32)(Field_unk19() * 9) >> 16) - 4) << 16),
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

void Effect_RunObjectZeroColorSequence(void)
{
    struct EffectObject *obj;
    u8 *state;

    state = *(u8 **)0x03001ebc;
    obj = Field_unk22_4(0);
    Field_unk50_4();
    Field_unk10_2(228);
    obj->callback = 0x020086a1;
    obj->color = 0x3333;
    Field_Apply9(0, 2);
    Field_unk5_3(0, 0, -6);
    Field_unk11_2(0);
    Field_Apply10(0, 15);
    Field_Apply11(Field_unk20(0), 0);
    obj->callback = 0;
    Field_unk12_2(30);
    Field_unk51_4();
    Field_unk52_4();
    Field_unk13_2(*(s16 *)(state + 0x16c));
    Field_unk53_4();
}

/* Runs a guarded one-shot setup on an entry record: only fires the first
 * time (while a global 0x109 lookup is still unset), positions the entry
 * from its own stored coordinates, drives an effect/param sequence, then
 * writes a stage byte and an override field on the entry before returning. */
void Scene_RunOpeningAuxiliarySequence(void)
{
    extern u8 gWork[];

    u32 i;
    u8 *entry;
    u8 *guard;
    u8 *sub;

    entry = Scene_GetRecord_1(0);
    guard = GameFlag_IsSet_1(0x109);
    if (guard == 0) {
        Battle_Reset_1();
        Motion_CamBounds_1(-1, -1, -1, 0);
        /* Stage byte at +85 of the entry record. */
        entry[85] = guard;
        /* Position, from the entry's own s16 coordinates at +10/+18
         * (converted to 16.16 fixed point; the y term is offset by -16.0). */
        Motion_SetHPosTerrain_1(0, (*(s16 *)(entry + 10) << 16), ((*(s16 *)(entry + 18) << 16) + -0x100000));
        ObjectGroup_ConfigureChildValue_1(0, 15);
        sub = Scene_GetRecord_2(0);
        Field_unk54_4(sub, 0);
        Battle_WaitMode0_1();
        Motion_SetSpeed_1();
        Audio_PlayCue_1(228);
        /* Override field at +108 of the entry record; holds an EWRAM
         * address while the effect sequence below runs. */
        *(s32 *)(entry + 108) = 0x20086a1;
        Motion_SetSpeed_2(0, 0x6666, 0x3333);
        Motion_CommitPositionAndActivate_1(0, 0, 8);
        ObjectGroup_ConfigureChildValue_2(0, 0);
        sub = Scene_GetRecord_3(0);
        Field_unk55_4(sub, 1);
        Motion_CommitPositionAndActivate_2(0, 0, 8);
        entry[85] = 3;
        /* Restore the +108 override field to the original (unset) value. */
        *(s32 *)(entry + 108) = guard;
        BattleFx_PlayQueuedSound_1();
        Battle_SchedShoulder_1();
    }
}

void State_StoreLookupZeroToWord24(void)
{
    extern u8 *gWork;

    *(s32 *)(*(u8 **)0x03001ee0 + 24) = Field_unk21(0);
}

void State_ClearWorkspaceWord24(void)
{
    extern u8 *gWork;

    *(s32 *)(*(u8 **)0x03001ee0 + 24) = 0;
}

s32 Actor_SetFlagBitByRelativeDepth(struct Actor *actor)
{
    struct Actor *ref;
    u8 *fp;
    u8 flags;
    ref = Field_unk29_4(0);
    fp = &actor->flags23;
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

void Actor_SwapPositionsByDepth(s32 group, s32 index)
{
    struct Position *first;
    struct Position *second;
    s32 value;

    first = Field_unk30_4(group);
    second = Field_unk31_4(index);
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
        Field_unk14_2(1);
    }
}

void OvObj_WaitUntilIdle(struct BusyObject *obj)
{
    s32 cnt;
    s32 busy;

    cnt = 60;
    while (cnt != 0) {
        Field_unk15_2(1);
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
void Field_unk56_4(struct Effect *effect)
{
    s32 velocity_z;
    struct Sprite *sprite;
    s32 velocity_x;

    /* The block keeps the Z load after the Y store. */
    do {
        velocity_x = effect->velocity[0];
        effect->position[0] += velocity_x;
        effect->position[1] += effect->velocity[1];
    } while (0);
    velocity_z = effect->velocity[2];
    effect->position[2] += velocity_z;

    effect->velocity[0] = velocity_x - Field_unk22(velocity_x, 18);
    effect->velocity[2] = velocity_z - velocity_z / 16;

    effect->accum18 += effect->rate30;
    effect->accum1c += effect->rate34;

    sprite = effect->sprite;
    sprite->angle += effect->step64;
}

void Scene_RunSupplementalSequenceOne(s32 a0)
{
    extern u8 gWork[];

    s32 rec;
    s32 v8;
    s32 sh;
    s32 record;
    s32 half;
    s32 w0;
    s32 w1;
    s32 w2;
    s32 w4;
    u8 *p10;
    u8 *p16;
    u8 slot28[40];
    u8 slot16[12];

    rec = Field_Check(0);
    Field_unk57_4();
    Field_Run(-1, -1, -1, 0);
    Field_unk58_4();
    Field_unk59_4(1);
    *(volatile s32 *)(rec + 12) = 0x820000;
    *(volatile s32 *)(rec + 72) = 0x8000;
    *(volatile s32 *)(rec + 68) = 0;
    *(volatile u8 *)(rec + 85) = 0;
    Field_unk60_4();
    Field_unk61_4();
    Field_unk62_4(30);
    Field_unk63_4(204);
    *(volatile u8 *)(rec + 85) = 3;
    Field_unk64_4(24);
    p10 = slot28;
    *(s32 *)(p10 + 4) = 7;
    *(s32 *)(p10 + 36) = 0x200896d;
    *(s32 *)(p10 + 8) = 0xcccc;
    *(s32 *)(p10 + 12) = 0xcccc;
    v8 = 0;
    p16 = slot16;
    do {
        sh = v8 << 12;
        *(s32 *)(p16) = Field_unk23(sh);
        *(s32 *)(p16 + 4) = 0;
        record = Field_unk24(sh);
        *(s32 *)(p16 + 8) = record;
        half = *(s32 *)p16;
        *(s32 *)(p16) = half + half / 2;
        w0 = *(volatile s32 *)(rec + 8);
        w2 = *(volatile s32 *)(rec + 16);
        w1 = *(volatile s32 *)(rec + 12);
        w4 = *(volatile s32 *)(p16 + 4);
        Field_unk65_4(w0, w1, w2, (*(s32 *)p16), w4, record, 0x1090001, (s32)p10);
        v8 = (v8 + 1);
    } while ((u32)v8 <= 16);
    Field_unk66_4(188);
    Field_unk2_4(0, 0x101);
    Field_unk67_4(0, 22);
    Field_Place(0x50000, 0x50000, 0x10000);
    Field_unk2_3(-1, -1, 0xe666);
    Field_unk68_4();
    Field_unk3_4(0, 0x100);
    Field_unk69_4();
    *(volatile s32 *)(rec + 72) = 0x10000;
    *(volatile s32 *)(rec + 68) = 0x4000;
    Field_unk70_4();
}

void Effect_SpawnNineRadialEffects(void)
{
    struct SceneObject *object;
    struct Vec vec;
    struct EffectParams params;
    u32 i;
    s32 v;
    s32 x;
    s32 z;

    object = Field_unk23_4();
    params.unk00 = 1;
    params.mode = 7;
    params.callback = 0x0200896d;
    for (i = 0; i <= 16; i += 2) {
        v = i << 12;
        vec.x = Field_unk25(v);
        vec.y = 0;
        z = Field_unk26(v);
        x = vec.x;
        vec.z = z;
        x = x + Field_Apply12(x, 3);
        vec.x = x;
        Field_unk25_5(object->x, object->y, object->z, x, vec.y, z, 0x01030001, &params);
    }
}

s32 Effect_SpawnRandomizedParticleEveryFourFrames(struct SceneObject *obj)
{
    struct EffectParams3 params;
    s32 y;
    s32 a;
    s32 b;
    s32 rnd;

    if ((gIw & 3) != 0) {
        return 0;
    }
    if ((u32)(Field_unk27() * 6) >> 16 == 0) {
        if (obj->near != 0x80000000 || obj->far != 0x80000000) {
            Field_unk16_2(246);
        }
    }
    y = 0;
    params.angle = 286;
    params.color1 = 0x10000;
    params.color2 = 0x10000;
    params.unk10 = -327;
    params.unk14 = -327;
    rnd = Field_unk28();
    a = Field_Apply13(((((u32)(rnd * 9)) >> 16) - 4) << 16, 10);
    rnd = Field_unk29();
    b = Field_Apply14(((((u32)(rnd * 9)) >> 16) - 4) << 16, 10);
    Field_unk26_5(obj->x, obj->y, obj->z - 0x10000, a, y, b, 0x001c0001, &params);
    return 0;
}

struct EffectObject *Effect_SpawnEffect284AtCell(s32 x, s32 z, s32 arg2)
{
    struct EffectObject *obj;
    s32 sx;
    s32 sz;

    sx = x << 16;
    sz = z << 16;
    obj = Field_unk24_4(284, sx, 0, sz);
    if (obj == 0) {
        return 0;
    }
    obj->scale_x = 0x10000;
    obj->scale_y = 0x10000;
    Field_Apply15(obj, 0);
    Field_Apply16(obj, 7);
    obj->state = 0;
    obj->timer = 0;
    obj->phase = 0;
    obj->mode = 2;
    obj->callback = 0x02008b99;
    obj->flag = 0;
    Field_Apply17(obj, arg2);
    return obj;
}

s32 Actor_TryMoveActorZeroTwoTilesAhead(void)
{
    struct SceneObject2 *obj;
    struct Vec vec;
    u8 *state;
    u8 old;
    s32 m;

    obj = Field_unk38_4(0);
    state = &obj->state;
    old = *state;
    vec.x = (obj->x & 0xfff00000) + 0x80000;
    vec.y = obj->y;
    vec.z = (obj->z & 0xfff00000) + 0x80000;
    m = (obj->angle + 0x2000) & 0xc000;
    Field_unk6_3(0x100000, m, &vec);
    if (Field_Apply18(obj, &vec) != 1 && Field_Apply19(&vec, obj) == 0) {
        vec.x = (obj->x & 0xfff00000) + 0x80000;
        vec.y = obj->y;
        vec.z = (obj->z & 0xfff00000) + 0x80000;
        Field_unk7_3(0x200000, (obj->angle + 0x2000) & 0xc000, &vec);
        if (Field_Apply20(&vec, obj) == 0 && Field_Apply21(obj, &vec) == 0) {
            Field_unk71_4();
            Field_Apply22(obj, 6);
            Field_unk17_2(6);
            Field_unk18_2(152);
            Field_Apply23(obj, 7);
            obj->scale_x = 0x30000;
            obj->scale_y = 0x20000;
            obj->accel = 0x40000;
            *state &= 0x7e;
            Field_Apply24(obj, 0);
            Field_unk8_3(0, ((union VecView *)&vec)->h[1], ((union VecView *)&vec)->h[5]);
            Field_Apply25(obj, 6);
            Field_Apply26(obj, 1);
            *state = old;
            Field_unk72_4();
            return 1;
        }
    }
    return 0;
}

/* Contiguous unnamed leaf-owner run for resource_3c8. */
s32 SceneData_SelectTableBySceneB5ToBa(void)
{
    extern s16 gCell[];

    s16 v = gCell[224];

    if (v == (s32)&Value_000000b5) {
        return (s32)gOv2;
    }
    if (v == (s32)&Value_000000b7) {
        return (s32)gOv3;
    }
    if (v == (s32)&Value_000000b8) {
        return (s32)gOv4;
    }
    if (v == (s32)&Value_000000b9) {
        return (s32)gOv5;
    }
    if (v == (s32)&Value_000000ba) {
        return (s32)gOv6;
    }
    return (s32)gOv7;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetTablee740(void)
{
    return gOv8;
}

u8 *SceneData_SelectTableBySceneAndApply(void)
{
    extern s16 gCell[];

    u8 *ret;
    s16 *tbl;
    s16 v;

    tbl = gCell;
    v = tbl[224];
    if (v == (s32)(u32)&Value_000000b5) {
        return gOv10;
    }
    if (v == (s32)(u32)&Value_000000b6) {
        ret = gOv11;
    } else if (v == (s32)(u32)&Value_000000b7) {
        ret = gOv12;
    } else if (v == (s32)(u32)&Value_000000b8) {
        ret = gOv13;
    } else if (v == (s32)(u32)&Value_000000b9) {
        ret = gOv14;
    } else if (v == (s32)(u32)&Value_000000ba) {
        ret = gOv15;
    } else {
        goto no_match;
    }
    Field_unk19_2(ret);
    return ret;

no_match:
    return gOv9;
}

void State_SetFlag953(void)
{
    extern u8 *gWork;

    Field_unk73_4();
    Field_Apply27(0x953, 1);
    Field_unk74_4();
}

void Scene_RunActorEightTenStepLoop(void)
{
    extern u8 gWork[];

    u32 n;
    u32 w;
    s32 a;
    s32 b;

    Field_unk75_4();
    Field_Apply28(8, 3);
    Field_unk20_2(0x266d);
    n = 10;
    w = 8;
    Field_unk9_3(8, 0, 20);
    do {
        Field_Apply29(8, 15);
        Field_unk21_2(2);
        Field_Apply30(8, 0);
        Field_unk22_2(w);
        if (w > 3) {
            w--;
        }
        n--;
    } while (n != 0);
    Field_unk23_2(0x981);
    Field_unk10_3(8, 0, 0);
    a = 7;
    b = 16;
    Field_unk27_5(7, 17, 2, 1, a, b);
    Field_unk76_4();
}

void Dialogue_RunActorElevenDialogue(void)
{
    Field_unk77_4();
    Field_unk24_2(0x2670);
    Field_unk11_3(11, 0, 20);
    Field_Apply31(11, 2);
    Field_Apply32(11, 0);
    Field_unk78_4();
}

void Scene_SetFlag987AtActorTwelveTile(void)
{
    Struct_0ff0 *s;

    s = Field_unk32_4(12);
    Field_unk79_4();
    if (s->unk8 >> 20 == 54 || s->unk10 >> 20 == 6) {
        Field_unk25_2(0x987);
    }
    Field_unk80_4();
}

void Dialogue_RunLine2682(void)
{
    Field_unk81_4();
    Field_Apply33(0, 1);
    Field_Apply34(0x2682, 1);
    Field_unk82_4();
}

void State_ApplySixRectsAfter161(void)
{
    extern u8 *gWork;

    s32 x;
    s32 y;
    s32 a;
    s32 b;

    Field_unk26_2(0x161);
    x = 23;
    y = 8;
    Field_unk28_5(35, 8, 1, 3, x, y);
    a = 3;
    b = 1;
    Field_unk29_5(35, 8, 23, 8, b, a);
    Field_unk30_5(99, 8, 87, 8, b, a);
    x = 46;
    y = 55;
    Field_unk31_5(57, 55, 3, 3, x, y);
    Field_unk32_5(57, 55, 46, 55, a, a);
    Field_unk33_5(121, 55, 110, 55, a, a);
}

void State_ApplySixRectsAfterFlag161(void)
{
    s32 x;
    s32 y;
    s32 a;
    s32 b;

    Field_unk27_2(0x161);
    x = 23;
    y = 8;
    Field_unk34_5(36, 8, 1, 3, x, y);
    a = 3;
    b = 1;
    Field_unk35_5(36, 8, 23, 8, b, a);
    Field_unk36_5(100, 8, 87, 8, b, a);
    x = 46;
    y = 55;
    Field_unk37_5(53, 55, 3, 3, x, y);
    Field_unk38_5(53, 55, 46, 55, a, a);
    Field_unk39_5(117, 55, 110, 55, a, a);
}

void Scene_RunScene3c8SequenceC(void)
{
    extern u8 gWork[];

    u32 i;
    s32 rec7;
    u8 *record;
    s32 v2;

    rec7 = Field_unk2(0);
    Field_unk83_4();
    Field_unk4_4(0, 0x200d21c);
    Field_unk84_4(0);
    Field_unk85_4(0, 6);
    *(volatile s32 *)(rec7 + 40) = 0x40000;
    Field_unk3_3(0, 0x40000, 0x20000);
    if ((*(volatile s32 *)(rec7 + 16) >> 20) <= 54) {
        *(u8 *)(Field_unk30(0) + 90) &= 254;
        v2 = 210;
    } else {
        *(u8 *)(Field_unk31(0) + 90) &= 254;
        v2 = 238;
    }
    Field_unk86_4(0, *(s16 *)(rec7 + 10), (v2 << 2));
    Field_unk87_4(1);
    {
        u8 *record = Field_unk25_4(0);
        u8 value = *(volatile u8 *)&record[90];

        record[90] = (u8)(value | 1);
    }
    Field_unk88_4(20);
    *(volatile s32 *)(rec7 + 108) = 0x20085e5;
    Field_unk4_3(0, 0x102, 60);
    Field_unk89_4(0, 4);
    Field_unk90_4(0, 0);
    Field_unk91_4(0, 4);
    *(volatile s32 *)(rec7 + 108) = 0;
    Field_unk92_4();
}

void Scene_RunScene3c8SequenceD(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;
    u8 *p7;

    p7 = *(volatile s32 *)gWork;
    {
        volatile u16 *target = (volatile u16 *)((s32)p7 + 0xcba);
        s32 shown = 0;

        *target = shown;
    }
    {
        s32 shown = 1;

        *(volatile u16 *)(((s32)p7 + 0xcb6)) = shown;
    }
    Field_unk93_4();
    Field_unk94_4(0, 1);
    Field_unk5_4(0x2688, 1);
    Field_unk6_4(0x10000, 0);
    Field_unk7_4(0x10005, 0);
    Field_unk95_4(120);
    Field_unk96_4(100);
    Field_unk97_4(142);
    Field_unk98_4(30);
    Field_unk8_4(0x7fff, 0);
    Field_unk99_4(60);
    Field_unk100_4(70);
    if (Field_unk3(0x982) == 0) {
        if (Field_unk4(0x983) == 0) {
            if ((*(volatile s32 *)0x03001e40 & 1) != 0) {
                Field_Do(0x982);
            } else {
                Field_unk2_2(0x983);
            }
        }
    }
    if (Field_unk5(0x982) == 0) {
        Field_unk3_2(0x982);
        Field_unk4_2(0x983);
        Field_SetRect(103, 27, 89, 27, 7, 8);
        Field_unk2_6(41, 90, 27, 92, 3, 2);
        Field_unk3_6(41, 90, 29, 93, 3, 2);
        Field_unk4_6(41, 90, 27, 94, 3, 2);
        Field_unk5_6(41, 90, 27, 96, 3, 2);
        Field_unk6_6(41, 90, 29, 97, 3, 2);
        Field_unk7_6(41, 96, 25, 91, 3, 2);
        Field_unk8_6(41, 92, 25, 93, 3, 2);
        Field_unk9_6(41, 96, 25, 95, 3, 2);
        Field_unk10_5(41, 96, 25, 97, 3, 2);
        Field_unk11_5(41, 96, 27, 96, 3, 2);
        Field_unk12_5(41, 96, 29, 97, 3, 2);
    } else {
        Field_unk5_2(0x983);
        Field_unk6_2(0x982);
        Field_unk13_5(111, 27, 89, 27, 7, 8);
        Field_unk14_5(41, 90, 25, 91, 3, 2);
        Field_unk15_5(41, 90, 25, 93, 3, 2);
        Field_unk16_5(41, 90, 25, 95, 3, 2);
        Field_unk17_5(41, 90, 25, 97, 3, 2);
        Field_unk18_5(41, 90, 27, 96, 3, 2);
        Field_unk19_5(41, 90, 29, 97, 3, 2);
        Field_unk20_5(41, 94, 27, 92, 3, 2);
        Field_unk21_5(41, 96, 29, 93, 3, 2);
        Field_unk22_5(41, 94, 27, 94, 3, 2);
        Field_unk23_5(41, 96, 27, 96, 3, 2);
        Field_unk24_5(41, 96, 29, 97, 3, 2);
    }
    Field_unk9_4(0x10000, 0);
    Field_unk101_4(20);
    Field_unk102_4(40);
    Field_unk10_4(0x8000, 0x1000);
    Field_unk11_4(0x1c80000, -1, 0x21e0000, 1);
    Field_unk103_4();
    Field_unk104_4(50);
    Field_unk12_4(0x1c80000, -1, 0x1a70000, 1);
    Field_unk105_4();
    Field_unk106_4();
    {
        volatile u16 *target = (volatile u16 *)((s32)p7 + 0xcb6);
        s32 shown = 0;

        *target = shown;
    }
}

void Scene_RunFlag986ActorOneScene(void)
{
    extern u8 gWork[];

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
    Field_unk107_4();
    o = Field_unk26_4(12);
    if (o->unk8 >> 20 == 53) {
        if (Field_unk32(g) == 0) {
            Field_unk108_4(g);
            o = Field_unk27_4(0);
            if (o != 0) {
                Field_unk12_3(1, o->unk8, o->unk10);
            }
            Field_unk109_4(1, m1, m2);
            Field_unk13_3(1, k, 88);
            Field_unk14_3(1, k, 104);
            Field_unk15_3(1, 0, 0);
            Field_unk110_4(20);
            Field_Apply35(1, 4);
            Field_unk28_2(20);
            Field_unk29_2(0x2691);
            Field_unk16_3(1, 0, 20);
            Field_unk17_3(1, 0, 10);
            Field_unk18_3(1, h, 60);
            Field_unk19_3(1, 0, 0);
            Field_unk30_2(20);
            Field_Apply36(1, 2);
            Field_unk31_2(20);
            Field_unk20_3(1, 0, 20);
            Field_Apply37(0, 3);
            Field_Apply38(1, 3);
            Field_unk32_2(30);
            Field_unk21_3(1, k, 88);
            Field_Apply39(1, 2);
            u = Field_unk28_4(0);
            if (u != 0) {
                Field_unk22_3(1, u->unkA, u->unk12);
            }
            Field_unk33_2(1);
            Field_unk23_3(1, 0, 0);
            Field_unk111_3();
        }
    }
}

void Scene_RunFiveCallSequence(void)
{
    void Field_unk112_3(s32, s32);

    Field_unk113_3();
    Field_unk114_3();
    Field_unk34_2(20);
    Field_unk115_3();
    Field_unk116_3();
}

void State_RunActor13AtColumn42Setup(void)
{
    Struct_1644 *obj;
    s32 val;
    s32 a;
    s32 b;

    obj = Field_unk39_4(13);
    Field_unk117_3();
    if (obj->unk8 >> 20 == 42) {
        Field_unk35_2(30);
        Field_unk36_2(188);
        obj->unk55 = 0;
        val = 0xfffe0000;
        obj->unk14 = val;
        obj->unkC = val;
        Field_unk40_4(0x200);
        a = 3;
        b = 5;
        Field_unk40_5(44, 117, 41, 117, a, b);
    }
    Field_unk118_3();
}

void Scene_PlaceAndPinSlots8To10(void)
{
    void Field_unk112_3(s32, s32);

    u32 i;
    Struct_18f8 *rec;
    s32 x;
    s32 y;
    s32 a;
    s32 b;

    Field_unk37_2(8);
    Field_unk119_3();
    x = 12;
    y = 44;
    Field_unk41_5(19, 44, 4, 1, x, y);
    x = 11;
    y = 51;
    Field_unk42_5(17, 51, 2, 2, x, y);
    i = 0;
    do {
        rec = Field_unk41_4(i + 8);
        a = rec->unk8 >> 20;
        b = rec->unk10 >> 20;
        Field_unk43_5(12, 50, 1, 1, a, b);
        i++;
    } while (i <= 2);
    Field_unk112_3(10, 9);
    Field_unk120_3();
}

void State_ApplyRectAt19_44AndRunThree(void)
{
    s32 x;
    s32 y;

    Field_unk121_3();
    x = 12;
    y = 44;
    Field_unk44_5(19, 44, 4, 1, x, y);
    Field_unk122_3();
    Field_unk123_3();
    Field_unk124_3();
}

void Actor_ApplyKind45AtActorsElevenAndTwelve(void)
{
    u32 i;
    Struct_199c *p;

    i = 0;
    do {
        p = Field_unk33_4(i + 11);
        i++;
        Field_SetMode(0, p->unk8, p->unk10, 45);
    } while (i <= 1);
}

void Actor_ApplyPositionsOfActors11And12(void)
{
    u32 i;
    Struct_19c0 *p;

    i = 0;
    do {
        p = Field_unk34_4(i + 11);
        if (p->unkC > -0x100000) {
            Field_SetMode2(0, p->unk8, p->unk10, 255);
        }
        i++;
    } while (i <= 1);
}

void Scene_RunGuardedThreeStepSetup(void)
{
    void Field_unk112_3(s32, s32);

    Field_unk125_3();
    if (Field_unk33() == 0) {
        Field_unk126_3();
        Field_unk127_3();
        Field_unk128_3();
    }
    Field_unk129_3();
}

void State_MarkActorAndApplyRectAtTile(Struct_1a14 *obj)
{
    extern u8 *gWork;

    s32 x;
    s32 z;

    obj->unk23 |= 2;
    obj->unk55 = 0;
    x = obj->unk8 >> 20;
    z = obj->unk10 >> 20;
    Field_unk45_5(9, 24, 1, 1, x, z);
}

void OvObj_ResetObjectWhenFlags12Set(Struct_1a50 *o)
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
        t = Field_SetMode3(o->unk8, 0, 0x2000000, 223);
        Field_unk38_2(o);
        o->unk8 = z;
        o->unk10 = z;
        Field_unk39_2(t);
    } else {
        Field_unk130_3();
    }
}

void Actor_UpdateSlots11And12ByTile(void)
{
    Struct_1a9c *o;

    Field_unk131_3();
    o = Field_unk35_4(11);
    if (o->unk8 >> 20 == 8) {
        Field_unk132_3();
        Field_unk40_2(o);
    } else {
        Field_unk41_2(o);
    }
    o = Field_unk36_4(12);
    if (o->unk8 >> 20 == 7) {
        Field_unk133_3();
        Field_unk42_2(o);
    } else {
        Field_unk43_2(o);
    }
    Field_unk134_3();
}

void Scene_RunGuardedRectStep(void)
{
    s32 x;
    s32 y;

    Field_unk135_3();
    if (Field_unk34() == 0) {
        x = 45;
        y = 43;
        Field_unk46_5(109, 43, 7, 5, x, y);
        Field_unk136_3();
    }
    Field_unk137_3();
    Field_unk138_3();
}

void State_RunConditionalStep(void)
{
    extern u8 *gWork;

    Field_unk139_3();
    if (Field_unk35() == 0) {
        s32 k5 = 44, k6 = 39;
        Field_unk47_5(108, 39, 13, 7, k5, k6);
        Field_unk140_3();
    }
    Field_unk141_2();
    Field_unk142_2();
}

s32 Actor_SetHeightAboveLinkedRecord(Struct_22a4 *obj)
{
    Struct_22a4b *rec;

    rec = Field_unk37_4(((s16 *)obj)[50]);
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
void Actor_PickHighestSlotAtSameTileAndRelease(s32 selector)
{
    extern u8 *gWork;
    void Effect_SpawnNineRadialEffects();

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

        cand = Field_unk20_4(no);
        sel = Field_unk21_4(selector);

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

    Field_unk143_2(selector, 0x40000, 0x20000);
    Field_unk144_2(sel,
                  *(s32 *)(sel + 8),
                  highest,
                  *(s32 *)(sel + 16));
    Field_unk145_2(selector);
    Field_unk146_2(188);
    Field_unk48_4(selector);
    Field_unk147_2(30);
}

void Scene_RunThreeCallSequence(void)
{
    void Field_unk112_3(s32, s32);

    Field_unk148_2();
    Field_unk149_2();
    Field_unk150_2();
}

void Scene_DrawTilesWhenCheckClear(void)
{
    void Field_unk112_3(s32, s32);

    Field_unk151_2();
    if (Field_unk36() == 0) {
        { s32 k5 = 5, k6 = 48; Field_unk48_5(69, 48, 4, 2, k5, k6); }
        { s32 j5 = 9, j6 = 37; Field_unk49_5(73, 37, 9, 13, j5, j6); }
        Field_unk152_2();
    }
    Field_unk153_2();
    Field_unk154_2();
}

/* Runs a fixed sequence of setup calls with literal parameters; most share
 * a leading 0 argument. */
void Scene_RunScene3c8SequenceB(void)
{
    extern u8 gWork[];

    BattleRuntime_Reset_1();
    ObjectMotion_SetSpeedParameters_1(0, 0x8000, 0x4000);
    Motion_SetPosReset_1(0, 0x208, 0x2c8);
    Motion_ArmCb_1(0, 0x4000, 10);
    (void)Field_unk37(0x2080000, 0, 0x3100000, 223);
    BattleFx_RunRisingObjectSequence_1(0, 6, 0);
    BattleRuntime_WaitIfModeZero_1(60);
    Field_unk155_2(20); /* main:0808a248 */
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
}

u8 *SceneData_SelectTableBySceneId(void)
{
    extern u8 gCell[];

    s32 off = 0x1c0;
    s32 v = *(s16 *)(gCell + off);

    if (v == (s32)&Value_000000b5) {
        return gOv16;
    }
    if (v == (s32)&Value_000000b6) {
        return gOv21;
    }
    if (v == (s32)&Value_000000b7) {
        return gOv17;
    }
    if (v == (s32)&Value_000000b8) {
        return gOv18;
    }
    if (v == (s32)&Value_000000b9) {
        return gOv19;
    }
    if (v == (s32)&Value_000000ba) {
        return gOv20;
    }
    return gOv21;
}

void Actor_ClearActorModeAndSetState5(s32 no)
{
    u8 *p;
    s32 mask;

    p = Field_unk42_4(no);
    p[0x55] = 0;
    mask = 252;
    mask &= p[0x59];
    p[0x59] = mask;
    Field_Apply40(p, 0);
    Field_Apply41(p, 5);
    Field_Apply42(no, 3);
    {
        s32 v = 2;
        v |= p[0x23];
        p[0x23] = v;
    }
}

void State_ApplyStepToSlots15To18(void)
{
    extern u8 *gWork;

    u32 i;

    i = 15;
    do {
        Field_unk44_2(i);
        i++;
    } while (i <= 18);
}

void State_PassZeroAndMinusOneRecord(void)
{
    extern u8 *gWork;

    struct Args args;

    args.first = 0;
    args.last = -1;
    Field_unk156_2(&args);
}

void State_PassRange0To1(void)
{
    extern u8 *gWork;

    struct Args2 args;

    args.first = 0;
    args.last = 1;
    Field_unk157_2(&args);
}

void State_PassRangeNeg1To0(void)
{
    extern u8 *gWork;

    struct Args3 args;

    args.first = -1;
    args.last = 0;
    Field_unk158_2(&args);
}

void State_CallHandlerWithFlagPair(void)
{
    extern u8 *gWork;

    struct Args4 args;

    args.first = 1;
    args.last = 0;
    Field_unk159_2(&args);
}

/*
 * Field_unk160_2 is the relocated call word for the in-overlay routine at
 * image offset 0x4520, not a runtime address.  The 16-byte owner loads no
 * literal, so it carries no pool word and no alignment halfword.  432 is
 * built from a shifted immediate and passed straight to the callee as a
 * value, not used as a displacement.
 */
void State_Call4520With432And32(void)
{
    extern u8 *gWork;

    Field_unk160_2(432, 32);
}

void Scene_RunStep230With44(void)
{
    extern u8 gWork[];

    Field_unk161_2(0x230, 44);
}

void Scene_RunScene3c8(void)
{
    extern u8 gWork[];

    u32 i;
    u8 *p8;
    s32 rec;
    u8 *rec8;
    s32 record;
    s32 none;
    s32 v2;
    s32 slot0;

    rec = Field_unk6(0);
    rec8 = Field_unk7(20);
    Field_unk162_2();
    Field_unk13_4(-1, -1, -1, 0);
    Field_unk163_2();
    Field_unk164_2(1);
    *(s32 *)(rec + 12) = 0x820000;
    *(s32 *)(rec + 72) = 0x8000;
    none = 0;
    *(s32 *)(rec + 68) = none;
    p8 = rec + 85;
    *p8 = none;
    Field_unk165_2();
    Field_unk166_2();
    Field_unk167_2(204);
    Field_unk168_2(30);
    *p8 = 3;
    Field_unk169_2(24);
    Field_unk14_4(0, 0x101);
    Field_unk170_2(0, 22);
    *p8 &= 254;
    *(s32 *)((s32)rec8 + 12) += -0x30000;
    *(s32 *)(rec + 12) += -0x30000;
    *(s32 *)(rec + 20) += -0x30000;
    Field_unk171_2(2);
    *(s32 *)((s32)rec8 + 12) += -0x20000;
    *(s32 *)(rec + 12) += -0x20000;
    *(s32 *)(rec + 20) += -0x20000;
    Field_unk172_2(10);
    *(s32 *)((s32)rec8 + 12) += 0x20000;
    *(s32 *)(rec + 12) += 0x20000;
    *(s32 *)(rec + 20) += 0x20000;
    Field_unk173_2(4);
    *(s32 *)((s32)rec8 + 12) += 0x20000;
    *(s32 *)(rec + 12) += 0x20000;
    *(s32 *)(rec + 20) += 0x20000;
    Field_unk174_2(4);
    *(s32 *)((s32)rec8 + 12) += 0x10000;
    *(s32 *)(rec + 12) += 0x10000;
    *(s32 *)(rec + 20) += 0x10000;
    *p8 = none;
    rec8[85] = none;
    Field_unk15_4(0, 0x100);
    Field_unk175_2(0, 1);
    Field_unk176_2(40);
    *(s32 *)(rec + 108) = 0x200c969;
    Field_unk177_2(60);
    Field_unk178_2(0, 1);
    Field_unk179_2(20, 1);
    Field_unk180_2(17);
    Field_unk7_2(0x134);
    Field_unk8_2(0x101);
    v2 = 0;
    do {
        *(s32 *)(rec + 12) += 0x10000;
        *(s32 *)(rec + 20) += 0x10000;
        *(s32 *)((s32)rec8 + 12) += 0x10000;
        slot0 = v2;
        Field_unk181_2(1);
        v2 = slot0;
        v2 = (v2 + 1);
    } while ((u32)v2 <= 127);
    Field_unk182_2(21);
}
