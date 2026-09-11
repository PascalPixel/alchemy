#include "types.h"
#include "spawn_configured_effect.h"
#include "spawn_configured_effect_body.inc"
#include "find_clear_actor_position.h"
#include "find_clear_actor_position_body.inc"
#include "staged_actor.h"

/* overlays/scene/actor/map_init/actor_depth_effect.c */
#define Actor_SetFlagBitByRelativeDepth Func_02000ec8
struct Actor { u8 unknown_00[8]; s32 x; s32 y; s32 z; u8 unknown_14[0xf]; u8 flags23; };

struct Actor *Func_02003fdc();

s32 Actor_SetFlagBitByRelativeDepth(struct Actor *actor)
{
    struct Actor *ref;
    u8 *fp;
    u8 flag;
    ref = Func_02003fdc(0);
    fp = &actor->flags23;
    flag = *fp | 2;
    *fp = flag;
    if (ref->z < actor->z) {
        s32 lim = actor->z - ref->z;
        s32 ay;
        lim += 0x00040000;
        ay = actor->y;
        ay += lim;
        if (ref->y <= ay) {
            flag &= 0xfd;
            *fp = flag;
        }
    }
    return 0;
}
#undef Actor_SetFlagBitByRelativeDepth

/* overlays/scene/actor/map_init/actor_position.c */
#define Actor_PassRaisedPointOfActorZero Func_02000dc8
#define Actor_PassActorZeroOffsetPoint Func_02000df4
#define Actor_CopyActor8PositionWhenAtRow10 Func_02000fe8
#define Actor_ApplyPointLeftOfActorZero Func_02001348
#define Actor_PassPointTwoRightOfActorZero Func_020013b4
#define Actor_LandOnHighestPlatform Func_02001c2c
struct Actor {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

typedef struct Record {
    s32 pad0;
    s32 pad4;
    s32 w8;
    s32 w12;
    s32 w16;
} Record;

/*
 * resource_3c4: land an actor on the highest platform sharing its tile.
 */
typedef struct Slot_02001c2c {
    u8 pad0[8];
    s32 x;                 /* +8,  12.20 fixed point */
    s32 y;                 /* +12 */
    s32 z;                 /* +16, 12.20 fixed point */
} Slot_02001c2c;

struct Actor *Func_02003edc(s32);
void Func_02001aba(s32 *);
struct Actor *Func_02003f08(s32);
void Func_02001ae4(s32 *);
Record *Func_020040fc(s32);
Record *Func_0200410a(s32);
Record *Func_02004118(s32);
Record *Func_02004126(s32);
struct Actor *Func_0200445c(s32);
void Func_02002038(s32 *);
struct Actor *Func_020044c8(s32);
void Func_020020a6(s32 *);
Slot_02001c2c *Func_02004d56();
Slot_02001c2c *Func_02004d5e();
Slot_02001c2c *Func_02004d6e();
Slot_02001c2c *Func_02004d76();
Slot_02001c2c *Func_02004d86();
Slot_02001c2c *Func_02004d98();
Slot_02001c2c *Func_02004da8();
void Func_02004dcc();
Slot_02001c2c *Func_02004dca();
Slot_02001c2c *Func_02004dd2();
Slot_02001c2c *Func_02004dda();
void Func_02004d4e();
void Func_02004e14();
void Func_02004e92();
void Func_02002ecc();
void Func_02004dde();

/*
 * Each site names the call word it holds, so the same slot accessor appears
 * here under several symbols. The declarations are old-style because the call
 * sites vary in arity.
 */

void Actor_PassRaisedPointOfActorZero(void)
{
    s32 pos[3];
    struct Actor *p = Func_02003edc(0);

    pos[0] = p->f08;
    pos[1] = p->f0c;
    pos[2] = p->f10 + 0x200000;
    Func_02001aba(pos);
}

void Actor_PassActorZeroOffsetPoint(void)
{
    s32 pos[3];
    struct Actor *actor = Func_02003f08(0);

    pos[0] = actor->f08;
    pos[1] = actor->f0c;
    pos[2] = actor->f10 + 0xFFE00000;
    Func_02001ae4(pos);
}

s32 Actor_CopyActor8PositionWhenAtRow10(Record *record)
{
    Record *ref = Func_020040fc(0);

    if (ref->w12 > (s32)0xffd00000
        && (Func_0200410a(8)->w16 >> 20) == 10) {
        record->w8 = Func_02004118(8)->w8;
        record->w12 = (s32)0xffe00000;
        record->w16 = Func_02004126(8)->w16;
    } else {
        record->w8 = 0;
        record->w12 = 0;
        record->w16 = 0;
    }
    return 0;
}

void Actor_ApplyPointLeftOfActorZero(void)
{
    s32 point[3];
    struct Actor *actor = Func_0200445c(0);

    point[0] = actor->f08 + 0xFFE00000;
    point[1] = actor->f0c;
    point[2] = actor->f10;
    Func_02002038(point);
}

void Actor_PassPointTwoRightOfActorZero(void)
{
    s32 pos[3];
    struct Actor *actor = Func_020044c8(0);

    pos[0] = actor->f08 + 0x200000;
    pos[1] = actor->f0c;
    pos[2] = actor->f10;
    Func_020020a6(pos);
}

/*
 * Scan slots 10 to 14, skipping the subject, keep those whose whole-tile x
 * and z match the subject's, and take the greatest height among them; the
 * winning slot index goes into the subject's tag at +100. The starting best
 * height is -5.0 in 12.20, held in the owner's one pool word, and it is what
 * the move receives when no slot matches -- the tag is then left untouched.
 * Every field read re-fetches its record, which is the shape to keep.
 */
void Actor_LandOnHighestPlatform(s32 subject)
{
    s32 best = (s32)0xffb00000;
    u32 i;

    for (i = 0; i <= 4; i++) {
        s32 slot = i + 10;

        if (slot == subject) continue;

        if ((Func_02004d56(slot)->x >> 20) != (Func_02004d5e(subject)->x >> 20)) continue;
        if ((Func_02004d6e(slot)->z >> 20) != (Func_02004d76(subject)->z >> 20)) continue;

        /*
         * 0x00100000 is one whole unit above the candidate's own height. The
         * comparison is signed, and a tie updates the best.
         */
        if (best > Func_02004d86(slot)->y + 0x100000) continue;

        best = Func_02004d98(slot)->y + 0x100000;
        *(u16 *)((u8 *)Func_02004da8(subject) + 100) = (u16)slot;
    }

    Func_02004dcc(subject, 0x40000, 0x20000);   /* 128 << 11, 128 << 10 */

    /*
     * Three separate lookups of the same record, in this order. The locals
     * fix the sequence, which argument evaluation order would not.
     */
    {
        Slot_02001c2c *target = Func_02004dca(subject);
        Slot_02001c2c *from = Func_02004dd2(subject);
        s32 z = Func_02004dda(subject)->z;

        Func_02004d4e(target, from->x, best, z);
    }

    Func_02004e14(subject);
    Func_02004e92(188);
    Func_02002ecc(subject);
    Func_02004dde(30);
}
#undef Actor_PassRaisedPointOfActorZero
#undef Actor_PassActorZeroOffsetPoint
#undef Actor_CopyActor8PositionWhenAtRow10
#undef Actor_ApplyPointLeftOfActorZero
#undef Actor_PassPointTwoRightOfActorZero
#undef Actor_LandOnHighestPlatform

/* overlays/scene/actor/map_init/actor_presentation.c */
#define Actor_SetActor14Field98ByFlag200 Func_020014ac
#define ActorDraw_ConfigureActorTwentyAndFlag200 Func_0200170c
s32 Func_0200458e_a(s32);
s32 Func_0200458e_b(s32);
u8 *Func_020045d0(s32);
u8 *Func_020045dc(s32);
u8 *Func_020045ec(s32);
u8 *Func_020045f8(s32);
void Func_02004858(s32, s32);
void Func_02004878(s32, s32);
void Func_02004868(s32, s32);
u8 *Func_02004836(s32);
void Func_02004818(s32);

void Actor_SetActor14Field98ByFlag200(void)
{
    u8 *p;
    u8 val;

    Func_0200458e_a(0x201);
    if (Func_0200458e_b(0x200) != 0) {
        Func_020045d0(14)[98] = 0;
        Func_020045dc(14)[89] &= (u8)0xf7;
    } else {
        Func_020045ec(14)[98] = 1;
        p = Func_020045f8(14);
        p += 89;
        val = 8;
        val |= *p;
        *p = val;
    }
}

void ActorDraw_ConfigureActorTwentyAndFlag200(void)
{
    u8 *flags;

    Func_02004858(20, 1);
    Func_02004878(20, 0);
    Func_02004868(20, 2);
    flags = Func_02004836(20) + 35;
    *flags &= 0xFD;
    Func_02004818(0x200);
}
#undef Actor_SetActor14Field98ByFlag200
#undef ActorDraw_ConfigureActorTwentyAndFlag200

/* overlays/scene/actor/map_init/actor_search.c */
#define Actor_CheckTwoUnitsAboveActorZero Func_02001424
struct Actor02001424 {
    u8 head[8];
    s32 x;
    s32 y;
    s32 z;
};

struct Actor02001424 *Func_02004538(s32);
s32 Func_02002116(s32 *);
void Func_0200282e(void);

void Actor_CheckTwoUnitsAboveActorZero(void)
{
    struct Actor02001424 *actor = Func_02004538(0);
    s32 target[3];

    target[0] = actor->x;
    target[1] = actor->y;
    target[2] = actor->z + 0x00200000;
    if (Func_02002116(target)!= 0) {
        Func_0200282e();
    }
}
#undef Actor_CheckTwoUnitsAboveActorZero

/* overlays/scene/actor/map_init/conditional_scene_setup.c */
#define State_RunUnlessActorZeroAtTile32x50 Func_02001510
struct Actor {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct Actor *Func_02004622(s32);
void Func_02002872(void);

void State_RunUnlessActorZeroAtTile32x50(void)
{
    struct Actor *actor = Func_02004622(0);

    if ((actor->f08 >> 20) != 32 || (actor->f10 >> 20) != 50) {
        Func_02002872();
    }
}
#undef State_RunUnlessActorZeroAtTile32x50

/* overlays/scene/actor/map_init/configurable_effect_spawn.c */
#define ConfigurableEffectDescriptors Data_0200b36c
#define GetPrimaryEffectRecord Func_02003c16
#define SpawnConfiguredEffectObject Func_02003b94
#define SetConfiguredEffectVariant Func_02003b9e
#define ApplyConfiguredEffectDescriptor Func_02003bb8
#define SetConfiguredEffectLink Func_02003d2a
#define ComputeConfiguredEffectRateFromDelta Func_02003c3c
#define ComputeConfiguredEffectRateFromOffset Func_02003c54
#define ComputeConfiguredEffectSecondRate Func_02003c62
#define SetConfiguredEffectCallbackMode Func_02003cb8
#define ApplyConfiguredEffectCallbackArgument Func_02003cc8
#define Effect_SpawnConfiguredEffect Func_02000ae8


void Effect_SpawnConfiguredEffect(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                           u32 flags, const struct Options *options)
{
}
#undef ConfigurableEffectDescriptors
#undef GetPrimaryEffectRecord
#undef SpawnConfiguredEffectObject
#undef SetConfiguredEffectVariant
#undef ApplyConfiguredEffectDescriptor
#undef SetConfiguredEffectLink
#undef ComputeConfiguredEffectRateFromDelta
#undef ComputeConfiguredEffectRateFromOffset
#undef ComputeConfiguredEffectSecondRate
#undef SetConfiguredEffectCallbackMode
#undef ApplyConfiguredEffectCallbackArgument
#undef Effect_SpawnConfiguredEffect

/* overlays/scene/actor/map_init/dialogue_layout.c */
#define Scene_RunLayoutAt93By30 Func_02001374
#define Scene_RunLayoutAt83By45 Func_02002300
#define Scene_PlaceAndPinSlots10And11 Func_02002410
typedef struct Slot_02002410 {
    s32 pad0;
    s32 pad4;
    s32 column;            /* +8,  12.20 fixed point */
    s32 pad12;
    s32 row;               /* +16, 12.20 fixed point */
} Slot_02002410;

void Func_0200446e(void);
void Func_0200442a(s32, s32, s32, s32, s32, s32);
void Func_02001456(void);
void Func_020037a6(void);
void Func_02004496(void);
void Func_020053fa(void);
void Func_020053b6(s32, s32, s32, s32, s32, s32);
void Func_020023e2(void);
void Func_020043c6(void);
void Func_02005422(void);
void Func_020054c2();
void Func_0200333e();
Slot_02002410 *Func_02005540();
Slot_02002410 *Func_02005548();
void Func_020054ee();
Slot_02002410 *Func_02005564();
Slot_02002410 *Func_0200556c();
void Func_02005512();

/*
 * Layout step in resource_3c4 for slots 10 and 11. One six-argument
 * placement, then two grid-cell pins built from each slot's +8 and +16 words
 * shifted right by 20.
 *
 * Several call sites reach the same routine, but each keeps its own call
 * word; the sites must not be collapsed onto one alias.
 */

void Scene_RunLayoutAt93By30(void)
{
    Func_0200446e();
    {
        s32 width = 29;
        s32 height = 30;

        Func_0200442a(93, 30, 6, 5, width, height);
    }
    Func_02001456();
    Func_020037a6();
    Func_02004496();
}

void Scene_RunLayoutAt83By45(void)
{
    Func_020053fa();
    {
        s32 width = 19;
        s32 height = 45;

        Func_020053b6(83, 45, 11, 8, width, height);
    }
    Func_020023e2();
    Func_020043c6();
    Func_02005422();
}

void Scene_PlaceAndPinSlots10And11(void)
{
    s32 row;

    {
        s32 k5 = 29, k6 = 30;
        Func_020054c2(93, 30, 6, 5, k5, k6);
    }
    Func_0200333e(11, 10);

    {
        s32 col20 = Func_02005540(10)->column >> 20;
        row = Func_02005548(10)->row >> 20;
        Func_020054ee(2, 36, 1, 1, col20, row);
    }

    {
        s32 col20 = Func_02005564(11)->column >> 20;
        row = Func_0200556c(11)->row >> 20;
        Func_02005512(2, 36, 1, 1, col20, row);
    }
}
#undef Scene_RunLayoutAt93By30
#undef Scene_RunLayoutAt83By45
#undef Scene_PlaceAndPinSlots10And11

/* overlays/scene/actor/map_init/effect.c */
#define OvObj_CreateAndInitialize Func_020009f4
#define Effect_SpawnNineRadialEffects Func_020011e0
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

u8 *Func_02003a6a(s32 kind, s32 x, s32 y, s32 z);
void Func_02003ad8(u8 *obj, s32 mode);
void Func_02003b98(u8 *obj, s32 mode);
void Func_02003af0(u8 *obj, s32 mode);
struct SceneObject *Func_020042fa(void);
s32 Func_02004254(s32);
s32 Func_02004258(s32);
s32 Func_0200423e(s32, s32);
void Func_02001d32(s32, s32, s32, s32, s32, s32, s32, struct EffectParams *);

/*
 * Create an object, clear the low bits of its owner record, and initialise it.
 * The four callees are declared at their in-overlay entry points, not at
 * veneers.  The +9 flag clear takes its mask from a local that is initialised
 * first and only afterwards ANDed with the loaded byte; written as one
 * expression it swaps which register holds the mask and which the byte.
 */

u8 *OvObj_CreateAndInitialize(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *ret;
    u8 *obj = Func_02003a6a(kind, x, y, z);

    if (obj != 0) {
        u8 *owner = *(u8 **)(obj + 80);
        u32 mask = ~12;
        mask = mask & owner[9];
        owner[9] = (u8)mask;
        obj[85] = 0;
        obj[89] = 8;
        Func_02003ad8(obj, 0);
        Func_02003b98(obj, 14);
        Func_02003af0(obj, 1);
        ret = obj;
    } else {
        ret = 0;
    }
    return ret;
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

    object = Func_020042fa();
    params.unk00 = 1;
    params.mode = 7;
    params.callback = 0x02009069;
    for (i = 0; i <= 16; i += 2) {
        v = i << 12;
        vec.x = Func_02004254(v);
        vec.y = 0;
        z = Func_02004258(v);
        x = vec.x;
        vec.z = z;
        x = x + Func_0200423e(x, 3);
        vec.x = x;
        Func_02001d32(object->x, object->y, object->z, x, vec.y, z, 0x01030001, &params);
    }
}
#undef OvObj_CreateAndInitialize
#undef Effect_SpawnNineRadialEffects

/* overlays/scene/actor/map_init/effect_motion_variant_a.c */
/*
 * Per-frame integrator for an effect record -- resource_3c4. It adds the
 * velocity at +68/+72/+76 into the position at +8/+12/+16, decays two of the
 * three velocities, accumulates the rates at +48/+52, and turns the sprite.
 */

/* Every field is named by offset; the layout is not verified. */
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

s32 Func_0200409e();

/*
 * A loader-relocated call word rather than a runtime address; it turns a value
 * and a duration into a per-frame step. Declared without a prototype, and the
 * call site passes two arguments.
 */

/*
 * The decay of the Z velocity stays a signed divide by sixteen: that shape is
 * what reproduces the negative bias and arithmetic shift in the reference.
 */
void Func_02001068(struct Effect *effect)
{
    s32 velocity_z;
    struct Sprite *sprite;
    s32 velocity_x;

    /* This block orders the Z load after the Y store; do not flatten it. */
    do {
        velocity_x = effect->velocity[0];
        effect->position[0] += velocity_x;
        effect->position[1] += effect->velocity[1];
    } while (0);
    velocity_z = effect->velocity[2];
    effect->position[2] += velocity_z;

    effect->velocity[0] = velocity_x - Func_0200409e(velocity_x, 18);
    effect->velocity[2] = velocity_z - velocity_z / 16;

    effect->accum18 += effect->rate30;
    effect->accum1c += effect->rate34;

    sprite = effect->sprite;
    sprite->angle += effect->step64;
}

/* overlays/scene/actor/map_init/move_actor_zero_to_target.c */
/*
 * Actor movement for overlay resource_3c4.  Declarations are left without
 * prototypes because Func_02003dec and Func_02003dca are each called twice
 * with different argument shapes.
 */

typedef struct Actor_02000cd0 {
    u8 pad0[6];
    u16 tag;               /* +6  */
    s32 x;                 /* +8,  12.20 fixed point */
    s32 y;                 /* +12 */
    s32 z;                 /* +16, 12.20 fixed point */
    u8 pad20[20];
    s32 speedZ;            /* +40 */
    s32 pad44;
    s32 speedX;            /* +48 */
    s32 speedY;            /* +52 */
    u8 pad56[29];
    u8 flags;              /* +85 */
} Actor_02000cd0;

typedef struct Target_02000cd0 {
    s32 x;                 /* +0, 12.20 fixed point */
    s32 pad4;
    s32 z;                 /* +8, 12.20 fixed point */
} Target_02000cd0;

Actor_02000cd0 *Func_02003dec_a();
s32 Func_02003d6a();
s32 Func_02003dca_a();
s32 Func_02003dca_b();
s32 Func_02003dd6();
void Func_02003e2e();
void Func_02003d96();
void Func_02003d6c();
void Func_02003efa();
void Func_02003daa();
void Func_02003e24();
void Func_02003eb4();
void Func_02003dec_b();
void Func_02003e4c();
void Func_02003eaa();

/*
 * Tries to move actor 0 onto the caller's target.  It builds a three-word
 * 12.20 probe from the record's own position -- the horizontal words snapped
 * to their whole-unit grid and lifted by half a unit -- asks the collision
 * service about it, and refuses when either the probe or the target is
 * rejected.  Returns 1 when refused and 0 when the move ran.  The 248-byte
 * owner includes its one pool word.
 */
s32 Actor_MoveActorZeroToTarget(const Target_02000cd0 *target)
{
    Actor_02000cd0 *actor = Func_02003dec_a(0);
    u8 saved = actor->flags;
    s32 probe[3];

    probe[0] = (actor->x & (s32)0xfff00000) + 0x00080000;
    probe[1] = actor->y;
    probe[2] = (actor->z & (s32)0xfff00000) + 0x00080000;

    Func_02003d6a(0x00100000, (actor->tag + 0x2000) & 0xc000, probe);

    /* Both guards branch to one shared exit placed after the body.  Writing
     * `return 1` twice would put an inline copy near the top instead. */
    if (Func_02003dca_a(actor, probe) == 1) {
        goto refuse;
    }
    if (Func_02003dd6(actor, target) != 0) {
        goto refuse;
    }

    Func_02003e2e();
    Func_02003d96(actor, 6);
    Func_02003d6c(6);
    Func_02003efa(152);
    Func_02003daa(actor, 7);

    actor->speedX = 0x00030000;
    actor->speedY = 0x00020000;
    actor->speedZ = 0x00040000;
    actor->flags &= (u8)0x7e;   /* masks the byte re-read here, not `saved` */

    Func_02003e24(actor, 0);
    Func_02003eb4(0, ((target->x >> 20) << 4) + 8, ((target->z >> 20) << 4) + 8);
    Func_02003dec_b(actor, 6);
    Func_02003e4c(actor, 1);
    Func_02003dca_b(6);

    actor->flags = saved;
    Func_02003eaa();
    return 0;

refuse:
    return 1;
}

/* overlays/scene/actor/map_init/overlay_object.c */
#define OvObj_PrepareSpawnedObjectMode4 Func_02000a4c
#define OvObj_IntegrateVelocities Func_02000ab0
#define OvObj_SetYAboveLinkedActor Func_02001a98
struct Actor {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
};

u8 *Func_02003ac2(s32 kind, s32 x, s32 y, s32 z);
void Func_02003b34(u8 *obj, s32 mode);
void Func_02003bf4(u8 *obj, s32 mode);
struct Actor *Func_02004bb2(s32);

/*
 * Spawn an object and prepare its owner record and flags.  The three callees
 * are declared at their in-overlay entry points, not at veneers.  Each
 * read-modify-write takes its mask from a local that is initialised first and
 * ANDed with the loaded byte afterwards, which is what puts the mask in r3 and
 * the byte in r2.  The +0x23 mask stays byte-wide as 0xfe rather than ~1,
 * which would widen it to a negated 2.
 */

u8 *OvObj_PrepareSpawnedObjectMode4(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *ret;
    u8 *obj = Func_02003ac2(kind, x, y, z);

    if (obj != 0) {
        u8 *rec = *(u8 **)(obj + 80);
        u32 mask = ~12;
        u32 lo;
        mask = mask & rec[9];
        rec[9] = (u8)(mask | 4);
        obj[85] = 0;
        obj[89] = 8;
        Func_02003b34(obj, 0);
        Func_02003bf4(obj, 15);
        lo = 0xfe;
        lo = lo & obj[35];
        obj[35] = (u8)(lo | 2);
        ret = obj;
    } else {
        ret = 0;
    }
    return ret;
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

s32 OvObj_SetYAboveLinkedActor(u8 *owner)
{
    s16 *id = (s16 *)(owner + 100);
    struct Actor *actor = Func_02004bb2(*id);

    *(s32 *)(owner + 12) = actor->f0c + 0x100000;
    return 0;
}
#undef OvObj_PrepareSpawnedObjectMode4
#undef OvObj_IntegrateVelocities
#undef OvObj_SetYAboveLinkedActor

/* overlays/scene/actor/map_init/scene_data.c */
#define SceneData_SelectDataByRuntimeSelector Func_02001270
#define SceneData_SelectTableB81cByWord224 Func_020012b0
#define SceneData_GetTableB85c Func_020012d8
#define SceneData_SelectAndApplyTableBySceneId Func_020012e0
#define SceneData_SelectTableByWord224 Func_02002370
extern s16 Data_02000240[];
extern u8 Value_000000ac;
extern u8 Value_000000ad;
extern u8 Data_0200b474[];
extern u8 Data_0200b654[];
extern u8 Data_0200b42c[];
extern u8 Data_0200b81c[];
extern u8 Data_0200b8f4[];
extern u8 Data_0200ba74[];
extern u8 Data_0200bc0c[];
extern u8 Data_0200bef4[];

void Func_02004402(u8 *);

/* Return this overlay's state block. */

s32 SceneData_SelectDataByRuntimeSelector(void)
{
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_000000ac) {
        return (s32)Data_0200b474;
    }
    if (v == (s32)&Value_000000ad) {
        return (s32)Data_0200b654;
    }
    return (s32)Data_0200b42c;
}

s32 SceneData_SelectTableB81cByWord224(void)
{
    if (Data_02000240[224] == (s32)&Value_000000ad) {
        return (s32)Data_0200b81c;
    }
    return 0;
}

u8 *SceneData_GetTableB85c(void) { return (u8 *)0x0200b85c; }

u8 *SceneData_SelectAndApplyTableBySceneId(void)
{
    u8 *tbl;

    if (Data_02000240[224] == (s32)&Value_000000ac) {
        tbl = Data_0200b8f4;
    } else {
        tbl = Data_0200ba74;
    }
    Func_02004402(tbl);
    return tbl;
}

s32 SceneData_SelectTableByWord224(void)
{
    if (Data_02000240[224] == (s32)&Value_000000ac) {
        return (s32)Data_0200bc0c;
    }
    return (s32)Data_0200bef4;
}
#undef SceneData_SelectDataByRuntimeSelector
#undef SceneData_SelectTableB81cByWord224
#undef SceneData_GetTableB85c
#undef SceneData_SelectAndApplyTableBySceneId
#undef SceneData_SelectTableByWord224

/* overlays/scene/actor/map_init/scene_dialogue.c */
#define Dialogue_RunFlag982Or983Dialogue Func_02001970
void Func_02004a68(void);
void Func_02004ac0(s32, s32);
s32 Func_02004a56(s32);
s32 Func_02004a60(s32);
void Func_02004a64(s32, s32);
void Func_02004a6e(s32, s32);
void Func_02004aa2(void);

void Dialogue_RunFlag982Or983Dialogue(void)
{
    Func_02004a68();
    Func_02004ac0(0, 1);
    if (Func_02004a56(0x982) != 0 || Func_02004a60(0x983) != 0) {
        Func_02004a64(0x268A, 1);
    } else {
        Func_02004a6e(0x2689, 1);
    }
    Func_02004aa2();
}
#undef Dialogue_RunFlag982Or983Dialogue

/* overlays/scene/actor/map_init/scene_primary_script.c */
#define EffectRuntime_SetCurrentPosition_1(a0, a1, a2) Call3(Func_020051a2, a0, a1, a2)
#define EffectRuntime_SetCurrentPosition_2(a0, a1, a2) Call3(Func_020051ac, a0, a1, a2)
#define Motion_SetHPosTerrain_1(a0, a1, a2) Call3(Func_02005162, a0, a1, a2)
#define Motion_SetHPosTerrain_2(a0, a1, a2) Call3(Func_02005170, a0, a1, a2)
#define EffectRuntime_SetCurrentPosition_1_02002040(a0, a1, a2) Call3(Func_0200520a, a0, a1, a2)
#define EffectRuntime_SetCurrentPosition_2_02002040(a0, a1, a2) Call3(Func_02005218, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1_02002040(a0, a1, a2) Call3(Func_020051ca, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2_02002040(a0, a1, a2) Call3(Func_020051d4, a0, a1, a2)
#define Scene_RunStepWith6 Func_020013a4
#define Scene_RunSupplementalSequenceTwo Func_02001550
#define Scene_RunSupplementalSequenceOne Func_02001744
#define Scene_SetActor19TableB3B8 Func_02001910
#define Scene_RunMiddleSequence Func_02001d04
#define Scene_RunLateSequenceHead Func_02001fdc
#define Scene_RunLateSequenceSecond Func_02002040
#define Scene_RunScene3c4SequenceA Func_020020a4
#define Scene_RunScriptedStep953 Func_02002354
#define FieldScene_RunScene3c4_02002480 Func_02002480

extern u8 Data_03001ebc[];

void Func_02004502(s32, s32, s32);
s32 Func_02001f74();
void Func_020020de();
void Func_020045cc();
s32 Func_020045ee();
s32 Func_02004600();
void Func_0200464e();
void Func_0200465c();
s32 Func_0200466c();
void Func_0200469e();
s32 Func_020046a0();
void Func_020046aa();
u8 *Func_020046ba();
void Func_020046e0();
void Func_020046f8();
void Func_02004700();
u8 *Func_02004726();
void Func_02004730();
void Func_02004736();
void Func_02004744();
void Func_0200474c();
void Func_0200476e();
void Func_02004770();
void Func_0200477a();
void Func_0200477c();
void Func_02004788();
void Func_0200478e();
void Func_020047ac();
void Func_020047b6();
void Func_020047d0();
void Func_02004804();
void Func_02004840();
s32 Func_02002172();
void Func_020022dc();
void Func_020047ca();
s32 Func_020047ec();
s32 Func_020047fe();
s32 Func_02004838();
void Func_02004842();
void Func_0200485c();
s32 Func_02004860();
void Func_0200489c();
s32 Func_0200489e();
void Func_020048a8();
u8 *Func_020048b8();
void Func_020048de();
void Func_020048f6();
void Func_020048fe();
u8 *Func_02004924();
void Func_0200492e();
void Func_02004934();
void Func_02004942();
void Func_0200494a();
void Func_0200496c();
void Func_0200496e();
void Func_02004978();
void Func_0200497a();
void Func_02004986();
void Func_0200498c();
void Func_020049aa();
void Func_020049b4();
void Func_020049ce();
void Func_02004a02();
void Func_02004a3e();
s32 Func_02004a34(s32, s32);
void Func_02002d82();
void Func_020039d4();
void Func_02003aee();
void Func_02004e0a();
s32 Func_02004e10();
s32 Func_02004e30();
void Func_02004e36();
void Func_02004e52();
s32 Func_02004e6e();
s32 Func_02004e7a();
s32 Func_02004e86();
void Func_02004e90();
s32 Func_02004e92_a();
s32 Func_02004e9e();
s32 Func_02004ea8();
s32 Func_02004eb0();
s32 Func_02004eba();
s32 Func_02004ec2();
s32 Func_02004ecc();
s32 Func_02004ed0();
s32 Func_02004ed8();
s32 Func_02004ee4();
s32 Func_02004ef4();
s32 Func_02004f16();
s32 Func_02004f1c();
s32 Func_02004f1e();
s32 Func_02004f26();
s32 Func_02004f32();
s32 Func_02004f3c();
s32 Func_02004f6a();
s32 Func_02004f74();
s32 Func_02004f7e();
s32 Func_02004f8a();
s32 Func_02004f96();
s32 Func_02004fd4();
s32 Func_02004fe2();
s32 Func_02004ff0();
s32 Func_02004ffe();
s32 Func_0200500c();
void Func_0200500e();
s32 Func_02005010();
s32 Func_0200501a();
s32 Func_0200502a();
s32 Func_0200502c();
void Func_02005034();
void Func_0200503e();
void Func_02005040();
void Func_0200508e();
void Func_020050a0();
void Func_02005162();
void Func_02005170();
void Func_020051a2();
void Func_020051ac();
void Func_020050f2();
void Func_02005104();
void Func_020051ca();
void Func_020051d4();
void Func_0200520a();
void Func_02005218();
void Func_0200515a();
void Func_0200517e();
void Func_0200519e();
void Func_020051a2_a();
void Func_020051c6();
s32 Func_020051d0();
s32 Func_020051d8();
void Func_020051ea();
s32 Func_020051f4();
s32 Func_020051fc();
void Func_0200520e();
s32 Func_02005218_a();
s32 Func_02005220();
s32 Func_0200523c();
s32 Func_02005244();
s32 Func_02005260();
s32 Func_02005268();
s32 Func_02005284();
s32 Func_02005294();
s32 Func_020052a4();
s32 Func_020052b2();
s32 Func_020052c2();
s32 Func_020052d0();
s32 Func_020052e0();
void Func_020052e2();
s32 Func_020052e8();
s32 Func_020052ee();
s32 Func_020052fe();
s32 Func_0200530c();
void Func_0200530e();
void Func_02005310();
void Func_02005318();
void Func_0200532a();
void Func_02005334();
void Func_02005348();
s32 Func_02005354();
void Func_02005364();
void Func_02005374();
void Func_02005376();
void Func_02005398();
void Func_0200539a();
void Func_020053a0();
void Func_020053ba();
void Func_020053c6();
void Func_020053ca();
void Func_020053de();
void Func_02005400();
void Func_02005406();
void Func_02005420();
void Func_0200542c();
void Func_02005466();
void Func_0200544c(void);
void Func_0200542c_a(s32, s32);
void Func_02005460(void);
void Func_02005532();
void Func_02005544();
void Func_02005574();
void Func_02005598();
s32 Func_020055ba();
void Func_020055bc();
s32 Func_020055c6();
s32 Func_020055ce();
s32 Func_020055ea();
s32 Func_020055f2();
s32 Func_0200560e();
s32 Func_02005616();

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

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

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call8(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7)
{
    f(a0, a1, a2, a3, a4, a5, a6, a7);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_02001d04(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_020020a4(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_02002480(s32 (*f)(), s32 a0)
{
    return f(a0);
}

void Scene_RunStepWith6(void)
{
    Func_02004502(0, 6, 0);
}

void Scene_RunSupplementalSequenceTwo(void)
{
    u32 i;
    s32 rec2;
    s32 rec7;
    u8 *record;
    s32 value;
    s32 v5;
    s32 v6;
    u8 slot16[40];
    u8 *frame;

    Func_0200464e();
    record = Value1(Func_0200466c, 18);
    if ((*(volatile s32 *)((s32)record + 8) >> 20) != 46) {
    } else {
        Func_0200465c(30);
        rec2 = Func_02001f74(0x2e80000, 0, 0xb80000, 253);
        frame = slot16;
        *(s32 *)(frame + 8) = 0x9999;
        *(s32 *)(frame + 12) = 0x9999;
        *(s32 *)(frame + 4) = 7;
        *(u8 *)(Func_020046a0(18) + 85) = 0;
        Func_0200474c(185);
        for (i = 0; i < 16; i++) {
            Func_020045cc(3);
            record = Func_020046ba(18);
            *(volatile s32 *)((s32)record + 12) += -0x10000;
            rec7 = Value0(Func_020045ee);
            rec7 = ((((u32)(rec7 << 4) >> 16) << 16) + 0x2e00000);
            value = Value0(Func_02004600);
            Func_020020de(rec7, 0, ((((u32)(((value << 3) + value) << 1) >> 16) << 16) + 0x800000), 0, 0, 0, 0x90000, frame);
        }
        Call6(Func_020046aa, 51, 8, 1, 1, 49, 8);
        Func_02004700(30);
        {
            u8 *record = Func_02004726(18);
            u8 value = *(volatile u8 *)&record[35];

            record[35] = (u8)(value | 2);
        }
        Func_02004770(18, 3);
        Func_0200469e(rec2);
        Call6(Func_020046e0, 45, 4, 1, 1, 46, 8);
        Call3(Func_0200478e, 20, 0x2e80000, 0x880000);
        v5 = 1;
        v6 = 3;
        Func_02004804(188);
        Call6(Func_020046f8, 58, 8, 49, 8, v5, v6);
        Call3(Func_02004736, 0, 0x50000, 0x10000);
        Call3(Func_02004744, -1, -1, 0xe666);
        Func_0200477a(20);
        Func_02004840(188);
        Call6(Func_02004730, 59, 8, 49, 8, v5, v6);
        Call3(Func_0200476e, 0, 0x50000, 0x10000);
        Call3(Func_0200477c, -1, -1, 0xe666);
        Func_02004788();
        Func_020047b6(10);
        Call1(Func_020047ac, 0x971);
    }
    Func_020047d0();
}

void Scene_RunSupplementalSequenceOne(s32 a0)
{
    u32 i;
    s32 rec2;
    s32 rec7;
    u8 *record;
    s32 value;
    s32 arg0;
    s32 arg2;
    s32 v5;
    s32 v6;
    u8 slot16[40];
    u8 *slot;

    Func_02004842(a0);
    record = Value1(Func_02004860, 19);
    if ((*(volatile s32 *)((s32)record + 8) >> 20) != 48) {
    } else {
        if (Value1(Func_02004838, 0x202) == 0) {
        } else {
            Func_0200485c(30);
            rec2 = Func_02002172(0x3020000, 0, 0x1120000, 223);
            slot = slot16;
            *(s32 *)(slot + 8) = 0x9999;
            *(s32 *)(slot + 12) = 0x9999;
            *(s32 *)(slot + 4) = 7;
            *(u8 *)(Func_0200489e(19) + 85) = 0;
            Func_0200494a(185);
            for (i = 0; i < 16; i++) {
                Func_020047ca(3);
                record = Func_020048b8(19);
                *(volatile s32 *)((s32)record + 12) += -0x10000;
                rec7 = Value0(Func_020047ec);
                arg0 = ((((u32)(rec7 << 4) >> 16) << 16) + 0x3000000);
                value = Value0(Func_020047fe);
                arg2 = ((((u32)(((value << 3) + value) << 1) >> 16) << 16) + 0xe00000);
                Func_020022dc(arg0, 0, arg2, 0, 0, 0, 0x90000, slot);
            }
            Call6(Func_020048a8, 51, 8, 1, 1, 45, 14);
            Func_020048fe(30);
            {
                u8 *record = Func_02004924(19);
                u8 value = *(volatile u8 *)&record[35];

                record[35] = (u8)(value | 2);
            }
            Func_0200496e(19, 3);
            Func_0200489c(rec2);
            Call6(Func_020048de, 45, 4, 1, 1, 48, 14);
            Call3(Func_0200498c, 21, 0x3080000, 0xe80000);
            v5 = 1;
            v6 = 3;
            Func_02004a02(188);
            Call6(Func_020048f6, 58, 8, 45, 14, v5, v6);
            Call3(Func_02004934, 0, 0x50000, 0x10000);
            Call3(Func_02004942, -1, -1, 0xe666);
            Func_02004978(20);
            Func_02004a3e(188);
            Call6(Func_0200492e, 59, 8, 45, 14, v5, v6);
            Call3(Func_0200496c, 0, 0x50000, 0x10000);
            Call3(Func_0200497a, -1, -1, 0xe666);
            Func_02004986();
            Func_020049b4(10);
            Call1(Func_020049aa, 0x972);
        }
    }
    Func_020049ce();
}

void Scene_SetActor19TableB3B8(void)
{
    Func_02004a34(19, 0x0200B3B8);
}

void Scene_RunMiddleSequence(void)
{
    s32 saved[28];
    s32 frame;
    s32 outer;
    s32 base;
    s32 zero;
    s32 found;
    s32 inner;
    s32 slot;
    s32 rec;
    s32 tmp;
    s32 dst;
    s32 pa;
    s32 pb;
    s32 v0;
    s32 va;
    s32 v4;
    s32 vb;
    s32 v3;

    Func_02004e0a();
    outer = 0;
    frame = (s32)saved;
    zero = outer;
    base = 12;
    L_02001d20:;
    rec = Value1_02001d04(Func_02004e30, base);
    if ((12 & *(u8 *)(*(volatile s32 *)(rec + 80) + 9)) == 12) {
        slot = outer + 0x200;
        if (Value1_02001d04(Func_02004e10, slot) == 0) {
            Func_02004e52(base);
            Func_02002d82();
            Func_02004e90(base, 0, 0);
            Func_02004e36(slot);
            goto L_02001f40;
        }
    }
    goto L_02001de4;
    L_02001d5e:;
    rec = Value1_02001d04(Func_02004e6e, base);
    *(s32 *)(frame + 8) = *(volatile s32 *)(rec + 8);
    rec = Value1_02001d04(Func_02004e7a, base);
    *(s32 *)(frame + 12) = *(volatile s32 *)(rec + 12);
    rec = Value1_02001d04(Func_02004e86, base);
    *(s32 *)(frame + 16) = *(volatile s32 *)(rec + 16);
    dst = Func_02004e92_a(base);
    pa = inner + 12;
    rec = Value1_02001d04(Func_02004e9e, pa);
    *(volatile s32 *)(dst + 8) = *(volatile s32 *)(rec + 8);
    dst = Value1_02001d04(Func_02004ea8, base);
    rec = Value1_02001d04(Func_02004eb0, pa);
    *(volatile s32 *)(dst + 12) = *(volatile s32 *)(rec + 12);
    dst = Value1_02001d04(Func_02004eba, base);
    rec = Value1_02001d04(Func_02004ec2, pa);
    *(volatile s32 *)(dst + 16) = *(volatile s32 *)(rec + 16);
    rec = Func_02004ecc(pa);
    *(volatile s32 *)(rec + 8) = *(s32 *)(frame + 8);
    rec = Func_02004ed8(pa);
    *(volatile s32 *)(rec + 12) = *(s32 *)(frame + 12);
    found = inner;
    rec = Func_02004ee4(pa);
    *(volatile s32 *)(rec + 16) = *(s32 *)(frame + 16);
    goto L_02001e56;
    L_02001de4:;
    rec = Value1_02001d04(Func_02004ef4, base);
    if ((*(volatile s32 *)(rec + 16) >> 20) != 9) {
        goto L_02001f36;
    }
    tmp = Value1_02001d04(Func_02004ed0, outer + 0x200);
    if (tmp != 0) {
        goto L_02001f36;
    }
    rec = Value1_02001d04(Func_02004f16, base);
    *(volatile s32 *)(rec + 20) = tmp;
    rec = Value1_02001d04(Func_02004f1e, base);
    *(volatile s32 *)(rec + 40) = tmp;
    rec = Func_02004f26(base);
    *(volatile s32 *)(rec + 60) = -0x80000000;
    *(u8 *)(Func_02004f32(base) + 85) = tmp;
    *(volatile u16 *)(Func_02004f3c(base) + 100) = tmp;
    found = outer;
    for (inner = 0; inner < outer; inner++) {
        if (Value1_02001d04(Func_02004f1c, 0x200 + inner) == 0) {
            goto L_02001d5e;
        }
    }
    L_02001e56:;
    pb = found + 12;
    rec = Func_02004f6a(pb);
    *(volatile s32 *)(rec + 20) = zero;
    rec = Func_02004f74(pb);
    *(volatile s32 *)(rec + 40) = zero;
    rec = Func_02004f7e(pb);
    *(volatile s32 *)(rec + 60) = -0x80000000;
    *(u8 *)(Func_02004f8a(pb) + 85) = zero;
    *(volatile u16 *)(Func_02004f96(pb) + 100) = zero;
    Value2(Func_02005010, 0x30000, 0x6000);
    *(u8 *)(Func_0200502c() + 85) = zero;
    Call4(Func_02005034, 0xa80000, 0x80000, 0xb80000, 1);
    Func_02005040();
    Func_02003aee(pb);
    rec = Value1_02001d04(Func_02004fd4, pb);
    if ((*(volatile s32 *)(rec + 8) >> 20) == 8) {
        vb = (Func_02004fe2(10) + 100);
        *(volatile u16 *)(vb) = (*(volatile u16 *)(vb) + 1);
        v0 = (Func_02004ff0(11) + 100);
        v3 = (*(volatile u16 *)(v0)- 1);
    } else {
        va = (Func_02004ffe(10) + 100);
        v4 = (*(volatile u16 *)(va)- 1);
        *(volatile u16 *)(va) = v4;
        v0 = (Func_0200500c(11) + 100);
        v3 = (*(volatile u16 *)(v0) + 1);
    }
    *(volatile u16 *)(v0) = v3;
    rec = Func_0200501a(pb);
    *(volatile s32 *)(rec + 108) = 0x2009a99;
    Func_020039d4(40);
    *(u8 *)(Func_0200502a(pb) + 35) |= 2;
    Func_0200500e(0x200 + found);
    goto L_02001f40;
    L_02001f36:;
    outer = outer + 1;
    base = base + 1;
    if (outer <= 2) {
        goto L_02001d20;
    }
    L_02001f40:;
    Func_0200503e();
}

/* Runs six queued setup calls for this scene: two paired 6-argument calls
 * whose last two args repeat the first two (72,49 / 113,43), two 3-argument
 * id calls (100, 101) with zeroed remaining args, and two more 3-argument
 * calls (id 15, 16) passing a start/end pair where the second call's start
 * value (198<<18) equals the first call's end value. */
void Scene_RunLateSequenceHead(void)
{
    Call6(Func_0200508e, 72, 49, 1, 1, 8, 49); /* main:080091c0 */
    Call6(Func_020050a0, 113, 43, 1, 1, 49, 43); /* main:080091c0 */
    EffectRuntime_SetCurrentPosition_1(100, 0, 0);
    EffectRuntime_SetCurrentPosition_2(101, 0, 0);
    Motion_SetHPosTerrain_1(15, 8912896, 51904512); /* 136<<16, 198<<18 */
    Motion_SetHPosTerrain_2(16, 51904512, 45613056); /* 198<<18, 174<<18 */
}

/* Two six-argument calls whose first and fifth arguments repeat the same id
 * (8 and 49 respectively), followed by four three-argument calls each keyed
 * by an id with a trailing pair of values (-1, -1 or 0, 0). */
void Scene_RunLateSequenceSecond(void)
{
    Call6(Func_020050f2, 8, 113, 1, 1, 8, 49);
    Call6(Func_02005104, 49, 107, 1, 1, 49, 43);
    EffectRuntime_SetCurrentPosition_1_02002040(100, -1, -1);
    EffectRuntime_SetCurrentPosition_2_02002040(101, -1, -1);
    ObjectMotion_SetHorizontalPositionWithTerrain_1_02002040(15, 0, 0);
    ObjectMotion_SetHorizontalPositionWithTerrain_2_02002040(16, 0, 0);
}

void Scene_RunScene3c4SequenceA(s32 a0)
{
    s32 record;
    s32 v6;
    s32 p5;
    s32 q;

    v6 = 0;
    Func_0200519e(a0);
    Call6(Func_0200515a, 83, 45, 11, 8, 19, 45);
    record = Value1(Func_020051d0, 19);
    p5 = *(volatile s32 *)(record + 8);
    q = *(volatile s32 *)(Value1(Func_020051d8, 19) + 16);
    q >>= 20;
    p5 >>= 20;
    Func_0200517e(20, 56, 1, 1, p5, q);
    record = Value1(Func_020051f4, 20);
    p5 = *(volatile s32 *)(record + 8);
    q = *(volatile s32 *)(Value1(Func_020051fc, 20) + 16);
    q >>= 20;
    p5 >>= 20;
    Func_020051a2_a(20, 56, 1, 1, p5, q);
    record = Value1(Func_02005218_a, 21);
    p5 = *(volatile s32 *)(record + 8);
    q = *(volatile s32 *)(Value1(Func_02005220, 21) + 16);
    q >>= 20;
    p5 >>= 20;
    Func_020051c6(20, 56, 1, 1, p5, q);
    record = Value1(Func_0200523c, 22);
    p5 = *(volatile s32 *)(record + 8);
    q = *(volatile s32 *)(Value1(Func_02005244, 22) + 16);
    q >>= 20;
    p5 >>= 20;
    Func_020051ea(20, 56, 1, 1, p5, q);
    record = Value1(Func_02005260, 23);
    p5 = *(volatile s32 *)(record + 8);
    q = *(volatile s32 *)(Value1(Func_02005268, 23) + 16);
    q >>= 20;
    p5 >>= 20;
    Func_0200520e(20, 56, 1, 1, p5, q);
    record = Value1(Func_02005284, 19);
    if ((*(volatile s32 *)(record + 8) >> 20) == 25) {
        record = Value1(Func_02005294, 19);
        if ((*(volatile s32 *)(record + 16) >> 20) == 49) {
            v6 = 1;
        }
    }
    record = Value1(Func_020052a4, 20);
    if ((*(volatile s32 *)(record + 8) >> 20) == 23) {
        record = Value1(Func_020052b2, 20);
        if ((*(volatile s32 *)(record + 16) >> 20) == 49) {
            v6 = (v6 + 1);
        }
    }
    record = Value1(Func_020052c2, 21);
    if ((*(volatile s32 *)(record + 8) >> 20) == 25) {
        record = Value1(Func_020052d0, 21);
        if ((*(volatile s32 *)(record + 16) >> 20) == 47) {
            v6 = (v6 + 1);
        }
    }
    record = Value1(Func_020052e0, 22);
    if ((*(volatile s32 *)(record + 8) >> 20) == 23) {
        record = Value1(Func_020052ee, 22);
        if ((*(volatile s32 *)(record + 16) >> 20) == 47) {
            v6 = (v6 + 1);
        }
    }
    record = Value1(Func_020052fe, 23);
    if ((*(volatile s32 *)(record + 8) >> 20) == 24) {
        record = Value1(Func_0200530c, 23);
        if ((*(volatile s32 *)(record + 16) >> 20) == 48) {
            v6 = (v6 + 1);
        }
    }
    if (v6 == 5) {
        if (Value1(Func_020052e8, 0x984) != 0) {
            Func_02005318();
            goto L_020022e4;
        }
        Func_02005310(20);
        Call2(Func_020053a0, 0xcccc, 0x1999);
        Call4(Func_020053ba, 0x1d80000, -1, 0x30c0000, 1);
        Func_020053c6();
        Func_02005334(30);
        Call1_020020a4(Func_0200532a, 0x984);
        Func_02005400(158);
        Call3(Func_020052e2, 0x200b3ec, 32, 46);
        Call6(Func_0200530e, 24, 60, 1, 1, 32, 47);
        Func_02005364(40);
    } else {
        if (Value1(Func_02005354, 0x984) != 0) {
            Func_02005376(20);
            Call2(Func_02005406, 0xcccc, 0x1999);
            Call4(Func_02005420, 0x1d80000, -1, 0x30c0000, 1);
            Func_0200542c();
            Func_0200539a(30);
            Call1_020020a4(Func_02005398, 0x984);
            Func_02005466(159);
            Call3(Func_02005348, 0x200b40c, 32, 46);
            Call6(Func_02005374, 31, 47, 1, 1, 32, 47);
            Func_020053ca(40);
        }
    }
    Func_020053de();
    L_020022e4:;
}

void Scene_RunScriptedStep953(void)
{
    Func_0200544c();
    Func_0200542c_a(0x953, 1);
    Func_02005460();
}

void FieldScene_RunScene3c4_02002480(void)
{
    s32 record;
    s32 p5;

    Call6(Func_02005532, 89, 49, 3, 2, 25, 49);
    Call6(Func_02005544, 89, 51, 8, 5, 25, 51);
    *(u8 *)(Func_020055ba(14) + 34) = 1;
    record = Value1_02002480(Func_020055c6, 12);
    p5 = *(volatile s32 *)(record + 8);
    record = Value1_02002480(Func_020055ce, 12);
    p5 = p5 >> 20;
    Call6(Func_02005574, 22, 52, 1, 1, p5, (*(volatile s32 *)(record + 16) >> 20));
    record = Value1_02002480(Func_020055ea, 13);
    p5 = *(volatile s32 *)(record + 8);
    record = Value1_02002480(Func_020055f2, 13);
    p5 = p5 >> 20;
    Call6(Func_02005598, 22, 52, 1, 1, p5, (*(volatile s32 *)(record + 16) >> 20));
    record = Value1_02002480(Func_0200560e, 14);
    p5 = *(volatile s32 *)(record + 8);
    record = Value1_02002480(Func_02005616, 14);
    p5 = p5 >> 20;
    Call6(Func_020055bc, 22, 52, 1, 1, p5, (*(volatile s32 *)(record + 16) >> 20));
}
#undef EffectRuntime_SetCurrentPosition_1
#undef EffectRuntime_SetCurrentPosition_2
#undef Motion_SetHPosTerrain_1
#undef Motion_SetHPosTerrain_2
#undef EffectRuntime_SetCurrentPosition_1_02002040
#undef EffectRuntime_SetCurrentPosition_2_02002040
#undef ObjectMotion_SetHorizontalPositionWithTerrain_1_02002040
#undef ObjectMotion_SetHorizontalPositionWithTerrain_2_02002040
#undef Scene_RunStepWith6
#undef Scene_RunSupplementalSequenceTwo
#undef Scene_RunSupplementalSequenceOne
#undef Scene_SetActor19TableB3B8
#undef Scene_RunMiddleSequence
#undef Scene_RunLateSequenceHead
#undef Scene_RunLateSequenceSecond
#undef Scene_RunScene3c4SequenceA
#undef Scene_RunScriptedStep953
#undef FieldScene_RunScene3c4_02002480

/* overlays/scene/actor/map_init/scene_setup.c */
#define Scene_RunFourCallSequenceB Func_020016f4
#define Scene_RunFourStepSequenceA Func_020018f8
#define Scene_RunTwoStepSequence Func_020019bc
#define Scene_RunFourStepSequenceB Func_020019cc
#define Actor_InstallSlotNineHandler Func_020019e4
#define Actor_SetupSlotNineAndInstallHandler Func_02001a10
#define Scene_RunThreeStepSequence Func_02001f5c
#define Scene_RunFourCallSequence Func_02001fc4
#define Scene_PlaceAndPinSlots8And9 Func_020023a0
typedef struct Slot_02001a10 {
    s32 pad0;
    s32 pad4;
    s32 col;            /* +8,  12.20 fixed point */
    s32 pad12;
    s32 row;               /* +16, 12.20 fixed point */
} Slot_02001a10;

typedef struct Slot_020023a0 {
    s32 pad0;
    s32 pad4;
    s32 column;            /* +8,  12.20 fixed point */
    s32 pad12;
    s32 row;               /* +16, 12.20 fixed point */
} Slot_020023a0;

void Func_020047ec_a(void);
void Func_020017c0(void);
void Func_020047fc(void);
void Func_02002c54(void);
void Func_020049f0(void);
void Func_020019c4(void);
void Func_02004a00(void);
void Func_0200304c(void);
void Func_02004ab4(void);
void Func_02004ac0_a(void);
void Func_02004ac4(void);
void Func_02001a98(void);
void Func_02004ad4(void);
void Func_02003398(void);
s32 Func_02004b08(s32, s32);
void Func_02004ace(s32);
u8 *Func_02004b04(s32);
void Func_02004b0a();
void Func_02004b8a_a();
void Func_02004b8a_b();
void Func_02004b6a();
void Func_02004b7a();
u8 *Func_02004b48();
void Func_02004b2a();
Slot_02001a10 *Func_02004b60();
Slot_02001a10 *Func_02004b68();
void Func_02004b0e();
u8 *Func_02004b84_a();
u8 *Func_02004b84_b();
u8 *Func_02004b8e();
s32 Func_02008ec8(struct Record_02000ec8 *record);
typedef s32(*Handler_02001a10)(struct Record_02000ec8 *record);
void Func_02005054(void);
void Func_02002028(void);
void Func_02005064(void);
void Func_020050bc(void);
void Func_02002090(void);
void Func_02003f40(void);
void Func_020050d0(void);
void Func_02005452();
void Func_020032ce();
Slot_020023a0 *Func_020054d0();
Slot_020023a0 *Func_020054d8();
void Func_0200547e();
Slot_020023a0 *Func_020054f4();
Slot_020023a0 *Func_020054fc();
void Func_020054a2();

/*
 * Slot nine setup for overlay resource_3c4.
 */

/*
 * Every call site branches through its own veneer, so the callees are named
 * per site. Func_02004b84 serves two sites with different argument shapes
 * and is declared without a prototype.
 */

/*
 * Setup step in resource_3c4. One six-argument placement, then two identical
 * follow-ups that pin an overlay at the 12.20 grid cell of slots 8 and 9: the
 * slot's +8 and +16 words are each shifted right by 20 and passed as the two
 * stack arguments of the placement call.
 *
 * Several call sites reach the same routine, but each keeps its own call
 * word; the sites must not be collapsed onto one alias.
 */

struct Record_02000ec8;

void Scene_RunFourCallSequenceB(void)
{
    Func_020047ec_a();
    Func_020017c0();
    Func_020047fc();
    Func_02002c54();
}

void Scene_RunFourStepSequenceA(void)
{
    Func_020049f0();
    Func_020019c4();
    Func_02004a00();
    Func_0200304c();
}

void Scene_RunTwoStepSequence(void)
{
    Func_02004ab4();
    Func_02004ac0_a();
}

void Scene_RunFourStepSequenceB(void)
{
    Func_02004ac4();
    Func_02001a98();
    Func_02004ad4();
    Func_02003398();
}

void Actor_InstallSlotNineHandler(void)
{
    u8 *owner;

    Func_02004b08(8, 0x0200B3B8);
    Func_02004ace(0x203);
    owner = Func_02004b04(9);
    *(s32 *)(owner + 108) = 0x02008FE9;
}

/*
 * Brings slot 9 up: four state writes, clear bit 1 of the byte at +35,
 * publish selector 0x204, pin an overlay at slot 9's 12.20 grid cell, then
 * install one handler on slots 9 and 8. The 136-byte owner at 0x02001a10
 * includes its alignment halfword and its one pool word; that word is an
 * odd Thumb pointer, so the handler is Func_02008ec8. The bit-clear folds
 * +35 into the returned pointer through the address local, not into a copy.
 */
void Actor_SetupSlotNineAndInstallHandler(void)
{
    u8 *desc;
    s32 col;
    s32 row;

    Func_02004b0a();
    Func_02004b8a_a(9, 1);
    Func_02004b6a(9, 1);
    Func_02004b8a_b(9, 0);
    Func_02004b7a(9, 2);

    {
        u8 *flag = Func_02004b48(9) + 35;
        *flag &= (u8)0xfd;
    }

    Func_02004b2a(0x204);

    col = Func_02004b60(9)->col;
    row = Func_02004b68(9)->row >> 20;
    Func_02004b0e(26, 8, 1, 1, col >> 20, row);

    desc = Func_02004b84_a(9);
    *(Handler_02001a10 *)(desc + 108) = Func_02008ec8;

    desc = Func_02004b8e(8);
    *(Handler_02001a10 *)(desc + 108) = Func_02008ec8;

    Func_02004b84_b(desc);
}

void Scene_RunThreeStepSequence(void)
{
    Func_02005054();
    Func_02002028();
    Func_02005064();
}

void Scene_RunFourCallSequence(void)
{
    Func_020050bc();
    Func_02002090();
    Func_02003f40();
    Func_020050d0();
}

void Scene_PlaceAndPinSlots8And9(void)
{
    s32 row;

    {
        s32 p5 = 9, p6 = 38;
        Func_02005452(73, 38, 5, 5, p5, p6);
    }
    Func_020032ce(9, 8);

    {
        s32 col = Func_020054d0(8)->column >> 20;
        row = Func_020054d8(8)->row >> 20;
        Func_0200547e(2, 36, 1, 1, col, row);
    }

    {
        s32 col = Func_020054f4(9)->column >> 20;
        row = Func_020054fc(9)->row >> 20;
        Func_020054a2(2, 36, 1, 1, col, row);
    }
}
#undef Scene_RunFourCallSequenceB
#undef Scene_RunFourStepSequenceA
#undef Scene_RunTwoStepSequence
#undef Scene_RunFourStepSequenceB
#undef Actor_InstallSlotNineHandler
#undef Actor_SetupSlotNineAndInstallHandler
#undef Scene_RunThreeStepSequence
#undef Scene_RunFourCallSequence
#undef Scene_PlaceAndPinSlots8And9

/* overlays/scene/actor/map_init/staged_actor.c */
#define TestActorPosition Func_02003600
#define ActorProbeOffsets Data_0200b248
#define ActorSearchStep Data_0200b1f0
#define StagedActor_FindClearPosition Func_02000474


s32 StagedActor_FindClearPosition(s32 *a)
{
}
#undef TestActorPosition
#undef ActorProbeOffsets
#undef ActorSearchStep
#undef StagedActor_FindClearPosition

/* overlays/scene/actor/map_init/staged_actor_navigation.c */
#define Actor_CheckAheadTileAndStop Func_020002a8
typedef struct { s32 unk0; s32 unk4; s32 unk8; } Desc;

typedef struct { u8 filler0[0x28]; s16 *unk28; } Sub;

typedef struct {
    u8 filler0[6]; u16 unk6; s32 unk8; s32 unkC; s32 unk10;
    u8 filler14[0x10]; s32 unk24; u8 filler28[4]; s32 unk2C;
    u8 filler30[8]; s32 unk38; u8 filler3C[4]; s32 unk40;
    u8 filler44[0xC]; Sub *unk50;
} Ent;

extern s32 Data_0200b1f0[];
extern s32 Data_0200b230[];

Ent *Func_02000342(Desc *, Ent *);
s32 Func_020033c6(Ent *, Desc *);

s32 Actor_CheckAheadTileAndStop(Ent *obj)
{
    Desc d;
    u32 idx;
    s32 m;
    Ent *hit;

    idx = obj->unk6 >> 12;
    m = Data_0200b1f0[idx];
    d.unk0 = obj->unk8 + (m & 0xffff0000);
    d.unk4 = obj->unkC;
    m = m << 16;
    d.unk8 = obj->unk10 + m;
    hit = Func_02000342(&d, obj);
    if (hit != 0) {
        u32 i = 0;
        s32 v = *hit->unk50->unk28;
        s32 *p = Data_0200b230;

        do {
            if (v == *p++) {
                goto done;
            }
            i++;
        } while (i <= 5);
        obj->unk24 = 0;
        obj->unk2C = 0;
        obj->unk38 = 0x80000000;
        obj->unk40 = 0x80000000;
    }
    m = Data_0200b1f0[idx];
    d.unk0 = obj->unk8 + (m & 0xffff0000);
    d.unk4 = obj->unkC;
    m = m << 16;
    d.unk8 = obj->unk10 + m;
    if (Func_020033c6(obj, &d) > 0) {
        obj->unk24 = 0;
        obj->unk2C = 0;
        obj->unk38 = 0x80000000;
        obj->unk40 = 0x80000000;
    }
done:
    return 0;
}
#undef Actor_CheckAheadTileAndStop

/* overlays/scene/actor/map_init/staged_actor_pair.c */
#define StagedActorStepTable Data_0200b1f0
#define GetStagedActor Func_020031e0
#define FindNextStagedActor Func_02000176
#define FindBlockingStagedActor Func_020001a2
#define FindElevatedBlockingStagedActor Func_020001ce
#define CanStartStagedActorMove Func_02003244
#define SetStagedActorMode Func_0200320c
#define SelectStagedActorSlot Func_020031e4
#define StartStagedActorEffect Func_02003372
#define StartNextStagedActorMove Func_0200324a
#define StartLeadStagedActorMove Func_0200325a
#define FinishStagedActorMove Func_02003268
#define FinishStagedActorEffect Func_02003394
#define SetStagedActorTransition Func_02003278
#define StagedActor_AdvanceActorPair Func_020000c4


extern u32 Data_0200b1f0[];

struct StagedActor *Func_020031e0(s32 arg0);
struct StagedActor *Func_02000176(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *Func_020001a2(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *Func_020001ce(s32 *arg0, struct StagedActor *arg1);
s32 Func_02003244(struct StagedActor *arg0, s32 *arg1);
void Func_0200320c(struct StagedActor *arg0, s32 arg1);
void Func_020031e4(s32 arg0);
void Func_02003372(s32 arg0);
void Func_0200324a(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_0200325a(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_02003268(struct StagedActor *arg0);
void Func_02003394(void);
void Func_02003278(struct StagedActor *arg0, s32 arg1);

void StagedActor_AdvanceActorPair(void)
{
    s32 dst[3];
    struct StagedActor *lead;
    struct StagedActor *next;
    struct StagedActor *block;
    s32 dir;
    u32 step;
    s32 rate;
    s32 zero;

    lead = GetStagedActor(0);
    dir = lead->direction_and_kind >> 12;
    step = StagedActorStepTable[dir];
    dst[0] = lead->x.value + (step & 0xffff0000);
    dst[1] = lead->y;
    step <<= 16;
    dst[2] = lead->z.value + step;
    next = FindNextStagedActor(dst, lead);
    if (next == 0) return;

    step = StagedActorStepTable[dir];
    dst[0] = next->x.value + (step & 0xffff0000);
    dst[1] = next->y;
    step <<= 16;
    dst[2] = next->z.value + step;
    block = FindBlockingStagedActor(dst, next);
    if (block != 0 && (block->collision_flags & 1) != 0) return;

    dst[0] = next->x.value;
    dst[1] = next->y + 0x100000;
    dst[2] = next->z.value;
    block = FindElevatedBlockingStagedActor(dst, next);
    if (block != 0 && (block->collision_flags & 1) != 0) return;

    next->transition_mode = 2;
    step = StagedActorStepTable[dir];
    dst[0] = next->x.value + (step & 0xffff0000);
    dst[1] = next->y;
    step <<= 16;
    dst[2] = next->z.value + step;
    if (CanStartStagedActorMove(next, dst) > 0) return;

    zero = next->transition_busy;
    if (zero != 0) return;

    SetStagedActorMode(lead, 8);
    rate = 0x3333;
    SelectStagedActorSlot(15);
    StartStagedActorEffect(185);
    next->move_rate_x = rate;
    next->move_rate_z = rate;
    StartNextStagedActorMove(next, dst[0], dst[1], dst[2]);
    lead->move_rate_x = rate;
    lead->move_rate_z = rate;
    StartLeadStagedActorMove(lead, dst[0], dst[1], dst[2]);
    FinishStagedActorMove(next);
    FinishStagedActorEffect();
    next->x.value = dst[0];
    next->z.value = dst[2];
    next->unknown_24 = zero;
    next->unknown_2c = zero;
    lead->unknown_38 = 0x80000000;
    lead->unknown_40 = 0x80000000;
    lead->unknown_24 = zero;
    lead->unknown_2c = zero;
    lead->x.value = lead->x.parts.cell << 16;
    lead->z.value = lead->z.parts.cell << 16;
    SetStagedActorTransition(lead, 1);
}
#undef StagedActorStepTable
#undef GetStagedActor
#undef FindNextStagedActor
#undef FindBlockingStagedActor
#undef FindElevatedBlockingStagedActor
#undef CanStartStagedActorMove
#undef SetStagedActorMode
#undef SelectStagedActorSlot
#undef StartStagedActorEffect
#undef StartNextStagedActorMove
#undef StartLeadStagedActorMove
#undef FinishStagedActorMove
#undef FinishStagedActorEffect
#undef SetStagedActorTransition
#undef StagedActor_AdvanceActorPair

/* overlays/scene/actor/map_init/state_update.c */
#define State_ApplyArgMode0AndReturnZero Func_02000cc0
#define State_SwapSlotPairByRank Func_02000f10
#define State_RunRect73x38Step Func_02001318
#define State_ApplyTwoRectsAndRunThree Func_020013e0
#define Actor_MirrorFlag201IntoSlot14 Func_02001458
#define State_ApplyFlag970 Func_02001500
#define State_RunUnlessActorZeroAt30_52 Func_02001530
#define State_SetValue202ThenCall Func_02001924
#define Actor_ConfigureSlot21AndSetFlag201 Func_02001938
#define State_SetSlot17And18Selectors Func_02001f70
#define State_SetValue268bInScene Func_02002330
struct Slot02000f10 {
    u8 head[8];
    s32 x;
    s32 y;
    s32 rank;
};

struct Actor {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

typedef struct Slot_02001f70 {
    s32 pad0;
    s32 pad4;
    s32 w8;                /* +8, 12.20 fixed point */
} Slot_02001f70;

void Func_02003d72(s32, s32);
struct Slot02000f10 *Func_02004022(s32);
struct Slot02000f10 *Func_0200402a(s32);
void Func_02003f68(s32);
void Func_02004412(void);
void Func_020043ce(s32, s32, s32, s32, s32, s32);
void Func_020013fa(void);
void Func_020036da(void);
void Func_0200443a(void);
void Func_020044da(void);
void Func_02004496_a(s32, s32, s32, s32, s32, s32);
void Func_020044a8(s32, s32, s32, s32, s32, s32);
void Func_020014d4(void);
void Func_02003894(void);
void Func_02004514(void);
void Func_0200453c(s32);
s32 Func_0200453a(s32);
u8 *Func_0200457c(s32);
u8 *Func_02004588(s32);
u8 *Func_02004598(s32);
u8 *Func_020045a4(s32);
void Func_020045e4(s32);
struct Actor *Func_02004642(s32);
void Func_0200233e(void);
void Func_02004a06(s32);
void Func_02003072(void);
void Func_02004a84(s32, s32);
void Func_02004aa4(s32, s32);
void Func_02004a94(s32, s32);
u8 *Func_02004a62(s32);
void Func_02004a42(s32);
void Func_02005068(void);
Slot_02001f70 *Func_02005086(s32 slot);
Slot_02001f70 *Func_020050a2(s32 slot);
void Func_02005064_a(s32 selector);
void Func_02005074(s32 selector);
void Func_02005080(s32 selector);
void Func_02005090(s32 selector);
void Func_020044dc(void);
void Func_020050b0(void);
void Func_02005428(void);
void Func_02005480(s32, s32);
void Func_02005410(s32, s32);
void Func_02005444(void);

/*
 * resource_3c4 @ 0x02001f70 (84 bytes: 72 code + alignment + two pool words).
 *
 * Publishes selector 0x974 for slot 17 and 0x975 for slot 18, choosing a
 * different publisher for each depending on whether that slot's +8 word sits
 * at 12.20 row 45 and 46 respectively.  `asrs #20` makes both tests signed.
 * Both pool words are selectors, not addresses.
 *
 * `pop {r0} ; bx r0` return: void.
 */

s32 State_ApplyArgMode0AndReturnZero(s32 no)
{
    Func_02003d72(no, 0);
    return 0;
}

void State_SwapSlotPairByRank(s32 first, s32 second)
{
    struct Slot02000f10 *a = Func_02004022(first);
    struct Slot02000f10 *b = Func_0200402a(second);

    if (a->rank <= b->rank) {
        s32 t;

        t = a->x;    a->x    = b->x;    b->x    = t;
        t = a->y;    a->y    = b->y;    b->y    = t;
        t = a->rank; a->rank = b->rank; b->rank = t;
        Func_02003f68(1);
    }
}

void State_RunRect73x38Step(void)
{
    Func_02004412();
    {
        s32 width = 9;
        s32 height = 38;

        Func_020043ce(73, 38, 5, 5, width, height);
    }
    Func_020013fa();
    Func_020036da();
    Func_0200443a();
}

void State_ApplyTwoRectsAndRunThree(void)
{
    s32 lead = 25;

    Func_020044da();
    Func_02004496_a(89, 49, 3, 2, lead, 49);
    Func_020044a8(89, 51, 8, 5, lead, 51);
    Func_020014d4();
    Func_02003894();
    Func_02004514();
}

/*
 * resource_3c4 @ 0x02001458 (84 bytes: 80 code and one pool word).
 *
 * This is the selector-reversed sibling immediately before 0x020014ac and is
 * written in that owner's proven shape.  It initializes query 0x200, tests
 * flag 0x201, then mirrors the queried state into slot 14's byte at +98 and
 * bit 3 of the byte at +89.  The zero halfword at 0x02001456 is alignment
 * after the preceding owner, not part of this one.
 *
 * Every call site bls its own veneer, so the callees are named at the decoded
 * per-site addresses.
 */
void Actor_MirrorFlag201IntoSlot14(void)
{
    u8 *flags;
    u8 value;

    Func_0200453c(0x200);
    if (Func_0200453a(0x201) != 0) {
        Func_0200457c(14)[98] = 0;
        Func_02004588(14)[89] &= (u8)0xf7;
    } else {
        Func_02004598(14)[98] = 1;
        flags = Func_020045a4(14);
        flags += 89;
        value = 8;
        value |= *flags;
        *flags = value;
    }
}

void State_ApplyFlag970(void)
{
    Func_020045e4(0x970);
}

void State_RunUnlessActorZeroAt30_52(void)
{
    struct Actor *actor = Func_02004642(0);

    if ((actor->f08 >> 20) != 30 || (actor->f10 >> 20) != 52) {
        Func_0200233e();
    }
}

void State_SetValue202ThenCall(void)
{
    Func_02004a06(0x202);
    Func_02003072();
}

void Actor_ConfigureSlot21AndSetFlag201(void)
{
    u8 *flags;

    Func_02004a84(21, 1);
    Func_02004aa4(21, 0);
    Func_02004a94(21, 2);
    flags = Func_02004a62(21) + 35;
    *flags &= 0xFD;
    Func_02004a42(0x201);
}

void State_SetSlot17And18Selectors(void)
{
    Func_02005068();

    if ((Func_02005086(17)->w8 >> 20) == 45) {
        Func_02005064_a(0x974);
    } else {
        Func_02005074(0x974);
    }

    if ((Func_020050a2(18)->w8 >> 20) == 46) {
        Func_02005080(0x975);
    } else {
        Func_02005090(0x975);
    }

    Func_020044dc();
    Func_020050b0();
}

void State_SetValue268bInScene(void)
{
    Func_02005428();
    Func_02005480(0, 1);
    Func_02005410(0x268B, 1);
    Func_02005444();
}
#undef State_ApplyArgMode0AndReturnZero
#undef State_SwapSlotPairByRank
#undef State_RunRect73x38Step
#undef State_ApplyTwoRectsAndRunThree
#undef Actor_MirrorFlag201IntoSlot14
#undef State_ApplyFlag970
#undef State_RunUnlessActorZeroAt30_52
#undef State_SetValue202ThenCall
#undef Actor_ConfigureSlot21AndSetFlag201
#undef State_SetSlot17And18Selectors
#undef State_SetValue268bInScene
